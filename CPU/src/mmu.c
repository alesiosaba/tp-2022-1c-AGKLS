#include "../include/mmu.h"

struct direccion_fisica traducir_dir_logica(pcb** pcb, int direccion_logica){

	struct direccion_fisica direccion_fisica_buscada;

	int numero_pagina = obtener_numero_pagina(direccion_logica);
	int desplazamiento = obtener_desplazamiento(direccion_logica, numero_pagina);

	// Buscamos si la pagina está en la TLB
	int marco_en_TLB = busqueda_pagina_en_TLB(numero_pagina);

	if(marco_en_TLB != -1){
		sleep(1);
		time_t nuevo_instante_referencia = time(NULL);

		int i = 0;

		struct entrada_TLB* elem = list_get(TLB,i);

		while(elem->marco != marco_en_TLB){
			elem = list_get(TLB,i);
			i++;
		}

		elem->insUltRef = nuevo_instante_referencia;

		log_debug(logger,"Finaliza traduccion de direccion logica");

		// Devuelvo la direccion fisica compuesta de marco + desplazamiento
		// return marco_en_TLB + desplazamiento?;
		//TODO:

		direccion_fisica_buscada.marco = marco_en_TLB;
		direccion_fisica_buscada.desplazamiento = desplazamiento;

		return direccion_fisica_buscada;

	}
	//log_debug(logger,"TLB MISS");

	int id_tablaN1 = (*pcb)->tabla_paginas;
	int entrada_tabla_primer_nivel = obtener_entrada_tabla_primer_nivel(numero_pagina);

	// SOLICITUD ENTRADA DE TABLA N1
	// ENVIO DIR TABLA N1 y NUM ENTRADA TABLA NIVEL 1
	send_solicitud_tabla_N1(conexionAMemoria, id_tablaN1, entrada_tabla_primer_nivel, (*pcb)->id);

	// RECIBO N° DE TABLA N2
	int id_tablaN2 = recv_respuesta_solicitud_N1(conexionAMemoria);

	log_debug(logger, "id_tabla2 recibido: %d", id_tablaN2);

	int entrada_tabla_segundo_nivel = obtener_entrada_tabla_segundo_nivel(numero_pagina);

	// SOLICITUD ENTRADA DE TABLA N2
	// ENVIO DIR TABLA N2 y NUM ENTRADA TABLA NIVEL 2
	// enviar_mensaje("send_solicitud_tabla_N2", conexionAMemoria);

	send_solicitud_tabla_N2(conexionAMemoria, id_tablaN2, entrada_tabla_segundo_nivel, (*pcb)->id);

	// RECIBO N° DE FRAME
	int frame = recv_respuesta_solicitud_N2(conexionAMemoria);

	log_debug(logger, "frame recibido: %d", frame);

	// log_debug(logger,"Frame: %d", frame);

	// Si no tuve TLB Hit SI o SI se agrega pagina con su marco a la TLB

	struct entrada_TLB* entrada_nueva = malloc(sizeof(struct entrada_TLB));

	entrada_nueva->numero_pagina = numero_pagina;
	entrada_nueva->marco = frame;

	// TODO: Borrar sleep, porque el retardo se da en Memoria
	// Esto es para que no asigne mal los instantes
	sleep(2);

	entrada_nueva->insUltRef = time(NULL);

	agregar_entrada_TLB(entrada_nueva);

	// La direccion fisica contiene el marco y desplazamiento para recorrer el void* en Memoria
	direccion_fisica_buscada.marco = frame;
	direccion_fisica_buscada.desplazamiento = desplazamiento;

	return direccion_fisica_buscada;
}

int obtener_numero_pagina(int direccion_logica){
	return 3; //floor(direccion_logica / tamanio_pagina);
}

int obtener_entrada_tabla_primer_nivel(int numero_pagina) {
	return 4; //floor(numero_pagina / cant_entradas_por_tabla);
}

int obtener_entrada_tabla_segundo_nivel(int numero_pagina) {
	return 5; // numero_pagina % cant_entradas_por_tabla;
}

int obtener_desplazamiento(int direccion_logica, int numero_pagina) {
	return 6; // direccion_logica - numero_pagina * tamanio_pagina;
}
