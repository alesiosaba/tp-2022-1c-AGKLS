
echo Limpiando cpu.config ...

cd ../../cpu/exec_conf/
rm cpu.config -f
touch cpu.config

echo "LOG_LEVEL=DEBUG" >> cpu.config
echo "ENTRADAS_TLB=4" >> cpu.config
echo "REEMPLAZO_TLB=LRU" >> cpu.config
echo "RETARDO_NOOP=1000" >> cpu.config
echo "IP_MEMORIA=127.0.0.1" >> cpu.config
echo "PUERTO_MEMORIA=8007" >> cpu.config
echo "PUERTO_ESCUCHA_DISPATCH=8001" >> cpu.config
echo "PUERTO_ESCUCHA_INTERRUPT=8005" >> cpu.config
echo "IP_ESCUCHA=127.0.0.1" >> cpu.config