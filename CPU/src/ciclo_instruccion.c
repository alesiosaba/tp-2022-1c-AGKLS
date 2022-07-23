#include "../include/ciclo_instruccion.h"

void ejecutar_ciclo_instruccion(pcb** pcb){
	log_debug(logger, COMIENZO_CICLO_INSTRUCCION, (*pcb)->id);



	// buscar la próxima instrucción a ejecutar

	nodo_instruccion* instruccion = fetch(pcb);
	printf("%s",instruccion->instruccion.identificador);
	int x = 0;
	        while(x < list_size(instruccion->instruccion.parametros)){
	            nodo_parametro* nodo_parametro = list_get(instruccion->instruccion.parametros, x);
	            printf("\t\tparametro: %d\n", nodo_parametro->parametro);

	            x++;
	        }

	// interpretar qué instrucción es la que se va a ejecutar (solo COPY ejecuta fetch_operands)
	int esCopy = decode(instruccion);
	// buscar valor en memoria del parametro de COPY
	log_info(logger, "%d",esCopy);
	int valorMemoria = 0;
	if (esCopy)
		valorMemoria = fetch_operands(instruccion);
	if (esCopy)
		log_info(logger, "%d",valorMemoria);

	// ejecucion de instruccion
	execute(instruccion);


	// chequear si el Kernel nos envió una interrupcion
	check_interrupt();

}
		
// buscar la próxima instrucción a ejecutar
nodo_instruccion* fetch(pcb** pcb){

	nodo_instruccion* instruccion;
	//instruccion = list_get((*pcb)->instrucciones,(*pcb)->program_counter);
	instruccion = list_get((*pcb)->instrucciones,7);
	(*pcb)->program_counter++;
return instruccion;
}

// interpretar qué instrucción es la que se va a ejecutar (evalua si ejecuta fetch_operands y como)
int decode(nodo_instruccion* instruccion){
	return (strcmp(instruccion->instruccion.identificador, "COPY")== 0);
}

// buscar valor en memoria del parametro de COPY
int fetch_operands(nodo_instruccion* instruccion){
log_info(logger, "entro al fetch operands");

return buscarValorEnMemoria(list_get(instruccion->instruccion.parametros,1));
}

// ejecucion de instruccion
void execute(nodo_instruccion* instruccion){

}
// chequear si el Kernel nos envió una interrupcion
void check_interrupt(){

}

int buscarValorEnMemoria(int* direccionLogica){
	return 14;
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
