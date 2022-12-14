#ifndef PCB_H_
#define PCB_H_

#include "../utils/utils.h"


const static struct {
    status val;
    const int str;
} conversion_status [] = {
	{NEW, 0},
	{READY, 1},
	{EXECUTION, 2},
	{BLOCKED, 3},
	{SUSPENDED_READY, 4},
	{SUSPENDED_BLOCKED, 5},
	{EXIT, 6},
};

status str_to_status_enum(char* str_status);

pcb* armar_PCB_nuevo(t_list* lista);
pcb* deserializar_PCB(t_list* lista);
char* generar_renglon_instruccion(struct instruccion* instruccion_a_enviar);
t_paquete* generar_paquete_pcb(struct pcb PCB_a_enviar, op_code codigo_paquete);
void mostrar_lista_instrucciones(t_list* lista_instrucciones);
void armar_lista_instrucciones(t_list* instrucciones_paquete, t_list* lista_instrucciones);
void deserializar_lista_instrucciones(t_list* instrucciones_paquete, t_list* lista_instrucciones);
void deserializar_instruccion(t_list* lista_instrucciones, void* buffer);
nodo_instruccion* nuevo_nodo_instruccion();
nodo_parametro* nuevo_nodo_parametro();
void agregar_instruccion(t_list* lista_instrucciones, void* buffer);
void completar_nodo_instruccion(nodo_instruccion* nodo_instruccion, char* buffer_original);
void agregar_nuevo_parametro(nodo_instruccion* nodo_instruccion, char* parametro);
void imprimir_PCB(pcb* nodo_pcb);
char* imprimir_status(status status);

void destruir_parametros(nodo_parametro* parametro);
void destruir_instrucciones(nodo_instruccion* ins);
void destruir_PCB(pcb* pcb);

//FOR TESTING
pcb* generar_pcb_dummy();

#endif
