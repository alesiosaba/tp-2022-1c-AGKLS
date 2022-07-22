#include "../include/gestion_estructuras_mem.h"
#include <math.h>

void inicializar_estructuras(){
	/*
		Esta funcion genera:
		 Un espacio contiguo de memoria en donde los procesos podrán leer y/o escribir.
		 bitmap de marcos ocupados
		 las listas de paginas de primer y segundo nivel
	*/

	log_info(logger, "-------------- Inicializacion de estructuras --------------");

	reservar_espacio_lectura_escritura();

	armar_bitmap_marcos();


	log_info(logger, "-------------- Finalizacion de Inicializacion de estructuras --------------");
}

// Reserva cierta cantidad de espacio en memoria para poder ser escrita o leida por los distintos procesos
void reservar_espacio_lectura_escritura(){

	log_debug(logger, "reservar_espacio_lectura_escritura()");

	log_info(logger, "------- Reservar espacio de memoria contiguo");

	log_info(logger, "Se reservarán %d cantidad de bytes en memoria" , config_values.tam_memoria);

	// La cantidad de espacio (en bytes) que se va a reservar viene ya por archivo de configuracion
	// config_values->tam_memoria

	espacio_lectura_escritura_procesos = malloc(config_values.tam_memoria);

	log_info(logger, "------- Finalizacion de Reservar espacio de memoria contiguo");
}

// Armar un array de bits que representen si los marcos están ocupados o no
void armar_bitmap_marcos(){
	log_debug(logger, "armar_bitmap_marcos()");

	log_info(logger, "------- Armar bitmap de marcos disponibles y ocupados");

	// En estos calculos usamos la funcion ceil() para no quedarnos con cantidades menores a las que necesitamos

	double cantidadMarcos_f = config_values.tam_memoria / config_values.tam_pagina;

	int cantidadMarcos = ceil(cantidadMarcos_f);

	log_info(logger, "La memoria tendrá %d marcos en total" , cantidadMarcos);

	double bytesNecesarios_f = cantidadMarcos / 8;

	int bytesNecesarios = ceil(bytesNecesarios_f);
	log_info(logger, "Se necesitarán %d (%d bits) para representar a todos los marcos de la memoria" , bytesNecesarios , cantidadMarcos);

	// Reservamos en memoria el espacio donde se almacenarán los bits representando los marcos
	char* bitsMarcos = malloc(bytesNecesarios);

	// bitmap - el bloque de memoria que contiene los bits a leer/escribir
	char* bitmap_real = malloc(bytesNecesarios);

	// bitarray_create retorna un struct t_bitarray*
	bitmap_marcos = bitarray_create(bitmap_real, bitsMarcos);

	limpiar_bitmap(cantidadMarcos);

	log_info(logger, "------- Finalizacion de Armado bitmap de marcos disponibles y ocupados");

}

void inicializar_tablas_de_entradas(){
	log_debug(logger, "inicializar_tablas_de_entradas()");

	log_info(logger, "------- Inicializar listas de entradas a ambas tablas de niveles");

	entradas_tabla_primer_nivel = list_create();
	entradas_tabla_segundo_nivel = list_create();

	log_info(logger, "------- Finalizacion de inicializacion de listas de entradas a ambas tablas de niveles");
}


void limpiar_bitmap(int cantidadMarcos){

	log_info(logger, "------- Limpiando bitmap de marcos");

	for(int i = 0; i < cantidadMarcos; i++)
	{
		// Usaremos el bit 0 como desocupado
		bitarray_clean_bit(bitmap_marcos, i);
	}
	log_info(logger, "------- Bitmap limpio");
}


void liberar_estructuras(){

	log_debug(logger, "liberar_estructuras()");

	log_info(logger, "------- Eliminando estructuras de memoria");

	// Liberar memoria ocupada por espacio en memoria para poder ser escrita o leida por los distintos procesos
	free(espacio_lectura_escritura_procesos);

	// Liberar memoria ocupada por bitmap de marcos
	free(bitmap_marcos->bitarray);
	free(bitmap_marcos);

	// Liberar memoria ocupada por las entradas de ambas tablas de paginas
	list_destroy(entradas_tabla_primer_nivel);
	list_destroy(entradas_tabla_segundo_nivel);

	log_info(logger, "------- Finalizacion de eliminacion de estructuras de memoria");

}
