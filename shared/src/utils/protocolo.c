#include "../include/utils/protocolo.h"



bool send_paquete_pcb(int fd, pcb* nodo_pcb){

	log_debug(logger, GENERANDO_PCB);
	t_paquete* paquete = generar_paquete_pcb(*nodo_pcb);
	enviar_paquete(paquete, fd);

	return eliminar_paquete(paquete);

}

bool recv_paquete_pcb(int fd, pcb** nodo_pcb){
	t_list* lista;
	lista = recibir_paquete(fd);
	*nodo_pcb = deserializar_PCB(lista);
	free(lista);

	return true;
}

bool recv_paquete_consola(int fd, pcb** nodo_pcb){
	t_list* lista;
	lista = recibir_paquete(fd);
	log_debug(logger, RECEPCION_PAQUETE_CPU);
	*nodo_pcb = armar_PCB_nuevo(lista);
	return true;
}
