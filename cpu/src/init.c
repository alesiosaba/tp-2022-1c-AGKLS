#include "../include/init.h"

void inicializar(char* config_path){

	// leo la configuracion del cpu
	config = iniciar_config(config_path);
	config_values = leer_config();

	// inicio el logger del cpu
	/*
	char* logger_name = string_new();
	char timestamp_string[18];
    time_t    caltime;
    struct tm * broketime;
    // find current time, convert to broken-down time
    time(&caltime);
    broketime = localtime(&caltime);
    // append timestamp in the format "_yymmdd_hhmmss"
    strftime(timestamp_string,18,"_%d-%m-%y_%H%M%S",broketime);
	string_append(&logger_name, ARCHIVO_DE_LOG);
	string_append(&logger_name, timestamp_string);
	string_append(&logger_name, ".log");
	logger = iniciar_logger(config_values.log_level,logger_name,LOGGER);
	*/

	logger = iniciar_logger(config_values.log_level,ARCHIVO_DE_LOG,LOGGER);

	// globales
	desactivar_flag_desalojo();
	desactivar_flag_interrupcion();

	// ID de proceso ejecutado en el CPU
	procesoAnterior = -1;

	// Entradas TLB
	TLB = list_create();

	// semaforos mutex
	pthread_mutex_init(&mtx_gv_flag_desalojar_proceso, NULL);
	pthread_mutex_init(&mtx_gv_flag_interrupcion, NULL);

	log_info(logger, "Archivo de configuracion:");
	log_info(logger, "LOG_LEVEL: %s", config_values.log_level);
	log_info(logger, "entradas_TLB: %s", config_values.entradas_TLB);
	log_info(logger, "reemplazo_TLB: %s", config_values.reemplazo_TLB);
	log_info(logger, "retardo_NOOP: %s", config_values.retardo_NOOP);
	log_info(logger, "ip_memoria: %s", config_values.IP_memoria);
	log_info(logger, "puerto_memoria: %s", config_values.puerto_memoria);
	log_info(logger, "puerto_escucha_dispatch: %s", config_values.puerto_escucha_dispatch);
	log_info(logger, "puerto_escucha_interrupt: %s", config_values.puerto_escucha_interrupt);
	log_info(logger, "ip_escucha: %s\n", config_values.ip_escucha);
}


config_t leer_config(){

	config_t config_values;

	config_values.log_level = config_get_string_value(config, "LOG_LEVEL");
	config_values.entradas_TLB = config_get_string_value(config, "ENTRADAS_TLB");
	config_values.reemplazo_TLB = config_get_string_value(config, "REEMPLAZO_TLB");
	config_values.retardo_NOOP = config_get_string_value(config, "RETARDO_NOOP");
	config_values.IP_memoria = config_get_string_value(config, "IP_MEMORIA");
	config_values.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	config_values.puerto_escucha_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
	config_values.puerto_escucha_interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
	config_values.ip_escucha = config_get_string_value(config, "IP_ESCUCHA");

	return config_values;

}
