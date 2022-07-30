#!/bin/bash

clear

echo
echo Actualizando consola.config ...
echo

read -p "Ingrese nueva IP Kernel: " IP_KERNEL
read -p "Ingrese el log level: " LOG_LEVEL

grep -RiIl 'IP_KERNEL' | xargs sed -i 's|\(IP_KERNEL\s*=\).*|\1'$IP_KERNEL'|'
grep -RiIl 'LOG_LEVEL' | xargs sed -i 's|\(LOG_LEVEL\s*=\).*|\1'$LOG_LEVEL'|'