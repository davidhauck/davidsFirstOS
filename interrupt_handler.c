void interrupt_handler() 
{
	__asm__("pusha");
   volatile unsigned char *videoram = (unsigned char *)0xB8000;
	videoram[3] = 65;
	__asm__("popa; leave; iret"); /* BLACK MAGIC! */
}
