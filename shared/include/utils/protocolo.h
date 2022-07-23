#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include "utils.h"

typedef struct handshake_inicial_s{
	int tamanio_pagina;
	int cant_entradas_por_tabla;
}handshake_inicial_s;

bool send_paquete_pcb(int fd, pcb* nodo_pcb, op_code codigo_paquete);
bool recv_paquete_pcb(int fd, pcb** nodo_pcb);

bool recv_paquete_consola(int fd, pcb** nodo_pcb);

bool send_paquete_kernel(int fd, op_code codigo_paquete);

<<<<<<< HEAD
bool send_handshake_inicial(int fd);

bool send_respuesta_handshake_inicial(int fd, int tamanio_pagina, int cant_entradas_por_tabla);

struct handshake_inicial_s recv_respuesta_handshake_inicial(int fd);
=======
bool send_interrupcion(int fd);


>>>>>>> a8a90ecc9151d2b57828d496c9edc847f7e68b02

/*
bool send_debug(int fd);
*/

#endif
