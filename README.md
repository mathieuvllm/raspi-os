# (Cross-)Compilation
Cross-compiler from x86 Linux to ARM : `arm-none-eabi-gcc`
## Compiling the kernel
`arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -c boot.S -o boot.o`
This compiles boot.S
`arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -c kernel.c -o kernel.o -O2 -Wall -Wextra`
This compiles kernel.c
`arm-none-eabi-gcc -T linker.ld -o myos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o`
This links everything
# QEMU
dnf package `qemu-system-arm`
## Running the kernel
`qemu-system-arm -m 1024 -M raspi2b -serial stdio -kernel myos.elf`
Emulates the Rpi 2 mobel B revision 1.1 with 1Gb of RAM, simulating serial connection for I/O, and running my kernel
