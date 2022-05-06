#include<stdio.h>
#include<stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <readline/readline.h>
#include <pthread.h>
#include "utils.h"


typedef struct nodo_parametro
{
	int parametro;
    struct nodo_parametro* sig;
} nodo_parametro;

typedef struct instruccion
{
	char identificador[10];
    struct nodo_parametro* parametros;
} instruccion;

typedef struct nodo_instruccion
{
	instruccion instruccion;
    struct nodo_instruccion* sig;
} nodo_instruccion;


typedef struct
{
	int id;
	int tamanio;
	struct nodo_instruccion* instrucciones;
	int program_counter;
	int tabla_paginas;
	int estimacion_rafaga;
} t_pcb;

void iterator(char* value);
int manejarConexion(int socket_cliente);
void manejar_consolas(int server_fd);
