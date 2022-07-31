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
void solicitud_lectura(int socket_cliente, t_log *logger);
int solicitud_tabla_paginas(int id_tabla, int entrada_en_tabla);
int solicitud_marco(int pid, int id_tabla, int entrada_en_tabla);
#endif /* GESTION_SOLICITUDES_MEM_H_ */
