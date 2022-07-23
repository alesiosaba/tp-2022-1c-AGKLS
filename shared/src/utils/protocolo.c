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
