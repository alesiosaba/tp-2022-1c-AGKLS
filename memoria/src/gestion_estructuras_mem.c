#include "../include/gestion_estructuras_mem.h"

#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../shared/include/utils/utils.h"
#include "../include/globals.h"


void inicializar_estructuras(){
	/*
		Esta funcion genera:
		 Un espacio contiguo de memoria en donde los procesos podrán leer y/o escribir.
		 bitmap de marcos ocupados
		 las listas de paginas de primer y segundo nivel
	*/

	log_debug(logger, "-------------- Inicializacion de estructuras --------------");

	reservar_espacio_lectura_escritura();

	armar_bitmap_marcos();

	inicializar_tablas_de_entradas();

	inicializar_lista_procesos_en_memoria();

	log_debug(logger, "-------------- Finalizacion de Inicializacion de estructuras --------------");
}

// Reserva cierta cantidad de espacio en memoria para poder ser escrita o leida por los distintos procesos
void reservar_espacio_lectura_escritura(){

	// log_debug(logger, "reservar_espacio_lectura_escritura()");

	// log_debug(logger, "------- Reservar espacio de memoria contiguo");

	// log_debug(logger, "Se reservarán %d cantidad de bytes en memoria" , config_values.tam_memoria);

	// La cantidad de espacio (en bytes) que se va a reservar viene ya por archivo de configuracion
	// config_values->tam_memoria

	espacio_lectura_escritura_procesos = malloc(config_values.tam_memoria);

	// log_debug(logger, "------- Finalizacion de Reservar espacio de memoria contiguo");
}

// Armar un array de bits que representen si los marcos están ocupados o no
void armar_bitmap_marcos(){
	// log_debug(logger, "armar_bitmap_marcos()");

	// log_debug(logger, "------- Armar bitmap de marcos disponibles y ocupados");

	// En estos calculos usamos la funcion ceil() para no quedarnos con cantidades menores a las que necesitamos

	double cantidadMarcos_f = config_values.tam_memoria / config_values.tam_pagina;

	int cantidadMarcos = ceil(cantidadMarcos_f);

	// log_debug(logger, "La memoria tendrá %d marcos en total" , cantidadMarcos);

	double bytesNecesarios_f = cantidadMarcos / 8;

	int bytesNecesarios = ceil(bytesNecesarios_f);
	// log_debug(logger, "Se necesitarán %d (%d bits) para representar a todos los marcos de la memoria" , bytesNecesarios , cantidadMarcos);

	// bitmap - el bloque de memoria que contiene los bits a leer/escribir
	char* bitmap_real = malloc(bytesNecesarios);

	// bitarray_create retorna un struct t_bitarray*
	bitmap_marcos = bitarray_create(bitmap_real, bytesNecesarios);

	limpiar_bitmap(cantidadMarcos);
	log_debug(logger, "armar_bitmap_marcos: Finalizacion de Armado bitmap de marcos");
    dump_bitmap(bitmap_marcos);
}

void inicializar_tablas_de_entradas(){
	// log_debug(logger, "inicializar_tablas_de_entradas()");

	// log_debug(logger, "------- Inicializar listas de entradas a ambas tablas de niveles");

	tablas_primer_nivel = list_create();
	tablas_segundo_nivel = list_create();

	// log_debug(logger, "------- Finalizacion de inicializacion de listas de entradas a ambas tablas de niveles");
}

entrada_tabla_N1* agregar_entrada_tabla_primer_nivel(tabla_primer_nivel *tabla){
	  entrada_tabla_N1* e = malloc(sizeof(entrada_tabla_N1));
	  e->numero_entrada = list_add(tabla, e);
	  e->indice_tabla_n2 = -1;
	  return e;
}

entrada_tabla_N2* agregar_entrada_tabla_segundo_nivel(tabla_segundo_nivel *tabla){
    entrada_tabla_N2* e = malloc(sizeof(entrada_tabla_N2));
    e->num_pag = -1;
    e->dir = -1;
    e->bit_modificacion = 0;
    e->bit_presencia = 0;
    e->bit_uso = 0;
    list_add(tabla, e);
    return e;
}


void inicializar_lista_procesos_en_memoria() {

	// log_debug(logger, "inicializar_lista_procesos_en_memoria()");

	log_debug(logger, "------- Inicializar lista de procesos en memoria");

	procesos_en_memoria = list_create();

	log_debug(logger, "------- Finalizacion de inicializacion de lista de procesos en memoria");
}



void limpiar_bitmap(int cantidadMarcos){

	log_debug(logger, "------- Limpiando bitmap de marcos");

	for(int i = 0; i < cantidadMarcos; i++)
	{
		// Usaremos el bit 0 como desocupado
		bitarray_clean_bit(bitmap_marcos, i);
	}
	log_debug(logger, "------- Bitmap limpio");
}


void liberar_estructuras(){

	log_debug(logger, "liberar_estructuras()");

	log_debug(logger, "------- Eliminando estructuras de memoria");

	// Liberar memoria ocupada por espacio en memoria para poder ser escrita o leida por los distintos procesos
	free(espacio_lectura_escritura_procesos);

	// Liberar memoria ocupada por bitmap de marcos
	free(bitmap_marcos->bitarray);
	free(bitmap_marcos);

	// Liberar memoria ocupada por las entradas de ambas tablas de paginas
	list_destroy(tablas_primer_nivel);
	list_destroy(tablas_segundo_nivel);

	// Liberar memoria ocupada por la lista de procesos en memoria
	list_destroy(procesos_en_memoria);

	log_debug(logger, "------- Finalizacion de eliminacion de estructuras de memoria");
	log_destroy(logger);
}

void asignar_nuevas_paginas(pcb* pcb){

	// Generamos el elemento del proceso en la lista de procesos en memoria
	proceso_en_memoria* proceso_nuevo = malloc(sizeof(proceso_en_memoria));

	proceso_nuevo->id_proceso = pcb->id;

	// Apunta a las entradas de primer nivel en las que se encuentran los marcos asignados al proceso
	proceso_nuevo->entradas_N1 = list_create();

	proceso_nuevo->entradas_N1 = reservar_paginas(pcb->id, pcb->tamanio);


}

t_list* reservar_paginas(int idProceso, int tamanioProceso){

	// Total de marcos que ocupara el proceso en memoria
	int paginasNecesarias = cantidad_paginas_necesarias(tamanioProceso);

	int entradasNecesariasN1 = ceil(paginasNecesarias / config_values.entradas_por_tabla);

	log_debug(logger, "Reservando paginas para nuevo proceso ID: %d",idProceso);

	log_debug(logger, "Tamaño proceso: %d", tamanioProceso);
	log_debug(logger, "Tamaño pagina: %d", config_values.tam_pagina);
	log_debug(logger, "Cantidad necesaria de marcos (tamaño proceso / tamaño pagina) : %d", paginasNecesarias);
	log_debug(logger, "Cantidad necesaria de entradas de N1 (cant necesaria entradas N1 / cantidad entradas por configuracion) : %d", entradasNecesariasN1);

	// Lista para ir guardando las entradas de primer nivel que vayamos generando
	t_list* entradas_N1_aux = list_create();

	// cantidad de paginas en N2 reservadas
	int paginasReservadas;

	for(paginasReservadas = 0 ; paginasReservadas < paginasNecesarias ; paginasReservadas++ ){


		if(se_necesita_generar_otra_entrada_N1()){
			log_debug(logger, "entre al IF");
		}

	}


	return entradas_N1_aux;

}

int se_necesita_generar_otra_entrada_N1(int paginasReservadas){
	// TODO: revisar esto
	// Si la cantidad de paginas reservadas ocupó "toda" una entrada?
	return (paginasReservadas % config_values.entradas_por_tabla) == 0;
}

int cantidad_paginas_necesarias(int tamanioProceso){
    double cantidad = (double) tamanioProceso / (double) config_values.tam_pagina;
    return ceil(cantidad);
}

proceso_en_memoria* buscar_proceso_por_id(int id)
{

    bool id_equals(proceso_en_memoria *p){
        return p->id_proceso == id;
    }

    proceso_en_memoria *aux = (proceso_en_memoria*)list_find(procesos_en_memoria, (void*)id_equals);
    return aux;
}

void imprimir_marcos_de_proceso(){
	for(int i = 0; i < list_size(procesos_en_memoria); i++){
		proceso_en_memoria *p = (proceso_en_memoria *)list_get(procesos_en_memoria, i);
	    t_list *marcos = conseguir_numeros_marcos_proceso(p->id_proceso);
	    char * pids = string_new();
	    for(int j = 0 ; j < list_size(marcos); j++){
	        int num_marco = (int)list_get(marcos, j);
	    	string_append(&pids, string_itoa(num_marco));
	    	string_append(&pids, "  ");
//	        log_info(logger,"Marco %d -> %d", j+1, num_marco);
	    }
	 log_info(logger,"Marcos PID: %d -> %s", p->id_proceso, pids);
	 free(pids);
	}

}

// Recibe una tabla N2 y un numero de marco y devuelve la entrada N2 que tiene ese marco o NULL si no la tiene
entrada_tabla_N2* obtener_entrada_n2_por_num_marco(tabla_segundo_nivel *t, int num_marco)
{
    entrada_tabla_N2 *ret;
    for(int i = 0; i < list_size(t); i++)
    {
        ret = list_get(t, i);
        if(ret->bit_presencia == 1 && ret->dir == num_marco * config_values.tam_pagina)
        {
            return ret;
        }
    }
    return NULL;
}

// ▁ ▂ ▄ ▅ ▆ ▇ █ ŴÃŘŇĮŇĞ █ ▇ ▆ ▅ ▄ ▂ ▁
entrada_tabla_N2* conseguir_entrada_pagina(int dir_tabla_n1, int pag)
{
    //conseguir tabla nivel 1
	tabla_primer_nivel *t = list_get(tablas_primer_nivel, dir_tabla_n1);

	// TODO: Revisarlo
    //numero entrada = division numero pagina por paginas tablas redondeado para arriba
    int num_entrada_n1 = ceil((double) pag / (double) config_values.entradas_por_tabla);
    entrada_tabla_N1 *e1 = list_get(t, num_entrada_n1);

    //conseguir tabla nivel 2
    tabla_segundo_nivel *t2 = list_get(tablas_segundo_nivel, e1->indice_tabla_n2);

    //desplazamiento en tabla = resto de division anterior
    int num_entrada_n2 = pag % config_values.entradas_por_tabla;
    entrada_tabla_N2 *e2 = list_get(t2, num_entrada_n2);
    return e2;
}

// Busca en TODAS las tablas N2 aquella entrada que tenga el num_marco y la devuelve
entrada_tabla_N2* buscar_entrada_n2_por_num_marco(int num_marco)
{
	log_debug(logger, "buscar_entrada_n2_por_num_marco: %d", num_marco);
    t_list_iterator *iterador = list_iterator_create(tablas_segundo_nivel);
    entrada_tabla_N2 *ret = NULL;
    tabla_segundo_nivel *t;
    // Recorremos todas las Tablas N2 en busqueda de aquella que tenga la entrada con el num_marco
    while(list_iterator_has_next(iterador))
    {
        t = list_iterator_next(iterador);
        ret = obtener_entrada_n2_por_num_marco(t, num_marco);
        if(ret != NULL){
            list_iterator_destroy(iterador);
            return ret;
        }
    }
    list_iterator_destroy(iterador);
    return ret;
}

t_list* conseguir_marcos_proceso(int dir_tabla_n1)
{
	tabla_primer_nivel *t = list_get(tablas_primer_nivel, dir_tabla_n1);
    t_list *marcos = list_create();
    t_list_iterator *iterador = list_iterator_create(t);
    while(list_iterator_has_next(iterador))
    {
        //ENTRADA TIENE DIR DE TABLA NIVEL 2
        entrada_tabla_N1 *e1 = list_iterator_next(iterador);
        //CONSIGUE TABLA
        t_list_iterator *iterador2 = list_iterator_create(list_get(tablas_segundo_nivel, e1->indice_tabla_n2));
        while(list_iterator_has_next(iterador2))
        {
            //CONSIGUE ENTRADA DE TABLA N2
            entrada_tabla_N2 *e2 = list_iterator_next(iterador2);
            if(e2->bit_presencia == 1)
            {
                list_add(marcos, e2);
            }
        }
        list_iterator_destroy(iterador2);
    }
    list_iterator_destroy(iterador);
    return marcos;
}

// Devuelve lista con los numeros de marco que tiene asignado el proceso
t_list* conseguir_numeros_marcos_proceso(int id)
{
    proceso_en_memoria *ret = buscar_proceso_por_id(id);
    return ret->marcos_reservados;
}

tabla_primer_nivel* crear_tablaN1(int tamanio_proceso)
{
    tabla_primer_nivel *t = list_create();
    // Cantidad TOTAL de paginas que necesitaria el proceso
    int paginas_necesarias = cantidad_paginas_necesarias(tamanio_proceso);
    log_info(logger, "crear_tabla_n1: Paginas necesarias %d segun tamanio %d", paginas_necesarias, tamanio_proceso);

    // Iteramos hasta que tenga reservadas todas las paginas que precisa
    for(int paginas_reservadas = 0; paginas_reservadas < paginas_necesarias; paginas_reservadas++)
    {
    	// Como T1 y T2 tienen misma cantidad de entradas, mediante el resto
    	// nos fijamos si hace falta crear una nueva entrada N1
        if(paginas_reservadas % config_values.entradas_por_tabla == 0)
        {
            // creo entrada N1
            entrada_tabla_N1 *e = agregar_entrada_tabla_primer_nivel(t);
            //creo tabla N2 que sera apuntada por esa tabla
            tabla_segundo_nivel *t2 = list_create();
            pthread_mutex_lock(&mutex_tablasN2);
            // Le asigno a la entrada N1 el numero de tabla N2 a la que apunta
            e->indice_tabla_n2 = list_add(tablas_segundo_nivel, t2);
            pthread_mutex_unlock(&mutex_tablasN2);
        	log_debug(logger, "Se creo entrada N1");
        	log_debug(logger, "se creo tabla N2 ");
        	log_debug(logger, "La entrada N1 numero apunta a la tabla N2 numero: %d", e->indice_tabla_n2);

        }

        // Asigno a la tabla N2 de la ultima entrada N1, una nueva entrada N2.

        //conseguir ultima entrada (ultima tabla 2)
        entrada_tabla_N1 *ultima_entrada_n1 = list_get(t, list_size(t) -1);
        //buscar tabla en la dir que dice la entrada de tabla 1
        tabla_segundo_nivel *tabla_n2_ultima_entrada_n1 = list_get(tablas_segundo_nivel, ultima_entrada_n1->indice_tabla_n2);
        entrada_tabla_N2 *aux3 = agregar_entrada_tabla_segundo_nivel(tabla_n2_ultima_entrada_n1);

        //DIR = NUMERO PAGINA * TAMANIO PAGINA
        aux3->num_pag = paginas_reservadas;
        aux3->dir = aux3->num_pag * config_values.tam_pagina;
        aux3->bit_presencia = 0;
    	log_debug(logger, "Se agrego entrada de segundo nivel numero: %d dir: %d", aux3->num_pag, aux3->dir);
    }
    log_info(logger, "crear_tabla_n1: Se creo Tabla N1 exitosamente");
    return t;
}

void reservar_marcos_proceso(proceso_en_memoria *p)
{
	log_debug(logger, "reservar_marcos_proceso: reservando para proceso PID: %d", p->id_proceso);

	int cant_marcos_reservados_actualmente = list_size(p->marcos_reservados);
	log_debug(logger, "reservar_marcos_proceso: El proceso PID: %d tiene actualmente: %d marcos reservados", p->id_proceso, cant_marcos_reservados_actualmente);

	if(cant_marcos_reservados_actualmente == config_values.marcos_por_proceso){
		log_debug(logger, "reservar_marcos_proceso: Se intento reservar un marco para PID: %d pero excedio marcos_por_proceso", p->id_proceso, cant_marcos_reservados_actualmente);
		return;
	}

    int cantidad_marcos_reservados = 0;
    for(int i = 0; cantidad_marcos_reservados < config_values.marcos_por_proceso; i++)
    {
        if(!bitarray_test_bit(bitmap_marcos, i))
        {
            int marco = i;

            list_add(p->marcos_reservados, (void*) marco);
            // A priori estoy poniendo el bit en 0 para reservarlo. Revisar si tengo que setear.
            bitarray_set_bit(bitmap_marcos, i);
            cantidad_marcos_reservados++;
        }
    }
	log_debug(logger, "reservar_marcos_proceso: Proceso PID: %d reservo sus marcos", p->id_proceso);
}

proceso_en_memoria* asignar_proceso(int id, int tamanio_proceso){
	log_debug(logger, "asignar_proceso: asignando proceso PID: %d en memoria", id);
	proceso_en_memoria *nuevoProceso = malloc(sizeof(proceso_en_memoria));
	nuevoProceso->id_proceso = id;
	nuevoProceso->posicion_puntero_clock = 0;
	nuevoProceso->tablaN1 = crear_tablaN1(tamanio_proceso);
	nuevoProceso->dir_tabla_n1 = -1;
	nuevoProceso->marcos_reservados = list_create();
	sem_init(&(nuevoProceso->suspension_completa), 0, 0);
	nuevoProceso->esta_suspendido = 0;
	log_debug(logger, "reservar_marcos_proceso: proceso PID %d asignado correctamente", id);
	return nuevoProceso;
}

void dump_bitmap(t_bitarray *bitmap)
{
    size_t cantidadDeBits =  bitarray_get_max_bit(bitmap);

    char* cadena = malloc(cantidadDeBits);

    strcpy(cadena, "\0");

    for(int aux=0;aux<cantidadDeBits;aux++){
        if(bitarray_test_bit(bitmap, aux)==1){
            string_append(&cadena, "1");
        }
        else
            string_append(&cadena, "0");
    }

    log_info(logger,"dump_bitmap - Estado actual: %s", cadena);
    free(cadena);
}

void liberar_marcos_bitmap(t_list *marcos)
{
	//Iteramos el bitarray con los marcos
    t_list_iterator *iterador = list_iterator_create(marcos);
    while(list_iterator_has_next(iterador))
    {
    	// Ponemos en cero los marcos a liberar
        int marco = (int) list_iterator_next(iterador);
        bitarray_clean_bit(bitmap_marcos, marco);
    }
    list_iterator_destroy(iterador);
}

void eliminar_paginas_proceso(int id, int dir_tabla_n1)
{
    log_debug(logger,"eliminar_paginas_proceso - Eliminando paginas del proceso %d con dir_tabla-n1", id, dir_tabla_n1);
    tabla_primer_nivel *t = list_get(tablas_primer_nivel, dir_tabla_n1);
    t_list_iterator *iterador = list_iterator_create(t);
    while(list_iterator_has_next(iterador))
    {
        entrada_tabla_N1 *e1 = list_iterator_next(iterador);
        tabla_segundo_nivel *t2 = list_get(tablas_segundo_nivel, e1->indice_tabla_n2);

        list_clean_and_destroy_elements(t2, free);

    }
    list_clean_and_destroy_elements(t, free);
    list_iterator_destroy(iterador);
    log_debug(logger,"eliminar_paginas_proceso - Liberando marcos de proceso %d", id);
    liberar_marcos_de_proceso(id);
    log_debug(logger,"eliminar_paginas_proceso  paginas del proceso %d liberadas", id);
}

void eliminar_estructura_proceso(int pid)
{
    bool id_equals(proceso_en_memoria *p){
        return p->id_proceso == pid;
    }
    log_info(logger,"eliminar_estructura_proceso - Eliminando proceso en memoria: %d", pid);
    pthread_mutex_lock(&mutex_procesos_en_memoria);
    list_remove_and_destroy_by_condition(procesos_en_memoria, (void*)id_equals, (void*)eliminar_proceso);
    pthread_mutex_unlock(&mutex_procesos_en_memoria);
    log_info(logger,"eliminar_estructura_proceso - Proceso en memoria PID: %d eliminado", pid);

}


void eliminar_proceso(proceso_en_memoria *p)
{
    sem_destroy(&(p->suspension_completa));
    free(p);
}
