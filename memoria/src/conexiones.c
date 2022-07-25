#include "../../shared/include/utils/protocolo.h"
#include "../include/conexiones.h"
#include "../include/init.h"

void iniciar_servidor_memoria(){

	serverMemoria = iniciar_servidor(IP, config_values.puerto_escucha);
	log_debug(logger, "Servidor de memoria listo para recibir conexiones.");

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
	struct consulta_en_tabla_paginas *consulta;

	while (1) {
		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;

		case PAQUETE:
			lista = recibir_paquete(socket_cliente);
			log_debug(logger, LECTURA_DE_VALORES);
			break;

		case HANDSHAKE_INICIAL:
			log_debug(logger, "HANDSHAKE INICIAL con modulo CPU");
			send_respuesta_handshake_inicial(socket_cliente, config_values.tam_pagina, config_values.entradas_por_tabla);
			break;

		case NUEVO_PROCESO:
			// El proceso tiene que ser almacenado en memoria
			recv_paquete_pcb(socket_cliente, &pcb);
			imprimir_PCB(pcb);

			// TODO: incompleto
			asignar_nuevas_paginas(pcb);
			break;

		case SOLICITUD_TABLA_PAGINA_N1:
			recv_solicitud_tabla(socket_cliente, &consulta);
			log_debug(logger, "id_tablaN1: %d\tentrada: %d", consulta->id_tabla, consulta->entrada_en_tabla);

			// buscar entrada tabla N1 utilizando el objeto consulta
			int numero_tabla_N2 = 786;

			send_respuesta_solicitud_tabla(socket_cliente, numero_tabla_N2);
			break;

		case SOLICITUD_TABLA_PAGINA_N2:
			recv_solicitud_tabla(socket_cliente, &consulta);
			log_debug(logger, "id_tablaN2: %d\tentrada: %d", consulta->id_tabla, consulta->entrada_en_tabla);

			// buscar entrada tabla N1 utilizando el objeto consulta
			int frame = 987;

			send_respuesta_solicitud_tabla(socket_cliente, frame);
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
