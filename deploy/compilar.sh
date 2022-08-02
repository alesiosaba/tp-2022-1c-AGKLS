clear
echo Eliminando logs...
cd ../consola/exec
rm consola.log -f
cd ../../cpu/exec
rm CPU.log -f
cd ../../kernel/exec
rm kernel.log -f
cd ../../memoria/exec
rm memoria.log -f
mkdir /home/utnso/valgrind-logs

echo
echo Compilando shared...
echo
cd ../../shared
make clean all

echo
echo Compilando modulos...

echo
echo Compilando consola...
echo
cd ../consola
make clean all

echo
echo Compilando kernel...
echo
cd ../kernel
make clean all

echo
echo Compilando CPU...
echo
cd ../cpu
make clean all

echo
echo Compilando Memoria...
echo
cd ../memoria
make clean all