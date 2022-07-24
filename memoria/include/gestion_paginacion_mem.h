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

int dir_marco_vacio_proceso(int id);
void traer_pagina_a_memoria(int id, int dir_tablaN1 ,entrada_tabla_N2 *e);



#endif /* GESTION_PAGINACION_MEM_H_ */
