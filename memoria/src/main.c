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

int main(void) {

	signal(SIGINT, sighandler); //Terminar el programa al oprimir ctrl + C en terminal

	// Leer config y arrancar logger
	inicializar();

	inicializar_estructuras();

	// abro un hilo para atender por el puerto de escucha al CPU
	pthread_create(&thr_memoriaCPU, NULL, (void*) &iniciar_servidor_memoriaCPU, NULL);


	// abro un hilo para atender por el puerto de escucha al Kernel
	pthread_create(&thr_memoriaKernel, NULL, (void*) &iniciar_servidor_memoriaKernel, NULL);
	
	// Hilo SWAP
    pthread_create(&thr_swap, NULL, (void*) &gestionar_solicitudes_swap, NULL);

	terminar_programa();

	return EXIT_SUCCESS;
}

void terminar_programa()
{

	pthread_join(thr_memoriaCPU, NULL);
	pthread_join(thr_memoriaKernel, NULL);

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
