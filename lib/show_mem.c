/*
 * Generic show_mem() implementation
 *
 * Copyright (C) 2008 Johannes Weiner <hannes@saeurebad.de>
 * All code subject to the GPL version 2.
 */

#include <linux/mm.h>
<<<<<<< HEAD
#include <linux/nmi.h>
#include <linux/quicklist.h>
=======
#include <linux/quicklist.h>
#include <linux/cma.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void show_mem(unsigned int filter)
{
	pg_data_t *pgdat;
<<<<<<< HEAD
	unsigned long total = 0, reserved = 0, shared = 0,
		nonshared = 0, highmem = 0;
=======
	unsigned long total = 0, reserved = 0, highmem = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	printk("Mem-Info:\n");
	show_free_areas(filter);

<<<<<<< HEAD
	if (filter & SHOW_MEM_FILTER_PAGE_COUNT)
		return;

	for_each_online_pgdat(pgdat) {
		unsigned long i, flags;

		pgdat_resize_lock(pgdat, &flags);
		for (i = 0; i < pgdat->node_spanned_pages; i++) {
			struct page *page;
			unsigned long pfn = pgdat->node_start_pfn + i;

			if (unlikely(!(i % MAX_ORDER_NR_PAGES)))
				touch_nmi_watchdog();

			if (!pfn_valid(pfn))
				continue;

			page = pfn_to_page(pfn);

			if (PageHighMem(page))
				highmem++;

			if (PageReserved(page))
				reserved++;
			else if (page_count(page) == 1)
				nonshared++;
			else if (page_count(page) > 1)
				shared += page_count(page) - 1;

			total++;
=======
	for_each_online_pgdat(pgdat) {
		unsigned long flags;
		int zoneid;

		pgdat_resize_lock(pgdat, &flags);
		for (zoneid = 0; zoneid < MAX_NR_ZONES; zoneid++) {
			struct zone *zone = &pgdat->node_zones[zoneid];
			if (!populated_zone(zone))
				continue;

			total += zone->present_pages;
			reserved += zone->present_pages - zone->managed_pages;

			if (is_highmem_idx(zoneid))
				highmem += zone->present_pages;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		pgdat_resize_unlock(pgdat, &flags);
	}

	printk("%lu pages RAM\n", total);
<<<<<<< HEAD
#ifdef CONFIG_HIGHMEM
	printk("%lu pages HighMem\n", highmem);
#endif
	printk("%lu pages reserved\n", reserved);
	printk("%lu pages shared\n", shared);
	printk("%lu pages non-shared\n", nonshared);
=======
	printk("%lu pages HighMem/MovableOnly\n", highmem);
	printk("%lu pages reserved\n", reserved);
#ifdef CONFIG_CMA
	printk("%lu pages cma reserved\n", totalcma_pages);
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_QUICKLIST
	printk("%lu pages in pagetable cache\n",
		quicklist_total_size());
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_MEMORY_FAILURE
	printk("%lu pages hwpoisoned\n", atomic_long_read(&num_poisoned_pages));
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
