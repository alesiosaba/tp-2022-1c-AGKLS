#include "../include/init.h"


void inicializar(){
//Logs y Config

	// inicio el logger
	logger = iniciar_logger("DEBUG",ARCHIVO_DE_LOG,LOGGER);
	log_info(logger, "Log de Memoria iniciado");

	// leo la configuracion
	config = iniciar_config(ARCHIVO_DE_CONFIGURACION);
	log_info(logger, "Leyendo config...\n");
	config_values = leer_config();
	log_info(logger, "Archivo de configuracion:");
	log_info(logger, "puerto_escucha: %s", config_values.puerto_escucha);
	log_info(logger, "tam_memoria: %d", config_values.tam_memoria);
	log_info(logger, "tam_pagina: %d", config_values.tam_pagina);
	log_info(logger, "entradas_por_tabla: %d", config_values.entradas_por_tabla);
	log_info(logger, "retardo_memoria: %d", config_values.retardo_memoria);
	log_info(logger, "algoritmo_reemplazo: %s", config_values.algoritmo_reemplazo);
	log_info(logger, "marcos_por_proceso: %d", config_values.marcos_por_proceso);
	log_info(logger, "retardo_swap: %d", config_values.retardo_swap);
	log_info(logger, "path_swap: %s\n", config_values.path_swap);

	//inicio mutex
    pthread_mutex_init(&mutex_tablasN1,NULL);
    pthread_mutex_init(&mutex_tablasN2,NULL);
    pthread_mutex_init(&mutex_procesos_en_memoria, NULL);

}


config_t leer_config(){

	config_t config_values;

	config_values.puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
	config_values.tam_memoria = config_get_int_value(config, "TAM_MEMORIA");
	config_values.tam_pagina = config_get_int_value(config, "TAM_PAGINA");
	config_values.entradas_por_tabla = config_get_int_value(config, "ENTRADAS_POR_TABLA");
	config_values.retardo_memoria = config_get_int_value(config, "RETARDO_MEMORIA");
	config_values.algoritmo_reemplazo = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	config_values.marcos_por_proceso = config_get_int_value(config, "MARCOS_POR_PROCESO");
	config_values.retardo_swap = config_get_int_value(config, "RETARDO_SWAP");
	config_values.path_swap = config_get_string_value(config, "PATH_SWAP");


	return config_values;

}
