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
	            t_pedido_disco *p = crear_pedido_escribir(id, aux->dir, aux->num_pag);
	            sem_wait(&(p->pedido_listo));
	            eliminar_pedido_disco(p);

	         }
	         aux->bit_presencia = 0;

	    }
	    t_pedido_disco *p = crear_pedido_lectura(id, dir_marco, e->num_pag);
	    sem_wait(&(p->pedido_listo));
	    eliminar_pedido_disco(p);
	    e->dir = dir_marco;
	    e->bit_presencia = 1;
	    log_info(logger, "el bit de presencia es: %d",e->bit_presencia);
	    log_info(logger,"pagina %d del proceso %d lista en memoria",e->num_pag,id);


}
