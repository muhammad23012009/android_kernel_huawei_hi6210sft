/*
 * include/asm-xtensa/io.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 */

#ifndef _XTENSA_IO_H
#define _XTENSA_IO_H

#ifdef __KERNEL__
#include <asm/byteorder.h>
#include <asm/page.h>
<<<<<<< HEAD
=======
#include <asm/vectors.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/bug.h>
#include <linux/kernel.h>

#include <linux/types.h>

<<<<<<< HEAD
#define XCHAL_KIO_CACHED_VADDR	0xe0000000
#define XCHAL_KIO_BYPASS_VADDR	0xf0000000
#define XCHAL_KIO_PADDR		0xf0000000
#define XCHAL_KIO_SIZE		0x10000000

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define IOADDR(x)		(XCHAL_KIO_BYPASS_VADDR + (x))
#define IO_SPACE_LIMIT ~0

#ifdef CONFIG_MMU
<<<<<<< HEAD
/*
 * Return the virtual address for the specified bus memory.
 * Note that we currently don't support any address outside the KIO segment.
=======

void __iomem *xtensa_ioremap_nocache(unsigned long addr, unsigned long size);
void __iomem *xtensa_ioremap_cache(unsigned long addr, unsigned long size);
void xtensa_iounmap(volatile void __iomem *addr);

/*
 * Return the virtual address for the specified bus memory.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static inline void __iomem *ioremap_nocache(unsigned long offset,
		unsigned long size)
{
	if (offset >= XCHAL_KIO_PADDR
	    && offset - XCHAL_KIO_PADDR < XCHAL_KIO_SIZE)
		return (void*)(offset-XCHAL_KIO_PADDR+XCHAL_KIO_BYPASS_VADDR);
	else
<<<<<<< HEAD
		BUG();
=======
		return xtensa_ioremap_nocache(offset, size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void __iomem *ioremap_cache(unsigned long offset,
		unsigned long size)
{
	if (offset >= XCHAL_KIO_PADDR
	    && offset - XCHAL_KIO_PADDR < XCHAL_KIO_SIZE)
		return (void*)(offset-XCHAL_KIO_PADDR+XCHAL_KIO_CACHED_VADDR);
	else
<<<<<<< HEAD
		BUG();
}

#define ioremap_wc ioremap_nocache
=======
		return xtensa_ioremap_cache(offset, size);
}
#define ioremap_cache ioremap_cache

#define ioremap_wc ioremap_nocache
#define ioremap_wt ioremap_nocache
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline void __iomem *ioremap(unsigned long offset, unsigned long size)
{
	return ioremap_nocache(offset, size);
}

static inline void iounmap(volatile void __iomem *addr)
{
<<<<<<< HEAD
=======
	unsigned long va = (unsigned long) addr;

	if (!(va >= XCHAL_KIO_CACHED_VADDR &&
	      va - XCHAL_KIO_CACHED_VADDR < XCHAL_KIO_SIZE) &&
	    !(va >= XCHAL_KIO_BYPASS_VADDR &&
	      va - XCHAL_KIO_BYPASS_VADDR < XCHAL_KIO_SIZE))
		xtensa_iounmap(addr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#define virt_to_bus     virt_to_phys
#define bus_to_virt     phys_to_virt

#endif /* CONFIG_MMU */

<<<<<<< HEAD
/*
 * Generic I/O
 */
#define readb_relaxed readb
#define readw_relaxed readw
#define readl_relaxed readl

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif	/* __KERNEL__ */

#include <asm-generic/io.h>

#endif	/* _XTENSA_IO_H */
