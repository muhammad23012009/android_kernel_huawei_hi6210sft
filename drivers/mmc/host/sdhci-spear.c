/*
 * drivers/mmc/host/sdhci-spear.c
 *
 * Support of SDHCI platform devices for spear soc family
 *
 * Copyright (C) 2010 ST Microelectronics
<<<<<<< HEAD
 * Viresh Kumar <viresh.linux@gmail.com>
=======
 * Viresh Kumar <vireshk@kernel.org>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Inspired by sdhci-pltfm.c
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/highmem.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/mmc/host.h>
<<<<<<< HEAD
#include <linux/mmc/sdhci-spear.h>
=======
#include <linux/mmc/slot-gpio.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/io.h>
#include "sdhci.h"

struct spear_sdhci {
	struct clk *clk;
<<<<<<< HEAD
	struct sdhci_plat_data *data;
=======
	int card_int_gpio;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* sdhci ops */
static const struct sdhci_ops sdhci_pltfm_ops = {
<<<<<<< HEAD
	/* Nothing to do for now. */
};

/* gpio card detection interrupt handler */
static irqreturn_t sdhci_gpio_irq(int irq, void *dev_id)
{
	struct platform_device *pdev = dev_id;
	struct sdhci_host *host = platform_get_drvdata(pdev);
	struct spear_sdhci *sdhci = dev_get_platdata(&pdev->dev);
	unsigned long gpio_irq_type;
	int val;

	val = gpio_get_value(sdhci->data->card_int_gpio);

	/* val == 1 -> card removed, val == 0 -> card inserted */
	/* if card removed - set irq for low level, else vice versa */
	gpio_irq_type = val ? IRQF_TRIGGER_LOW : IRQF_TRIGGER_HIGH;
	irq_set_irq_type(irq, gpio_irq_type);

	if (sdhci->data->card_power_gpio >= 0) {
		if (!sdhci->data->power_always_enb) {
			/* if card inserted, give power, otherwise remove it */
			val = sdhci->data->power_active_high ? !val : val ;
			gpio_set_value(sdhci->data->card_power_gpio, val);
		}
	}

	/* inform sdhci driver about card insertion/removal */
	tasklet_schedule(&host->card_tasklet);

	return IRQ_HANDLED;
}

#ifdef CONFIG_OF
static struct sdhci_plat_data *sdhci_probe_config_dt(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct sdhci_plat_data *pdata = NULL;
=======
	.set_clock = sdhci_set_clock,
	.set_bus_width = sdhci_set_bus_width,
	.reset = sdhci_reset,
	.set_uhs_signaling = sdhci_set_uhs_signaling,
};

static void sdhci_probe_config_dt(struct device_node *np,
				struct spear_sdhci *host)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int cd_gpio;

	cd_gpio = of_get_named_gpio(np, "cd-gpios", 0);
	if (!gpio_is_valid(cd_gpio))
		cd_gpio = -1;

<<<<<<< HEAD
	/* If pdata is required */
	if (cd_gpio != -1) {
		pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata) {
			dev_err(&pdev->dev, "DT: kzalloc failed\n");
			return ERR_PTR(-ENOMEM);
		}
	}

	pdata->card_int_gpio = cd_gpio;

	return pdata;
}
#else
static struct sdhci_plat_data *sdhci_probe_config_dt(struct platform_device *pdev)
{
	return ERR_PTR(-ENOSYS);
}
#endif

static int sdhci_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct sdhci_host *host;
	struct resource *iomem;
	struct spear_sdhci *sdhci;
	int ret;

	iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!iomem) {
		ret = -ENOMEM;
		dev_dbg(&pdev->dev, "memory resource not defined\n");
		goto err;
	}

	if (!devm_request_mem_region(&pdev->dev, iomem->start,
				resource_size(iomem), "spear-sdhci")) {
		ret = -EBUSY;
		dev_dbg(&pdev->dev, "cannot request region\n");
		goto err;
	}

	sdhci = devm_kzalloc(&pdev->dev, sizeof(*sdhci), GFP_KERNEL);
	if (!sdhci) {
		ret = -ENOMEM;
		dev_dbg(&pdev->dev, "cannot allocate memory for sdhci\n");
		goto err;
	}

	/* clk enable */
	sdhci->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(sdhci->clk)) {
		ret = PTR_ERR(sdhci->clk);
		dev_dbg(&pdev->dev, "Error getting clock\n");
		goto err;
=======
	host->card_int_gpio = cd_gpio;
}

static int sdhci_probe(struct platform_device *pdev)
{
	struct sdhci_host *host;
	struct resource *iomem;
	struct spear_sdhci *sdhci;
	struct device *dev;
	int ret;

	dev = pdev->dev.parent ? pdev->dev.parent : &pdev->dev;
	host = sdhci_alloc_host(dev, sizeof(*sdhci));
	if (IS_ERR(host)) {
		ret = PTR_ERR(host);
		dev_dbg(&pdev->dev, "cannot allocate memory for sdhci\n");
		goto err;
	}

	iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	host->ioaddr = devm_ioremap_resource(&pdev->dev, iomem);
	if (IS_ERR(host->ioaddr)) {
		ret = PTR_ERR(host->ioaddr);
		dev_dbg(&pdev->dev, "unable to map iomem: %d\n", ret);
		goto err_host;
	}

	host->hw_name = "sdhci";
	host->ops = &sdhci_pltfm_ops;
	host->irq = platform_get_irq(pdev, 0);
	host->quirks = SDHCI_QUIRK_BROKEN_ADMA;

	sdhci = sdhci_priv(host);

	/* clk enable */
	sdhci->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(sdhci->clk)) {
		ret = PTR_ERR(sdhci->clk);
		dev_dbg(&pdev->dev, "Error getting clock\n");
		goto err_host;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	ret = clk_prepare_enable(sdhci->clk);
	if (ret) {
		dev_dbg(&pdev->dev, "Error enabling clock\n");
<<<<<<< HEAD
		goto put_clk;
=======
		goto err_host;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	ret = clk_set_rate(sdhci->clk, 50000000);
	if (ret)
		dev_dbg(&pdev->dev, "Error setting desired clk, clk=%lu\n",
				clk_get_rate(sdhci->clk));

<<<<<<< HEAD
	if (np) {
		sdhci->data = sdhci_probe_config_dt(pdev);
		if (IS_ERR(sdhci->data)) {
			dev_err(&pdev->dev, "DT: Failed to get pdata\n");
			return -ENODEV;
		}
	} else {
		sdhci->data = dev_get_platdata(&pdev->dev);
	}

	pdev->dev.platform_data = sdhci;

	if (pdev->dev.parent)
		host = sdhci_alloc_host(pdev->dev.parent, 0);
	else
		host = sdhci_alloc_host(&pdev->dev, 0);

	if (IS_ERR(host)) {
		ret = PTR_ERR(host);
		dev_dbg(&pdev->dev, "error allocating host\n");
		goto disable_clk;
	}

	host->hw_name = "sdhci";
	host->ops = &sdhci_pltfm_ops;
	host->irq = platform_get_irq(pdev, 0);
	host->quirks = SDHCI_QUIRK_BROKEN_ADMA;

	host->ioaddr = devm_ioremap(&pdev->dev, iomem->start,
			resource_size(iomem));
	if (!host->ioaddr) {
		ret = -ENOMEM;
		dev_dbg(&pdev->dev, "failed to remap registers\n");
		goto free_host;
=======
	sdhci_probe_config_dt(pdev->dev.of_node, sdhci);
	/*
	 * It is optional to use GPIOs for sdhci card detection. If
	 * sdhci->card_int_gpio < 0, then use original sdhci lines otherwise
	 * GPIO lines. We use the built-in GPIO support for this.
	 */
	if (sdhci->card_int_gpio >= 0) {
		ret = mmc_gpio_request_cd(host->mmc, sdhci->card_int_gpio, 0);
		if (ret < 0) {
			dev_dbg(&pdev->dev,
				"failed to request card-detect gpio%d\n",
				sdhci->card_int_gpio);
			goto disable_clk;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	ret = sdhci_add_host(host);
	if (ret) {
		dev_dbg(&pdev->dev, "error adding host\n");
<<<<<<< HEAD
		goto free_host;
=======
		goto disable_clk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	platform_set_drvdata(pdev, host);

<<<<<<< HEAD
	/*
	 * It is optional to use GPIOs for sdhci Power control & sdhci card
	 * interrupt detection. If sdhci->data is NULL, then use original sdhci
	 * lines otherwise GPIO lines.
	 * If GPIO is selected for power control, then power should be disabled
	 * after card removal and should be enabled when card insertion
	 * interrupt occurs
	 */
	if (!sdhci->data)
		return 0;

	if (sdhci->data->card_power_gpio >= 0) {
		int val = 0;

		ret = devm_gpio_request(&pdev->dev,
				sdhci->data->card_power_gpio, "sdhci");
		if (ret < 0) {
			dev_dbg(&pdev->dev, "gpio request fail: %d\n",
					sdhci->data->card_power_gpio);
			goto set_drvdata;
		}

		if (sdhci->data->power_always_enb)
			val = sdhci->data->power_active_high;
		else
			val = !sdhci->data->power_active_high;

		ret = gpio_direction_output(sdhci->data->card_power_gpio, val);
		if (ret) {
			dev_dbg(&pdev->dev, "gpio set direction fail: %d\n",
					sdhci->data->card_power_gpio);
			goto set_drvdata;
		}
	}

	if (sdhci->data->card_int_gpio >= 0) {
		ret = devm_gpio_request(&pdev->dev, sdhci->data->card_int_gpio,
				"sdhci");
		if (ret < 0) {
			dev_dbg(&pdev->dev, "gpio request fail: %d\n",
					sdhci->data->card_int_gpio);
			goto set_drvdata;
		}

		ret = gpio_direction_input(sdhci->data->card_int_gpio);
		if (ret) {
			dev_dbg(&pdev->dev, "gpio set direction fail: %d\n",
					sdhci->data->card_int_gpio);
			goto set_drvdata;
		}
		ret = devm_request_irq(&pdev->dev,
				gpio_to_irq(sdhci->data->card_int_gpio),
				sdhci_gpio_irq, IRQF_TRIGGER_LOW,
				mmc_hostname(host->mmc), pdev);
		if (ret) {
			dev_dbg(&pdev->dev, "gpio request irq fail: %d\n",
					sdhci->data->card_int_gpio);
			goto set_drvdata;
		}

	}

	return 0;

set_drvdata:
	platform_set_drvdata(pdev, NULL);
	sdhci_remove_host(host, 1);
free_host:
	sdhci_free_host(host);
disable_clk:
	clk_disable_unprepare(sdhci->clk);
put_clk:
	clk_put(sdhci->clk);
=======
	return 0;

disable_clk:
	clk_disable_unprepare(sdhci->clk);
err_host:
	sdhci_free_host(host);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
err:
	dev_err(&pdev->dev, "spear-sdhci probe failed: %d\n", ret);
	return ret;
}

static int sdhci_remove(struct platform_device *pdev)
{
	struct sdhci_host *host = platform_get_drvdata(pdev);
<<<<<<< HEAD
	struct spear_sdhci *sdhci = dev_get_platdata(&pdev->dev);
	int dead = 0;
	u32 scratch;

	platform_set_drvdata(pdev, NULL);
=======
	struct spear_sdhci *sdhci = sdhci_priv(host);
	int dead = 0;
	u32 scratch;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	scratch = readl(host->ioaddr + SDHCI_INT_STATUS);
	if (scratch == (u32)-1)
		dead = 1;

	sdhci_remove_host(host, dead);
<<<<<<< HEAD
	sdhci_free_host(host);
	clk_disable_unprepare(sdhci->clk);
	clk_put(sdhci->clk);
=======
	clk_disable_unprepare(sdhci->clk);
	sdhci_free_host(host);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int sdhci_suspend(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct spear_sdhci *sdhci = dev_get_platdata(dev);
=======
	struct spear_sdhci *sdhci = sdhci_priv(host);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	ret = sdhci_suspend_host(host);
	if (!ret)
		clk_disable(sdhci->clk);

	return ret;
}

static int sdhci_resume(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct spear_sdhci *sdhci = dev_get_platdata(dev);
=======
	struct spear_sdhci *sdhci = sdhci_priv(host);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	ret = clk_enable(sdhci->clk);
	if (ret) {
		dev_dbg(dev, "Resume: Error enabling clock\n");
		return ret;
	}

	return sdhci_resume_host(host);
}
#endif

static SIMPLE_DEV_PM_OPS(sdhci_pm_ops, sdhci_suspend, sdhci_resume);

#ifdef CONFIG_OF
static const struct of_device_id sdhci_spear_id_table[] = {
	{ .compatible = "st,spear300-sdhci" },
	{}
};
MODULE_DEVICE_TABLE(of, sdhci_spear_id_table);
#endif

static struct platform_driver sdhci_driver = {
	.driver = {
		.name	= "sdhci",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.pm	= &sdhci_pm_ops,
		.of_match_table = of_match_ptr(sdhci_spear_id_table),
	},
	.probe		= sdhci_probe,
	.remove		= sdhci_remove,
};

module_platform_driver(sdhci_driver);

MODULE_DESCRIPTION("SPEAr Secure Digital Host Controller Interface driver");
<<<<<<< HEAD
MODULE_AUTHOR("Viresh Kumar <viresh.linux@gmail.com>");
=======
MODULE_AUTHOR("Viresh Kumar <vireshk@kernel.org>");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_LICENSE("GPL v2");
