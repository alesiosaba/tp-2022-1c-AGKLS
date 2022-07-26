#include "../include/utils/protocolo.h"

bool send_paquete_pcb(int fd, pcb* nodo_pcb, op_code codigo_paquete){

	t_paquete* paquete = generar_paquete_pcb(*nodo_pcb, codigo_paquete);
	enviar_paquete(paquete, fd);
	log_debug(logger, PCB_ENVIADO, nodo_pcb->id);
	return eliminar_paquete(paquete);

}

bool recv_paquete_pcb(int fd, pcb** nodo_pcb){
	t_list* lista;
	lista = recibir_paquete(fd);
	*nodo_pcb = deserializar_PCB(lista);
	log_debug(logger, PCB_DESERIALIZADO, (*nodo_pcb)->id);
	free(lista);
	return true;
}

// SERIALIZACION CPU - MEMORIA

bool send_solicitud_tabla_N1(int fd, int id_tablaN1, int entrada_tabla_primer_nivel){
	log_debug(logger, "Entro a send_solicitud_tabla_N1()");

	t_paquete *paquete = crear_paquete(SOLICITUD_TABLA_PAGINA_N1);
	char* id_tablaN1_str = string_new();
	sprintf(id_tablaN1_str, "%d\0", id_tablaN1);
	agregar_a_paquete(paquete, id_tablaN1_str, strlen(id_tablaN1_str) + 1);

	char* entrada_tabla_primer_nivel_str = string_new();
	sprintf(entrada_tabla_primer_nivel_str, "%d\0", entrada_tabla_primer_nivel);
	agregar_a_paquete(paquete, entrada_tabla_primer_nivel_str, strlen(entrada_tabla_primer_nivel_str) + 1);

	log_debug(logger, "Arme paquete de solicitud tabla N1");

	enviar_paquete(paquete, fd);

	log_debug(logger, "Envie solicitud tabla N1");

	return eliminar_paquete(paquete);
}

// retorna entrada de tabla N1 (eso apunta a una tabla N2)
int recv_respuesta_solicitud_N1(int fd){

	log_debug(logger, "Entro a recv_respuesta_solicitud_N1()");

	t_list* lista;
	lista = recibir_paquete(fd);
	int tablaN2 = atoi(list_remove(lista, 0));
	list_destroy(lista);

	log_debug(logger, "Recibi respuesta de solicitud N1");

	return tablaN2;
}

bool send_solicitud_tabla_N2(int fd, int id_tablaN2, int entrada_tabla_segundo_nivel){
	log_debug(logger, "Entro a send_solicitud_tabla_N2()");

	t_paquete *paquete = crear_paquete(SOLICITUD_TABLA_PAGINA_N2);
	char* id_tablaN2_str = string_new();
	sprintf(id_tablaN2_str, "%d\0", id_tablaN2);
	agregar_a_paquete(paquete, id_tablaN2_str, strlen(id_tablaN2_str) + 1);

	char* entrada_tabla_segundo_nivel_str = string_new();
	sprintf(entrada_tabla_segundo_nivel_str, "%d\0", entrada_tabla_segundo_nivel);
	agregar_a_paquete(paquete, entrada_tabla_segundo_nivel_str, strlen(entrada_tabla_segundo_nivel_str) + 1);


	log_debug(logger, "Arme paquete de solicitud tabla N2");

	enviar_paquete(paquete, fd);

	log_debug(logger, "Envie solicitud tabla N2");

	return eliminar_paquete(paquete);
}

int recv_respuesta_solicitud_N2(int fd){
	log_debug(logger, "Entro a recv_respuesta_solicitud_N1()");

	t_list* lista;
	lista = recibir_paquete(fd);
	int frame = atoi(list_remove(lista, 0));
	list_destroy(lista);

	log_debug(logger, "Recibi respuesta de solicitud N2");

	return frame;
}

// esta funcion responde tanto a consultas de tabla N1 y N2
void send_respuesta_solicitud_tabla(int fd, int valor_solicitado){
	log_debug(logger, "Entro a send_respuesta_solicitud_tabla()");

	t_paquete* paquete = crear_paquete();
	char* valor_a_enviar = string_new();
	sprintf(valor_a_enviar, "%d\0", valor_solicitado);
	agregar_a_paquete(paquete, valor_a_enviar, strlen(valor_a_enviar) + 1);
	enviar_paquete(paquete, fd);
	eliminar_paquete(paquete);

	log_debug(logger, "Salgo de send_respuesta_solicitud_tabla()");
}

// esta funcion recibe en MEMORIA tanto consultas de tabla N1 y N2
void recv_solicitud_tabla(int fd, consulta_en_tabla_paginas *consulta){
	log_debug(logger, "Entro a recv_solicitud_tabla()");

	t_list* lista;
	lista = recibir_paquete(fd);

	int *id_tabla = list_remove(lista, 0);
	consulta->id_tabla = atoi(id_tabla);

	int* entrada_en_tabla = list_remove(lista, 0);
	consulta->entrada_en_tabla = atoi(entrada_en_tabla);

	log_debug(logger, "id_tabla: %d\tentrada: %d", consulta->id_tabla, consulta->entrada_en_tabla);

	log_debug(logger, "Salgo de recv_solicitud_tabla()");

	list_destroy(lista);
}


bool recv_paquete_consola(int fd, pcb** nodo_pcb){
	t_list* lista;
	lista = recibir_paquete(fd);
	*nodo_pcb = armar_PCB_nuevo(lista);
	return true;
}

bool send_paquete_kernel(int fd, op_code codigo_paquete){
	t_paquete* paquete = crear_paquete(codigo_paquete);
	enviar_paquete(paquete, fd);
	log_debug(logger, "Enviando respuesta a la consola");
	return eliminar_paquete(paquete);
}

// En CPU
bool send_handshake_inicial(int fd){
	t_paquete* paquete = crear_paquete(HANDSHAKE_INICIAL);
	enviar_paquete(paquete, fd);
	log_debug(logger, "Handshake inicial con modulo Memoria");
	eliminar_paquete(paquete);
	return true;
}

// En CPU
struct handshake_inicial_s recv_respuesta_handshake_inicial(int fd){
	t_list* lista;
	lista = recibir_paquete(fd);

	struct handshake_inicial_s hd_inicial;

	t_list* tam_pagina = list_remove(lista,0);
	hd_inicial.tamanio_pagina = atoi(tam_pagina);

	t_list* cant_entradas_por_tabla = list_remove(lista,0);
	hd_inicial.cant_entradas_por_tabla = atoi(cant_entradas_por_tabla);

	log_debug(logger,"deserializada la resp de handshake");

	list_destroy(lista);

	return hd_inicial;
}

bool send_respuesta_handshake_inicial(int fd, int tamanio_pagina, int cant_entradas_por_tabla){
	log_debug(logger, "armado de paquete con int tamanio_pagina = %d",tamanio_pagina);
	log_debug(logger, "armado de paquete con int cant_entradas_por_tabla = %d",cant_entradas_por_tabla);

	t_paquete* paquete = crear_paquete(RESPUESTA_HANDSHAKE_INICIAL);
	log_debug(logger, "Paquete creado");

	char* tamanio_pagina_enviar = string_new();
	sprintf(tamanio_pagina_enviar, "%d\0", tamanio_pagina);
	agregar_a_paquete(paquete, tamanio_pagina_enviar, strlen(tamanio_pagina_enviar) + 1);
	log_debug(logger, "tamanio_pagina que se manda: %s",tamanio_pagina_enviar);

	char* cant_entradas_por_tabla_enviar = string_new();
	sprintf(cant_entradas_por_tabla_enviar, "%d\0", cant_entradas_por_tabla);
	agregar_a_paquete(paquete, cant_entradas_por_tabla_enviar, strlen(cant_entradas_por_tabla_enviar) + 1);
	log_debug(logger, "cant_entradas que se manda: %s",cant_entradas_por_tabla_enviar);

	log_debug(logger, "Paquete armado");
	enviar_paquete(paquete, fd);
	log_debug(logger, "Se envia tamaño de pagina: %s" , tamanio_pagina_enviar);
	log_debug(logger, "Se envia cant_entradas: %s" , cant_entradas_por_tabla_enviar);
	log_debug(logger, "Respuesta de Handshake inicial hacia modulo CPU");
	return eliminar_paquete(paquete);
}




bool send_interrupcion(int fd){
	t_paquete* paquete = crear_paquete(INTERRUPCION);
	enviar_paquete(paquete, fd);
	log_debug(logger, "Enviando interrupcion a CPU");
	return eliminar_paquete(paquete);
}
