#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/messages.h"
#include "../../shared/include/utils/gestionPCB.h"

void ejecutar_ciclo_instruccion(pcb** pcb);
// buscar la próxima instrucción a ejecutar
void fetch();
// interpretar qué instrucción es la que se va a ejecutar (evalua si ejecuta fetch_operands y como)
void decode();
// buscar valor en memoria del parametro de COPY
void fetch_operands();
// ejecucion de instruccion
void execute();
// chequear si el Kernel nos envió una interrupcion
void check_interrupt();

#endif
