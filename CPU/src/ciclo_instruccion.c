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
		uint32_t valorLeidoDeMemoria = 0;
		if(esCopy){
			log_debug(logger, COMIENZO_ETAPA_FETCH_OPERANDS, (*pcb)->id);
			valorLeidoDeMemoria = fetch_operands(pcb, instruccion);
			log_debug(logger, "FETCH OPERANDS retorna: %u",valorLeidoDeMemoria);
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
	(*pcb)->program_counter++;
	return instruccion;
}

// interpretar qué instrucción es la que se va a ejecutar (evalua si ejecuta fetch_operands y como)
int decode(nodo_instruccion* instruccion){

	return (strcmp(instruccion->instruccion.identificador, "COPY") == 0);
}

// buscar valor en memoria del parametro de COPY
uint32_t fetch_operands(pcb** pcb, nodo_instruccion* instruccion){
	int* direccionLogica = list_get(instruccion->instruccion.parametros,1);

	return buscarValorEnMemoria(pcb, *direccionLogica);
}

void exec_no_op(){
	log_debug(logger, "CASE de EXECUTE: entro en NO_OP");

	char* retardo_config = config_values.retardo_NOOP;
	int retardo = atoi(retardo_config);
	msleep(retardo);

	log_debug(logger, "Finalizacion instruccion NO_OP");
}

void exec_read(nodo_instruccion* instruccion, pcb** pcb){
	log_debug(logger, "CASE de EXECUTE: entro en READ");

	int* direccionLogica = list_get(instruccion->instruccion.parametros,0);

	int valor = buscarValorEnMemoria(pcb, *direccionLogica);

	log_info(logger, "VALOR RETORNADO POR READ: %d",valor);
}

void exec_write(nodo_instruccion* instruccion, pcb** pcb){
	log_debug(logger, "CASE de EXECUTE: entro en WRITE");

	nodo_parametro* nodo_parametro = list_get(instruccion->instruccion.parametros,1);
	uint32_t valorAescribir = nodo_parametro->parametro;
	int* direccionLogica = list_get(instruccion->instruccion.parametros,0);

	escribirValorEnMemoria(pcb, *direccionLogica, valorAescribir);
}

void exec_copy(nodo_instruccion* instruccion, uint32_t valorMemoria, pcb** pcb){
	log_debug(logger, "CASE de EXECUTE: entro en COPY");

	int* direccionLogica = list_get(instruccion->instruccion.parametros,0);
	escribirValorEnMemoria(pcb, *direccionLogica, valorMemoria);
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
void execute(nodo_instruccion* instruccion, pcb** pcb, uint32_t valorMemoria){
	log_debug(logger, COMIENZO_ETAPA_EXECUTE, (*pcb)->id);

	IDENTIFICADOR_INSTRUCCION identificador_instruccion = str_to_identificador_enum(instruccion->instruccion.identificador);

	switch (identificador_instruccion){
		case NO_OP:
			exec_no_op();
			break;

		case READ:
			exec_read(instruccion, pcb);
			break;

		case WRITE:
			exec_write(instruccion, pcb);
			break;

		case COPY:
			exec_copy(instruccion, valorMemoria, pcb);
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

uint32_t buscarValorEnMemoria(pcb** pcb, int direccionLogica){

	log_debug(logger,"Entro en buscarValorEnMemoria()");

	// log_debug(logger,"\nbuscar:\n\tPID: %d\n\tid_tablaN1: %d\n\tdir. logica:%d", (*pcb)->id, id_tablaN1, direccionLogica);

	// int direccion_fisica = traducir_dir_logica(pcb, direccionLogica);

	uint32_t resultado = 14;

	//TODO: implementar correcto comportamiento
	return resultado;
}

bool escribirValorEnMemoria(pcb** pcb, int direccionLogica, uint32_t valor){

	log_debug(logger,"Entro en escribirValorEnMemoria()");

	// int direccion_fisica = traducir_dir_logica(pcb, direccionLogica);

	// log_debug(logger,"\nescribir:\n\tPID: %d\n\tid_tablaN1: %d\n\tdir. logica:%d\n\tValor en uint32_t: %u", (*pcb)->id, id_tablaN1, direccionLogica, valor);

	//TODO: devolver bool que confirme lectura correcta
	return true;
}

IDENTIFICADOR_INSTRUCCION str_to_identificador_enum (char *str)
{
     int j;
     for (j = 0;  j < sizeof (conversion) / sizeof (conversion[0]);  ++j)
         if (!strcmp (str, conversion[j].str))
             return conversion[j].val;
}
