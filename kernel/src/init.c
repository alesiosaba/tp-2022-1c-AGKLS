#include "../include/init.h"
//#include "../../shared/include/utils/utils.h"


config_t inicializar(char* config_path){
	system("clear");
	errorMessageAux = string_new();
//Logs y Config
	char* log_level;
	config = iniciar_config(config_path);
	config_values = leer_config();
	log_level = config_values.log_level;
	/*
	char* logger_name = string_new();
	char timestamp_string[18];
    time_t    caltime;
    struct tm * broketime;
    // find current time, convert to broken-down time
    time(&caltime);
    broketime = localtime(&caltime);
    // append timestamp in the format "_yymmdd_hhmmss"
    strftime(timestamp_string,18,"_%d-%m-%y_%H%M%S",broketime);
	string_append(&logger_name, ARCHIVO_DE_LOG);
	string_append(&logger_name, timestamp_string);
	string_append(&logger_name, ".log");
    logger = iniciar_logger(log_level, logger_name, LOGGER);
    */
    logger = iniciar_logger(log_level, ARCHIVO_DE_LOG, LOGGER);
//globales
	idProceso = 0;
    listaNew = list_create();
    listaReady = list_create();
    listaExec = list_create();
    listaExit = list_create();
    listaBlocked = list_create();
    listaSuspendedReady = list_create();
    listaSuspendedBlocked = list_create();
    listaProcesos = list_create();
    listaDesbloqueoPendiente = list_create();
//semaforos
    sem_init(&sem_ProcesosNew,1,0);
    sem_init(&sem_ProcesosSuspendedBlk,1,0);
    sem_init(&sem_ProcesosSuspended,1,0);
    sem_init(&sem_enviarPCB,1,0);
    sem_init(&sem_respuestaPCB,1,0);
    sem_init(&sem_comenzarProcesos,1,1);
    sem_init(&sem_multiprogramacion,1,config_values.grado_multiprog);
//MUTEX
    pthread_mutex_init(&mtx_ready,NULL);
    pthread_mutex_init(&mtx_new,NULL);
    pthread_mutex_init(&mtx_exec,NULL);
    pthread_mutex_init(&mtx_planificador,NULL);
    pthread_mutex_init(&mtx_susblk, NULL);
    pthread_mutex_init(&mtx_susrdy, NULL);
    pthread_mutex_init(&mtx_blocked, NULL);
    pthread_mutex_init(&mtx_exit, NULL);
    pthread_mutex_init(&mtx_desbloqueo, NULL);
    pthread_mutex_init(&mtx_procesos, NULL);

    return config_values;
}


ALGORITMOS str2enum (char *str)
{
     int j;
     for (j = 0;  j < sizeof (conversion) / sizeof (conversion[0]);  ++j)
         if (!strcmp (str, conversion[j].str))
             return conversion[j].val;
}

config_t leer_config(){

	config_t config_values;

	config_values.log_level = config_get_string_value(config, "LOG_LEVEL");
	config_values.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
	config_values.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	config_values.ip_cpu = config_get_string_value(config, "IP_CPU");
	config_values.puerto_cpu_dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
	config_values.puerto_cpu_interrupt= config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
	config_values.puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
	config_values.algoritmo_planificacion = str2enum(config_get_string_value(config, "ALGORITMO_PLANIFICACION"));
	config_values.estimacion_inicial = config_get_double_value(config, "ESTIMACION_INICIAL");
	config_values.alfa = config_get_double_value(config, "ALFA");
	config_values.grado_multiprog = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
	config_values.t_bloqueo_max = config_get_int_value(config, "TIEMPO_MAXIMO_BLOQUEADO");
	config_values.ip_escucha = config_get_string_value(config, "IP_ESCUCHA");

	return config_values;

}

void imprimir_config(){

    log_info(logger, "Archivo de configuracion:");
	log_info(logger, "log_level: %s", config_values.log_level);
    log_info(logger, "ip_memoria: %s", config_values.ip_memoria);
    log_info(logger, "puerto_memoria: %s", config_values.puerto_memoria);
    log_info(logger, "ip_cpu: %s", config_values.ip_cpu);
    log_info(logger, "puerto_cpu_dispatch: %s", config_values.puerto_cpu_dispatch);
    log_info(logger, "puerto_cpu_interrupt: %s", config_values.puerto_cpu_interrupt);
    log_info(logger, "puerto_escucha: %s", config_values.puerto_escucha);
    log_info(logger, "algoritmo_planificacion: %s", config_get_string_value(config, "ALGORITMO_PLANIFICACION"));
    log_info(logger, "estimacion_inicial: %f", config_values.estimacion_inicial);
    log_info(logger, "alfa: %f", config_values.alfa);
    log_info(logger, "grado_multiprog: %d", config_values.grado_multiprog);
    log_info(logger, "t_bloqueo_max: %d", config_values.t_bloqueo_max);
    log_info(logger, "ip_escucha: %s\n", config_values.ip_escucha);
}


