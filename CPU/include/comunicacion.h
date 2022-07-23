#ifndef COMUNICACION_H_
#define COMUNICACION_H_

#include "ciclo_instruccion.h"

void conexiones ();
int manejarConexion(int socket_cliente);
void servidorDispatch();
void servidorInterrupt();
<<<<<<< HEAD
void realizar_handshake_inicial();
=======
void manejarInterrupt(int socket_cliente);

>>>>>>> a8a90ecc9151d2b57828d496c9edc847f7e68b02

#endif
