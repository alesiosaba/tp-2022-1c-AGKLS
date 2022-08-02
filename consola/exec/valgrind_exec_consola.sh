clear
cd ..
valgrind --leak-check=full --log-file=/home/utnso/valgrind-logs/consola.log -v ./bin/consola.out ./test/script 4096