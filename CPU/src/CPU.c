#include "../include/CPU.h"




int main(void) {
	inicializar();

}
/*
	// inicio el logger del cpu
	loggerCPU = iniciar_logger("DEBUG","CPU.log","CPU LOG");
	log_info(loggerCPU, "Log de CPU iniciado");

	//leo la configuracion del cpu
	configCPU = iniciar_config("CPU.config");
//	config_values = leer_config();
//	log_info(logger, "Lei la siguiente config: %s \n %s \n %s \n %s \n %s \n %s \n %s \n",config_values.entradas_TLB,config_values.reemplazo_TLB,config_values.retardo_NOOP,config_values.IP_memoria,config_values.puerto_memoria,config_values.puerto_escucha_dispatch,config_values.puerto_escucha_interrupt);


//	terminar_programa(conexion, logger, config);
//	terminar_programa(loggerCPU, configCPU);

	return EXIT_SUCCESS;
}
/*
config_t leer_config(){

	config_t config_values;

	config_values.entradas_TLB = config_get_string_value(configCPU, "ENTRADAS_TLB");
	//config_values.entradas_TLB = config_get_int_value(configCPU, "ENTRADAS_TLB");
	config_values.reemplazo_TLB = config_get_string_value(configCPU, "REEMPLAZO_TLB");
	config_values.retardo_NOOP = config_get_string_value(configCPU, "RETARDO_NOOP");
	//config_values.retardo_NOOP = config_get_int_value(configCPU, "RETARDO_NOOP");
	config_values.IP_memoria = config_get_string_value(configCPU, "	IP_MEMORIA");
	config_values.puerto_memoria = config_get_string_value(configCPU, "PUERTO_MEMORIA");
	config_values.puerto_escucha_dispatch = config_get_string_value(configCPU, "PUERTO_ESCUCHA_DISPATCH");
	config_values.puerto_escucha_interrupt = config_get_string_value(configCPU, "PUERTO_ESCUCHA_INTERRUPT");

	return config_values;
/*
}


void terminar_programa(t_log* logger, t_config* config)
{
	// Libero lo que utilizamos (conexion, log y config)

	//TO DO
	//conexion

	if(logger != NULL) {
		log_destroy(logger);
		printf("\nlogger destruido \n");
	}
	if(config != NULL) {
		config_destroy(config);
		printf("config destruido \n");
	}

}*/

