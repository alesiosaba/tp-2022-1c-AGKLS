#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/messages.h"
#include "../../shared/include/utils/gestionPCB.h"

void ejecutar_ciclo_instruccion(pcb** pcb);
// buscar la próxima instrucción a ejecutar
nodo_instruccion* fetch(pcb** pcb);
// interpretar qué instrucción es la que se va a ejecutar (evalua si ejecuta fetch_operands y como)
int decode(nodo_instruccion*);
// buscar valor en memoria del parametro de COPY
int fetch_operands(nodo_instruccion*);
// ejecucion de instruccion
void execute(nodo_instruccion*);
// chequear si el Kernel nos envió una interrupcion
void check_interrupt();
// busca valor en direccion logica de memoria
int buscarValorEnMemoria(int*);

#endif
