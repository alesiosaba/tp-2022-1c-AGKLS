#include "stdio.h"
#include "stdlib.h"
#include "../include/init.h"
#include "../include/comunicacion.h"
#include "../include/globals.h"

void servidorDispatch(){
	serverDispatch = iniciar_servidor(IP, config_values.puerto_escucha_dispatch);
	log_info(logger, "Servidor listo para recibir la conexion dispatch del kernel");

	pthread_t thr_dispatch_individual;

	while(1){
		clienteDispatch = esperar_cliente(serverDispatch);
        pthread_create(&thr_dispatch_individual, NULL, (void*) manejarConexion, (void*) clienteDispatch);
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
        pthread_create(&thr_interrupt_individual, NULL, (void*) manejarConexion, (void*) clienteInterrupt);
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

	pthread_t thr_memoria_individual;

	if(pthread_create(&thr_memoria_individual, NULL, (void*) manejarConexion,(void*)conexionAMemoria) != 0){
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

int manejarConexion(int socket_cliente){

	t_list* lista;
	struct pcb *pcb;

	while (1) {
		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			enviar_mensaje("HOLA", socket_cliente);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, LECTURA_DE_VALORES);
			break;
		case PAQUETE_PCB:
			log_debug(logger, RECEPCION_PAQUETE_PCB);
			recv_paquete_pcb(socket_cliente, &pcb);
			imprimir_PCB(pcb);
			pcb->state = EXIT;
			send_paquete_pcb(socket_cliente, pcb);

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
