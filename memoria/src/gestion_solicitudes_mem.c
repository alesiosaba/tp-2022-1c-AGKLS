#include "../include/gestion_solicitudes_mem.h"

void solicitud_tabla_paginas(int socket_cliente, t_log *logger){
	// Recibe parametros del socket fd
    t_list *parametros = recibir_paquete(socket_cliente);
    // Direccion de la tabla y numero de entrada
    int *dir_tabla = list_get(parametros, 0);
    int *num_entrada = list_get(parametros, 1);
    log_info(logger,"solicitud_tabla_paginas - Dir tabla: %d Entrada: %d", *dir_tabla, *num_entrada);

    // Tablas de 1er nivel
    t_tablaN1 *tabla = list_get(tablasN1, *dir_tabla);
    log_info(logger, "Tamanio tabla 1er Nivel: %d", list_size(tabla));
    // Busca la entrada
    entrada_tabla_N1 *entrada = list_get(tabla, *num_entrada);
    // Direccion N2
    t_tablaN2 *direccionN2 = list_get(tablasN2, entrada->dir);
    // Enviamos direccion N2 encontrada
    log_info(logger,"Enviando  %d", *num_entrada);
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

	// TODO: Contemplar procesos suspendidos



    entrada_tabla_N2 *e2 = conseguir_entrada_pagina(*dir_tablaN1, *num_pag);

    if(e2->bit_presencia == 0)
    {
        traer_pagina_a_memoria(*id, *dir_tablaN1, e2);
    }

    enviar_num(socket_cliente, e2->dir, logger);
    log_info(logger, "envio de marco para el proceso: %d", *id);
    list_destroy_and_destroy_elements(parametros,free);

}
