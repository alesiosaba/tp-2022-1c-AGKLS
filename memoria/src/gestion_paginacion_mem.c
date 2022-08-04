#include "../include/gestion_paginacion_mem.h"

int criterio_clock(entrada_tabla_N2 *e)
{
    if(e == NULL)
        return 0;
    if(e->bit_uso == 1)
    {
        e->bit_uso = 0;
        return 0;
    }
    else
        return 1;
}

entrada_tabla_N2* obtener_entrada_n2_clock(int id, int dir_tablaN1)
{
    log_warning(logger, "obtener_entrada_n2_clock - SE APLICA CRITERIO CLOCK");

    proceso_en_memoria *p = buscar_proceso_por_id(id);
    t_list *marcos_proceso = conseguir_marcos_proceso(dir_tablaN1);
    entrada_tabla_N2 *ret = NULL;
    while(criterio_clock(ret) == 0)
    {
        log_warning(logger, "criterio_clock - ******* ACCESO A SWAP *******");
        ret = list_get(marcos_proceso, p->posicion_puntero_clock);
        p->posicion_puntero_clock++;
        if(p->posicion_puntero_clock == list_size(marcos_proceso))
            p->posicion_puntero_clock = 0;
    }
    list_destroy(marcos_proceso);
    log_info(logger, "obtener_entrada_n2_clock: Pagina a reemplazar %d en marco %d", ret->num_pag, ret->dir);
    return ret;
}

int criterio_clock_mejorado(entrada_tabla_N2 *e, int vuelta)
{
    switch(vuelta)
    {
        case 0:
        	return (e->bit_uso == 0 && e->bit_modificacion == 0);
        break;

        case 1:
        if(e->bit_uso == 0 && e->bit_modificacion == 1){
            return 1;
        }
        else
        {
            e->bit_uso = 0;
            return 0;
        }
        break;

        case 2:
        	return (e->bit_uso == 0 && e->bit_modificacion == 0);
        break;

        case 3:
        	return (e->bit_uso == 0 && e->bit_modificacion == 1);
        break;

        default:
        	return 0;
        break;
    }
}

entrada_tabla_N2* obtener_entrada_n2_clock_mejorado(int id, int dir_tablaN1)
{
    log_warning(logger, "obtener_entrada_n2_clock_mejorado - SE APLICA CRITERIO CLOCK MEJORADO");

    proceso_en_memoria *p = buscar_proceso_por_id(id);
    t_list *marcos_proceso = conseguir_marcos_proceso(dir_tablaN1);

    entrada_tabla_N2 *ret = list_get(marcos_proceso, p->posicion_puntero_clock);
    int vuelta = 0;
    int pos_inicial = p->posicion_puntero_clock;

    while(criterio_clock_mejorado(ret, vuelta))
    {
        log_warning(logger, "criterio_clock_mejorado - ****** ACCESO SWAP ******");
        ret = list_get(marcos_proceso, p->posicion_puntero_clock);
        p->posicion_puntero_clock++;
        if(p->posicion_puntero_clock == list_size(marcos_proceso))
        {
            p->posicion_puntero_clock = 0;
        }
        if(p->posicion_puntero_clock == pos_inicial)
        {
            vuelta++;
        }
    }
    list_destroy(marcos_proceso);
    log_debug(logger, "obtener_entrada_n2_clock_mejorado: Pagina a reemplazar %d en marco %d", ret->num_pag, ret->dir);
    return ret;
}

// Obtiene numero de marco proceso que no este cargado en memoria
int obtener_marco_vacio_de_proceso(int pid)
{

	// Trae los numeros de marcos asignados para pid
    t_list *marcos = conseguir_numeros_marcos_proceso(pid);

    for(int i = 0; i < list_size(marcos); i++) {
        int num_marco = (int)list_get(marcos, i);
	    log_warning(logger,"obtener_marco_vacio_de_proceso: el marco numero %d de PID: %d es %d", i, pid, num_marco);
         // Por cada numero de marco, me fijo si existe una entrada N2 que lo contenga
            if(buscar_entrada_n2_por_num_marco(num_marco) == NULL){
        	    log_warning(logger,"obtener_marco_vacio_de_proceso: No se encontro entrada. El marco %d esta disponible", num_marco);
            	// Si no existe ninguna, devuelvo el numero marco
                return num_marco;
            }
    }
    log_warning(logger,"obtener_marco_vacio_de_proceso: MARCO VACIO NO ENCONTRADO");

    return MARCO_VACIO_NO_ENCONTRADO;
}

void traer_pagina_a_memoria(int id, int dir_tabla_n1 , entrada_tabla_N2 *e){
	    log_warning(logger,"traer_pagina_a_memoria: PID: %d pide de la tabla N1: %d la entrada N2: %d", id, dir_tabla_n1, e->num_pag);
	    dump_bitmap(bitmap_marcos);
	    // Nos fijamos si hay algun marco del PID que no este referenciado por ninguna entrada n2
	    int num_marco_vacio = obtener_marco_vacio_de_proceso(id);
	    // De haberlo, usamos ese numero de marco para calcular la direccion
	    int dir_marco = num_marco_vacio * config_values.tam_pagina;

	    // Si no hay, tenemos que elegir con los algoritmos de reemplazo
	    if(num_marco_vacio == MARCO_VACIO_NO_ENCONTRADO){
	        entrada_tabla_N2 *aux;
		    log_warning(logger,"PAGINA NO ENCONTRADA");

	        if(strcmp(config_values.algoritmo_reemplazo, "CLOCK") == 0)
	          {
	        	log_debug(logger, "Buscando por criterio CLOCK");
	             aux = obtener_entrada_n2_clock(id, dir_tabla_n1);
	          }
	        else if(strcmp(config_values.algoritmo_reemplazo, "CLOCK-M") == 0)
	          {
	        	log_debug(logger, "Buscando por criterio CLOCK-M");
	             aux = obtener_entrada_n2_clock_mejorado(id, dir_tabla_n1);
	          }

	        // Guardamos la direccion del marco que elegimos
	        dir_marco = aux->dir;

	        // Escribir en SWAP si fue modificada la pagina
	        if(aux->bit_modificacion == 1)
	         {
	            pedido_swap *p = crear_pedido_escribir_swap(id, aux->dir, aux->num_pag);
	            sem_wait(&(p->pedido_swap_listo));
	            eliminar_pedido_disco(p);

	         }
        	log_warning(logger, "traer_pagina_a_memoria: Tabla N1: %d se pone en 0 bit de presencia de entrada N2 n: %d dir fisica %d", dir_tabla_n1, e->num_pag, e->dir);
        	// Actualizamos bit de presencia
	         aux->bit_presencia = 0;

	    }
	    pedido_swap *p = crear_pedido_leer_swap(id, dir_marco, e->num_pag);
	    sem_wait(&(p->pedido_swap_listo));
	    eliminar_pedido_disco(p);
	    e->dir = dir_marco;
	    e->bit_presencia = 1;
    	log_debug(logger, "traer_pagina_a_memoria: Tabla N1: %d se pone en 1 bit de presencia de entrada N2 n: %d dir fisica %d", dir_tabla_n1, e->num_pag, e->dir);
	    log_warning(logger,"traer_pagina_a_memoria: pagina %d del proceso %d lista en memoria",e->num_pag,id);
}

// ej marco 0 despl 0
// retorna -> |X--------|---------|---------|---------|---------|---------|
// ej marco 0 despl 5
// retorna -> |----X----|---------|---------|---------|---------|---------|
// ej marco 1 despl 7
// retorna -> |---------|------X--|---------|---------|---------|---------|

uint32_t direccion_fisica_completa(uint32_t marco, uint32_t desplazamiento){
	return config_values.tam_pagina * marco + desplazamiento;
}

int escribir_memoria(int dato, uint32_t marco, uint32_t desplazamiento)
{
    log_debug(logger, "escribir_memoria: Se intentara escribir %d en el marco %d", dato, marco);
    // Buscamos la pagina que tiene el marco solicitado para actualizarle los bits
    log_debug(logger, "escribir_memoria: Buscando pagina que contiene marco %d",marco);
    entrada_tabla_N2 *pag = buscar_entrada_n2_por_num_marco(marco);
    log_debug(logger, "escribir_memoria: Se encontro la pagina %d", pag->num_pag);
    // Marcamos el uso
    pag->bit_uso = 1;
    // Marcamos modificacion
    pag->bit_modificacion = 1;
    log_debug(logger, "escribir_memoria: Escribiendo %d", dato);
    uint32_t desplazamiento_total = direccion_fisica_completa(marco, desplazamiento);
    log_debug(logger, "escribir_memoria: La direccion fisica final es: %d", desplazamiento_total);
    // Nos movemos la cantidad de marcos + desplazamiento interno para escribir en dicha direccion el contenido de &dato
    memcpy(espacio_lectura_escritura_procesos + desplazamiento_total, &dato, sizeof(uint32_t));
    log_debug(logger, "escribir_memoria: Se escribio exitosamente");
    return ESCRITURA_MEMORIA_EXITOSA;
}

uint32_t leer_memoria(uint32_t marco, uint32_t desplazamiento){
    log_debug(logger, "leer_memoria: Se leera el marco %d con despl %d", marco, desplazamiento);
    // Busca la pagina segun el marco
    entrada_tabla_N2 *pag = buscar_entrada_n2_por_num_marco(marco);
    // Bit de uso
    pag->bit_uso = 1;
    uint32_t dato;
    uint32_t desplazamiento_total = direccion_fisica_completa(marco, desplazamiento);
    log_debug(logger, "leer_memoria: La direccion fisica final es: %d", desplazamiento_total);
    // Nos movemos la cantidad de marcos + desplazamiento interno para leer en &dato el contenido de esa direccion
    memcpy(&dato, espacio_lectura_escritura_procesos + desplazamiento_total, sizeof(uint32_t));
    log_debug(logger, "leer_memoria: El marco %d con despl %d contiene el dato: %d", marco, desplazamiento, dato);
    return dato;
}

void liberar_marcos_de_proceso(int pid){
    log_debug(logger, "liberar_marcos_de_proceso: Liberando marcos del proceso %d", pid);
	 // Buscamos el proceso por pid en nuestra lista global de procesos
	 proceso_en_memoria *p = buscar_proceso_por_id(pid);
	 // Liberamos los marcos en nuestra lista global de marcos reservados
	 liberar_marcos_bitmap(p->marcos_reservados);
	 log_debug(logger, "liberar_marcos_de_proceso: liberados marcos en bitmap global");
	 // Liberamos los marcos de la lista de marcos_reservados del proceso
	 list_clean(p->marcos_reservados);
	 log_debug(logger, "liberar_marcos_de_proceso: liberados marcos lista de marcos reservados del proceso");
	 // TODO: ver posible leak
//	 free(p);
}



