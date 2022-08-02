#include "../include/gestion_solicitudes_mem.h"

int solicitud_tabla_paginas(int id_tabla, int entrada_en_tabla){
    log_info(logger,"solicitud_tabla_paginas - Dir tabla: %d Entrada: %d",id_tabla, entrada_en_tabla);
    // Busco la tabla N1 correspondiente a la dir_tabla recibida
    t_tablaN1 *tabla = list_get(tablas_primer_nivel,id_tabla);
    log_info(logger, "solicitud_tabla_paginas - Tamanio tabla 1er Nivel: %d", list_size(tabla));
    // Busco la entrada solicitada en mi tabla N1
    entrada_tabla_N1 *entrada = list_get(tabla, entrada_en_tabla);
    // Devolvemos el numero de entrada N2
    log_info(logger, "solicitud_tabla_paginas - devuelve nro de tabla N2: %d", entrada->num_tabla);
    return entrada->num_tabla;

}

int solicitud_marco(int pid, int id_tabla, int entrada_en_tabla){
	// Recibe parametros del socket fd
	log_info(logger,"solicitud_marco - dir tabla: %d, numero pagina: %d", id_tabla, entrada_en_tabla);

    proceso_en_memoria *proceso = buscar_proceso_por_id(pid);
    if(proceso->esta_suspendido == 1){
        log_info(logger,"solicitud_marco - Proceso %d esta suspendido. Desuspendiendo", pid);
        sem_wait(&(proceso->suspension_completa));
        reservar_marcos_proceso(proceso);
        proceso->esta_suspendido = 0;
        log_info(logger,"solicitud_marco - Proceso desuspendido: %d", pid);
    }

    entrada_tabla_N2 *e2 = conseguir_entrada_pagina(id_tabla, entrada_en_tabla);
    log_info(logger,"solicitud_marco - asignada tabla en entrada segundo nivel");
    if(e2->bit_presencia == 0)
    {
        log_info(logger,"solicitud_marco - bit de presencia en cero. Trayendo pagina de memoria");
        traer_pagina_a_memoria(pid, id_tabla, e2);
    }
    log_info(logger,"solicitud_marco - se devuelve marco nro %d", e2->dir);

    return e2->dir;

}

void solicitud_nuevo_proceso(int socket_cliente){
	pcb *nodo_pcb;
	recv_paquete_pcb(socket_cliente, &nodo_pcb);
	imprimir_PCB(nodo_pcb);

    log_info(logger,"solicitud_nuevo_proceso - solicitud para proceso %d de tamanio %d",nodo_pcb->id, nodo_pcb->tamanio);

    proceso_en_memoria *proceso = asignar_proceso(nodo_pcb->id, nodo_pcb->tamanio);

    // Sincronizacion tabla primer nivel
    pthread_mutex_lock(&mutex_tablasN1);
    int dir_tabla = list_add(tablas_primer_nivel, proceso->tablaN1);
    log_info(logger,"solicitud_nuevo_proceso - direccion tabla asignada %d", dir_tabla);
    pthread_mutex_unlock(&mutex_tablasN1);
    // Sincronizacion procesos en memoria
    pthread_mutex_lock(&mutex_procesos_en_memoria);
    list_add(procesos_en_memoria, proceso);
    log_info(logger,"solicitud_nuevo_proceso - agregado nuevo proceso a lista procesos_en_memoria");
    pthread_mutex_unlock(&mutex_procesos_en_memoria);

    // Reservar marcos
    reservar_marcos_proceso(proceso);
    log_debug(logger,"LOG DE TEST");
    log_info(logger,"solicitud_nuevo_proceso - reservados marcos");
    dump_bitmap(bitmap_marcos);
    //Crear solicitud de creacion de archivo swap
    t_pedido_disco *p = crear_pedido_crear_archivo_swap(nodo_pcb->id);
    log_info(logger,"solicitud_nuevo_proceso - creada solicitud de creacion archivo swap");
    sem_wait(&(p->pedido_swap_listo));
    eliminar_pedido_disco(p);
    log_info(logger,"solicitud_nuevo_proceso - eliminado pedido disco");
    //Retornar direccion tabla primer nivel
    send_respuesta_nuevo_proceso(socket_cliente, dir_tabla);
    log_info(logger,"solicitud_nuevo_proceso - termino ejecucion para proceso %d",nodo_pcb->id);
    //list_destroy_and_destroy_elements(nodo_pcb,free);
}


int solicitud_escritura_memoria(int dato, uint32_t marco, uint32_t desplazamiento){
    log_debug(logger,"solicitud_escritura_memoria - Escribiendo %d en marco %d con desplazamiento %d", dato, marco, desplazamiento);
    return escribir_memoria(dato, marco, desplazamiento);
}

uint32_t solicitud_lectura_memoria(uint32_t marco, uint32_t desplazamiento){
    log_debug(logger,"solicitud_lectura_memoria - Leyeno marco %d con desplazamiento %d", marco, desplazamiento);
    return leer_memoria(marco, desplazamiento);
}


void solicitud_suspension_proceso(int socket_cliente){
	pcb *nodo_pcb;
    recv_paquete_pcb(socket_cliente, &nodo_pcb);
    log_debug(logger,"solicitud_suspension_proceso - pcb recibido");
	imprimir_PCB(nodo_pcb);

   // TODO: Ver si la direccion tabla N1 la recibimos o la definimos en proceso_en_memoria
    proceso_en_memoria *proceso = buscar_proceso_por_id(nodo_pcb->id);

    log_info(logger, "solicitud_suspension_proceso: Se intentara suspender proceso PID: %d con dir_tabla_n1: %d", nodo_pcb->id,  nodo_pcb->tabla_paginas );

    proceso->esta_suspendido = 1;

    log_info(logger, "solicitud_suspension_proceso: Creando solicitud de suspension de proceso en SWAP" );

    // Escribir en swap aquellas paginas con bit de modificacion en 1
    t_pedido_disco* p = crear_pedido_suspension_proceso_swap(nodo_pcb->id, nodo_pcb->tabla_paginas);
    sem_wait(&(p->pedido_swap_listo));
    eliminar_pedido_disco(p);
    log_info(logger, "solicitud_suspension_proceso: Liberando marcos del proceso" );
    liberar_marcos_de_proceso(nodo_pcb->id);
    log_info(logger, "solicitud_suspension_proceso: Marcos liberados." );
    dump_bitmap(bitmap_marcos);

    log_info(logger, "solicitud_suspension_proceso: Proceso suspendido. Posteando semaforo suspension completa." );
    sem_post(&(proceso->suspension_completa));
}



void solicitud_desuspension_proceso(int socket_cliente){
	pcb *nodo_pcb;
    recv_paquete_pcb(socket_cliente, &nodo_pcb);
	log_debug(logger,"solicitud_desuspension_proceso - pcb recibido: %d", nodo_pcb->id);
    proceso_en_memoria *proceso = buscar_proceso_por_id(nodo_pcb->id);
    log_info(logger, "solicitud_desuspension_proceso: Reservando marcos para el proceso" );
	reservar_marcos_proceso(proceso);
	dump_bitmap(bitmap_marcos);
	(*proceso).esta_suspendido = 0;

}



void solicitud_eliminar_proceso(int socket_cliente){
	pcb *nodo_pcb;
	recv_paquete_pcb(socket_cliente, &nodo_pcb);
	log_debug(logger,"solicitud_eliminar_proceso - pcb recibido");
    imprimir_PCB(nodo_pcb);
    proceso_en_memoria *proceso = buscar_proceso_por_id(nodo_pcb->id);
    log_info(logger, "solicitud_eliminar_proceso: Se intentara eliminar proceso PID: %d con dir_tabla_n1: %d", nodo_pcb->id,  nodo_pcb->tabla_paginas );

    if(proceso->esta_suspendido == 1){
		log_debug(logger, "Eliminando proceso suspendido PID: %d",
				nodo_pcb->id);
    	sem_wait(&(proceso->suspension_completa));
		dump_bitmap(bitmap_marcos);
    	reservar_marcos_proceso(proceso);
    	dump_bitmap(bitmap_marcos);
    	proceso->esta_suspendido = 0;
    	log_info(logger,"solicitud_eliminar_proceso - se desuspendio PID: %d", nodo_pcb->id);
    }

	log_info(logger,"solicitud_eliminar_proceso - eliminando paginas del proceso");
    eliminar_paginas_proceso(nodo_pcb->id , nodo_pcb->tabla_paginas);
    t_pedido_disco *p = crear_pedido_eliminar_archivo_swap(nodo_pcb->id);
    sem_wait(&(p->pedido_swap_listo));
    eliminar_pedido_disco(p);
	log_info(logger,"solicitud_eliminar_proceso - eliminando estructura del proceso en memoria");
	dump_bitmap(bitmap_marcos);
    eliminar_estructura_proceso(nodo_pcb->id);
    //list_destroy(parametros);
}










