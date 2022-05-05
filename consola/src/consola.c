#include "consola.h"

// Ejemplo de ejecucion de modulo Consola
// ./consola.c /ejemplo/pseudo.txt 128

int main(int argc, char** argv) {
	
	// Chequear que están los 2 parametros que pide el TP
	// Path al archivo de pseudocodigo y tamanio de espacio de direcciones del proceso

	if(argc != 2){	
		// TO DO
		// Loguear que se mandó cantidad erronea de parametros
		return EXIT_FAILURE;
	}

	char* path_pseudocodigo = argv[0];
	int tamanio_proceso = argv[1];

	instruccion** lista_instrucciones;

	// Leer archivo de instrucciones
	lista_instrucciones = armar_lista_instrucciones(path_pseudocodigo);

	// Crear conexion hacia Kernel


	return EXIT_SUCCESS;
}

instruccion** armar_lista_instrucciones(char* path_pseudocodigo){
	instruccion** lista_instrucciones;

	int cantidad_instrucciones = cantidad_lineas

	lista_instrucciones = malloc(sizeof(instruccion*) * 4);

}