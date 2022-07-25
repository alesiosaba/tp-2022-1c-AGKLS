/*
 * disco.h
 *
 *  Created on: 25 jul. 2022
 *      Author: utnso
 */

#ifndef DISCO_H_
#define DISCO_H_


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <commons/collections/queue.h>

typedef struct t_pedido_disco
{
    int operacion_disco;
    int argumentos[3];
    sem_t pedido_listo;
} t_pedido_disco;

enum OPERACIONES_DISCO
{
    CREAR_ARCHIVO,
    ESCRIBIR_ARCHIVO,
    LEER_ARCHIVO,
    DISCO_SUSPENDER_PROCESO,
    ELIMINAR_ARCHIVO
};

t_queue* pedidos_disco;
pthread_mutex_t mutex_cola_pedidos;
sem_t lista_tiene_pedidos;

// Funciones
t_pedido_disco* crear_pedido_escribir(int id, int dir_marco, int num_pag);
t_pedido_disco* crear_pedido_lectura(int id, int dir_marco, int num_pag);
void eliminar_pedido_disco(t_pedido_disco *p);

#endif /* DISCO_H_ */
