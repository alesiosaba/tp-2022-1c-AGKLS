echo
echo Actualizando memoria.config ...
echo

read -p "Ingrese LOG_LEVEL: " LOG_LEVEL
grep -RiIl 'LOG_LEVEL' | xargs sed -i 's|\(LOG_LEVEL\s*=\).*|\1'$LOG_LEVEL'|'

read -p "Ingrese IP_MEMORIA: " IP_MEMORIA
grep -RiIl 'IP_MEMORIA' | xargs sed -i 's|\(IP_MEMORIA\s*=\).*|\1'$IP_MEMORIA'|'

read -p "IP_CPU: " IP_CPU
grep -RiIl 'IP_CPU' | xargs sed -i 's|\(IP_CPU\s*=\).*|\1'$IP_CPU'|'

read -p "Ingrese IP_ESCUCHA: " IP_ESCUCHA
grep -RiIl 'IP_ESCUCHA' | xargs sed -i 's|\(IP_ESCUCHA\s*=\).*|\1'$IP_ESCUCHA'|'