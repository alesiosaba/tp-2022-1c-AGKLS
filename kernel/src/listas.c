#include "../include/listas.h"

pcb* dequeu_ready(){
	pcb* pcb;
	pthread_mutex_lock(&mtx_ready);
	if(!planificador_es_fifo()){
		list_sort(listaReady, (void*)menor_rafaga);
	}
	pcb = list_remove(listaReady,0);
	pthread_mutex_unlock(&mtx_ready);
	return pcb;
}

pcb* dequeue_blocked(){
	pthread_mutex_lock(&mtx_blocked);
	pcb* pcb = list_remove(listaBlocked, 0);
	pthread_mutex_unlock(&mtx_blocked);
	return pcb;
}

pcb* dequeue_blocked_at_index(int index){
	pthread_mutex_lock(&mtx_blocked);
	pcb* pcb = list_remove(listaBlocked, index);
	pthread_mutex_unlock(&mtx_blocked);
	return pcb;
}

pcb* dequeue_suspendedblk(){
	pthread_mutex_lock(&mtx_blocked);
	pcb* pcb = list_remove(listaSuspendedBlocked, 0);
	pthread_mutex_unlock(&mtx_blocked);
	return pcb;
}

pcb* dequeue_suspendedblk_at_index(int index){
	pthread_mutex_lock(&mtx_susblk);
	pcb* pcb = list_remove(listaSuspendedBlocked, index);
	pthread_mutex_unlock(&mtx_susblk);
	return pcb;
}

pcb* dequeue_suspended_ready(){
	pthread_mutex_lock(&mtx_susrdy);
	pcb* pcb = list_remove(listaSuspendedReady, 0);
	pthread_mutex_unlock(&mtx_susrdy);
	return pcb;
}


bool menor_rafaga(pcb *pcb1, pcb *pcb2) {
    return pcb1->estimacion_rafaga <= pcb2->estimacion_rafaga;
}


