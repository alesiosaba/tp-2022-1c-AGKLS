#include "../include/main.h"
#include "../include/comunicacion.h"
#include "../include/globals.h"

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Cantidad erronea de parametros.\n");
        return EXIT_FAILURE;
    }

	signal(SIGINT, sighandler); //Terminar el programa al oprimir ctrl + C en terminal

	char* config_path = argv[1];

	inicializar(config_path);

	// aca se maneja la conexion dispatch, interrupt y con memoria
	conexiones();

	realizar_handshake_inicial();

	esperar_hilos();


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

void esperar_hilos(){
	pthread_join(thr_dispatch, NULL);
	pthread_join(thr_interrupt, NULL);
	pthread_join(thr_memoria, NULL);
}

void matar_hilos(){
	pthread_cancel(thr_dispatch);
	pthread_cancel(thr_interrupt);
	pthread_cancel(thr_memoria);
}

void terminar_programa(){
	matar_hilos();
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	liberar_conexion(&conexionAMemoria);
	close(serverDispatch);
	close(serverInterrupt);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);

}
