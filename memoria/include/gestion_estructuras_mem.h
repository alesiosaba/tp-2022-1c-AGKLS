#ifndef GESTION_ESTRUCTURAS_MEM_H_
#define GESTION_ESTRUCTURAS_MEM_H_

#include "main.h"
#include <semaphore.h>

//MEMORIA PRINCIPAL (espacio usuario)
// Crear espacio de estructuras utilizadas por el modulo
void inicializar_estructuras();

// Dentro de inicializar_estructuras()
void reservar_espacio_lectura_escritura();
void armar_bitmap_marcos();
void inicializar_tablas_de_entradas();
entrada_tabla_N1* agregar_entrada_tabla_primer_nivel(t_tablaN1 *tabla);
entrada_tabla_N2* agregar_entrada_tabla_segundo_nivel(t_tablaN2 *tabla);
entrada_tabla_N2* tabla_contiene_marco(t_tablaN2 *t, int num_marco);
entrada_tabla_N2* conseguir_entrada_pagina(int dir_tablaN1, int pag);
entrada_tabla_N2* conseguir_pagina_en_marco(int num_marco);
t_list* conseguir_marcos_proceso(int dir_tablaN1);
t_list* conseguir_numeros_marcos_proceso(int id);
t_tablaN1* crear_tablaN1(int tamanio_proceso);
void reservar_marcos_proceso(proceso_en_memoria *p);
// Procesos
proceso_en_memoria* buscar_proceso_por_id(int id);
proceso_en_memoria* asignar_proceso(int id, int tamanio_proceso);

t_list* reservar_paginas(int idProceso, int tamanioProceso);

// Liberar espacio de estructuras utilizadas por el modulo
void liberar_estructuras();
void limpiar_bitmap();
void dump_bitmap();
void liberar_marcos_bitmap(t_list *marcos);
#endif
