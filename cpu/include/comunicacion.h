#ifndef COMUNICACION_H_
#define COMUNICACION_H_

#include "ciclo_instruccion.h"

typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

void conexiones ();
void servidorDispatch();
void servidorInterrupt();
void realizar_handshake_inicial();
int manejarInterrupt(int socket_cliente);
int manejarDispatch(int socket_cliente);
void manejarMemoria(int socket_cliente);


#endif
