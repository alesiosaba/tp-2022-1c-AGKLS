#ifndef INIT_H_
#define INIT_H_

#include "globals.h"

#define ARCHIVO_DE_LOG "./../logs/CPU"
#define LOGGER "CPU Log"


typedef struct config_t
{
	char*  log_level;
	char*  entradas_TLB;
	char*  reemplazo_TLB;
	char*  retardo_NOOP;
	char*  IP_memoria;
	char*  puerto_memoria;
	char*  puerto_escucha_dispatch;
	char*  puerto_escucha_interrupt;
	char* ip_escucha;

} config_t;
config_t config_values;

void inicializar(char* config_path);
config_t leer_config();

#endif
