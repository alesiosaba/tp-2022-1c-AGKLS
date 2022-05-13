#include "../include/main.h"

int main(int argc, char** argv) {

	inicializar();


	// Chequear que están los 2 parametros que pide el TP
	// Path al archivo de pseudocodigo y tamanio de espacio de direcciones del proceso

	if(argc != 3){
		// TO DO
		//log_error(logger , "Cantidad erronea de parametros");
		printf("cantidad erronea parametros");
		return EXIT_FAILURE;
	}

	// Asigno parametros enviados por consola
	char* path_pseudocodigo = argv[1];
	int tamanio_proceso = atoi(argv[2]);


	// este nodo apuntara al primer nodo de la lista de instrucciones para poder recorrerla
	nodo_instruccion* lista_instrucciones;

	// Leer archivo de instrucciones y armar la lista en memoria
	lista_instrucciones = armar_lista_instrucciones(path_pseudocodigo);

	// Mostrar por pantalla la lista de instrucciones:
	mostrar_lista(lista_instrucciones);

	// log_info(logger, "Finalizo correctamente el programa");
	return EXIT_SUCCESS;
}
