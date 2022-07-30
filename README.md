# TP SO 2022 1C - Grupo: AGKLS

## Enunciado 

https://docs.google.com/document/d/17WP76Vsi6ZrYlpYT8xOPXzLf42rQgtyKsOdVkyL5Jj0/edit

## Links de descarga de vm's

https://www.utnso.com.ar/recursos/maquinas-virtuales/

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

### una vez terminado el programa obtenemos la informacion que nos brinda valgrind

![image](https://user-images.githubusercontent.com/62452207/182001907-9839476b-03a4-4bab-92db-181ff424ceeb.png)

## parametros de valgrind 

- *definitely lost*: **muy importante no tener**
- *indirectly lost*: **muy importante no tener**
- *possibly lost*:
- *still reacheable*:
- *suppressed*:

![image](https://user-images.githubusercontent.com/62452207/182001710-08fb0ce0-3817-4df0-a1e2-373f1c228ac1.png)

