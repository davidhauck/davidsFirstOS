all:
	nasm -f elf -o loader.o loader.s
	nasm -f elf -o gdt.o gdt.s
	nasm -f elf -o interrupt.o interrupt.s
	gcc -o kernel.o -c main.c -Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs -ffreestanding	
	ld -T linker.ld -o kernel.bin loader.o kernel.o gdt.o interrupt.o
