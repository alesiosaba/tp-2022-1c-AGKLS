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
#include "gestion_swap.h"

// Recibe del Kernel un PID y devuelve (en el socket) el ID de la tabla N1 en la lista de procesos globales
void solicitud_nuevo_proceso(int socket_cliente);
// Recibe un ID de lista y un numero de entrada y devuelve el ID de la tabla N2 asociada
int solicitud_tabla_n1(int id_tabla_n1, int entrada_en_tabla_n1);
// Recibe un ID tabla N2 y un numero de entrada y devuelve el numero de marco
int solicitud_tabla_n2(int pid, int id_tabla_n2, int entrada_en_tabla_n2);
int solicitud_escritura_memoria(int dato, uint32_t marco, uint32_t desplazamiento);
uint32_t solicitud_lectura_memoria(uint32_t marco, uint32_t desplazamiento);
void solicitud_suspension_proceso(int socket_cliente);
void solicitud_desuspension_proceso(int socket_cliente);
void solicitud_eliminar_proceso(int socket_cliente);
#endif /* GESTION_SOLICITUDES_MEM_H_ */
