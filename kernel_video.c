#include "kernel_video.h"

unsigned short* _kvideo_ram = (unsigned short*)0xB8000;	// where the video ram starts
unsigned int _kvideo_block_offset = 0;
unsigned int _kvideo_block = 0;

void kvideo_clrscr()
{
	unsigned int i = 0;
	for (;i < _KVIDEO_RAM_SIZE;i++)
	{
		_kvideo_ram[i] = (unsigned char)' ' | 0x0700;
	}
	_kvideo_block_offset = 0;
	_kvideo_block = 0;
}

void kvideo_handle_non_printable_char(char c)
{
	if (c == '\n')
	{
		_kvideo_block += _KVIDEO_HORIZONTAL_CHARACTERS;
		_kvideo_block_offset = 0;
	}
	else if (c == '\r')
	{
		_kvideo_block_offset = 0;
	}
}


void kvideo_write_char(char c)
{
	if (c < 30)
	{
		kvideo_handle_non_printable_char(c);
		return;
	}
	if (_kvideo_block_offset >= _KVIDEO_HORIZONTAL_CHARACTERS)
	{
		_kvideo_block_offset = 0;
		_kvideo_block += _KVIDEO_HORIZONTAL_CHARACTERS;
	}

	if (_kvideo_block >= _KVIDEO_RAM_SIZE)
	{
		kvideo_clrscr();
	}
	_kvideo_ram[_kvideo_block + _kvideo_block_offset] = (unsigned char)c | 0x0700;
	_kvideo_block_offset++;
}

void kvideo_write_str(char* str)
{
	while (*str)
	{
		kvideo_write_char(*str);
		str++;
	}
}
