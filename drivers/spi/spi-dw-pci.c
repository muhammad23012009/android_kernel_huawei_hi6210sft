/*
 * PCI interface driver for DW SPI Core
 *
<<<<<<< HEAD
 * Copyright (c) 2009, Intel Corporation.
=======
 * Copyright (c) 2009, 2014 Intel Corporation.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
<<<<<<< HEAD
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/spi/spi.h>
#include <linux/module.h>

#include "spi-dw.h"

#define DRIVER_NAME "dw_spi_pci"

<<<<<<< HEAD
struct dw_spi_pci {
	struct pci_dev	*pdev;
	struct dw_spi	dws;
};

static int spi_pci_probe(struct pci_dev *pdev,
	const struct pci_device_id *ent)
{
	struct dw_spi_pci *dwpci;
	struct dw_spi *dws;
	int pci_bar = 0;
	int ret;

	printk(KERN_INFO "DW: found PCI SPI controller(ID: %04x:%04x)\n",
		pdev->vendor, pdev->device);

	ret = pci_enable_device(pdev);
	if (ret)
		return ret;

	dwpci = kzalloc(sizeof(struct dw_spi_pci), GFP_KERNEL);
	if (!dwpci) {
		ret = -ENOMEM;
		goto err_disable;
	}

	dwpci->pdev = pdev;
	dws = &dwpci->dws;

	/* Get basic io resource and map it */
	dws->paddr = pci_resource_start(pdev, pci_bar);
	dws->iolen = pci_resource_len(pdev, pci_bar);

	ret = pci_request_region(pdev, pci_bar, dev_name(&pdev->dev));
	if (ret)
		goto err_kfree;

	dws->regs = ioremap_nocache((unsigned long)dws->paddr,
				pci_resource_len(pdev, pci_bar));
	if (!dws->regs) {
		ret = -ENOMEM;
		goto err_release_reg;
	}

	dws->parent_dev = &pdev->dev;
	dws->bus_num = 0;
	dws->num_cs = 4;
	dws->irq = pdev->irq;

	/*
	 * Specific handling for Intel MID paltforms, like dma setup,
	 * clock rate, FIFO depth.
	 */
	if (pdev->device == 0x0800) {
		ret = dw_spi_mid_init(dws);
		if (ret)
			goto err_unmap;
	}

	ret = dw_spi_add_host(dws);
	if (ret)
		goto err_unmap;

	/* PCI hook and SPI hook use the same drv data */
	pci_set_drvdata(pdev, dwpci);
	return 0;

err_unmap:
	iounmap(dws->regs);
err_release_reg:
	pci_release_region(pdev, pci_bar);
err_kfree:
	kfree(dwpci);
err_disable:
	pci_disable_device(pdev);
	return ret;
=======
struct spi_pci_desc {
	int	(*setup)(struct dw_spi *);
	u16	num_cs;
	u16	bus_num;
};

static struct spi_pci_desc spi_pci_mid_desc_1 = {
	.setup = dw_spi_mid_init,
	.num_cs = 5,
	.bus_num = 0,
};

static struct spi_pci_desc spi_pci_mid_desc_2 = {
	.setup = dw_spi_mid_init,
	.num_cs = 2,
	.bus_num = 1,
};

static int spi_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct dw_spi *dws;
	struct spi_pci_desc *desc = (struct spi_pci_desc *)ent->driver_data;
	int pci_bar = 0;
	int ret;

	ret = pcim_enable_device(pdev);
	if (ret)
		return ret;

	dws = devm_kzalloc(&pdev->dev, sizeof(*dws), GFP_KERNEL);
	if (!dws)
		return -ENOMEM;

	/* Get basic io resource and map it */
	dws->paddr = pci_resource_start(pdev, pci_bar);

	ret = pcim_iomap_regions(pdev, 1 << pci_bar, pci_name(pdev));
	if (ret)
		return ret;

	dws->regs = pcim_iomap_table(pdev)[pci_bar];
	dws->irq = pdev->irq;

	/*
	 * Specific handling for platforms, like dma setup,
	 * clock rate, FIFO depth.
	 */
	if (desc) {
		dws->num_cs = desc->num_cs;
		dws->bus_num = desc->bus_num;

		if (desc->setup) {
			ret = desc->setup(dws);
			if (ret)
				return ret;
		}
	} else {
		return -ENODEV;
	}

	ret = dw_spi_add_host(&pdev->dev, dws);
	if (ret)
		return ret;

	/* PCI hook and SPI hook use the same drv data */
	pci_set_drvdata(pdev, dws);

	dev_info(&pdev->dev, "found PCI SPI controller(ID: %04x:%04x)\n",
		pdev->vendor, pdev->device);

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void spi_pci_remove(struct pci_dev *pdev)
{
<<<<<<< HEAD
	struct dw_spi_pci *dwpci = pci_get_drvdata(pdev);

	pci_set_drvdata(pdev, NULL);
	dw_spi_remove_host(&dwpci->dws);
	iounmap(dwpci->dws.regs);
	pci_release_region(pdev, 0);
	kfree(dwpci);
	pci_disable_device(pdev);
}

#ifdef CONFIG_PM
static int spi_suspend(struct pci_dev *pdev, pm_message_t state)
{
	struct dw_spi_pci *dwpci = pci_get_drvdata(pdev);
	int ret;

	ret = dw_spi_suspend_host(&dwpci->dws);
	if (ret)
		return ret;
	pci_save_state(pdev);
	pci_disable_device(pdev);
	pci_set_power_state(pdev, pci_choose_state(pdev, state));
	return ret;
}

static int spi_resume(struct pci_dev *pdev)
{
	struct dw_spi_pci *dwpci = pci_get_drvdata(pdev);
	int ret;

	pci_set_power_state(pdev, PCI_D0);
	pci_restore_state(pdev);
	ret = pci_enable_device(pdev);
	if (ret)
		return ret;
	return dw_spi_resume_host(&dwpci->dws);
}
#else
#define spi_suspend	NULL
#define spi_resume	NULL
#endif

static DEFINE_PCI_DEVICE_TABLE(pci_ids) = {
	/* Intel MID platform SPI controller 0 */
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x0800) },
=======
	struct dw_spi *dws = pci_get_drvdata(pdev);

	dw_spi_remove_host(dws);
}

#ifdef CONFIG_PM_SLEEP
static int spi_suspend(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct dw_spi *dws = pci_get_drvdata(pdev);

	return dw_spi_suspend_host(dws);
}

static int spi_resume(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct dw_spi *dws = pci_get_drvdata(pdev);

	return dw_spi_resume_host(dws);
}
#endif

static SIMPLE_DEV_PM_OPS(dw_spi_pm_ops, spi_suspend, spi_resume);

static const struct pci_device_id pci_ids[] = {
	/* Intel MID platform SPI controller 0 */
	/*
	 * The access to the device 8086:0801 is disabled by HW, since it's
	 * exclusively used by SCU to communicate with MSIC.
	 */
	/* Intel MID platform SPI controller 1 */
	{ PCI_VDEVICE(INTEL, 0x0800), (kernel_ulong_t)&spi_pci_mid_desc_1},
	/* Intel MID platform SPI controller 2 */
	{ PCI_VDEVICE(INTEL, 0x0812), (kernel_ulong_t)&spi_pci_mid_desc_2},
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{},
};

static struct pci_driver dw_spi_driver = {
	.name =		DRIVER_NAME,
	.id_table =	pci_ids,
	.probe =	spi_pci_probe,
	.remove =	spi_pci_remove,
<<<<<<< HEAD
	.suspend =	spi_suspend,
	.resume	=	spi_resume,
=======
	.driver         = {
		.pm     = &dw_spi_pm_ops,
	},
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

module_pci_driver(dw_spi_driver);

MODULE_AUTHOR("Feng Tang <feng.tang@intel.com>");
MODULE_DESCRIPTION("PCI interface driver for DW SPI Core");
MODULE_LICENSE("GPL v2");
