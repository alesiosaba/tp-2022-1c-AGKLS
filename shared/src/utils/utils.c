#include "../include/utils/utils.h"

// Implementacion de Comportamientos del Logger & Config

t_log* iniciar_logger(char* log_level, char* logger_path, char* logger)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create(logger_path, logger ,true,log_level_from_string(log_level));
	return nuevo_logger;
}

t_config* iniciar_config(char* config_path)
{
	t_config* nuevo_config;
	nuevo_config = config_create(config_path);
	return nuevo_config;
}


// Implementacion de Otras Utilidades

//msleep es una funcion que duerme por la cantidad de tiempo en milisegundos ingresada.
// tms es la cantidad de segundos del sleep
int msleep(unsigned int tms) {
	//usleep funciona con microsegundos por eso esta multiplicada por 1000
  return usleep(tms * 1000000);
}

int tamanioStringArray(char** a)
{
	int i = 0;
	while(a[i] != NULL)
	{
		i++;
	}
	return i;
}

void liberarStringArray(char** stringArray){

	int size = tamanioStringArray(stringArray);

	for(int i = 0; i< size; i++){

		free(stringArray[i]);


	}

	free(stringArray);

}

// esta funcion abre un archivo en modo lectura y retorna un puntero al "file"
FILE* abrir_archivo_lectura(char* path_pseudocodigo){
	FILE* file = fopen(path_pseudocodigo,"r");

	if(file == NULL) {
		// TO DO
		// log_error(logger, "Error al abrir archivo");
		printf("no se pudo abrir el archivo con la ruta especificada");
		exit(1);
	}

	return file;
};

void error_handler(t_log* logger, char* message_handler, char *message, char *var1, char *var2){
	message_handler = string_new();

	string_append(&message_handler, message);
	if(var1)
		string_append(&message_handler, var1);
	if(var2)
		string_append(&message_handler, var2);

	log_error(logger, message_handler);

}
