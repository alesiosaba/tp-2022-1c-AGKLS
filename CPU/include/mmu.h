#ifndef MMU_MAIN_H_
#define MMU_MAIN_H_

#include "globals.h"
#include "tlb.h"
#include <math.h>

int traduccion_direccion_logica(pcb* pcb, int direccion_logica);
int obtener_numero_pagina(int direccion_logica);
int obtener_entrada_tabla_primer_nivel(int numero_pagina);
int obtener_entrada_tabla_segundo_nivel(int numero_pagina);

#endif
