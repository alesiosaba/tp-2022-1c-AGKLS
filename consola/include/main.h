#ifndef CONSOLA_MAIN_H_
#define CONSOLA_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

#include "parserFile.h"
#include "init.h"

#include "../../shared/include/utils/utils.h"

void enviar_info_proceso(char* path_pseudocodigo, char* tamanio_proceso);
void paquete(char* path_pseudocodigo, char* tamanio_proceso);
void terminar_programa();

#endif
