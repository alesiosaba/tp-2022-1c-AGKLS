#include "../include/conexiones.h"
#include "../include/init.h"

void iniciar_servidor_memoria(){

	serverMemoria = iniciar_servidor(IP, config_values.puerto_escucha);
	log_info(logger, "Servidor de memoria listo para recibir conexiones.");

	pthread_t thr_memoria_individual;

	while(1){
		clienteMemoria = esperar_cliente(serverMemoria);
        pthread_create(&thr_memoria_individual, NULL, (void*) manejarConexion, (void*) clienteMemoria);
        pthread_detach(thr_memoria_individual);
	}

	close(serverMemoria);
}

void retardo_memoria() {
	// retard_memoria viene en milisegundos
	sleep(config_values.retardo_memoria / 1000);
}

int manejarConexion(int socket_cliente){

	//TIEMPO RETARDO MEMORIA
	retardo_memoria();

	t_list* lista;
	struct pcb *pcb;

	while (1) {
		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, LECTURA_DE_VALORES);
			break;
		case NUEVO_PROCESO:
			// El proceso tiene que ser almacenado en memoria
			recv_paquete_pcb(socket_cliente, &pcb);
			imprimir_PCB(pcb);

			// TODO: incompleto
			asignar_nuevas_paginas(pcb);
			break;
		case -1:
			log_error(logger, SERVIDOR_DESCONEXION);
			return EXIT_FAILURE;
		default:
			log_warning(logger,OPERACION_DESCONOCIDA);
			break;
		}
	}

}
