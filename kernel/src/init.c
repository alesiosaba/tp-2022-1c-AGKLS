#include "../include/init.h"
#include "../../shared/include/utils/utils.h"


void inicializar(){
//Logs y Config
	char* log_level;
	config_t config_values;

	config = iniciar_config(ARCHIVO_DE_CONFIGURACION);
	config_values = leer_config();
	log_level = config_values.log_level;
	logger = iniciar_logger(log_level, ARCHIVO_DE_LOG, LOGGER);


}


config_t leer_config(){

	config_t config_values;

	config_values.log_level = config_get_string_value(config, "LOG_LEVEL");
	config_values.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
	config_values.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	config_values.ip_cpu = config_get_string_value(config, "IP_CPU");
	config_values.puerto_cpu_dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
	config_values.puerto_cpu_interrupt= config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
	config_values.puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
	config_values.algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	config_values.estimacion_inicial = config_get_double_value(config, "ESTIMACION_INICIAL");
	config_values.alfa = config_get_double_value(config, "ALFA");
	config_values.grado_multiprog = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
	config_values.t_bloqueo_max = config_get_int_value(config, "TIEMPO_MAXIMO_BLOQUEADO");

	return config_values;

}
