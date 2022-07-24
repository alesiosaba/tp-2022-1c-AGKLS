#ifndef MENSSGES_H_
#define MESSAGES_H_

//MENSAJES UTILES UTILIZADOS PARA LOGS y PRINTS
#define ERROR_RECEPCION_PCB "Error al recibir el PCB desde el CPU"
#define ERROR_AL_ABRIR_ARCHIVO "Error al abrir el Archivo"
#define TERMINANDO_EL_LOG "Terminando el Log"
#define CONFIGURACION_CERRADA "Archivo de configuracion cerrado"
#define SEMAFOROS_DESTRUIDOS "Los semaforos fueron destruidos"
#define CONEXION_CERRADA "Las conexiones fueron cerradas"
#define OPERACION_DESCONOCIDA "Operacion desconocida. No quieras meter la pata"
#define LECTURA_DE_VALORES "Me llegaron los siguientes valores:\n"
#define RECEPCION_PAQUETE_PCB "Se recibio un paquete de PCB"
#define RECEPCION_PAQUETE_CPU "Se recibio un paquete de CPU"
#define RECEPCION_PROCESO "Se recibio un nuevo proceso"
#define GENERANDO_PCB "Generando paquete PCB ..."
#define INICIALIZACION_PROCESOS "Se inicializa el proceso %d"
#define PCB_ENVIADO "Se envio un PCB - PID: %d"
#define PCB_DESERIALIZADO "Se deserializo el PCB- PID: %d"

#define PROCESS_MOVE_EXEC "El PID: %d se pasó a EXECUTION"
#define PROCESS_MOVE_READY "El PID: %d se pasó a READY"
#define PROCESS_MOVE_EXIT "El PID: %d se pasó a EXIT"
#define PROCESS_MOVE_NEW "El PID: %d se pasó a NEW"
#define PROCESS_MOVE_BLOCKED "El PID: %d se pasó a Blocked"
#define PROCESS_MOVE_SUSBLK"El PID: %d se pasó a Suspended Blocked"

#define TERMINO_EL_PROCESO "El Proceso Termino Exitosamente"


#define INTERRUPCION_ENVIADA "Se envio una interrupcion a CPU"
#define INTERRUPCION_RECIBIDA "Se recibio una interrupcion de Kernel"

// MENSAJES UTILIZADOS EN  CPU

#define COMIENZO_CICLO_INSTRUCCION "Comienza ciclo de instruccion - PID: %d"
#define COMIENZO_ETAPA_FETCH "Comienza etapa FETCH - PID: %d"
#define COMIENZO_ETAPA_DECODE "Comienza etapa DECODE - PID: %d"
#define COMIENZO_ETAPA_FETCH_OPERANDS "Comienza etapa FETCH_OPERANDS - PID: %d"
#define COMIENZO_ETAPA_EXECUTE "Comienza etapa EXECUTE - PID: %d"
#define COMIENZO_ETAPA_CHECK_INTERRUPT "Comienza etapa CHECK_INTERRUPT - PID: %d"

// SOCKETS:

#define SERVIDOR_LISTO "Servidor listo para recibir al cliente"
#define SERVIDOR_AUSENTE "El Servidor no se encuentra inicializado. Error al conectar con"
#define SERVIDOR_DESCONEXION "el cliente se desconecto. Terminando servidor"
#define RECEPCION_PAQUETE_CONSOLA "Recibi un paquete de consola"
#define RECEPCION_PAQUETE_PCB "Recibi un paquete con un PCB"


#endif
