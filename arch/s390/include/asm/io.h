/*
 *  S390 version
 *    Copyright IBM Corp. 1999
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 *  Derived from "include/asm-i386/io.h"
 */

#ifndef _S390_IO_H
#define _S390_IO_H

#include <linux/kernel.h>
#include <asm/page.h>
#include <asm/pci_io.h>

<<<<<<< HEAD
/*
 * Change virtual addresses to physical addresses and vv.
 * These are pretty trivial
 */
static inline unsigned long virt_to_phys(volatile void * address)
{
	unsigned long real_address;
	asm volatile(
		 "	lra	%0,0(%1)\n"
		 "	jz	0f\n"
		 "	la	%0,0\n"
		 "0:"
		 : "=a" (real_address) : "a" (address) : "cc");
	return real_address;
}
#define virt_to_phys virt_to_phys

static inline void * phys_to_virt(unsigned long address)
{
	return (void *) address;
}

void *xlate_dev_mem_ptr(unsigned long phys);
#define xlate_dev_mem_ptr xlate_dev_mem_ptr
void unxlate_dev_mem_ptr(unsigned long phys, void *addr);
=======
#define xlate_dev_mem_ptr xlate_dev_mem_ptr
void *xlate_dev_mem_ptr(phys_addr_t phys);
#define unxlate_dev_mem_ptr unxlate_dev_mem_ptr
void unxlate_dev_mem_ptr(phys_addr_t phys, void *addr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Convert a virtual cached pointer to an uncached pointer
 */
#define xlate_dev_kmem_ptr(p)	p

#define IO_SPACE_LIMIT 0

#ifdef CONFIG_PCI

#define ioremap_nocache(addr, size)	ioremap(addr, size)
#define ioremap_wc			ioremap_nocache
<<<<<<< HEAD
=======
#define ioremap_wt			ioremap_nocache
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline void __iomem *ioremap(unsigned long offset, unsigned long size)
{
	return (void __iomem *) offset;
}

static inline void iounmap(volatile void __iomem *addr)
{
}

<<<<<<< HEAD
=======
static inline void __iomem *ioport_map(unsigned long port, unsigned int nr)
{
	return NULL;
}

static inline void ioport_unmap(void __iomem *p)
{
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * s390 needs a private implementation of pci_iomap since ioremap with its
 * offset parameter isn't sufficient. That's because BAR spaces are not
 * disjunctive on s390 so we need the bar parameter of pci_iomap to find
 * the corresponding device and create the mapping cookie.
 */
#define pci_iomap pci_iomap
#define pci_iounmap pci_iounmap
<<<<<<< HEAD
=======
#define pci_iomap_wc pci_iomap
#define pci_iomap_wc_range pci_iomap_range
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define memcpy_fromio(dst, src, count)	zpci_memcpy_fromio(dst, src, count)
#define memcpy_toio(dst, src, count)	zpci_memcpy_toio(dst, src, count)
#define memset_io(dst, val, count)	zpci_memset_io(dst, val, count)

#define __raw_readb	zpci_read_u8
#define __raw_readw	zpci_read_u16
#define __raw_readl	zpci_read_u32
#define __raw_readq	zpci_read_u64
#define __raw_writeb	zpci_write_u8
#define __raw_writew	zpci_write_u16
#define __raw_writel	zpci_write_u32
#define __raw_writeq	zpci_write_u64

<<<<<<< HEAD
#define readb_relaxed	readb
#define readw_relaxed	readw
#define readl_relaxed	readl
#define readq_relaxed	readq

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* CONFIG_PCI */

#include <asm-generic/io.h>

#endif
