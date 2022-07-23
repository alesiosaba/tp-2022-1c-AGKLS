#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include "utils.h"


bool send_paquete_pcb(int fd, pcb* nodo_pcb, op_code codigo_paquete);
bool recv_paquete_pcb(int fd, pcb** nodo_pcb);

bool recv_paquete_consola(int fd, pcb** nodo_pcb);

bool send_paquete_kernel(int fd, op_code codigo_paquete);

/*
bool send_debug(int fd);
*/

#endif
