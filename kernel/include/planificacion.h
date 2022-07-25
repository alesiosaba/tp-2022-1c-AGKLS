#ifndef STS_H_
#define STS_H_

#include "../include/init.h"


void movePCBto(pcb** new_pcb, status new_status);

void planificadorLargoPlazo();
void planificadorCortoPlazo();
void planificadorMedianoPlazo();

void planificacion_cpu(int socket_fd);
void planificacion_bloqueo();
void planificacion_suspended();

//UTILS
bool planificador_es_fifo();
void estimar_proxima_rafaga(pcb* pcb, double rafaga);
void print_grado_multiprogramacion();
int  proceso_esta_en_lista(t_list* lista, int id);

#endif
