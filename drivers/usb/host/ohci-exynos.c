/*
 * SAMSUNG EXYNOS USB HOST OHCI Controller
 *
 * Copyright (C) 2011 Samsung Electronics Co.Ltd
 * Author: Jingoo Han <jg1.han@samsung.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/clk.h>
<<<<<<< HEAD
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/platform_data/usb-ohci-exynos.h>
#include <linux/usb/phy.h>
#include <linux/usb/samsung_usb_phy.h>

struct exynos_ohci_hcd {
	struct device *dev;
	struct usb_hcd *hcd;
	struct clk *clk;
	struct usb_phy *phy;
	struct usb_otg *otg;
	struct exynos4_ohci_platdata *pdata;
};

static void exynos_ohci_phy_enable(struct exynos_ohci_hcd *exynos_ohci)
{
	struct platform_device *pdev = to_platform_device(exynos_ohci->dev);

	if (exynos_ohci->phy)
		usb_phy_init(exynos_ohci->phy);
	else if (exynos_ohci->pdata && exynos_ohci->pdata->phy_init)
		exynos_ohci->pdata->phy_init(pdev, USB_PHY_TYPE_HOST);
}

static void exynos_ohci_phy_disable(struct exynos_ohci_hcd *exynos_ohci)
{
	struct platform_device *pdev = to_platform_device(exynos_ohci->dev);

	if (exynos_ohci->phy)
		usb_phy_shutdown(exynos_ohci->phy);
	else if (exynos_ohci->pdata && exynos_ohci->pdata->phy_exit)
		exynos_ohci->pdata->phy_exit(pdev, USB_PHY_TYPE_HOST);
}

static int ohci_exynos_reset(struct usb_hcd *hcd)
{
	return ohci_init(hcd_to_ohci(hcd));
}

static int ohci_exynos_start(struct usb_hcd *hcd)
{
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);
	int ret;

	ohci_dbg(ohci, "ohci_exynos_start, ohci:%p", ohci);

	ret = ohci_run(ohci);
	if (ret < 0) {
		dev_err(hcd->self.controller, "can't start %s\n",
			hcd->self.bus_name);
		ohci_stop(hcd);
		return ret;
=======
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/phy/phy.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>

#include "ohci.h"

#define DRIVER_DESC "OHCI EXYNOS driver"

static const char hcd_name[] = "ohci-exynos";
static struct hc_driver __read_mostly exynos_ohci_hc_driver;

#define to_exynos_ohci(hcd) (struct exynos_ohci_hcd *)(hcd_to_ohci(hcd)->priv)

#define PHY_NUMBER 3

struct exynos_ohci_hcd {
	struct clk *clk;
	struct phy *phy[PHY_NUMBER];
};

static int exynos_ohci_get_phy(struct device *dev,
				struct exynos_ohci_hcd *exynos_ohci)
{
	struct device_node *child;
	struct phy *phy;
	int phy_number;
	int ret;

	/* Get PHYs for the controller */
	for_each_available_child_of_node(dev->of_node, child) {
		ret = of_property_read_u32(child, "reg", &phy_number);
		if (ret) {
			dev_err(dev, "Failed to parse device tree\n");
			of_node_put(child);
			return ret;
		}

		if (phy_number >= PHY_NUMBER) {
			dev_err(dev, "Invalid number of PHYs\n");
			of_node_put(child);
			return -EINVAL;
		}

		phy = devm_of_phy_get(dev, child, NULL);
		exynos_ohci->phy[phy_number] = phy;
		if (IS_ERR(phy)) {
			ret = PTR_ERR(phy);
			if (ret == -EPROBE_DEFER) {
				of_node_put(child);
				return ret;
			} else if (ret != -ENOSYS && ret != -ENODEV) {
				dev_err(dev,
					"Error retrieving usb2 phy: %d\n", ret);
				of_node_put(child);
				return ret;
			}
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return 0;
}

<<<<<<< HEAD
static const struct hc_driver exynos_ohci_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "EXYNOS OHCI Host Controller",
	.hcd_priv_size		= sizeof(struct ohci_hcd),

	.irq			= ohci_irq,
	.flags			= HCD_MEMORY|HCD_USB11,

	.reset			= ohci_exynos_reset,
	.start			= ohci_exynos_start,
	.stop			= ohci_stop,
	.shutdown		= ohci_shutdown,

	.get_frame_number	= ohci_get_frame,

	.urb_enqueue		= ohci_urb_enqueue,
	.urb_dequeue		= ohci_urb_dequeue,
	.endpoint_disable	= ohci_endpoint_disable,

	.hub_status_data	= ohci_hub_status_data,
	.hub_control		= ohci_hub_control,
#ifdef	CONFIG_PM
	.bus_suspend		= ohci_bus_suspend,
	.bus_resume		= ohci_bus_resume,
#endif
	.start_port_reset	= ohci_start_port_reset,
};

static int exynos_ohci_probe(struct platform_device *pdev)
{
	struct exynos4_ohci_platdata *pdata = pdev->dev.platform_data;
	struct exynos_ohci_hcd *exynos_ohci;
	struct usb_hcd *hcd;
	struct ohci_hcd *ohci;
	struct resource *res;
	struct usb_phy *phy;
=======
static int exynos_ohci_phy_enable(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct exynos_ohci_hcd *exynos_ohci = to_exynos_ohci(hcd);
	int i;
	int ret = 0;

	for (i = 0; ret == 0 && i < PHY_NUMBER; i++)
		if (!IS_ERR(exynos_ohci->phy[i]))
			ret = phy_power_on(exynos_ohci->phy[i]);
	if (ret)
		for (i--; i >= 0; i--)
			if (!IS_ERR(exynos_ohci->phy[i]))
				phy_power_off(exynos_ohci->phy[i]);

	return ret;
}

static void exynos_ohci_phy_disable(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct exynos_ohci_hcd *exynos_ohci = to_exynos_ohci(hcd);
	int i;

	for (i = 0; i < PHY_NUMBER; i++)
		if (!IS_ERR(exynos_ohci->phy[i]))
			phy_power_off(exynos_ohci->phy[i]);
}

static int exynos_ohci_probe(struct platform_device *pdev)
{
	struct exynos_ohci_hcd *exynos_ohci;
	struct usb_hcd *hcd;
	struct resource *res;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int irq;
	int err;

	/*
	 * Right now device-tree probed devices don't get dma_mask set.
	 * Since shared usb code relies on it, set it here for now.
	 * Once we move to full device tree support this will vanish off.
	 */
<<<<<<< HEAD
	if (!pdev->dev.dma_mask)
		pdev->dev.dma_mask = &pdev->dev.coherent_dma_mask;
	if (!pdev->dev.coherent_dma_mask)
		pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);

	exynos_ohci = devm_kzalloc(&pdev->dev, sizeof(struct exynos_ohci_hcd),
					GFP_KERNEL);
	if (!exynos_ohci)
		return -ENOMEM;
=======
	err = dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	if (err)
		return err;

	hcd = usb_create_hcd(&exynos_ohci_hc_driver,
				&pdev->dev, dev_name(&pdev->dev));
	if (!hcd) {
		dev_err(&pdev->dev, "Unable to create HCD\n");
		return -ENOMEM;
	}

	exynos_ohci = to_exynos_ohci(hcd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (of_device_is_compatible(pdev->dev.of_node,
					"samsung,exynos5440-ohci"))
		goto skip_phy;

<<<<<<< HEAD
	phy = devm_usb_get_phy(&pdev->dev, USB_PHY_TYPE_USB2);
	if (IS_ERR(phy)) {
		/* Fallback to pdata */
		if (!pdata) {
			dev_warn(&pdev->dev, "no platform data or transceiver defined\n");
			return -EPROBE_DEFER;
		} else {
			exynos_ohci->pdata = pdata;
		}
	} else {
		exynos_ohci->phy = phy;
		exynos_ohci->otg = phy->otg;
	}

skip_phy:

	exynos_ohci->dev = &pdev->dev;

	hcd = usb_create_hcd(&exynos_ohci_hc_driver, &pdev->dev,
					dev_name(&pdev->dev));
	if (!hcd) {
		dev_err(&pdev->dev, "Unable to create HCD\n");
		return -ENOMEM;
	}

	exynos_ohci->hcd = hcd;
=======
	err = exynos_ohci_get_phy(&pdev->dev, exynos_ohci);
	if (err)
		goto fail_clk;

skip_phy:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	exynos_ohci->clk = devm_clk_get(&pdev->dev, "usbhost");

	if (IS_ERR(exynos_ohci->clk)) {
		dev_err(&pdev->dev, "Failed to get usbhost clock\n");
		err = PTR_ERR(exynos_ohci->clk);
		goto fail_clk;
	}

	err = clk_prepare_enable(exynos_ohci->clk);
	if (err)
		goto fail_clk;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD
	if (!res) {
		dev_err(&pdev->dev, "Failed to get I/O memory\n");
		err = -ENXIO;
		goto fail_io;
	}

	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);
	hcd->regs = devm_ioremap(&pdev->dev, res->start, hcd->rsrc_len);
	if (!hcd->regs) {
		dev_err(&pdev->dev, "Failed to remap I/O memory\n");
		err = -ENOMEM;
		goto fail_io;
	}

	irq = platform_get_irq(pdev, 0);
	if (!irq) {
		dev_err(&pdev->dev, "Failed to get IRQ\n");
		err = -ENODEV;
		goto fail_io;
	}

	if (exynos_ohci->otg)
		exynos_ohci->otg->set_host(exynos_ohci->otg,
					&exynos_ohci->hcd->self);

	exynos_ohci_phy_enable(exynos_ohci);

	ohci = hcd_to_ohci(hcd);
	ohci_hcd_init(ohci);
=======
	hcd->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(hcd->regs)) {
		err = PTR_ERR(hcd->regs);
		goto fail_io;
	}
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		err = irq;
		goto fail_io;
	}

	platform_set_drvdata(pdev, hcd);

	err = exynos_ohci_phy_enable(&pdev->dev);
	if (err) {
		dev_err(&pdev->dev, "Failed to enable USB phy\n");
		goto fail_io;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	err = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (err) {
		dev_err(&pdev->dev, "Failed to add USB HCD\n");
		goto fail_add_hcd;
	}
<<<<<<< HEAD

	platform_set_drvdata(pdev, exynos_ohci);

	return 0;

fail_add_hcd:
	exynos_ohci_phy_disable(exynos_ohci);
=======
	device_wakeup_enable(hcd->self.controller);
	return 0;

fail_add_hcd:
	exynos_ohci_phy_disable(&pdev->dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
fail_io:
	clk_disable_unprepare(exynos_ohci->clk);
fail_clk:
	usb_put_hcd(hcd);
	return err;
}

static int exynos_ohci_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct exynos_ohci_hcd *exynos_ohci = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = exynos_ohci->hcd;

	usb_remove_hcd(hcd);

	if (exynos_ohci->otg)
		exynos_ohci->otg->set_host(exynos_ohci->otg,
					&exynos_ohci->hcd->self);

	exynos_ohci_phy_disable(exynos_ohci);
=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct exynos_ohci_hcd *exynos_ohci = to_exynos_ohci(hcd);

	usb_remove_hcd(hcd);

	exynos_ohci_phy_disable(&pdev->dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	clk_disable_unprepare(exynos_ohci->clk);

	usb_put_hcd(hcd);

	return 0;
}

static void exynos_ohci_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct exynos_ohci_hcd *exynos_ohci = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = exynos_ohci->hcd;
=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);
}

#ifdef CONFIG_PM
static int exynos_ohci_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct exynos_ohci_hcd *exynos_ohci = dev_get_drvdata(dev);
	struct usb_hcd *hcd = exynos_ohci->hcd;
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);
	unsigned long flags;
	int rc = 0;

	/*
	 * Root hub was already suspended. Disable irq emission and
	 * mark HW unaccessible, bail out if RH has been resumed. Use
	 * the spinlock to properly synchronize with possible pending
	 * RH suspend or resume activity.
	 */
	spin_lock_irqsave(&ohci->lock, flags);
	if (ohci->rh_state != OHCI_RH_SUSPENDED &&
			ohci->rh_state != OHCI_RH_HALTED) {
		rc = -EINVAL;
		goto fail;
	}

	clear_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);

	if (exynos_ohci->otg)
		exynos_ohci->otg->set_host(exynos_ohci->otg,
					&exynos_ohci->hcd->self);

	exynos_ohci_phy_disable(exynos_ohci);

	clk_disable_unprepare(exynos_ohci->clk);

fail:
	spin_unlock_irqrestore(&ohci->lock, flags);

	return rc;
=======
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct exynos_ohci_hcd *exynos_ohci = to_exynos_ohci(hcd);
	bool do_wakeup = device_may_wakeup(dev);
	int rc = ohci_suspend(hcd, do_wakeup);

	if (rc)
		return rc;

	exynos_ohci_phy_disable(dev);

	clk_disable_unprepare(exynos_ohci->clk);

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int exynos_ohci_resume(struct device *dev)
{
<<<<<<< HEAD
	struct exynos_ohci_hcd *exynos_ohci = dev_get_drvdata(dev);
	struct usb_hcd *hcd = exynos_ohci->hcd;

	clk_prepare_enable(exynos_ohci->clk);

	if (exynos_ohci->otg)
		exynos_ohci->otg->set_host(exynos_ohci->otg,
					&exynos_ohci->hcd->self);

	exynos_ohci_phy_enable(exynos_ohci);
=======
	struct usb_hcd *hcd			= dev_get_drvdata(dev);
	struct exynos_ohci_hcd *exynos_ohci	= to_exynos_ohci(hcd);
	int ret;

	clk_prepare_enable(exynos_ohci->clk);

	ret = exynos_ohci_phy_enable(dev);
	if (ret) {
		dev_err(dev, "Failed to enable USB phy\n");
		clk_disable_unprepare(exynos_ohci->clk);
		return ret;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ohci_resume(hcd, false);

	return 0;
}
#else
#define exynos_ohci_suspend	NULL
#define exynos_ohci_resume	NULL
#endif

<<<<<<< HEAD
=======
static const struct ohci_driver_overrides exynos_overrides __initconst = {
	.extra_priv_size =	sizeof(struct exynos_ohci_hcd),
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static const struct dev_pm_ops exynos_ohci_pm_ops = {
	.suspend	= exynos_ohci_suspend,
	.resume		= exynos_ohci_resume,
};

#ifdef CONFIG_OF
static const struct of_device_id exynos_ohci_match[] = {
	{ .compatible = "samsung,exynos4210-ohci" },
	{ .compatible = "samsung,exynos5440-ohci" },
	{},
};
MODULE_DEVICE_TABLE(of, exynos_ohci_match);
#endif

static struct platform_driver exynos_ohci_driver = {
	.probe		= exynos_ohci_probe,
	.remove		= exynos_ohci_remove,
	.shutdown	= exynos_ohci_shutdown,
	.driver = {
		.name	= "exynos-ohci",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.pm	= &exynos_ohci_pm_ops,
		.of_match_table	= of_match_ptr(exynos_ohci_match),
	}
};
<<<<<<< HEAD

MODULE_ALIAS("platform:exynos-ohci");
MODULE_AUTHOR("Jingoo Han <jg1.han@samsung.com>");
=======
static int __init ohci_exynos_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info("%s: " DRIVER_DESC "\n", hcd_name);
	ohci_init_driver(&exynos_ohci_hc_driver, &exynos_overrides);
	return platform_driver_register(&exynos_ohci_driver);
}
module_init(ohci_exynos_init);

static void __exit ohci_exynos_cleanup(void)
{
	platform_driver_unregister(&exynos_ohci_driver);
}
module_exit(ohci_exynos_cleanup);

MODULE_ALIAS("platform:exynos-ohci");
MODULE_AUTHOR("Jingoo Han <jg1.han@samsung.com>");
MODULE_LICENSE("GPL v2");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
