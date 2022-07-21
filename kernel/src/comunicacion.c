#include "../include/comunicacion.h"

void manejar_consolas(int server_fd){
	while(1) {
        arr_procesos[idProceso] = esperar_cliente(server_fd);
        log_info(logger,"Nuevo proceso recibido");
        server_escuchar(logger, "Consola", arr_procesos[idProceso]);
    }
}


void manejar_cpu(int socket_fd){
	t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
	    args->log = logger;
	    args->fd = socket_fd;

	while(1){
		sem_wait(&sem_enviarPCB);
		pcb* pcb = list_remove(listaExec, 0);
		op_code codigo_paquete = PAQUETE_PCB;
		send_paquete_pcb(conexionACPU, pcb, codigo_paquete);
		//destruir_PCB(pcb);
		manejarConexion(args);
		sem_post(&sem_comenzarProcesos);
	}
 }

void manejar_memoria(int socket_fd){
    t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
    args->log = logger;
    args->fd = socket_fd;
    // ACA VA LA ESCUCHA DEL KERNEL A LA MEMORIA
    manejarConexion(args);
 }


int manejarConexion(void* void_args){

	t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* server_name = args->server_name;
    free(args);

	t_list* lista;
	pcb* nodo_pcb;
	while (socket_cliente != -1) {
		op_code cod_op;
		cod_op = recibir_operacion(socket_cliente);

		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, LECTURA_DE_VALORES);
			list_iterate(lista, (void*) iterator);
			break;
		case PAQUETE_CONSOLA:
			recibir_pcb(socket_cliente, &nodo_pcb);
	        list_add(listaNew,nodo_pcb);
	        list_add(listaProcesos,nodo_pcb);
	        log_info(logger,"Se recibió un nuevo proceso - PID:%d",idProceso-1);
	        sem_post(&sem_ProcesosNew);

			break;
		case PAQUETE_PCB:
			sem_post(&sem_respuestaPCB);
			log_debug(logger, RECEPCION_PAQUETE_PCB);
			recv_paquete_pcb(socket_cliente, &nodo_pcb);
			imprimir_PCB(nodo_pcb);
			break;
		case PAQUETE_PCB_IO:
			break;
		case PAQUETE_PCB_EXIT:
			break;
		case -1:
			log_warning(logger, SERVIDOR_DESCONEXION);
			return EXIT_FAILURE;
		default:
			log_warning(logger,OPERACION_DESCONOCIDA);
			break;
		}
	}
	log_warning(logger, "El cliente se desconecto de %s server", server_name);

	return EXIT_SUCCESS;

}

int server_escuchar(t_log* logger, char* server_name, int client_socket) {
	t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
	args->log = logger;
	args->fd = client_socket;
	args->server_name = server_name;
	manejarConexion(args);
	free(args);
	return EXIT_SUCCESS;
}

void recibir_pcb(int fd, pcb** nodo_pcb){
	recv_paquete_consola(fd, nodo_pcb);
	(*nodo_pcb)->id = idProceso;
	idProceso ++;
	(*nodo_pcb)->estimacion_rafaga = config_values.estimacion_inicial;
}

// Esta funcion notifica a la memoria de un proceso nuevo y envia un pcb para asignar su tabla de paginas
void inicializar_estructuras_en_memoria(pcb* nodo_pcb){
	op_code codigo_paquete = PAQUETE_PCB;
	send_paquete_pcb(conexionAMemoria, nodo_pcb, codigo_paquete);
}

