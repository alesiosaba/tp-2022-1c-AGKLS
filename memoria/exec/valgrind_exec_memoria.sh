clear
echo
echo Ejecutando memoria con valgrind
echo
valgrind --leak-check=full --log-file=/home/utnso/valgrind-logs/memoria.log -v ./../bin/memoria.out ./../exec_conf/memoria.config