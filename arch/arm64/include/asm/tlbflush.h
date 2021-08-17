/*
 * Based on arch/arm/include/asm/tlbflush.h
 *
 * Copyright (C) 1999-2003 Russell King
 * Copyright (C) 2012 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __ASM_TLBFLUSH_H
#define __ASM_TLBFLUSH_H

#ifndef __ASSEMBLY__

#include <linux/sched.h>
#include <asm/cputype.h>
<<<<<<< HEAD

extern void __cpu_flush_user_tlb_range(unsigned long, unsigned long, struct vm_area_struct *);
extern void __cpu_flush_kern_tlb_range(unsigned long, unsigned long);

extern struct cpu_tlb_fns cpu_tlb;
=======
#include <asm/mmu.h>

/*
 * Raw TLBI operations.
 *
 * Where necessary, use the __tlbi() macro to avoid asm()
 * boilerplate. Drivers and most kernel code should use the TLB
 * management routines in preference to the macro below.
 *
 * The macro can be used as __tlbi(op) or __tlbi(op, arg), depending
 * on whether a particular TLBI operation takes an argument or
 * not. The macros handles invoking the asm with or without the
 * register argument as appropriate.
 */
#define __TLBI_0(op, arg)		asm ("tlbi " #op)
#define __TLBI_1(op, arg)		asm ("tlbi " #op ", %0" : : "r" (arg))
#define __TLBI_N(op, arg, n, ...)	__TLBI_##n(op, arg)

#define __tlbi(op, ...)		__TLBI_N(op, ##__VA_ARGS__, 1, 0)

#define __tlbi_user(op, arg) do {						\
	if (arm64_kernel_unmapped_at_el0())					\
		__tlbi(op, (arg) | USER_ASID_FLAG);				\
} while (0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 *	TLB Management
 *	==============
 *
<<<<<<< HEAD
 *	The arch/arm64/mm/tlb.S files implement these methods.
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *	The TLB specific code is expected to perform whatever tests it needs
 *	to determine if it should invalidate the TLB for each call.  Start
 *	addresses are inclusive and end addresses are exclusive; it is safe to
 *	round these addresses down.
 *
 *	flush_tlb_all()
 *
 *		Invalidate the entire TLB.
 *
 *	flush_tlb_mm(mm)
 *
 *		Invalidate all TLB entries in a particular address space.
 *		- mm	- mm_struct describing address space
 *
 *	flush_tlb_range(mm,start,end)
 *
 *		Invalidate a range of TLB entries in the specified address
 *		space.
 *		- mm	- mm_struct describing address space
 *		- start - start address (may not be aligned)
 *		- end	- end address (exclusive, may not be aligned)
 *
 *	flush_tlb_page(vaddr,vma)
 *
 *		Invalidate the specified page in the specified address range.
 *		- vaddr - virtual address (may not be aligned)
 *		- vma	- vma_struct describing address range
 *
 *	flush_kern_tlb_page(kaddr)
 *
 *		Invalidate the TLB entry for the specified page.  The address
 *		will be in the kernels virtual memory space.  Current uses
 *		only require the D-TLB to be invalidated.
 *		- kaddr - Kernel virtual memory address
 */
<<<<<<< HEAD
static inline void flush_tlb_all(void)
{
	dsb(ishst);
	asm("tlbi	vmalle1is");
=======
static inline void local_flush_tlb_all(void)
{
	dsb(nshst);
	__tlbi(vmalle1);
	dsb(nsh);
	isb();
}

static inline void flush_tlb_all(void)
{
	dsb(ishst);
	__tlbi(vmalle1is);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dsb(ish);
	isb();
}

static inline void flush_tlb_mm(struct mm_struct *mm)
{
<<<<<<< HEAD
	unsigned long asid = (unsigned long)ASID(mm) << 48;

	dsb(ishst);
	asm("tlbi	aside1is, %0" : : "r" (asid));
	dsb(ish);
}

#ifdef CONFIG_TTBR1_48BITS_VARANGE_BUG
#define TTBR1_SHIFT_BITS	39
#endif

static inline void flush_tlb_page(struct vm_area_struct *vma,
				  unsigned long uaddr)
{
	unsigned long addr = uaddr >> 12 |
		((unsigned long)ASID(vma->vm_mm) << 48);

#ifdef CONFIG_TTBR1_48BITS_VARANGE_BUG
	if (uaddr >> TTBR1_SHIFT_BITS) {
		flush_tlb_mm(vma->vm_mm);
	} else
#endif
	{
		dsb(ishst);
		asm("tlbi	vae1is, %0" : : "r" (addr));
		dsb(ish);
	}
}

static inline void __flush_tlb_range(struct vm_area_struct *vma,
				     unsigned long start, unsigned long end)
{
	unsigned long asid = (unsigned long)ASID(vma->vm_mm) << 48;
	unsigned long addr;

#ifdef CONFIG_TTBR1_48BITS_VARANGE_BUG
	if (start >> TTBR1_SHIFT_BITS) {
		flush_tlb_mm(vma->vm_mm);
	} else
#endif
	{
		start = asid | (start >> 12);
		end = asid | (end >> 12);

		dsb(ishst);
		for (addr = start; addr < end; addr += 1 << (PAGE_SHIFT - 12))
			asm("tlbi vae1is, %0" : : "r"(addr));
		dsb(ish);
	}
}

static inline void __flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	unsigned long addr;

#ifdef CONFIG_TTBR1_48BITS_VARANGE_BUG
	if (start >> TTBR1_SHIFT_BITS) {
		flush_tlb_all();
	} else
#endif
	{
		start >>= 12;
		end >>= 12;

		dsb(ishst);
		for (addr = start; addr < end; addr += 1 << (PAGE_SHIFT - 12))
			asm("tlbi vaae1is, %0" : : "r"(addr));
		dsb(ish);

		isb();
	}
}

/*
 * This is meant to avoid soft lock-ups on large TLB flushing ranges and not
 * necessarily a performance improvement.
 */
#define MAX_TLB_RANGE   (1024UL << PAGE_SHIFT)

static inline void flush_tlb_range(struct vm_area_struct *vma,
                                   unsigned long start, unsigned long end)
{
        if ((end - start) <= MAX_TLB_RANGE)
                __flush_tlb_range(vma, start, end);
        else
                flush_tlb_mm(vma->vm_mm);
=======
	unsigned long asid = ASID(mm) << 48;

	dsb(ishst);
	__tlbi(aside1is, asid);
	__tlbi_user(aside1is, asid);
	dsb(ish);
}

static inline void flush_tlb_page(struct vm_area_struct *vma,
				  unsigned long uaddr)
{
	unsigned long addr = uaddr >> 12 | (ASID(vma->vm_mm) << 48);

	dsb(ishst);
	__tlbi(vale1is, addr);
	__tlbi_user(vale1is, addr);
	dsb(ish);
}

/*
 * This is meant to avoid soft lock-ups on large TLB flushing ranges and not
 * necessarily a performance improvement.
 */
#define MAX_TLB_RANGE	(1024UL << PAGE_SHIFT)

static inline void __flush_tlb_range(struct vm_area_struct *vma,
				     unsigned long start, unsigned long end,
				     bool last_level)
{
	unsigned long asid = ASID(vma->vm_mm) << 48;
	unsigned long addr;

	if ((end - start) > MAX_TLB_RANGE) {
		flush_tlb_mm(vma->vm_mm);
		return;
	}

	start = asid | (start >> 12);
	end = asid | (end >> 12);

	dsb(ishst);
	for (addr = start; addr < end; addr += 1 << (PAGE_SHIFT - 12)) {
		if (last_level) {
			__tlbi(vale1is, addr);
			__tlbi_user(vale1is, addr);
		} else {
			__tlbi(vae1is, addr);
			__tlbi_user(vae1is, addr);
		}
	}
	dsb(ish);
}

static inline void flush_tlb_range(struct vm_area_struct *vma,
				   unsigned long start, unsigned long end)
{
	__flush_tlb_range(vma, start, end, false);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
<<<<<<< HEAD
        if ((end - start) <= MAX_TLB_RANGE)
                __flush_tlb_kernel_range(start, end);
        else
                flush_tlb_all();
}


/*
 * On AArch64, the cache coherency is handled via the set_pte_at() function.
 */
static inline void update_mmu_cache(struct vm_area_struct *vma,
				    unsigned long addr, pte_t *ptep)
{
	/*
	 * set_pte() does not have a DSB for user mappings, so make sure that
	 * the page table write is visible.
	 */
	dsb(ishst);
}

#define update_mmu_cache_pmd(vma, address, pmd) do { } while (0)
=======
	unsigned long addr;

	if ((end - start) > MAX_TLB_RANGE) {
		flush_tlb_all();
		return;
	}

	start >>= 12;
	end >>= 12;

	dsb(ishst);
	for (addr = start; addr < end; addr += 1 << (PAGE_SHIFT - 12))
		__tlbi(vaae1is, addr);
	dsb(ish);
	isb();
}

/*
 * Used to invalidate the TLB (walk caches) corresponding to intermediate page
 * table levels (pgd/pud/pmd).
 */
static inline void __flush_tlb_pgtable(struct mm_struct *mm,
				       unsigned long uaddr)
{
	unsigned long addr = uaddr >> 12 | (ASID(mm) << 48);

	__tlbi(vae1is, addr);
	__tlbi_user(vae1is, addr);
	dsb(ish);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif

#endif
