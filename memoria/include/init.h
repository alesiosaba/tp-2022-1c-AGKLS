#ifndef INIT_H_
#define INIT_H_

#define ARCHIVO_DE_LOG "./../logs/memoria"
#define LOGGER "Memoria Log"

#include "globals.h"
#include "../../shared/include/utils/utils.h"

void inicializar(char* config_path);
config_t leer_config();


#endif
