/* Changed from asm-m68k version, Lineo Inc. 	May 2001	*/

#ifndef _ASM_BFIN_PCI_H
#define _ASM_BFIN_PCI_H

<<<<<<< HEAD
#include <asm/scatterlist.h>
#include <asm-generic/pci-dma-compat.h>
=======
#include <linux/scatterlist.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/pci.h>

#define PCIBIOS_MIN_IO 0x00001000
#define PCIBIOS_MIN_MEM 0x10000000

<<<<<<< HEAD
static inline void pcibios_penalize_isa_irq(int irq)
{
	/* We don't do dynamic PCI IRQ allocation */
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif				/* _ASM_BFIN_PCI_H */
