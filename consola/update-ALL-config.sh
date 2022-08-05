clear

echo
echo Actualizando todos los archivos de configuracion de consola...
echo

read -p "Ingrese LOG_LEVEL: " LOG_LEVEL
grep -RiIl 'LOG_LEVEL' | xargs sed -i 's|\(LOG_LEVEL\s*=\).*|\1'$LOG_LEVEL'|'

read -p "Ingrese IP_KERNEL: " IP_KERNEL
grep -RiIl 'IP_KERNEL' | xargs sed -i 's|\(IP_KERNEL\s*=\).*|\1'$IP_KERNEL'|'