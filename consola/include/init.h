#ifndef INIT_H_
#define INIT_H_


#define ARCHIVO_DE_CONFIGURACION "consola.config"
#define ARCHIVO_DE_LOG "consola.log"
#define LOGGER "Consola Log"


typedef struct config_t
{
	char*  log_level;
	char*  ip_kernel;
	char*  puerto_kernel;
} config_t;


void inicializar();
config_t leer_config();

#endif