#ifndef STS_H_
#define STS_H_

#include "../include/init.h"


void movePCBto(pcb** new_pcb, status new_status);

void planificadorLargoPlazo();
void planificadorCortoPlazo();
void planificadorMedianoPlazo();

void planificacion_cpu(int socket_fd);
void planificacion_bloqueo();

//UTILS
bool planificador_es_fifo();
void estimar_proxima_rafaga(pcb* pcb, double rafaga);
pcb* dequeu_ready();
bool menor_rafaga(pcb *pcb1, pcb *pcb2);
void print_grado_multiprogramacion();
int  proceso_esta_en_lista(t_list* lista, int id);

//MUTEX
pcb* dequeue_blocked();
pcb* dequeue_blocked_at_index(int index);

#endif
