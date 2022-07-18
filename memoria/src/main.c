#include "../include/init.h"
#include "../include/main.h"
#include "../include/conexiones.h"

int main(void) {
	// Leer config y arrancar logger
	inicializar();

	// Iniciamos puerto escucha de memoria
	iniciar_servidor_memoria();

	while(1){
		manejarConexion(socket_cliente);
	}

	terminar_programa(logger, config);
	return EXIT_SUCCESS;
}

void terminar_programa()
{
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	close(socket_cliente);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);
}
