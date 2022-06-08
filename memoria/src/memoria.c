/*
 ============================================================================
 Name        : memoria.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/memoria.h"

int main(void) {
	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	inicializar();
	terminar_programa(logger, config);
	return EXIT_SUCCESS;
}

void terminar_programa()
{
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	close(conexion);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);
}
