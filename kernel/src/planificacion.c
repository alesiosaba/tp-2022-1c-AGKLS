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

		if(!planificador_es_fifo() && !list_is_empty(listaExec)){
			send_interrupcion(conexionACPU_interrupt);
			log_debug(logger, INTERRUPCION_ENVIADA);
		}

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
	case EXIT:
			list_add(listaExit, *new_pcb);
			log_debug(logger, PROCESS_MOVE_EXIT, (*new_pcb)->id);
			break;
	}

}

bool planificador_es_fifo(){
	return config_values.algoritmo_planificacion == FIFO;
}

void planificadorCortoPlazo(pcb *nodo_pcb){
	pcb* pcb;

	while(1){
		sem_wait(&sem_comenzarProcesos);
		sem_wait(&sem_ProcesosReady);

		pcb = dequeu_ready();

		movePCBto(&pcb, EXECUTION);
		sem_post(&sem_enviarPCB);
		log_info(logger,"Pongo a ejecutar al proceso %d",pcb->id);
	}

}

pcb* dequeu_ready(){

	if(planificador_es_fifo()){
		return list_remove(listaReady,0);
	}else{
		list_sort(listaReady, (void*)menor_rafaga);
		return list_remove(listaReady,0);
	}

}

bool menor_rafaga(pcb *pcb1, pcb *pcb2) {
    return pcb1->estimacion_rafaga <= pcb2->estimacion_rafaga;
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
		pcb* pcb = list_get(listaExec, 0);
		op_code codigo_paquete = PAQUETE_PCB;
		start_time = time(NULL);
		send_paquete_pcb(conexionACPU, pcb, codigo_paquete);
		destruir_PCB(pcb);
		pcb = recv_mensajes_cpu(socket_fd, &tipo_instruccion);
		end_time = time(NULL);
		list_remove(listaExec, 0);
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
			sem_post(&sem_ProcesosReady);
			break;
		case 1: //IO
			log_debug(logger, "Replanificacion: IO");
			break;
		case 2: //EXIT
			//TODO avisar memoria
			log_debug(logger, "Replanificacion: EXIT");
			movePCBto(&pcb, EXIT);
			send_paquete_kernel(arr_procesos[pcb->id],TERMINO_EL_PROCESO);
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


