#include "../include/init.h"
#include "../include/main.h"

int main(void) {
	inicializar();

	getchar();

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
