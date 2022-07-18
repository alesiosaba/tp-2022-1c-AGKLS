#include "../include/init.h"

void inicializar(){

	// inicio el logger del cpu
	logger = iniciar_logger("DEBUG",ARCHIVO_DE_LOG,LOGGER);
	log_info(logger, "Log de CPU iniciado");

	//leo la configuracion del cpu
	config = iniciar_config(ARCHIVO_DE_CONFIGURACION);
	log_info(logger, "Leyendo config");
	config_values = leer_config();
	log_info(logger, "Lei la siguiente config: \n %s \n %s \n %s \n %s \n %s \n %s \n %s \n",config_values.entradas_TLB,config_values.reemplazo_TLB,config_values.retardo_NOOP,config_values.IP_memoria,config_values.puerto_memoria,config_values.puerto_escucha_dispatch,config_values.puerto_escucha_interrupt);

}


config_t leer_config(){

	config_t config_values;

	config_values.entradas_TLB = config_get_string_value(config, "ENTRADAS_TLB");
	config_values.reemplazo_TLB = config_get_string_value(config, "REEMPLAZO_TLB");
	config_values.retardo_NOOP = config_get_string_value(config, "RETARDO_NOOP");
	config_values.IP_memoria = config_get_string_value(config, "IP_MEMORIA");
	config_values.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	config_values.puerto_escucha_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
	config_values.puerto_escucha_interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");

	return config_values;

}
