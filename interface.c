#include "common.c"

unsigned int cursorY = 0;
unsigned int cursorX = 0;


void scroll();
static void moveCursor();
void putChar(char);
void println(char* output);


void println(char * output)
{
	unsigned int i;
	for (i = 0; i <= sizeof(output); i++)
	{
		putChar(output[i]);
	}
	putChar('\n');
}

void putChar(char c)
{
	volatile unsigned char *videoram = (unsigned char *)0xB8000;
	if (c == 0x08 && cursorX)
	{
		cursorX--;
	}
	else if (c == '\r')
	{
		cursorX = 0;
	}
	else if (c == '\n')
	{
		cursorX = 0;
		cursorY++;
	}
	else if (c >= ' ')
	{
		videoram[cursorY * 80 * 2 + cursorX * 2] = c;
		cursorX++;
	}
	if (cursorX >= 80)
	{
		cursorX = 0;
		cursorY++;
	}

	scroll();
	moveCursor();
}

void scroll(volatile unsigned char * videoram)
{
	if (cursorY >=24)
	{
		unsigned char blank = 0x20;
		int i;
		for (i = 0; i < 2*24*80; i++)
		{
			videoram[i] = videoram[i+ 2 * 80];
		}
		for (i = 2*24*80; i < 2*25*80; i+=2)
		{
			videoram[i] = blank;
		}

		cursorY = 23;
	}
}

static void moveCursor()
{
	unsigned short cursorLocation = cursorY * 80 + cursorX;
	outb(0x3D4, 14);
	outb(0x3D5, cursorLocation >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, cursorLocation);
}
