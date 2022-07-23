#ifndef MMU_MAIN_H_
#define MMU_MAIN_H_

#include "globals.h"

int busqueda_pagina_en_TLB(pcb* pcb, int numero_pagina);
bool estaEnTLB(marco_en_TLB);

#endif
