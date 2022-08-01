# TP SO 2022 1C - Grupo: AGKLS

## [Enunciado - TP SO 2022 1C](https://docs.google.com/document/d/17WP76Vsi6ZrYlpYT8xOPXzLf42rQgtyKsOdVkyL5Jj0/edit)

## [Guia de deploy ](https://docs.utnso.com.ar/guias/herramientas/deploy.html#objetivo)

## [Guia de bash](https://docs.utnso.com.ar/guias/consola/bash.html#comandos-basicos)

##[ Commons](https://github.dev/sisoputnfrba/so-commons-library/tree/master/src)

## [Documento de pruebas](https://docs.google.com/document/d/1SBBTCweMCiBg6TPTt7zxdinRh4ealRasbu0bVlkty5o/edit) 

https://github.com/sisoputnfrba/kiss-pruebas

## [Links de descarga de vm's](https://www.utnso.com.ar/recursos/maquinas-virtuales/)

## VM ubuntu-server

### Configurar network adapter en la vm

- Verificar que se encuentra seleccionada la opcion "Cable Connected"

![image](https://user-images.githubusercontent.com/62452207/181865772-c5fca0cc-d5cf-45e2-a120-b103bf49cdf7.png)

### Una vez iniciada la vm comprobar la ip local asignada a ubuntu-server:

**Usar el comando** `ifconfig`

![image](https://user-images.githubusercontent.com/62452207/181865973-ed7442f3-d4de-4601-9e8f-297a115b7e61.png)

**Si no se configura una ip local automaticamente usar:**

`sudo systemctl restart networking`

### Iniciar servidor http en maquina linux 

usar comando: `python -m SimpleHTTPServer`

este comando levanta un server http en la ip que indica `ifconfig` y el puerto 8000 por defecto

verificar en que ruta se ejecuta el comando, recomendado moverse a carpeta root

![image](https://user-images.githubusercontent.com/62452207/181870733-9f381f8f-6629-4827-8845-6e74048fd245.png)

Para comprobar que esta iniciado el server podemos verlo desde un navegador: 

![image](https://user-images.githubusercontent.com/62452207/181870711-18651211-1e46-439a-8a62-66981ade3926.png)

Para descargar el archivo de deploy podemos descargarlo desde nuestro "server" utilizando `wget`

`wget 192.168.0.69:8000/home/utnso/shared_folder/deploy.sh`

![image](https://user-images.githubusercontent.com/62452207/181871505-c68fbb81-49e5-4ded-90a2-22553a38bf79.png)

# Usando Valgrind para memory leaks

- Guia

https://docs.utnso.com.ar/guias/herramientas/valgrind.html#introduccion

- Video charla

https://www.youtube.com/watch?v=knRei6OBU4Q&t=264s

![image](https://user-images.githubusercontent.com/62452207/182000819-71c6f3a8-56e6-4399-b29f-593e0310f806.png)

## viendo memory leaks en Eclipse

![image](https://user-images.githubusercontent.com/62452207/182001168-79480544-77b6-449a-96b1-90b7819cacea.png) 

## viendo memory leaks en la terminal

![image](https://user-images.githubusercontent.com/62452207/182001697-a7cebbfd-38c0-43bc-a259-76b6fc1bb61b.png)

### para ver memory leaks producidos por un programa debemos ejecutar el programa de la siguiente manera:

`valgrind ./bin/consola.out`

- Algunas opciones por parámetro copadas:

`--leak-check=yes` habilita el detector de memory leaks.	

`--log-file=<nombreArchivo>` crea un log de lo que nos muestra Valgrind por pantalla. 

`valgrind ./bin/kernel.out --log-file=/home/utnso/shared_folder/valgrind-logs/kernel-valgrind.log`

- Si no habilitamos la opción `--leak-check` y existen memory leaks en nuestro programa, Valgrind nos dejará un mensaje mágico de éste tipo: `Rerun with --leak-check=full to see details of leaked memory.`

### una vez terminado el programa obtenemos la informacion que nos brinda valgrind

![image](https://user-images.githubusercontent.com/62452207/182001907-9839476b-03a4-4bab-92db-181ff424ceeb.png)

## conceptos de valgrind 

- *definitely lost*: **muy importante no tener**
- *indirectly lost*: **muy importante no tener**
- *possibly lost*: **se puede generar, readline genera un poco, tampoco darle mucha bola**
- *still reacheable*: **no darle bola**
- *suppressed*: **no darle bola**

![image](https://user-images.githubusercontent.com/62452207/182001710-08fb0ce0-3817-4df0-a1e2-373f1c228ac1.png)

# Helgrind

## Analiza que esta pasando con los hilos

## Usando Helgrind en Eclipse

![image](https://user-images.githubusercontent.com/62452207/182002029-bdf5b64e-5b25-4c92-88e7-a85a6da81abf.png)

### cambiar tool to run:

![image](https://user-images.githubusercontent.com/62452207/182002048-3da26c85-7ea5-4e7c-922d-241a20dd357f.png)

### helgrind nos indica en que linea del programa y en que hilo exactamente hay problemas entre hilos

- RaceCondition: posible condicion de carrrera 

![image](https://user-images.githubusercontent.com/62452207/182002082-19756c83-28a0-4403-9fc3-9e694022cc64.png)

## Usando Helgrind en la terminal

`valgrind --tool=helgrind ./bin/memoria.out`

![image](https://user-images.githubusercontent.com/62452207/182002145-aca607ab-b3c5-4c66-a711-b5f1d5a1543e.png)

## conceptos de helgrind 

- **CUIDADO** que ejecutando con helgrind un programa la herramienta _**puede salvar y atajar**_ errores como segmentation fault, obviamente no se puede usar en la entrega.

- ***Thread #2 was created by 0x10924A: main(ejemplo.c:35)*** nos indica en que programa y en que linea se creo el hilo

![image](https://user-images.githubusercontent.com/62452207/182002331-ee6e374e-5151-480c-ad20-3b97f1e52f02.png)

- ***Possible data race ... Locks held: none***: significa sin tener ningun semaforo, tambien nos indica en que linea se genera el problema

![image](https://user-images.githubusercontent.com/62452207/182002319-e3310532-79f9-4549-ba3f-617e9b517aff.png)

- ***Lock at 0x483CC9C was first observed*** puede indicar un deadlock y la linea del programa que la genera

![image](https://user-images.githubusercontent.com/62452207/182002404-ec2a9916-d8ac-4e6e-88de-31add1ef5e7a.png)
