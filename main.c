// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "monitor.c"
#include "descriptor_tables.c"
#include "timer.c"
#include "kb.c"

int main(void)
{
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();
    // Write out a sample string
    monitor_write("Hello, world!\n");

    asm volatile("int $0x3");
    asm volatile("int $0x4");

    init_timer(50);
    init_keyboard();
    asm volatile("sti");

	 while(1)
		asm("hlt");

    return 0;
}
