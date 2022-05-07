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

// Ejemplo de ejecucion de modulo Consola 		./consola.c /ejemplo/pseudo.txt 128

int main() {

	// este nodo apuntara al primer nodo de la lista de instrucciones para poder recorrerla
	nodo_instruccion* lista_instrucciones;

	char* path_pseudocodigo = "script.txt";

	// Leer archivo de instrucciones y armar la lista en memoria
	lista_instrucciones = armar_lista_instrucciones(path_pseudocodigo);

	// Mostrar por pantalla la lista de instrucciones:
	mostrar_lista(lista_instrucciones);

	return 0;
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
