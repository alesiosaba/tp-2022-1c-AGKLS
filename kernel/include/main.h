#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <readline/readline.h>
#include <pthread.h>
#include "init.h"
#include "../../shared/include/utils/utils.h"

config_t config_values;

// Definiciones de Inicio y Fin de programa
void terminar_programa();

// Definiciones Utiles
void iterator(char* value);
int manejarConexion(int socket_cliente);
void manejar_consolas(int server_fd);

#endif
