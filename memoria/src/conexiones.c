#include "../../shared/include/utils/protocolo.h"
#include "../include/conexiones.h"
#include "../include/init.h"

void iniciar_servidor_memoriaCPU(){
	serverMemoriaCPU = iniciar_servidor(config_values.ip_escucha, config_values.puerto_escucha_CPU);
	log_debug(logger, "Servidor de memoria - Memoria CPU listo: IP: %s, Puerto: %s", config_values.ip_escucha, config_values.puerto_escucha_CPU);

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
	log_debug(logger, "Servidor de memoria - Memoria Kernel listo: IP: %s, Puerto: %s", config_values.ip_escucha, config_values.puerto_escucha_kernel);

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
	msleep(config_values.retardo_memoria);
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

	struct consulta_en_tabla_paginas* consulta;
	struct direccion_fisica* direccion_fisica_lectura;

		while (1){
			// log_debug(logger, "dentro de manejarConexionCPU() socket: %d", socket_cliente);
			// log_debug(logger,"Esperando nueva operacion en socket: %d", socket_cliente);
			int cod_op = recibir_operacion(socket_cliente);

			//TIEMPO RETARDO MEMORIA
			retardo_memoria();

			switch (cod_op) {

			case SOLICITUD_TABLA_PAGINA_N1:
				log_warning(logger, "Memoria recibio SOLICITUD_TABLA_PAGINA_N1");
				recv_solicitud_tabla(socket_cliente, &consulta);
				log_info(logger, "SOLICITUD_TABLA_PAGINA_N1: PID: %d ID_TABLA_N1: %d ENTRADA_TABLA_N1 %d", consulta->id_proceso, consulta->id_tabla, consulta->entrada_en_tabla);
				int numero_tabla_N2 = solicitud_tabla_n1(consulta->id_tabla, consulta->entrada_en_tabla);
				send_respuesta_solicitud_tabla(socket_cliente, numero_tabla_N2, RESPUESTA_SOLICITUD_N1);
				break;

			case SOLICITUD_TABLA_PAGINA_N2:
				log_warning(logger, "Memoria recibio SOLICITUD_TABLA_PAGINA_N2");
				recv_solicitud_tabla(socket_cliente, &consulta);
				// buscar entrada tabla N1 utilizando el objeto consulta
				int frame = solicitud_tabla_n2(consulta->id_proceso, consulta->id_tabla, consulta->entrada_en_tabla);
				send_respuesta_solicitud_tabla(socket_cliente, frame, RESPUESTA_SOLICITUD_N2);
				break;

			case PEDIDO_LECTURA:
				log_warning(logger, "Memoria recibio PEDIDO_LECTURA");
				recv_pedido_lectura(socket_cliente, &direccion_fisica_lectura);
				uint32_t valor_leido = solicitud_lectura_memoria(direccion_fisica_lectura->marco, direccion_fisica_lectura->desplazamiento);
				send_respuesta_pedido_lectura(socket_cliente, valor_leido);
				break;

			case PEDIDO_ESCRITURA:
				log_warning(logger, "Memoria recibio PEDIDO_ESCRITURA");
				int valor_a_escribir = recv_pedido_escritura(socket_cliente, &direccion_fisica_lectura);
				int resultadoEscritura = solicitud_escritura_memoria(valor_a_escribir, direccion_fisica_lectura->marco, direccion_fisica_lectura->desplazamiento);
				send_respuesta_pedido_escritura(socket_cliente,resultadoEscritura);
				break;

			case -1:
				log_error(logger, SERVIDOR_DESCONEXION);
				return EXIT_FAILURE;
			default:
				log_warning(logger,OPERACION_DESCONOCIDA);
				break;
			}
		}

	free(consulta);
	free(direccion_fisica_lectura);
}

int manejarConexionKernel(int socket_cliente){

	while (1){

	// log_debug(logger, "dentro de manejarConexionKERNEL() socket: %d", socket_cliente);
	// log_debug(logger,"Esperando nueva operacion en socket: %d", socket_cliente);
	int cod_op = recibir_operacion(socket_cliente);
	switch (cod_op) {

	case SOLICITUD_NUEVO_PROCESO:
		log_warning(logger, "Memoria recibio SOLICITUD_NUEVO_PROCESO");
		solicitud_nuevo_proceso(socket_cliente, logger);
		break;

	case SOLICITUD_SUSPENSION_PROCESO:
		log_warning(logger, "Memoria recibio SOLICITUD_SUSPENSION_PROCESO");
		solicitud_suspension_proceso(socket_cliente);
		break;

	case SOLICITUD_DESUSPENSION_PROCESO:
		log_warning(logger, "Memoria recibio SOLICITUD_DESUSPENSION_PROCESO");
		solicitud_desuspension_proceso(socket_cliente);
		break;
	case SOLICITUD_FINALIZAR_PROCESO:
		log_warning(logger, "Memoria recibio SOLICITUD_FINALIZAR_PROCESO");
		solicitud_eliminar_proceso(socket_cliente);
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
