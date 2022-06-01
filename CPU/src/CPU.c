#include "../include/CPU.h"




int main(void) {

	inicializar();
	conexiones();

//	terminar_programa(conexion, logger, config);
	terminar_programa(logger, config);
	return EXIT_SUCCESS;
}


