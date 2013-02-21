all:
	nasm -f elf -o loader.o loader.s
	gcc -o kernel.o -c kernel.c -Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs
	ld -T linker.ld -o kernel.bin loader.o kernel.o
