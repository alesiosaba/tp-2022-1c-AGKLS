#include "../include/mmu.h"

/*unsigned long calcular_ultimo_instante_referencia(
	return
			(nuevo_instante_referencia->tv_sec - inicio_ejecucion_modulo_CPU.tv_sec) * 1000000
			+ nuevo_instante_referencia->tv_usec - inicio_ejecucion_modulo_CPU.tv_usec;
}*/

struct direccion_fisica traducir_dir_logica(pcb** pcb, int direccion_logica){

	log_debug(logger , "Traducir direccion logica: %d" , direccion_logica);

	struct direccion_fisica direccion_fisica_buscada;

	// Con esto vamos a calcular los ins_ult_ref
	struct timeval nuevo_instante_referencia;

	int numero_pagina = obtener_numero_pagina(direccion_logica);
	int desplazamiento = obtener_desplazamiento(direccion_logica, numero_pagina);

	// Buscamos si la pagina está en la TLB
	int marco_en_TLB = busqueda_pagina_en_TLB(numero_pagina);

	// Si la pagina tiene una entrada en la TLB (TLB-HIT)
	if(marco_en_TLB != -1){

		log_warning(logger, "TLB_HIT");
		log_info(logger, "Se encontró una entrada de TLB de la pagina %d con marco %d", numero_pagina , marco_en_TLB);
		imprimir_TLB();


		gettimeofday(&nuevo_instante_referencia, NULL);

		int i = 0;

		struct entrada_TLB* elem = list_get(TLB,i);

		while(elem->marco != marco_en_TLB){
			elem = list_get(TLB,i);
			i++;
		}

		elem->insUltRef =
				(nuevo_instante_referencia.tv_sec - inicio_ejecucion_modulo_CPU.tv_sec) * 1000000
				+ nuevo_instante_referencia.tv_usec - inicio_ejecucion_modulo_CPU.tv_usec;

		log_warning(logger , "Se actualizo la entrada con un nuevo instante de referencia");
		imprimir_entrada_TLB(i);

		direccion_fisica_buscada.marco = marco_en_TLB;
		direccion_fisica_buscada.desplazamiento = desplazamiento;

		log_debug(logger , "Traduccion a direccion fisica: %d" , direccion_fisica_buscada.marco * tamanio_pagina + desplazamiento);

		log_debug(logger,"Finaliza traduccion de direccion logica - TLB HIT");

		return direccion_fisica_buscada;

	}
	log_warning(logger,"TLB MISS");

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

	send_solicitud_tabla_N2(conexionAMemoria, id_tablaN2, entrada_tabla_segundo_nivel, (*pcb)->id);

	// RECIBO N° DE FRAME
	int frame = recv_respuesta_solicitud_N2(conexionAMemoria);

	log_debug(logger, "frame recibido: %d", frame);

	// log_debug(logger,"Frame: %d", frame);

	// Si no tuve TLB Hit SI o SI se agrega pagina con su marco a la TLB

	struct entrada_TLB* entrada_nueva = malloc(sizeof(struct entrada_TLB));

	entrada_nueva->numero_pagina = numero_pagina;
	entrada_nueva->marco = frame;

	gettimeofday(&nuevo_instante_referencia, NULL);

	entrada_nueva->insUltRef = (nuevo_instante_referencia.tv_sec - inicio_ejecucion_modulo_CPU.tv_sec) * 1000000
			+ nuevo_instante_referencia.tv_usec - inicio_ejecucion_modulo_CPU.tv_usec;

	log_info(logger, "Agregando una entrada a TLB con pagina %d marco %d ins_ult_ref %d", entrada_nueva->numero_pagina , entrada_nueva->marco, entrada_nueva->insUltRef);
	agregar_entrada_TLB(entrada_nueva);

	// La direccion fisica contiene el marco y desplazamiento para recorrer el void* en Memoria
	direccion_fisica_buscada.marco = frame;
	direccion_fisica_buscada.desplazamiento = desplazamiento;

	log_debug(logger , "Traduccion a direccion fisica: %d" , direccion_fisica_buscada.marco * tamanio_pagina + desplazamiento);

	log_debug(logger,"Finaliza traduccion de direccion logica - TLB MISS");

	return direccion_fisica_buscada;
}

// TODO: TESTEAR
int obtener_numero_pagina(int direccion_logica){
	return floor( (double) direccion_logica / (double) tamanio_pagina);
}
// TODO: TESTEAR
int obtener_entrada_tabla_primer_nivel(int numero_pagina) {
	return floor( (double) numero_pagina / (double) cant_entradas_por_tabla);
}
// TODO: TESTEAR
int obtener_entrada_tabla_segundo_nivel(int numero_pagina) {
	return numero_pagina % cant_entradas_por_tabla;
}

int obtener_desplazamiento(int direccion_logica, int numero_pagina) {
	return direccion_logica - numero_pagina * tamanio_pagina;
}
