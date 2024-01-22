#/bin/sh
echo "Название: Hello world"
echo "Лицензия: Публичное достояние"

CC="gcc"
ARCH_FLAGS="-ffreestanding -O0 -g -fPIC -static -nostdlib "

if [ -d "../../sdk" ]; then
	CC="../../sdk/bin/x86_64-elf-gcc"
fi


$CC $ARCH_FLAGS -I../../modlib -finput-charset=UTF-8 -fexec-charset=cp1251 -c main.c -o hello.o
$CC $ARCH_FLAGS -Wl,--entry=init,--build-id=none -T ../link.ld hello.o -L../../modlib/lib/ -lmod -o hello.ko

cp hello.ko ../bin/
echo "Сборка завершена, файл: hello.ko"
