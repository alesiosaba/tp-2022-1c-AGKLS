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

	int valorMemoria = 0;
	if (esCopy)
		valorMemoria = fetch_operands(instruccion);
	if (esCopy)
		log_info(logger, "%d",valorMemoria);

	// ejecucion de instruccion
	execute(instruccion,pcb,valorMemoria);


	// chequear si el Kernel nos envió una interrupcion
	check_interrupt();

}
		
// buscar la próxima instrucción a ejecutar
nodo_instruccion* fetch(pcb** pcb){

	nodo_instruccion* instruccion;
	//instruccion = list_get((*pcb)->instrucciones,(*pcb)->program_counter);
	instruccion = list_get((*pcb)->instrucciones,7); //para pruebas
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
void execute(nodo_instruccion* instruccion,pcb** pcb, int valorMemoria){
	switch (instruccion->instruccion.identificador[0]){
				case 'N': // NO_OP

					log_info(logger, "CASE: entro en NO_OP");
					msleep(atoi(config_values.retardo_NOOP));
					log_info(logger, "me desperte");

				  break;

				case 'R': // READ
					log_info(logger, "CASE: entro en READ");
					int valor = buscarValorEnMemoria(list_get(instruccion->instruccion.parametros,0));
					log_info(logger, "Valor leido %d",valor);
				  break;

				case 'W': // Write
					log_info(logger, "CASE: entro en WRITE");
					nodo_parametro* valorAescribir = list_get(instruccion->instruccion.parametros,1);
					escribirValorEnMemoria(list_get(instruccion->instruccion.parametros,0),valorAescribir->parametro);
					log_info(logger, "Valor escrito en memoria %d",valorAescribir->parametro);
				  break;

				case 'C': // COPY
					log_info(logger, "CASE: entro en COPY");
					escribirValorEnMemoria(list_get(instruccion->instruccion.parametros,0),valorMemoria);
					log_info(logger, "Valor escrito en memoria %d",valorMemoria);
				  break;

				case 'I': // I/O
					log_info(logger, "CASE: entro en I/0");
					nodo_parametro* tiempoDeBloqueo = list_get(instruccion->instruccion.parametros,0);
					(*pcb)->tiempo_a_bloquearse = tiempoDeBloqueo->parametro;
					log_info(logger, "Tiempo a bloquearse %d",(*pcb)->tiempo_a_bloquearse);
					imprimir_PCB(*pcb);
					send_paquete_pcb(clienteDispatch,*pcb,PAQUETE_PCB_IO);

				  break;

				case 'E': // EXIT
					log_info(logger, "CASE: entro en EXIT");
					send_paquete_pcb(clienteDispatch,pcb,PAQUETE_PCB_EXIT);

				  break;
			}
}
// chequear si el Kernel nos envió una interrupcion
void check_interrupt(){

}

int buscarValorEnMemoria(int* direccionLogica){
	//TODO
	return 14;
}

int escribirValorEnMemoria(int* direccionLogica, int valor){
	//TODO
	return 322;
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
