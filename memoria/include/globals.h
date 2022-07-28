#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "../include/conexiones.h"
#include "../include/globals.h"

#include "../../shared/include/utils/utils.h"
#include <commons/bitarray.h>
#include <semaphore.h>

typedef struct config_t
{
	char* puerto_escucha;
	int tam_memoria;
	int tam_pagina;
	int entradas_por_tabla;
	int retardo_memoria;
	char* algoritmo_reemplazo;
	int marcos_por_proceso;
	int retardo_swap;
	char* path_swap;
} config_t;

config_t config_values;

// Nodos de las listas de entradas de paginas

typedef struct entrada_primer_nivel
{
	// La tabla de primer nivel estará compuesta por referencias a las tablas de páginas de segundo nivel
	int nroTabla;
} entrada_primer_nivel;

typedef struct entrada_segundo_nivel
{
	// La tabla de primer nivel estará compuesta por referencias a las tablas de páginas de segundo nivel
	int nroMarco;
	int bitPresencia;
	int bitUso;
	int bitModificacion;
} entrada_segundo_nivel;

// Nodos de la lista de procesos en memoria
typedef struct
{
    int id_proceso;
    t_list* entradas_N1;
    t_list* marcos_reservados;
	int posicion_puntero_clock;
    int esta_suspendido;
    sem_t suspension_completa;
	t_tablaN1 *tablaN1;
}proceso_en_memoria;


// Estructuras propias del modulo Memoria

t_bitarray* bitmap_marcos;
t_list* entradas_tabla_primer_nivel;
t_list* entradas_tabla_segundo_nivel;

// Esta lista nos permite averiguar en que paginas está el proceso
t_list* procesos_en_memoria;

// hilo de atencion a servidor memoria
pthread_t thr_memoria;
pthread_t thr_swap;

// Sincronizacion
pthread_mutex_t mutex_tablasN1;
pthread_mutex_t mutex_tablasN2;
pthread_mutex_t mutex_procesos_en_memoria;
// ------------------

void* espacio_lectura_escritura_procesos;

#endif
