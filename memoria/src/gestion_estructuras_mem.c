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

	log_debug(logger, "reservar_espacio_lectura_escritura()");

	log_debug(logger, "------- Reservar espacio de memoria contiguo");

	log_debug(logger, "Se reservarán %d cantidad de bytes en memoria" , config_values.tam_memoria);

	// La cantidad de espacio (en bytes) que se va a reservar viene ya por archivo de configuracion
	// config_values->tam_memoria

	espacio_lectura_escritura_procesos = malloc(config_values.tam_memoria);

	log_debug(logger, "------- Finalizacion de Reservar espacio de memoria contiguo");
}

// Armar un array de bits que representen si los marcos están ocupados o no
void armar_bitmap_marcos(){
	log_debug(logger, "armar_bitmap_marcos()");

	log_debug(logger, "------- Armar bitmap de marcos disponibles y ocupados");

	// En estos calculos usamos la funcion ceil() para no quedarnos con cantidades menores a las que necesitamos

	double cantidadMarcos_f = config_values.tam_memoria / config_values.tam_pagina;

	int cantidadMarcos = ceil(cantidadMarcos_f);

	log_debug(logger, "La memoria tendrá %d marcos en total" , cantidadMarcos);

	double bytesNecesarios_f = cantidadMarcos / 8;

	int bytesNecesarios = ceil(bytesNecesarios_f);
	log_debug(logger, "Se necesitarán %d (%d bits) para representar a todos los marcos de la memoria" , bytesNecesarios , cantidadMarcos);

	// bitmap - el bloque de memoria que contiene los bits a leer/escribir
	char* bitmap_real = malloc(bytesNecesarios);

	// bitarray_create retorna un struct t_bitarray*
	bitmap_marcos = bitarray_create(bitmap_real, bytesNecesarios);

	limpiar_bitmap(cantidadMarcos);

	log_debug(logger, "------- Finalizacion de Armado bitmap de marcos disponibles y ocupados");

}

void inicializar_tablas_de_entradas(){
	log_debug(logger, "inicializar_tablas_de_entradas()");

	log_debug(logger, "------- Inicializar listas de entradas a ambas tablas de niveles");

	tablas_primer_nivel = list_create();
	tablas_segundo_nivel = list_create();

	log_debug(logger, "------- Finalizacion de inicializacion de listas de entradas a ambas tablas de niveles");
}

entrada_tabla_N1* agregar_entrada_tabla_primer_nivel(t_tablaN1 *tabla){
	  entrada_tabla_N1* e = malloc(sizeof(entrada_tabla_N1));
	  e->num_tabla = list_add(tabla, e);
	  e->dir = -1;
	  return e;
}

entrada_tabla_N2* agregar_entrada_tabla_segundo_nivel(t_tablaN2 *tabla){
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

	log_debug(logger, "inicializar_lista_procesos_en_memoria()");

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

entrada_tabla_N2* tabla_contiene_marco(t_tablaN2 *t, int num_marco)
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


entrada_tabla_N2* conseguir_entrada_pagina(int dir_tablaN1, int pag)
{
    //conseguir tabla nivel 1
	t_tablaN1 *t = list_get(tablas_primer_nivel, dir_tablaN1);

    //numero entrada = division numero pagina por paginas tablas redondeado para arriba
    int num_entrada_n1 = ceil(pag / config_values.entradas_por_tabla);
    entrada_tabla_N1 *e1 = list_get(t, num_entrada_n1);

    //conseguir tabla nivel 2
    t_tablaN2 *t2 = list_get(tablas_segundo_nivel, e1->dir);

    //desplazamiento en tabla = resto de division anterior
    int num_entrada_n2 = pag % config_values.entradas_por_tabla;
    entrada_tabla_N2 *e2 = list_get(t2, num_entrada_n2);
    return e2;
}

entrada_tabla_N2* conseguir_pagina_en_marco(int num_marco)
{
    t_list_iterator *iterador = list_iterator_create(tablas_segundo_nivel);
    entrada_tabla_N2 *ret = NULL;
    t_tablaN2 *t;
    while(list_iterator_has_next(iterador))
    {

        t = list_iterator_next(iterador);

        ret = tabla_contiene_marco(t, num_marco);
        if(ret != NULL)
        {
            list_iterator_destroy(iterador);
            return ret;
        }
    }
    list_iterator_destroy(iterador);
    return ret;
}

t_list* conseguir_marcos_proceso(int dir_tablaN1)
{
	t_tablaN1 *t = list_get(tablas_primer_nivel, dir_tablaN1);
    t_list *marcos = list_create();
    t_list_iterator *iterador = list_iterator_create(t);
    while(list_iterator_has_next(iterador))
    {
        //ENTRADA TIENE DIR DE TABLA NIVEL 2
        entrada_tabla_N1 *e1 = list_iterator_next(iterador);
        //CONSIGUE TABLA
        t_list_iterator *iterador2 = list_iterator_create(list_get(tablas_segundo_nivel, e1->dir));
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

t_list* conseguir_numeros_marcos_proceso(int id)
{
    proceso_en_memoria *ret = buscar_proceso_por_id(id);
    return ret->marcos_reservados;
}

t_tablaN1* crear_tablaN1(int tamanio_proceso)
{
    t_tablaN1 *t = list_create();
    int paginas_necesarias = cantidad_paginas_necesarias(tamanio_proceso);
    log_info(logger, "Paginas necesarias %d segun tamanio %d", paginas_necesarias, tamanio_proceso);

    for(int paginas_reservadas = 0; paginas_reservadas < paginas_necesarias; paginas_reservadas++)
    {
        if(paginas_reservadas % config_values.entradas_por_tabla == 0)
        {
            //NECESITO TABLA NUEVA
            //creo entrada
            entrada_tabla_N1 *e = agregar_entrada_tabla_primer_nivel(t);
            //creo tabla nivel 2
            t_tablaN2 *t2 = list_create();
            //dir = index en lista general
            pthread_mutex_lock(&mutex_tablasN2);
            e->dir = list_add(tablas_segundo_nivel, t2);
            pthread_mutex_unlock(&mutex_tablasN2);
        }
        //conseguir ultima entrada (ultima tabla 2)
        entrada_tabla_N1 *aux = list_get(t, list_size(t) -1);
        //buscar tabla en la dir que dice la entrada de tabla 1
        t_tablaN2 *aux2 = list_get(tablas_segundo_nivel, aux->dir);
        entrada_tabla_N2 *aux3 = agregar_entrada_tabla_segundo_nivel(aux2);

        //DIR = NUMERO PAGINA * TAMANIO PAGINA
        aux3->num_pag = paginas_reservadas;
        aux3->dir = aux3->num_pag * config_values.tam_pagina;
        aux3->bit_presencia = 0;
    }
    return t;
}

void reservar_marcos_proceso(proceso_en_memoria *p)
{
// TODO: Revisar criterio en funcion a que el 1 es ocupado y 0 es sin ocupar
    int cantidad_marcos_reservados = 0;
    for(int i = 0; cantidad_marcos_reservados < config_values.marcos_por_proceso; i++)
    {
        if(bitarray_test_bit(bitmap_marcos, i))
        {
            int marco = i;

            list_add(p->marcos_reservados, (void*) marco);
            // A priori estoy poniendo el bit en 0 para reservarlo. Revisar si tengo que setear.
            bitarray_clean_bit(bitmap_marcos, i);
            cantidad_marcos_reservados++;
        }
    }
}

proceso_en_memoria* asignar_proceso(int id, int tamanio_proceso){

	proceso_en_memoria *ret = malloc(sizeof(proceso_en_memoria));
	ret->id_proceso = id;
	ret->posicion_puntero_clock = 0;
	ret->tablaN1 = crear_tablaN1(tamanio_proceso);
	ret->marcos_reservados = list_create();
	sem_init(&(ret->suspension_completa), 0, 0);
	ret->esta_suspendido = 0;
	return ret;
}

void dump_bitmap(t_bitarray *bitmap)
{
    size_t cantidadDeBits =  bitarray_get_max_bit(bitmap);

    char* cadenaDeBitmap = malloc(cantidadDeBits);

    strcpy(cadenaDeBitmap, "\0");

    for(int aux=0;aux<cantidadDeBits;aux++){
        if(bitarray_test_bit(bitmap, aux)==1){
            string_append(&cadenaDeBitmap, "1");
        }
        else
            string_append(&cadenaDeBitmap, "0");
    }

    log_info(logger,"el estado del bitmap es: %s",cadenaDeBitmap);
    free(cadenaDeBitmap);
}
