#include "../include/gestion_solicitudes_mem.h"

void solicitud_tabla_paginas(int socket_cliente, t_log *logger){
	// Recibe parametros del socket fd
    t_list *parametros = recibir_paquete(socket_cliente);
    // Direccion de la tabla y numero de entrada
    int *dir_tabla = list_get(parametros, 0);
    int *num_entrada = list_get(parametros, 1);
    log_info(logger,"solicitud_tabla_paginas - Dir tabla: %d Entrada: %d", *dir_tabla, *num_entrada);
    // Tablas de 1er nivel
    t_tablaN1 *tabla = list_get(entradas_tabla_primer_nivel, *dir_tabla);
    log_info(logger, "solicitud_tabla_paginas - Tamanio tabla 1er Nivel: %d", list_size(tabla));
    // Busca la entrada
    entrada_tabla_N1 *entrada = list_get(tabla, *num_entrada);
    // Direccion N2
    t_tablaN2 *direccionN2 = list_get(entradas_tabla_segundo_nivel, entrada->dir);
    // Enviamos direccion N2 encontrada
    log_info(logger,"solicitud_tabla_paginas - Enviando  %d", *num_entrada);
    enviar_tabla_N2(socket_cliente, direccionN2, logger);
    // Liberamos memoria
    list_destroy_and_destroy_elements(parametros,free);
}

void solicitud_marco(int socket_cliente, t_log *logger){
	// Recibe parametros del socket fd
	t_list *parametros = recibir_paquete(socket_cliente);
	int *id = list_get(parametros, 0);
	int *dir_tablaN1 = list_get(parametros, 1);
	int *num_pag = list_get(parametros, 2);
	log_info(logger,"solicitud_marco - dir tabla: %d, numero pagina: %d", *dir_tablaN1, *num_pag);

    proceso_en_memoria *proceso = buscar_proceso_por_id(*id);
    if(proceso->esta_suspendido == 1){
        log_info(logger,"solicitud_marco - Proceso %d esta suspendido. Desuspendiendo", *id);
        sem_wait(&(proceso->suspension_completa));
        reservar_marcos_proceso(proceso);
        proceso->esta_suspendido = 0;
        log_info(logger,"solicitud_marco - Proceso desuspendido: %d", *id);
    }

    entrada_tabla_N2 *e2 = conseguir_entrada_pagina(*dir_tablaN1, *num_pag);
    log_info(logger,"solicitud_marco - asignada tabla en entrada segundo nivel");
    if(e2->bit_presencia == 0)
    {
        log_info(logger,"solicitud_marco - bit de presencia en cero. Trayendo pagina de memoria");
        traer_pagina_a_memoria(*id, *dir_tablaN1, e2);
    }

    enviar_num(socket_cliente, e2->dir, logger);
    log_info(logger, "solicitud_marco - enviado direccion n2");
    list_destroy_and_destroy_elements(parametros,free);

}

void solicitud_nuevo_proceso(int socket_cliente){
	pcb *nodo_pcb;
	recv_paquete_pcb(socket_cliente, &nodo_pcb);

    log_info(logger,"solicitud_nuevo_proceso - solicitud para proceso %d de tamanio %d",nodo_pcb->id, nodo_pcb->tamanio);

    proceso_en_memoria *proceso = asignar_proceso(nodo_pcb->id, nodo_pcb->tamanio);

    // Sincronizacion tabla primer nivel
    pthread_mutex_lock(&mutex_tablasN1);
    int dir_tabla = list_add(entradas_tabla_primer_nivel, proceso->tablaN1);
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
   // log_info(logger,"solicitud_nuevo_proceso - reservados marcos");
    dump_bitmap(bitmap_marcos);
    //Crear solicitud de creacion de archivo swap
    t_pedido_disco *p = crear_pedido_crear_archivo_swap(nodo_pcb->id);
    log_info(logger,"solicitud_nuevo_proceso - creada solicitud de creacion archivo swap");
    // TODO: Postear semaforo cuando esta listo el pedido
    sem_wait(&(p->pedido_swap_listo));
    eliminar_pedido_disco(p);
    log_info(logger,"solicitud_nuevo_proceso - eliminado pedido disco");
//    Retornar direccion tabla primer nivel

    send_respuesta_nuevo_proceso(socket_cliente, dir_tabla);

    log_info(logger,"solicitud_nuevo_proceso - termino ejecucion para proceso %d",nodo_pcb->id);
    //list_destroy_and_destroy_elements(nodo_pcb,free);
}
