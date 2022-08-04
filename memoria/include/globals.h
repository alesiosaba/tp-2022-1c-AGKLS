#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "../include/conexiones.h"
#include "../include/globals.h"

#include "../../shared/include/utils/utils.h"
#include <commons/bitarray.h>
#include <semaphore.h>

typedef struct config_t
{
	char* log_level;
	char* puerto_escucha_kernel;
	char* puerto_escucha_CPU;
	int tam_memoria;
	int tam_pagina;
	int entradas_por_tabla;
	int retardo_memoria;
	char* algoritmo_reemplazo;
	int marcos_por_proceso;
	int retardo_swap;
	char* path_swap;
	char* ip_escucha;
} config_t;

config_t config_values;

// Nodos de las listas de entradas de paginas


// TODO: estods dos structs no se usan. Checkear.
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
    int id_proceso; // PID
    t_list* entradas_N1; // TODO: creo que quedo viejo, revisar.
    t_list* marcos_reservados; // Lista con los numeros de marcos que tiene reservados
	int posicion_puntero_clock;
    int esta_suspendido;
    sem_t suspension_completa;
	tabla_primer_nivel *tablaN1; // Lista a las entradas N1 que tiene
	int dir_tabla_n1; // Indice de la tabla N1 asignada al proceso dentro de la lista global
}proceso_en_memoria;


// Estructuras propias del modulo Memoria

t_bitarray* bitmap_marcos;
t_list* tablas_primer_nivel;
t_list* tablas_segundo_nivel;

// Esta lista nos permite averiguar en que paginas está el proceso
t_list* procesos_en_memoria;

// hilo de atencion de servidor memoria a CPU
pthread_t thr_memoriaCPU;
// hilo de atencion de servidor memoria a Kernel
pthread_t thr_memoriaKernel;
// hilo de atencion SWAP
pthread_t thr_swap;

// Sincronizacion
pthread_mutex_t mutex_tablasN1;
pthread_mutex_t mutex_tablasN2;
pthread_mutex_t mutex_procesos_en_memoria;
// ------------------

void* espacio_lectura_escritura_procesos;

#endif
