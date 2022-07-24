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
	conexionAMemoria = crear_conexion(config_values.IP_memoria,config_values.puerto_memoria);

	if(conexionAMemoria == -1){
		log_error(logger, "El servidor de memoria no esta disponible");
		terminar_programa();
		exit(-1);
	}

	// TODO: Mejor lugar para este pedido?
	realizar_handshake_inicial();

	pthread_t thr_memoria_individual;

	if(pthread_create(&thr_memoria_individual, NULL, (void*) manejarDispatch,(void*)conexionAMemoria) != 0){
		log_error(logger, "Error al crear el hilo la Memoria");
	}

	log_info(logger, "Conexión exitosa con la Memoria");
}

void conexiones (){

	// abro un hilo para escuchar por el puerto dispatch
	pthread_create(&thr_dispatch, NULL, (void*) &servidorDispatch, NULL);

	// abro un hilo para escuchar por el puerto interrupt
	pthread_create(&thr_interrupt, NULL, (void*) &servidorInterrupt, NULL);

	// abro un hilo para escuchar por el puerto dedicado a la memoria
	pthread_create(&thr_memoria, NULL, (void*) &conectar_memoria, NULL);

}


// manejar conexion de Interrupt
void manejarInterrupt(int socket_cliente){
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

			ejecutar_ciclo_instruccion(&pcb);

			log_debug(logger, "Desalojando proceso del CPU - PID: %d", pcb->id);

			break;

		case RESPUESTA_HANDSHAKE_INICIAL:
			log_debug(logger, "Respuesta de Handshake inicial");
			struct handshake_inicial_s hd_inicial = recv_respuesta_handshake_inicial(socket_cliente);
			tamanio_pagina = hd_inicial.tamanio_pagina;
			cant_entradas_por_tabla = hd_inicial.cant_entradas_por_tabla;
			log_debug(logger,"tm : %d",tamanio_pagina);
			log_debug(logger,"ce : %d",cant_entradas_por_tabla);
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
	send_handshake_inicial(conexionAMemoria);
}
