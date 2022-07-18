#ifndef COMU_H_
#define COMU_H_

#include "planificacion.h"


typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

void iterator(char* value);
void manejar_consolas(int server_fd);
int manejarConexion(void* void_args);
int server_escuchar(t_log* logger, char* server_name, int server_socket);
void manejar_cpu(int socket_fd);

#endif
