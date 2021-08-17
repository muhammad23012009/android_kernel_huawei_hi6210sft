/*
 *  linux/arch/cris/mm/init.c
 *
 *  Copyright (C) 1995  Linus Torvalds
 *  Copyright (C) 2000,2001  Axis Communications AB
 *
 *  Authors:  Bjorn Wesen (bjornw@axis.com)
 *
 */

#include <linux/gfp.h>
#include <linux/init.h>
#include <linux/bootmem.h>
<<<<<<< HEAD
=======
#include <linux/proc_fs.h>
#include <linux/kcore.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/tlb.h>
#include <asm/sections.h>

unsigned long empty_zero_page;
<<<<<<< HEAD

void __init
mem_init(void)
{
	int codesize, reservedpages, datasize, initsize;
	unsigned long tmp;

=======
EXPORT_SYMBOL(empty_zero_page);

void __init mem_init(void)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	BUG_ON(!mem_map);

	/* max/min_low_pfn was set by setup.c
	 * now we just copy it to some other necessary places...
	 *
	 * high_memory was also set in setup.c
	 */
<<<<<<< HEAD

	max_mapnr = num_physpages = max_low_pfn - min_low_pfn;
 
	/* this will put all memory onto the freelists */
        totalram_pages = free_all_bootmem();

	reservedpages = 0;
	for (tmp = 0; tmp < max_mapnr; tmp++) {
		/*
                 * Only count reserved RAM pages
                 */
		if (PageReserved(mem_map + tmp))
			reservedpages++;
	}

	codesize =  (unsigned long) &_etext - (unsigned long) &_stext;
        datasize =  (unsigned long) &_edata - (unsigned long) &_etext;
        initsize =  (unsigned long) &__init_end - (unsigned long) &__init_begin;
	
        printk(KERN_INFO
               "Memory: %luk/%luk available (%dk kernel code, %dk reserved, %dk data, "
	       "%dk init)\n" ,
	       nr_free_pages() << (PAGE_SHIFT-10),
	       max_mapnr << (PAGE_SHIFT-10),
	       codesize >> 10,
	       reservedpages << (PAGE_SHIFT-10),
	       datasize >> 10,
	       initsize >> 10
               );
}

/* free the pages occupied by initialization code */

void 
free_initmem(void)
{
	free_initmem_default(0);
}
=======
	max_mapnr = max_low_pfn - min_low_pfn;
        free_all_bootmem();
	mem_init_print_info(NULL);
}

/* Free a range of init pages. Virtual addresses. */

void free_init_pages(const char *what, unsigned long begin, unsigned long end)
{
	unsigned long addr;

	for (addr = begin; addr < end; addr += PAGE_SIZE) {
		ClearPageReserved(virt_to_page(addr));
		init_page_count(virt_to_page(addr));
		free_page(addr);
		totalram_pages++;
	}

	printk(KERN_INFO "Freeing %s: %ldk freed\n", what, (end - begin) >> 10);
}

/* Free the pages occupied by initialization code. */

void free_initmem(void)
{
	free_initmem_default(-1);
}

/* Free the pages occupied by initrd code. */

#ifdef CONFIG_BLK_DEV_INITRD
void free_initrd_mem(unsigned long start, unsigned long end)
{
	free_init_pages("initrd memory",
	                start,
	                end);
}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
