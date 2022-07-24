#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/sockets.h"
#include "../../shared/include/utils/gestionPCB.h"
#include "../../shared/include/utils/protocolo.h"


int serverMemoria;
int clienteMemoria;

void iniciar_servidor_memoria();
int manejarDispatch(int socket_cliente);

#endif
