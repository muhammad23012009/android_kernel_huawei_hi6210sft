/*
 * Synopsys DesignWare Multimedia Card Interface driver
 *
 * Copyright (C) 2009 NXP Semiconductors
 * Copyright (C) 2009, 2010 Imagination Technologies Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/dw_mmc.h>
#include <linux/of.h>
<<<<<<< HEAD

#include "dw_mmc.h"
#ifdef CONFIG_ARCH_HI6XXX
#include <linux/pwrctrl_power_state_manager.h>
#endif

int dw_mci_pltfm_register(struct platform_device *pdev,
				const struct dw_mci_drv_data *drv_data)
{
	struct dw_mci *host;
	struct resource	*regs;
	int ret;
=======
#include <linux/clk.h>

#include "dw_mmc.h"
#include "dw_mmc-pltfm.h"

int dw_mci_pltfm_register(struct platform_device *pdev,
			  const struct dw_mci_drv_data *drv_data)
{
	struct dw_mci *host;
	struct resource	*regs;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	host = devm_kzalloc(&pdev->dev, sizeof(struct dw_mci), GFP_KERNEL);
	if (!host)
		return -ENOMEM;

<<<<<<< HEAD
	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!regs)
		return -ENXIO;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	host->irq = platform_get_irq(pdev, 0);
	if (host->irq < 0)
		return host->irq;

<<<<<<< HEAD
#ifdef CONFIG_ARCH_HI6XXX
      host->emmc_pwr_status_id = PWRCTRL_POWER_STAT_INVALID_ID;
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	host->drv_data = drv_data;
	host->dev = &pdev->dev;
	host->irq_flags = 0;
	host->pdata = pdev->dev.platform_data;
<<<<<<< HEAD
=======

	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	host->regs = devm_ioremap_resource(&pdev->dev, regs);
	if (IS_ERR(host->regs))
		return PTR_ERR(host->regs);

<<<<<<< HEAD
    /*私有IP数据初始化*/
	if (drv_data && drv_data->init) {
		ret = drv_data->init(host);
		if (ret)
			return ret;
	}

	platform_set_drvdata(pdev, host);
	
	ret = dw_mci_probe(host);
	return ret;
}
EXPORT_SYMBOL_GPL(dw_mci_pltfm_register);

static int dw_mci_pltfm_probe(struct platform_device *pdev)
{
	return dw_mci_pltfm_register(pdev, NULL);
}

int dw_mci_pltfm_remove(struct platform_device *pdev)
{
	struct dw_mci *host = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);
	dw_mci_remove(host);
	return 0;
}
EXPORT_SYMBOL_GPL(dw_mci_pltfm_remove);

=======
	/* Get registers' physical base address */
	host->phy_regs = regs->start;

	platform_set_drvdata(pdev, host);
	return dw_mci_probe(host);
}
EXPORT_SYMBOL_GPL(dw_mci_pltfm_register);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_PM_SLEEP
/*
 * TODO: we should probably disable the clock to the card in the suspend path.
 */
static int dw_mci_pltfm_suspend(struct device *dev)
{
<<<<<<< HEAD
	int ret;
	struct dw_mci *host = dev_get_drvdata(dev);

	ret = dw_mci_suspend(host);
	if (ret)
		return ret;

	return 0;
=======
	struct dw_mci *host = dev_get_drvdata(dev);

	return dw_mci_suspend(host);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int dw_mci_pltfm_resume(struct device *dev)
{
<<<<<<< HEAD
	int ret;
	struct dw_mci *host = dev_get_drvdata(dev);

	ret = dw_mci_resume(host);
	if (ret)
		return ret;

	return 0;
}
#else
#define dw_mci_pltfm_suspend	NULL
#define dw_mci_pltfm_resume	NULL
=======
	struct dw_mci *host = dev_get_drvdata(dev);

	return dw_mci_resume(host);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* CONFIG_PM_SLEEP */

SIMPLE_DEV_PM_OPS(dw_mci_pltfm_pmops, dw_mci_pltfm_suspend, dw_mci_pltfm_resume);
EXPORT_SYMBOL_GPL(dw_mci_pltfm_pmops);

static const struct of_device_id dw_mci_pltfm_match[] = {
	{ .compatible = "snps,dw-mshc", },
<<<<<<< HEAD
=======
	{ .compatible = "altr,socfpga-dw-mshc", },
	{ .compatible = "img,pistachio-dw-mshc", },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{},
};
MODULE_DEVICE_TABLE(of, dw_mci_pltfm_match);

<<<<<<< HEAD
=======
static int dw_mci_pltfm_probe(struct platform_device *pdev)
{
	const struct dw_mci_drv_data *drv_data = NULL;
	const struct of_device_id *match;

	if (pdev->dev.of_node) {
		match = of_match_node(dw_mci_pltfm_match, pdev->dev.of_node);
		drv_data = match->data;
	}

	return dw_mci_pltfm_register(pdev, drv_data);
}

int dw_mci_pltfm_remove(struct platform_device *pdev)
{
	struct dw_mci *host = platform_get_drvdata(pdev);

	dw_mci_remove(host);
	return 0;
}
EXPORT_SYMBOL_GPL(dw_mci_pltfm_remove);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct platform_driver dw_mci_pltfm_driver = {
	.probe		= dw_mci_pltfm_probe,
	.remove		= dw_mci_pltfm_remove,
	.driver		= {
		.name		= "dw_mmc",
<<<<<<< HEAD
		.of_match_table	= of_match_ptr(dw_mci_pltfm_match),
=======
		.of_match_table	= dw_mci_pltfm_match,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.pm		= &dw_mci_pltfm_pmops,
	},
};

module_platform_driver(dw_mci_pltfm_driver);

MODULE_DESCRIPTION("DW Multimedia Card Interface driver");
MODULE_AUTHOR("NXP Semiconductor VietNam");
MODULE_AUTHOR("Imagination Technologies Ltd");
MODULE_LICENSE("GPL v2");
