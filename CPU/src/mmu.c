#include "../include/mmu.h"

int traduccion_direccion_logica(pcb* pcb, int direccion_logica){

	int numero_pagina = obtener_numero_pagina(direccion_logica);
	int desplazamiento = obtener_desplazamiento(direccion_logica,numero_pagina);

	// Buscamos si la pagina está en la TLB
	int marco_en_TLB = busqueda_pagina_en_TLB(pcb,numero_pagina);

	if(estaEnTLB(marco_en_TLB)){
		log_debug(logger,"TLB HIT");
		log_debug(logger,"La pagina buscada esta en el marco %d", marco_en_TLB);

		// Devuelvo la direccion fisica compuesta de marco + desplazamiento
		return marco_en_TLB + desplazamiento;

	}
	log_debug(logger,"TLB MISS");

	int entrada_tabla_primer_nivel = obtener_entrada_tabla_primer_nivel(numero_pagina);
	int entrada_tabla_segundo_nivel = obtener_entrada_tabla_segundo_nivel(numero_pagina);


	return 1;

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
