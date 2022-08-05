clear

echo
echo Actualizando solo IP_MEMORIA en todos los archivos de configuracion de cpu ...
echo

read -p "Ingrese IP_MEMORIA: " IP_MEMORIA
grep -RiIl 'IP_MEMORIA' | xargs sed -i 's|\(IP_MEMORIA\s*=\).*|\1'$IP_MEMORIA'|'
