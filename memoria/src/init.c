#include "../include/init.h"
#include "../../shared/include/utils/utils.h"


void inicializar(){
//Logs y Config

	// inicio el logger
	logger = iniciar_logger("DEBUG",ARCHIVO_DE_LOG,LOGGER);
	log_info(logger, "Log de Memoria iniciado");

	//leo la configuracion
	config = iniciar_config(ARCHIVO_DE_CONFIGURACION);
	log_info(logger, "Leyendo config");
	config_values = leer_config();
	log_info(logger, "Lei la siguiente config: \n %s \n %s \n %s \n %s \n %s \n %s \n %s \n",config_values.puerto_escucha,config_values.tam_memoria,config_values.tam_pagina,config_values.entradas_por_tabla,config_values.retardo_memoria,config_values.algoritmo_reemplazo,config_values.marcos_por_proceso,config_values.retardo_swap,config_values.path_swap);

}


config_t leer_config(){

	config_t config_values;

	config_values.puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
	config_values.tam_memoria = config_get_string_value(config, "TAM_MEMORIA");
	config_values.tam_pagina = config_get_string_value(config, "TAM_PAGINA");
	config_values.entradas_por_tabla = config_get_string_value(config, "ENTRADAS_POR_TABLA");
	config_values.retardo_memoria = config_get_string_value(config, "RETARDO_MEMORIA");
	config_values.algoritmo_reemplazo = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	config_values.marcos_por_proceso = config_get_string_value(config, "MARCOS_POR_PROCESO");
	config_values.retardo_swap = config_get_string_value(config, "RETARDO_SWAP");
	config_values.path_swap = config_get_string_value(config, "PATH_SWAP");


	return config_values;

}
