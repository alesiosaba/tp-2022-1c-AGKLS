#include "../include/mmu.h"

int traducir_dir_logica(pcb** pcb, int direccion_logica){

	log_debug(logger,"Inicia traduccion de direccion logica");

	int numero_pagina = obtener_numero_pagina(direccion_logica);
	int desplazamiento = obtener_desplazamiento(direccion_logica, numero_pagina);

	// Buscamos si la pagina está en la TLB
	int marco_en_TLB = busqueda_pagina_en_TLB(numero_pagina);

	// imprimir_TLB();

	// log_debug(logger, "marco_en_TLB: %d", marco_en_TLB);

	if(marco_en_TLB != -1){
		// log_debug(logger,"TLB HIT");
		// log_debug(logger,"La pagina buscada esta en el marco %d", marco_en_TLB);

		sleep(1);
		time_t nuevo_instante_referencia = time(NULL);

		int i = 0;

		struct entrada_TLB* elem = list_get(TLB,i);

		while(elem->marco != marco_en_TLB){
			elem = list_get(TLB,i);
			i++;
		}

		elem->insUltRef = nuevo_instante_referencia;

		// imprimir_TLB();

		log_debug(logger,"Finaliza traduccion de direccion logica");

		// Devuelvo la direccion fisica compuesta de marco + desplazamiento
		// return marco_en_TLB + desplazamiento?;
		//TODO:
		return 77;

	}
	// log_debug(logger,"TLB MISS");

	int id_tablaN1 = (*pcb)->tabla_paginas;
	int entrada_tabla_primer_nivel = obtener_entrada_tabla_primer_nivel(numero_pagina);


	// TODO: hacer que las solicitudes a memoria funcionen contra Memoria

	// SOLICITUD ENTRADA DE TABLA N1
	// ENVIO DIR TABLA N1 y NUM ENTRADA TABLA NIVEL 1
	// send_solicitud_tabla_N1(conexionAMemoria, id_tablaN1, entrada_tabla_primer_nivel);

	// RECIBO N° DE TABLA N2
	// int id_tablaN2 = recv_respuesta_solicitud_N1(conexionAMemoria);
	// int entrada_tabla_segundo_nivel = obtener_entrada_tabla_segundo_nivel(numero_pagina);
	int id_tablaN2 = 8;
	int entrada_tabla_segundo_nivel = 6;


	// SOLICITUD ENTRADA DE TABLA N2
	// ENVIO DIR TABLA N2 y NUM ENTRADA TABLA NIVEL 2
	// send_solicitud_tabla_N2(conexionAMemoria, id_tablaN2, entrada_tabla_segundo_nivel);

	// RECIBO N° DE FRAME
	// int frame = recv_respuesta_solicitud_N2(conexionAMemoria);

	int frame = 0;
	frame = (numero_pagina) * 10;

	// log_debug(logger,"Frame: %d", frame);

	// Si no tuve TLB Hit SI o SI se agrega pagina con su marco a la TLB

	struct entrada_TLB* entrada_nueva = malloc(sizeof(struct entrada_TLB));

	entrada_nueva->numero_pagina = numero_pagina;
	entrada_nueva->marco = frame;

	// TODO: Borrar sleep, porque el retardo se da en Memoria
	// Esto es para que no asigne mal los instantes
	sleep(2);

	entrada_nueva->insUltRef = time(NULL);

	// log_debug(logger, "Algoritmo: %s", config_values.reemplazo_TLB);
	// log_debug(logger, "Voy a reemplazar una entrada TLB con la pagina %d - marco: %d - instanteRef: %f", entrada_nueva->numero_pagina , entrada_nueva->marco , (float) entrada_nueva->insUltRef);

	agregar_entrada_TLB(entrada_nueva);

	// imprimir_TLB();

	log_debug(logger,"Finaliza traduccion de direccion logica");

	// TODO: como devolver la direccion fisica
	return 666;

}

int obtener_numero_pagina(int direccion_logica){
	return floor(direccion_logica / tamanio_pagina);
}

int obtener_entrada_tabla_primer_nivel(int numero_pagina) {
	return floor(numero_pagina / cant_entradas_por_tabla);
}

int obtener_entrada_tabla_segundo_nivel(int numero_pagina) {
	return numero_pagina % cant_entradas_por_tabla;
}

int obtener_desplazamiento(int direccion_logica, int numero_pagina) {
	return direccion_logica - numero_pagina * tamanio_pagina;
}
