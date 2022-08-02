#include "../include/utils/protocolo.h"

bool send_paquete_pcb(int fd, pcb* nodo_pcb, op_code codigo_paquete){
	t_paquete* paquete = generar_paquete_pcb(*nodo_pcb, codigo_paquete);
	enviar_paquete(paquete, fd);
	log_debug(logger, PCB_ENVIADO, nodo_pcb->id, codigo_paquete);
	return eliminar_paquete(paquete);
}

bool recv_paquete_pcb(int fd, pcb** nodo_pcb){
	t_list* lista;
	lista = recibir_paquete(fd);
	*nodo_pcb = deserializar_PCB(lista);
	log_debug(logger, PCB_DESERIALIZADO, (*nodo_pcb)->id);

	//free(lista);
	return true;
}

// SERIALIZACION CPU - MEMORIA

bool send_solicitud_tabla_N1(int fd, int id_tablaN1, int entrada_tabla_primer_nivel, int proceso_id){
	log_debug(logger, "Entro a send_solicitud_tabla_N1()");

	t_paquete* paquete = crear_paquete(SOLICITUD_TABLA_PAGINA_N1);
	char* id_tablaN1_str = string_itoa(id_tablaN1);
	agregar_a_paquete(paquete, id_tablaN1_str, strlen(id_tablaN1_str) + 1);
	free(id_tablaN1_str);

	char* entrada_tabla_primer_nivel_str = string_itoa(entrada_tabla_primer_nivel);
	agregar_a_paquete(paquete, entrada_tabla_primer_nivel_str, strlen(entrada_tabla_primer_nivel_str) + 1);
	free(entrada_tabla_primer_nivel_str);

	char* proceso_id_str = string_itoa(proceso_id);
	agregar_a_paquete(paquete, proceso_id_str, strlen(proceso_id_str) + 1);
	free(proceso_id_str);

	log_debug(logger, "Arme paquete de solicitud tabla N1");

	enviar_paquete(paquete, fd);

	log_debug(logger, "Envie solicitud tabla N1");

	return eliminar_paquete(paquete);
}

// retorna entrada de tabla N1 (eso apunta a una tabla N2)
int recv_respuesta_solicitud_N1(int fd){

	log_debug(logger, "Entro a recv_respuesta_solicitud_N1()");

	op_code cod_op;
	cod_op = recibir_operacion(fd);

	t_list* lista;
	lista = recibir_paquete(fd);
	int tablaN2 = atoi(list_get(lista, 0));
	list_destroy_and_destroy_elements(lista, free);

	log_debug(logger, "Recibi respuesta de solicitud N1");

	return tablaN2;
}

bool send_solicitud_tabla_N2(int fd, int id_tablaN2, int entrada_tabla_segundo_nivel, int proceso_id){
	log_debug(logger, "Entro a send_solicitud_tabla_N2()");

	t_paquete *paquete = crear_paquete(SOLICITUD_TABLA_PAGINA_N2);

	char* id_tablaN2_str = string_itoa(id_tablaN2);
	agregar_a_paquete(paquete, id_tablaN2_str, strlen(id_tablaN2_str) + 1);
	free(id_tablaN2_str);

	char* entrada_tabla_segundo_nivel_str = string_itoa(entrada_tabla_segundo_nivel);
	agregar_a_paquete(paquete, entrada_tabla_segundo_nivel_str, strlen(entrada_tabla_segundo_nivel_str) + 1);
	free(entrada_tabla_segundo_nivel_str);

	char* proceso_id_str = string_itoa(proceso_id);
	agregar_a_paquete(paquete, proceso_id_str, strlen(proceso_id_str) + 1);
	free(proceso_id_str);

	log_debug(logger, "Arme paquete de solicitud tabla N2");

	enviar_paquete(paquete, fd);

	log_debug(logger, "Envie solicitud tabla N2");

	return eliminar_paquete(paquete);
}

int recv_respuesta_solicitud_N2(int fd){
	log_debug(logger, "Entro a recv_respuesta_solicitud_N1()");

	op_code cod_op;
	cod_op = recibir_operacion(fd);

	t_list* lista;
	lista = recibir_paquete(fd);
	int frame = atoi(list_get(lista, 0));
	list_destroy_and_destroy_elements(lista,free);

	log_debug(logger, "Recibi respuesta de solicitud N2");

	return frame;
}

// esta funcion responde tanto a consultas de tabla N1 y N2
void send_respuesta_solicitud_tabla(int fd, int valor_solicitado, op_code cod_op){
	log_debug(logger, "Entro a send_respuesta_solicitud_tabla()");

	t_paquete* paquete = crear_paquete(cod_op);
	char* valor_a_enviar = string_new();
	sprintf(valor_a_enviar, "%d\0", valor_solicitado);
	agregar_a_paquete(paquete, valor_a_enviar, strlen(valor_a_enviar) + 1);
	enviar_paquete(paquete, fd);
	eliminar_paquete(paquete);

	log_debug(logger, "Salgo de send_respuesta_solicitud_tabla()");
}

consulta_en_tabla_paginas* deserializar_solicitud_tablas(t_list* lista){
	consulta_en_tabla_paginas* consulta_aux =(struct consulta_en_tabla_paginas*) malloc(sizeof(struct consulta_en_tabla_paginas));

	int* id_tabla = list_remove(lista, 0);
	consulta_aux->id_tabla = atoi(id_tabla);

	int* entrada_en_tabla = list_remove(lista, 0);
	consulta_aux->entrada_en_tabla = atoi(entrada_en_tabla);

	int* id_proceso = list_remove(lista, 0);
	consulta_aux->id_proceso = atoi(id_proceso);

	return consulta_aux;
}

// esta funcion recibe en MEMORIA tanto consultas de tabla N1 y N2
void recv_solicitud_tabla(int fd, consulta_en_tabla_paginas** consulta){
	log_debug(logger, "Entro a recv_solicitud_tabla()");

	t_list* lista;
	lista = recibir_paquete(fd);

	*consulta = deserializar_solicitud_tablas(lista);

	log_debug(logger, "PID: %d\tid_tabla: %d\tentrada: %d", (*consulta)->id_proceso ,(*consulta)->id_tabla, (*consulta)->entrada_en_tabla);

	log_debug(logger, "Salgo de recv_solicitud_tabla()");

	list_destroy_and_destroy_elements(lista, free);
}

bool recv_paquete_consola(int fd, pcb** nodo_pcb){
	t_list* lista;
	lista = recibir_paquete(fd);
	*nodo_pcb = armar_PCB_nuevo(lista);
	list_destroy_and_destroy_elements(lista, free);
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
	log_debug(logger, "Enviando handshake con cod_op: %d", paquete->codigo_operacion);
	eliminar_paquete(paquete);
	return true;
}

// En CPU
struct handshake_inicial_s recv_respuesta_handshake_inicial(int fd){

	t_list* lista;
	log_debug(logger, "recibir_paquete en recv_respuesta_handshake_inicial");

	op_code cod_op;
	cod_op = recibir_operacion(fd);

	lista = recibir_paquete(fd);

	struct handshake_inicial_s hd_inicial;

	int tam_pagina = atoi(list_remove(lista,0));
	hd_inicial.tamanio_pagina = tam_pagina;
	log_debug(logger, "tam_pagina: %d", tam_pagina);

	int cant_entradas_por_tabla = atoi(list_remove(lista,0));
	hd_inicial.cant_entradas_por_tabla = cant_entradas_por_tabla;
	log_debug(logger, "cant_entradas_por_tabla: %d", cant_entradas_por_tabla);

	log_debug(logger,"deserializada la resp de handshake");

	list_destroy(lista);

	log_debug(logger, "salgo de recibir_paquete en recv_respuesta_handshake_inicial");

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

bool send_respuesta_nuevo_proceso(int fd, int numero_tabla){
	// log_debug(logger, "Enviando respuesta a Kernel, numero de tabla %d", numero_tabla);

	t_paquete *paquete = crear_paquete(RESPUESTA_NUEVO_PROCESO);
	char* numero_tabla_str = string_new();
	sprintf(numero_tabla_str, "%d\0", numero_tabla);

	agregar_a_paquete(paquete, numero_tabla_str, strlen(numero_tabla_str) + 1);
	enviar_paquete(paquete, fd);
	return true;
}

int recv_respuesta_nuevo_proceso(int fd){
	t_list* lista;
	op_code cod_op;
	cod_op = recibir_operacion(fd);

	switch (cod_op) {

	case RESPUESTA_NUEVO_PROCESO:
		lista = recibir_paquete(fd);
		int numero_tabla = atoi(list_remove(lista, 0));
		list_destroy(lista);
		return numero_tabla;
	case -1:
		log_warning(logger, SERVIDOR_DESCONEXION);
		return NULL;
	default:
		log_warning(logger,OPERACION_DESCONOCIDA);
		break;
	}

	log_warning(logger, "La memoria respondio erroneamente a la creacion de un proceso nuevo");

	return 9999;
}

struct direccion_fisica* deserializar_direccion_fisica(t_list* lista){
	direccion_fisica* direccion_fisica_aux = (struct direccion_fisica*)malloc(sizeof(struct direccion_fisica));

	int* marco = list_remove(lista, 0);
	direccion_fisica_aux->marco = atoi(marco);

	int* desplazamiento = list_remove(lista, 0);
	direccion_fisica_aux->desplazamiento = atoi(desplazamiento);

	return direccion_fisica_aux;
}

void recv_pedido_lectura(int socket_cliente, struct direccion_fisica** direccion_fisica_lectura){
	// log_debug(logger, "Entro a recv_pedido_lectura()");

	t_list* lista;
	lista = recibir_paquete(socket_cliente);

	*direccion_fisica_lectura = deserializar_direccion_fisica(lista);

	/*
	 log_debug(logger, "Direccion fisica recibida:");
	log_debug(logger, "Marco: %d\tDesplazamiento: %d", (*direccion_fisica_lectura)->marco, (*direccion_fisica_lectura)->desplazamiento);
	log_debug(logger, "Salgo de recv_solicitud_tabla()");
	 */
	list_destroy_and_destroy_elements(lista, free);
}

void send_respuesta_pedido_lectura(int socket_cliente, uint32_t valor_leido){

	// log_debug(logger, "Entro a send_respuesta_pedido_lectura()");

	t_paquete* paquete = crear_paquete(RESPUESTA_PEDIDO_LECTURA);

	char* valor_a_enviar = string_new();
	sprintf(valor_a_enviar, "%d\0", valor_leido);

	// log_debug(logger,"valor a enviar por la lectura: %s", valor_a_enviar);

	agregar_a_paquete(paquete, valor_a_enviar, strlen(valor_a_enviar) + 1);
	enviar_paquete(paquete, socket_cliente);
	eliminar_paquete(paquete);

	// log_debug(logger, "Salgo de send_respuesta_pedido_lectura()");
}

void send_pedido_lectura(int socketCliente, struct direccion_fisica direccion_fisica_lectura){
	// Parte del send que hagamos
	// log_debug(logger, "Entro a send_PEDIDO_LECTURA()");

	t_paquete* paquete = crear_paquete(PEDIDO_LECTURA);
	char* marco_str = string_new();
	sprintf(marco_str, "%d\0", direccion_fisica_lectura.marco);
	agregar_a_paquete(paquete, marco_str, strlen(marco_str) + 1);

	char* desplazamiento_str = string_new();
	sprintf(desplazamiento_str, "%d\0",direccion_fisica_lectura.desplazamiento);
	agregar_a_paquete(paquete, desplazamiento_str,strlen(desplazamiento_str) + 1);

	// log_debug(logger, "Arme paquete de PEDIDO_LECTURA");
	enviar_paquete(paquete, socketCliente);

	// log_debug(logger, "Envie PEDIDO_LECTURA");
	eliminar_paquete(paquete);
}

uint32_t recv_respuesta_pedido_lectura(int socketCliente){
	// log_debug(logger, "Entro a recv_PEDIDO_LECTURA()");

	op_code cod_op;
	cod_op = recibir_operacion(socketCliente);

	t_list* lista;
	lista = recibir_paquete(socketCliente);

	uint32_t valor_buscado = atoi(list_remove(lista, 0));
	list_destroy(lista);

	return valor_buscado;
}

void send_pedido_escritura(int socketCliente, struct direccion_fisica direccion_fisica_lectura, uint32_t valor_a_escribir){
	// log_debug(logger, "Entro a send_pedido_escritura()");

	t_paquete* paquete = crear_paquete(PEDIDO_ESCRITURA);

	char* marco_str = string_itoa(direccion_fisica_lectura.marco);
	agregar_a_paquete(paquete, marco_str, strlen(marco_str) + 1);
	free(marco_str);

	char* desplazamiento_str = string_itoa(direccion_fisica_lectura.desplazamiento);
	agregar_a_paquete(paquete, desplazamiento_str,strlen(desplazamiento_str) + 1);
	free(desplazamiento_str);

	char* valor_escritura_str = string_itoa(valor_a_escribir);
	agregar_a_paquete(paquete, valor_escritura_str,strlen(valor_escritura_str) + 1);
	free(valor_escritura_str);

	// log_debug(logger, "Arme paquete de PEDIDO_ESCRITURA");
	enviar_paquete(paquete, socketCliente);

	log_debug(logger, "Envie PEDIDO_ESCRITURA");

	eliminar_paquete(paquete);
}

int recv_pedido_escritura(int socket_cliente, struct direccion_fisica** direccion_fisica_lectura){
	 log_debug(logger, "Entro a recv_pedido_escritura()");

	t_list* lista;
	lista = recibir_paquete(socket_cliente);

	*direccion_fisica_lectura = deserializar_direccion_fisica(lista);

	int valor_a_escribir = atoi(list_remove(lista, 0));
	 log_debug(logger, "recv_pedido_escritura: El valor recibido a ser leido es %d", valor_a_escribir);

	/*
	log_debug(logger, "Direccion fisica recibida:");
	log_debug(logger, "Marco: %d\tDesplazamiento: %d", (*direccion_fisica_lectura)->marco, (*direccion_fisica_lectura)->desplazamiento);
	log_debug(logger, "Valor a ser escrito en memoria: %d", valor_a_escribir);

	log_debug(logger, "Salgo de recv_pedido_escritura()");
	*/
	list_destroy_and_destroy_elements(lista, free);

	return valor_a_escribir;
}

void send_respuesta_pedido_escritura(int socket_cliente, int resultadoEscritura){

	log_debug(logger, "Entro a send_respuesta_pedido_escritura()");

	t_paquete* paquete = crear_paquete(RESPUESTA_PEDIDO_LECTURA);

	char* resultadoEscritura_str = string_new();
	sprintf(resultadoEscritura_str, "%d\0", resultadoEscritura);

	log_debug(logger,"valor a enviar por la escritura: %s", resultadoEscritura_str);

	agregar_a_paquete(paquete, resultadoEscritura_str, strlen(resultadoEscritura_str) + 1);
	enviar_paquete(paquete, socket_cliente);
	eliminar_paquete(paquete);

	log_debug(logger, "Salgo de send_respuesta_pedido_escritura()");
}

bool recv_respuesta_pedido_escritura(int socketCliente){
	log_debug(logger, "Entro a recv_respuesta_pedido_escritura()");

	op_code cod_op;
	cod_op = recibir_operacion(socketCliente);

	t_list* lista;
	lista = recibir_paquete(socketCliente);

    // TODO: Si la ejecucion es exitosa se devuelve 0, pero 0 es interpretado como false.
	// Ver de normalizar esto
	bool resultadoEscritura = atoi(list_get(lista, 0));


	list_destroy_and_destroy_elements(lista , free);

	return resultadoEscritura;
}











