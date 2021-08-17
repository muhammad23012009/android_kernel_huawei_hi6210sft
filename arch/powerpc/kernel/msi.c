/*
 * Copyright 2006-2007, Michael Ellerman, IBM Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/msi.h>
#include <linux/pci.h>

#include <asm/machdep.h>

<<<<<<< HEAD
int arch_msi_check_device(struct pci_dev* dev, int nvec, int type)
{
	if (!ppc_md.setup_msi_irqs || !ppc_md.teardown_msi_irqs) {
=======
int arch_setup_msi_irqs(struct pci_dev *dev, int nvec, int type)
{
	struct pci_controller *phb = pci_bus_to_host(dev->bus);

	if (!phb->controller_ops.setup_msi_irqs ||
	    !phb->controller_ops.teardown_msi_irqs) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		pr_debug("msi: Platform doesn't provide MSI callbacks.\n");
		return -ENOSYS;
	}

	/* PowerPC doesn't support multiple MSI yet */
	if (type == PCI_CAP_ID_MSI && nvec > 1)
		return 1;

<<<<<<< HEAD
	if (ppc_md.msi_check_device) {
		pr_debug("msi: Using platform check routine.\n");
		return ppc_md.msi_check_device(dev, nvec, type);
	}

        return 0;
}

int arch_setup_msi_irqs(struct pci_dev *dev, int nvec, int type)
{
	return ppc_md.setup_msi_irqs(dev, nvec, type);
=======
	return phb->controller_ops.setup_msi_irqs(dev, nvec, type);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void arch_teardown_msi_irqs(struct pci_dev *dev)
{
<<<<<<< HEAD
	ppc_md.teardown_msi_irqs(dev);
=======
	struct pci_controller *phb = pci_bus_to_host(dev->bus);

	/*
	 * We can be called even when arch_setup_msi_irqs() returns -ENOSYS,
	 * so check the pointer again.
	 */
	if (phb->controller_ops.teardown_msi_irqs)
		phb->controller_ops.teardown_msi_irqs(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
