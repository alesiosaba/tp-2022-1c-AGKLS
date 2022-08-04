clear

echo
echo Actualizando solo IP_ESCUCHA en todos los archivos de configuracion de cpu ...
echo

read -p "Ingrese IP_ESCUCHA: " IP_ESCUCHA
grep -RiIl 'IP_ESCUCHA' | xargs sed -i 's|\(IP_ESCUCHA\s*=\).*|\1'$IP_ESCUCHA'|'
