#include "../include/parserFile.h"

// esta funcion abre un archivo en modo lectura y retorna un puntero al "file"
FILE* abrir_archivo_lectura(char* path_pseudocodigo){
	FILE* file = fopen(path_pseudocodigo,"r");

	if(file == NULL) {
		// TO DO
		// log_error(logger, "Error al abrir archivo");
		printf("no se pudo abrir el archivo con la ruta especificada");
		exit(1);
	}

	return file;
};

void mostrar_lista(nodo_instruccion* lista_instrucciones){
	nodo_instruccion* aux_ins = lista_instrucciones;
	nodo_parametro* aux_param = NULL;

	while(aux_ins != NULL){
		printf("\ninstruccion: %s\n", aux_ins->instruccion.identificador);
		aux_param = aux_ins->instruccion.parametros;

		while(aux_param != NULL){
			printf("\tparametro: %d\n", aux_param->parametro);
			aux_param = aux_param->sig;
		}

		aux_ins = aux_ins->sig;
	}
};

nodo_instruccion* armar_lista_instrucciones(char* path_pseudocodigo){

	// creo un puntero para leer el script
	FILE* file;
	file = abrir_archivo_lectura(path_pseudocodigo);

	// en buffer se almacena cada linea del script de codigo
	char buffer[1024];

	fgets(buffer,sizeof(buffer),file);
	// printf("%s", buffer);

	// primer nodo de la lista de instrucciones, esto sera retornado por la funcion
	// para agregar este nodo utilizamos una funcion distinta porque en C no hay referencia
	nodo_instruccion* lista_instrucciones = agregar_primera_instruccion(buffer);

	while(!feof(file)){
		fgets(buffer,sizeof(buffer),file);
		// printf("%s", buffer);
		agregar_nueva_instruccion(lista_instrucciones, buffer);
	}

	fclose(file);

	return lista_instrucciones;
}

nodo_instruccion* nuevo_nodo_instruccion(){
	nodo_instruccion* nodo = (struct nodo_instruccion*)malloc(sizeof(struct nodo_instruccion));
	nodo->sig = NULL;
	nodo->instruccion.parametros = NULL;
	return nodo;
}

nodo_parametro* nuevo_nodo_parametro(){
	nodo_parametro* nodo = (struct nodo_parametro*)malloc(sizeof(struct nodo_parametro));
	nodo->sig = NULL;
	return nodo;
}

nodo_instruccion* agregar_primera_instruccion(char* buffer){

	// genero el primer nodo de la lista de instrucciones y lo completo con los datos
	nodo_instruccion* nodo_primera_instruccion = nuevo_nodo_instruccion();
	completar_nodo_instruccion(nodo_primera_instruccion, buffer);

	return nodo_primera_instruccion;
}


// esta funcion recibe un puntero al primer nodo de la lista y agrega un nodo al final (recibe la instruccion como WRITE 4 42 y lo parsea)
void agregar_nueva_instruccion(nodo_instruccion* lista_instrucciones, char* buffer){

	// uso este auxiliar para recorrer la lista de instrucciones
	nodo_instruccion* aux = lista_instrucciones;

	// genero un nuevo nodo de la lista de instruccion y lo completo con los datos
	nodo_instruccion* nueva_instruccion = nuevo_nodo_instruccion();
	completar_nodo_instruccion(nueva_instruccion, buffer);

	// avanzo hasta el ultimo nodo de la lista
	while(aux->sig != NULL){
		aux = aux->sig;
	}

	aux->sig = nueva_instruccion;

};

void completar_nodo_instruccion(nodo_instruccion* nodo_instruccion, char* buffer){
	// utilizamos strtok para complertar el identificador y los parametros de la instruccion
	char str[30];
	strcpy(str, buffer);
	char separator[] = " ";
	char* token;
	char* rest = str;

	// get the first token
	token = strtok_r(str, separator, &rest);

	// cargo el identificador de la instruccion
	strcpy(nodo_instruccion->instruccion.identificador, token);

	// leo el primer parametro si existiera
	token = strtok_r(NULL, separator, &rest);

	// si la lista de parametros no está iniciada Y buffer leyó al menos 1 parametro
	if(nodo_instruccion->instruccion.parametros == NULL && token != NULL){
		nodo_instruccion->instruccion.parametros = agregar_primer_parametro(token);
	}

	token = strtok_r(NULL, separator, &rest);

	// walk through other tokens
	while(token != NULL) {
		agregar_nuevo_parametro(nodo_instruccion, token);
		token = strtok_r(NULL, separator, &rest);
	}

};

nodo_parametro* agregar_primer_parametro(char* parametro){

	// genero el primer nodo de la lista de parametros
	nodo_parametro* nodo_primer_parametro = nuevo_nodo_parametro();
	// y lo completo con su valor
	nodo_primer_parametro->parametro = atoi(parametro);

	return nodo_primer_parametro;
};

void agregar_nuevo_parametro(nodo_instruccion* nodo_instruccion, char* parametro){

	// uso este auxiliar para recorrer la lista de parametros de la instruccion
	nodo_parametro* aux = nodo_instruccion->instruccion.parametros;

	// avanzo hasta el ultimo nodo de la lista
	while(aux->sig != NULL){
		aux = aux->sig;
	}

	aux->sig = nuevo_nodo_parametro();
	aux->sig->parametro = atoi(parametro);

};
