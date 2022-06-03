#include "../include/init.h"
#include "../include/comunicacion.h"
#include "../../shared/include/utils/utils.h"

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void armar_PCB(t_list* lista){

	t_pcb* pcb = (struct t_pcb*)malloc(sizeof(struct t_pcb));
	pcb->instrucciones = NULL;


	// agarro el primer elemento (tamanio del proceso)
	t_list* tamanio_proceso = list_remove(lista, 0);
	pcb->tamanio = atoi(tamanio_proceso);

	// armo la lista de instrucciones y la guardo en el PCB
	pcb->instrucciones = armar_lista_instrucciones(lista);

	pcb->program_counter = pcb->instrucciones;

//	imprimir_PCB(pcb);
	mostrar_lista(pcb->instrucciones);
}

void imprimir_PCB(t_pcb pcb){

	log_debug(logger, "PCB.PID : %d",pcb.id);
	log_debug(logger, "PCB.PC  : %p",pcb.program_counter);
	log_debug(logger, "PCB.SIZE: %s",pcb.tamanio);
	log_debug(logger, "PCB.PAG : %d",pcb.tabla_paginas);
	log_debug(logger, "PCB.EST : %d",pcb.estimacion_rafaga);
	mostrar_lista(pcb.instrucciones);

}


int manejarConexion(void* void_args){

	t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* server_name = args->server_name;
    free(args);

	t_list* lista;
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
			lista = recibir_paquete(socket_cliente);
			log_info(logger, RECEPCION_PAQUETE_CONSOLA);
			armar_PCB(lista);
			log_info(logger, "Se armó un PCB correctamente.");
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



