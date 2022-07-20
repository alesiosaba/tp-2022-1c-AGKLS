#ifndef COMUNICACION_H_
#define COMUNICACION_H_

#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/sockets.h"
#include "../../shared/include/utils/protocolo.h"

void conexiones ();
int manejarConexion(int socket_cliente);
void servidorDispatch();
void servidorInterrupt();

#endif
