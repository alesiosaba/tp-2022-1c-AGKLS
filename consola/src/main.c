#include "../include/main.h"

int main(int argc, char** argv) {

	inicializar();

	// Chequear que están los 2 parametros que pide el TP
	// Path al archivo de pseudocodigo y tamanio de espacio de direcciones del proceso
	if(argc != 3){
		log_error(logger, "Cantidad erronea de parametros");
		return EXIT_FAILURE;
	}


	// Asigno parametros enviados por consola
	char* path_pseudocodigo = argv[1];
	char* tamanio_proceso = argv[2];

	system("clear");

	enviar_info_proceso(path_pseudocodigo, tamanio_proceso);


	while (1){
		sleep(1);

		char* leido = readline(">");
		char** split = string_split(leido, " ");
		if (string_equals_ignore_case(split[0], "exit"))
		{
			free(leido);
			liberarStringArray(split);
			break;
		}

	}


	log_info(logger, "Finalizo correctamente el programa");
	terminar_programa();
	return EXIT_SUCCESS;
}

void enviar_info_proceso(char* path_pseudocodigo, char* tamanio_proceso){
	// arma y envia el paquete con info del proceso
	paquete(path_pseudocodigo, tamanio_proceso);
}

void paquete(char* path_pseudocodigo, char* tamanio_proceso)
{
	t_paquete* paquete = crear_paquete(PAQUETE_CONSOLA);

	int tamanio;

	// creo un puntero para leer el script
	FILE* file;
	file = abrir_archivo_lectura(path_pseudocodigo);

	// en leido se almacena cada linea leida del script de codigo
	char buffer[1024];

	// primero agrego el "tamanio_proceso" al paquete
	tamanio = strlen(tamanio_proceso) + 1;
	agregar_a_paquete(paquete, tamanio_proceso, tamanio);

	// luego se agregan todas las lineas del archivo al paquete
	fgets(buffer,sizeof(buffer),file);
	while(!feof(file)){

		tamanio = strlen(buffer) + 1;
		agregar_a_paquete(paquete, buffer, tamanio);

		fgets(buffer,sizeof(buffer),file);
	}

	tamanio = strlen(buffer) + 1;
	agregar_a_paquete(paquete, buffer, tamanio);

	fclose(file);

	// se envia el paquete a kernel con el tamanio y las instrucciones del proceso
	enviar_paquete(paquete, conexion);

	// libera la memoria ocupada por el proceso
	eliminar_paquete(paquete);
}

void terminar_programa()
{
	config_destroy(config);
	log_debug(logger,CONFIGURACION_CERRADA);
	close(conexion);
	log_debug(logger,TERMINANDO_EL_LOG);
	log_destroy(logger);
}
