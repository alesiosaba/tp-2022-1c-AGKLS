#include "../include/init.h"


void inicializar(char* config_path){

	// leo la configuracion
	config = iniciar_config(config_path);
	config_values = leer_config();

	// inicio el logger
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
	// logger = iniciar_logger(config_values.log_level,logger_name,LOGGER);
	*/
	logger = iniciar_logger(config_values.log_level,ARCHIVO_DE_LOG,LOGGER);
	log_info(logger, "Log de Memoria iniciado");

	log_info(logger, "Archivo de configuracion:");
	log_info(logger, "LOG_LEVEL: %s", config_values.log_level);
	log_info(logger, "puerto_escucha_kernel: %s", config_values.puerto_escucha_kernel);
	log_info(logger, "puerto_escucha_CPU: %s", config_values.puerto_escucha_CPU);
	log_info(logger, "tam_memoria: %d", config_values.tam_memoria);
	log_info(logger, "tam_pagina: %d", config_values.tam_pagina);
	log_info(logger, "entradas_por_tabla: %d", config_values.entradas_por_tabla);
	log_info(logger, "retardo_memoria: %d", config_values.retardo_memoria);
	log_info(logger, "algoritmo_reemplazo: %s", config_values.algoritmo_reemplazo);
	log_info(logger, "marcos_por_proceso: %d", config_values.marcos_por_proceso);
	log_info(logger, "retardo_swap: %d", config_values.retardo_swap);
	log_info(logger, "path_swap: %s", config_values.path_swap);
	log_info(logger, "ip_escucha: %s\n", config_values.ip_escucha);

	//inicio mutex
    pthread_mutex_init(&mutex_tablasN1,NULL);
    pthread_mutex_init(&mutex_tablasN2,NULL);
    pthread_mutex_init(&mutex_procesos_en_memoria, NULL);
    pthread_mutex_init(&mutex_accesos_swap, NULL);

    ACCESOS_A_SWAP_GLOBALES = 0;
}


config_t leer_config(){

	config_t config_values;

	config_values.log_level = config_get_string_value(config, "LOG_LEVEL");
	config_values.puerto_escucha_kernel = config_get_string_value(config, "PUERTO_ESCUCHA_KERNEL");
	config_values.puerto_escucha_CPU = config_get_string_value(config, "PUERTO_ESCUCHA_CPU");
	config_values.tam_memoria = config_get_int_value(config, "TAM_MEMORIA");
	config_values.tam_pagina = config_get_int_value(config, "TAM_PAGINA");
	config_values.entradas_por_tabla = config_get_int_value(config, "ENTRADAS_POR_TABLA");
	config_values.retardo_memoria = config_get_int_value(config, "RETARDO_MEMORIA");
	config_values.algoritmo_reemplazo = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	config_values.marcos_por_proceso = config_get_int_value(config, "MARCOS_POR_PROCESO");
	config_values.retardo_swap = config_get_int_value(config, "RETARDO_SWAP");
	config_values.path_swap = config_get_string_value(config, "PATH_SWAP");
	config_values.ip_escucha = config_get_string_value(config, "IP_ESCUCHA");


	return config_values;

}
