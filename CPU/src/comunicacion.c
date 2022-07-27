#include <readline/readline.h>

#include "../include/globals.h"
#include "../include/init.h"
#include "../include/comunicacion.h"


void servidorDispatch(){
	serverDispatch = iniciar_servidor(IP, config_values.puerto_escucha_dispatch);
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
	serverInterrupt = iniciar_servidor(IP, config_values.puerto_escucha_interrupt);
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

	if(pthread_create(&thr_memoria, NULL, (void*) manejarMemoria, (void*) conexionAMemoria) != 0){
		log_error(logger, "Error al crear el hilo la Memoria");
	}

	log_debug(logger, "Se creo un thread para %s", "Memoria");
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
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;

		case PAQUETE:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, LECTURA_DE_VALORES);
			break;

		case PAQUETE_PCB:
			log_debug(logger, RECEPCION_PAQUETE_PCB);
			recv_paquete_pcb(socket_cliente, &pcb);
			log_debug(logger, "Me llego el pcb PID: %d", pcb->id);
			imprimir_PCB(pcb);

			log_debug(logger, "Alojando proceso en el CPU - PID: %d", pcb->id);

			// limpiar_tlb(pcb->id);
			ejecutar_ciclo_instruccion(&pcb);

			log_debug(logger, "Estaba como procesoAnterior el PID : %d", procesoAnterior);
			procesoAnterior = pcb->id;
			log_debug(logger, "quedó como procesoAnterior el PID : %d", procesoAnterior);

			log_debug(logger, "Desalojando proceso del CPU - PID: %d", pcb->id);


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

void realizar_handshake_inicial(){

	t_paquete* paquete = crear_paquete(HANDSHAKE_INICIAL);
	enviar_paquete(paquete, conexionAMemoria);
	eliminar_paquete(paquete);

	log_debug(logger, "esperando recv_respuesta_handshake_inicial");

	struct handshake_inicial_s handshake_inicial = recv_respuesta_handshake_inicial(conexionAMemoria);

	log_debug(logger, "struct de handsake asignada");

	tamanio_pagina = handshake_inicial.tamanio_pagina;
	cant_entradas_por_tabla = handshake_inicial.cant_entradas_por_tabla;

	log_debug(logger, "Respuesta de Handshake inicial");
	log_debug(logger, "tam pag: %d",tamanio_pagina);
	log_debug(logger, "cant entradas : %d",cant_entradas_por_tabla);
}
