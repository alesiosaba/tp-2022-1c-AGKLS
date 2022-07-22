#include "../include/planificacion.h"


void iniciarPlanificacion() {
    bool error=true;
    if (pthread_create(&thr_planifST, NULL, (void*) &planificadorCortoPlazo, NULL) != 0) {
		log_error(logger, "Error al crear el hilo del planificador de corto plazo");
        error = false;
	}
    log_debug(logger, "se creo un thread para %s", "STS");
    if (pthread_create(&thr_planifLT, NULL, (void*) &planificadorLargoPlazo, NULL) != 0) {
		log_error(logger, "Error al crear el hilo del planificador");
        error = false;
	}
    log_debug(logger, "se creo un thread para %s", "LTS");
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
		log_debug(logger, PROCESS_MOVE_READY, (*new_pcb)->id);
		break;
	case NEW:
		list_add(listaNew, *new_pcb);
		log_debug(logger, PROCESS_MOVE_NEW, (*new_pcb)->id);
		break;
	case EXECUTION:
		list_add(listaExec, *new_pcb);
		log_debug(logger, PROCESS_MOVE_EXEC, (*new_pcb)->id);
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

void planificacion_cpu(int socket_fd){
	time_t start_time, end_time;
	double tiempo_rafaga;
	int* tipo_instruccion = malloc(sizeof(int));
	while(1){
		sem_wait(&sem_enviarPCB);
		pcb* pcb = list_remove(listaExec, 0);
		op_code codigo_paquete = PAQUETE_PCB;
		start_time = time(NULL);
		send_paquete_pcb(conexionACPU, pcb, codigo_paquete);
		destruir_PCB(pcb);
		pcb = recv_mensajes_cpu(socket_fd, &tipo_instruccion);
		end_time = time(NULL);
		tiempo_rafaga=difftime(end_time,start_time)*1000;
		log_debug(logger,"Duracion de la rafaga de CPU: %f milisegundos",tiempo_rafaga);
		if(pcb == NULL){
			log_error(logger, ERROR_RECEPCION_PCB);
		}

		switch(*tipo_instruccion){
		case 0: //UPDATE
			log_debug(logger, "Replanificacion: UPDATE");
			pcb->estimacion_rafaga = pcb->estimacion_rafaga - tiempo_rafaga ;
			movePCBto(&pcb, READY);
			list_add(listaReady,pcb);
			sem_post(&sem_ProcesosReady);
			break;
		case 1: //IO
			log_debug(logger, "Replanificacion: IO");
			break;
		case 2: //EXIT
			log_debug(logger, "Replanificacion: EXIT");
			break;
		default:
			break;
		}
		sem_post(&sem_comenzarProcesos);
	}
 }


void estimar_proxima_rafaga(pcb* pcb, double rafaga){
	//E(n+1) = Alfa * Rn + (1-Alfa) * En
	double estimacion = config_values.alfa * rafaga + (1 - config_values.alfa) * pcb->estimacion_rafaga;
	pcb->estimacion_rafaga = estimacion;
}


