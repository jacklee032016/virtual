# /bin/bash
# mk.sh

ARM_BUILD=arm-none-eabi
#ARM_BUILD=arm-histbv310-linux

echo "build start object"
$ARM_BUILD-as -mcpu=arm926ej-s startup.s -o startup.o
$ARM_BUILD-gcc -mcpu=arm926ej-s -c init.c -o init.o

echo "link.."
$ARM_BUILD-ld -T linker.ld startup.o init.o -o init.elf

echo "output binary code..."
$ARM_BUILD-objcopy -O binary init.elf init.bin


