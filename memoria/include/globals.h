#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "../../shared/include/utils/utils.h"
#include <commons/bitarray.h>

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

// Estructuras propias del modulo Memoria

void* espacio_lectura_escritura_procesos;
t_bitarray* bitmap_marcos;
t_list* entradas_tabla_primer_nivel;
t_list* entradas_tabla_segundo_nivel;

// hilo de atencion a servidor memoria
pthread_t thr_memoria;


#endif
