#include "../../shared/include/utils/utils.h"
#include "../include/init.h"
#include "../include/shortTerm.h"


nodo_readyQueue* nuevo_nodo_readyQueue(){
	nodo_readyQueue* nodo = (struct nodo_radyQueue*)malloc(sizeof(struct nodo_readyQueue));
	nodo->sig = NULL;
	nodo->pcb = NULL;
	return nodo;
}

nodo_readyQueue* agregarPrimerPCB(pcb* PCB){

	// genero el primer nodo de la lista de la cola de listo
	nodo_readyQueue* nodo_pcb = nuevo_nodo_readyQueue();

	nodo_pcb->pcb = PCB;

	return nodo_pcb;
};

void agregarPCB(pcb* PCB){

	nodo_readyQueue* aux = readyQueue;

	// genero el primer nodo de la lista de la cola de listo
	while(aux->sig != NULL){
		aux = aux->sig;
	}

	aux->sig = nuevo_nodo_readyQueue();
	aux->sig->pcb = *PCB;

};



void movePCBtoReady(pcb* new_pcb){
	(*new_pcb).state = READY ;

	if(readyQueue == NULL){
		agregarPrimerPCB(new_pcb);
	}else{
		agregarPCB(new_pcb);
	}

}
