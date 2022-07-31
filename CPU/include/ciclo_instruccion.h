#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include "../../shared/include/utils/gestionPCB.h"
#include "../../shared/include/utils/messages.h"

#include "init.h"
#include "mmu.h"

void ejecutar_ciclo_instruccion(pcb** pcb);

// pasos de ciclo de instruccion
nodo_instruccion* fetch(pcb** pcb);
int decode(nodo_instruccion*);
uint32_t fetch_operands(pcb** pcb, nodo_instruccion*);
void execute(nodo_instruccion*, pcb** pcb, uint32_t valorMemoria);
void check_interrupt(pcb** pcb);


// ejecucion de instrucciones
void exec_no_op();
void exec_read(nodo_instruccion* instruccion, pcb** pcb);
void exec_write(nodo_instruccion* instruccion, pcb** pcb);
void exec_copy(nodo_instruccion* instruccion, uint32_t valorMemoria, pcb** pcb);
void exec_i_o(nodo_instruccion* instruccion, pcb** pcb);
void exec_exit(pcb** pcb);

// busca valor en direccion logica de memoria
uint32_t buscarValorEnMemoria(pcb** pcb, int direccionLogica);

// realizar escritura en una direccion logica de memoria
void escribirValorEnMemoria(pcb** pcb, int direccionLogica, uint32_t valor_a_escribir);

// imprimir instruccion a ejecutar por el ciclo de instruccion
void imprimir_instruccion(nodo_instruccion* instruccion);

uint32_t lectura_valor_en_memoria(direccion_fisica);

// manejo de flags globales en CPU
bool hay_desalojo_proceso();
void activar_flag_desalojo();
void desactivar_flag_desalojo();

bool hay_interrupcion();
void desactivar_flag_interrupcion();
void activar_flag_interrupcion();

#endif
