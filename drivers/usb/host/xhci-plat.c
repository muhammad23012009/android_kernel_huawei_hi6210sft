/*
 * xhci-plat.c - xHCI host controller driver platform Bus Glue.
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com
 * Author: Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 *
 * A lot of code borrowed from the Linux xHCI driver.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "xhci.h"
=======
#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/usb/phy.h>
#include <linux/slab.h>
#include <linux/acpi.h>

#include "xhci.h"
#include "xhci-plat.h"
#include "xhci-mvebu.h"
#include "xhci-rcar.h"

static struct hc_driver __read_mostly xhci_plat_hc_driver;

static int xhci_plat_setup(struct usb_hcd *hcd);
static int xhci_plat_start(struct usb_hcd *hcd);

static const struct xhci_driver_overrides xhci_plat_overrides __initconst = {
	.extra_priv_size = sizeof(struct xhci_plat_priv),
	.reset = xhci_plat_setup,
	.start = xhci_plat_start,
};

static void xhci_priv_plat_start(struct usb_hcd *hcd)
{
	struct xhci_plat_priv *priv = hcd_to_xhci_priv(hcd);

	if (priv->plat_start)
		priv->plat_start(hcd);
}

static int xhci_priv_init_quirk(struct usb_hcd *hcd)
{
	struct xhci_plat_priv *priv = hcd_to_xhci_priv(hcd);

	if (!priv->init_quirk)
		return 0;

	return priv->init_quirk(hcd);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void xhci_plat_quirks(struct device *dev, struct xhci_hcd *xhci)
{
	/*
	 * As of now platform drivers don't provide MSI support so we ensure
	 * here that the generic code does not try to make a pci_dev from our
	 * dev struct in order to setup MSI
	 */
	xhci->quirks |= XHCI_PLAT;
}

/* called during probe() after chip reset completes */
static int xhci_plat_setup(struct usb_hcd *hcd)
{
<<<<<<< HEAD
	return xhci_gen_setup(hcd, xhci_plat_quirks);
}

static const struct hc_driver xhci_plat_xhci_driver = {
	.description =		"xhci-hcd",
	.product_desc =		"xHCI Host Controller",
	.hcd_priv_size =	sizeof(struct xhci_hcd *),

	/*
	 * generic hardware linkage
	 */
	.irq =			xhci_irq,
	.flags =		HCD_MEMORY | HCD_USB3 | HCD_SHARED,

	/*
	 * basic lifecycle operations
	 */
	.reset =		xhci_plat_setup,
	.start =		xhci_run,
	.stop =			xhci_stop,
	.shutdown =		xhci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		xhci_urb_enqueue,
	.urb_dequeue =		xhci_urb_dequeue,
	.alloc_dev =		xhci_alloc_dev,
	.free_dev =		xhci_free_dev,
	.alloc_streams =	xhci_alloc_streams,
	.free_streams =		xhci_free_streams,
	.add_endpoint =		xhci_add_endpoint,
	.drop_endpoint =	xhci_drop_endpoint,
	.endpoint_reset =	xhci_endpoint_reset,
	.check_bandwidth =	xhci_check_bandwidth,
	.reset_bandwidth =	xhci_reset_bandwidth,
	.address_device =	xhci_address_device,
	.update_hub_device =	xhci_update_hub_device,
	.reset_device =		xhci_discover_or_reset_device,

	/*
	 * scheduling support
	 */
	.get_frame_number =	xhci_get_frame,

	/* Root hub support */
	.hub_control =		xhci_hub_control,
	.hub_status_data =	xhci_hub_status_data,
	.bus_suspend =		xhci_bus_suspend,
	.bus_resume =		xhci_bus_resume,
};

static int xhci_plat_probe(struct platform_device *pdev)
{
=======
	int ret;


	ret = xhci_priv_init_quirk(hcd);
	if (ret)
		return ret;

	return xhci_gen_setup(hcd, xhci_plat_quirks);
}

static int xhci_plat_start(struct usb_hcd *hcd)
{
	xhci_priv_plat_start(hcd);
	return xhci_run(hcd);
}

#ifdef CONFIG_OF
static const struct xhci_plat_priv xhci_plat_marvell_armada = {
	.init_quirk = xhci_mvebu_mbus_init_quirk,
};

static const struct xhci_plat_priv xhci_plat_renesas_rcar_gen2 = {
	.firmware_name = XHCI_RCAR_FIRMWARE_NAME_V1,
	.init_quirk = xhci_rcar_init_quirk,
	.plat_start = xhci_rcar_start,
};

static const struct xhci_plat_priv xhci_plat_renesas_rcar_gen3 = {
	.firmware_name = XHCI_RCAR_FIRMWARE_NAME_V2,
	.init_quirk = xhci_rcar_init_quirk,
	.plat_start = xhci_rcar_start,
};

static const struct of_device_id usb_xhci_of_match[] = {
	{
		.compatible = "generic-xhci",
	}, {
		.compatible = "xhci-platform",
	}, {
		.compatible = "marvell,armada-375-xhci",
		.data = &xhci_plat_marvell_armada,
	}, {
		.compatible = "marvell,armada-380-xhci",
		.data = &xhci_plat_marvell_armada,
	}, {
		.compatible = "renesas,xhci-r8a7790",
		.data = &xhci_plat_renesas_rcar_gen2,
	}, {
		.compatible = "renesas,xhci-r8a7791",
		.data = &xhci_plat_renesas_rcar_gen2,
	}, {
		.compatible = "renesas,xhci-r8a7793",
		.data = &xhci_plat_renesas_rcar_gen2,
	}, {
		.compatible = "renesas,xhci-r8a7795",
		.data = &xhci_plat_renesas_rcar_gen3,
	}, {
		.compatible = "renesas,rcar-gen2-xhci",
		.data = &xhci_plat_renesas_rcar_gen2,
	}, {
		.compatible = "renesas,rcar-gen3-xhci",
		.data = &xhci_plat_renesas_rcar_gen3,
	},
	{},
};
MODULE_DEVICE_TABLE(of, usb_xhci_of_match);
#endif

static int xhci_plat_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	const struct hc_driver	*driver;
	struct xhci_hcd		*xhci;
	struct resource         *res;
	struct usb_hcd		*hcd;
<<<<<<< HEAD
=======
	struct clk              *clk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int			ret;
	int			irq;

	if (usb_disabled())
		return -ENODEV;

<<<<<<< HEAD
	driver = &xhci_plat_xhci_driver;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return -ENODEV;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;
=======
	driver = &xhci_plat_hc_driver;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	/* Try to set 64-bit DMA first */
	if (WARN_ON(!pdev->dev.dma_mask))
		/* Platform did not initialize dma_mask */
		ret = dma_coerce_mask_and_coherent(&pdev->dev,
						   DMA_BIT_MASK(64));
	else
		ret = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));

	/* If seting 64-bit DMA mask fails, fall back to 32-bit DMA mask */
	if (ret) {
		ret = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
		if (ret)
			return ret;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	hcd = usb_create_hcd(driver, &pdev->dev, dev_name(&pdev->dev));
	if (!hcd)
		return -ENOMEM;

<<<<<<< HEAD
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len,
				driver->description)) {
		dev_dbg(&pdev->dev, "controller already in use\n");
		ret = -EBUSY;
		goto put_hcd;
	}

	hcd->regs = ioremap_nocache(hcd->rsrc_start, hcd->rsrc_len);
	if (!hcd->regs) {
		dev_dbg(&pdev->dev, "error mapping memory\n");
		ret = -EFAULT;
		goto release_mem_region;
	}

	ret = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (ret)
		goto unmap_registers;

	/* USB 2.0 roothub is stored in the platform_device now. */
	hcd = dev_get_drvdata(&pdev->dev);
	xhci = hcd_to_xhci(hcd);
=======
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	hcd->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(hcd->regs)) {
		ret = PTR_ERR(hcd->regs);
		goto put_hcd;
	}

	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);

	/*
	 * Not all platforms have a clk so it is not an error if the
	 * clock does not exists.
	 */
	clk = devm_clk_get(&pdev->dev, NULL);
	if (!IS_ERR(clk)) {
		ret = clk_prepare_enable(clk);
		if (ret)
			goto put_hcd;
	} else if (PTR_ERR(clk) == -EPROBE_DEFER) {
		ret = -EPROBE_DEFER;
		goto put_hcd;
	}

	xhci = hcd_to_xhci(hcd);
	match = of_match_node(usb_xhci_of_match, pdev->dev.of_node);
	if (match) {
		const struct xhci_plat_priv *priv_match = match->data;
		struct xhci_plat_priv *priv = hcd_to_xhci_priv(hcd);

		/* Just copy data for now */
		if (priv_match)
			*priv = *priv_match;
	}

	device_wakeup_enable(hcd->self.controller);

	xhci->clk = clk;
	xhci->main_hcd = hcd;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	xhci->shared_hcd = usb_create_shared_hcd(driver, &pdev->dev,
			dev_name(&pdev->dev), hcd);
	if (!xhci->shared_hcd) {
		ret = -ENOMEM;
<<<<<<< HEAD
		goto dealloc_usb2_hcd;
	}

	/*
	 * Set the xHCI pointer before xhci_plat_setup() (aka hcd_driver.reset)
	 * is called by usb_add_hcd().
	 */
	*((struct xhci_hcd **) xhci->shared_hcd->hcd_priv) = xhci;

	ret = usb_add_hcd(xhci->shared_hcd, irq, IRQF_SHARED);
	if (ret)
		goto put_usb3_hcd;

	return 0;

put_usb3_hcd:
	usb_put_hcd(xhci->shared_hcd);
=======
		goto disable_clk;
	}

	if (device_property_read_bool(&pdev->dev, "usb3-lpm-capable"))
		xhci->quirks |= XHCI_LPM_SUPPORT;

	if (device_property_read_bool(&pdev->dev, "quirk-broken-port-ped"))
		xhci->quirks |= XHCI_BROKEN_PORT_PED;

	hcd->usb_phy = devm_usb_get_phy_by_phandle(&pdev->dev, "usb-phy", 0);
	if (IS_ERR(hcd->usb_phy)) {
		ret = PTR_ERR(hcd->usb_phy);
		if (ret == -EPROBE_DEFER)
			goto put_usb3_hcd;
		hcd->usb_phy = NULL;
	} else {
		ret = usb_phy_init(hcd->usb_phy);
		if (ret)
			goto put_usb3_hcd;
	}

	ret = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (ret)
		goto disable_usb_phy;

	if (HCC_MAX_PSA(xhci->hcc_params) >= 4)
		xhci->shared_hcd->can_do_streams = 1;

	ret = usb_add_hcd(xhci->shared_hcd, irq, IRQF_SHARED);
	if (ret)
		goto dealloc_usb2_hcd;

	return 0;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

dealloc_usb2_hcd:
	usb_remove_hcd(hcd);

<<<<<<< HEAD
unmap_registers:
	iounmap(hcd->regs);

release_mem_region:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
=======
disable_usb_phy:
	usb_phy_shutdown(hcd->usb_phy);

put_usb3_hcd:
	usb_put_hcd(xhci->shared_hcd);

disable_clk:
	if (!IS_ERR(clk))
		clk_disable_unprepare(clk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

put_hcd:
	usb_put_hcd(hcd);

	return ret;
}

static int xhci_plat_remove(struct platform_device *dev)
{
	struct usb_hcd	*hcd = platform_get_drvdata(dev);
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);
<<<<<<< HEAD
=======
	struct clk *clk = xhci->clk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	xhci->xhc_state |= XHCI_STATE_REMOVING;

	usb_remove_hcd(xhci->shared_hcd);
<<<<<<< HEAD
	usb_put_hcd(xhci->shared_hcd);

	usb_remove_hcd(hcd);
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
	kfree(xhci);
=======
	usb_phy_shutdown(hcd->usb_phy);

	usb_remove_hcd(hcd);
	usb_put_hcd(xhci->shared_hcd);

	if (!IS_ERR(clk))
		clk_disable_unprepare(clk);
	usb_put_hcd(hcd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static struct platform_driver usb_xhci_driver = {
	.probe	= xhci_plat_probe,
	.remove	= xhci_plat_remove,
	.driver	= {
		.name = "xhci-hcd",
=======
#ifdef CONFIG_PM_SLEEP
static int xhci_plat_suspend(struct device *dev)
{
	struct usb_hcd	*hcd = dev_get_drvdata(dev);
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);

	/*
	 * xhci_suspend() needs `do_wakeup` to know whether host is allowed
	 * to do wakeup during suspend. Since xhci_plat_suspend is currently
	 * only designed for system suspend, device_may_wakeup() is enough
	 * to dertermine whether host is allowed to do wakeup. Need to
	 * reconsider this when xhci_plat_suspend enlarges its scope, e.g.,
	 * also applies to runtime suspend.
	 */
	return xhci_suspend(xhci, device_may_wakeup(dev));
}

static int xhci_plat_resume(struct device *dev)
{
	struct usb_hcd	*hcd = dev_get_drvdata(dev);
	struct xhci_hcd	*xhci = hcd_to_xhci(hcd);

	return xhci_resume(xhci, 0);
}

static const struct dev_pm_ops xhci_plat_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(xhci_plat_suspend, xhci_plat_resume)
};
#define DEV_PM_OPS	(&xhci_plat_pm_ops)
#else
#define DEV_PM_OPS	NULL
#endif /* CONFIG_PM */

static const struct acpi_device_id usb_xhci_acpi_match[] = {
	/* XHCI-compliant USB Controller */
	{ "PNP0D10", },
	{ }
};
MODULE_DEVICE_TABLE(acpi, usb_xhci_acpi_match);

static struct platform_driver usb_xhci_driver = {
	.probe	= xhci_plat_probe,
	.remove	= xhci_plat_remove,
	.shutdown = usb_hcd_platform_shutdown,
	.driver	= {
		.name = "xhci-hcd",
		.pm = DEV_PM_OPS,
		.of_match_table = of_match_ptr(usb_xhci_of_match),
		.acpi_match_table = ACPI_PTR(usb_xhci_acpi_match),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
};
MODULE_ALIAS("platform:xhci-hcd");

<<<<<<< HEAD
int xhci_register_plat(void)
{
	return platform_driver_register(&usb_xhci_driver);
}

void xhci_unregister_plat(void)
{
	platform_driver_unregister(&usb_xhci_driver);
}
=======
static int __init xhci_plat_init(void)
{
	xhci_init_driver(&xhci_plat_hc_driver, &xhci_plat_overrides);
	return platform_driver_register(&usb_xhci_driver);
}
module_init(xhci_plat_init);

static void __exit xhci_plat_exit(void)
{
	platform_driver_unregister(&usb_xhci_driver);
}
module_exit(xhci_plat_exit);

MODULE_DESCRIPTION("xHCI Platform Host Controller Driver");
MODULE_LICENSE("GPL");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
