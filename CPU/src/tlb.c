#include "../include/tlb.h"

void limpiar_tlb(int procesoEnEjecucion) {
	if(procesoEnEjecucion != procesoAnterior)
		list_clean_and_destroy_elements(TLB,free);
}


int busqueda_pagina_en_TLB(int pagina_buscada) {

	int i = 0;
	struct entrada_TLB* entrada_TLB;

	while(i < list_size(TLB)){

		entrada_TLB = list_get(TLB, i);

		if(entrada_TLB->numero_pagina == pagina_buscada)
			return entrada_TLB->marco;

		i++;
	}

	// Si no estaba la pagina en TLB
	return PAGEFAULT;
}

bool TLB_completa() {
	return list_size(TLB) == atoi(config_values.entradas_TLB);
}

bool algoritmo_es_FIFO() {
	return strcmp(config_values.reemplazo_TLB, "FIFO") == 0;
}

void ordenar_TLB() {
	if (!algoritmo_es_FIFO()) {
		// ordeno por LRU
		// TODO: usar list_sort(comparar_por_timestamps)
	}
}

void aplicar_algoritmo_reemplazo_TLB(struct entrada_TLB* entrada_nueva) {
	ordenar_TLB();

	// Sacar elemento decidido por algoritmo
	// todo: Fijarse que LRU deje adelante de todooo el elemento a sacar
	list_remove(TLB,0);
}

void agregar_entrada_TLB(struct entrada_TLB* entrada_nueva) {

	if(TLB_completa())
		aplicar_algoritmo_reemplazo_TLB(entrada_nueva);  // Hacer reeemplazo con algoritmo

	list_add(TLB, entrada_nueva);

}

bool estaEnTLB(int marco_en_TLB) {
	// Si no es PAGE FAULT
	return marco_en_TLB != PAGEFAULT;
}
