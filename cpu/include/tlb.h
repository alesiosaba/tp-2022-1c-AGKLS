#ifndef MMU_MAIN_H_
#define MMU_MAIN_H_

#include "globals.h"
#include "init.h"

const int PAGEFAULT = -1;

void limpiar_tlb(int procesoEnEjecucion);
int busqueda_pagina_en_TLB(int numero_pagina);
bool TLB_completa();
bool algoritmo_es_FIFO();
void ordenar_TLB();
void aplicar_algoritmo_reemplazo_TLB(struct entrada_TLB* entrada_nueva);
void agregar_entrada_TLB(struct entrada_TLB* entrada_nueva);
bool estaEnTLB(int marco_en_TLB);
void limpiar_tlb(int procesoEnEjecucion);

#endif
