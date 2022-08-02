#ifndef MMU_MAIN_H_
#define MMU_MAIN_H_

#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/protocolo.h"
#include "tlb.h"
#include "init.h"
#include <math.h>

struct direccion_fisica traducir_dir_logica(pcb** pcb, int direccion_logica);
int obtener_numero_pagina(int direccion_logica);
int obtener_entrada_tabla_primer_nivel(int numero_pagina);
int obtener_entrada_tabla_segundo_nivel(int numero_pagina);

#endif
