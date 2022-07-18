#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include "../include/utils/gestionPCB.h"
#include "../include/utils/utils.h"


bool send_paquete_pcb(int fd, pcb* nodo_pcb);
bool recv_paquete_pcb(int fd, pcb** nodo_pcb);

bool recv_paquete_consola(int fd, pcb** nodo_pcb);

/*
bool send_debug(int fd);
*/

#endif
