#include "stdio.h"
#include "stdlib.h"
#include "../include/init.h"
#include "../include/comunicacion.h"

void conexiones (){

/*
Al iniciarse el módulo, se conectará con la Memoria y realizará un handshake inicial para recibir
la configuración relevante de la misma que le permita traducir las direcciones lógicas a físicas.
Esto debería incluir al menos cantidad de entradas por tabla de páginas y tamaño de página.
*/

	conexionAMemoria = crear_conexion(config_values.IP_memoria, config_values.puerto_memoria);

	enviar_mensaje("Soy un mensaje enviado desde CPU", conexionAMemoria);
/*
Quedará a la espera de conexión a través del puerto dispatch por parte del Kernel, quien, una vez conectado,
le enviará un PCB para ejecutar. Habiéndose recibido, se procederá a realizar el ciclo de instrucción
tomando como punto de partida la instrucción que indique el Program Counter del PCB recibido.
*/

	serverDispatch = iniciar_servidor(IP, config_values.puerto_escucha_dispatch);
	log_info(logger, "Servidor listo para recibir la conexion dispatch del kernel");
	clienteDispatch = esperar_cliente(serverDispatch);


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

int manejarConexion(int socket_cliente){

	t_list* lista;
	struct pcb *pcb;

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
			log_debug(logger, RECEPCION_PAQUETE_PCB);
			recv_paquete_pcb(socket_cliente, &pcb);
			imprimir_PCB(pcb);
			pcb->state = EXIT;
			send_paquete_pcb(socket_cliente, pcb);

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
