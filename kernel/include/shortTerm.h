#ifndef STS_H_
#define STS_H_

typedef struct nodo_readyQueue
{
	pcb* pcb;
	struct nodo_pcb* sig;
} nodo_readyQueue;

extern nodo_readyQueue* readyQueue;


nodo_readyQueue* nuevo_nodo_readyQueue();
nodo_readyQueue* agregarPrimerPCB(pcb* PCB);
void agregarPCB(pcb* pcb);

void movePCBtoReady(pcb* PCB);

#endif
