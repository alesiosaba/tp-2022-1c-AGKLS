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
//	             TODO: Aplicar asignacion por clock
//	             aux = aplicar_busqueda_clock(id, dir_tablaN1);
	          }
	            else if(strcmp(config_values.algoritmo_reemplazo, "CLOCK-M") == 0)
	               {
	                   log_info(logger, "Buscando por criterio CLOCK-M");
	                   //	TODO: Aplicar asignacion por clock mejorado
//	                   aux = aplicar_busqueda_clock_mejorado(id, dir_tablaN1);
	                }



	    }


}
