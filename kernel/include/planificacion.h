#ifndef STS_H_
#define STS_H_

#include "../include/init.h"


void movePCBto(pcb** new_pcb, status new_status);

void planificadorCortoPlazo();
void planificadorLargoPlazo();
void planificacion_cpu(int socket_fd);

void estimar_proxima_rafaga(pcb* pcb, double rafaga);
#endif
