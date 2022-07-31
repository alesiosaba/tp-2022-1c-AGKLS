#include "../include/main.h"

//Finalizacion por interrupción ctrl + c
void sighandler(int s){
	if(clienteMemoriaCPU || clienteMemoriaKernel){
		liberar_conexion(&clienteMemoriaCPU);
		liberar_conexion(&clienteMemoriaKernel);
	}
	terminar_programa();
	exit(0);
}

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Cantidad erronea de parametros.\n");
        return EXIT_FAILURE;
    }

	signal(SIGINT, sighandler); //Terminar el programa al oprimir ctrl + C en terminal

	char* config_path = argv[1];

	// Leer config y arrancar logger
	inicializar(config_path);

	inicializar_estructuras();

	// abro un hilo para atender por el puerto de escucha al CPU
	pthread_create(&thr_memoriaCPU, NULL, (void*) &iniciar_servidor_memoriaCPU, NULL);


	// abro un hilo para atender por el puerto de escucha al Kernel
	pthread_create(&thr_memoriaKernel, NULL, (void*) &iniciar_servidor_memoriaKernel, NULL);
	
	// Hilo SWAP
    pthread_create(&thr_swap, NULL, (void*) &gestionar_solicitudes_swap, NULL);

    esperar_hilos();

	terminar_programa();

	return EXIT_SUCCESS;
}

void esperar_hilos(){
	pthread_join(thr_memoriaCPU, NULL);
	pthread_join(thr_memoriaKernel, NULL);
	pthread_join(thr_swap, NULL);
}

void matar_hilos(){
	pthread_cancel(thr_memoriaCPU);
	pthread_cancel(thr_memoriaKernel);
	pthread_cancel(thr_swap);
}

void terminar_programa()
{

	matar_hilos();
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	if(clienteMemoriaCPU || clienteMemoriaKernel){
			liberar_conexion(&clienteMemoriaCPU);
			liberar_conexion(&clienteMemoriaKernel);
	}
	close(serverMemoriaCPU);
	close(serverMemoriaKernel);
	log_debug(logger,TERMINANDO_EL_LOG);
	liberar_estructuras();
}
