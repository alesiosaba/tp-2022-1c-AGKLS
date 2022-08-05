/*
 * gestion_paginacion_mem.h
 *
 *  Created on: 23 jul. 2022
 *      Author: utnso
 */

#ifndef GESTION_PAGINACION_MEM_H_
#define GESTION_PAGINACION_MEM_H_

#define MARCO_VACIO_NO_ENCONTRADO -1
#define TOMA_ENTRADA_VICTIMA 1
#define AVANZAR_PUNTERO_CLOCK 0

#include <stdio.h>
#include "./gestion_estructuras_mem.h"
#include "../../shared/include/utils/utils.h"
#include "gestion_swap.h"
#include "globals.h"

#define ESCRITURA_MEMORIA_EXITOSA 1
int obtener_marco_vacio_de_proceso(int id);
void traer_pagina_a_memoria(int id, int dir_tabla_n1 ,entrada_tabla_N2 *e);
// Planificacion

int criterio_clock(entrada_tabla_N2 *e);
entrada_tabla_N2* obtener_entrada_n2_clock(int id, int dir_tabla_n1);

int criterio_clock_mejorado(entrada_tabla_N2 *e, int vuelta);
entrada_tabla_N2* obtener_entrada_n2_clock_mejorado(int id, int dir_tabla_n1);

int escribir_memoria(int dato, uint32_t marco, uint32_t desplazamiento);
uint32_t leer_memoria(uint32_t marco, uint32_t desplazamiento);
void liberar_marcos_de_proceso(int pid);
uint32_t direccion_fisica_completa(uint32_t marco, uint32_t desplazamiento);

void incrementar_accesos_a_swap();

#endif /* GESTION_PAGINACION_MEM_H_ */
