all:
	nasm -f elf -o loader.o loader.s
	gcc -o kernel.o -c kernel.c -Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs -ffreestanding
	gcc -o interrupts.o -c interrupt_handler.c -Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs -ffreestanding
	ld -T linker.ld -o kernel.bin loader.o kernel.o interrupts.o
