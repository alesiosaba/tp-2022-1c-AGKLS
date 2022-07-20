#include "../include/planificacion.h"


void iniciarPlanificacion() {
    bool error=true;
    if (pthread_create(&thr_planifST, NULL, (void*) &planificadorCortoPlazo, NULL) != 0) {
		log_error(logger, "Error al crear el hilo del planificador de corto plazo");
        error = false;
	}
    if (pthread_create(&thr_planifLT, NULL, (void*) &planificadorLargoPlazo, NULL) != 0) {
		log_error(logger, "Error al crear el hilo del planificador");
        error = false;
	}
    /*
    if (pthread_create(&hiloBloqueos, NULL, (void*) &resolverBloqueos, NULL) != 0) {
		log_error(logger, "Error al crear el hilo de bloqueos");
        error++;
	}
    if (pthread_create(&hiloSuspended, NULL, (void*) &resolverSuspended, NULL) != 0) {
		log_error(logger, "Error al crear el hilo de suspended");
        error++;
	}*/
	if(error) log_info(logger,"Planificadores Inicializados");
}


nodo_readyQueue* nuevo_nodo_readyQueue(){
	nodo_readyQueue* nodo = (struct nodo_readyQueue*)malloc(sizeof(struct nodo_readyQueue));
	nodo->sig = NULL;
	nodo->pcb = NULL;
	return nodo;
}

void agregarPrimerPCB(pcb* PCB){

	// genero el primer nodo de la lista de la cola de listo
	nodo_readyQueue* nodo_pcb = nuevo_nodo_readyQueue();

	nodo_pcb->pcb = PCB;

	readyQueue = nodo_pcb;

};

void agregarPCB(pcb* PCB){

	nodo_readyQueue* aux = readyQueue;

	// genero el primer nodo de la lista de la cola de listo
	while(aux->sig != NULL){
		aux = aux->sig;
	}

	aux->sig = nuevo_nodo_readyQueue();
	aux->sig->pcb = *PCB;
	aux->sig->sig = NULL;

};



void movePCBtoReady(pcb** new_pcb){
	(*new_pcb)->state = READY ;

	if(readyQueue == NULL){
		agregarPrimerPCB(*new_pcb);
	}else{
		agregarPCB(*new_pcb);
	}

}


void planificadorCortoPlazo(pcb *nodo_pcb){
	pcb* pcb;
	switch (config_values.algoritmo_planificacion){
		case FIFO:
			while(1){
				    sem_wait(&sem_ProcesosReady);
			        pcb = list_remove(listaReady,0);
			        list_add(listaExec,pcb);
			        log_info(logger,"Pongo a ejecutar al proceso %d",pcb->id);
			}
			break;
		default:
			while(1){

			}
			break;
	}
}


void planificadorLargoPlazo(pcb *nodo_pcb){
	pcb* pcb;
	int respuestaMemoriaDummy = 10;
	int numeroTabla;
	while(1){
		sem_wait(&sem_ProcesosNew);
		pcb = list_get(listaNew,0);
		numeroTabla = respuestaMemoriaDummy; //todo Acá pido el numero de tabla para la memoria
		if(numeroTabla < 9999){
			log_info(logger,INICIALIZACION_PROCESOS,pcb->id);
			list_add(listaReady,pcb);
			sem_post(&sem_ProcesosReady);
		}
	}
}



void ejecutarFIFO(){

}


