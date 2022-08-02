clear
echo
echo Ejecutando cpu con Valgrind
echo
valgrind --leak-check=full --log-file=/home/utnso/valgrind-logs/cpu.log -v ./bin/cpu.out ./exec_conf/cpu.config