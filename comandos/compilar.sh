clear
echo Eliminando logs...
cd ../consola
rm consola.log
cd ../CPU
rm CPU.log
cd ../kernel
rm kernel.log
cd ../memoria
rm memoria.log

echo
echo Compilando modulos...

echo
echo Compilando shared...
echo
cd ../shared
make clean all

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
cd ../CPU
make clean all

echo
echo Compilando Memoria...
echo
cd ../memoria
make clean all