#include "../include/utils/utils.h"


// Implementacion de Comportamientos del Logger & Config

t_log* iniciar_logger(char* log_level, char* logger_path, char* logger)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create(logger_path, logger ,true,log_level_from_string(log_level));
	return nuevo_logger;
}

t_config* iniciar_config(char* config_path)
{
	t_config* nuevo_config;
	nuevo_config = config_create(config_path);
	return nuevo_config;
}

// Implementacion de Comportamientos del Cliente

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = 0;
	socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_super_paquete(void)
{
	//me falta un malloc!
	t_paquete* paquete = malloc(sizeof(t_paquete));

	//descomentar despues de arreglar
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

t_paquete* crear_paquete(op_code codigo_operacion)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo_operacion;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}


// Implementacion de Comportamientos del Servidor

int iniciar_servidor(char* puerto_escucha)
{

	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(IP, puerto_escucha, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family,
							 servinfo->ai_socktype,
							 servinfo->ai_protocol );

	// Asociamos el socket a un puerto

	bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	// Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	// Aceptamos un nuevo cliente
	int socket_cliente;

	socket_cliente = accept(socket_servidor, NULL, NULL);

	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}


// Implementacion de Otras Utilidades

int tamanioStringArray(char** a)
{
	int i = 0;
	while(a[i] != NULL)
	{
		i++;
	}
	return i;
}

void liberarStringArray(char** stringArray){

	int size = tamanioStringArray(stringArray);

	for(int i = 0; i< size; i++){

		free(stringArray[i]);


	}

	free(stringArray);

}

// esta funcion abre un archivo en modo lectura y retorna un puntero al "file"
FILE* abrir_archivo_lectura(char* path_pseudocodigo){
	FILE* file = fopen(path_pseudocodigo,"r");

	if(file == NULL) {
		// TO DO
		// log_error(logger, "Error al abrir archivo");
		printf("no se pudo abrir el archivo con la ruta especificada");
		exit(1);
	}

	return file;
};

// Definicion de funciones para armar/leer PCB

void mostrar_lista(nodo_instruccion* lista_instrucciones){
	nodo_instruccion* aux_ins = lista_instrucciones;
	nodo_parametro* aux_param = NULL;

	while(aux_ins != NULL){
		printf("\ninstruccion: %s\n", aux_ins->instruccion.identificador);
		aux_param = aux_ins->instruccion.parametros;

		while(aux_param != NULL){
			printf("\tparametro: %d\n", aux_param->parametro);
			aux_param = aux_param->sig;
		}

		aux_ins = aux_ins->sig;
	}
};

nodo_instruccion* armar_lista_instrucciones(t_list* lista){

	// agarro la primera instruccion
	t_list* nueva_instruccion = list_remove(lista, 0);

	// primer nodo de la lista de instrucciones, esto sera retornado por la funcion
	nodo_instruccion* lista_instrucciones = agregar_instruccion(NULL, nueva_instruccion);

	while(!list_is_empty(lista)){
		nueva_instruccion = list_remove(lista, 0);
		agregar_instruccion(lista_instrucciones, nueva_instruccion);
	}

	return lista_instrucciones;
}

nodo_instruccion* nuevo_nodo_instruccion(){
	nodo_instruccion* nodo = (struct nodo_instruccion*)malloc(sizeof(struct nodo_instruccion));
	nodo->sig = NULL;
	nodo->instruccion.parametros = NULL;
	return nodo;
}

nodo_parametro* nuevo_nodo_parametro(){
	nodo_parametro* nodo = (struct nodo_parametro*)malloc(sizeof(struct nodo_parametro));
	nodo->sig = NULL;
	return nodo;
}

nodo_instruccion* agregar_instruccion(nodo_instruccion* lista_instrucciones, void* buffer){

    // Lista de instrucciones esta vacia
    if(lista_instrucciones == NULL)
    {
        // genero el primer nodo de la lista de instrucciones y lo completo con los datos
        nodo_instruccion* nodo_primera_instruccion = nuevo_nodo_instruccion();
        completar_nodo_instruccion(nodo_primera_instruccion, buffer);

        return nodo_primera_instruccion;
    }
    else
    {
        // uso este auxiliar para recorrer la lista de instrucciones
        nodo_instruccion* aux = lista_instrucciones;

        // genero un nuevo nodo de la lista de instruccion y lo completo con los datos
        nodo_instruccion* nueva_instruccion = nuevo_nodo_instruccion();
        completar_nodo_instruccion(nueva_instruccion, buffer);

        // avanzo hasta el ultimo nodo de la lista
        while(aux->sig != NULL){
            aux = aux->sig;
        }
        aux->sig = nueva_instruccion;

        return lista_instrucciones;
    }

}

void completar_nodo_instruccion(nodo_instruccion* nodo_instruccion, char* buffer){
	// utilizamos strtok para complertar el identificador y los parametros de la instruccion
	char str[30];
	strcpy(str, buffer);
	char separator[] = " ";
	char* token;
	char* rest = str;

	// get the first token
	token = strtok_r(str, separator, &rest);

	// cargo el identificador de la instruccion
	strcpy(nodo_instruccion->instruccion.identificador, token);

	// leo el primer parametro si existiera
	token = strtok_r(NULL, separator, &rest);

	// si la lista de parametros no está iniciada Y buffer leyó al menos 1 parametro
	if(nodo_instruccion->instruccion.parametros == NULL && token != NULL){
		nodo_instruccion->instruccion.parametros = agregar_primer_parametro(token);
	}

	token = strtok_r(NULL, separator, &rest);

	// walk through other tokens
	while(token != NULL) {
		agregar_nuevo_parametro(nodo_instruccion, token);
		token = strtok_r(NULL, separator, &rest);
	}

};

nodo_parametro* agregar_primer_parametro(char* parametro){

	// genero el primer nodo de la lista de parametros
	nodo_parametro* nodo_primer_parametro = nuevo_nodo_parametro();
	// y lo completo con su valor
	nodo_primer_parametro->parametro = atoi(parametro);

	return nodo_primer_parametro;
};

void agregar_nuevo_parametro(nodo_instruccion* nodo_instruccion, char* parametro){

	// uso este auxiliar para recorrer la lista de parametros de la instruccion
	nodo_parametro* aux = nodo_instruccion->instruccion.parametros;

	// avanzo hasta el ultimo nodo de la lista
	while(aux->sig != NULL){
		aux = aux->sig;
	}

	aux->sig = nuevo_nodo_parametro();
	aux->sig->parametro = atoi(parametro);

};

