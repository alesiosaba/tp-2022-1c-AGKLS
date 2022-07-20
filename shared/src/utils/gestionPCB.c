#include "../../include/utils/gestionPCB.h"

void iterator(char* value) {
	log_info(logger,"%s", value);
}

// esta funcion se usa para deserializar un PCB "nuevo" enviado desde consola a kernel
pcb* armar_PCB_nuevo(t_list* lista){
	pcb* pcb = (struct pcb*)malloc(sizeof(struct pcb));
	pcb->instrucciones = NULL;
	t_list* tamanio_proceso = list_remove(lista, 0);
	pcb->tamanio= atoi(tamanio_proceso);
	pcb->instrucciones = armar_lista_instrucciones(lista);
	pcb->program_counter = pcb->instrucciones;
	pcb->status = NEW;

	return pcb;
}

// esta funcion se usa para deserializar un PCB enviado entre kernel, cpu, memoria, etc.
pcb* deserializar_PCB(t_list* lista){

	pcb* pcb = (struct pcb*)malloc(sizeof(struct pcb));
	pcb->instrucciones = NULL;


	// TODO:
	// ESTA FUNCION SE CORRESPONDE CON generar_paquete_pcb()

	// agarro el primer elemento (id)
	t_list* id = list_remove(lista, 0);
	pcb->id = atoi(id);

	// agarro el segundo elemento (tamanio)
	t_list* tamanio = list_remove(lista, 0);
	pcb->tamanio = atoi(tamanio);

	// agarro el tercer elemento (state)
	t_list* status = list_remove(lista, 0);
	pcb->status = atoi(status);

	// agarro el tercer elemento (state)
	t_list* estimacion = list_remove(lista, 0);
	pcb->estimacion_rafaga = atof(estimacion);

	// armo la lista de instrucciones y la guardo en el PCB
	pcb->instrucciones = deserializar_lista_instrucciones(lista);

	return pcb;
}

char* generar_renglon_instruccion(struct instruccion instruccion_a_enviar){
	char* renglon_instruccion = string_new();

	// Extraigo el codigo de instruccion
	string_append(&renglon_instruccion, instruccion_a_enviar.identificador);

	// Genero un puntero auxiliar para recorrer la lista de parametros
	struct nodo_parametro* parametros_aux = instruccion_a_enviar.parametros;

	// Si la instruccion no tiene parametros directamente devuelve el renglon
	if(parametros_aux != NULL){
		char* parametroStr =  string_new();
		// Caso que la instruccion tenga al menos 1 parametro
		do{
			string_append(&renglon_instruccion, " ");
			sprintf(parametroStr, "%d", parametros_aux->parametro);
			string_append(&renglon_instruccion, parametroStr);
			parametros_aux = parametros_aux->sig;
		}while(parametros_aux != NULL);
	}

	string_append(&renglon_instruccion,"\0");
	string_trim_right(&renglon_instruccion);

	return renglon_instruccion;

}

// esta funcion se utiliza para generar el paquete de PCB para serializarlo desde kernel, cpu, memoria, etc.
t_paquete* generar_paquete_pcb(struct pcb PCB_a_enviar){

	t_paquete* paquete = crear_paquete(PAQUETE_PCB);
	nodo_instruccion* nodo_instruccion = PCB_a_enviar.instrucciones;

	char* id =  string_new();
	sprintf(id, "%d\0", PCB_a_enviar.id);
	agregar_a_paquete(paquete, id, strlen(id) + 1);

	char* tamanio = string_new();
	sprintf(tamanio, "%d\0", PCB_a_enviar.tamanio);
	agregar_a_paquete(paquete, tamanio, strlen(tamanio));

	char* state = string_new();
	sprintf(state, "%d\0", PCB_a_enviar.status);
	agregar_a_paquete(paquete, state, strlen(state));

	char estimacion[100];
	sprintf(estimacion, "%f\0", PCB_a_enviar.estimacion_rafaga);
	agregar_a_paquete(paquete, estimacion, strlen(estimacion));

	// double tabla_paginas = PCB_a_enviar.tabla_paginas;

	// Este caso es distinto porque no hay conversion directa desde double a char*
	/*char estimacion_rafaga[sizeof(PCB_a_enviar.estimacion_rafaga)];
	memcpy(
			estimacion_rafaga
			, &PCB_a_enviar.estimacion_rafaga
			, sizeof(PCB_a_enviar.estimacion_rafaga)
	);*/

	// agregar_a_paquete(paquete, (char*) tabla_paginas, sizeof(tabla_paginas));
	// agregar_a_paquete(paquete, estimacion_rafaga, sizeof(estimacion_rafaga));

	// buffer para concatenar instruccion y sus parametros
	char* renglon_instruccion = string_new();

	//GENERAR LISTA DE INSTRUCCIONES COMO RENGLONES

	while(nodo_instruccion != NULL){
		renglon_instruccion = generar_renglon_instruccion(nodo_instruccion->instruccion);
		int size = strlen(renglon_instruccion) + 1;
		agregar_a_paquete(paquete, renglon_instruccion, size);
		nodo_instruccion = nodo_instruccion->sig;
	}


	return paquete;

}

// Definicion de funciones para armar/leer PCB

void mostrar_lista_instrucciones(nodo_instruccion* lista_instrucciones){
	nodo_instruccion* aux_ins = lista_instrucciones;
	nodo_parametro* aux_param = NULL;

	while(aux_ins != NULL){

		printf("\ninstruccion: %s\n",aux_ins->instruccion.identificador);

		aux_param = aux_ins->instruccion.parametros;
		while(aux_param != NULL){
			printf("\tparametro: %d\n", aux_param->parametro);
			aux_param = aux_param->sig;
		}

		aux_ins = aux_ins->sig;
	}
};

nodo_instruccion* armar_lista_instrucciones(t_list* lista){

	// agarro la primera instruccion
	t_list* nueva_instruccion = list_remove(lista, 0);

	// primer nodo de la lista de instrucciones, esto sera retornado por la funcion
	nodo_instruccion* lista_instrucciones = agregar_instruccion(NULL, nueva_instruccion);

	while(!list_is_empty(lista)){
		nueva_instruccion = list_remove(lista, 0);
		agregar_instruccion(lista_instrucciones, nueva_instruccion);
	}

	return lista_instrucciones;
}

// esta funcion se usa para armar la lista de inst en deserializar_PCB()
nodo_instruccion* deserializar_lista_instrucciones(t_list* lista){

	// agarro la primera instruccion
	t_list* nueva_instruccion = list_remove(lista, 0);

	// primer nodo de la lista de instrucciones, esto sera retornado por la funcion
	nodo_instruccion* lista_instrucciones = deserializar_instruccion(NULL, nueva_instruccion);

	while(!list_is_empty(lista)){
		nueva_instruccion = list_remove(lista, 0);
		deserializar_instruccion(lista_instrucciones, nueva_instruccion);
	}

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

nodo_instruccion* agregar_instruccion(nodo_instruccion* lista_instrucciones, void* buffer){

	// utilizamos strtok para complertar el identificador y los parametros de la instruccion
		char str[30];
		strcpy(str, buffer);
		char separator[] = " ";
		char* token;
		char* rest = str;

		// get the first token
		token = strtok_r(str, separator, &rest);


    // Lista de instrucciones esta vacia
    if(lista_instrucciones == NULL)
    {
        // genero el primer nodo de la lista de instrucciones y lo completo con los datos
        nodo_instruccion* nodo_primera_instruccion = nuevo_nodo_instruccion();

        // preguntar que instruccion es
        // Caso que el codigo de operacion == NO_OP
        if(strcmp(token,"NO_OP") == 0){
        	// Esto genera el primer nodo para el caso de NO_OP
			completar_nodo_instruccion(nodo_primera_instruccion, "NO_OP");

			// leo el primer parametro (cantidad de NO_OP a ejecutar)
			token = strtok_r(NULL, separator, &rest);

			// Restamos 1 porque ya agregamos el primer nodo anteriormente
			int i = atoi(token) - 1;

			// uso este auxiliar para recorrer la lista de instrucciones
			nodo_instruccion* aux = nodo_primera_instruccion;

			while(i > 0){
				// Lo reinicio para cada nodo de NO_OP restante
				aux = nodo_primera_instruccion;

				// genero un nuevo nodo de la lista de instruccion y lo completo con los datos
				nodo_instruccion* nueva_instruccion = nuevo_nodo_instruccion();
				completar_nodo_instruccion(nueva_instruccion, "NO_OP");

				// avanzo hasta el ultimo nodo de la lista
				while(aux->sig != NULL){
					aux = aux->sig;
				}
				aux->sig = nueva_instruccion;

				i--;
			}

			return nodo_primera_instruccion;
        }

        // Caso que el codigo de operacion NO es NO_OP
        completar_nodo_instruccion(nodo_primera_instruccion, buffer);

        return nodo_primera_instruccion;
    }
    else
    {
        // uso este auxiliar para recorrer la lista de instrucciones
        nodo_instruccion* aux = lista_instrucciones;

		if(strcmp(token,"NO_OP") == 0){
			// leo el primer parametro (cantidad de NO_OP a ejecutar)
			token = strtok_r(NULL, separator, &rest);

			// Cantidad de nuevos nodos de NO_OP
			int i = atoi(token);

			while(i > 0){
				// Lo reinicio para cada nodo de NO_OP restante
				aux = lista_instrucciones;

				// genero un nuevo nodo de la lista de instruccion y lo completo con los datos
				nodo_instruccion* nueva_instruccion = nuevo_nodo_instruccion();
				completar_nodo_instruccion(nueva_instruccion, "NO_OP");

				// avanzo hasta el ultimo nodo de la lista
				while(aux->sig != NULL){
					aux = aux->sig;
				}
				aux->sig = nueva_instruccion;

				i--;
			}

			return lista_instrucciones;
		}

		// Caso de un nuevo nodo que no es NO_OP

        // genero un nuevo nodo de la lista de instruccion y lo completo con los datos
        nodo_instruccion* nueva_instruccion = nuevo_nodo_instruccion();

        completar_nodo_instruccion(nueva_instruccion, buffer);

        // avanzo hasta el ultimo nodo de la lista
        while(aux->sig != NULL){
            aux = aux->sig;
        }
        aux->sig = nueva_instruccion;

        return lista_instrucciones;
    }

}

// esta funcion se usa en deserializar_lista_instrucciones()
nodo_instruccion* deserializar_instruccion(nodo_instruccion* lista_instrucciones, void* buffer){

    // Lista de instrucciones esta vacia
    if(lista_instrucciones == NULL)
    {
        // genero el primer nodo de la lista de instrucciones y lo completo con los datos
        nodo_instruccion* nodo_primera_instruccion = nuevo_nodo_instruccion();

        // Completo el nodo de la primera instruccion
        completar_nodo_instruccion(nodo_primera_instruccion, buffer);

        return nodo_primera_instruccion;
    }
    else
    {
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

        return lista_instrucciones;
    }
}

void completar_nodo_instruccion(nodo_instruccion* nodo_instruccion, char* buffer_original){

	// Limpiamos los caracteres basura del final del buffer
	char* buffer = string_new();
	strcpy(buffer, buffer_original);
	string_trim_right(&buffer);

	// utilizamos strtok para completar el identificador y los parametros de la instruccion
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

// esta funcion sirve para recorrer cualquier PCB ya sea en kernel, cpu, etc.
void imprimir_PCB(pcb* nodo_pcb){
	printf("ID %d", nodo_pcb->id);
	printf("\nSTATUS %d", nodo_pcb->status);
	printf("\nEST %f", nodo_pcb->estimacion_rafaga);

	mostrar_lista_instrucciones(nodo_pcb->instrucciones);

}

//FOR TESTING
pcb* generar_pcb_dummy(){


}

