/*
 * disco.h
 *
 *  Created on: 25 jul. 2022
 *      Author: utnso
 */

#ifndef GESTION_SWAP_H_
#define GESTION_SWAP_H_


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <commons/collections/queue.h>
#include "../../shared/include/utils/utils.h"

typedef struct t_pedido_disco
{
    int operacion_disco;
    int argumentos[3];
    sem_t pedido_swap_listo;
} pedido_swap;

enum OPERACIONES_DISCO
{
    CREAR_ARCHIVO_SWAP,
    ESCRIBIR_ARCHIVO_SWAP,
    LEER_ARCHIVO_SWAP,
    SUSPENDER_PROCESO_SWAP,
    ELIMINAR_ARCHIVO_SWAP
};

t_queue* pedidos_disco;
pthread_mutex_t mutex_cola_pedidos_swap;
sem_t semaforo_cola_pedidos_swap;

// Funciones
void gestionar_solicitudes_swap();
//Utils
char* path_archivo_swap(int pid);
// Gestion de pedidos
pedido_swap* crear_pedido_escribir_swap(int id, int dir_marco, int num_pag);
pedido_swap* crear_pedido_leer_swap(int id, int dir_marco, int num_pag);
pedido_swap* crear_pedido_crear_archivo_swap(int id);
pedido_swap* crear_pedido_eliminar_archivo_swap(int id);
pedido_swap* crear_pedido_suspension_proceso_swap(int id, int dir_tabla_n1);
void eliminar_pedido_disco(pedido_swap *p);
// Manejo swap
void crear_archivo_swap(int pid);
void escribir_archivo_swap(int pid, int dir_pag, int num_pag);
void eliminar_archivo_swap(int pid);
void enviar_pagina_a_memoria(int pid, int dir_pag, int num_pag);
#endif /* GESTION_SWAP_H_ */
