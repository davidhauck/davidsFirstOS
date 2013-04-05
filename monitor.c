#include "common.c"

#ifndef MONITOR_C
#define MONITOR_C

// The VGA framebuffer starts at 0xB8000.
u16int *video_memory = (u16int *)0xB8000;

u8int cursor_x = 0;
u8int cursor_y = 0;

static void move_cursor()
{
    // The screen is 80 characters wide
    u16int cursorLocation = cursor_y * 80 + cursor_x;
    outb(0x3D4, 14);                  // Tell VGA board its setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell VGA board its setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll()
{


    u8int attributeByte = (0 << 4) | (15 & 0x0F);
    u16int blank = 0x20 | (attributeByte << 8);

    // Row 25 is the end, scroll up
    if(cursor_y >= 25)
    {
        int i;
        for (i = 0*80; i < 24*80; i++)
        {
            video_memory[i] = video_memory[i+80];
        }


        for (i = 24*80; i < 25*80; i++)
        {
            video_memory[i] = blank;
        }

        cursor_y = 24;
    }
}

void monitor_put(char c)
{
    u8int backColour = 0;
    u8int foreColour = 15;

    u8int  attributeByte = (backColour << 4) | (foreColour & 0x0F);

    u16int attribute = attributeByte << 8;
    u16int *location;

	//backspace
    if (c == 0x08 && cursor_x)
    {
        cursor_x--;
    }

    //tab
    else if (c == 0x09)
    {
        cursor_x = (cursor_x+8) & ~(8-1);
    }

    // carriage return
    else if (c == '\r')
    {
        cursor_x = 0;
    }

    // newline 
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    // any other character
    else if(c >= ' ')
    {
        location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y ++;
    }

    scroll();

    move_cursor();

}


void monitor_clear()
{
    u8int attributeByte = (0 << 4) | (15 & 0x0F);
    u16int blank = 0x20 | (attributeByte << 8);

    int i;
    for (i = 0; i < 80*25; i++)
    {
        video_memory[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void monitor_write(char *c)
{
    int i = 0;
    while (c[i])
    {
        monitor_put(c[i++]);
    }
}

void monitor_write_hex(u32int n)
{
    s32int tmp;

    monitor_write("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            monitor_put (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            monitor_put( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        monitor_put (tmp-0xA+'a');
    }
    else
    {
        monitor_put (tmp+'0');
    }

}

void monitor_write_dec(u32int n)
{

    if (n == 0)
    {
        monitor_put('0');
        return;
    }

    s32int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    monitor_write(c2);

}

#endif
