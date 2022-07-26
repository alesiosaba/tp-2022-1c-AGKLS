#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <sys/socket.h>
#include "utils.h"

#define IP "127.0.0.1"

// Definicion de Comportamientos de Cliente

int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(op_code codigo_operacion);
t_paquete* crear_super_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int *socket_cliente);
bool eliminar_paquete(t_paquete* paquete);
int enviar_num(int conexion, int num, t_log *logger);

// Definicion de Comportamientos de Servidor

void* recibir_buffer(int*, int);
int iniciar_servidor(char*, char*);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);


// Memoria
// Serializacion
void* serializar_tabla_N2(op_code op, t_tablaN2 *t, size_t *size);

// Envio
void enviar_tabla_N2(int socket_cliente, t_tablaN2 *t, t_log *logger);


#endif
