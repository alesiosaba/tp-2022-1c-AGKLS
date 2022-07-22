#ifndef INIT_H_
#define INIT_H_

#define ARCHIVO_DE_CONFIGURACION "memoria.config"
#define ARCHIVO_DE_LOG "memoria.log"
#define LOGGER "Memoria Log"

#include "globals.h"
#include "../../shared/include/utils/utils.h"

void inicializar();
config_t leer_config();


#endif
