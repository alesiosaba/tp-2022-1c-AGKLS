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

int dir_marco_vacio_proceso(int id);
void traer_pagina_a_memoria(int id, int dir_tablaN1 ,entrada_tabla_N2 *e);
// Planificacion
int criterio_clock(entrada_tabla_N2 *e);
entrada_tabla_N2* aplicar_busqueda_clock(int id, int dir_tablaN1);
int criterio_clock_mejorado(entrada_tabla_N2 *e, int vuelta);



#endif /* GESTION_PAGINACION_MEM_H_ */
