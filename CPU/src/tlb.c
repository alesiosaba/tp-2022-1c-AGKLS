#include "../include/tlb.h"

int busqueda_pagina_en_TLB(pcb* pcb, int numero_pagina){
	return 14;
}

bool estaEnTLB(int marco_en_TLB){
	// Si no es PAGE FAULT
	return marco_en_TLB != 1;
}
