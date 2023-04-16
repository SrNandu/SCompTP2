#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "../inc/getPrice.h"
#include "../inc/simbolos.h"

#define MAXLINES 20  // Maximas lineas que se leen de archivo price
#define MAXLEGHT 512 // Macimo numero 8de caracteres por linea
#define URL_MAX 512  // Largo maximo del URL

// Estructura que guarda el header
struct curl_slist header;

/* Funcion que obtiene el precio de un simbolo
  @param simbolo simbolo a obtener el precio
  @param combre Referencia al String donde se guardara el nombre del simbolo
  @return el precio en formato double
*/
double getPrice(int simbolo, char *nombre_simbolo, int fiat, char *nombre_fiat)
{
  // Se escribe el nombre del simbolo y de la fiat soliciadas
  strcpy(nombre_simbolo, getSymbolName(simbolo));
  strcpy(nombre_fiat, getFiatName(fiat));

  // Se pide al servidor el precio del Simbolo
  serverRequest(simbolo, fiat);

  return formatPrice();
}

/* Funcion que pide al servidor el valor de un simbolo
  @param simbolo simbolo a obtener su precio
*/
void serverRequest(int simbolo, int fiat)
{
  // Se crea el objeto curl
  CURL *curl;
  CURLcode res;

  // Se llena la estructura header
  header.data = "X-CoinAPI-Key: 8F6869F0-403C-410F-8365-4BAE173CC508";
  header.next = NULL;
  // key de respaldo CD7B2B0E-4577-4035-A59C-54F68F576A4F

  // Si la carpeta tmp no existe crearla
  struct stat st = {0};
  if (stat("./tmp", &st) == -1)
  {
    mkdir("./tmp", 0700);
  }

  // Se abre el archivo
  FILE *f = fopen("./tmp/price", "w");

  // Se inicia curl
  curl = curl_easy_init();

  if (curl)
  {
    // Se genera el url para el simbolo obtenido
    char url[URL_MAX] = "https://rest.coinapi.io/v1/exchangerate/";
    strcat(url, getSymbolName(simbolo));
    strcat(url, "/");
    strcat(url, getFiatName(fiat));

    // Se configura curl
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0); // Poner este bit en 1 para que la libreria exprese los comandos
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, &header);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);

    // Se realiza una peticion
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    // Se realiza cleanup de la libreria
    curl_easy_cleanup(curl);
  }

  // Se cierra el archivo
  fclose(f);
}

/* Funcion que formatea el valor del precio a un valor double
  @return valor del simbolo en formato double
*/
double formatPrice()
{
  // Se abre el archivo
  FILE *f = fopen("./tmp/price", "r");
  // Variables que sirven para recorrer el archivo
  int i = 0;
  char saver[MAXLINES][MAXLEGHT];
  char price[MAXLEGHT];
  // Recorremos el archivo buscando el precio
  while (!feof(f) && !ferror(f))
  {
    if (fgets(saver[i], MAXLEGHT, f) != NULL)
    {
      if (strstr(saver[i], "rate") != NULL)
      {
        strcpy(price, saver[i]);
        break;
      }
      i++;
    }
  }

  // Cortamos la cadena para dejar solo el precio
  strcpy(price, strtok(price, " "));
  strcpy(price, strtok(NULL, " "));

  // Se cierra el archivo
  fclose(f);

  // Devolvemos el precio en formato double
  return atof(price);
}