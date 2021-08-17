#ifndef _ASM_X86_PAGE_64_DEFS_H
#define _ASM_X86_PAGE_64_DEFS_H

<<<<<<< HEAD
#define THREAD_SIZE_ORDER	1
#define THREAD_SIZE  (PAGE_SIZE << THREAD_SIZE_ORDER)
#define CURRENT_MASK (~(THREAD_SIZE - 1))

#define EXCEPTION_STACK_ORDER 0
=======
#ifndef __ASSEMBLY__
#include <asm/kaslr.h>
#endif

#ifdef CONFIG_KASAN
#ifdef CONFIG_KASAN_EXTRA
#define KASAN_STACK_ORDER 2
#else
#define KASAN_STACK_ORDER 1
#endif
#else
#define KASAN_STACK_ORDER 0
#endif

#define THREAD_SIZE_ORDER	(2 + KASAN_STACK_ORDER)
#define THREAD_SIZE  (PAGE_SIZE << THREAD_SIZE_ORDER)
#define CURRENT_MASK (~(THREAD_SIZE - 1))

#define EXCEPTION_STACK_ORDER (0 + KASAN_STACK_ORDER)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define EXCEPTION_STKSZ (PAGE_SIZE << EXCEPTION_STACK_ORDER)

#define DEBUG_STACK_ORDER (EXCEPTION_STACK_ORDER + 1)
#define DEBUG_STKSZ (PAGE_SIZE << DEBUG_STACK_ORDER)

<<<<<<< HEAD
#define IRQ_STACK_ORDER 2
=======
#define IRQ_STACK_ORDER (2 + KASAN_STACK_ORDER)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define IRQ_STACK_SIZE (PAGE_SIZE << IRQ_STACK_ORDER)

#define DOUBLEFAULT_STACK 1
#define NMI_STACK 2
#define DEBUG_STACK 3
#define MCE_STACK 4
#define N_EXCEPTION_STACKS 4  /* hw limit: 7 */

<<<<<<< HEAD
#define PUD_PAGE_SIZE		(_AC(1, UL) << PUD_SHIFT)
#define PUD_PAGE_MASK		(~(PUD_PAGE_SIZE-1))

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Set __PAGE_OFFSET to the most negative possible address +
 * PGDIR_SIZE*16 (pgd slot 272).  The gap is to allow a space for a
 * hypervisor to fit.  Choosing 16 slots here is arbitrary, but it's
 * what Xen requires.
 */
<<<<<<< HEAD
#define __PAGE_OFFSET           _AC(0xffff880000000000, UL)

#define __PHYSICAL_START	((CONFIG_PHYSICAL_START +	 	\
				  (CONFIG_PHYSICAL_ALIGN - 1)) &	\
				 ~(CONFIG_PHYSICAL_ALIGN - 1))

#define __START_KERNEL		(__START_KERNEL_map + __PHYSICAL_START)
=======
#define __PAGE_OFFSET_BASE      _AC(0xffff880000000000, UL)
#ifdef CONFIG_RANDOMIZE_MEMORY
#define __PAGE_OFFSET           page_offset_base
#else
#define __PAGE_OFFSET           __PAGE_OFFSET_BASE
#endif /* CONFIG_RANDOMIZE_MEMORY */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define __START_KERNEL_map	_AC(0xffffffff80000000, UL)

/* See Documentation/x86/x86_64/mm.txt for a description of the memory map. */
#define __PHYSICAL_MASK_SHIFT	46
#define __VIRTUAL_MASK_SHIFT	47

/*
<<<<<<< HEAD
 * Kernel image size is limited to 512 MB (see level2_kernel_pgt in
 * arch/x86/kernel/head_64.S), and it is mapped here:
 */
#define KERNEL_IMAGE_SIZE	(512 * 1024 * 1024)
=======
 * Kernel image size is limited to 1GiB due to the fixmap living in the
 * next 1GiB (see level2_kernel_pgt in arch/x86/kernel/head_64.S). Use
 * 512MiB by default, leaving 1.5GiB for modules once the page tables
 * are fully set up. If kernel ASLR is configured, it can extend the
 * kernel page table mapping, reducing the size of the modules area.
 */
#if defined(CONFIG_RANDOMIZE_BASE)
#define KERNEL_IMAGE_SIZE	(1024 * 1024 * 1024)
#else
#define KERNEL_IMAGE_SIZE	(512 * 1024 * 1024)
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _ASM_X86_PAGE_64_DEFS_H */
