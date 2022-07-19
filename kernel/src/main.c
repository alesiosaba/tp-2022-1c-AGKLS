#include "../include/main.h"

nodo_readyQueue* readyQueue = NULL;

//Finalizacion por interrupción cntrl + c
void sighandler(int s){
	if(conexionACPU){
		liberar_conexion(&conexionACPU);
	}
	terminar_programa();
	exit(0);
}

int main(void) {

	signal(SIGINT, sighandler); //Terminar el programa al oprimir ctrl + C en terminal
	inicializar();
	pthread_create(&thr_comandos, NULL, (void*) &recibirComandos, NULL);
	pthread_detach(&thr_comandos);
	servidor_procesos();
	conectar_cpu();

	log_info(logger, SERVIDOR_LISTO);

	enviar_mensaje("Soy un mensaje de kernel a CPU por Dispatch.", conexionACPU);
	enviar_mensaje("Soy un mensaje de kernel a CPU por Interrupt.", conexionACPU_interrupt);

	esperar_hilos();

	terminar_programa();
	return EXIT_SUCCESS;
}


void matar_hilos(){
	pthread_cancel(thr_consolas);
    pthread_cancel(thr_cpu);


}

void recibirComandos(){
	while (1){
		sleep(1);
		char* leido = readline(">");
		char** split = string_split(leido, " ");
		if (string_equals_ignore_case(split[0], "exit"))
		{
			free(leido);
			liberarStringArray(split);
			break;
		}
	}
	matar_hilos();
}


void esperar_hilos() {
        pthread_join(thr_cpu,NULL);
        pthread_join(thr_consolas, NULL);

}


void terminar_programa()
{
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	liberar_conexion(&server_fd);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);
}
