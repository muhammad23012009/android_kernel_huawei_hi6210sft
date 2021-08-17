/*
 * asm/leon_pci.h
 *
 * Copyright (C) 2011 Aeroflex Gaisler AB, Daniel Hellstrom
 */

#ifndef _ASM_LEON_PCI_H_
#define _ASM_LEON_PCI_H_

/* PCI related definitions */
struct leon_pci_info {
	struct pci_ops *ops;
	struct resource	io_space;
	struct resource	mem_space;
	struct resource	busn;
	int (*map_irq)(const struct pci_dev *dev, u8 slot, u8 pin);
};

<<<<<<< HEAD
extern void leon_pci_init(struct platform_device *ofdev,
				struct leon_pci_info *info);
=======
void leon_pci_init(struct platform_device *ofdev,
		   struct leon_pci_info *info);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _ASM_LEON_PCI_H_ */
