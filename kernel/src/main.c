#include "../include/main.h"

//Finalizacion por interrupción cntrl + c
void sighandler(int s){
	matar_hilos();
	terminar_programa();
	exit(0);
}

int main(void) {
	signal(SIGINT, sighandler); //Terminar el programa al oprimir ctrl + C en terminal
	inicializar();
	imprimir_config();
	pthread_create(&thr_comandos, NULL, (void*) &recibirComandos, NULL);
	pthread_detach(&thr_comandos);
	log_debug(logger, "se creo un thread para %s", "comandos");
	servidor_procesos();
	iniciarPlanificacion();
	conectar_cpu();
	conectar_memoria();
	esperar_hilos();
	terminar_programa();
	return EXIT_SUCCESS;
}


void matar_hilos(){
	pthread_cancel(thr_consolas);
    pthread_cancel(thr_cpu);
    pthread_cancel(thr_memoria);
    pthread_cancel(thr_planifLT);
    pthread_cancel(thr_planifST);
    pthread_cancel(thr_bloqueos);
    pthread_cancel(thr_suspended);
}

void recibirComandos(){
	while (1){
		char* leido = readline(">");
		char** split = string_split(leido, " ");
		if (string_equals_ignore_case(split[0], "exit"))
		{
			free(leido);
			liberarStringArray(split);
			matar_hilos();
			break;
		}
	}

}


void esperar_hilos() {
		pthread_join(thr_consolas, NULL);
        pthread_join(thr_cpu,NULL);
        pthread_join(thr_memoria, NULL);
        pthread_join(thr_planifST, NULL);
        pthread_join(thr_planifLT, NULL);
        pthread_join(thr_bloqueos, NULL);
        pthread_join(thr_suspended, NULL);
}


void terminar_programa()
{
	//conexiones
	liberar_conexion(&conexionACPU);
	liberar_conexion(&conexionACPU_interrupt);
	liberar_conexion(&conexionAMemoria);
	log_debug(logger,CONEXION_CERRADA);
	//server
	close(server_fd);
	//Semaforos
	sem_destroy(&sem_ProcesosNew);
	sem_destroy(&sem_enviarPCB);
	sem_destroy(&sem_comenzarProcesos);
	sem_destroy(&sem_ProcesosReady);
	sem_destroy(&sem_respuestaPCB);
	sem_destroy(&sem_multiprogramacion);
	sem_destroy(&sem_ProcesosSuspended);
	sem_destroy(&sem_ProcesosSuspendedBlk);
	log_debug(logger,SEMAFOROS_DESTRUIDOS);
	//config
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	//log
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);

}
