#ifndef MENSSGES_H_
#define MESSAGES_H_

//MENSAJES UTILES UTILIZADOS PARA LOGS y PRINTS
#define ERROR_RECEPCION_PCB 		"Error al recibir el PCB desde el CPU"
#define ERROR_AL_ABRIR_ARCHIVO 		"Error al abrir el Archivo %d"
#define ERROR_NUEVO_PROCESO_MEMORIA "Error al crear la estructura de memoria para el proceso %d"

#define TERMINANDO_EL_LOG 		"Terminando el Log"
#define CONFIGURACION_CERRADA 	"Archivo de configuracion cerrado"
#define SEMAFOROS_DESTRUIDOS 	"Los semaforos fueron destruidos"
#define CONEXION_CERRADA 		"Las conexiones fueron cerradas"
#define OPERACION_DESCONOCIDA 	"Operacion desconocida. No quieras meter la pata"
#define LECTURA_DE_VALORES 		"Me llegaron los siguientes valores:\n"
#define RECEPCION_PAQUETE_PCB 	"Se recibio un paquete de PCB"
#define RECEPCION_PAQUETE_CPU 	"Se recibio un paquete de CPU"
#define RECEPCION_PROCESO 		"Se recibio un nuevo proceso"
#define GENERANDO_PCB 			"Generando paquete PCB ..."
#define INICIALIZACION_PROCESOS "Se inicializa el proceso %d"
#define PCB_ENVIADO 			"Se envio un PCB - PID: %d - op code: %d"
#define PCB_DESERIALIZADO		"Se deserializo el PCB- PID: %d"

#define TERMINO_EL_PROCESO "El Proceso Termino Exitosamente"

#define PROCESS_MOVE_EXEC 		"El PID: %d se pasó a EXECUTION"
#define PROCESS_MOVE_READY		"El PID: %d se pasó a READY"
#define PROCESS_MOVE_EXIT 		"El PID: %d se pasó a EXIT"
#define PROCESS_MOVE_NEW 		"El PID: %d se pasó a NEW"
#define PROCESS_MOVE_BLOCKED 	"El PID: %d se pasó a BLOCKED"
#define PROCESS_MOVE_SUSBLK 	"El PID: %d se pasó a SUSPENDED BLOCKED"
#define PROCESS_MOVE_SUSRDY 	"El PID: %d se pasó a SUSPENDED READY"


#define INTERRUPCION_ENVIADA    "Se envio una interrupcion a CPU"
#define INTERRUPCION_RECIBIDA   "Se recibio una interrupcion de Kernel"

#define INICIALIZACION_PROCESOS "-----ENCOLANDO EL PID %d-----"
#define FINALIZACION_PROCESO    "-----FINALIZO EL PID %d-----"
#define BLOQUEO_PROCESO         "-----SE BLOQUEO EL PID %d-----"
#define DESBLOQUEO_PROCESO      "-----SE DESBLOQUEO EL PID %d-----"
#define SUSPENCION_PROCESO      "-----SE SUSPENDIO EL PID %d-----"
#define DESUSPENCION_PROCESO    "-----SE TERMINO LA SUSPENCION DEL PID %d-----"
#define EJECUTANDO_PROCESO		"-----EJECUTANDO PID %d-----"
#define TERMINO_EL_PROCESO		"-----El Proceso Termino Exitosamente-----"

// MENSAJES UTILIZADOS EN  CPU

#define SERVIDOR_KERNEL_DISPATCH_LISTO 	"Servidor listo para recibir la conexion dispatch del kernel"
#define SERVIDOR_KERNEL_INTERRUPT_LISTO "Servidor listo para recibir la conexion interrupt del kernel"
#define INICIO_HANDSHAKE_CON_MEMORIA	"Realizando handshake con Memoria"
#define FINAL_HANDSHAKE_CON_MEMORIA		"Handshake con Memoria finalizado"
#define PROCESO_ALOJADO					"----- PROCESO ALOJADO           ----- PID: %d -----"
#define PROCESO_DESALOJADO				"----- PROCESO DESALOJADO        ----- PID: %d -----"
#define COMIENZO_CICLO_INSTRUCCION 		"-----   COMIENZA CICLO DE INSTRUCCION - PID: %d -----"
#define FIN_CICLO_INSTRUCCION 			"-----   FINALIZA CICLO DE INSTRUCCION - PID: %d -----"

#define EJECUCION_NO_OP					"-----      INSTRUCCION NO_OP - PID: %d -----"
#define EJECUCION_READ					"-----      INSTRUCCION READ  - PID: %d -----"
#define EJECUCION_WRITE					"-----      INSTRUCCION WRITE - PID: %d -----"
#define EJECUCION_COPY					"-----      INSTRUCCION COPY  - PID: %d -----"
#define EJECUCION_IO					"-----      INSTRUCCION I/O   - PID: %d -----"
#define EJECUCION_EXIT					"-----      INSTRUCCION EXIT  - PID: %d -----"

#define COMIENZO_ETAPA_FETCH 			"----- ETAPA FETCH 			 - PID: %d -----"
#define COMIENZO_ETAPA_DECODE 			"----- ETAPA DECODE 		 - PID: %d -----"
#define COMIENZO_ETAPA_FETCH_OPERANDS 	"----- ETAPA FETCH_OPERANDS  - PID: %d -----"
#define COMIENZO_ETAPA_EXECUTE 			"----- ETAPA EXECUTE 		 - PID: %d -----"
#define COMIENZO_ETAPA_CHECK_INTERRUPT 	"----- ETAPA CHECK_INTERRUPT - PID: %d -----"

#define PROCESO_ENVIADO_A_IO			"-----    PROCESO ENVIADO A I/O ----- PID: %d -----"
#define EJECUCION_PROCESO_FINALIZADA	"----- PROCESO EJECUTADO COMPLETAMENTE  ----- PID: %d -----"

// SOCKETS:

#define SERVIDOR_LISTO "Servidor listo para recibir al cliente"
#define SERVIDOR_AUSENTE "El Servidor no se encuentra inicializado. Error al conectar con"
#define SERVIDOR_DESCONEXION "el cliente se desconecto. Terminando servidor"
#define RECEPCION_PAQUETE_CONSOLA "Recibi un paquete de consola"
#define RECEPCION_PAQUETE_PCB "Recibi un paquete con un PCB"


#endif
