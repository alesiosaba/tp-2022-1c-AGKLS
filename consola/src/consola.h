#include <stdio.h>
#include <stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>

typedef enum
{
	NO_OP,
    IO,
	READ,
    COPY,
    WRITE,
    EXIT
} identificador_instruccion;

typedef struct
{
	identificador_instruccion identificador;
    int** parametros;
} instruccion;

instruccion** armar_lista_instrucciones(char* path_pseudocodigo);