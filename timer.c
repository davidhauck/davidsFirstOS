#include "isr.c"
#include "monitor.c"
#include "common.c"

#ifndef TIMER_H
#define TIMER_H

void init_timer(u32int frequency);


u32int tick = 0;

static void timer_callback(registers_t regs)
{
	 regs = regs;
    tick++;
	 if (tick % 200 == 0)
	 {
    	 monitor_write("Tick: ");
	 	 monitor_write_dec(tick / 200);
	 	 monitor_put('\n');
	 }
}

void init_timer(u32int frequency)
{
    register_interrupt_handler(IRQ0, &timer_callback);

    u32int divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise
    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)(divisor>>8);

    outb(0x40, l);
    outb(0x40, h);
}

#endif
