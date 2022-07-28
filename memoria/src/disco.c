#include "../include/disco.h"
#include "../include/init.h"

void gestionar_solicitudes_swap(){
	// Inicializamos lista de pedidos
    pedidos_disco = queue_create();
    // Inicializamos semaforo sincronizados lista de pedidos
    sem_init(&semaforo_cola_pedidos_swap, 0, 0);

    while(1){
       sem_wait(&semaforo_cola_pedidos_swap);
       log_info(logger, "gestionar_solicitudes_swap - Solicitud de SWAP recibida");
       // Aplicar retardo SWAP
       usleep(config_values.retardo_swap * 1000);
       // Tomamos el proximo pedido de la cola de pedidos
       pthread_mutex_lock(&mutex_cola_pedidos_swap);
       t_pedido_disco *p = queue_pop(pedidos_disco);
       pthread_mutex_unlock(&mutex_cola_pedidos_swap);

       int pid, direccion, pag;
       switch(p->operacion_disco){
      	case CREAR_ARCHIVO_SWAP:
      		pid = p->argumentos[0];
      		crear_archivo_swap(pid);
      		break;
     	case ESCRIBIR_ARCHIVO_SWAP:
      		pid = p->argumentos[0];
      		direccion = p->argumentos[1];
      		pag = p->argumentos[2];
      		escribir_archivo_swap(pid, direccion, pag);
      		break;
       }

       sem_post(&(p->pedido_swap_listo));

    }

}


// Funciones
// Utils
char* path_archivo_swap(int pid){
	// Armamos la cadena del nombre del archivo
	// String vacio
	char *filename  = string_new();
    // Le agregamos el root pasado por config
	string_append(&filename, config_values.path_swap);
	string_append(&filename, "/");
	// Hacemos un string con el PID y lo agregamos a la ruta
	char *pidString = string_itoa(pid);
	string_append(&filename, pidString);
    // agregamos extension
	string_append(&filename, ".swap.txt");
	return filename;
}

// Gestion de pedidos
t_pedido_disco* crear_pedido_escribir_swap(int id, int dir_marco, int num_pag)
{
    t_pedido_disco *p = malloc(sizeof(t_pedido_disco));
    p->operacion_disco = ESCRIBIR_ARCHIVO_SWAP;
    p->argumentos[0] = id;
    p->argumentos[1] = dir_marco;
    p->argumentos[2] = num_pag;
    sem_init(&(p->pedido_swap_listo), 0, 0);

    pthread_mutex_lock(&mutex_cola_pedidos_swap);
    queue_push(pedidos_disco, p);
    pthread_mutex_unlock(&mutex_cola_pedidos_swap);

    sem_post(&semaforo_cola_pedidos_swap);
    return p;
}

t_pedido_disco* crear_pedido_leer_swap(int id, int dir_marco, int num_pag)
{
    t_pedido_disco *p = malloc(sizeof(t_pedido_disco));
    p->operacion_disco = LEER_ARCHIVO_SWAP;
    p->argumentos[0] = id;
    p->argumentos[1] = dir_marco;
    p->argumentos[2] = num_pag;
    sem_init(&(p->pedido_swap_listo), 0, 0);

    pthread_mutex_lock(&mutex_cola_pedidos_swap);
    queue_push(pedidos_disco, p);
    pthread_mutex_unlock(&mutex_cola_pedidos_swap);

    sem_post(&semaforo_cola_pedidos_swap);
    return p;
}

void eliminar_pedido_disco(t_pedido_disco *p)
{
    // Ver de liberar los argumentos individualmente
    free(p);
}

t_pedido_disco* crear_pedido_crear_archivo_swap(int id)
{
    t_pedido_disco *p = malloc(sizeof(t_pedido_disco));
    p->operacion_disco = CREAR_ARCHIVO_SWAP;
    p->argumentos[0] = id;
    sem_init(&(p->pedido_swap_listo), 0,0);

    pthread_mutex_lock(&mutex_cola_pedidos_swap);
    queue_push(pedidos_disco, p);
    pthread_mutex_unlock(&mutex_cola_pedidos_swap);
    // TODO: Implementar gestion de pedidos en Disco
    sem_post(&semaforo_cola_pedidos_swap);
    return p;
}


// Manejo swap
void crear_archivo_swap(int pid){
	log_info(logger,"crear_archivo_swap: Solicitud de creacion de archivo");
	// Validacion del directorio del archivo a crear
	struct stat buffer;
	// Nos fijamos si el directorio de creacion de archivos swap existe
    if(stat(config_values.path_swap, &buffer) != 0){
    	// TODO: Revisar path. Queda generico?
        system("mkdir /home/utnso/swap");
        log_info(logger,"crear_archivo_swap: El directorio %s no existe. Se crea nuevo directorio.", config_values.path_swap);
    }


    char * filename = path_archivo_swap(pid);
    log_info(logger,"crear_archivo_swap: Se va a crear archivo: %s", filename);

    // Creacion del archivo
    FILE * archivo;
    archivo = fopen(filename,"w+");

    free(filename);
    fclose(archivo);

}

void escribir_archivo_swap(int pid, int dir_pag, int num_pag){
	char * path = path_archivo_swap(pid);
	FILE * file;
	file = fopen(path, "r+");
	fseek(file, config_values.tam_pagina * num_pag, SEEK_SET);
	fwrite(espacio_lectura_escritura_procesos + dir_pag,config_values.tam_pagina,1,file);
	fclose(file);
	free(path);
}
