#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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

nodo_instruccion* armar_lista_instrucciones(char* path_pseudocodigo);
void agregar_nueva_instruccion(nodo_instruccion* lista_instrucciones, char* buffer);
FILE* abrir_archivo_lectura(char* path_pseudocodigo);

nodo_instruccion* nuevo_nodo_instruccion();
void completar_nodo_instruccion(nodo_instruccion* nodo_instruccion, char* buffer);

nodo_parametro* nuevo_nodo_parametro();
void agregar_nuevo_parametro(nodo_instruccion* nodo_instruccion, char* parametro);

// Ejemplo de ejecucion de modulo Consola 		./consola.c /ejemplo/pseudo.txt 128

int main() {

	// este nodo apuntar� al primer nodo de la lista de instrucciones para poder recorrerla
	nodo_instruccion* lista_instrucciones;

	char* path_pseudocodigo = "script.txt";

	// Leer archivo de instrucciones y armar la lista en memoria
	lista_instrucciones = armar_lista_instrucciones(path_pseudocodigo);

	// Mostrar por pantalla la lista de instrucciones
	printf("instruccion->identificador: %s\n", lista_instrucciones->instruccion.identificador);
	printf("instruccion->parametros->parametro: %d\n", lista_instrucciones->instruccion.parametros->parametro);
	printf("instruccion->parametros->sig->parametro: %d\n", lista_instrucciones->instruccion.parametros->sig->parametro);

	return 0;
}

nodo_instruccion* armar_lista_instrucciones(char* path_pseudocodigo){

	// primer nodo de la lista de instrucciones, esto sera retornado por la funcion
	nodo_instruccion* lista_instrucciones = nuevo_nodo_instruccion();

	// creo un puntero para leer el script
	FILE* file;
	file = abrir_archivo_lectura(path_pseudocodigo);

	// en buffer se almacena cada linea del script de codigo
	char buffer[1024];

	fgets(buffer,sizeof(buffer),file);
	printf("%s", buffer);
	agregar_nueva_instruccion(lista_instrucciones, buffer);

	while(!feof(file)){
		fgets(buffer,sizeof(buffer),file);
		printf("%s", buffer);
		agregar_nueva_instruccion(lista_instrucciones, buffer);
	}

	fclose(file);

	return lista_instrucciones;
}

// esta funcion abre un archivo en modo lectura y retorna un puntero al "file"
FILE* abrir_archivo_lectura(char* path_pseudocodigo){
	FILE* file = fopen(path_pseudocodigo,"r");

	if(file == NULL) {
      printf("ERROR OPENING FILE;");
      exit(-1);
	}

	return file;
};

nodo_instruccion* nuevo_nodo_instruccion(){
	nodo_instruccion* nodo = (struct nodo_instruccion*)malloc(sizeof(struct nodo_instruccion));
	nodo->sig = NULL;
	nodo->instruccion.parametros = nuevo_nodo_parametro();
	return nodo;
}

nodo_parametro* nuevo_nodo_parametro(){
	nodo_parametro* nodo = (struct nodo_parametro*)malloc(sizeof(struct nodo_parametro));
	nodo->sig = NULL;
	return nodo;
}

// esta funcion recibe un puntero al primer nodo de la lista y agrega un nodo al final (recibe la instruccion como WRITE 4 42 y lo parsea)
void agregar_nueva_instruccion(nodo_instruccion* lista_instrucciones, char* buffer){

	// uso este auxiliar para recorrer la lista de instrucciones
	nodo_instruccion* aux = lista_instrucciones;

	// avanzo hasta el ultimo nodo de la lista
	while(aux->sig != NULL){
		aux = aux->sig;
	}

	// apunto la ultima instruccion al nodo de la nueva instruccion
	nodo_instruccion* nueva_instruccion = nuevo_nodo_instruccion();
	aux->sig = nueva_instruccion;

	completar_nodo_instruccion(nueva_instruccion, buffer);
};

void completar_nodo_instruccion(nodo_instruccion* nodo_instruccion, char* buffer){
	// utilizamos strtok para complertar el identificador y los parametros de la instruccion
	char str[10];
	strcpy(str, buffer);
	char separator[] = " ";
	char* token;

	// get the first token
	token = strtok(str, separator);

	// cargo el identificador de la instruccion
	strcpy(nodo_instruccion->instruccion.identificador, token);
	// leo el primer parametro si existiera
	token = strtok(NULL, separator);

	// walk through other tokens
	while(token != NULL) {
		agregar_nuevo_parametro(nodo_instruccion, token);
		token = strtok(NULL, separator);
	}

};

void agregar_nuevo_parametro(nodo_instruccion* nodo_instruccion, char* parametro){

	// TO DO: USAR nuevo_nodo_parametro


	// uso este auxiliar para recorrer la lista de parametros de la instruccion
	nodo_parametro* aux = nodo_instruccion->instruccion.parametros;

	// avanzo hasta el ultimo nodo de la lista
	while(aux->sig != NULL){
		aux = aux->sig;
	}

	aux->parametro = atoi(parametro);

	aux->sig = nuevo_nodo

	NO_OP->I/O->WRITE->

		aux
	4->42->

};
