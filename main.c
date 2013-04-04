// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "monitor.c"
#include "descriptor_tables.c"
#include "timer.c"
#include "kb.c"
#include "paging.c"

int main(void)
{
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();
    // Write out a sample string
    monitor_write("Hello, world!\n");

//these will force interrupts to occur
//    asm volatile("int $0x3");
//    asm volatile("int $0x4");

    init_timer(50);
    init_keyboard();
	initialize_paging();
    asm volatile("sti"); //enable interrutps

//these force a page fault to occur
//	u32int *ptr = (u32int*)0xA0000000;
//	u32int do_page_fault = *ptr;

//so the compiler doesnt throw an error
//	do_page_fault = 0;
//	do_page_fault++;

	//wait for something to happen
 	while(1)
		asm("hlt");
	return 0;
}
