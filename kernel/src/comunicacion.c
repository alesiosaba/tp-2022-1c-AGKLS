#include "../include/comunicacion.h"

void manejar_consolas(int server_fd){
	while(1) {
        arr_procesos[idProceso] = esperar_cliente(server_fd); // @suppress("Symbol is not resolved")
        log_info(logger,"Nueva consola recibida");
        server_escuchar(logger, "Consola", arr_procesos[idProceso]);
    }
}


void manejar_memoria(int socket_fd){
    t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
    args->log = logger;
    args->fd = socket_fd;
    // ACA VA LA ESCUCHA DEL KERNEL A LA MEMORIA
    manejarConexion(args);
 }

void manejar_cpu_interrupcion(int socket_fd){
	while(1){
	}
}


pcb* recv_mensajes_cpu(int socket_cpu, int ** tipo_instruccion){

	t_list* lista;
	pcb* nodo_pcb;
	//while(socket_cpu != -1){
	op_code cod_op;
	cod_op = recibir_operacion(socket_cpu);

	switch (cod_op) {

	case PAQUETE_PCB:
		recv_paquete_pcb(socket_cpu, &nodo_pcb);
		imprimir_PCB(nodo_pcb);
		**tipo_instruccion = 0;
		//return nodo_pcb;
		break;
	case PAQUETE_PCB_IO:
		recv_paquete_pcb(socket_cpu, &nodo_pcb);
		**tipo_instruccion = 1;
		break;
	case PAQUETE_PCB_EXIT:
		recv_paquete_pcb(socket_cpu, &nodo_pcb);
		**tipo_instruccion = 2;
		break;
	case -1:
		log_warning(logger, SERVIDOR_DESCONEXION);
		return NULL;
	default:
		log_warning(logger,OPERACION_DESCONOCIDA);
		break;
	}

	log_warning(logger, "Se recibio un nuevo PCB para el Proceso %d", nodo_pcb->id);

	return nodo_pcb;

}



int manejarConexion(void* void_args){

	t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* server_name = args->server_name;
    free(args);

	t_list* lista;
	pcb* nodo_pcb;
//	while (socket_cliente != -1) {
	op_code cod_op;
	cod_op = recibir_operacion(socket_cliente);

	switch (cod_op) {
	case PAQUETE_CONSOLA:
		recibir_pcb(socket_cliente, &nodo_pcb);
//			imprimir_PCB(nodo_pcb);
		list_add(listaNew,nodo_pcb);
		list_add(listaProcesos,nodo_pcb);
		log_info(logger,"Se recibió un nuevo proceso - PID:%d",idProceso-1);
		sem_post(&sem_ProcesosNew);
		break;
	case -1:
		log_warning(logger, SERVIDOR_DESCONEXION);
		return EXIT_FAILURE;
	default:
		log_warning(logger,OPERACION_DESCONOCIDA);
		break;
	}
//	}
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

