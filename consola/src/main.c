#include "../include/main.h"

int main(int argc, char** argv) {

	// Utilizados para calcular el tiempo total de ejecucion
	struct timeval start, stop;

	// Chequear que están los 2 parametros que pide el TP
	// Path al archivo de pseudocodigo y tamanio de espacio de direcciones del proceso
	if(argc != 3){
		printf("Cantidad erronea de parametros.\n");
		return EXIT_FAILURE;
	}

	int respuesta_kernel = 0;

	inicializar();

	// Asigno parametros enviados por consola
	char* path_pseudocodigo = argv[1];
	char* tamanio_proceso = argv[2];

	enviar_info_proceso(path_pseudocodigo, tamanio_proceso);

	// instante en el que "empieza" el proceso
	log_info(logger, "Agarro start");
	gettimeofday(&start, NULL);

	log_info(logger, "Aguardando Respuesta de Kernel...");

	respuesta_kernel = recv_mensajes_kernel(conexion);
<<<<<<< HEAD

	if (respuesta_kernel == TERMINO_EL_PROCESO) {
		log_info(logger, "Terminando la consola...");
		terminar_programa();
	}

	sleep(60);
	// instante en el que "termina" el proceso
	log_info(logger, "Agarro end");
	gettimeofday(&stop, NULL);

	// Diferencia entre los instantes, resultado en microsegundos
	unsigned long tiempo_ejecucion = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;

	log_info(logger, "Finalizo correctamente el programa");
	log_info(logger, "tiempo en microsegundos: %lu", tiempo_ejecucion);

	int cant_minutos = tiempo_ejecucion / 60000000;
	int cant_segundos = tiempo_ejecucion / 6000000;
	int cant_milisegundos = tiempo_ejecucion / 6000000000;

	log_info(logger, "Tiempo de ejecucion total: %d minutos %d segundos %d milisegundos",
			cant_minutos,
			cant_segundos,
			cant_milisegundos);

=======
	if (respuesta_kernel == PAQUETE_KERNEL_EXIT) {
		log_info(logger, "Finalizo correctamente el programa");
		terminar_programa();
	}
	else {
		log_info(logger, "Error: Terminando la consola...");
		terminar_programa();
	}
>>>>>>> 6b08867dcc9bcd4a6fb98c6c22dff4421e21142c
	return EXIT_SUCCESS;
}

void enviar_info_proceso(char* path_pseudocodigo, char* tamanio_proceso){
	// arma y envia el paquete con info del proceso
	paquete(path_pseudocodigo, tamanio_proceso);
}

void paquete(char* path_pseudocodigo, char* tamanio_proceso)
{
	t_paquete* paquete = crear_paquete(PAQUETE_CONSOLA);

	int tamanio;

	// creo un puntero para leer el script
	FILE* file;
	file = abrir_archivo_lectura(path_pseudocodigo);

	// en leido se almacena cada linea leida del script de codigo
	char buffer[1024];

	// primero agrego el "tamanio_proceso" al paquete
	tamanio = strlen(tamanio_proceso) + 1;
	agregar_a_paquete(paquete, tamanio_proceso, tamanio);

	// luego se agregan todas las lineas del archivo al paquete
	fgets(buffer,sizeof(buffer),file);
	while(!feof(file)){

		tamanio = strlen(buffer) + 1;
		agregar_a_paquete(paquete, buffer, tamanio);

		fgets(buffer,sizeof(buffer),file);
	}

	tamanio = strlen(buffer) + 1;
	agregar_a_paquete(paquete, buffer, tamanio);

	fclose(file);

	// se envia el paquete a kernel con el tamanio y las instrucciones del proceso
	enviar_paquete(paquete, conexion);

	// libera la memoria ocupada por el proceso
	eliminar_paquete(paquete);
}



int recv_mensajes_kernel(int socket_kernel){

	t_list* lista; // @suppress("Type cannot be resolved")

	op_code cod_op;
	cod_op = recibir_operacion(socket_kernel);

	switch (cod_op) {

	case PAQUETE_KERNEL_EXIT:
		log_info(logger, TERMINO_EL_PROCESO);
		break;
	case -1:
		log_warning(logger, SERVIDOR_DESCONEXION);
		return NULL;
	default:
		log_warning(logger,OPERACION_DESCONOCIDA);
		break;
	}

	return cod_op;

}


void terminar_programa()
{
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	close(conexion);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);
}
