/*
 * init.h
 *
 *  Created on: 6 jun. 2022
 *      Author: utnso
 */

#ifndef INIT_H_
#define INIT_H_

#define ARCHIVO_DE_CONFIGURACION "memoria.config"
#define ARCHIVO_DE_LOG "memoria.log"
#define LOGGER "Memoria Log"


typedef struct config_t
{
	char* puerto_escucha;
	char* tam_memoria;
	char* tam_pagina;
	char* entradas_por_tabla;
	char* retardo_memoria;
	char* algoritmo_reemplazo;
	char* marcos_por_proceso;
	char* retardo_swap;
	char* path_swap;
} config_t;

config_t config_values;

void inicializar();
config_t leer_config();



#endif /* INIT_H_ */
