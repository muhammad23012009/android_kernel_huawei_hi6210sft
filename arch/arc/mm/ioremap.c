/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/cache.h>

<<<<<<< HEAD
void __iomem *ioremap(unsigned long paddr, unsigned long size)
{
	unsigned long end;
=======
static inline bool arc_uncached_addr_space(phys_addr_t paddr)
{
	if (is_isa_arcompact()) {
		if (paddr >= ARC_UNCACHED_ADDR_SPACE)
			return true;
	} else if (paddr >= perip_base && paddr <= perip_end) {
		return true;
	}

	return false;
}

void __iomem *ioremap(phys_addr_t paddr, unsigned long size)
{
	phys_addr_t end;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Don't allow wraparound or zero size */
	end = paddr + size - 1;
	if (!size || (end < paddr))
		return NULL;

<<<<<<< HEAD
	/* If the region is h/w uncached, avoid MMU mappings */
	if (paddr >= ARC_UNCACHED_ADDR_SPACE)
		return (void __iomem *)paddr;
=======
	/*
	 * If the region is h/w uncached, MMU mapping can be elided as optim
	 * The cast to u32 is fine as this region can only be inside 4GB
	 */
	if (arc_uncached_addr_space(paddr))
		return (void __iomem *)(u32)paddr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ioremap_prot(paddr, size, PAGE_KERNEL_NO_CACHE);
}
EXPORT_SYMBOL(ioremap);

/*
 * ioremap with access flags
 * Cache semantics wise it is same as ioremap - "forced" uncached.
<<<<<<< HEAD
 * However unline vanilla ioremap which bypasses ARC MMU for addresses in
=======
 * However unlike vanilla ioremap which bypasses ARC MMU for addresses in
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * ARC hardware uncached region, this one still goes thru the MMU as caller
 * might need finer access control (R/W/X)
 */
void __iomem *ioremap_prot(phys_addr_t paddr, unsigned long size,
			   unsigned long flags)
{
<<<<<<< HEAD
	void __iomem *vaddr;
	struct vm_struct *area;
	unsigned long off, end;
=======
	unsigned long vaddr;
	struct vm_struct *area;
	phys_addr_t off, end;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pgprot_t prot = __pgprot(flags);

	/* Don't allow wraparound, zero size */
	end = paddr + size - 1;
	if ((!size) || (end < paddr))
		return NULL;

	/* An early platform driver might end up here */
	if (!slab_is_available())
		return NULL;

	/* force uncached */
	prot = pgprot_noncached(prot);

	/* Mappings have to be page-aligned */
	off = paddr & ~PAGE_MASK;
	paddr &= PAGE_MASK;
	size = PAGE_ALIGN(end + 1) - paddr;

	/*
	 * Ok, go for it..
	 */
	area = get_vm_area(size, VM_IOREMAP);
	if (!area)
		return NULL;
	area->phys_addr = paddr;
<<<<<<< HEAD
	vaddr = (void __iomem *)area->addr;
	if (ioremap_page_range((unsigned long)vaddr,
			       (unsigned long)vaddr + size, paddr, prot)) {
=======
	vaddr = (unsigned long)area->addr;
	if (ioremap_page_range(vaddr, vaddr + size, paddr, prot)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		vunmap((void __force *)vaddr);
		return NULL;
	}
	return (void __iomem *)(off + (char __iomem *)vaddr);
}
EXPORT_SYMBOL(ioremap_prot);


void iounmap(const void __iomem *addr)
{
<<<<<<< HEAD
	if (addr >= (void __force __iomem *)ARC_UNCACHED_ADDR_SPACE)
=======
	/* weird double cast to handle phys_addr_t > 32 bits */
	if (arc_uncached_addr_space((phys_addr_t)(u32)addr))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;

	vfree((void *)(PAGE_MASK & (unsigned long __force)addr));
}
EXPORT_SYMBOL(iounmap);
