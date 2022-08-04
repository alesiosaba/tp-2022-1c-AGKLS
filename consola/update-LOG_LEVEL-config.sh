clear

echo
echo Actualizando solo LOG_LEVEL en todos los archivos de configuracion de consola...
echo

read -p "Ingrese LOG_LEVEL: " LOG_LEVEL
grep -RiIl 'LOG_LEVEL' | xargs sed -i 's|\(LOG_LEVEL\s*=\).*|\1'$LOG_LEVEL'|'
