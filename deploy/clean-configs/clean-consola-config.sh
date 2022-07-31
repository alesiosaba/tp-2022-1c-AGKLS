echo Limpiando consola.config ...

cd ../../consola/
rm consola.config
touch consola.config

echo "LOG_LEVEL=DEBUG" >> consola.config
echo "IP_KERNEL=127.0.0.1" >> consola.config
echo "PUERTO_KERNEL=8000" >> consola.config