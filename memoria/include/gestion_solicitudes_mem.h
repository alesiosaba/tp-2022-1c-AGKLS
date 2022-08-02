/*
 * gestion_solicitudes_mem.h
 *
 *  Created on: 23 jul. 2022
 *      Author: utnso
 */

#ifndef GESTION_SOLICITUDES_MEM_H_
#define GESTION_SOLICITUDES_MEM_H_


#include <stdio.h>
#include "./gestion_estructuras_mem.h"
#include "../../shared/include/utils/sockets.h"
#include "./disco.h"

void solicitud_nuevo_proceso(int socket_cliente);
int solicitud_tabla_paginas(int id_tabla, int entrada_en_tabla);
int solicitud_marco(int pid, int id_tabla, int entrada_en_tabla);
int solicitud_escritura_memoria(int dato, uint32_t marco, uint32_t desplazamiento);
uint32_t solicitud_lectura_memoria(uint32_t marco, uint32_t desplazamiento);
void solicitud_suspension_proceso(int socket_cliente);
void solicitud_desuspension_proceso(int socket_cliente);

#endif /* GESTION_SOLICITUDES_MEM_H_ */
