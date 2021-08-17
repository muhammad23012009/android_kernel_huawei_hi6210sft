#ifndef _ASM_FIXMAP_H
#define _ASM_FIXMAP_H

<<<<<<< HEAD
/*
 * Nothing too fancy for now.
 *
 * On ARM we already have well known fixed virtual addresses imposed by
 * the architecture such as the vector page which is located at 0xffff0000,
 * therefore a second level page table is already allocated covering
 * 0xfff00000 upwards.
 *
 * The cache flushing code in proc-xscale.S uses the virtual area between
 * 0xfffe0000 and 0xfffeffff.
 */

#if defined (CHIP_BB_HI6210)
/*由于72KB的SRAM虚实地址必须相同(低功耗要求)，SRAM物理地址为0xFFF80000*/
#define FIXADDR_START       0xffe00000UL
#define FIXADDR_TOP         0xfff00000UL
#else
#define FIXADDR_START		0xfff00000UL
#define FIXADDR_TOP		    0xfffe0000UL
#endif
#define FIXADDR_SIZE		(FIXADDR_TOP - FIXADDR_START)

#define FIX_KMAP_BEGIN		0
#define FIX_KMAP_END		(FIXADDR_SIZE >> PAGE_SHIFT)

#define __fix_to_virt(x)	(FIXADDR_START + ((x) << PAGE_SHIFT))
#define __virt_to_fix(x)	(((x) - FIXADDR_START) >> PAGE_SHIFT)

extern void __this_fixmap_does_not_exist(void);

static inline unsigned long fix_to_virt(const unsigned int idx)
{
	if (idx >= FIX_KMAP_END)
		__this_fixmap_does_not_exist();
	return __fix_to_virt(idx);
}

static inline unsigned int virt_to_fix(const unsigned long vaddr)
{
	BUG_ON(vaddr >= FIXADDR_TOP || vaddr < FIXADDR_START);
	return __virt_to_fix(vaddr);
}

=======
#define FIXADDR_START		0xffc00000UL
#define FIXADDR_END		0xfff00000UL
#define FIXADDR_TOP		(FIXADDR_END - PAGE_SIZE)

#include <asm/kmap_types.h>
#include <asm/pgtable.h>

enum fixed_addresses {
	FIX_EARLYCON_MEM_BASE,
	__end_of_permanent_fixed_addresses,

	FIX_KMAP_BEGIN = __end_of_permanent_fixed_addresses,
	FIX_KMAP_END = FIX_KMAP_BEGIN + (KM_TYPE_NR * NR_CPUS) - 1,

	/* Support writing RO kernel text via kprobes, jump labels, etc. */
	FIX_TEXT_POKE0,
	FIX_TEXT_POKE1,

	__end_of_fixmap_region,

	/*
	 * Share the kmap() region with early_ioremap(): this is guaranteed
	 * not to clash since early_ioremap() is only available before
	 * paging_init(), and kmap() only after.
	 */
#define NR_FIX_BTMAPS		32
#define FIX_BTMAPS_SLOTS	7
#define TOTAL_FIX_BTMAPS	(NR_FIX_BTMAPS * FIX_BTMAPS_SLOTS)

	FIX_BTMAP_END = __end_of_permanent_fixed_addresses,
	FIX_BTMAP_BEGIN = FIX_BTMAP_END + TOTAL_FIX_BTMAPS - 1,
	__end_of_early_ioremap_region
};

static const enum fixed_addresses __end_of_fixed_addresses =
	__end_of_fixmap_region > __end_of_early_ioremap_region ?
	__end_of_fixmap_region : __end_of_early_ioremap_region;

#define FIXMAP_PAGE_COMMON	(L_PTE_YOUNG | L_PTE_PRESENT | L_PTE_XN | L_PTE_DIRTY)

#define FIXMAP_PAGE_NORMAL	(FIXMAP_PAGE_COMMON | L_PTE_MT_WRITEBACK)
#define FIXMAP_PAGE_RO		(FIXMAP_PAGE_NORMAL | L_PTE_RDONLY)

/* Used by set_fixmap_(io|nocache), both meant for mapping a device */
#define FIXMAP_PAGE_IO		(FIXMAP_PAGE_COMMON | L_PTE_MT_DEV_SHARED | L_PTE_SHARED)
#define FIXMAP_PAGE_NOCACHE	FIXMAP_PAGE_IO

#define __early_set_fixmap	__set_fixmap

#ifdef CONFIG_MMU

void __set_fixmap(enum fixed_addresses idx, phys_addr_t phys, pgprot_t prot);
void __init early_fixmap_init(void);

#include <asm-generic/fixmap.h>

#else

static inline void early_fixmap_init(void) { }

#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
