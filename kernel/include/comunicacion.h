#ifndef COMU_H_
#define COMU_H_

#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <inttypes.h>
#include <commons/log.h>

#include "../../shared/include/utils/utils.h"


typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;

void iterator(char* value);
int manejarConexion(void* void_args);
int server_escuchar(t_log* logger, char* server_name, int server_socket);


#endif
