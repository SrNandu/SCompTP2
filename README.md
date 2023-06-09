# **_SistCompTP2_**

#### En el TP#2 se diseñó e implementó una Calculadora de Cotización de Criptomonedas. La capa superior recupera la cotización de 7 criptomonedas obtenidas mediante la biblioteca CURL de C y la API Rest de la pagina BITSTAMP. El programa luego convoca con los datos de la consulta a rutinas en ensamblador que realizan la multiplicación de la cantidad de monedas a comprar por el precio de las mismas y devuelve la cotización solicitada.

## Modo de uso:

  #### 1. ```$~ make all ``` (para construir el proyecto).
  #### 2. ``` $~ ./bin./calculadora ``` (para ejetuctar el programa).
  #### 3. Luego, se pide ingresar por consola un número correspondiente al signo del cual se quiere saber su cotización.
  #### 4. A continuación, se pide ingresar otro número correspondiente a la fiat contra la cual se quiere expresar el valor del signo.
  #### 5. Por último, se pide ingresar por consola la cantidad de monedas a comprar. 
> Ahora el programa imprimirá la cotización solicitada 

## Depuración
Corremos la culculadora, elijiendo la opcion de BTC y USD para obtener el precio en USD del bitcoin:
![image](https://user-images.githubusercontent.com/30204958/232549865-d30845c3-6805-4ef7-b650-26266efef221.png)
Vemos como main llama a la subrutina escrita en asembler llamada mult:
![image](https://user-images.githubusercontent.com/30204958/232525142-24965656-45ee-43dc-91e5-66327c83f072.png)
Por lo que accediendo a los datos en memoria en 0xffffce38 podemos ver los datos q se encuentran en el stack:
![image](https://user-images.githubusercontent.com/30204958/232528169-6f9fd094-b28d-4d33-b8d9-0368aa56ebf5.png)
Luego observamos los valores de los parametros pasados, el parametro cant se encuentra en 0xffffce40 y precio en 0xffffce48:
![image](https://user-images.githubusercontent.com/30204958/232549760-c44999c4-775f-4412-8c43-cc799e634c7c.png)
Convirtiendo los valores de hexa a double obtenemos: cant=2 y precio=29382.729908202193.
