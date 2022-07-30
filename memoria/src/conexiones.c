#include "../../shared/include/utils/protocolo.h"
#include "../include/conexiones.h"
#include "../include/init.h"

void iniciar_servidor_memoriaCPU(){
	serverMemoriaCPU = iniciar_servidor(config_values.ip_escucha, config_values.puerto_escucha_CPU);
	log_debug(logger, "Servidor de memoria - CPU listo para recibir conexiones.");

	pthread_t thr_memoria_CPU;

	// clienteMemoriaCPU = esperar_cliente(serverMemoriaCPU);

	// handshake_inicial();

	while(1){
		clienteMemoriaCPU = esperar_cliente(serverMemoriaCPU);

		handshake_inicial();

        pthread_create(&thr_memoria_CPU, NULL, (void*) manejarConexionCPU, (void*) clienteMemoriaCPU);
        pthread_detach(thr_memoria_CPU);
	}

	close(serverMemoriaCPU);
}

void iniciar_servidor_memoriaKernel(){
	serverMemoriaKernel = iniciar_servidor(config_values.ip_escucha, config_values.puerto_escucha_kernel);
	log_debug(logger, "Servidor de memoria - Kernel listo para recibir conexiones.");

	pthread_t thr_memoria_Kernel;

	while(1){
        clienteMemoriaKernel = esperar_cliente(serverMemoriaKernel);
        pthread_create(&thr_memoria_Kernel, NULL, (void*) manejarConexionKernel, (void*) clienteMemoriaKernel);
        pthread_detach(thr_memoria_Kernel);
	}

	close(serverMemoriaKernel);
}

void retardo_memoria() {
	// retard_memoria viene en milisegundos
	sleep(config_values.retardo_memoria / 1000);
}

void handshake_inicial(){
	uint32_t handshake;
	uint32_t tam_pagina = config_values.tam_pagina;
	uint32_t entradas_por_tabla = config_values.entradas_por_tabla;

	log_info(logger, "COMIENZA HANDSHAKE");

	// log_info(logger, "Recibiendo handshake tam_pagina...");
	recv(clienteMemoriaCPU, &handshake, sizeof(uint32_t), MSG_WAITALL);
	// log_info(logger, "Enviando tam_pagina...");
	send(clienteMemoriaCPU, &tam_pagina, sizeof(uint32_t), NULL);
	// log_info(logger, "handshake tam_pagina enviado...");

	// log_info(logger, "Recibiendo handshake entradas_por_tabla...");
	recv(clienteMemoriaCPU, &handshake, sizeof(uint32_t), MSG_WAITALL);
	// log_info(logger, "Enviando entradas_por_tabla...");
	send(clienteMemoriaCPU, &entradas_por_tabla, sizeof(uint32_t), NULL);
	// log_info(logger, "handshake entradas_por_tabla enviado...");

	log_info(logger, "FINALIZA HANDSHAKE...");

}

int manejarConexionCPU(int socket_cliente){

	//TIEMPO RETARDO MEMORIA
	retardo_memoria();

	t_list* lista;
	struct consulta_en_tabla_paginas* consulta;

		while (1){
			// log_debug(logger, "dentro de manejarConexionCPU() socket: %d", socket_cliente);
			// log_debug(logger,"Esperando nueva operacion en socket: %d", socket_cliente);
			int cod_op = recibir_operacion(socket_cliente);
			switch (cod_op) {
			case MENSAJE:
				recibir_mensaje(socket_cliente);
				break;

			case PAQUETE:
				lista = recibir_paquete(socket_cliente);
				log_debug(logger, LECTURA_DE_VALORES);
				break;

			case SOLICITUD_TABLA_PAGINA_N1:
				log_info(logger, "Memoria recibio SOLICITUD_TABLA_PAGINA_N1");
				recv_solicitud_tabla(socket_cliente, &consulta);
				log_warning(logger, "Despues del recv en SOLICITUD_TABLA_PAGINA_N1");


				// buscar entrada tabla N1 utilizando el objeto consulta
				// TODO: numero_tabla_N2 = solicitud_tabla_paginas(socket_cliente, logger); pasandole los valores que necesiten
				int numero_tabla_N2 = consulta->entrada_en_tabla;

				log_warning(logger, "numero_tabla_N2 que se envia: %d" , numero_tabla_N2);

				send_respuesta_solicitud_tabla(socket_cliente, numero_tabla_N2, RESPUESTA_SOLICITUD_N1);
				break;

			case SOLICITUD_TABLA_PAGINA_N2:
				log_info(logger, "Memoria recibio SOLICITUD_TABLA_PAGINA_N2");
				recv_solicitud_tabla(socket_cliente, &consulta);
				log_warning(logger, "Despues del recv en SOLICITUD_TABLA_PAGINA_N2");

				// buscar entrada tabla N1 utilizando el objeto consulta
				// TODO: frame = solicitud_marco(socket_cliente, logger); pasandole los valores que necesiten
				int frame = consulta->id_tabla;

				log_warning(logger, "frame que se envia: %d" , frame);

				send_respuesta_solicitud_tabla(socket_cliente, frame, RESPUESTA_SOLICITUD_N2);
				break;


			case SOLICITUD_TABLA_PAGINAS:
				log_info(logger, "Memoria recibio SOLICITUD_TABLA_PAGINAS");
				lista = recibir_paquete(socket_cliente);
				// solicitud_tabla_paginas(socket_cliente, logger);
				break;

			case SOLICITUD_MARCO:
				log_info(logger, "Memoria recibio SOLICITUD_MARCO");
				lista = recibir_paquete(socket_cliente);
				// solicitud_marco(socket_cliente, logger);
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

int manejarConexionKernel(int socket_cliente){

	//TIEMPO RETARDO MEMORIA
	retardo_memoria();

	t_list* lista;
	struct pcb *pcb;

	// handshake_inicial();

	while (1){
			// log_debug(logger, "dentro de manejarConexionKERNEL() socket: %d", socket_cliente);
			// log_debug(logger,"Esperando nueva operacion en socket: %d", socket_cliente);
			int cod_op = recibir_operacion(socket_cliente);
			switch (cod_op) {
			case MENSAJE:
				recibir_mensaje(socket_cliente);
				break;

			case PAQUETE:
				lista = recibir_paquete(socket_cliente);
				log_debug(logger, LECTURA_DE_VALORES);
				break;

			case SOLICITUD_NUEVO_PROCESO:
				log_info(logger, "Memoria recibio SOLICITUD_NUEVO_PROCESO");
				solicitud_nuevo_proceso(socket_cliente, logger);
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

/*
int manejarConexion(int socket_cliente){
	//TIEMPO RETARDO MEMORIA
	retardo_memoria();

	t_list* lista;
	struct pcb *pcb;
	struct consulta_en_tabla_paginas *consulta;

	while (1){
		// log_debug(logger, "dentro de manejarConexion() socket: %d", socket_cliente);
		// log_debug(logger,"Esperando nueva operacion en socket: %d", socket_cliente);
		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;

		case PAQUETE:
			lista = recibir_paquete(socket_cliente);
			log_debug(logger, LECTURA_DE_VALORES);
			break;

		/*
		case HANDSHAKE_INICIAL:
			log_debug(logger, "HANDSHAKE INICIAL con modulo CPU");
			send_respuesta_handshake_inicial(socket_cliente, config_values.tam_pagina, config_values.entradas_por_tabla);
			break;
		*/

		/*
		case NUEVO_PROCESO:
			// El proceso tiene que ser almacenado en memoria
			recv_paquete_pcb(socket_cliente, &pcb);
			imprimir_PCB(pcb);

			// TODO: incompleto
			asignar_nuevas_paginas(pcb);
			break;

      
		case SOLICITUD_NUEVO_PROCESO:
			log_info(logger, "Memoria recibio SOLICITUD_NUEVO_PROCESO");
			solicitud_nuevo_proceso(socket_cliente, logger);
			break;
      
		case SOLICITUD_TABLA_PAGINAS:
			log_info(logger, "Memoria recibio SOLICITUD_TABLA_PAGINAS");
            solicitud_tabla_paginas(socket_cliente, logger);
			break;
      
		case SOLICITUD_MARCO:
			log_info(logger, "Memoria recibio SOLICITUD_MARCO");
            solicitud_marco(socket_cliente, logger);
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
*/
