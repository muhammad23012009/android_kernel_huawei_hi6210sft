/*
 *  linux/arch/arm/mm/init.c
 *
 *  Copyright (C) 1995-2005 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/swap.h>
#include <linux/init.h>
#include <linux/bootmem.h>
#include <linux/mman.h>
#include <linux/export.h>
#include <linux/nodemask.h>
#include <linux/initrd.h>
#include <linux/of_fdt.h>
#include <linux/highmem.h>
#include <linux/gfp.h>
#include <linux/memblock.h>
#include <linux/dma-contiguous.h>
#include <linux/sizes.h>
<<<<<<< HEAD
=======
#include <linux/stop_machine.h>

#include <asm/cp15.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/mach-types.h>
#include <asm/memblock.h>
#include <asm/prom.h>
#include <asm/sections.h>
#include <asm/setup.h>
<<<<<<< HEAD
=======
#include <asm/system_info.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/tlb.h>
#include <asm/fixmap.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include "mm.h"

<<<<<<< HEAD
static unsigned long phys_initrd_start __initdata = 0;
=======
#ifdef CONFIG_CPU_CP15_MMU
unsigned long __init __clear_cr(unsigned long mask)
{
	cr_alignment = cr_alignment & ~mask;
	return cr_alignment;
}
#endif

static phys_addr_t phys_initrd_start __initdata = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static unsigned long phys_initrd_size __initdata = 0;

static int __init early_initrd(char *p)
{
<<<<<<< HEAD
	unsigned long start, size;
=======
	phys_addr_t start;
	unsigned long size;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	char *endp;

	start = memparse(p, &endp);
	if (*endp == ',') {
		size = memparse(endp + 1, NULL);

		phys_initrd_start = start;
		phys_initrd_size = size;
	}
	return 0;
}
early_param("initrd", early_initrd);

static int __init parse_tag_initrd(const struct tag *tag)
{
<<<<<<< HEAD
	printk(KERN_WARNING "ATAG_INITRD is deprecated; "
=======
	pr_warn("ATAG_INITRD is deprecated; "
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		"please update your bootloader.\n");
	phys_initrd_start = __virt_to_phys(tag->u.initrd.start);
	phys_initrd_size = tag->u.initrd.size;
	return 0;
}

__tagtable(ATAG_INITRD, parse_tag_initrd);

static int __init parse_tag_initrd2(const struct tag *tag)
{
	phys_initrd_start = tag->u.initrd.start;
	phys_initrd_size = tag->u.initrd.size;
	return 0;
}

__tagtable(ATAG_INITRD2, parse_tag_initrd2);

<<<<<<< HEAD
#ifdef CONFIG_OF_FLATTREE
void __init early_init_dt_setup_initrd_arch(u64 start, u64 end)
{
	phys_initrd_start = start;
	phys_initrd_size = end - start;
}
#endif /* CONFIG_OF_FLATTREE */

/*
 * This keeps memory configuration data used by a couple memory
 * initialization functions, as well as show_mem() for the skipping
 * of holes in the memory map.  It is populated by arm_add_memory().
 */
struct meminfo meminfo;

void show_mem(unsigned int filter)
{
	int free = 0, total = 0, reserved = 0;
	int shared = 0, cached = 0, slab = 0, i;
	struct meminfo * mi = &meminfo;

	printk("Mem-info:\n");
	show_free_areas(filter);

	if (filter & SHOW_MEM_FILTER_PAGE_COUNT)
		return;

	for_each_bank (i, mi) {
		struct membank *bank = &mi->bank[i];
		unsigned int pfn1, pfn2;
		struct page *page, *end;

		pfn1 = bank_pfn_start(bank);
		pfn2 = bank_pfn_end(bank);

		page = pfn_to_page(pfn1);
		end  = pfn_to_page(pfn2 - 1) + 1;

		do {
			total++;
			if (PageReserved(page))
				reserved++;
			else if (PageSwapCache(page))
				cached++;
			else if (PageSlab(page))
				slab++;
			else if (!page_count(page))
				free++;
			else
				shared += page_count(page) - 1;
			page++;
		} while (page < end);
	}

	printk("%d pages of RAM\n", total);
	printk("%d free pages\n", free);
	printk("%d reserved pages\n", reserved);
	printk("%d slab pages\n", slab);
	printk("%d pages shared\n", shared);
	printk("%d pages swap cached\n", cached);
}

static void __init find_limits(unsigned long *min, unsigned long *max_low,
			       unsigned long *max_high)
{
	struct meminfo *mi = &meminfo;
	int i;

	/* This assumes the meminfo array is properly sorted */
	*min = bank_pfn_start(&mi->bank[0]);
	for_each_bank (i, mi)
		if (mi->bank[i].highmem)
				break;
	*max_low = bank_pfn_end(&mi->bank[i - 1]);
	*max_high = bank_pfn_end(&mi->bank[mi->nr_banks - 1]);
}

static void __init arm_bootmem_init(unsigned long start_pfn,
	unsigned long end_pfn)
{
	struct memblock_region *reg;
	unsigned int boot_pages;
	phys_addr_t bitmap;
	pg_data_t *pgdat;

	/*
	 * Allocate the bootmem bitmap page.  This must be in a region
	 * of memory which has already been mapped.
	 */
	boot_pages = bootmem_bootmap_pages(end_pfn - start_pfn);
	bitmap = memblock_alloc_base(boot_pages << PAGE_SHIFT, L1_CACHE_BYTES,
				__pfn_to_phys(end_pfn));

	/*
	 * Initialise the bootmem allocator, handing the
	 * memory banks over to bootmem.
	 */
	node_set_online(0);
	pgdat = NODE_DATA(0);
	init_bootmem_node(pgdat, __phys_to_pfn(bitmap), start_pfn, end_pfn);

	/* Free the lowmem regions from memblock into bootmem. */
	for_each_memblock(memory, reg) {
		unsigned long start = memblock_region_memory_base_pfn(reg);
		unsigned long end = memblock_region_memory_end_pfn(reg);

		if (end >= end_pfn)
			end = end_pfn;
		if (start >= end)
			break;

		free_bootmem(__pfn_to_phys(start), (end - start) << PAGE_SHIFT);
	}

	/* Reserve the lowmem memblock reserved regions in bootmem. */
	for_each_memblock(reserved, reg) {
		unsigned long start = memblock_region_reserved_base_pfn(reg);
		unsigned long end = memblock_region_reserved_end_pfn(reg);

		if (end >= end_pfn)
			end = end_pfn;
		if (start >= end)
			break;

		reserve_bootmem(__pfn_to_phys(start),
			        (end - start) << PAGE_SHIFT, BOOTMEM_DEFAULT);
	}
=======
static void __init find_limits(unsigned long *min, unsigned long *max_low,
			       unsigned long *max_high)
{
	*max_low = PFN_DOWN(memblock_get_current_limit());
	*min = PFN_UP(memblock_start_of_DRAM());
	*max_high = PFN_DOWN(memblock_end_of_DRAM());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifdef CONFIG_ZONE_DMA

<<<<<<< HEAD
unsigned long arm_dma_zone_size __read_mostly;
=======
phys_addr_t arm_dma_zone_size __read_mostly;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL(arm_dma_zone_size);

/*
 * The DMA mask corresponding to the maximum bus address allocatable
 * using GFP_DMA.  The default here places no restriction on DMA
 * allocations.  This must be the smallest DMA mask in the system,
 * so a successful GFP_DMA allocation will always satisfy this.
 */
phys_addr_t arm_dma_limit;
<<<<<<< HEAD
=======
unsigned long arm_dma_pfn_limit;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void __init arm_adjust_dma_zone(unsigned long *size, unsigned long *hole,
	unsigned long dma_size)
{
	if (size[0] <= dma_size)
		return;

	size[ZONE_NORMAL] = size[0] - dma_size;
	size[ZONE_DMA] = dma_size;
	hole[ZONE_NORMAL] = hole[0];
	hole[ZONE_DMA] = 0;
}
#endif

<<<<<<< HEAD
void __init setup_dma_zone(struct machine_desc *mdesc)
=======
void __init setup_dma_zone(const struct machine_desc *mdesc)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
#ifdef CONFIG_ZONE_DMA
	if (mdesc->dma_zone_size) {
		arm_dma_zone_size = mdesc->dma_zone_size;
		arm_dma_limit = PHYS_OFFSET + arm_dma_zone_size - 1;
	} else
		arm_dma_limit = 0xffffffff;
<<<<<<< HEAD
#endif
}

static void __init arm_bootmem_free(unsigned long min, unsigned long max_low,
=======
	arm_dma_pfn_limit = arm_dma_limit >> PAGE_SHIFT;
#endif
}

static void __init zone_sizes_init(unsigned long min, unsigned long max_low,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long max_high)
{
	unsigned long zone_size[MAX_NR_ZONES], zhole_size[MAX_NR_ZONES];
	struct memblock_region *reg;

	/*
	 * initialise the zones.
	 */
	memset(zone_size, 0, sizeof(zone_size));

	/*
	 * The memory size has already been determined.  If we need
	 * to do anything fancy with the allocation of this memory
	 * to the zones, now is the time to do it.
	 */
	zone_size[0] = max_low - min;
#ifdef CONFIG_HIGHMEM
	zone_size[ZONE_HIGHMEM] = max_high - max_low;
#endif

	/*
	 * Calculate the size of the holes.
	 *  holes = node_size - sum(bank_sizes)
	 */
	memcpy(zhole_size, zone_size, sizeof(zhole_size));
	for_each_memblock(memory, reg) {
		unsigned long start = memblock_region_memory_base_pfn(reg);
		unsigned long end = memblock_region_memory_end_pfn(reg);

		if (start < max_low) {
			unsigned long low_end = min(end, max_low);
			zhole_size[0] -= low_end - start;
		}
#ifdef CONFIG_HIGHMEM
		if (end > max_low) {
			unsigned long high_start = max(start, max_low);
			zhole_size[ZONE_HIGHMEM] -= end - high_start;
		}
#endif
	}

#ifdef CONFIG_ZONE_DMA
	/*
	 * Adjust the sizes according to any special requirements for
	 * this machine type.
	 */
	if (arm_dma_zone_size)
		arm_adjust_dma_zone(zone_size, zhole_size,
			arm_dma_zone_size >> PAGE_SHIFT);
#endif

	free_area_init_node(0, zone_size, min, zhole_size);
}

#ifdef CONFIG_HAVE_ARCH_PFN_VALID
int pfn_valid(unsigned long pfn)
{
<<<<<<< HEAD
	return memblock_is_memory(__pfn_to_phys(pfn));
=======
	phys_addr_t addr = __pfn_to_phys(pfn);

	if (__phys_to_pfn(addr) != pfn)
		return 0;

	return memblock_is_map_memory(__pfn_to_phys(pfn));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL(pfn_valid);
#endif

#ifndef CONFIG_SPARSEMEM
static void __init arm_memory_present(void)
{
}
#else
static void __init arm_memory_present(void)
{
	struct memblock_region *reg;

	for_each_memblock(memory, reg)
		memory_present(0, memblock_region_memory_base_pfn(reg),
			       memblock_region_memory_end_pfn(reg));
}
#endif

static bool arm_memblock_steal_permitted = true;

phys_addr_t __init arm_memblock_steal(phys_addr_t size, phys_addr_t align)
{
	phys_addr_t phys;

	BUG_ON(!arm_memblock_steal_permitted);

	phys = memblock_alloc_base(size, align, MEMBLOCK_ALLOC_ANYWHERE);
	memblock_free(phys, size);
	memblock_remove(phys, size);

	return phys;
}

<<<<<<< HEAD
void __init arm_memblock_init(struct meminfo *mi, struct machine_desc *mdesc)
{
	int i;

	for (i = 0; i < mi->nr_banks; i++)
		memblock_add(mi->bank[i].start, mi->bank[i].size);

=======
void __init arm_memblock_init(const struct machine_desc *mdesc)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Register the kernel text, kernel data and initrd with memblock. */
#ifdef CONFIG_XIP_KERNEL
	memblock_reserve(__pa(_sdata), _end - _sdata);
#else
	memblock_reserve(__pa(_stext), _end - _stext);
#endif
#ifdef CONFIG_BLK_DEV_INITRD
<<<<<<< HEAD
	if (phys_initrd_size &&
	    !memblock_is_region_memory(phys_initrd_start, phys_initrd_size)) {
		pr_err("INITRD: 0x%08lx+0x%08lx is not a memory region - disabling initrd\n",
		       phys_initrd_start, phys_initrd_size);
=======
	/* FDT scan will populate initrd_start */
	if (initrd_start && !phys_initrd_size) {
		phys_initrd_start = __virt_to_phys(initrd_start);
		phys_initrd_size = initrd_end - initrd_start;
	}
	initrd_start = initrd_end = 0;
	if (phys_initrd_size &&
	    !memblock_is_region_memory(phys_initrd_start, phys_initrd_size)) {
		pr_err("INITRD: 0x%08llx+0x%08lx is not a memory region - disabling initrd\n",
		       (u64)phys_initrd_start, phys_initrd_size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		phys_initrd_start = phys_initrd_size = 0;
	}
	if (phys_initrd_size &&
	    memblock_is_region_reserved(phys_initrd_start, phys_initrd_size)) {
<<<<<<< HEAD
		pr_err("INITRD: 0x%08lx+0x%08lx overlaps in-use memory region - disabling initrd\n",
		       phys_initrd_start, phys_initrd_size);
=======
		pr_err("INITRD: 0x%08llx+0x%08lx overlaps in-use memory region - disabling initrd\n",
		       (u64)phys_initrd_start, phys_initrd_size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		phys_initrd_start = phys_initrd_size = 0;
	}
	if (phys_initrd_size) {
		memblock_reserve(phys_initrd_start, phys_initrd_size);

		/* Now convert initrd to virtual addresses */
		initrd_start = __phys_to_virt(phys_initrd_start);
		initrd_end = initrd_start + phys_initrd_size;
	}
#endif

	arm_mm_memblock_reserve();
<<<<<<< HEAD
	arm_dt_memblock_reserve();

	early_init_fdt_scan_reserved_mem();
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* reserve any platform specific memblock areas */
	if (mdesc->reserve)
		mdesc->reserve();

<<<<<<< HEAD
	/*
	 * reserve memory for DMA contigouos allocations,
	 * must come from DMA area inside low memory
	 */
	dma_contiguous_reserve(min(arm_dma_limit, arm_lowmem_limit));

	arm_memblock_steal_permitted = false;
	memblock_allow_resize();
=======
	early_init_fdt_reserve_self();
	early_init_fdt_scan_reserved_mem();

	/* reserve memory for DMA contiguous allocations */
	dma_contiguous_reserve(arm_dma_limit);

	arm_memblock_steal_permitted = false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	memblock_dump_all();
}

void __init bootmem_init(void)
{
	unsigned long min, max_low, max_high;

<<<<<<< HEAD
=======
	memblock_allow_resize();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	max_low = max_high = 0;

	find_limits(&min, &max_low, &max_high);

<<<<<<< HEAD
	arm_bootmem_init(min, max_low);
=======
	early_memtest((phys_addr_t)min << PAGE_SHIFT,
		      (phys_addr_t)max_low << PAGE_SHIFT);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Sparsemem tries to allocate bootmem in memory_present(),
	 * so must be done after the fixed reservations
	 */
	arm_memory_present();

	/*
	 * sparse_init() needs the bootmem allocator up and running.
	 */
	sparse_init();

	/*
	 * Now free the memory - free_area_init_node needs
	 * the sparse mem_map arrays initialized by sparse_init()
	 * for memmap_init_zone(), otherwise all PFNs are invalid.
	 */
<<<<<<< HEAD
	arm_bootmem_free(min, max_low, max_high);
=======
	zone_sizes_init(min, max_low, max_high);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * This doesn't seem to be used by the Linux memory manager any
	 * more, but is used by ll_rw_block.  If we can get rid of it, we
	 * also get rid of some of the stuff above as well.
<<<<<<< HEAD
	 *
	 * Note: max_low_pfn and max_pfn reflect the number of _pages_ in
	 * the system, not the maximum PFN.
	 */
	max_low_pfn = max_low - PHYS_PFN_OFFSET;
	max_pfn = max_high - PHYS_PFN_OFFSET;
=======
	 */
	min_low_pfn = min;
	max_low_pfn = max_low;
	max_pfn = max_high;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Poison init memory with an undefined instruction (ARM) or a branch to an
 * undefined instruction (Thumb).
 */
static inline void poison_init_mem(void *s, size_t count)
{
	u32 *p = (u32 *)s;
	for (; count != 0; count -= 4)
		*p++ = 0xe7fddef0;
}

static inline void
free_memmap(unsigned long start_pfn, unsigned long end_pfn)
{
	struct page *start_pg, *end_pg;
<<<<<<< HEAD
	unsigned long pg, pgend;
=======
	phys_addr_t pg, pgend;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Convert start_pfn/end_pfn to a struct page pointer.
	 */
	start_pg = pfn_to_page(start_pfn - 1) + 1;
	end_pg = pfn_to_page(end_pfn - 1) + 1;

	/*
	 * Convert to physical addresses, and
	 * round start upwards and end downwards.
	 */
<<<<<<< HEAD
	pg = (unsigned long)PAGE_ALIGN(__pa(start_pg));
	pgend = (unsigned long)__pa(end_pg) & PAGE_MASK;
=======
	pg = PAGE_ALIGN(__pa(start_pg));
	pgend = __pa(end_pg) & PAGE_MASK;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * If there are free pages between these,
	 * free the section of the memmap array.
	 */
	if (pg < pgend)
<<<<<<< HEAD
		free_bootmem(pg, pgend - pg);
=======
		memblock_free_early(pg, pgend - pg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * The mem_map array can get very big.  Free the unused area of the memory map.
 */
<<<<<<< HEAD
static void __init free_unused_memmap(struct meminfo *mi)
{
	unsigned long bank_start, prev_bank_end = 0;
	unsigned int i;
=======
static void __init free_unused_memmap(void)
{
	unsigned long start, prev_end = 0;
	struct memblock_region *reg;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * This relies on each bank being in address order.
	 * The banks are sorted previously in bootmem_init().
	 */
<<<<<<< HEAD
	for_each_bank(i, mi) {
		struct membank *bank = &mi->bank[i];

		bank_start = bank_pfn_start(bank);
=======
	for_each_memblock(memory, reg) {
		start = memblock_region_memory_base_pfn(reg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_SPARSEMEM
		/*
		 * Take care not to free memmap entries that don't exist
		 * due to SPARSEMEM sections which aren't present.
		 */
<<<<<<< HEAD
		bank_start = min(bank_start,
				 ALIGN(prev_bank_end, PAGES_PER_SECTION));
=======
		start = min(start,
				 ALIGN(prev_end, PAGES_PER_SECTION));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
		/*
		 * Align down here since the VM subsystem insists that the
		 * memmap entries are valid from the bank start aligned to
		 * MAX_ORDER_NR_PAGES.
		 */
<<<<<<< HEAD
		bank_start = round_down(bank_start, MAX_ORDER_NR_PAGES);
=======
		start = round_down(start, MAX_ORDER_NR_PAGES);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
		/*
		 * If we had a previous bank, and there is a space
		 * between the current bank and the previous, free it.
		 */
<<<<<<< HEAD
		if (prev_bank_end && prev_bank_end < bank_start)
			free_memmap(prev_bank_end, bank_start);
=======
		if (prev_end && prev_end < start)
			free_memmap(prev_end, start);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/*
		 * Align up here since the VM subsystem insists that the
		 * memmap entries are valid from the bank end aligned to
		 * MAX_ORDER_NR_PAGES.
		 */
<<<<<<< HEAD
		prev_bank_end = ALIGN(bank_pfn_end(bank), MAX_ORDER_NR_PAGES);
	}

#ifdef CONFIG_SPARSEMEM
	if (!IS_ALIGNED(prev_bank_end, PAGES_PER_SECTION))
		free_memmap(prev_bank_end,
			    ALIGN(prev_bank_end, PAGES_PER_SECTION));
=======
		prev_end = ALIGN(memblock_region_memory_end_pfn(reg),
				 MAX_ORDER_NR_PAGES);
	}

#ifdef CONFIG_SPARSEMEM
	if (!IS_ALIGNED(prev_end, PAGES_PER_SECTION))
		free_memmap(prev_end,
			    ALIGN(prev_end, PAGES_PER_SECTION));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
}

#ifdef CONFIG_HIGHMEM
static inline void free_area_high(unsigned long pfn, unsigned long end)
{
	for (; pfn < end; pfn++)
		free_highmem_page(pfn_to_page(pfn));
}
#endif

static void __init free_highpages(void)
{
#ifdef CONFIG_HIGHMEM
<<<<<<< HEAD
	unsigned long max_low = max_low_pfn + PHYS_PFN_OFFSET;
=======
	unsigned long max_low = max_low_pfn;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct memblock_region *mem, *res;

	/* set highmem page free */
	for_each_memblock(memory, mem) {
		unsigned long start = memblock_region_memory_base_pfn(mem);
		unsigned long end = memblock_region_memory_end_pfn(mem);

		/* Ignore complete lowmem entries */
		if (end <= max_low)
			continue;

<<<<<<< HEAD
=======
		if (memblock_is_nomap(mem))
			continue;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* Truncate partial highmem entries */
		if (start < max_low)
			start = max_low;

		/* Find and exclude any reserved regions */
		for_each_memblock(reserved, res) {
			unsigned long res_start, res_end;

			res_start = memblock_region_reserved_base_pfn(res);
			res_end = memblock_region_reserved_end_pfn(res);

			if (res_end < start)
				continue;
			if (res_start < start)
				res_start = start;
			if (res_start > end)
				res_start = end;
			if (res_end > end)
				res_end = end;
			if (res_start != start)
				free_area_high(start, res_start);
			start = res_end;
			if (start == end)
				break;
		}

		/* And now free anything which remains */
		if (start < end)
			free_area_high(start, end);
	}
#endif
}

/*
 * mem_init() marks the free areas in the mem_map and tells us how much
 * memory is free.  This is done after various parts of the system have
 * claimed their memory after the kernel image.
 */
void __init mem_init(void)
{
<<<<<<< HEAD
	unsigned long reserved_pages, free_pages;
	struct memblock_region *reg;
	int i;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_HAVE_TCM
	/* These pointers are filled in on TCM detection */
	extern u32 dtcm_end;
	extern u32 itcm_end;
#endif

<<<<<<< HEAD
	max_mapnr   = pfn_to_page(max_pfn + PHYS_PFN_OFFSET) - mem_map;

	/* this will put all unused low memory onto the freelists */
	free_unused_memmap(&meminfo);

	totalram_pages += free_all_bootmem();

#ifdef CONFIG_SA1111
	/* now that our DMA memory is actually so designated, we can free it */
	free_reserved_area(__va(PHYS_OFFSET), swapper_pg_dir, 0, NULL);
=======
	set_max_mapnr(pfn_to_page(max_pfn) - mem_map);

	/* this will put all unused low memory onto the freelists */
	free_unused_memmap();
	free_all_bootmem();

#ifdef CONFIG_SA1111
	/* now that our DMA memory is actually so designated, we can free it */
	free_reserved_area(__va(PHYS_OFFSET), swapper_pg_dir, -1, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

	free_highpages();

<<<<<<< HEAD
	reserved_pages = free_pages = 0;

	for_each_bank(i, &meminfo) {
		struct membank *bank = &meminfo.bank[i];
		unsigned int pfn1, pfn2;
		struct page *page, *end;

		pfn1 = bank_pfn_start(bank);
		pfn2 = bank_pfn_end(bank);

		page = pfn_to_page(pfn1);
		end  = pfn_to_page(pfn2 - 1) + 1;

		do {
			if (PageReserved(page))
				reserved_pages++;
			else if (!page_count(page))
				free_pages++;
			page++;
		} while (page < end);
	}

	/*
	 * Since our memory may not be contiguous, calculate the
	 * real number of pages we have in this system
	 */
	printk(KERN_INFO "Memory:");
	num_physpages = 0;
	for_each_memblock(memory, reg) {
		unsigned long pages = memblock_region_memory_end_pfn(reg) -
			memblock_region_memory_base_pfn(reg);
		num_physpages += pages;
		printk(" %ldMB", pages >> (20 - PAGE_SHIFT));
	}
	printk(" = %luMB total\n", num_physpages >> (20 - PAGE_SHIFT));

	printk(KERN_NOTICE "Memory: %luk/%luk available, %luk reserved, %luK highmem\n",
		nr_free_pages() << (PAGE_SHIFT-10),
		free_pages << (PAGE_SHIFT-10),
		reserved_pages << (PAGE_SHIFT-10),
		totalhigh_pages << (PAGE_SHIFT-10));
=======
	mem_init_print_info(NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define MLK(b, t) b, t, ((t) - (b)) >> 10
#define MLM(b, t) b, t, ((t) - (b)) >> 20
#define MLK_ROUNDUP(b, t) b, t, DIV_ROUND_UP(((t) - (b)), SZ_1K)

<<<<<<< HEAD
	printk(KERN_NOTICE "Virtual kernel memory layout:\n"
=======
	pr_notice("Virtual kernel memory layout:\n"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			"    vector  : 0x%08lx - 0x%08lx   (%4ld kB)\n"
#ifdef CONFIG_HAVE_TCM
			"    DTCM    : 0x%08lx - 0x%08lx   (%4ld kB)\n"
			"    ITCM    : 0x%08lx - 0x%08lx   (%4ld kB)\n"
#endif
			"    fixmap  : 0x%08lx - 0x%08lx   (%4ld kB)\n"
			"    vmalloc : 0x%08lx - 0x%08lx   (%4ld MB)\n"
			"    lowmem  : 0x%08lx - 0x%08lx   (%4ld MB)\n"
#ifdef CONFIG_HIGHMEM
			"    pkmap   : 0x%08lx - 0x%08lx   (%4ld MB)\n"
#endif
#ifdef CONFIG_MODULES
			"    modules : 0x%08lx - 0x%08lx   (%4ld MB)\n"
#endif
<<<<<<< HEAD
			"      .text : 0x%p" " - 0x%p" "   (%4d kB)\n"
			"      .init : 0x%p" " - 0x%p" "   (%4d kB)\n"
			"      .data : 0x%p" " - 0x%p" "   (%4d kB)\n"
			"       .bss : 0x%p" " - 0x%p" "   (%4d kB)\n",
=======
			"      .text : 0x%p" " - 0x%p" "   (%4td kB)\n"
			"      .init : 0x%p" " - 0x%p" "   (%4td kB)\n"
			"      .data : 0x%p" " - 0x%p" "   (%4td kB)\n"
			"       .bss : 0x%p" " - 0x%p" "   (%4td kB)\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			MLK(UL(CONFIG_VECTORS_BASE), UL(CONFIG_VECTORS_BASE) +
				(PAGE_SIZE)),
#ifdef CONFIG_HAVE_TCM
			MLK(DTCM_OFFSET, (unsigned long) dtcm_end),
			MLK(ITCM_OFFSET, (unsigned long) itcm_end),
#endif
<<<<<<< HEAD
			MLK(FIXADDR_START, FIXADDR_TOP),
=======
			MLK(FIXADDR_START, FIXADDR_END),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			MLM(VMALLOC_START, VMALLOC_END),
			MLM(PAGE_OFFSET, (unsigned long)high_memory),
#ifdef CONFIG_HIGHMEM
			MLM(PKMAP_BASE, (PKMAP_BASE) + (LAST_PKMAP) *
				(PAGE_SIZE)),
#endif
#ifdef CONFIG_MODULES
			MLM(MODULES_VADDR, MODULES_END),
#endif

			MLK_ROUNDUP(_text, _etext),
			MLK_ROUNDUP(__init_begin, __init_end),
			MLK_ROUNDUP(_sdata, _edata),
			MLK_ROUNDUP(__bss_start, __bss_stop));

#undef MLK
#undef MLM
#undef MLK_ROUNDUP

	/*
	 * Check boundaries twice: Some fundamental inconsistencies can
	 * be detected at build time already.
	 */
#ifdef CONFIG_MMU
	BUILD_BUG_ON(TASK_SIZE				> MODULES_VADDR);
	BUG_ON(TASK_SIZE 				> MODULES_VADDR);
#endif

#ifdef CONFIG_HIGHMEM
	BUILD_BUG_ON(PKMAP_BASE + LAST_PKMAP * PAGE_SIZE > PAGE_OFFSET);
	BUG_ON(PKMAP_BASE + LAST_PKMAP * PAGE_SIZE	> PAGE_OFFSET);
#endif

<<<<<<< HEAD
	if (PAGE_SIZE >= 16384 && num_physpages <= 128) {
=======
	if (PAGE_SIZE >= 16384 && get_num_physpages() <= 128) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		extern int sysctl_overcommit_memory;
		/*
		 * On a machine this small we won't get
		 * anywhere without overcommit, so turn
		 * it on by default.
		 */
		sysctl_overcommit_memory = OVERCOMMIT_ALWAYS;
	}
}

<<<<<<< HEAD
void free_initmem(void)
=======
#ifdef CONFIG_DEBUG_RODATA
struct section_perm {
	const char *name;
	unsigned long start;
	unsigned long end;
	pmdval_t mask;
	pmdval_t prot;
	pmdval_t clear;
};

/* First section-aligned location at or after __start_rodata. */
extern char __start_rodata_section_aligned[];

static struct section_perm nx_perms[] = {
	/* Make pages tables, etc before _stext RW (set NX). */
	{
		.name	= "pre-text NX",
		.start	= PAGE_OFFSET,
		.end	= (unsigned long)_stext,
		.mask	= ~PMD_SECT_XN,
		.prot	= PMD_SECT_XN,
	},
	/* Make init RW (set NX). */
	{
		.name	= "init NX",
		.start	= (unsigned long)__init_begin,
		.end	= (unsigned long)_sdata,
		.mask	= ~PMD_SECT_XN,
		.prot	= PMD_SECT_XN,
	},
	/* Make rodata NX (set RO in ro_perms below). */
	{
		.name	= "rodata NX",
		.start  = (unsigned long)__start_rodata_section_aligned,
		.end    = (unsigned long)__init_begin,
		.mask   = ~PMD_SECT_XN,
		.prot   = PMD_SECT_XN,
	},
};

static struct section_perm ro_perms[] = {
	/* Make kernel code and rodata RX (set RO). */
	{
		.name	= "text/rodata RO",
		.start  = (unsigned long)_stext,
		.end    = (unsigned long)__init_begin,
#ifdef CONFIG_ARM_LPAE
		.mask   = ~(L_PMD_SECT_RDONLY | PMD_SECT_AP2),
		.prot   = L_PMD_SECT_RDONLY | PMD_SECT_AP2,
#else
		.mask   = ~(PMD_SECT_APX | PMD_SECT_AP_WRITE),
		.prot   = PMD_SECT_APX | PMD_SECT_AP_WRITE,
		.clear  = PMD_SECT_AP_WRITE,
#endif
	},
};

/*
 * Updates section permissions only for the current mm (sections are
 * copied into each mm). During startup, this is the init_mm. Is only
 * safe to be called with preemption disabled, as under stop_machine().
 */
static inline void section_update(unsigned long addr, pmdval_t mask,
				  pmdval_t prot, struct mm_struct *mm)
{
	pmd_t *pmd;

	pmd = pmd_offset(pud_offset(pgd_offset(mm, addr), addr), addr);

#ifdef CONFIG_ARM_LPAE
	pmd[0] = __pmd((pmd_val(pmd[0]) & mask) | prot);
#else
	if (addr & SECTION_SIZE)
		pmd[1] = __pmd((pmd_val(pmd[1]) & mask) | prot);
	else
		pmd[0] = __pmd((pmd_val(pmd[0]) & mask) | prot);
#endif
	flush_pmd_entry(pmd);
	local_flush_tlb_kernel_range(addr, addr + SECTION_SIZE);
}

/* Make sure extended page tables are in use. */
static inline bool arch_has_strict_perms(void)
{
	if (cpu_architecture() < CPU_ARCH_ARMv6)
		return false;

	return !!(get_cr() & CR_XP);
}

void set_section_perms(struct section_perm *perms, int n, bool set,
			struct mm_struct *mm)
{
	size_t i;
	unsigned long addr;

	if (!arch_has_strict_perms())
		return;

	for (i = 0; i < n; i++) {
		if (!IS_ALIGNED(perms[i].start, SECTION_SIZE) ||
		    !IS_ALIGNED(perms[i].end, SECTION_SIZE)) {
			pr_err("BUG: %s section %lx-%lx not aligned to %lx\n",
				perms[i].name, perms[i].start, perms[i].end,
				SECTION_SIZE);
			continue;
		}

		for (addr = perms[i].start;
		     addr < perms[i].end;
		     addr += SECTION_SIZE)
			section_update(addr, perms[i].mask,
				set ? perms[i].prot : perms[i].clear, mm);
	}

}

static void update_sections_early(struct section_perm perms[], int n)
{
	struct task_struct *t, *s;

	read_lock(&tasklist_lock);
	for_each_process(t) {
		if (t->flags & PF_KTHREAD)
			continue;
		for_each_thread(t, s)
			if (s->mm)
				set_section_perms(perms, n, true, s->mm);
	}
	read_unlock(&tasklist_lock);
	set_section_perms(perms, n, true, current->active_mm);
	set_section_perms(perms, n, true, &init_mm);
}

int __fix_kernmem_perms(void *unused)
{
	update_sections_early(nx_perms, ARRAY_SIZE(nx_perms));
	return 0;
}

void fix_kernmem_perms(void)
{
	stop_machine(__fix_kernmem_perms, NULL, NULL);
}

int __mark_rodata_ro(void *unused)
{
	update_sections_early(ro_perms, ARRAY_SIZE(ro_perms));
	return 0;
}

static int kernel_set_to_readonly __read_mostly;

void mark_rodata_ro(void)
{
	kernel_set_to_readonly = 1;
	stop_machine(__mark_rodata_ro, NULL, NULL);
}

void set_kernel_text_rw(void)
{
	if (!kernel_set_to_readonly)
		return;

	set_section_perms(ro_perms, ARRAY_SIZE(ro_perms), false,
				current->active_mm);
}

void set_kernel_text_ro(void)
{
	if (!kernel_set_to_readonly)
		return;

	set_section_perms(ro_perms, ARRAY_SIZE(ro_perms), true,
				current->active_mm);
}

#else
static inline void fix_kernmem_perms(void) { }
#endif /* CONFIG_DEBUG_RODATA */

void free_tcmmem(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
#ifdef CONFIG_HAVE_TCM
	extern char __tcm_start, __tcm_end;

	poison_init_mem(&__tcm_start, &__tcm_end - &__tcm_start);
<<<<<<< HEAD
	free_reserved_area(&__tcm_start, &__tcm_end, 0, "TCM link");
#endif

	poison_init_mem(__init_begin, __init_end - __init_begin);
	if (!machine_is_integrator() && !machine_is_cintegrator())
		free_initmem_default(0);
=======
	free_reserved_area(&__tcm_start, &__tcm_end, -1, "TCM link");
#endif
}

void free_initmem(void)
{
	fix_kernmem_perms();
	free_tcmmem();

	poison_init_mem(__init_begin, __init_end - __init_begin);
	if (!machine_is_integrator() && !machine_is_cintegrator())
		free_initmem_default(-1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifdef CONFIG_BLK_DEV_INITRD

static int keep_initrd;

void free_initrd_mem(unsigned long start, unsigned long end)
{
	if (!keep_initrd) {
<<<<<<< HEAD
		poison_init_mem((void *)start, PAGE_ALIGN(end) - start);
		free_reserved_area(start, end, 0, "initrd");
=======
		if (start == initrd_start)
			start = round_down(start, PAGE_SIZE);
		if (end == initrd_end)
			end = round_up(end, PAGE_SIZE);

		poison_init_mem((void *)start, PAGE_ALIGN(end) - start);
		free_reserved_area((void *)start, (void *)end, -1, "initrd");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

static int __init keepinitrd_setup(char *__unused)
{
	keep_initrd = 1;
	return 1;
}

__setup("keepinitrd", keepinitrd_setup);
#endif
