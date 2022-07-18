#ifndef INIT_H_
#define INIT_H_

#include "../include/globals.h"

#define ARCHIVO_DE_CONFIGURACION "kernel.config"
#define ARCHIVO_DE_LOG "kernel.log"
#define LOGGER "Kernel Log"

config_t inicializar();
config_t leer_config();

#endif
