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
    if (pthread_create(&thr_bloqueos, NULL, (void*) &planificacion_bloqueo, NULL) != 0) {
		log_error(logger, "Error al crear el hilo de bloqueos");
        error++;
    }
    if (pthread_create(&thr_suspended, NULL, (void*) &planificacion_suspended, NULL) != 0) {
		log_error(logger, "Error al crear el hilo de suspended");
        error++;
	}
	if(error) log_info(logger,"Planificadores Inicializados");
}

void planificadorLargoPlazo(pcb *nodo_pcb){
	pcb* pcb;
	int respuestaMemoriaDummy = 10;
	int numeroTabla;
	while(1){
		sem_wait(&sem_ProcesosNew);
		print_grado_multiprogramacion();
		sem_wait(&sem_multiprogramacion);
		pcb = list_get(listaNew,0);
		imprimir_PCB(pcb);
		send_paquete_pcb(conexionAMemoria, pcb, SOLICITUD_NUEVO_PROCESO); //todo Acá pido el numero de tabla para la memoria
		numeroTabla = recv_respuesta_nuevo_proceso(conexionAMemoria);
		log_debug(logger, "Se recibió el numero de tabla %d desde Memoria", numeroTabla);
		if(numeroTabla < 9999 && conexionAMemoria != -1){
			pcb->tabla_paginas = numeroTabla;
			log_info(logger,INICIALIZACION_PROCESOS,pcb->id);
			pcb = dequeue_new();
			movePCBto(&pcb, READY);
			sem_post(&sem_ProcesosReady);
		}else{
			error_handler(logger, errorMessageAux, ERROR_NUEVO_PROCESO_MEMORIA,pcb->id, NULL);
		}
	}
}

void planificadorMedianoPlazo(pcb *nodo_pcb){
	log_info(logger,"Empieza el contador para la suspensión del proceso %d",nodo_pcb->id);
	usleep(config_values.t_bloqueo_max*1000);
	pthread_mutex_lock(&mtx_planificador);
	int index = proceso_esta_en_lista(listaBlocked, nodo_pcb->id);
	if(index != -1){
		//TODO avisar a memoria para suspender el proceso
		log_info(logger,"El proceso %d se suspende por exceso de tiempo bloqueado",nodo_pcb->id);
		dequeue_blocked_at_index(index);
		movePCBto(&nodo_pcb, SUSPENDED_BLOCKED);
		sem_post(&sem_ProcesosSuspendedBlk);
		sem_post(&sem_multiprogramacion);
	}
	pthread_mutex_unlock(&mtx_planificador);
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

void planificacion_cpu(int socket_fd){
	time_t start_time, end_time;
	double tiempo_rafaga;
	int* tipo_instruccion = malloc(sizeof(int));
	while(1){
		sem_wait(&sem_enviarPCB);
		pcb* pcb = list_get(listaExec, 0);
		op_code codigo_paquete = PAQUETE_PCB;
		start_time = time(NULL);
		imprimir_PCB(pcb);
		send_paquete_pcb(conexionACPU, pcb, codigo_paquete);
		destruir_PCB(pcb);
		pcb = recv_mensajes_cpu(socket_fd, &tipo_instruccion);
		end_time = time(NULL);
		dequeue_execution();
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
			if(!planificador_es_fifo()) estimar_proxima_rafaga(pcb, tiempo_rafaga);
			movePCBto(&pcb, BLOCKED);
			enqueue_desbloqueo_pendiente(&pcb);
			//list_add(listaDesbloqueoPendiente, pcb);
            pthread_create(&thr_planifMT[pcb->id], NULL, (void*) &planificadorMedianoPlazo, pcb);
            pthread_detach(thr_planifMT[pcb->id]);
            sem_post(&sem_ProcesosBloqueo);
			break;
		case 2: //EXIT
			//TODO avisar memoria
			log_debug(logger, "Replanificacion: EXIT");
			movePCBto(&pcb, EXIT);
			send_paquete_kernel(arr_procesos[pcb->id],PAQUETE_KERNEL_EXIT);
			close(arr_procesos[pcb->id]);
			break;
		default:
			break;
		}
		sem_post(&sem_comenzarProcesos);
	}
 }

void planificacion_bloqueo(){
	pcb* pcb;
	int tiempo_bloqueo;
	int index;
	while(1){
		sem_wait(&sem_ProcesosBloqueo);
		pcb = dequeue_desbloqueo_pend();
		tiempo_bloqueo = pcb->tiempo_a_bloquearse;
		log_info(logger, "El proceso %d esta realizando una IO", pcb->id);
		usleep(tiempo_bloqueo*1000);

		pthread_mutex_lock(&mtx_planificador);
		index = proceso_esta_en_lista(listaBlocked, pcb->id);
		if(index != -1){
			dequeue_blocked_at_index(index);
			movePCBto(&pcb, READY);
			log_debug(logger,BLOQUEO_TERMINADO,pcb->id);
            sem_post(&sem_ProcesosReady);
		}else{
			index = proceso_esta_en_lista(listaSuspendedBlocked, pcb->id);
			if(index != -1){
				pcb = dequeue_suspendedblk_at_index(index);
				movePCBto(&pcb, SUSPENDED_READY);
				sem_wait(&sem_ProcesosSuspendedBlk);
				sem_post(&sem_ProcesosSuspended);
			}
		}
		pthread_mutex_unlock(&mtx_planificador);
	}

}

void planificacion_suspended(){
	pcb* pcb;
	while(1){
		sem_wait(&sem_ProcesosSuspended);
		sem_wait(&sem_multiprogramacion);
		pcb = dequeue_suspended_ready();
		movePCBto(&pcb, READY);
		log_info(logger, SUSPENCION_TERMINADA, pcb->id);
		sem_post(&sem_ProcesosReady);
	}
}


void movePCBto(pcb** new_pcb, status new_status){
	(*new_pcb)->status = new_status ;
	switch(new_status){
	case READY:
		pthread_mutex_lock(&mtx_ready);
		list_add(listaReady, *new_pcb);
		pthread_mutex_unlock(&mtx_ready);
		if(!planificador_es_fifo() && !list_is_empty(listaExec)){
			send_interrupcion(conexionACPU_interrupt);
			log_debug(logger, INTERRUPCION_ENVIADA);
		}
		log_debug(logger, PROCESS_MOVE_READY, (*new_pcb)->id);
		break;
	case NEW:
		pthread_mutex_lock(&mtx_new);
		list_add(listaNew, *new_pcb);
		pthread_mutex_unlock(&mtx_new);
		log_debug(logger, PROCESS_MOVE_NEW, (*new_pcb)->id);
		break;
	case EXECUTION:
		pthread_mutex_lock(&mtx_exec);
		list_add(listaExec, *new_pcb);
		pthread_mutex_unlock(&mtx_exec);
		log_debug(logger, PROCESS_MOVE_EXEC, (*new_pcb)->id);
		break;
	case BLOCKED:
		pthread_mutex_lock(&mtx_blocked);
		list_add(listaBlocked, *new_pcb);
		pthread_mutex_unlock(&mtx_blocked);
		log_debug(logger, PROCESS_MOVE_BLOCKED, (*new_pcb)->id);
		break;
	case SUSPENDED_BLOCKED:
		pthread_mutex_lock(&mtx_susblk);
		list_add(listaSuspendedBlocked, *new_pcb);
		pthread_mutex_unlock(&mtx_susblk);
		log_debug(logger, PROCESS_MOVE_SUSBLK, (*new_pcb)->id);
		break;
	case SUSPENDED_READY:
		pthread_mutex_lock(&mtx_susrdy);
		list_add(listaSuspendedReady, *new_pcb);
		pthread_mutex_unlock(&mtx_susrdy);
		log_debug(logger, PROCESS_MOVE_SUSRDY, (*new_pcb)->id);
		break;
	case EXIT:
		pthread_mutex_lock(&mtx_exit);
		list_add(listaExit, *new_pcb);
		sem_post(&sem_multiprogramacion);
		pthread_mutex_unlock(&mtx_exit);
		log_debug(logger, PROCESS_MOVE_EXIT, (*new_pcb)->id);
		break;
	}

}

void print_grado_multiprogramacion(){
	int cantidad_procesos_ex = list_size(listaExec);
	int cantidad_procesos_re = list_size(listaReady);
	int cantidad_procesos_bl = list_size(listaBlocked);

	int procesos = cantidad_procesos_ex + cantidad_procesos_re + cantidad_procesos_bl;

	int grado_multiprogramacion = config_values.grado_multiprog;

	log_info(logger, "Grado de multiprogramacion: %d", grado_multiprogramacion);
	log_info(logger, "Cantidad de procesos Actual: %d", procesos);

}

void estimar_proxima_rafaga(pcb* pcb, double rafaga){
	//E(n+1) = Alfa * Rn + (1-Alfa) * En
	double estimacion = config_values.alfa * rafaga + (1 - config_values.alfa) * pcb->estimacion_rafaga;
	pcb->estimacion_rafaga = estimacion;
}


bool planificador_es_fifo(){
	return config_values.algoritmo_planificacion == FIFO;
}

int proceso_esta_en_lista(t_list* lista, int id){
    int i;
    pcb* pcb;
    for (i=0; i<list_size(lista); i++) {
        pcb = list_get(lista,i);
        if (pcb->id==id) return i;
    }
    return -1;
}


