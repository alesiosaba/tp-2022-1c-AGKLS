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

	// creamos una lista y la completa la funcion armar_lista_instrucciones()
	pcb->instrucciones = list_create();
	armar_lista_instrucciones(lista, pcb->instrucciones);

    pcb->program_counter = 0;
	// la tabla de paginas se asigna cuando el proceso pasa a READY (solicita a memoria la tabla de pag)
	pcb->tabla_paginas = NULL;
	pcb->status = NEW;
	pcb->tiempo_a_bloquearse = 0;

	return pcb;
}

// esta funcion se usa para deserializar un PCB enviado entre kernel, cpu, memoria, etc.
	// esta funcion se corresponde con generar_paquete_pcb()
pcb* deserializar_PCB(t_list* lista){

	pcb* pcb = (struct pcb*)malloc(sizeof(struct pcb));
	pcb->instrucciones = list_create();

	// agarro el primer elemento (id)
	t_list* id = list_remove(lista, 0);
	pcb->id = atoi(id);

	// agarro el segundo elemento (tamanio)
	t_list* tamanio = list_remove(lista, 0);
	pcb->tamanio = atoi(tamanio);

	// agarro el tercer elemento (program_counter)
	t_list* program_counter = list_remove(lista, 0);
	pcb->program_counter = atoi(program_counter);

	// agarro el cuarto elemento (tabla_paginas)
	t_list* tabla_paginas = list_remove(lista, 0);
	pcb->tabla_paginas = atoi(tabla_paginas);

	// agarro el quinto elemento (estimacion)
	t_list* estimacion = list_remove(lista, 0);
	pcb->estimacion_rafaga = atof(estimacion);

	// agarro el sexto elemento (status)
	t_list* status = list_remove(lista, 0);
	pcb->status = str_to_status_enum(status);

	// agarro el septimo elemento (tiempo_a_bloquearse)
	t_list* tiempo_a_bloquearse = list_remove(lista, 0);
	pcb->tiempo_a_bloquearse = atoi(tiempo_a_bloquearse);

	// armo la lista de instrucciones y la guardo en el PCB
	deserializar_lista_instrucciones(lista, pcb->instrucciones);

	return pcb;
}

status str_to_status_enum(char* str){
	int j;
	     for (j = 0;  j < sizeof (conversion_status) / sizeof (conversion_status[0]);  ++j)
	         if (atoi(str) != conversion_status[j].str)
	             return conversion_status[j].val;
}

char* generar_renglon_instruccion(struct instruccion* instruccion_a_enviar){

	char* renglon_instruccion = string_new();

	// Extraigo el codigo de instruccion
	string_append(&renglon_instruccion, instruccion_a_enviar->identificador);

	// Si la instruccion no tiene parametros directamente devuelve el renglon
	if(!list_is_empty(instruccion_a_enviar->parametros)){
		char* parametroStr =  string_new();

		// Caso que la instruccion tenga al menos 1 parametro
		int i = 0;
		while(i < list_size(instruccion_a_enviar->parametros)){

			string_append(&renglon_instruccion, " ");

			nodo_parametro* param = list_get(instruccion_a_enviar->parametros, i);
			sprintf(parametroStr, "%d", param->parametro);

			string_append(&renglon_instruccion, parametroStr);

			i++;
		}
	}

	string_append(&renglon_instruccion,"\0");
	string_trim_right(&renglon_instruccion);

	return renglon_instruccion;

}

// esta funcion se utiliza para generar el paquete de PCB para serializarlo desde kernel, cpu, memoria, etc.
t_paquete* generar_paquete_pcb(struct pcb PCB_a_enviar, op_code codigo_paquete){

	t_paquete* paquete = crear_paquete(codigo_paquete);
	t_list* lista_instrucciones = PCB_a_enviar.instrucciones;

	// pcb -> id
	char* id =  string_new();
	sprintf(id, "%d\0", PCB_a_enviar.id);
	agregar_a_paquete(paquete, id, strlen(id) + 1);

	// pcb -> tamanio
	char* tamanio = string_new();
	sprintf(tamanio, "%d\0", PCB_a_enviar.tamanio);
	agregar_a_paquete(paquete, tamanio, strlen(tamanio) + 1);

	// pcb -> program_counter
	char* program_counter = string_new();
	sprintf(program_counter, "%d\0", PCB_a_enviar.program_counter);
	agregar_a_paquete(paquete, program_counter, strlen(program_counter) + 1);

	// pcb -> tabla_paginas
	char* tabla_paginas = string_new();
	sprintf(tabla_paginas, "%d\0", PCB_a_enviar.tabla_paginas);
	agregar_a_paquete(paquete, tabla_paginas, strlen(tabla_paginas) + 1);

	// pcb -> estimacion
	char estimacion[100];
	sprintf(estimacion, "%f\0", PCB_a_enviar.estimacion_rafaga);
	agregar_a_paquete(paquete, estimacion, strlen(estimacion) + 1);

	// pcb -> status
	char* status = string_new();
	sprintf(status, "%d\0", PCB_a_enviar.status);
	agregar_a_paquete(paquete, status, strlen(status));

	// pcb -> tiempo_a_bloquearse
	char* tiempo_a_bloquearse = string_new();
	sprintf(tiempo_a_bloquearse, "%d\0", PCB_a_enviar.tiempo_a_bloquearse);
	agregar_a_paquete(paquete, tiempo_a_bloquearse, strlen(tiempo_a_bloquearse) + 1);

	// pcb -> instrucciones
	// buffer para concatenar instruccion y sus parametros
	char* renglon_instruccion = string_new();
	//GENERAR LISTA DE INSTRUCCIONES COMO RENGLONES
	int i = 0;
	while(i < list_size(lista_instrucciones)){

		renglon_instruccion = generar_renglon_instruccion(list_get(lista_instrucciones, i));
		int size = strlen(renglon_instruccion) + 1;
		agregar_a_paquete(paquete, renglon_instruccion, size);

		i++;
	}

	return paquete;
}

// Definicion de funciones para armar/leer PCB

void mostrar_lista_instrucciones(t_list* lista_instrucciones){

	printf("\nLista de instrucciones:\n");

	int i = 0;
	while(i < list_size(lista_instrucciones)){

		nodo_instruccion* nodo_instruccion = list_get(lista_instrucciones, i);
		printf("\tinstruccion: %s\n", nodo_instruccion->instruccion.identificador);

		int x = 0;
		while(x < list_size(nodo_instruccion->instruccion.parametros)){
			nodo_parametro* nodo_parametro = list_get(nodo_instruccion->instruccion.parametros, x);
			printf("\t\tparametro: %d\n", nodo_parametro->parametro);

			x++;
		}

		i++;
	}
};

void armar_lista_instrucciones(t_list* instrucciones_paquete, t_list* lista_instrucciones){

	t_list* nueva_instruccion;

	while(!list_is_empty(instrucciones_paquete)){
		nueva_instruccion = list_remove(instrucciones_paquete, 0);
		agregar_instruccion(lista_instrucciones, nueva_instruccion);
	}

}

// esta funcion se usa para armar la lista de inst en deserializar_PCB()
void deserializar_lista_instrucciones(t_list* instrucciones_paquete, t_list* lista_instrucciones){

	t_list* nueva_instruccion;

	while(!list_is_empty(instrucciones_paquete)){
		nueva_instruccion = list_remove(instrucciones_paquete, 0);
		deserializar_instruccion(lista_instrucciones, nueva_instruccion);
	}

}

// esta funcion se usa en deserializar_lista_instrucciones()
void deserializar_instruccion(t_list* lista_instrucciones, void* buffer){

	// genero un nuevo nodo de la lista de instruccion y lo completo con los datos
	nodo_instruccion* nueva_instruccion = nuevo_nodo_instruccion();
	completar_nodo_instruccion(nueva_instruccion, buffer);
	list_add(lista_instrucciones, nueva_instruccion);

}

nodo_instruccion* nuevo_nodo_instruccion(){
	nodo_instruccion* nodo = (struct nodo_instruccion*)malloc(sizeof(struct nodo_instruccion));
	nodo->instruccion.parametros = list_create();
	return nodo;
}

nodo_parametro* nuevo_nodo_parametro(){
	nodo_parametro* nodo = (struct nodo_parametro*)malloc(sizeof(struct nodo_parametro));
	return nodo;
}

void agregar_instruccion(t_list* lista_instrucciones, void* buffer){

	// utilizamos strtok para complertar el identificador y los parametros de la instruccion
	char str[30];
	strcpy(str, buffer);
	char separator[] = " ";
	char* token;
	char* rest = str;

	// guardo el codigo de instruccion en token
	token = strtok_r(str, separator, &rest);

	// preguntar que instruccion es
	// Caso que el codigo de operacion == NO_OP
	if(strcmp(token,"NO_OP") == 0){

		// leo el primer parametro (cantidad de NO_OP a ejecutar)
		token = strtok_r(NULL, separator, &rest);

		int i = atoi(token);
		while(i > 0){

			// genero un nuevo nodo de la lista de instruccion y lo completo con los datos
			nodo_instruccion* nueva_instruccion = nuevo_nodo_instruccion();
			completar_nodo_instruccion(nueva_instruccion, "NO_OP");
			list_add(lista_instrucciones, nueva_instruccion);

			i--;
		}
	}
	else
	{

		// Caso que el codigo de operacion NO es NO_OP

		// genero un nuevo nodo de la lista de instruccion y lo completo con los datos
		nodo_instruccion* nueva_instruccion = nuevo_nodo_instruccion();
		completar_nodo_instruccion(nueva_instruccion, buffer);
		list_add(lista_instrucciones, nueva_instruccion);

	}

}

void completar_nodo_instruccion(nodo_instruccion* nodo_instruccion, char* buffer_original){

	// Limpiamos los caracteres basura del final del buffer
	char* buffer = string_new();
	memset(buffer,'\0',strlen(buffer_original));
	strcpy(buffer, buffer_original);
	// TODO: string_trim_right porque revienta
	// string_trim_right(&buffer);

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

	// walk through other tokens
	while(token != NULL) {
		agregar_nuevo_parametro(nodo_instruccion, token);
		token = strtok_r(NULL, separator, &rest);
	}

};

void agregar_nuevo_parametro(nodo_instruccion* nodo_instruccion, char* parametro){

	nodo_parametro* nodo_parametro = nuevo_nodo_parametro();
	nodo_parametro->parametro = atoi(parametro);

	list_add(nodo_instruccion->instruccion.parametros, nodo_parametro);

};

// esta funcion sirve para recorrer cualquier PCB ya sea en kernel, cpu, etc.
void imprimir_PCB(pcb* nodo_pcb){
	printf("\nInformacion del PCB:");
	printf("\n\tID: %d", nodo_pcb->id);
	printf("\n\tTAMANIO: %d", nodo_pcb->tamanio);
	printf("\n\tPC: %d", nodo_pcb->program_counter);
	printf("\n\tTabla paginas: %d", nodo_pcb->tabla_paginas);
	printf("\n\tESTIMACION: %f", nodo_pcb->estimacion_rafaga);
	printf("\n\tSTATUS: %s", imprimir_status(nodo_pcb->status));
	printf("\n\tSTATUS int: %d", nodo_pcb->status);
	printf("\n\tint STATUS: %d", nodo_pcb->status);
	printf("\n\tTiempo a bloquearse: %d\n", nodo_pcb->tiempo_a_bloquearse);
	mostrar_lista_instrucciones(nodo_pcb->instrucciones);
	printf("\n");
}

char* imprimir_status(status status){
	switch(status){
		case 0:
			return "NEW";
			break;
		case 1:
			return "READY";
			break;
		case 2:
			return "EXECUTION";
			break;
		case 3:
			return "BLOCKED";
			break;
		case 4:
			return "SUSPENDED_READY";
			break;
		case 5:
			return "SUSPENDED_BLOCKED";
			break;
		case 6:
			return "EXIT";
			break;
	}
}

void destruir_PCB(pcb* pcb){
	list_destroy_and_destroy_elements(pcb->instrucciones, (void*)destruir_instrucciones);
	free(pcb);
}

void destruir_instrucciones(nodo_instruccion* ins){
	list_destroy_and_destroy_elements(ins->instruccion.parametros, (void *)destruir_parametros);
	free(ins);
}

void destruir_parametros(nodo_parametro* parametro){
	free(parametro);
}

