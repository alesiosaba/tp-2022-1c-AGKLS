#include "../include/main.h"
#include "../include/comunicacion.h"
#include "../include/globals.h"

int main(void) {

	signal(SIGINT, sighandler); //Terminar el programa al oprimir ctrl + C en terminal

	inicializar();

	conexiones();

//	cicloDeInstruccion();

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
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	liberar_conexion(&conexionAMemoria);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);

}

/*
void cicloDeInstruccion (){

	//representacion del pcb
	t_pcb PCB;
	instruccion instruccion;
	int interrupcion = 0;
	do {
		//fetch();

		//hablar con leo y ale. Cuando crean el PCB el PC no es NULL,es la primera es = a la primera instruccion
		// tambien hablar  sobre la interpretacion literal de lo q dice el enunciado y lo realmente hace nuestro PC
		instruccion = PCB.program_counter->instruccion;
		PCB.program_counter = PCB.program_counter->sig;

		//decode();
			//esto compara un char[10] con "copy". funca ? por q es un char 10 y no un char*
		if (strcmp(instruccion.identificador, "COPY")== 0)
		{
			//fetch_operands ();
		}

		//execute();
		switch (instruccion.identificador[0])
		​{
			case 'N': // NO_OP


				msleep(config_values.retardo_NOOP);


			  break;

			case 'I': // I/O

				//hablar sobre el tema tiempo de bloqueo

			  break;

			case 'E': // EXIT

				//devolver PCB al kernel

			  break;

		}

		//check interrupt
		interrupcion = checkInterrupt();
	}while(interrupcion != 1);

	//devolver PCB al kernel

}

int checkInterrupt(){
// TO DO

return 0;
}
*/
