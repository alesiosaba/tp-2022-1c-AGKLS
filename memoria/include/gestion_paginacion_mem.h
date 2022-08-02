/*
 * gestion_paginacion_mem.h
 *
 *  Created on: 23 jul. 2022
 *      Author: utnso
 */

#ifndef GESTION_PAGINACION_MEM_H_
#define GESTION_PAGINACION_MEM_H_
#define PAGINA_NO_ENCONTRADA -1

#include <stdio.h>
#include "./gestion_estructuras_mem.h"
#include "../../shared/include/utils/utils.h"
#include "./disco.h"

#define ESCRITURA_MEMORIA_EXITOSA 1
int dir_marco_vacio_proceso(int id);
void traer_pagina_a_memoria(int id, int dir_tablaN1 ,entrada_tabla_N2 *e);
// Planificacion
int criterio_clock(entrada_tabla_N2 *e);
entrada_tabla_N2* aplicar_busqueda_clock(int id, int dir_tablaN1);
uint32_t direccion_fisica_completa(uint32_t marco, uint32_t desplazamiento);
int criterio_clock_mejorado(entrada_tabla_N2 *e, int vuelta);
int escribir_memoria(int dato, uint32_t marco, uint32_t desplazamiento);
uint32_t leer_memoria(uint32_t marco, uint32_t desplazamiento);


#endif /* GESTION_PAGINACION_MEM_H_ */
