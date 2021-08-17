/**
 * dwc3-pci.c - PCI Specific glue layer
 *
 * Copyright (C) 2010-2011 Texas Instruments Incorporated - http://www.ti.com
 *
 * Authors: Felipe Balbi <balbi@ti.com>,
 *	    Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 *
<<<<<<< HEAD
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the above-listed copyright holders may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2, as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=======
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/pci.h>
<<<<<<< HEAD
#include <linux/platform_device.h>

#include <linux/usb/otg.h>
#include <linux/usb/nop-usb-xceiv.h>

/* FIXME define these in <linux/pci_ids.h> */
#define PCI_VENDOR_ID_SYNOPSYS		0x16c3
#define PCI_DEVICE_ID_SYNOPSYS_HAPSUSB3	0xabcd
#define PCI_DEVICE_ID_INTEL_BYT		0x0f37
#define PCI_DEVICE_ID_INTEL_MRFLD	0x119e

struct dwc3_pci {
	struct device		*dev;
	struct platform_device	*dwc3;
	struct platform_device	*usb2_phy;
	struct platform_device	*usb3_phy;
};

static int dwc3_pci_register_phys(struct dwc3_pci *glue)
{
	struct nop_usb_xceiv_platform_data pdata;
	struct platform_device	*pdev;
	int			ret;

	memset(&pdata, 0x00, sizeof(pdata));

	pdev = platform_device_alloc("nop_usb_xceiv", 0);
	if (!pdev)
		return -ENOMEM;

	glue->usb2_phy = pdev;
	pdata.type = USB_PHY_TYPE_USB2;

	ret = platform_device_add_data(glue->usb2_phy, &pdata, sizeof(pdata));
	if (ret)
		goto err1;

	pdev = platform_device_alloc("nop_usb_xceiv", 1);
	if (!pdev) {
		ret = -ENOMEM;
		goto err1;
	}

	glue->usb3_phy = pdev;
	pdata.type = USB_PHY_TYPE_USB3;

	ret = platform_device_add_data(glue->usb3_phy, &pdata, sizeof(pdata));
	if (ret)
		goto err2;

	ret = platform_device_add(glue->usb2_phy);
	if (ret)
		goto err2;

	ret = platform_device_add(glue->usb3_phy);
	if (ret)
		goto err3;

	return 0;

err3:
	platform_device_del(glue->usb2_phy);

err2:
	platform_device_put(glue->usb3_phy);

err1:
	platform_device_put(glue->usb2_phy);

	return ret;
=======
#include <linux/pm_runtime.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/acpi.h>
#include <linux/delay.h>

#define PCI_DEVICE_ID_SYNOPSYS_HAPSUSB3		0xabcd
#define PCI_DEVICE_ID_SYNOPSYS_HAPSUSB3_AXI	0xabce
#define PCI_DEVICE_ID_SYNOPSYS_HAPSUSB31	0xabcf
#define PCI_DEVICE_ID_INTEL_BYT			0x0f37
#define PCI_DEVICE_ID_INTEL_MRFLD		0x119e
#define PCI_DEVICE_ID_INTEL_BSW			0x22b7
#define PCI_DEVICE_ID_INTEL_SPTLP		0x9d30
#define PCI_DEVICE_ID_INTEL_SPTH		0xa130
#define PCI_DEVICE_ID_INTEL_BXT			0x0aaa
#define PCI_DEVICE_ID_INTEL_BXT_M		0x1aaa
#define PCI_DEVICE_ID_INTEL_APL			0x5aaa
#define PCI_DEVICE_ID_INTEL_KBP			0xa2b0
#define PCI_DEVICE_ID_INTEL_GLK			0x31aa

static const struct acpi_gpio_params reset_gpios = { 0, 0, false };
static const struct acpi_gpio_params cs_gpios = { 1, 0, false };

static const struct acpi_gpio_mapping acpi_dwc3_byt_gpios[] = {
	{ "reset-gpios", &reset_gpios, 1 },
	{ "cs-gpios", &cs_gpios, 1 },
	{ },
};

static int dwc3_pci_quirks(struct pci_dev *pdev, struct platform_device *dwc3)
{
	if (pdev->vendor == PCI_VENDOR_ID_AMD &&
	    pdev->device == PCI_DEVICE_ID_AMD_NL_USB) {
		struct property_entry properties[] = {
			PROPERTY_ENTRY_BOOL("snps,has-lpm-erratum"),
			PROPERTY_ENTRY_U8("snps,lpm-nyet-threshold", 0xf),
			PROPERTY_ENTRY_BOOL("snps,u2exit_lfps_quirk"),
			PROPERTY_ENTRY_BOOL("snps,u2ss_inp3_quirk"),
			PROPERTY_ENTRY_BOOL("snps,req_p1p2p3_quirk"),
			PROPERTY_ENTRY_BOOL("snps,del_p1p2p3_quirk"),
			PROPERTY_ENTRY_BOOL("snps,del_phy_power_chg_quirk"),
			PROPERTY_ENTRY_BOOL("snps,lfps_filter_quirk"),
			PROPERTY_ENTRY_BOOL("snps,rx_detect_poll_quirk"),
			PROPERTY_ENTRY_BOOL("snps,tx_de_emphasis_quirk"),
			PROPERTY_ENTRY_U8("snps,tx_de_emphasis", 1),
			/*
			 * FIXME these quirks should be removed when AMD NL
			 * tapes out
			 */
			PROPERTY_ENTRY_BOOL("snps,disable_scramble_quirk"),
			PROPERTY_ENTRY_BOOL("snps,dis_u3_susphy_quirk"),
			PROPERTY_ENTRY_BOOL("snps,dis_u2_susphy_quirk"),
			{ },
		};

		return platform_device_add_properties(dwc3, properties);
	}

	if (pdev->vendor == PCI_VENDOR_ID_INTEL) {
		int ret;

		struct property_entry properties[] = {
			PROPERTY_ENTRY_STRING("dr_mode", "peripheral"),
			{ }
		};

		ret = platform_device_add_properties(dwc3, properties);
		if (ret < 0)
			return ret;

		if (pdev->device == PCI_DEVICE_ID_INTEL_BYT) {
			struct gpio_desc *gpio;

			acpi_dev_add_driver_gpios(ACPI_COMPANION(&pdev->dev),
					acpi_dwc3_byt_gpios);

			/*
			 * These GPIOs will turn on the USB2 PHY. Note that we have to
			 * put the gpio descriptors again here because the phy driver
			 * might want to grab them, too.
			 */
			gpio = gpiod_get_optional(&pdev->dev, "cs", GPIOD_OUT_LOW);
			if (IS_ERR(gpio))
				return PTR_ERR(gpio);

			gpiod_set_value_cansleep(gpio, 1);
			gpiod_put(gpio);

			gpio = gpiod_get_optional(&pdev->dev, "reset", GPIOD_OUT_LOW);
			if (IS_ERR(gpio))
				return PTR_ERR(gpio);

			if (gpio) {
				gpiod_set_value_cansleep(gpio, 1);
				gpiod_put(gpio);
				usleep_range(10000, 11000);
			}
		}
	}

	if (pdev->vendor == PCI_VENDOR_ID_SYNOPSYS &&
	    (pdev->device == PCI_DEVICE_ID_SYNOPSYS_HAPSUSB3 ||
	     pdev->device == PCI_DEVICE_ID_SYNOPSYS_HAPSUSB3_AXI ||
	     pdev->device == PCI_DEVICE_ID_SYNOPSYS_HAPSUSB31)) {
		struct property_entry properties[] = {
			PROPERTY_ENTRY_BOOL("snps,usb3_lpm_capable"),
			PROPERTY_ENTRY_BOOL("snps,has-lpm-erratum"),
			PROPERTY_ENTRY_BOOL("snps,dis_enblslpm_quirk"),
			{ },
		};

		return platform_device_add_properties(dwc3, properties);
	}

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int dwc3_pci_probe(struct pci_dev *pci,
		const struct pci_device_id *id)
{
	struct resource		res[2];
	struct platform_device	*dwc3;
<<<<<<< HEAD
	struct dwc3_pci		*glue;
	int			ret = -ENOMEM;
	struct device		*dev = &pci->dev;

	glue = devm_kzalloc(dev, sizeof(*glue), GFP_KERNEL);
	if (!glue) {
		dev_err(dev, "not enough memory\n");
		return -ENOMEM;
	}

	glue->dev = dev;

	ret = pci_enable_device(pci);
=======
	int			ret;
	struct device		*dev = &pci->dev;

	ret = pcim_enable_device(pci);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret) {
		dev_err(dev, "failed to enable pci device\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	pci_set_power_state(pci, PCI_D0);
	pci_set_master(pci);

	ret = dwc3_pci_register_phys(glue);
	if (ret) {
		dev_err(dev, "couldn't register PHYs\n");
		return ret;
	}

	dwc3 = platform_device_alloc("dwc3", PLATFORM_DEVID_AUTO);
	if (!dwc3) {
		dev_err(dev, "couldn't allocate dwc3 device\n");
		ret = -ENOMEM;
		goto err1;
=======
	pci_set_master(pci);

	dwc3 = platform_device_alloc("dwc3", PLATFORM_DEVID_AUTO);
	if (!dwc3) {
		dev_err(dev, "couldn't allocate dwc3 device\n");
		return -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	memset(res, 0x00, sizeof(struct resource) * ARRAY_SIZE(res));

	res[0].start	= pci_resource_start(pci, 0);
	res[0].end	= pci_resource_end(pci, 0);
	res[0].name	= "dwc_usb3";
	res[0].flags	= IORESOURCE_MEM;

	res[1].start	= pci->irq;
	res[1].name	= "dwc_usb3";
	res[1].flags	= IORESOURCE_IRQ;

	ret = platform_device_add_resources(dwc3, res, ARRAY_SIZE(res));
	if (ret) {
		dev_err(dev, "couldn't add resources to dwc3 device\n");
<<<<<<< HEAD
		goto err1;
	}

	pci_set_drvdata(pci, glue);

	dma_set_coherent_mask(&dwc3->dev, dev->coherent_dma_mask);

	dwc3->dev.dma_mask = dev->dma_mask;
	dwc3->dev.dma_parms = dev->dma_parms;
	dwc3->dev.parent = dev;
	glue->dwc3 = dwc3;
=======
		goto err;
	}

	dwc3->dev.parent = dev;
	ACPI_COMPANION_SET(&dwc3->dev, ACPI_COMPANION(dev));

	ret = dwc3_pci_quirks(pci, dwc3);
	if (ret)
		goto err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = platform_device_add(dwc3);
	if (ret) {
		dev_err(dev, "failed to register dwc3 device\n");
<<<<<<< HEAD
		goto err3;
	}

	return 0;

err3:
	pci_set_drvdata(pci, NULL);
	platform_device_put(dwc3);
err1:
	pci_disable_device(pci);

=======
		goto err;
	}

	device_init_wakeup(dev, true);
	device_set_run_wake(dev, true);
	pci_set_drvdata(pci, dwc3);
	pm_runtime_put(dev);

	return 0;
err:
	platform_device_put(dwc3);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static void dwc3_pci_remove(struct pci_dev *pci)
{
<<<<<<< HEAD
	struct dwc3_pci	*glue = pci_get_drvdata(pci);

	platform_device_unregister(glue->dwc3);
	platform_device_unregister(glue->usb2_phy);
	platform_device_unregister(glue->usb3_phy);
	pci_set_drvdata(pci, NULL);
	pci_disable_device(pci);
}

static DEFINE_PCI_DEVICE_TABLE(dwc3_pci_id_table) = {
=======
	device_init_wakeup(&pci->dev, false);
	pm_runtime_get(&pci->dev);
	acpi_dev_remove_driver_gpios(ACPI_COMPANION(&pci->dev));
	platform_device_unregister(pci_get_drvdata(pci));
}

static const struct pci_device_id dwc3_pci_id_table[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{
		PCI_DEVICE(PCI_VENDOR_ID_SYNOPSYS,
				PCI_DEVICE_ID_SYNOPSYS_HAPSUSB3),
	},
<<<<<<< HEAD
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_BYT), },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_MRFLD), },
=======
	{
		PCI_DEVICE(PCI_VENDOR_ID_SYNOPSYS,
				PCI_DEVICE_ID_SYNOPSYS_HAPSUSB3_AXI),
	},
	{
		PCI_DEVICE(PCI_VENDOR_ID_SYNOPSYS,
				PCI_DEVICE_ID_SYNOPSYS_HAPSUSB31),
	},
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_BSW), },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_BYT), },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_MRFLD), },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_SPTLP), },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_SPTH), },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_BXT), },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_BXT_M), },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_APL), },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_KBP), },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_GLK), },
	{ PCI_DEVICE(PCI_VENDOR_ID_AMD, PCI_DEVICE_ID_AMD_NL_USB), },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{  }	/* Terminating Entry */
};
MODULE_DEVICE_TABLE(pci, dwc3_pci_id_table);

#ifdef CONFIG_PM
<<<<<<< HEAD
static int dwc3_pci_suspend(struct device *dev)
{
	struct pci_dev	*pci = to_pci_dev(dev);

	pci_disable_device(pci);

	return 0;
}

static int dwc3_pci_resume(struct device *dev)
{
	struct pci_dev	*pci = to_pci_dev(dev);
	int		ret;

	ret = pci_enable_device(pci);
	if (ret) {
		dev_err(dev, "can't re-enable device --> %d\n", ret);
		return ret;
	}

	pci_set_master(pci);

	return 0;
}

static const struct dev_pm_ops dwc3_pci_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(dwc3_pci_suspend, dwc3_pci_resume)
};

#define DEV_PM_OPS	(&dwc3_pci_dev_pm_ops)
#else
#define DEV_PM_OPS	NULL
#endif /* CONFIG_PM */

=======
static int dwc3_pci_runtime_suspend(struct device *dev)
{
	if (device_run_wake(dev))
		return 0;

	return -EBUSY;
}

static int dwc3_pci_runtime_resume(struct device *dev)
{
	struct platform_device *dwc3 = dev_get_drvdata(dev);

	return pm_runtime_get(&dwc3->dev);
}
#endif /* CONFIG_PM */

#ifdef CONFIG_PM_SLEEP
static int dwc3_pci_pm_dummy(struct device *dev)
{
	/*
	 * There's nothing to do here. No, seriously. Everything is either taken
	 * care either by PCI subsystem or dwc3/core.c, so we have nothing
	 * missing here.
	 *
	 * So you'd think we didn't need this at all, but PCI subsystem will
	 * bail out if we don't have a valid callback :-s
	 */
	return 0;
}
#endif /* CONFIG_PM_SLEEP */

static struct dev_pm_ops dwc3_pci_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(dwc3_pci_pm_dummy, dwc3_pci_pm_dummy)
	SET_RUNTIME_PM_OPS(dwc3_pci_runtime_suspend, dwc3_pci_runtime_resume,
		NULL)
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct pci_driver dwc3_pci_driver = {
	.name		= "dwc3-pci",
	.id_table	= dwc3_pci_id_table,
	.probe		= dwc3_pci_probe,
	.remove		= dwc3_pci_remove,
	.driver		= {
<<<<<<< HEAD
		.pm	= DEV_PM_OPS,
	},
};

MODULE_AUTHOR("Felipe Balbi <balbi@ti.com>");
MODULE_LICENSE("Dual BSD/GPL");
=======
		.pm	= &dwc3_pci_dev_pm_ops,
	}
};

MODULE_AUTHOR("Felipe Balbi <balbi@ti.com>");
MODULE_LICENSE("GPL v2");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_DESCRIPTION("DesignWare USB3 PCI Glue Layer");

module_pci_driver(dwc3_pci_driver);
