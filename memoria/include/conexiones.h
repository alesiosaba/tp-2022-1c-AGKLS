#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/sockets.h"
#include "../../shared/include/utils/gestionPCB.h"
#include "../../shared/include/utils/protocolo.h"

int serverMemoriaCPU;
int serverMemoriaKernel;

int clienteMemoriaCPU;
int clienteMemoriaKernel;

void iniciar_servidor_memoriaCPU();
void iniciar_servidor_memoriaKernel();
int manejarConexionKernel(int socket_cliente);
int manejarConexionCPU(int socket_cliente);
void handshake_inicial();

#endif
