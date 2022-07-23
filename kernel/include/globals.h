#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <readline/readline.h>
#include <signal.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
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
	double  alfa;
	int	   grado_multiprog;
	int	   t_bloqueo_max;
} config_t;


config_t config_values;

pthread_t thr_cpu;
pthread_t thr_cpu_interrupt;
pthread_t thr_memoria;
pthread_t thr_comandos;
pthread_t thr_consolas;
pthread_t thr_planifLT;
pthread_t thr_planifST;
pthread_t thr_planifMT[1024];
pthread_t thr_bloqueos;

//SOCKETS
int server_fd;
int socketMemoria;
int conexionACPU;
int conexionACPU_interrupt;
int conexionAMemoria;


char* errorMessageAux;

//PROCESOS
int arr_procesos[1024];
int idProceso;

//LISTAS
t_list* listaNew;
t_list* listaReady;
t_list* listaExec;
t_list* listaExit;
t_list* listaBlocked;
t_list* listaSuspendedReady;
t_list* listaSuspendedBlocked;
t_list* listaProcesos;
t_list* listaDesbloqueoPendiente;

//SEMAFOROS
sem_t sem_ProcesosNew;
sem_t sem_ProcesosReady;
sem_t sem_comenzarProcesos;
sem_t sem_enviarPCB;
sem_t sem_respuestaPCB;
sem_t sem_multiprogramacion;
sem_t sem_ProcesosBloqueo;

//MUTEX
pthread_mutex_t mtx_blocked;
pthread_mutex_t mtx_susblk;
pthread_mutex_t	mtx_planificador;


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
