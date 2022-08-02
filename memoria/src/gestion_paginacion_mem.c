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

entrada_tabla_N2* aplicar_busqueda_clock(int id, int dir_tablaN1)
{
    proceso_en_memoria *p = buscar_proceso_por_id(id);
    t_list *marcos_proceso = conseguir_marcos_proceso(dir_tablaN1);
    entrada_tabla_N2 *ret = NULL;
    while(criterio_clock(ret) == 0)
    {
        ret = list_get(marcos_proceso, p->posicion_puntero_clock);
        p->posicion_puntero_clock++;
        if(p->posicion_puntero_clock == list_size(marcos_proceso))
            p->posicion_puntero_clock = 0;
    }
    list_destroy(marcos_proceso);
    log_info(logger, "Pagina a reemplazar %d en marco %d", ret->num_pag, ret->dir);
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
        if(e->bit_uso == 0 && e->bit_modificacion == 1)
            return 1;
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

entrada_tabla_N2* aplicar_busqueda_clock_mejorado(int id, int dir_tablaN1)
{
    proceso_en_memoria *p = buscar_proceso_por_id(id);
    t_list *marcos_proceso = conseguir_marcos_proceso(dir_tablaN1);

    entrada_tabla_N2 *ret = list_get(marcos_proceso, p->posicion_puntero_clock);
    int vuelta = 0;
    int pos_inicial = p->posicion_puntero_clock;

    while(criterio_clock_mejorado(ret, vuelta))
    {
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
    log_info(logger, "Pagina a reemplazar %d en marco %d", ret->num_pag, ret->dir);
    return ret;
}

int dir_marco_vacio_proceso(int id)
{

    t_list *marcos = conseguir_numeros_marcos_proceso(id);
    for(int i = 0; i < list_size(marcos); i++) {
        int num_marco = (int)list_get(marcos, i);
         //SI ENCUENTRA VACIO LO RETORNA
            if(conseguir_pagina_en_marco(num_marco) == NULL)
                return num_marco;
    }
    return PAGINA_NO_ENCONTRADA;
}

void traer_pagina_a_memoria(int id, int dir_tablaN1 , entrada_tabla_N2 *e){
	  //DIR MARCO VACIO O -1 SI NO ENCUENTRA
	    int num_marco = dir_marco_vacio_proceso(id);
	    int dir_marco = num_marco * config_values.tam_pagina;
	    if(num_marco == PAGINA_NO_ENCONTRADA){
	        entrada_tabla_N2 *aux;

	        if(strcmp(config_values.algoritmo_reemplazo, "CLOCK") == 0)
	          {
	             log_info(logger, "Buscando por criterio CLOCK");
	             aux = aplicar_busqueda_clock(id, dir_tablaN1);
	          }
	        else if(strcmp(config_values.algoritmo_reemplazo, "CLOCK-M") == 0)
	          {
	             log_info(logger, "Buscando por criterio CLOCK-M");
	             aux = aplicar_busqueda_clock_mejorado(id, dir_tablaN1);
	          }

	        //GUARDAR DIR MARCO ELEGIDO
	        dir_marco = aux->dir;
	        //SI FUE MODIFICADO, ESCRIBIR PAGINA EN MEMORIA
	        if(aux->bit_modificacion == 1)
	         {
	            t_pedido_disco *p = crear_pedido_escribir_swap(id, aux->dir, aux->num_pag);
	            sem_wait(&(p->pedido_swap_listo));
	            eliminar_pedido_disco(p);

	         }
	         aux->bit_presencia = 0;

	    }
	    t_pedido_disco *p = crear_pedido_leer_swap(id, dir_marco, e->num_pag);
	    sem_wait(&(p->pedido_swap_listo));
	    eliminar_pedido_disco(p);
	    e->dir = dir_marco;
	    e->bit_presencia = 1;
	    log_info(logger, "el bit de presencia es: %d",e->bit_presencia);
	    log_info(logger,"pagina %d del proceso %d lista en memoria",e->num_pag,id);


}


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
    log_info(logger, "escribir_memoria: Buscando pagina que contiene marco %d",marco);
    entrada_tabla_N2 *pag = conseguir_pagina_en_marco(marco);
    log_info(logger, "escribir_memoria: Se encontro la pagina %d", pag->num_pag);
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
    entrada_tabla_N2 *pag = conseguir_pagina_en_marco(marco);
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
    log_info(logger, "liberar_marcos_de_proceso: Liberando marcos del proceso %d", pid);
	 // Buscamos el proceso por pid en nuestra lista global de procesos
	 proceso_en_memoria *p = buscar_proceso_por_id(pid);
	 // Liberamos los marcos en nuestra lista global de marcos reservados
	 liberar_marcos_bitmap(p->marcos_reservados);
	 log_info(logger, "liberar_marcos_de_proceso: liberados marcos en bitmap global");
	 // Liberamos los marcos de la lista de marcos_reservados del proceso
	 list_clean(p->marcos_reservados);
	 log_info(logger, "liberar_marcos_de_proceso: liberados marcos lista de marcos reservados del proceso");
	 // TODO: ver posible leak
//	 free(p);
}



