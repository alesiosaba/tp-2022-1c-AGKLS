#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "../../shared/include/utils/protocolo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <unistd.h>


#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/sockets.h"
#include "../../shared/include/utils/protocolo.h"

bool gv_flag_interrupcion;

// hilo de atencion a servidor dispatch
pthread_t thr_dispatch;
// hilo de atencion a servidor interrupt
pthread_t thr_interrupt;
// hilo de atencion a servidor memoria
pthread_t thr_memoria;

// Info del Handshake Inicial
// Tamaño de pagina (viene de la config del modulo Memoria)
int tamanio_pagina;
// Cantidad de entradas por tabla (viene de la config del modulo Memoria)
int cant_entradas_por_tabla;

#endif
