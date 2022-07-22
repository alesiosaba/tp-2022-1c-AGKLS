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


void movePCBto(pcb** new_pcb, status new_status){
	(*new_pcb)->status = new_status ;
	switch(new_status){
	case READY:
		list_add(listaReady, *new_pcb);
		break;
	case NEW:
		list_add(listaNew, *new_pcb);
		break;
	case EXECUTION:
		list_add(listaExec, *new_pcb);
		break;
	}

}


void planificadorCortoPlazo(pcb *nodo_pcb){
	pcb* pcb;
	switch (config_values.algoritmo_planificacion){
		case FIFO:
			while(1){
					sem_wait(&sem_comenzarProcesos);
				    sem_wait(&sem_ProcesosReady);
			        pcb = list_remove(listaReady,0);
			        movePCBto(&pcb, EXECUTION);
			        //list_add(listaExec,pcb);
			        sem_post(&sem_enviarPCB);
			        log_info(logger,"Pongo a ejecutar al proceso %d",pcb->id);
			}
			break;
		default:
			while(1){
				sem_wait(&sem_comenzarProcesos);

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
			pcb = list_remove(listaNew,0);
			movePCBto(&pcb, READY);
			sem_post(&sem_ProcesosReady);
		}
	}
}


