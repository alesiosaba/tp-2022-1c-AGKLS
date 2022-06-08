#include "../include/main.h"

//Finalizacion por interrupción cntrl + c
void sighandler(int s){
	if(conexionACPU){
		liberar_conexion(&conexionACPU);
	}
	terminar_programa();
	exit(0);
}

int main(void) {

	signal(SIGINT, sighandler); //Terminar el programa al oprimir ctrl + C en terminal

	char* errorMessageAux = NULL;

	inicializar();

	conexionACPU = crear_conexion(IP,config_values.puerto_cpu_dispatch);

	if(conexionACPU == -1){
		strcpy(errorMessageAux, SERVIDOR_AUSENTE);
		strcat(errorMessageAux, " CPU");
		log_error(logger, errorMessageAux);
		terminar_programa();
	}

	//Servidor
	int server_fd = iniciar_servidor(IP, config_values.puerto_escucha);

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

	liberar_conexion(&conexionACPU);
	terminar_programa();
	return EXIT_SUCCESS;
}

void terminar_programa()
{
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	close(conexion);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);
}


void manejar_consolas(int server_fd){

	while(server_escuchar(logger, "Consola", server_fd));

}

