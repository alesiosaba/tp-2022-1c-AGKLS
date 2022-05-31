#include "../include/init.h"
#include "../../shared/include/utils/utils.h"

config_t config_values;
t_log* loggerCPU;
t_config* configCPU;

void inicializar(){

	// inicio el logger del cpu
	loggerCPU = iniciar_logger("DEBUG","CPU.log","CPU LOG");
	log_info(loggerCPU, "Log de CPU iniciado");

	/*
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
	*/
}


config_t leer_config(){

	config_t config_values;

	config_values.entradas_TLB = config_get_string_value(configCPU, "ENTRADAS_TLB");
	//config_values.entradas_TLB = config_get_int_value(configCPU, "ENTRADAS_TLB");
	config_values.reemplazo_TLB = config_get_string_value(configCPU, "REEMPLAZO_TLB");
	config_values.retardo_NOOP = config_get_string_value(configCPU, "RETARDO_NOOP");
	//config_values.retardo_NOOP = config_get_int_value(configCPU, "RETARDO_NOOP");
	config_values.IP_memoria = config_get_string_value(configCPU, "	IP_MEMORIA");
	config_values.puerto_memoria = config_get_string_value(configCPU, "PUERTO_MEMORIA");
	config_values.puerto_escucha_dispatch = config_get_string_value(configCPU, "PUERTO_ESCUCHA_DISPATCH");
	config_values.puerto_escucha_interrupt = config_get_string_value(configCPU, "PUERTO_ESCUCHA_INTERRUPT");


	return config_values;

}
