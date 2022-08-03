#include <readline/readline.h>

#include "../include/globals.h"
#include "../include/init.h"
#include "../include/comunicacion.h"


void servidorDispatch(){
	serverDispatch = iniciar_servidor(config_values.ip_escucha, config_values.puerto_escucha_dispatch);
	log_info(logger, "Servidor listo para recibir la conexion dispatch del kernel");

	pthread_t thr_dispatch_individual;

	while(1){
		clienteDispatch = esperar_cliente(serverDispatch);
        pthread_create(&thr_dispatch_individual, NULL, (void*) manejarDispatch, (void*) clienteDispatch);
        pthread_detach(thr_dispatch_individual);
	}

	close(serverDispatch);
}

void servidorInterrupt(){
	serverInterrupt = iniciar_servidor(config_values.ip_escucha, config_values.puerto_escucha_interrupt);
	log_info(logger, "Servidor listo para recibir la conexion interrupt del kernel");

	pthread_t thr_interrupt_individual;

	while(1){
		clienteInterrupt = esperar_cliente(serverInterrupt);
        pthread_create(&thr_interrupt_individual, NULL, (void*) manejarInterrupt, (void*) clienteInterrupt);
        pthread_detach(thr_interrupt_individual);
	}

	close(serverInterrupt);
}

void conectar_memoria(){
	conexionAMemoria = crear_conexion(config_values.IP_memoria, config_values.puerto_memoria);

	if(conexionAMemoria == -1){
		log_error(logger, "El servidor de memoria no esta disponible");
		terminar_programa();
		exit(-1);
	}

	/*
	if(pthread_create(&thr_memoria, NULL, (void*) manejarMemoria, (void*) conexionAMemoria) != 0){
		log_error(logger, "Error al crear el hilo la Memoria");
	}
	log_debug(logger, "Se creo un thread para %s", "Memoria");
	*/

	log_info(logger, "Conexión exitosa con la Memoria");
}

void conexiones(){

	// abro un hilo para escuchar por el puerto dispatch
	pthread_create(&thr_dispatch, NULL, (void*) &servidorDispatch, NULL);

	// abro un hilo para escuchar por el puerto interrupt
	pthread_create(&thr_interrupt, NULL, (void*) &servidorInterrupt, NULL);

	conectar_memoria();
}


// manejar conexion de Interrupt
int manejarInterrupt(int socket_cliente){
	while (1) {
		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case INTERRUPCION:
			activar_flag_interrupcion();
			log_info(logger, INTERRUPCION_RECIBIDA);
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

void manejarMemoria(int socket_fd){
    t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
    args->log = logger;
    args->fd = socket_fd;
 }

// manejar conexion de Dispatch
int manejarDispatch(int socket_cliente){

	t_list* lista;
	struct pcb *pcb;

	int i = 0;

	while (1) {
		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case PAQUETE_PCB:
			log_debug(logger, RECEPCION_PAQUETE_PCB);
			recv_paquete_pcb(socket_cliente, &pcb);

			log_debug(logger, "Me llego el pcb PID: %d", pcb->id);
			//imprimir_PCB(pcb);
			log_info(logger, "Alojando proceso en el CPU - PID: %d", pcb->id);

			limpiar_tlb(pcb->id);
			ejecutar_ciclo_instruccion(&pcb);

			log_debug(logger, "Estaba como procesoAnterior el PID : %d", procesoAnterior);
			procesoAnterior = pcb->id;
			log_debug(logger, "quedó como procesoAnterior el PID : %d", procesoAnterior);

			log_info(logger, "Desalojando proceso del CPU - PID: %d", pcb->id);

			break;

		case -1:
			log_error(logger, SERVIDOR_DESCONEXION);
			return EXIT_FAILURE;

		default:
			log_warning(logger,OPERACION_DESCONOCIDA);
			break;
		}
	}

	free(lista);
}

void realizar_handshake_inicial(){

	uint32_t handshake = 1;
	uint32_t tam_pagina;
	uint32_t entradas_por_tabla;

	log_info(logger, "INICIO HANDSHAKE con Memoria");

	send(conexionAMemoria, &handshake, sizeof(uint32_t), NULL);
	recv(conexionAMemoria, &tam_pagina, sizeof(uint32_t), MSG_WAITALL);

	send(conexionAMemoria, &handshake, sizeof(uint32_t), NULL);
	recv(conexionAMemoria, &entradas_por_tabla, sizeof(uint32_t), MSG_WAITALL);

	log_info(logger, "FINAL HANDSHAKE con Memoria");

	// log_debug(logger, "Valores obtenidos de la devolucion del handshake");
	// log_debug(logger, "tam_pagina: %d", tam_pagina);
	// log_debug(logger, "entradas_por_tabla: %d", entradas_por_tabla);

	// asigno variables globales de CPU con valores enviados por memoria
	tamanio_pagina = tam_pagina;
	cant_entradas_por_tabla = entradas_por_tabla;

	/*
	t_paquete* paquete = crear_paquete(HANDSHAKE_INICIAL);
	enviar_paquete(paquete, conexionAMemoria);
	eliminar_paquete(paquete);

	log_debug(logger, "esperando recv_respuesta_handshake_inicial");

	//struct handshake_inicial_s handshake_inicial = recv_respuesta_handshake_inicial(conexionAMemoria);
	//

	t_list* lista;
	log_debug(logger, "recibir_paquete en recv_respuesta_handshake_inicial");

	op_code cod_op;
	cod_op = recibir_operacion(fd);

	lista = recibir_paquete(fd);

	struct handshake_inicial_s hd_inicial;

	int tam_pagina = atoi(list_remove(lista,0));
	hd_inicial.tamanio_pagina = tam_pagina;
	log_debug(logger, "tam_pagina: %d", tam_pagina);

	int cant_entradas_por_tabla = atoi(list_remove(lista,0));
	hd_inicial.cant_entradas_por_tabla = cant_entradas_por_tabla;
	log_debug(logger, "cant_entradas_por_tabla: %d", cant_entradas_por_tabla);

	log_debug(logger,"deserializada la resp de handshake");

	list_destroy(lista);

	log_debug(logger, "salgo de recibir_paquete en recv_respuesta_handshake_inicial");

	//
	log_debug(logger, "struct de handsake asignada");

	tamanio_pagina = handshake_inicial.tamanio_pagina;
	cant_entradas_por_tabla = handshake_inicial.cant_entradas_por_tabla;

	log_debug(logger, "Respuesta de Handshake inicial");
	log_debug(logger, "tam pag: %d",tamanio_pagina);
	log_debug(logger, "cant entradas : %d",cant_entradas_por_tabla);
	*/
}
