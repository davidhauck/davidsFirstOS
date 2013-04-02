#include "common.c"
#include "interface.c"
#include <stddef.h>
struct gdtEntryStruct
{
	unsigned short limitLow;
	unsigned short baseLow;
	unsigned char baseMiddle;
	unsigned char access;
	unsigned char granularity;
	unsigned char baseHigh;
} __attribute__((packed));

typedef struct gdtEntryStruct gdtEntryT;

struct gdtPtrStruct
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));
typedef struct gdtPtrStruct  gdtPtrT;

struct idtEntryStruct
{
	unsigned short baseLo;
	unsigned short sel;
	unsigned char always0;
	unsigned char flags;
	unsigned short baseHi;
}__attribute__((packed));
typedef struct idtEntryStruct idtEntryT;

struct idtPtrStruct
{
	unsigned short limit;
	unsigned int base;
}__attribute__((packed));
typedef struct idtPtrStruct idtPtrT;

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void initDescriptorTables();

extern void gdt_flush(unsigned int);
extern void idt_flush(unsigned int);

static void initGdt();
static void initIdt();
static void gdtSetGate(int, unsigned int, unsigned int, unsigned char, unsigned char);
static void idtSetGate(unsigned char, unsigned int, unsigned short, unsigned char);

gdtEntryT gdtEntries[5];
gdtPtrT gdtPtr;
idtEntryT idtEntries[256];
idtPtrT idtPtr;

void * memset (void *str, int c, size_t len)
{
  register char *st = str;

  while (len-- > 0)
    *st++ = c;
  return str;
}

void initDescriptorTables()
{
	initGdt();
	initIdt();
}

static void initGdt()
{
	gdtPtr.limit = (sizeof(gdtEntryT) * 5) - 1;
	gdtPtr.base = (unsigned int)&gdtEntries;

	gdtSetGate(0, 0, 0, 0, 0);
	gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	gdt_flush((unsigned int)&gdtPtr);
}

static void initIdt()
{
	idtPtr.limit = sizeof(idtEntryT) * 256 - 1;
	idtPtr.base = (unsigned int)&idtEntries;

	memset(&idtEntries, 0, sizeof(idtEntryT)*256);
	
	idtSetGate(0, (unsigned int)isr0, 0x08, 0x8E);
	idtSetGate(1, (unsigned int)isr1, 0x08, 0x8E);
	idtSetGate(2, (unsigned int)isr2, 0x08, 0x8E);
	idtSetGate(3, (unsigned int)isr3, 0x08, 0x8E);
	idtSetGate(4, (unsigned int)isr4, 0x08, 0x8E);
	idtSetGate(5, (unsigned int)isr5, 0x08, 0x8E);
	idtSetGate(6, (unsigned int)isr6, 0x08, 0x8E);
	idtSetGate(7, (unsigned int)isr7, 0x08, 0x8E);
	idtSetGate(8, (unsigned int)isr8, 0x08, 0x8E);
	idtSetGate(9, (unsigned int)isr9, 0x08, 0x8E);
	idtSetGate(10, (unsigned int)isr10, 0x08, 0x8E);
	idtSetGate(11, (unsigned int)isr11, 0x08, 0x8E);
	idtSetGate(12, (unsigned int)isr12, 0x08, 0x8E);
	idtSetGate(13, (unsigned int)isr13, 0x08, 0x8E);
	idtSetGate(14, (unsigned int)isr14, 0x08, 0x8E);
	idtSetGate(15, (unsigned int)isr15, 0x08, 0x8E);
	idtSetGate(16, (unsigned int)isr16, 0x08, 0x8E);
	idtSetGate(17, (unsigned int)isr17, 0x08, 0x8E);
	idtSetGate(18, (unsigned int)isr18, 0x08, 0x8E);
	idtSetGate(19, (unsigned int)isr19, 0x08, 0x8E);
	idtSetGate(20, (unsigned int)isr20, 0x08, 0x8E);
	idtSetGate(21, (unsigned int)isr21, 0x08, 0x8E);
	idtSetGate(22, (unsigned int)isr22, 0x08, 0x8E);
	idtSetGate(23, (unsigned int)isr23, 0x08, 0x8E);
	idtSetGate(24, (unsigned int)isr24, 0x08, 0x8E);
	idtSetGate(25, (unsigned int)isr25, 0x08, 0x8E);
	idtSetGate(26, (unsigned int)isr26, 0x08, 0x8E);
	idtSetGate(27, (unsigned int)isr27, 0x08, 0x8E);
	idtSetGate(28, (unsigned int)isr28, 0x08, 0x8E);
	idtSetGate(29, (unsigned int)isr29, 0x08, 0x8E);
	idtSetGate(30, (unsigned int)isr30, 0x08, 0x8E);
	idtSetGate(31, (unsigned int)isr31, 0x08, 0x8E);

	idt_flush((unsigned int)&idtPtr);
}

static void gdtSetGate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran)
{
	gdtEntries[num].baseLow		= (base & 0xFFFF);
	gdtEntries[num].baseMiddle	= (base >> 16) & 0xFF;
	gdtEntries[num].baseHigh	= (base >> 24) & 0xFF;

	gdtEntries[num].limitLow	= (limit & 0xFFFF);
	gdtEntries[num].granularity	= (limit >> 16) & 0x0F;
	
	gdtEntries[num].granularity 	|= gran & 0xF0;
	gdtEntries[num].access		= access;
}

static void idtSetGate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags)
{
	idtEntries[num].baseLo = base & 0xFFFF;
	idtEntries[num].baseHi = (base >> 16) & 0xFFFF;
	
	idtEntries[num].sel 	= sel;
	idtEntries[num].always0	= 0;

	//TODO: when we start using user-mode, we need to uncomment this code, as it sets the interrupt gate's privilege level to 3
	idtEntries[num].flags	= flags /* | 0x60 */;
}
