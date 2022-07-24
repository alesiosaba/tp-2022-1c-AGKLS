#ifndef COMUNICACION_H_
#define COMUNICACION_H_

#include "ciclo_instruccion.h"

void conexiones ();
void servidorDispatch();
void servidorInterrupt();
void realizar_handshake_inicial();
void manejarInterrupt(int socket_cliente);
int manejarDispatch(int socket_cliente);


#endif
