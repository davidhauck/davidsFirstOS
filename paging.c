#ifndef PAGING_C
#define PAGING_C

#include "common.c"
#include "isr.c"
#include "kheap.c"

typedef struct page
{
	u32int present	: 1;
	u32int rw	: 1;
	u32int user	: 1;
	u32int accessed	: 1;
	u32int dirty	: 1;
	u32int unused	: 7;
	u32int frame	: 20;
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	//array of pointers to pagetables
	page_table_t *tables[1024];

	//holds physical location of the pointers to the pagetable
	u32int tablesPhysical[1024];

	//physical address of tablesPhysical, because the directory will be in a different location in virtual mem
	u32int physicalAddr;
} page_directory_t;

u32int *frames;
u32int nframes;

// The kernel's page directory
page_directory_t *kernel_directory=0;

// The current page directory;
page_directory_t *current_directory=0;

extern u32int placement_address;

#define OFFSET_FROM_BIT(a) (a%32)
#define INDEX_FROM_BIT(a) (a/32)

void page_fault(registers_t regs)
{
	//faulting address is stored in CR2
	u32int faulting_address;
	asm volatile ("mov %%cr2, %0" : "=r" (faulting_address));

   int present   = !(regs.err_code & 0x1); // Page not present
	int rw = regs.err_code & 0x02;        //probably tried to write to a read-only page
	int us = regs.err_code & 0x04;	      //processor was in user-mode tried to access kernel page?
	int reserved = regs.err_code & 0x8;   //page entry reserved bits may have been overwritten
	//int id = regs.err_code & 0x10;        //was caused by an instruction fetch

	monitor_write("Page fault! ( ");
	if (present) {monitor_write("present ");}
	if (rw) {monitor_write("read-only ");}
	if (us) {monitor_write("user-mode ");}
	if (reserved) {monitor_write("reserved ");}
	
//	monitor_write(") at 0x");
	monitor_write_hex(faulting_address);
//	monitor_write("\n");
//	monitor_write("PANIC(Page fault)");
}

static void set_frame(u32int frame_addr)
{
	u32int frame = frame_addr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

static void clear_frame(u32int frame_addr)
{
	u32int frame = frame_addr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

//static u32int test_frame(u32int frame_addr)
//{
//	u32int frame = frame_addr / 0x1000;
//	u32int idx = INDEX_FROM_BIT(frame);
//	u32int off = OFFSET_FROM_BIT(frame);
//	return (frames[idx] & (0x1 << off));
//}

static u32int first_frame()
{
	u32int i, j;
	for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
	{
		if (frames[i] != 0xFFFFFFFF)
		{
			for (j = 0; j < 32; j++)
			{
				u32int toTest = 0x1 << j;
				if ( ! (frames[i]&toTest))
				{
					return i * 4 * 8 + j;
				}
			}
		}
	}
	return (u32int) -1;
}

void alloc_frame (page_t *page, int is_kernel, int is_writable)
{
	if (page->frame != 0)
	{
		return; //Frame was already alocated
	}
	else
	{
		u32int idx = first_frame();
		if (idx == (u32int) -1)
		{
			monitor_write("No free frames!");//TODO: PANIC!!
		}
		set_frame(idx * 0x1000);
		page->present = 1; //the page is present
		page->rw = (is_writable) ? 1 : 0;
		page->user = (is_kernel) ? 0 : 1;
		page->frame = idx;
	}
}

void free_frame(page_t *page)
{
	u32int frame;
	if (!(frame=page->frame))
	{
		return; //the page wasnt allocated
	}
	else
	{
		clear_frame(frame);
		page->frame = 0x0;
	}
}

void switch_page_directory(page_directory_t *dir)
{
	current_directory = dir;
	asm volatile ("mov %0, %%cr3" :: "r"(&dir->tablesPhysical));
	u32int cr0;
	asm volatile ("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000; //init paging
	asm volatile ("mov %0, %%cr0" :: "r"(cr0));
}

page_t *get_page(u32int address, int make, page_directory_t *dir)
{
	address /= 0x1000;

	u32int table_idx = address / 1024;
	if (dir->tables[table_idx])
	{
		return &dir->tables[table_idx]->pages[address%1024];
	}
	else if (make)
	{
		u32int tmp;
		dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_idx], 0, 0x1000);
		dir->tablesPhysical[table_idx] = tmp | 0x7; //Present, rw, us
		return &dir->tables[table_idx]->pages[address%1024];
	}
	else
	{
		return 0;
	}
}

void initialize_paging()
{
	u32int mem_end_page = 0x1000000; //for now assumer 16MB big memory
	nframes = mem_end_page / 0x1000;
	frames = (u32int*) kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));
	
	//create a page directory
	kernel_directory = (page_directory_t*) kmalloc_a(sizeof(page_directory_t));
	memset(kernel_directory, 0, sizeof(page_directory_t));
	
	current_directory = kernel_directory;

	unsigned int i = 0;
	while (i < placement_address)
	{
		alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}

	//register page fault handler
	register_interrupt_handler(14, page_fault);

	//enable paging
	switch_page_directory(kernel_directory);
}

#endif
