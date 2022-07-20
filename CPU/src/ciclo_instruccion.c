#include "../include/ciclo_instruccion.h"

void ejecutar_ciclo_instruccion(pcb** pcb){
	log_debug(logger, COMIENZO_CICLO_INSTRUCCION, (*pcb)->id);

	log_debug(logger, "Tiempo de bloqueo: %d", (*pcb)->tiempo_a_bloquearse);
	(*pcb)->tiempo_a_bloquearse = 99;
	log_debug(logger, "Tiempo de bloqueo: %d", (*pcb)->tiempo_a_bloquearse);

	// buscar la próxima instrucción a ejecutar
	fetch();
	// interpretar qué instrucción es la que se va a ejecutar (solo COPY ejecuta fetch_operands)
	decode();
	// buscar valor en memoria del parametro de COPY
	fetch_operands();
	// ejecucion de instruccion
	execute();
	// chequear si el Kernel nos envió una interrupcion
	check_interrupt();

}

// buscar la próxima instrucción a ejecutar
void fetch(){

}
// interpretar qué instrucción es la que se va a ejecutar (evalua si ejecuta fetch_operands y como)
void decode(){

}
// buscar valor en memoria del parametro de COPY
void fetch_operands(){

}
// ejecucion de instruccion
void execute(){

}
// chequear si el Kernel nos envió una interrupcion
void check_interrupt(){

}

/*void cicloDeInstruccion (){

	//representacion del pcb
	pcb PCB;
	instruccion instruccion;
	int interrupcion = 0;
	do {
		//fetch();

		//hablar con leo y ale. Cuando crean el PCB el PC no es NULL,es la primera es = a la primera instruccion
		// tambien hablar  sobre la interpretacion literal de lo q dice el enunciado y lo realmente hace nuestro PC
		// instruccion = PCB.program_counter->instruccion;
		// PCB.program_counter = PCB.program_counter->sig;

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
