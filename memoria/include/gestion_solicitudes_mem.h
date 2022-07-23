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

void solicitud_lectura(int socket_cliente, t_log *logger);
void solicitud_tabla_paginas(int socket_cliente, t_log *logger);

#endif /* GESTION_SOLICITUDES_MEM_H_ */
