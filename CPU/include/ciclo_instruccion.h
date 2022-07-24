#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include "../../shared/include/utils/gestionPCB.h"
#include "../../shared/include/utils/messages.h"

#include "init.h"

void ejecutar_ciclo_instruccion(pcb** pcb);

// pasos de ciclo de instruccion
nodo_instruccion* fetch(pcb** pcb);
int decode(nodo_instruccion*);
int fetch_operands(nodo_instruccion*);
void execute(nodo_instruccion*, pcb** pcb, int valorMemoria);
void check_interrupt(pcb** pcb);


// ejecucion de instrucciones
void exec_no_op();
void exec_read(nodo_instruccion* instruccion);
void exec_write(nodo_instruccion* instruccion);
void exec_copy(nodo_instruccion* instruccion, int valorMemoria);
void exec_i_o(nodo_instruccion* instruccion, pcb** pcb);
void exec_exit(pcb** pcb);

// busca valor en direccion logica de memoria
int buscarValorEnMemoria(int*);

// imprimir instruccion a ejecutar por el ciclo de instruccion
void imprimir_instruccion(nodo_instruccion* instruccion);

// manejo de flags globales en CPU
bool hay_desalojo_proceso();
void activar_flag_desalojo();
void desactivar_flag_desalojo();

bool hay_interrupcion();
void desactivar_flag_interrupcion();
void activar_flag_interrupcion();

#endif
