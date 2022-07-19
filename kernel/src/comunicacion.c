#include "../include/comunicacion.h"

void manejar_consolas(int server_fd){
	while(server_escuchar(logger, "Consola", server_fd));
}


void manejar_cpu(int socket_fd){
    t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
    args->log = logger;
    args->fd = socket_fd;
    // ACA VA LA ESCUCHA DEL KERNEL AL CPU
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
			recv_paquete_consola(socket_cliente, &nodo_pcb);

			planificar(config_values.algoritmo_planificacion, nodo_pcb);

			imprimir_PCB(nodo_pcb);

			break;
		case PAQUETE_PCB:
			log_debug(logger, RECEPCION_PAQUETE_PCB);
			recv_paquete_pcb(socket_cliente, &nodo_pcb);
			imprimir_PCB(nodo_pcb);

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

	return 0;

}

int server_escuchar(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) manejarConexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}


