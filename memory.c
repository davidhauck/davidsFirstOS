unsigned int getCR2(void)
{
	unsigned int _cr2;
	
	asm("mov %%cr2, %0":"r" (_cr2):);
	return _cr2;
}

unsigned int getCR3(unsigned int pagedir)
{
	asm("mov %0, %%cr3": :"r" (pagedir));
}

unsigned int getCR3(void)
{
	unsigned int _cr3;

	asm ("mov %%cr3, %0":"=r" (_cr3));
	return _cr3;
}
