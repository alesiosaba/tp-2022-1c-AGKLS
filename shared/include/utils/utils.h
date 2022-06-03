#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <assert.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>

// Definiciones Utiles

#define IP "127.0.0.1"

// Mensajes y Returns

#define ERROR_AL_ABRIR_ARCHIVO "Error al abrir el Archivo"
#define TERMINANDO_EL_LOG "Terminando el Log"
#define CONFIGURACION_CERRADA "Archivo de configuracion cerrado"
#define SERVIDOR_LISTO "Servidor listo para recibir al cliente"
#define SERVIDOR_AUSENTE "El Servidor no se encuentra inicializado. Error al conectar con"
#define SERVIDOR_DESCONEXION "el cliente se desconecto. Terminando servidor"
#define OPERACION_DESCONOCIDA "Operacion desconocida. No quieras meter la pata"
#define LECTURA_DE_VALORES "Me llegaron los siguientes valores:\n"
#define RECEPCION_PAQUETE_CONSOLA "Recibi un paquete de consola"

// Nuevos tipos y estructuras de datos

typedef enum
{
	MENSAJE,
	PAQUETE,
	PAQUETE_CONSOLA
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

// Estructuras de datos para el PCB

typedef struct nodo_parametro
{
	int parametro;
    struct nodo_parametro* sig;
} nodo_parametro;

typedef struct instruccion
{
	char identificador[10];
    struct nodo_parametro* parametros;
} instruccion;

typedef struct nodo_instruccion
{
	instruccion instruccion;
    struct nodo_instruccion* sig;
} nodo_instruccion;

typedef struct t_pcb
{
	int id;
	int tamanio;
	struct nodo_instruccion* instrucciones;
	struct nodo_instruccion* program_counter;
	int tabla_paginas;
	double estimacion_rafaga;
} t_pcb;

//Variables globales

t_log* logger;
t_config* config;
int conexion;

// Definicion de Implementacion obligatoria (Inicialización & Terminacion)


// Definicion de Comportamientos Logger & Config

t_log* iniciar_logger(char* log_level, char* logger_path, char* logger);
t_config* iniciar_config(char* config_path);


// Definicion de Comportamientos de Cliente

int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(op_code codigo_operacion);
t_paquete* crear_super_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int *socket_cliente);
void eliminar_paquete(t_paquete* paquete);

// Definicion de Comportamientos de Servidor

void* recibir_buffer(int*, int);

int iniciar_servidor(char*, char*);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);

// Definicion de funciones para armar/leer PCB
// listas
void mostrar_lista(nodo_instruccion* lista_instrucciones);
nodo_instruccion* armar_lista_instrucciones(t_list* lista);
// instrucciones
nodo_instruccion* agregar_instruccion(nodo_instruccion* lista_instrucciones, void* buffer);
// nodos instrucciones
nodo_instruccion* nuevo_nodo_instruccion();
void completar_nodo_instruccion(nodo_instruccion* nodo_instruccion, char* buffer);
// nodos parametro
nodo_parametro* nuevo_nodo_parametro();
// parametros
nodo_parametro* agregar_primer_parametro(char* parametro);
void agregar_nuevo_parametro(nodo_instruccion* nodo_instruccion, char* parametro);

// Definicion de otras utilidades
void liberarStringArray(char** stringArray);
int tamanioStringArray(char** a);
FILE* abrir_archivo_lectura(char* path_pseudocodigo);


#endif /* UTILS_H_ */
