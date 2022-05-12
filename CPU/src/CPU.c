
#include <stdio.h>
#include <stdlib.h>
#include "../../shared/include/utils/utils.h"

int main(void) {
	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	//inicio el logger del cpu
	logger = iniciar_logger();
	log_info(logger, "Log de CPU iniciado");

	//leo la configuracion del cpu
	config = iniciar_config();
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	log_info(logger, "Lei la IP %s y el puerto %s \n",ip,puerto);


	conexion = 0;

	terminar_programa(conexion, logger, config);

	return EXIT_SUCCESS;
}
/*
t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("CPU.log", "CPU", 1, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	if (( nuevo_config = config_create("CPU.config")) == NULL) {
		printf("No pude leer la config \n");
		exit(2);
	}

	return nuevo_config;
}
*/

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	// Libero lo que utilizamos (conexion, log y config)

	//TO DO
	//conexion

	if(logger != NULL) {
		log_destroy(logger);
		printf("\nlogger destruido \n");
	}
	if(config != NULL) {
		config_destroy(config);
		printf("config destruido \n");
	}
}

