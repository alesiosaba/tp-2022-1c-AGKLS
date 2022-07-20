#include "../include/main.h"

nodo_readyQueue* readyQueue = NULL;

//Finalizacion por interrupción cntrl + c
void sighandler(int s){
	if(conexionACPU){
		liberar_conexion(&conexionACPU);
	}

	if(conexionACPU_interrupt){
		liberar_conexion(&conexionACPU_interrupt);
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
	conectar_memoria(); //todo
	conectar_cpu();
<<<<<<< HEAD
=======
	conectar_memoria();

	log_info(logger, SERVIDOR_LISTO);
	
>>>>>>> 9dbf26a45ea18a152f5fe8dfa74492056f165985
	esperar_hilos();
	terminar_programa();
	return EXIT_SUCCESS;
}


void matar_hilos(){
	pthread_cancel(thr_consolas);
    pthread_cancel(thr_cpu);
    pthread_cancel(thr_cpu_interrupt);
    pthread_cancel(thr_memoria);
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
        pthread_join(thr_cpu_interrupt,NULL);
        pthread_join(thr_consolas, NULL);
        pthread_join(thr_memoria, NULL);
}


void terminar_programa()
{
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	liberar_conexion(&conexionACPU);
	liberar_conexion(&conexionACPU_interrupt);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);
}
