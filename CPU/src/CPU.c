#include "../include/CPU.h"




int main(void) {

	inicializar();
	conexiones();


//	cicloDeInstruccion();


//	terminar_programa(conexion, logger, config);
	terminar_programa(logger, config);
	return EXIT_SUCCESS;
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



