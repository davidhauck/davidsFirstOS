#include "common.c"
#include "interface.c"

#ifndef ISR_C
#define ISR_C

typedef struct registers
{
	unsigned int ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int intNo, errCode;
	unsigned int eip, cs, eflags, usresp, ss;
} registers_t;

void isr_handler(registers_t regs)
{
	println("recieved interrupt");
	putDec(regs.intNo);
	putChar('\n');
}

#endif
