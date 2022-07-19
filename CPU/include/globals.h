#ifndef GLOBALS_H_
#define GLOBALS_H_


#include "../../shared/include/utils/utils.h"

// hilo de atencion a servidor dispatch
pthread_t thr_dispatch;
// hilo de atencion a servidor interrupt
pthread_t thr_interrupt;
// hilo de atencion a servidor memoria
pthread_t thr_memoria;

#endif
