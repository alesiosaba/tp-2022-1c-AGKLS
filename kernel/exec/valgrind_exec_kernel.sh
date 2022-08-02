clear
echo
echo Ejecutando kernel con Valgrind
echo
valgrind --leak-check=full --log-file=/home/utnso/valgrind-logs/kernel.log -v ./bin/kernel.out ./exec_conf/kernel.config