#include "../include/gestion_paginacion_mem.h"

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
