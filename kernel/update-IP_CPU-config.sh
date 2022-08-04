clear

echo
echo Actualizando solo IP_CPU en todos los archivos de configuracion de kernel ...
echo

read -p "IP_CPU: " IP_CPU
grep -RiIl 'IP_CPU' | xargs sed -i 's|\(IP_CPU\s*=\).*|\1'$IP_CPU'|'
