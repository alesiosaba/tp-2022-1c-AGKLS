#include "../include/main.h"


int main(void) {

	config_t config_values = inicializar();

	//Servidor
	int server_fd = iniciar_servidor(config_values.puerto_escucha);

	pthread_t consolas;
	pthread_create(&consolas, NULL, (void*) manejar_consolas,(void*)server_fd);
	pthread_detach(consolas);


	log_info(logger, SERVIDOR_LISTO);

	while (1){
		sleep(1);

		char* leido = readline(">");
		char** split = string_split(leido, " ");
		if (string_equals_ignore_case(split[0], "exit"))
		{
			free(leido);
			liberarStringArray(split);
			break;
		}

	}

	terminar_programa();
	return EXIT_SUCCESS;
}

void terminar_programa()
{
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
//	close(conexion);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);
}


void iterator(char* value) {
	log_info(logger,"%s", value);
}

void manejar_consolas(int server_fd){

	while(1){

		int socketCliente = esperar_cliente(server_fd);

		pthread_t t;
		pthread_create(&t, NULL, (void*) manejarConexion, (void*)socketCliente);
		pthread_detach(t);
	}

}

void armar_PCB(t_list* lista){

	t_pcb* pcb = (struct t_pcb*)malloc(sizeof(struct t_pcb));
	pcb->instrucciones = NULL;
	pcb->program_counter = NULL;

	// agarro el primer elemento (tamanio del proceso)
	t_list* tamanio_proceso = list_remove(lista, 0);
	pcb->tamanio = atoi(tamanio_proceso);

	// armo la lista de instrucciones y la guardo en el PCB
	pcb->instrucciones = armar_lista_instrucciones(lista);

	mostrar_lista(pcb->instrucciones);
}

int manejarConexion(int socket_cliente){

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, LECTURA_DE_VALORES);
			list_iterate(lista, (void*) iterator);
			break;
		case PAQUETE_CONSOLA:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, RECEPCION_PAQUETE_CONSOLA);
			armar_PCB(lista);
			log_info(logger, "Se armó un PCB correctamente.");
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

