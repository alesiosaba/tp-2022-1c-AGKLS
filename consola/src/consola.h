#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>


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

void mostrar_lista(nodo_instruccion* lista_instrucciones);
nodo_instruccion* armar_lista_instrucciones(char* path_pseudocodigo);
nodo_instruccion* agregar_primera_instruccion(char* buffer);
void agregar_nueva_instruccion(nodo_instruccion* lista_instrucciones, char* buffer);
FILE* abrir_archivo_lectura(char* path_pseudocodigo);

nodo_instruccion* nuevo_nodo_instruccion();
void completar_nodo_instruccion(nodo_instruccion* nodo_instruccion, char* buffer);

nodo_parametro* nuevo_nodo_parametro();
nodo_parametro* agregar_primer_parametro(char* parametro);
void agregar_nuevo_parametro(nodo_instruccion* nodo_instruccion, char* parametro);
