clear

echo
echo Actualizando memoria.config ...
echo

read -p "Ingrese nueva IP_ESCUCHA: " IP_ESCUCHA

grep -RiIl 'IP_ESCUCHA' | xargs sed -i 's|\(IP_ESCUCHA\s*=\).*|\1'$IP_ESCUCHA'|'