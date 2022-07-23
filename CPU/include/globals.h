#ifndef GLOBALS_H_
#define GLOBALS_H_


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

#endif
