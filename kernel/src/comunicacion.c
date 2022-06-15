#include "../include/init.h"
#include "../include/comunicacion.h"
#include "../../shared/include/utils/utils.h"

void iterator(char* value) {
	log_info(logger,"%s", value);
}

nodo_pcb* armar_PCB(t_list* lista){

	nodo_pcb* nodo_pcb = (struct nodo_pcb*)malloc(sizeof(struct nodo_pcb));
	nodo_pcb->pcb.instrucciones= NULL;

	// agarro el primer elemento (tamanio del proceso)
	t_list* tamanio_proceso = list_remove(lista, 0);
	nodo_pcb->pcb.tamanio = atoi(tamanio_proceso);

	nodo_pcb->pcb.id = 555;

	// armo la lista de instrucciones y la guardo en el PCB
	nodo_pcb->pcb.instrucciones = armar_lista_instrucciones(lista);

	nodo_pcb->pcb.program_counter = nodo_pcb->pcb.instrucciones;

	return nodo_pcb;
}

char* generar_renglon_instruccion(struct instruccion instruccion_a_enviar){
	char* renglon_instruccion = string_new();

	// Extraigo el codigo de instruccion
	string_append(&renglon_instruccion, instruccion_a_enviar.identificador);

	// Genero un puntero auxiiar para recorrer la lista de parametros
	struct nodo_parametro* parametros_aux = instruccion_a_enviar.parametros;

	// Si la instruccion no tiene parametros directamente devuelve el renglon
	if(parametros_aux != NULL){
		// Caso que la instruccion tenga al menos 1 parametro
		while(parametros_aux->sig != NULL){
			string_append(&renglon_instruccion, " ");
			string_append(&renglon_instruccion, (char*) parametros_aux->parametro);
			parametros_aux = parametros_aux->sig;
		}
	}

	string_append(&renglon_instruccion,"\n");

	return renglon_instruccion;

}


t_paquete* generar_paquete_pcb(struct pcb PCB_a_enviar){

	t_paquete* paquete = crear_paquete(PAQUETE_PCB);

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

	// genero el renglon para el program_counter
	renglon_instruccion = generar_renglon_instruccion(PCB_a_enviar.program_counter->instruccion);

	agregar_a_paquete(paquete, renglon_instruccion, sizeof(renglon_instruccion));

	return paquete;

	/* Orden dentro del buffer del paquete

	 id
	 tamanio
	 tabla_paginas 		-- Todavia no implementado
	 estimacion_rafaga  -- Todavia no implementado

	 program_counter

	 lista_instrucciones -- Todavia no implementado

	 */
}




int manejarConexion(void* void_args){

	t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* server_name = args->server_name;
    free(args);

	t_list* lista;
	nodo_pcb* nodo_pcb;
	while (socket_cliente != -1) {
		op_code cod_op;
		cod_op = recibir_operacion(socket_cliente);

		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, LECTURA_DE_VALORES);
			list_iterate(lista, (void*) iterator);
			break;
		case PAQUETE_CONSOLA:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, RECEPCION_PAQUETE_CONSOLA);
			nodo_pcb = armar_PCB(lista);
			imprimir_PCB(nodo_pcb);
			log_info(logger, "Se armó un PCB correctamente.");

			t_paquete* paquete = generar_paquete_pcb(nodo_pcb->pcb);

			enviar_paquete(paquete, conexionACPU);
			eliminar_paquete(paquete);

			log_info(logger, "Se enviará un PCB a CPU");

			break;
		case -1:
			log_warning(logger, SERVIDOR_DESCONEXION);
			return EXIT_FAILURE;
		default:
			log_warning(logger,OPERACION_DESCONOCIDA);
			break;
		}
	}
	log_warning(logger, "El cliente se desconecto de %s server", server_name);

	return 0;

}

int server_escuchar(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) manejarConexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}



