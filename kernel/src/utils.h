#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<assert.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/list.h>

// Definiciones Utiles

#define IP "127.0.0.1"
#define PUERTO "4444"
#define ARCHIVO_DE_CONFIGURACION "kernel.config"
#define ARCHIVO_DE_LOG "kernel.log"
#define LOGGER "Kernel Log"

// Mensajes y Returns

#define ERROR_AL_ABRIR_ARCHIVO "Error al abrir el Archivo"
#define TERMINANDO_EL_LOG "Terminando el Log"
#define CONFIGURACION_CERRADA "Archivo de configuracion cerrado"
#define SERVIDOR_LISTO "Servidor listo para recibir al cliente"
#define SERVIDOR_DESCONEXION "el cliente se desconecto. Terminando servidor"
#define OPERACION_DESCONOCIDA "Operacion desconocida. No quieras meter la pata"
#define LECTURA_DE_VALORES "Me llegaron los siguientes valores:\n"

// Nuevos tipos y estructuras de datos

typedef enum
{
	MENSAJE,
	PAQUETE
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

//Variables globales

t_log* logger;
t_config* config;


// Definicion de Implementacion obligatoria (Inicialización & Terminacion)

void inicializar();
void terminar_programa();


// Definicion de Comportamientos Logger & Config

t_log* iniciar_logger(char*);
t_config* iniciar_config(void);


// Definicion de Comportamientos de Cliente

int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(void);
t_paquete* crear_super_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);

// Definicion de Comportamientos de Servidor

void* recibir_buffer(int*, int);

int iniciar_servidor(void);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);

// Definicion de otras utilidades
void liberarStringArray(char** stringArray);
int tamanioStringArray(char** a);


#endif /* UTILS_H_ */


