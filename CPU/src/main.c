#include "../include/main.h"
#include "../include/comunicacion.h"
#include "../include/globals.h"

int main(void) {

	signal(SIGINT, sighandler); //Terminar el programa al oprimir ctrl + C en terminal

	inicializar();

	// aca se maneja la conexion dispatch, interrupt y con memoria
	conexiones();

	realizar_handshake_inicial();

	terminar_programa(logger, config);

	return EXIT_SUCCESS;
}


//Finalizacion por interrupción ctrl + c
void sighandler(int s){

	if(conexionAMemoria){
		liberar_conexion(&conexionAMemoria);
	}

	terminar_programa();
	exit(0);
}

void terminar_programa(){
	pthread_join(thr_dispatch, NULL);
	pthread_join(thr_interrupt, NULL);
	pthread_join(thr_memoria, NULL);
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	liberar_conexion(&conexionAMemoria);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);

}
