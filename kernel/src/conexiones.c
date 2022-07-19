#include "../include/conexiones.h"

void conectar_memoria() {
    log_info(logger,"Conexion exitosa con Memoria");
}


void conectar_cpu(){

	conexionACPU = crear_conexion(config_values.ip_cpu,config_values.puerto_cpu_dispatch);
	if(conexionACPU == -1){
		error_handler(logger, errorMessageAux, SERVIDOR_AUSENTE, " CPU", NULL);
		terminar_programa();
		exit(-1);
	}

	if(pthread_create(&thr_cpu, NULL, (void*) manejar_cpu,(void*)conexionACPU) != 0){
		log_error(logger, "Error al crear el hilo con el CPU - Dispatch");
	}
	log_info(logger, "Conexión exitosa con el CPU-Dispatch");

	conexionACPU_interrupt = crear_conexion(config_values.ip_cpu,config_values.puerto_cpu_interrupt);
	if(conexionACPU_interrupt == -1){
		error_handler(logger, errorMessageAux, SERVIDOR_AUSENTE, " CPU-Interrupt", NULL);
		terminar_programa();
	}

	if(pthread_create(&thr_cpu_interrupt, NULL, (void*) manejar_cpu,(void*)conexionACPU_interrupt) != 0){
		log_error(logger, "Error al crear el hilo con el CPU - Interrupt");
	}

	log_info(logger,"Conexion exitosa con CPU-Interrupt");

}


void servidor_procesos() {
	server_fd = iniciar_servidor(IP, config_values.puerto_escucha);
	if (pthread_create(&thr_consolas, NULL, (void*) manejar_consolas,(void*)server_fd) != 0) {
		log_error(logger, "Error al crear el hilo del servidor");
	}
	else log_info(logger,"Servidor iniciado y esperando que lleguen procesos");
}


