#include "../include/init.h"
#include "../../shared/include/utils/utils.h"

void inicializar(){

	// inicio el logger del cpu
	logger = iniciar_logger("DEBUG",ARCHIVO_DE_LOG,LOGGER);
	log_info(logger, "Log de CPU iniciado");

	//leo la configuracion del cpu
	config = iniciar_config(ARCHIVO_DE_CONFIGURACION);
	log_info(logger, "Leyendo config");
	config_values = leer_config();
	log_info(logger, "Lei la siguiente config: \n %s \n %s \n %s \n %s \n %s \n %s \n %s \n",config_values.entradas_TLB,config_values.reemplazo_TLB,config_values.retardo_NOOP,config_values.IP_memoria,config_values.puerto_memoria,config_values.puerto_escucha_dispatch,config_values.puerto_escucha_interrupt);

}


config_t leer_config(){

	config_t config_values;

	config_values.entradas_TLB = config_get_string_value(config, "ENTRADAS_TLB");
	//config_values.entradas_TLB = config_get_int_value(config, "ENTRADAS_TLB");
	config_values.reemplazo_TLB = config_get_string_value(config, "REEMPLAZO_TLB");
	config_values.retardo_NOOP = config_get_string_value(config, "RETARDO_NOOP");
	//config_values.retardo_NOOP = config_get_int_value(config, "RETARDO_NOOP");
	config_values.IP_memoria = config_get_string_value(config, "IP_MEMORIA");
	config_values.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	config_values.puerto_escucha_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
	config_values.puerto_escucha_interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");


	return config_values;

}

// tms es la cantidad de segundos del sleep
int msleep(unsigned int tms) {
	//msleep es una funcion que duerme por la cantidad de tiempo en milisegundos ingresada.
	//usleep funciona con microsegundos por eso esta multiplicada por 1000
  return usleep(tms * 1000000);
}


void conexiones (){

/*
Al iniciarse el módulo, se conectará con la Memoria y realizará un handshake inicial para recibir
la configuración relevante de la misma que le permita traducir las direcciones lógicas a físicas.
Esto debería incluir al menos cantidad de entradas por tabla de páginas y tamaño de página.
*/

	//conexion = crear_conexion(config_values.IP_memoria, config_values.puerto_memoria);


/*
Quedará a la espera de conexión a través del puerto dispatch por parte del Kernel, quien, una vez conectado,
le enviará un PCB para ejecutar. Habiéndose recibido, se procederá a realizar el ciclo de instrucción
tomando como punto de partida la instrucción que indique el Program Counter del PCB recibido.
*/

	serverDispatch = iniciar_servidor(IP, config_values.puerto_escucha_dispatch);
	log_info(logger, "Servidor listo para recibir la conexion dispatch del kernel");
	clienteDispatch = esperar_cliente(serverDispatch);
	msleep(10);

	// conexionAKernel = crear_conexion(IP,"8000");
	while(1){
		manejarConexion(clienteDispatch);
	}

/*
Quedará a la espera también de conexión a través del puerto interrupt por parte del Kernel para recibir
mensajes de interrupción en cualquier momento, por lo que estos módulos mantendrán dos conexiones
en simultáneo.
/*
	/*
	int serverInterrupt = iniciar_servidor(config_values.puerto_escucha_interrupt);
	log_info(logger, "Servidor listo para recibir la conexion interrupt del kernel");
	int clienteInterrupt = esperar_cliente(serverInterrupt);
	*/
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

struct nodo_instruccion* armar_nodo_instruccion(void* buffer){

	nodo_instruccion* nuevo_nodo = nuevo_nodo_instruccion();

	completar_nodo_instruccion(nuevo_nodo, buffer);

	return nuevo_nodo;
}

struct pcb armar_PCB(t_list* lista){
	struct pcb pcb_nuevo;

	t_list* id = list_remove(lista, 0);
	pcb_nuevo.id = atoi(id);

	t_list* tamanio = list_remove(lista, 0);
	pcb_nuevo.tamanio = atoi(tamanio);

	/*
	t_list* tabla_paginas= list_remove(lista, 0);
	pcb_nuevo.tabla_paginas = atoi(tabla_paginas);

	t_list* estimacion_rafaga = list_remove(lista, 0);
	pcb_nuevo.estimacion_rafaga = atof(estimacion_rafaga);
	*/

	t_list* program_counter = list_remove(lista, 0);
	pcb_nuevo.program_counter = armar_nodo_instruccion(program_counter);

	/* Orden dentro del buffer del paquete

		 id
		 tamanio
		 tabla_paginas
		 estimacion_rafaga

		 program_counter

		 lista_instrucciones

	*/

	return pcb_nuevo;

}

int manejarConexion(int socket_cliente){

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente);
			log_info(logger, LECTURA_DE_VALORES);
			break;
		case PAQUETE_PCB:
			log_info(logger, "Recibi un pcb");
			lista = recibir_paquete(socket_cliente);
			struct pcb pcb_que_llega = armar_PCB(lista);
			// TO DO Debugger
			log_info(logger, lista->head->data);
			getchar();

			break;
		case -1:
			log_error(logger, SERVIDOR_DESCONEXION);
			return EXIT_FAILURE;
		default:
			log_warning(logger,OPERACION_DESCONOCIDA);
			break;
		}
	}

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
		printf("config destruida \n");
	}

}
