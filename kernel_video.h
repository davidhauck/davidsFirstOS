#ifndef _KVIDEO_H_
#define _KVIDEO_H_

#define _KVIDEO_HORIZONTAL_CHARACTERS	80
#define _KVIDEO_VERTICAL_LINES	 25
#define _KVIDEO_RAM_SIZE	 2000

void kvideo_clrscr();
void kvideo_handle_non_printable_char(char c);
void kvideo_write_char(char c);
void kvideo_write_str(char* str);


#endif
