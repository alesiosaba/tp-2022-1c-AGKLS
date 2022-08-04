#include "../include/gestion_swap.h"
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
       pedido_swap *p = queue_pop(pedidos_disco);
       pthread_mutex_unlock(&mutex_cola_pedidos_swap);
       log_warning(logger, "******* ACCESO A SWAP *******");
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
      		log_debug(logger,"gestionar_solicitudes_swap: solicitud ESCRIBIR_ARCHIVO_SWAP recibida: PID %d Direccion: %d, num_pag: %d", pid, direccion, pag);
      		escribir_archivo_swap(pid, direccion, pag);
      		log_debug(logger,"gestionar_solicitudes_swap: solicitud ESCRIBIR_ARCHIVO_SWAP resuelta");
      		break;
     	case ELIMINAR_ARCHIVO_SWAP:
     	    pid = p->argumentos[0];
     	    eliminar_archivo_swap(pid);
     	    break;
     	case LEER_ARCHIVO_SWAP:
     	    pid = p->argumentos[0];
     	    direccion = p->argumentos[1];
     	    pag = p->argumentos[2];
     	    enviar_pagina_a_memoria(pid, direccion, pag);
     	    break;
     	 case SUSPENDER_PROCESO_SWAP:
     	    pid = p->argumentos[0];
     	    direccion = p->argumentos[1];
     	    suspender_paginas(pid, direccion);
     	    break;

       }
 	  log_debug(logger,"gestionar_solicitudes_swap: Solicitud SWAP Resuelta");
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
pedido_swap* crear_pedido_escribir_swap(int id, int dir_marco, int num_pag)
{
    pedido_swap *p = malloc(sizeof(pedido_swap));
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

pedido_swap* crear_pedido_leer_swap(int id, int dir_marco, int num_pag)
{
    pedido_swap *p = malloc(sizeof(pedido_swap));
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

//LEER ARCHIVO SWAP
void enviar_pagina_a_memoria(int pid, int dir_pag, int num_pag){
    log_info(logger, "enviar_pagina_a_memoria: Para PID: %d se leera la pagina nro %d y se guardara en dir: %d ", pid, num_pag, dir_pag);

    char * path = path_archivo_swap(pid);

    FILE * file;

    file = fopen(path, "r");

    log_debug(logger, "enviar_pagina_a_memoria: Para PID: %d se busca archivo swap: %s", pid, path);
    if(fseek(file,config_values.tam_pagina*num_pag,SEEK_SET)){
        log_debug(logger, "No se pudo ubicar pagina en swap");
    }

    log_debug(logger, "enviar_pagina_a_memoria: Se comienza a leer el archivo swap desde: %d", config_values.tam_pagina * num_pag);


    fread(espacio_lectura_escritura_procesos + dir_pag, config_values.tam_pagina,1, file);

    log_info(logger,"enviar_pagina_a_memoria: Se envio la pagina %d a memoria ",num_pag);
    fclose(file);
    free(path);
}

pedido_swap* crear_pedido_eliminar_archivo_swap(int id)
{
	log_debug(logger,"crear_pedido_eliminar_archivo_swap - creando solicitud eliminar archivos SWAP");
    pedido_swap *p = malloc(sizeof(pedido_swap));
    p->operacion_disco = ELIMINAR_ARCHIVO_SWAP;
    p->argumentos[0] = id;
    sem_init(&(p->pedido_swap_listo), 0, 0);

    pthread_mutex_lock(&mutex_cola_pedidos_swap);
    queue_push(pedidos_disco, p);
    pthread_mutex_unlock(&mutex_cola_pedidos_swap);

    sem_post(&semaforo_cola_pedidos_swap);
	log_debug(logger,"crear_pedido_eliminar_archivo_swap - solicitud eliminar archivos SWAP terminada");

    return p;
}

void eliminar_pedido_disco(pedido_swap *p){
    // TODO: Ver de liberar los argumentos individualmente. Debe estar leakeando.
    free(p);
}

pedido_swap* crear_pedido_crear_archivo_swap(int id)
{
    pedido_swap *p = malloc(sizeof(pedido_swap));
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

pedido_swap* crear_pedido_suspension_proceso_swap(int id, int dir_tabla_n1){
	pedido_swap *p = malloc(sizeof(pedido_swap));
	p->operacion_disco = SUSPENDER_PROCESO_SWAP;
	p->argumentos[0] = id;
	p->argumentos[1] = dir_tabla_n1;
	sem_init(&(p->pedido_swap_listo), 0, 0);

	pthread_mutex_lock(&mutex_cola_pedidos_swap);
	queue_push(pedidos_disco, p);
	pthread_mutex_unlock(&mutex_cola_pedidos_swap);

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
        log_debug(logger,"crear_archivo_swap: El directorio %s no existe. Se crea nuevo directorio.", config_values.path_swap);
    }


    char * filename = path_archivo_swap(pid);
    log_debug(logger,"crear_archivo_swap: Se va a crear archivo: %s", filename);

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

void eliminar_archivo_swap(int pid){

    char* aux = path_archivo_swap(pid);

    if(remove(aux)==0){
        log_info(logger, "eliminar_archivo_swap: el archivo %s fue eliminado correctamente", aux);
    }
    else{
        log_error(logger, "eliminar_archivo_swap: el archivo no se pudo eliminar");
    }

    free(aux);
}

void suspender_paginas(int pid, int dir_tablaN1)
{
    t_list *marcos = conseguir_marcos_proceso(dir_tablaN1);
    /*
        NOTA: este valor se usa para que en la primer vuelta
        disco no haga un usleep dos veces (una por el pedido suspension
        y otra por el primer pedido de escribir pagina)
    */
    int contador = 0;
    for(int i = 0; i < list_size(marcos); i++)
    {
        entrada_tabla_N2 *e = list_get(marcos, i);
        if(e->bit_modificacion == 1)
        {
            usleep(contador * 1000);
            contador = config_values.retardo_swap;
            escribir_archivo_swap(pid, e->dir, e->num_pag);
            log_debug(logger, "suspender_paginas: se guardaron los cambios de la pagina %d correctamente del proceso %d",e->num_pag, pid);
        }
        e->bit_modificacion = 0;
        e->bit_presencia = 0;
    }
    list_destroy(marcos);
}


