#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <readline/readline.h>
#include <pthread.h>
#include <string.h>
#include "../../shared/include/utils/utils.h"
#include "../../shared/include/utils/sockets.h"

typedef enum {FIFO, SRT} ALGORITMOS;

const static struct {
	ALGORITMOS  val;
    const char *str;
} conversion [] = {
    {FIFO, "FIFO"},
    {SRT, "SRT"},
};

ALGORITMOS str2enum (char *str);

typedef struct config_t
{
	char*  log_level;
	char*  ip_memoria;
	char*  puerto_memoria;
	char*  ip_cpu;
	char*  puerto_cpu_dispatch;
	char*  puerto_cpu_interrupt;
	char*  puerto_escucha;
	ALGORITMOS  algoritmo_planificacion;
	double estimacion_inicial;
	float  alfa;
	int	   grado_multiprog;
	int	   t_bloqueo_max;
} config_t;


config_t config_values;

pthread_t thr_cpu;
pthread_t thr_cpu_interrupt;
pthread_t thr_comandos;
pthread_t thr_consolas;

//SOCKETS
int server_fd, socketMemoria, conexionACPU, conexionACPU_interrupt;


char* errorMessageAux;


/*
pthread_t hiloServer;
pthread_t hiloDispatch;
pthread_t hiloParaComandos;
pthread_t hiloPlanificacionCortoPlazo;
pthread_t hiloPlanificacionMedianoPlazo[1000];
pthread_t hiloPlanificacionLargoPlazo;
pthread_t hiloBloqueos;
pthread_t hiloSuspended;
*/

#endif
