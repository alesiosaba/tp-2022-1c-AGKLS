#ifndef LISTAS_H_
#define LISTAS_H_


#include "../include/conexiones.h"

//MUTEX
pcb* dequeu_ready();

pcb* dequeu_execution();

pcb* dequeue_new();

pcb* dequeue_blocked();
pcb* dequeue_blocked_at_index(int index);

pcb* dequeue_suspendedblk();
pcb* dequeue_suspendedblk_at_index(int index);

pcb* dequeue_desbloqueo_pend();

pcb* dequeue_suspended_ready();

bool enqueue_proceso(pcb** pcb);

bool enqueue_desbloqueo_pendiente(pcb** pcb);

bool menor_rafaga(pcb *pcb1, pcb *pcb2);

#endif
