#ifndef PCB_H_
#define PCB_H_


nodo_pcb* armar_PCB(t_list* lista);
char* generar_renglon_instruccion(struct instruccion instruccion_a_enviar);
t_paquete* generar_paquete_pcb(struct pcb PCB_a_enviar);






#endif
