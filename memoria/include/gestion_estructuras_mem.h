#ifndef GESTION_ESTRUCTURAS_MEM_H_
#define GESTION_ESTRUCTURAS_MEM_H_

#include "main.h"

// Crear espacio de estructuras utilizadas por el modulo
void inicializar_estructuras();

// Dentro de inicializar_estructuras()
void reservar_espacio_lectura_escritura();
void armar_bitmap_marcos();
void inicializar_tablas_de_entradas();

// ------------------

// Liberar espacio de estructuras utilizadas por el modulo
void liberar_estructuras();
void limpiar_bitmap();

#endif
