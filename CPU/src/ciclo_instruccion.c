#include "../include/ciclo_instruccion.h"


void ejecutar_ciclo_instruccion(pcb** pcb){

	// el ciclo de instruccion queda en loop hasta una I/O, EXIT o INTERRUPCION de kernel
	do {
		log_debug(logger, COMIENZO_CICLO_INSTRUCCION, (*pcb)->id);
		log_debug(logger, "PC = %d", (*pcb)->program_counter);

		desactivar_flag_desalojo();

		// buscar la próxima instrucción a ejecutar
		nodo_instruccion* instruccion = fetch(pcb);

		// imprimir instruccion a ejecutar por el ciclo de instruccion
		imprimir_instruccion(instruccion);

		// interpretar qué instrucción es la que se va a ejecutar (solo COPY ejecuta fetch_operands)
		log_debug(logger, COMIENZO_ETAPA_DECODE, (*pcb)->id);
		int esCopy = decode(instruccion);

		// buscar valor en memoria del parametro de COPY
		int valorLeidoDeMemoria = 0;
		if(esCopy){
			log_debug(logger, COMIENZO_ETAPA_FETCH_OPERANDS, (*pcb)->id);
			valorLeidoDeMemoria = fetch_operands(instruccion);
			log_debug(logger, "%d",valorLeidoDeMemoria);
		}

		// ejecucion de instruccion
		execute(instruccion, pcb, valorLeidoDeMemoria);

		// chequear si el Kernel nos envió una interrupcion
		check_interrupt(pcb);

		log_debug(logger, "gv_flag_desalojar_proceso: %d", gv_flag_desalojar_proceso);
		log_debug(logger, "gv_flag_interrupcion: %d", gv_flag_interrupcion);

		log_warning(logger, "Esperando getchar()");
		getchar();


	} while(!hay_desalojo_proceso());

	log_debug(logger, "Saliendo de ejecutar_ciclo_instruccion()");
}
		
void imprimir_instruccion(nodo_instruccion* instruccion) {
	log_debug(logger, "Instruccion a ejecutar: %s", instruccion->instruccion.identificador);
	int x = 0;
	while (x < list_size(instruccion->instruccion.parametros)) {
		nodo_parametro* nodo_parametro = list_get(
				instruccion->instruccion.parametros, x);
		log_debug(logger, "\t\tparametro: %d\n", nodo_parametro->parametro);
		x++;
	}
}

// buscar la próxima instrucción a ejecutar
nodo_instruccion* fetch(pcb** pcb){
	log_debug(logger, COMIENZO_ETAPA_FETCH, (*pcb)->id);

	nodo_instruccion* instruccion;
	instruccion = list_get((*pcb)->instrucciones,(*pcb)->program_counter);
	//para pruebas
	//instruccion = list_get((*pcb)->instrucciones,7);
	(*pcb)->program_counter++;
	return instruccion;
}

// interpretar qué instrucción es la que se va a ejecutar (evalua si ejecuta fetch_operands y como)
int decode(nodo_instruccion* instruccion){

	return (strcmp(instruccion->instruccion.identificador, "COPY") == 0);
}

// buscar valor en memoria del parametro de COPY
int fetch_operands(nodo_instruccion* instruccion){
	return buscarValorEnMemoria(list_get(instruccion->instruccion.parametros,1));
}

void exec_no_op(){
	log_debug(logger, "CASE de EXECUTE: entro en NO_OP");

	char* retardo_config = config_values.retardo_NOOP;
	int retardo = atoi(retardo_config);
	msleep(retardo);

	log_debug(logger, "Finalizacion instruccion NO_OP");
}

void exec_read(nodo_instruccion* instruccion){
	log_debug(logger, "CASE de EXECUTE: entro en READ");

	int direccionLogica = list_get(instruccion->instruccion.parametros,0);

	int valor = buscarValorEnMemoria(direccionLogica);

	log_info(logger, "VALOR RETORNADO POR READ: %d",valor);
}

void exec_write(nodo_instruccion* instruccion){
	log_debug(logger, "CASE de EXECUTE: entro en WRITE");

	nodo_parametro* nodo_parametro = list_get(instruccion->instruccion.parametros,1);
	int valorAescribir = nodo_parametro->parametro;
	int* direccionLogica = list_get(instruccion->instruccion.parametros,0);

	escribirValorEnMemoria(direccionLogica, valorAescribir);

	log_debug(logger, "Direccion logica donde escribimos: %d", *direccionLogica);
	log_debug(logger, "Valor escrito en memoria %d", valorAescribir);
}

void exec_copy(nodo_instruccion* instruccion, int valorMemoria){
	log_debug(logger, "CASE de EXECUTE: entro en COPY");

	int* direccionLogica = list_get(instruccion->instruccion.parametros,0);
	escribirValorEnMemoria(direccionLogica, valorMemoria);

	log_debug(logger, "Direccion logica donde escribimos: %d", *direccionLogica);
	log_debug(logger, "Valor escrito en memoria %d",valorMemoria);
}

void exec_i_o(nodo_instruccion* instruccion, pcb** pcb){
	log_debug(logger, "CASE de EXECUTE: entro en I/0");

	nodo_parametro* tiempoDeBloqueo = list_get(instruccion->instruccion.parametros,0);
	(*pcb)->tiempo_a_bloquearse = tiempoDeBloqueo->parametro;
	log_debug(logger, "Tiempo a bloquearse %d",(*pcb)->tiempo_a_bloquearse);

	send_paquete_pcb(clienteDispatch, *pcb, PAQUETE_PCB_IO);
	activar_flag_desalojo();
}

void exec_exit(pcb** pcb){
	log_debug(logger, "CASE: entro en EXIT");

	send_paquete_pcb(clienteDispatch, *pcb, PAQUETE_PCB_EXIT);
	activar_flag_desalojo();

	log_debug(logger, "Finalizando exec_exit()");
}

// ejecucion de instruccion
void execute(nodo_instruccion* instruccion, pcb** pcb, int valorMemoria){
	log_debug(logger, COMIENZO_ETAPA_EXECUTE, (*pcb)->id);

	IDENTIFICADOR_INSTRUCCION identificador_instruccion = str2enum(instruccion->instruccion.identificador);

	switch (identificador_instruccion){
		case NO_OP:
			exec_no_op();
			break;

		case READ:
			exec_read(instruccion);
			break;

		case WRITE:
			exec_write(instruccion);
			break;

		case COPY:
			exec_copy(instruccion, valorMemoria);
			break;

		case IO:
			exec_i_o(instruccion, pcb);
			break;

		case EXIT_:
			exec_exit(pcb);
			break;
	}
}

// chequear si el Kernel nos envió una interrupcion
void check_interrupt(pcb** pcb){
	log_debug(logger, COMIENZO_ETAPA_CHECK_INTERRUPT, (*pcb)->id);

	if(hay_interrupcion()){
		log_debug(logger,"check_interrupt() detecto una INTERRUPCION");
		// (desalojo por interrupcion) se envia el PCB de nuevo a kernel para replanificar
		send_paquete_pcb(clienteDispatch, pcb, PAQUETE_PCB);
		activar_flag_desalojo();
		desactivar_flag_interrupcion();
	}

}

bool hay_interrupcion(){

	pthread_mutex_lock(&mtx_gv_flag_interrupcion);
	bool flag = gv_flag_interrupcion;
	pthread_mutex_unlock(&mtx_gv_flag_interrupcion);

	return flag;
}

void desactivar_flag_interrupcion(){
	// desactivo flag de interrupcion recibida de kernel
	pthread_mutex_lock(&mtx_gv_flag_interrupcion);
	gv_flag_interrupcion = false;
	pthread_mutex_unlock(&mtx_gv_flag_interrupcion);
}

void activar_flag_interrupcion(){
	// activo flag de interrupcion recibida de kernel
	pthread_mutex_lock(&mtx_gv_flag_interrupcion);
	gv_flag_interrupcion = true;
	pthread_mutex_unlock(&mtx_gv_flag_interrupcion);
}

bool hay_desalojo_proceso(){

	pthread_mutex_lock(&mtx_gv_flag_desalojar_proceso);
	bool flag = gv_flag_desalojar_proceso;
	pthread_mutex_unlock(&mtx_gv_flag_desalojar_proceso);

	return flag;
}

void activar_flag_desalojo() {
	// activo flag de desalojo de proceso
	pthread_mutex_lock(&mtx_gv_flag_desalojar_proceso);
	gv_flag_desalojar_proceso = true;
	pthread_mutex_unlock(&mtx_gv_flag_desalojar_proceso);
}

void desactivar_flag_desalojo() {
	// desactivo flag de desalojo de proceso
	pthread_mutex_lock(&mtx_gv_flag_desalojar_proceso);
	gv_flag_desalojar_proceso = false;
	pthread_mutex_unlock(&mtx_gv_flag_desalojar_proceso);
}

int buscarValorEnMemoria(int* direccionLogica){
	//TODO:
	return 14;
}

int escribirValorEnMemoria(int* direccionLogica, int valor){
	//TODO:
	return 322;
}

IDENTIFICADOR_INSTRUCCION str2enum (char *str)
{
     int j;
     for (j = 0;  j < sizeof (conversion) / sizeof (conversion[0]);  ++j)
         if (!strcmp (str, conversion[j].str))
             return conversion[j].val;
}
