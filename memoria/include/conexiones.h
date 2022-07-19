#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/sockets.h"


int serverMemoria;
int clienteMemoria;

void iniciar_servidor_memoria();
int manejarConexion(int socket_cliente);

#endif
