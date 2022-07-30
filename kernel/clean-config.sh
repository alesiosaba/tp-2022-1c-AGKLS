#!/bin/bash

clear

echo
echo Limpiando cpu.config ...
echo

grep -RiIl '127.0.0.1' | xargs sed -i 's|\(IP_ESCUCHA\s*=\).*|\1'$IP_ESCUCHA'|'
grep -RiIl 'debug' | xargs sed -i 's|\(LOG_LEVEL\s*=\).*|\1'$LOG_LEVEL'|'