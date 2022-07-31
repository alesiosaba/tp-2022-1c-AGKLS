#ifndef INIT_H_
#define INIT_H_

#include "../include/globals.h"

#define ARCHIVO_DE_LOG "./../logs/kernel"
#define LOGGER "Kernel Log"

config_t inicializar(char* config_path);
config_t leer_config();
void imprimir_config();

#endif
