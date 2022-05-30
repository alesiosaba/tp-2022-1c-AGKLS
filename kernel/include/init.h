#ifndef INIT_H_
#define INIT_H_


#define ARCHIVO_DE_CONFIGURACION "kernel.config"
#define ARCHIVO_DE_LOG "kernel.log"
#define LOGGER "Kernel Log"


typedef struct config_t
{
	char*  log_level;
	char*  ip_memoria;
	char*  puerto_memoria;
	char*  ip_cpu;
	char*  puerto_cpu_dispatch;
	char*  puerto_cpu_interrupt;
	char*  puerto_escucha;
	char*  algoritmo_planificacion;
	double estimacion_inicial;
	float  alfa;
	int	   grado_multiprog;
	int	   t_bloqueo_max;
} config_t;

config_t config_values;

config_t inicializar();
config_t leer_config();

#endif
