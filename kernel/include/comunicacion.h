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
<<<<<<< HEAD
void conectar_memoria();
=======
void manejar_memoria(int socket_fd);
>>>>>>> 9dbf26a45ea18a152f5fe8dfa74492056f165985

#endif
