/*
 * Memory-mapped interface driver for DW SPI Core
 *
 * Copyright (c) 2010, Octasic semiconductor.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 */

#include <linux/clk.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spi/spi.h>
#include <linux/scatterlist.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/property.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "spi-dw.h"

#define DRIVER_NAME "dw_spi_mmio"

struct dw_spi_mmio {
	struct dw_spi  dws;
	struct clk     *clk;
};

static int dw_spi_mmio_probe(struct platform_device *pdev)
{
	struct dw_spi_mmio *dwsmmio;
	struct dw_spi *dws;
<<<<<<< HEAD
	struct resource *mem, *ioarea;
	int ret;

	dwsmmio = kzalloc(sizeof(struct dw_spi_mmio), GFP_KERNEL);
	if (!dwsmmio) {
		ret = -ENOMEM;
		goto err_end;
	}
=======
	struct resource *mem;
	int ret;
	int num_cs;

	dwsmmio = devm_kzalloc(&pdev->dev, sizeof(struct dw_spi_mmio),
			GFP_KERNEL);
	if (!dwsmmio)
		return -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	dws = &dwsmmio->dws;

	/* Get basic io resource and map it */
	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD
	if (!mem) {
		dev_err(&pdev->dev, "no mem resource?\n");
		ret = -EINVAL;
		goto err_kfree;
	}

	ioarea = request_mem_region(mem->start, resource_size(mem),
			pdev->name);
	if (!ioarea) {
		dev_err(&pdev->dev, "SPI region already claimed\n");
		ret = -EBUSY;
		goto err_kfree;
	}

	dws->regs = ioremap_nocache(mem->start, resource_size(mem));
	if (!dws->regs) {
		dev_err(&pdev->dev, "SPI region already mapped\n");
		ret = -ENOMEM;
		goto err_release_reg;
=======
	dws->regs = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(dws->regs)) {
		dev_err(&pdev->dev, "SPI region map failed\n");
		return PTR_ERR(dws->regs);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	dws->irq = platform_get_irq(pdev, 0);
	if (dws->irq < 0) {
		dev_err(&pdev->dev, "no irq resource?\n");
<<<<<<< HEAD
		ret = dws->irq; /* -ENXIO */
		goto err_unmap;
	}

	dwsmmio->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(dwsmmio->clk)) {
		ret = PTR_ERR(dwsmmio->clk);
		goto err_irq;
	}
	clk_enable(dwsmmio->clk);

	dws->parent_dev = &pdev->dev;
	dws->bus_num = 0;
	dws->num_cs = 4;
	dws->max_freq = clk_get_rate(dwsmmio->clk);

	ret = dw_spi_add_host(dws);
	if (ret)
		goto err_clk;
=======
		return dws->irq; /* -ENXIO */
	}

	dwsmmio->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(dwsmmio->clk))
		return PTR_ERR(dwsmmio->clk);
	ret = clk_prepare_enable(dwsmmio->clk);
	if (ret)
		return ret;

	dws->bus_num = pdev->id;

	dws->max_freq = clk_get_rate(dwsmmio->clk);

	device_property_read_u32(&pdev->dev, "reg-io-width", &dws->reg_io_width);

	num_cs = 4;

	device_property_read_u32(&pdev->dev, "num-cs", &num_cs);

	dws->num_cs = num_cs;

	if (pdev->dev.of_node) {
		int i;

		for (i = 0; i < dws->num_cs; i++) {
			int cs_gpio = of_get_named_gpio(pdev->dev.of_node,
					"cs-gpios", i);

			if (cs_gpio == -EPROBE_DEFER) {
				ret = cs_gpio;
				goto out;
			}

			if (gpio_is_valid(cs_gpio)) {
				ret = devm_gpio_request(&pdev->dev, cs_gpio,
						dev_name(&pdev->dev));
				if (ret)
					goto out;
			}
		}
	}

	ret = dw_spi_add_host(&pdev->dev, dws);
	if (ret)
		goto out;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	platform_set_drvdata(pdev, dwsmmio);
	return 0;

<<<<<<< HEAD
err_clk:
	clk_disable(dwsmmio->clk);
	clk_put(dwsmmio->clk);
	dwsmmio->clk = NULL;
err_irq:
	free_irq(dws->irq, dws);
err_unmap:
	iounmap(dws->regs);
err_release_reg:
	release_mem_region(mem->start, resource_size(mem));
err_kfree:
	kfree(dwsmmio);
err_end:
=======
out:
	clk_disable_unprepare(dwsmmio->clk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static int dw_spi_mmio_remove(struct platform_device *pdev)
{
	struct dw_spi_mmio *dwsmmio = platform_get_drvdata(pdev);
<<<<<<< HEAD
	struct resource *mem;

	platform_set_drvdata(pdev, NULL);

	clk_disable(dwsmmio->clk);
	clk_put(dwsmmio->clk);
	dwsmmio->clk = NULL;

	free_irq(dwsmmio->dws.irq, &dwsmmio->dws);
	dw_spi_remove_host(&dwsmmio->dws);
	iounmap(dwsmmio->dws.regs);
	kfree(dwsmmio);

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(mem->start, resource_size(mem));
	return 0;
}

=======

	dw_spi_remove_host(&dwsmmio->dws);
	clk_disable_unprepare(dwsmmio->clk);

	return 0;
}

static const struct of_device_id dw_spi_mmio_of_match[] = {
	{ .compatible = "snps,dw-apb-ssi", },
	{ /* end of table */}
};
MODULE_DEVICE_TABLE(of, dw_spi_mmio_of_match);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct platform_driver dw_spi_mmio_driver = {
	.probe		= dw_spi_mmio_probe,
	.remove		= dw_spi_mmio_remove,
	.driver		= {
		.name	= DRIVER_NAME,
<<<<<<< HEAD
		.owner	= THIS_MODULE,
=======
		.of_match_table = dw_spi_mmio_of_match,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
};
module_platform_driver(dw_spi_mmio_driver);

MODULE_AUTHOR("Jean-Hugues Deschenes <jean-hugues.deschenes@octasic.com>");
MODULE_DESCRIPTION("Memory-mapped I/O interface driver for DW SPI Core");
MODULE_LICENSE("GPL v2");
