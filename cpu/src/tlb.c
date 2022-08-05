#include "../include/tlb.h"

void limpiar_tlb(int procesoEnEjecucion) {
	if(procesoEnEjecucion != procesoAnterior){
		log_warning(logger, "Tengo que limpiar TLB - PID anterior: %d - PID nuevo: %d" , procesoAnterior, procesoEnEjecucion);
		list_clean_and_destroy_elements(TLB,free);
	}

}

void imprimir_entrada_TLB(int i){
	struct entrada_TLB* entrada_a_imprimir = malloc(sizeof(struct entrada_TLB));
	entrada_a_imprimir = list_get(TLB, i);

	log_warning(logger, "%d       | %d     | %lu", entrada_a_imprimir->numero_pagina, entrada_a_imprimir->marco, entrada_a_imprimir->insUltRef);
}

void imprimir_TLB(){
	log_warning(logger, "Imprimiendo TLB");
	log_warning(logger, "Pagina   | Marco  | ins_ult_ref");

	int i = 0;
	while(i < list_size(TLB)){
		imprimir_entrada_TLB(i);
		i++;
	}
}


int busqueda_pagina_en_TLB(int pagina_buscada) {

	log_info(logger, "Busco pagina %d en TLB" , pagina_buscada);

	int i = 0;
	struct entrada_TLB* entrada_TLB;

	while(i < list_size(TLB)){

		entrada_TLB = list_get(TLB, i);

		if(entrada_TLB->numero_pagina == pagina_buscada)
			return entrada_TLB->marco;

		i++;
	}

	// Si no estaba la pagina en TLB
	return -1;
}

bool TLB_completa() {
	// Cantidad de entradas de TLB ocupadas = config_values.entradas_TLB
	return list_size(TLB) == atoi(config_values.entradas_TLB);
}

bool algoritmo_es_FIFO() {
	return strcmp(config_values.reemplazo_TLB, "FIFO") == 0;
}

bool instante_referencia_mas_viejo(struct entrada_TLB* entrada1, struct entrada_TLB* entrada2){

	// Si el de la izq es mas viejo la resta es negativa
	// Por lo tanto devuelve true

	return entrada1->insUltRef <= entrada2->insUltRef;
	// 1658884111 - 1658884117
	// (inst 1) - (inst 1 despues de 6 segundos)
}

void ordenar_TLB() {

	log_debug(logger, "TLB antes de ordenarla:");

	if(strcmp("DEBUG", config_values.log_level) == 0)
		imprimir_TLB();

	if (!algoritmo_es_FIFO()) {
		// ordeno por LRU
		log_info(logger, "Hago Ordenamiento LRU");
		list_sort(TLB, (void*)instante_referencia_mas_viejo);
	}

	log_debug(logger, "TLB despues de ordenarla:");

	if(strcmp("DEBUG", config_values.log_level) == 0)
		imprimir_TLB();

}

void aplicar_algoritmo_reemplazo_TLB(struct entrada_TLB* entrada_nueva) {
	ordenar_TLB();

	// Sacar elemento decidido por algoritmo
	list_remove(TLB,0);
}

void agregar_entrada_TLB(struct entrada_TLB* entrada_nueva) {

	if(TLB_completa()){
		log_warning(logger, "La TLB esta completa!");
		aplicar_algoritmo_reemplazo_TLB(entrada_nueva);  // Hacer reeemplazo con algoritmo
		list_add(TLB, entrada_nueva);

		log_info(logger , "Imprimiendo TLB despues de aplicar algoritmo de reemplazo");
		imprimir_TLB();
	}
	else{
		log_info(logger, "La TLB NO esta completa! SOLO agrego la nueva entrada a la TLB");
		list_add(TLB, entrada_nueva);

		log_debug(logger , "Imprimiendo TLB despues de SOLO agregar una nueva entrada");

		if(strcmp("DEBUG", config_values.log_level) == 0)
				imprimir_TLB();
	}

}

bool estaEnTLB(int marco_en_TLB) {
	// Si no es PAGE FAULT
	return marco_en_TLB != -1;
	// PAGE_FAULT es -1
}
