#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/sockets.h"

int conexionACPU;
int socket_escucha;
int socket_cliente;

void iniciar_servidor_memoria();
int manejarConexion(int socket_cliente);

#endif
