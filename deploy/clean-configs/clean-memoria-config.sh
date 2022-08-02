echo Limpiando memoria.config ...

cd ../../memoria/exec_conf/
rm memoria.config -f
touch memoria.config

echo "LOG_LEVEL=DEBUG" >> memoria.config
echo "PUERTO_ESCUCHA_KERNEL=8002" >> memoria.config
echo "PUERTO_ESCUCHA_CPU=8007" >> memoria.config
echo "TAM_MEMORIA=4096" >> memoria.config
echo "TAM_PAGINA=64" >> memoria.config
echo "ENTRADAS_POR_TABLA=4" >> memoria.config
echo "RETARDO_MEMORIA=1000" >> memoria.config
echo "ALGORITMO_REEMPLAZO=CLOCK-M" >> memoria.config
echo "MARCOS_POR_PROCESO=4" >> memoria.config
echo "RETARDO_SWAP=2000" >> memoria.config
echo "PATH_SWAP=/home/utnso/swap" >> memoria.config
echo "IP_ESCUCHA=127.0.0.1" >> memoria.config

echo