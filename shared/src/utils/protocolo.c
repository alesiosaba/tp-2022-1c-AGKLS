#include "../include/utils/protocolo.h"



bool send_paquete_pcb(int fd, pcb* nodo_pcb){

	t_paquete* paquete = generar_paquete_pcb(*nodo_pcb);
	enviar_paquete(paquete, fd);
	log_debug(logger, PCB_ENVIADO, nodo_pcb->id);
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
	log_debug(logger, RECEPCION_PROCESO);
	*nodo_pcb = armar_PCB_nuevo(lista);
	return true;
}
