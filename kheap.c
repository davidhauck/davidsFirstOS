#ifndef KHEAP_C
#define KHEAP_C

// make type declarations nicer
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

extern u32int end;
u32int placement_address = (u32int)&end;

u32int kmalloc_int (u32int sz, int align, u32int *phys)
{	
	if (align == 1 && (placement_address & 0xFFFFF000))
	{
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}

	if (phys)
	{
		*phys = placement_address; //idk what this does, the internet told me to do it
	}

	u32int tmp = placement_address;
	placement_address += sz;
	return tmp;
}

u32int kmalloc_a(u32int sz)
{
	return kmalloc_int (sz, 1, 0);
}

u32int kmalloc_p(u32int sz, u32int *phys)
{
	return kmalloc_int (sz, 0, phys);
}

u32int kmalloc_ap(u32int sz, u32int *phys)
{
	return kmalloc_int (sz, 1, phys);
}

u32int kmalloc(u32int sz)
{
	return kmalloc_int(sz, 0, 0);
}

#endif
