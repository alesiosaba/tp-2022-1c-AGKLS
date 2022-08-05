clear

echo
echo Actualizando solo IP_KERNEL en todos los archivos de configuracion de consola...
echo

read -p "Ingrese IP_KERNEL: " IP_KERNEL
grep -RiIl 'IP_KERNEL' | xargs sed -i 's|\(IP_KERNEL\s*=\).*|\1'$IP_KERNEL'|'

