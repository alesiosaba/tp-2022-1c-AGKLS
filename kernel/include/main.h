#ifndef MAIN_H_
#define MAIN_H_


#include "../include/conexiones.h"

// Definiciones de Inicio y Fin de programa
void terminar_programa();

// Definiciones Utiles
void iterator(char* value);
//int manejarConexion(int socket_cliente);
void manejar_consolas(int server_fd);
void manejar_cpu(int socket_fd);

void matar_hilos();
void esperar_hilos();

void recibirComandos();

#endif
