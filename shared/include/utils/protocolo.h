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

bool send_handshake_inicial(int fd);

bool send_respuesta_handshake_inicial(int fd, int tamanio_pagina, int cant_entradas_por_tabla);

struct handshake_inicial_s recv_respuesta_handshake_inicial(int fd);
bool send_interrupcion(int fd);

// serializacion CPU - Memoria
bool send_solicitud_tabla_N1(int fd, int id_tablaN1, int entrada_tabla_primer_nivel, int proceso_id);
int recv_respuesta_solicitud_N1(int fd);
bool send_solicitud_tabla_N2(int fd, int id_tablaN2, int entrada_tabla_segundo_nivel, int proceso_id);
int recv_respuesta_solicitud_N2(int fd);
void recv_solicitud_tabla(int fd, consulta_en_tabla_paginas** consulta);
void send_respuesta_solicitud_tabla(int fd, int valor_solicitado, op_code cod_op);
consulta_en_tabla_paginas* deserializar_solicitud_tablas(t_list* lista);

// comunicaciones entre memoria y kernel
bool send_respuesta_nuevo_proceso(int fd, int numero_tabla);
int recv_respuesta_nuevo_proceso(int fd);
/*
bool send_debug(int fd);
*/

#endif
