#ifndef PCB_H_
#define PCB_H_

#include "../utils/utils.h"

pcb* armar_PCB_nuevo(t_list* lista);
pcb* deserializar_PCB(t_list* lista);
char* generar_renglon_instruccion(struct instruccion instruccion_a_enviar);
t_paquete* generar_paquete_pcb(struct pcb PCB_a_enviar);
void mostrar_lista_instrucciones(nodo_instruccion* lista_instrucciones);
nodo_instruccion* armar_lista_instrucciones(t_list* lista);
nodo_instruccion* deserializar_lista_instrucciones(t_list* lista);
nodo_instruccion* nuevo_nodo_instruccion();
nodo_parametro* nuevo_nodo_parametro();
nodo_instruccion* agregar_instruccion(nodo_instruccion* lista_instrucciones, void* buffer);
nodo_instruccion* deserializar_instruccion(nodo_instruccion* lista_instrucciones, void* buffer);
void completar_nodo_instruccion(nodo_instruccion* nodo_instruccion, char* buffer_original);
nodo_parametro* agregar_primer_parametro(char* parametro);
void agregar_nuevo_parametro(nodo_instruccion* nodo_instruccion, char* parametro);
void imprimir_PCB(pcb* nodo_pcb);

#endif
