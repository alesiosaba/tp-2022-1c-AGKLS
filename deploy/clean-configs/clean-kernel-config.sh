echo Limpiando kernel.config ...

cd ../../kernel/exec_conf/
rm kernel.config -f
touch kernel.config

echo "LOG_LEVEL=DEBUG" >> kernel.config
echo "IP_MEMORIA=127.0.0.1" >> kernel.config
echo "PUERTO_MEMORIA=8002" >> kernel.config
echo "IP_CPU=127.0.0.1" >> kernel.config
echo "PUERTO_CPU_DISPATCH=8001" >> kernel.config
echo "PUERTO_CPU_INTERRUPT=8005" >> kernel.config
echo "PUERTO_ESCUCHA=8000" >> kernel.config
echo "ALGORITMO_PLANIFICACION=SRT" >> kernel.config
echo "ESTIMACION_INICIAL=10000" >> kernel.config
echo "ALFA=0.5" >> kernel.config
echo "GRADO_MULTIPROGRAMACION=4" >> kernel.config
echo "TIEMPO_MAXIMO_BLOQUEADO=15000" >> kernel.config
echo "IP_ESCUCHA=127.0.0.1" >> kernel.config