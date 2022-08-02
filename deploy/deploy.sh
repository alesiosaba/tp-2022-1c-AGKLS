echo
echo Clonando tp-2022-1c-AGKLS...
echo
git clone https://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@github.com/sisoputnfrba/tp-2022-1c-AGKLS.git

cd tp-2022-1c-AGKLS
echo
echo Clonando commons...
echo
git clone https://github.com/sisoputnfrba/so-commons-library.git
cd so-commons-library
echo
echo Instalando commons...
echo
sudo make install

echo
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
cd ../cpu
make clean all

echo
echo Compilando Memoria...
echo
cd ../memoria
make clean all

mkdir /home/utnso/valgrind-logs

echo
echo deploy finalizado con exito
echo