#include "../include/utils/protocolo.h"



bool send_paquete_pcb(int fd, pcb* nodo_pcb, op_code codigo_paquete){

	t_paquete* paquete = generar_paquete_pcb(*nodo_pcb, codigo_paquete);
	enviar_paquete(paquete, fd);
	log_debug(logger, PCB_ENVIADO, nodo_pcb->id);
	return eliminar_paquete(paquete);

}

bool recv_paquete_pcb(int fd, pcb** nodo_pcb){
	t_list* lista;
	lista = recibir_paquete(fd);
	*nodo_pcb = deserializar_PCB(lista);
	log_debug(logger, PCB_DESERIALIZADO, (*nodo_pcb)->id);
	free(lista);
	return true;
}

bool recv_paquete_consola(int fd, pcb** nodo_pcb){
	t_list* lista;
	lista = recibir_paquete(fd);
	*nodo_pcb = armar_PCB_nuevo(lista);
	return true;
}

bool send_paquete_kernel(int fd, op_code codigo_paquete){
	t_paquete* paquete = crear_paquete(codigo_paquete);
	enviar_paquete(paquete, fd);
	log_debug(logger, "Enviando respuesta a la consola");
	return eliminar_paquete(paquete);
}

