#ifndef COMUNICACION_H_
#define COMUNICACION_H_

#include "ciclo_instruccion.h"

void conexiones ();
int manejarConexion(int socket_cliente);
void servidorDispatch();
void servidorInterrupt();
void manejarInterrupt(int socket_cliente);


#endif
