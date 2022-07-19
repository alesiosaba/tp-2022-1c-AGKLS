#ifndef INIT_H_
#define INIT_H_

#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/gestionPCB.h"

#define ARCHIVO_DE_CONFIGURACION "CPU.config"
#define ARCHIVO_DE_LOG "CPU.log"
#define LOGGER "CPU Log"


typedef struct config_t
{
	char*  entradas_TLB;
	char*  reemplazo_TLB;
	char*  retardo_NOOP;
	char*  IP_memoria;
	char*  puerto_memoria;
	char*  puerto_escucha_dispatch;
	char*  puerto_escucha_interrupt;

} config_t;
config_t config_values;

// se utilizan para abrir un hilo para escuchar por el puerto dispatch
int serverDispatch;
int clienteDispatch;
// se utilizan para abrir un hilo para escuchar por el puerto interrupt
int serverInterrupt;
int clienteInterrupt;

int conexionAMemoria;

void inicializar();
config_t leer_config();

#endif
