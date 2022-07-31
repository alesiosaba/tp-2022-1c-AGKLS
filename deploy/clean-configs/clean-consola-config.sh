echo
echo Limpiando consola.config ...
echo

cd ../../consola/
rm consola.config
touch consola.config

echo "LOG_LEVEL=debug" >> consola.config
echo "IP_KERNEL=127.0.0.1" >> consola.config
echo "PUERTO_KERNEL=8000" >> consola.config