#include "../include/init.h"
#include "../../shared/include/utils/utils.h"


void inicializar(){
//Logs y Config
	char* log_level;
	config_t config_values;
	char* ip;
	char* puerto;

	config = iniciar_config(ARCHIVO_DE_CONFIGURACION);
	config_values = leer_config();

	log_level = config_values.log_level;
	logger = iniciar_logger(log_level, ARCHIVO_DE_LOG, LOGGER);

	log_info(logger, "Se generó correctamente el logger");

	log_info(logger, "Se generó correctamente la configuracion");

	ip = config_get_string_value(config, "IP_KERNEL");
	puerto = config_get_string_value(config, "PUERTO_KERNEL");

	// se crea la conexion con el modulo kernel
	conexion = crear_conexion(ip, puerto);

	log_info(logger, "Se generó correctamente la conexion");

	log_info(logger, ip);
	log_info(logger, puerto);
}


config_t leer_config(){

	config_t config_values;

	config_values.log_level = config_get_string_value(config, "LOG_LEVEL");
	config_values.ip_kernel = config_get_string_value(config, "IP_KERNEL");
    config_values.puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

	return config_values;

}
