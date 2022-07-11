#include "../include/init.h"
#include "../include/comunicacion.h"
#include "../include/gestionPCB.h"
#include "../../shared/include/utils/utils.h"



void iterator(char* value) {
	log_info(logger,"%s", value);
}

pcb* armar_PCB(t_list* lista){

	//nodo_pcb* nodo_pcb = (struct nodo_pcb*)malloc(sizeof(struct nodo_pcb));
	pcb* pcb = (struct pcb*)malloc(sizeof(struct pcb));
	pcb->instrucciones= NULL;

	// agarro el primer elemento (tamanio del proceso)
	t_list* tamanio_proceso = list_remove(lista, 0);
	pcb->tamanio= atoi(tamanio_proceso);

	pcb->id = 555;

	// armo la lista de instrucciones y la guardo en el PCB
	pcb->instrucciones = armar_lista_instrucciones(lista);

	pcb->program_counter = pcb->instrucciones;

	pcb->state = NEW;

	return pcb;
}

char* generar_renglon_instruccion(struct instruccion instruccion_a_enviar){
	char* renglon_instruccion = string_new();

	// Extraigo el codigo de instruccion
	string_append(&renglon_instruccion, instruccion_a_enviar.identificador);

	// Genero un puntero auxiiar para recorrer la lista de parametros
	struct nodo_parametro* parametros_aux = instruccion_a_enviar.parametros;

	// Si la instruccion no tiene parametros directamente devuelve el renglon
	if(parametros_aux != NULL){
		char* parametroStr =  string_new();
		// Caso que la instruccion tenga al menos 1 parametro
		do{
			string_append(&renglon_instruccion, " ");
			sprintf(parametroStr, "%d", parametros_aux->parametro);
			string_append(&renglon_instruccion, parametroStr);
			parametros_aux = parametros_aux->sig;
		}while(parametros_aux);
	}

	string_append(&renglon_instruccion,"\n");

	return renglon_instruccion;

}


t_paquete* generar_paquete_pcb(struct pcb PCB_a_enviar){

	t_paquete* paquete = crear_paquete(PAQUETE_PCB);
	nodo_instruccion* nodo_instruccion = PCB_a_enviar.instrucciones;

	// Saco lo del PCB a enviar a variables
	// lo que NO sean instrucciones
	char* id =  string_new();
	sprintf(id, "%d\n", PCB_a_enviar.id);
	agregar_a_paquete(paquete, id, sizeof(id));

	char* tamanio = string_new();
	sprintf(tamanio, "%d\n", PCB_a_enviar.tamanio);
	agregar_a_paquete(paquete, tamanio, sizeof(tamanio));


	// double tabla_paginas = PCB_a_enviar.tabla_paginas;

	// Este caso es distinto porque no hay conversion directa desde double a char*
	/*char estimacion_rafaga[sizeof(PCB_a_enviar.estimacion_rafaga)];
	memcpy(
			estimacion_rafaga
			, &PCB_a_enviar.estimacion_rafaga
			, sizeof(PCB_a_enviar.estimacion_rafaga)
	);*/

	// agregar_a_paquete(paquete, (char*) tabla_paginas, sizeof(tabla_paginas));
	// agregar_a_paquete(paquete, estimacion_rafaga, sizeof(estimacion_rafaga));

	// buffer para concatenar instruccion y sus parametros
	char* renglon_instruccion = string_new();

/* GUIDO: No enviar el program counter, porque la CPU lo puede tomar como la primer direccion de la lista de instrucciones.

	// genero el renglon para el program_counter
	renglon_instruccion = generar_renglon_instruccion(PCB_a_enviar.program_counter->instruccion);

	agregar_a_paquete(paquete, renglon_instruccion, sizeof(renglon_instruccion));
*/
	//GENERAR LISTA DE INSTRUCCIONES COMO RENGLONES

	while(nodo_instruccion->sig){
		renglon_instruccion = generar_renglon_instruccion(nodo_instruccion->instruccion);
		int size = strlen(renglon_instruccion)*sizeof(char);
		agregar_a_paquete(paquete, renglon_instruccion, size);
		nodo_instruccion = nodo_instruccion->sig;
	}


	//





	return paquete;

	/* Orden dentro del buffer del paquete

	 id
	 tamanio
	 tabla_paginas 		-- Todavia no implementado
	 estimacion_rafaga  -- Todavia no implementado

	 program_counter    -- GR: NO SE DEBERÍA ENVIAR, GUARDA DIRECTO EN CPU LA DIRECCION DE LA PRIMERA INSTR.

	 lista_instrucciones -- Todavia no implementado

	 */
}


