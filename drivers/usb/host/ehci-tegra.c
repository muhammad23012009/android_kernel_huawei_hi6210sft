/*
 * EHCI-compliant USB host controller driver for NVIDIA Tegra SoCs
 *
 * Copyright (C) 2010 Google, Inc.
 * Copyright (C) 2009 - 2013 NVIDIA Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#include <linux/clk.h>
<<<<<<< HEAD
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/platform_data/tegra_usb.h>
#include <linux/irq.h>
#include <linux/usb/otg.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/pm_runtime.h>
#include <linux/usb/ehci_def.h>
#include <linux/usb/tegra_usb_phy.h>
#include <linux/clk/tegra.h>

#define TEGRA_USB_BASE			0xC5000000
#define TEGRA_USB2_BASE			0xC5004000
#define TEGRA_USB3_BASE			0xC5008000

/* PORTSC registers */
#define TEGRA_USB_PORTSC1			0x184
#define TEGRA_USB_PORTSC1_PTS(x)	(((x) & 0x3) << 30)
#define TEGRA_USB_PORTSC1_PHCD	(1 << 23)

#define TEGRA_USB_DMA_ALIGN 32

struct tegra_ehci_hcd {
	struct ehci_hcd *ehci;
	struct tegra_usb_phy *phy;
	struct clk *clk;
	struct usb_phy *transceiver;
	int host_resumed;
=======
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/reset.h>
#include <linux/slab.h>
#include <linux/usb/ehci_def.h>
#include <linux/usb/tegra_usb_phy.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/usb/otg.h>

#include "ehci.h"

#define PORT_WAKE_BITS (PORT_WKOC_E|PORT_WKDISC_E|PORT_WKCONN_E)

#define TEGRA_USB_DMA_ALIGN 32

#define DRIVER_DESC "Tegra EHCI driver"
#define DRV_NAME "tegra-ehci"

static struct hc_driver __read_mostly tegra_ehci_hc_driver;
static bool usb1_reset_attempted;

struct tegra_ehci_soc_config {
	bool has_hostpc;
};

struct tegra_ehci_hcd {
	struct tegra_usb_phy *phy;
	struct clk *clk;
	struct reset_control *rst;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int port_resuming;
	bool needs_double_reset;
	enum tegra_usb_phy_port_speed port_speed;
};

<<<<<<< HEAD
static void tegra_ehci_power_up(struct usb_hcd *hcd)
{
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);

	clk_prepare_enable(tegra->clk);
	usb_phy_set_suspend(hcd->phy, 0);
	tegra->host_resumed = 1;
}

static void tegra_ehci_power_down(struct usb_hcd *hcd)
{
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);

	tegra->host_resumed = 0;
	usb_phy_set_suspend(hcd->phy, 1);
	clk_disable_unprepare(tegra->clk);
=======
/*
 * The 1st USB controller contains some UTMI pad registers that are global for
 * all the controllers on the chip. Those registers are also cleared when
 * reset is asserted to the 1st controller. This means that the 1st controller
 * can only be reset when no other controlled has finished probing. So we'll
 * reset the 1st controller before doing any other setup on any of the
 * controllers, and then never again.
 *
 * Since this is a PHY issue, the Tegra PHY driver should probably be doing
 * the resetting of the USB controllers. But to keep compatibility with old
 * device trees that don't have reset phandles in the PHYs, do it here.
 * Those old DTs will be vulnerable to total USB breakage if the 1st EHCI
 * device isn't the first one to finish probing, so warn them.
 */
static int tegra_reset_usb_controller(struct platform_device *pdev)
{
	struct device_node *phy_np;
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct tegra_ehci_hcd *tegra =
		(struct tegra_ehci_hcd *)hcd_to_ehci(hcd)->priv;
	bool has_utmi_pad_registers = false;

	phy_np = of_parse_phandle(pdev->dev.of_node, "nvidia,phy", 0);
	if (!phy_np)
		return -ENOENT;

	if (of_property_read_bool(phy_np, "nvidia,has-utmi-pad-registers"))
		has_utmi_pad_registers = true;

	if (!usb1_reset_attempted) {
		struct reset_control *usb1_reset;

		if (!has_utmi_pad_registers)
			usb1_reset = of_reset_control_get(phy_np, "utmi-pads");
		else
			usb1_reset = tegra->rst;

		if (IS_ERR(usb1_reset)) {
			dev_warn(&pdev->dev,
				 "can't get utmi-pads reset from the PHY\n");
			dev_warn(&pdev->dev,
				 "continuing, but please update your DT\n");
		} else {
			reset_control_assert(usb1_reset);
			udelay(1);
			reset_control_deassert(usb1_reset);

			if (!has_utmi_pad_registers)
				reset_control_put(usb1_reset);
		}

		usb1_reset_attempted = true;
	}

	if (!has_utmi_pad_registers) {
		reset_control_assert(tegra->rst);
		udelay(1);
		reset_control_deassert(tegra->rst);
	}

	of_node_put(phy_np);

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int tegra_ehci_internal_port_reset(
	struct ehci_hcd	*ehci,
	u32 __iomem	*portsc_reg
)
{
	u32		temp;
	unsigned long	flags;
	int		retval = 0;
	int		i, tries;
	u32		saved_usbintr;

	spin_lock_irqsave(&ehci->lock, flags);
	saved_usbintr = ehci_readl(ehci, &ehci->regs->intr_enable);
	/* disable USB interrupt */
	ehci_writel(ehci, 0, &ehci->regs->intr_enable);
	spin_unlock_irqrestore(&ehci->lock, flags);

	/*
	 * Here we have to do Port Reset at most twice for
	 * Port Enable bit to be set.
	 */
	for (i = 0; i < 2; i++) {
		temp = ehci_readl(ehci, portsc_reg);
		temp |= PORT_RESET;
		ehci_writel(ehci, temp, portsc_reg);
		mdelay(10);
		temp &= ~PORT_RESET;
		ehci_writel(ehci, temp, portsc_reg);
		mdelay(1);
		tries = 100;
		do {
			mdelay(1);
			/*
			 * Up to this point, Port Enable bit is
			 * expected to be set after 2 ms waiting.
			 * USB1 usually takes extra 45 ms, for safety,
			 * we take 100 ms as timeout.
			 */
			temp = ehci_readl(ehci, portsc_reg);
		} while (!(temp & PORT_PE) && tries--);
		if (temp & PORT_PE)
			break;
	}
	if (i == 2)
		retval = -ETIMEDOUT;

	/*
	 * Clear Connect Status Change bit if it's set.
	 * We can't clear PORT_PEC. It will also cause PORT_PE to be cleared.
	 */
	if (temp & PORT_CSC)
		ehci_writel(ehci, PORT_CSC, portsc_reg);

	/*
	 * Write to clear any interrupt status bits that might be set
	 * during port reset.
	 */
	temp = ehci_readl(ehci, &ehci->regs->status);
	ehci_writel(ehci, temp, &ehci->regs->status);

	/* restore original interrupt enable bits */
	ehci_writel(ehci, saved_usbintr, &ehci->regs->intr_enable);
	return retval;
}

static int tegra_ehci_hub_control(
	struct usb_hcd	*hcd,
	u16		typeReq,
	u16		wValue,
	u16		wIndex,
	char		*buf,
	u16		wLength
)
{
<<<<<<< HEAD
	struct ehci_hcd	*ehci = hcd_to_ehci(hcd);
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);
=======
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	struct tegra_ehci_hcd *tegra = (struct tegra_ehci_hcd *)ehci->priv;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32 __iomem	*status_reg;
	u32		temp;
	unsigned long	flags;
	int		retval = 0;

	status_reg = &ehci->regs->port_status[(wIndex & 0xff) - 1];

	spin_lock_irqsave(&ehci->lock, flags);

	if (typeReq == GetPortStatus) {
		temp = ehci_readl(ehci, status_reg);
		if (tegra->port_resuming && !(temp & PORT_SUSPEND)) {
			/* Resume completed, re-enable disconnect detection */
			tegra->port_resuming = 0;
<<<<<<< HEAD
			tegra_usb_phy_postresume(hcd->phy);
=======
			tegra_usb_phy_postresume(hcd->usb_phy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	else if (typeReq == SetPortFeature && wValue == USB_PORT_FEAT_SUSPEND) {
		temp = ehci_readl(ehci, status_reg);
		if ((temp & PORT_PE) == 0 || (temp & PORT_RESET) != 0) {
			retval = -EPIPE;
			goto done;
		}

		temp &= ~(PORT_RWC_BITS | PORT_WKCONN_E);
		temp |= PORT_WKDISC_E | PORT_WKOC_E;
		ehci_writel(ehci, temp | PORT_SUSPEND, status_reg);

		/*
		 * If a transaction is in progress, there may be a delay in
		 * suspending the port. Poll until the port is suspended.
		 */
<<<<<<< HEAD
		if (handshake(ehci, status_reg, PORT_SUSPEND,
=======
		if (ehci_handshake(ehci, status_reg, PORT_SUSPEND,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						PORT_SUSPEND, 5000))
			pr_err("%s: timeout waiting for SUSPEND\n", __func__);

		set_bit((wIndex & 0xff) - 1, &ehci->suspended_ports);
		goto done;
	}

	/* For USB1 port we need to issue Port Reset twice internally */
	if (tegra->needs_double_reset &&
	   (typeReq == SetPortFeature && wValue == USB_PORT_FEAT_RESET)) {
		spin_unlock_irqrestore(&ehci->lock, flags);
		return tegra_ehci_internal_port_reset(ehci, status_reg);
	}

	/*
	 * Tegra host controller will time the resume operation to clear the bit
	 * when the port control state switches to HS or FS Idle. This behavior
	 * is different from EHCI where the host controller driver is required
	 * to set this bit to a zero after the resume duration is timed in the
	 * driver.
	 */
	else if (typeReq == ClearPortFeature &&
					wValue == USB_PORT_FEAT_SUSPEND) {
		temp = ehci_readl(ehci, status_reg);
		if ((temp & PORT_RESET) || !(temp & PORT_PE)) {
			retval = -EPIPE;
			goto done;
		}

		if (!(temp & PORT_SUSPEND))
			goto done;

		/* Disable disconnect detection during port resume */
<<<<<<< HEAD
		tegra_usb_phy_preresume(hcd->phy);
=======
		tegra_usb_phy_preresume(hcd->usb_phy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		ehci->reset_done[wIndex-1] = jiffies + msecs_to_jiffies(25);

		temp &= ~(PORT_RWC_BITS | PORT_WAKE_BITS);
		/* start resume signalling */
		ehci_writel(ehci, temp | PORT_RESUME, status_reg);
		set_bit(wIndex-1, &ehci->resuming_ports);

		spin_unlock_irqrestore(&ehci->lock, flags);
		msleep(20);
		spin_lock_irqsave(&ehci->lock, flags);

		/* Poll until the controller clears RESUME and SUSPEND */
<<<<<<< HEAD
		if (handshake(ehci, status_reg, PORT_RESUME, 0, 2000))
			pr_err("%s: timeout waiting for RESUME\n", __func__);
		if (handshake(ehci, status_reg, PORT_SUSPEND, 0, 2000))
=======
		if (ehci_handshake(ehci, status_reg, PORT_RESUME, 0, 2000))
			pr_err("%s: timeout waiting for RESUME\n", __func__);
		if (ehci_handshake(ehci, status_reg, PORT_SUSPEND, 0, 2000))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			pr_err("%s: timeout waiting for SUSPEND\n", __func__);

		ehci->reset_done[wIndex-1] = 0;
		clear_bit(wIndex-1, &ehci->resuming_ports);

		tegra->port_resuming = 1;
		goto done;
	}

	spin_unlock_irqrestore(&ehci->lock, flags);

	/* Handle the hub control events here */
	return ehci_hub_control(hcd, typeReq, wValue, wIndex, buf, wLength);
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
done:
	spin_unlock_irqrestore(&ehci->lock, flags);
	return retval;
}

<<<<<<< HEAD
static void tegra_ehci_restart(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);

	ehci_reset(ehci);

	/* setup the frame list and Async q heads */
	ehci_writel(ehci, ehci->periodic_dma, &ehci->regs->frame_list);
	ehci_writel(ehci, (u32)ehci->async->qh_dma, &ehci->regs->async_next);
	/* setup the command register and set the controller in RUN mode */
	ehci->command &= ~(CMD_LRESET|CMD_IAAD|CMD_PSE|CMD_ASE|CMD_RESET);
	ehci->command |= CMD_RUN;
	ehci_writel(ehci, ehci->command, &ehci->regs->command);

	down_write(&ehci_cf_port_reset_rwsem);
	ehci_writel(ehci, FLAG_CF, &ehci->regs->configured_flag);
	/* flush posted writes */
	ehci_readl(ehci, &ehci->regs->command);
	up_write(&ehci_cf_port_reset_rwsem);
}

static void tegra_ehci_shutdown(struct usb_hcd *hcd)
{
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);

	/* ehci_shutdown touches the USB controller registers, make sure
	 * controller has clocks to it */
	if (!tegra->host_resumed)
		tegra_ehci_power_up(hcd);

	ehci_shutdown(hcd);
}

static int tegra_ehci_setup(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);

	/* EHCI registers start at offset 0x100 */
	ehci->caps = hcd->regs + 0x100;

	/* switch to host mode */
	hcd->has_tt = 1;

	return ehci_setup(hcd);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct dma_aligned_buffer {
	void *kmalloc_ptr;
	void *old_xfer_buffer;
	u8 data[0];
};

static void free_dma_aligned_buffer(struct urb *urb)
{
	struct dma_aligned_buffer *temp;
<<<<<<< HEAD
=======
	size_t length;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!(urb->transfer_flags & URB_ALIGNED_TEMP_BUFFER))
		return;

	temp = container_of(urb->transfer_buffer,
		struct dma_aligned_buffer, data);

<<<<<<< HEAD
	if (usb_urb_dir_in(urb))
		memcpy(temp->old_xfer_buffer, temp->data,
		       urb->transfer_buffer_length);
=======
	if (usb_urb_dir_in(urb)) {
		if (usb_pipeisoc(urb->pipe))
			length = urb->transfer_buffer_length;
		else
			length = urb->actual_length;

		memcpy(temp->old_xfer_buffer, temp->data, length);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	urb->transfer_buffer = temp->old_xfer_buffer;
	kfree(temp->kmalloc_ptr);

	urb->transfer_flags &= ~URB_ALIGNED_TEMP_BUFFER;
}

static int alloc_dma_aligned_buffer(struct urb *urb, gfp_t mem_flags)
{
	struct dma_aligned_buffer *temp, *kmalloc_ptr;
	size_t kmalloc_size;

	if (urb->num_sgs || urb->sg ||
	    urb->transfer_buffer_length == 0 ||
	    !((uintptr_t)urb->transfer_buffer & (TEGRA_USB_DMA_ALIGN - 1)))
		return 0;

	/* Allocate a buffer with enough padding for alignment */
	kmalloc_size = urb->transfer_buffer_length +
		sizeof(struct dma_aligned_buffer) + TEGRA_USB_DMA_ALIGN - 1;

	kmalloc_ptr = kmalloc(kmalloc_size, mem_flags);
	if (!kmalloc_ptr)
		return -ENOMEM;

	/* Position our struct dma_aligned_buffer such that data is aligned */
	temp = PTR_ALIGN(kmalloc_ptr + 1, TEGRA_USB_DMA_ALIGN) - 1;
	temp->kmalloc_ptr = kmalloc_ptr;
	temp->old_xfer_buffer = urb->transfer_buffer;
	if (usb_urb_dir_out(urb))
		memcpy(temp->data, urb->transfer_buffer,
		       urb->transfer_buffer_length);
	urb->transfer_buffer = temp->data;

	urb->transfer_flags |= URB_ALIGNED_TEMP_BUFFER;

	return 0;
}

static int tegra_ehci_map_urb_for_dma(struct usb_hcd *hcd, struct urb *urb,
				      gfp_t mem_flags)
{
	int ret;

	ret = alloc_dma_aligned_buffer(urb, mem_flags);
	if (ret)
		return ret;

	ret = usb_hcd_map_urb_for_dma(hcd, urb, mem_flags);
	if (ret)
		free_dma_aligned_buffer(urb);

	return ret;
}

static void tegra_ehci_unmap_urb_for_dma(struct usb_hcd *hcd, struct urb *urb)
{
	usb_hcd_unmap_urb_for_dma(hcd, urb);
	free_dma_aligned_buffer(urb);
}

<<<<<<< HEAD
static const struct hc_driver tegra_ehci_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "Tegra EHCI Host Controller",
	.hcd_priv_size		= sizeof(struct ehci_hcd),
	.flags			= HCD_USB2 | HCD_MEMORY,

	/* standard ehci functions */
	.irq			= ehci_irq,
	.start			= ehci_run,
	.stop			= ehci_stop,
	.urb_enqueue		= ehci_urb_enqueue,
	.urb_dequeue		= ehci_urb_dequeue,
	.endpoint_disable	= ehci_endpoint_disable,
	.endpoint_reset		= ehci_endpoint_reset,
	.get_frame_number	= ehci_get_frame,
	.hub_status_data	= ehci_hub_status_data,
	.clear_tt_buffer_complete = ehci_clear_tt_buffer_complete,
	.relinquish_port	= ehci_relinquish_port,
	.port_handed_over	= ehci_port_handed_over,

	/* modified ehci functions for tegra */
	.reset			= tegra_ehci_setup,
	.shutdown		= tegra_ehci_shutdown,
	.map_urb_for_dma	= tegra_ehci_map_urb_for_dma,
	.unmap_urb_for_dma	= tegra_ehci_unmap_urb_for_dma,
	.hub_control		= tegra_ehci_hub_control,
#ifdef CONFIG_PM
	.bus_suspend		= ehci_bus_suspend,
	.bus_resume		= ehci_bus_resume,
#endif
};

static int setup_vbus_gpio(struct platform_device *pdev,
			   struct tegra_ehci_platform_data *pdata)
{
	int err = 0;
	int gpio;

	gpio = pdata->vbus_gpio;
	if (!gpio_is_valid(gpio))
		gpio = of_get_named_gpio(pdev->dev.of_node,
					 "nvidia,vbus-gpio", 0);
	if (!gpio_is_valid(gpio))
		return 0;

	err = gpio_request(gpio, "vbus_gpio");
	if (err) {
		dev_err(&pdev->dev, "can't request vbus gpio %d", gpio);
		return err;
	}
	err = gpio_direction_output(gpio, 1);
	if (err) {
		dev_err(&pdev->dev, "can't enable vbus\n");
		return err;
	}

	return err;
}

#ifdef CONFIG_PM

static int controller_suspend(struct device *dev)
{
	struct tegra_ehci_hcd *tegra =
			platform_get_drvdata(to_platform_device(dev));
	struct ehci_hcd	*ehci = tegra->ehci;
	struct usb_hcd *hcd = ehci_to_hcd(ehci);
	struct ehci_regs __iomem *hw = ehci->regs;
	unsigned long flags;

	if (time_before(jiffies, ehci->next_statechange))
		msleep(10);

	ehci_halt(ehci);

	spin_lock_irqsave(&ehci->lock, flags);
	tegra->port_speed = (readl(&hw->port_status[0]) >> 26) & 0x3;
	clear_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);
	spin_unlock_irqrestore(&ehci->lock, flags);

	tegra_ehci_power_down(hcd);
	return 0;
}

static int controller_resume(struct device *dev)
{
	struct tegra_ehci_hcd *tegra =
			platform_get_drvdata(to_platform_device(dev));
	struct ehci_hcd	*ehci = tegra->ehci;
	struct usb_hcd *hcd = ehci_to_hcd(ehci);
	struct ehci_regs __iomem *hw = ehci->regs;
	unsigned long val;

	set_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);
	tegra_ehci_power_up(hcd);

	if (tegra->port_speed > TEGRA_USB_PHY_PORT_SPEED_HIGH) {
		/* Wait for the phy to detect new devices
		 * before we restart the controller */
		msleep(10);
		goto restart;
	}

	/* Force the phy to keep data lines in suspend state */
	tegra_ehci_phy_restore_start(hcd->phy, tegra->port_speed);

	/* Enable host mode */
	tdi_reset(ehci);

	/* Enable Port Power */
	val = readl(&hw->port_status[0]);
	val |= PORT_POWER;
	writel(val, &hw->port_status[0]);
	udelay(10);

	/* Check if the phy resume from LP0. When the phy resume from LP0
	 * USB register will be reset. */
	if (!readl(&hw->async_next)) {
		/* Program the field PTC based on the saved speed mode */
		val = readl(&hw->port_status[0]);
		val &= ~PORT_TEST(~0);
		if (tegra->port_speed == TEGRA_USB_PHY_PORT_SPEED_HIGH)
			val |= PORT_TEST_FORCE;
		else if (tegra->port_speed == TEGRA_USB_PHY_PORT_SPEED_FULL)
			val |= PORT_TEST(6);
		else if (tegra->port_speed == TEGRA_USB_PHY_PORT_SPEED_LOW)
			val |= PORT_TEST(7);
		writel(val, &hw->port_status[0]);
		udelay(10);

		/* Disable test mode by setting PTC field to NORMAL_OP */
		val = readl(&hw->port_status[0]);
		val &= ~PORT_TEST(~0);
		writel(val, &hw->port_status[0]);
		udelay(10);
	}

	/* Poll until CCS is enabled */
	if (handshake(ehci, &hw->port_status[0], PORT_CONNECT,
						 PORT_CONNECT, 2000)) {
		pr_err("%s: timeout waiting for PORT_CONNECT\n", __func__);
		goto restart;
	}

	/* Poll until PE is enabled */
	if (handshake(ehci, &hw->port_status[0], PORT_PE,
						 PORT_PE, 2000)) {
		pr_err("%s: timeout waiting for USB_PORTSC1_PE\n", __func__);
		goto restart;
	}

	/* Clear the PCI status, to avoid an interrupt taken upon resume */
	val = readl(&hw->status);
	val |= STS_PCD;
	writel(val, &hw->status);

	/* Put controller in suspend mode by writing 1 to SUSP bit of PORTSC */
	val = readl(&hw->port_status[0]);
	if ((val & PORT_POWER) && (val & PORT_PE)) {
		val |= PORT_SUSPEND;
		writel(val, &hw->port_status[0]);

		/* Wait until port suspend completes */
		if (handshake(ehci, &hw->port_status[0], PORT_SUSPEND,
							 PORT_SUSPEND, 1000)) {
			pr_err("%s: timeout waiting for PORT_SUSPEND\n",
								__func__);
			goto restart;
		}
	}

	tegra_ehci_phy_restore_end(hcd->phy);
	goto done;

 restart:
	if (tegra->port_speed <= TEGRA_USB_PHY_PORT_SPEED_HIGH)
		tegra_ehci_phy_restore_end(hcd->phy);

	tegra_ehci_restart(hcd);

 done:
	tegra_usb_phy_preresume(hcd->phy);
	tegra->port_resuming = 1;
	return 0;
}

static int tegra_ehci_suspend(struct device *dev)
{
	struct tegra_ehci_hcd *tegra =
			platform_get_drvdata(to_platform_device(dev));
	struct usb_hcd *hcd = ehci_to_hcd(tegra->ehci);
	int rc = 0;

	/*
	 * When system sleep is supported and USB controller wakeup is
	 * implemented: If the controller is runtime-suspended and the
	 * wakeup setting needs to be changed, call pm_runtime_resume().
	 */
	if (HCD_HW_ACCESSIBLE(hcd))
		rc = controller_suspend(dev);
	return rc;
}

static int tegra_ehci_resume(struct device *dev)
{
	int rc;

	rc = controller_resume(dev);
	if (rc == 0) {
		pm_runtime_disable(dev);
		pm_runtime_set_active(dev);
		pm_runtime_enable(dev);
	}
	return rc;
}

static int tegra_ehci_runtime_suspend(struct device *dev)
{
	return controller_suspend(dev);
}

static int tegra_ehci_runtime_resume(struct device *dev)
{
	return controller_resume(dev);
}

static const struct dev_pm_ops tegra_ehci_pm_ops = {
	.suspend	= tegra_ehci_suspend,
	.resume		= tegra_ehci_resume,
	.runtime_suspend = tegra_ehci_runtime_suspend,
	.runtime_resume	= tegra_ehci_runtime_resume,
};

#endif

/* Bits of PORTSC1, which will get cleared by writing 1 into them */
#define TEGRA_PORTSC1_RWC_BITS (PORT_CSC | PORT_PEC | PORT_OCC)

static void tegra_ehci_set_pts(struct usb_phy *x, u8 pts_val)
{
	unsigned long val;
	struct usb_hcd *hcd = bus_to_hcd(x->otg->host);
	void __iomem *base = hcd->regs;

	val = readl(base + TEGRA_USB_PORTSC1) & ~TEGRA_PORTSC1_RWC_BITS;
	val &= ~TEGRA_USB_PORTSC1_PTS(3);
	val |= TEGRA_USB_PORTSC1_PTS(pts_val & 3);
	writel(val, base + TEGRA_USB_PORTSC1);
}

static void tegra_ehci_set_phcd(struct usb_phy *x, bool enable)
{
	unsigned long val;
	struct usb_hcd *hcd = bus_to_hcd(x->otg->host);
	void __iomem *base = hcd->regs;

	val = readl(base + TEGRA_USB_PORTSC1) & ~TEGRA_PORTSC1_RWC_BITS;
	if (enable)
		val |= TEGRA_USB_PORTSC1_PHCD;
	else
		val &= ~TEGRA_USB_PORTSC1_PHCD;
	writel(val, base + TEGRA_USB_PORTSC1);
}

static int tegra_ehci_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct usb_hcd *hcd;
	struct tegra_ehci_hcd *tegra;
	struct tegra_ehci_platform_data *pdata;
	int err = 0;
	int irq;
	int instance = pdev->id;
	struct usb_phy *u_phy;

	pdata = pdev->dev.platform_data;
	if (!pdata) {
		dev_err(&pdev->dev, "Platform data missing\n");
		return -EINVAL;
	}
=======
static const struct tegra_ehci_soc_config tegra30_soc_config = {
	.has_hostpc = true,
};

static const struct tegra_ehci_soc_config tegra20_soc_config = {
	.has_hostpc = false,
};

static const struct of_device_id tegra_ehci_of_match[] = {
	{ .compatible = "nvidia,tegra30-ehci", .data = &tegra30_soc_config },
	{ .compatible = "nvidia,tegra20-ehci", .data = &tegra20_soc_config },
	{ },
};

static int tegra_ehci_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;
	const struct tegra_ehci_soc_config *soc_config;
	struct resource *res;
	struct usb_hcd *hcd;
	struct ehci_hcd *ehci;
	struct tegra_ehci_hcd *tegra;
	int err = 0;
	int irq;
	struct usb_phy *u_phy;

	match = of_match_device(tegra_ehci_of_match, &pdev->dev);
	if (!match) {
		dev_err(&pdev->dev, "Error: No device match found\n");
		return -ENODEV;
	}
	soc_config = match->data;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Right now device-tree probed devices don't get dma_mask set.
	 * Since shared usb code relies on it, set it here for now.
	 * Once we have dma capability bindings this can go away.
	 */
<<<<<<< HEAD
	if (!pdev->dev.dma_mask)
		pdev->dev.dma_mask = &pdev->dev.coherent_dma_mask;
	if (!pdev->dev.coherent_dma_mask)
		pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);

	setup_vbus_gpio(pdev, pdata);

	tegra = devm_kzalloc(&pdev->dev, sizeof(struct tegra_ehci_hcd),
			     GFP_KERNEL);
	if (!tegra)
		return -ENOMEM;
=======
	err = dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	if (err)
		return err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	hcd = usb_create_hcd(&tegra_ehci_hc_driver, &pdev->dev,
					dev_name(&pdev->dev));
	if (!hcd) {
		dev_err(&pdev->dev, "Unable to create HCD\n");
		return -ENOMEM;
	}
<<<<<<< HEAD

	platform_set_drvdata(pdev, tegra);
=======
	platform_set_drvdata(pdev, hcd);
	ehci = hcd_to_ehci(hcd);
	tegra = (struct tegra_ehci_hcd *)ehci->priv;

	hcd->has_tt = 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	tegra->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(tegra->clk)) {
		dev_err(&pdev->dev, "Can't get ehci clock\n");
		err = PTR_ERR(tegra->clk);
<<<<<<< HEAD
		goto fail_clk;
=======
		goto cleanup_hcd_create;
	}

	tegra->rst = devm_reset_control_get(&pdev->dev, "usb");
	if (IS_ERR(tegra->rst)) {
		dev_err(&pdev->dev, "Can't get ehci reset\n");
		err = PTR_ERR(tegra->rst);
		goto cleanup_hcd_create;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	err = clk_prepare_enable(tegra->clk);
	if (err)
<<<<<<< HEAD
		goto fail_clk;

	tegra_periph_reset_assert(tegra->clk);
	udelay(1);
	tegra_periph_reset_deassert(tegra->clk);
=======
		goto cleanup_hcd_create;

	err = tegra_reset_usb_controller(pdev);
	if (err)
		goto cleanup_clk_en;

	u_phy = devm_usb_get_phy_by_phandle(&pdev->dev, "nvidia,phy", 0);
	if (IS_ERR(u_phy)) {
		err = -EPROBE_DEFER;
		goto cleanup_clk_en;
	}
	hcd->usb_phy = u_phy;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	tegra->needs_double_reset = of_property_read_bool(pdev->dev.of_node,
		"nvidia,needs-double-reset");

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD
	if (!res) {
		dev_err(&pdev->dev, "Failed to get I/O memory\n");
		err = -ENXIO;
		goto fail_io;
	}
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);
	hcd->regs = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (!hcd->regs) {
		dev_err(&pdev->dev, "Failed to remap I/O memory\n");
		err = -ENOMEM;
		goto fail_io;
	}

	/* This is pretty ugly and needs to be fixed when we do only
	 * device-tree probing. Old code relies on the platform_device
	 * numbering that we lack for device-tree-instantiated devices.
	 */
	if (instance < 0) {
		switch (res->start) {
		case TEGRA_USB_BASE:
			instance = 0;
			break;
		case TEGRA_USB2_BASE:
			instance = 1;
			break;
		case TEGRA_USB3_BASE:
			instance = 2;
			break;
		default:
			err = -ENODEV;
			dev_err(&pdev->dev, "unknown usb instance\n");
			goto fail_io;
		}
	}

	tegra->phy = tegra_usb_phy_open(&pdev->dev, instance, hcd->regs,
					pdata->phy_config,
					TEGRA_USB_PHY_MODE_HOST,
					tegra_ehci_set_pts,
					tegra_ehci_set_phcd);
	if (IS_ERR(tegra->phy)) {
		dev_err(&pdev->dev, "Failed to open USB phy\n");
		err = -ENXIO;
		goto fail_io;
	}

	hcd->phy = u_phy = &tegra->phy->u_phy;
	usb_phy_init(hcd->phy);

	u_phy->otg = devm_kzalloc(&pdev->dev, sizeof(struct usb_otg),
			     GFP_KERNEL);
	if (!u_phy->otg) {
		dev_err(&pdev->dev, "Failed to alloc memory for otg\n");
		err = -ENOMEM;
		goto fail_io;
	}
	u_phy->otg->host = hcd_to_bus(hcd);

	err = usb_phy_set_suspend(hcd->phy, 0);
	if (err) {
		dev_err(&pdev->dev, "Failed to power on the phy\n");
		goto fail_phy;
	}

	tegra->host_resumed = 1;
	tegra->ehci = hcd_to_ehci(hcd);

=======
	hcd->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(hcd->regs)) {
		err = PTR_ERR(hcd->regs);
		goto cleanup_clk_en;
	}
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);

	ehci->caps = hcd->regs + 0x100;
	ehci->has_hostpc = soc_config->has_hostpc;

	err = usb_phy_init(hcd->usb_phy);
	if (err) {
		dev_err(&pdev->dev, "Failed to initialize phy\n");
		goto cleanup_clk_en;
	}

	u_phy->otg = devm_kzalloc(&pdev->dev, sizeof(struct usb_otg),
			     GFP_KERNEL);
	if (!u_phy->otg) {
		err = -ENOMEM;
		goto cleanup_phy;
	}
	u_phy->otg->host = hcd_to_bus(hcd);

	err = usb_phy_set_suspend(hcd->usb_phy, 0);
	if (err) {
		dev_err(&pdev->dev, "Failed to power on the phy\n");
		goto cleanup_phy;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	irq = platform_get_irq(pdev, 0);
	if (!irq) {
		dev_err(&pdev->dev, "Failed to get IRQ\n");
		err = -ENODEV;
<<<<<<< HEAD
		goto fail_phy;
	}

	if (pdata->operating_mode == TEGRA_USB_OTG) {
		tegra->transceiver =
			devm_usb_get_phy(&pdev->dev, USB_PHY_TYPE_USB2);
		if (!IS_ERR(tegra->transceiver))
			otg_set_host(tegra->transceiver->otg, &hcd->self);
	} else {
		tegra->transceiver = ERR_PTR(-ENODEV);
	}
=======
		goto cleanup_phy;
	}

	otg_set_host(u_phy->otg, &hcd->self);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	err = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (err) {
		dev_err(&pdev->dev, "Failed to add USB HCD\n");
<<<<<<< HEAD
		goto fail;
	}

	pm_runtime_set_active(&pdev->dev);
	pm_runtime_get_noresume(&pdev->dev);

	/* Don't skip the pm_runtime_forbid call if wakeup isn't working */
	/* if (!pdata->power_down_on_bus_suspend) */
		pm_runtime_forbid(&pdev->dev);
	pm_runtime_enable(&pdev->dev);
	pm_runtime_put_sync(&pdev->dev);
	return err;

fail:
	if (!IS_ERR(tegra->transceiver))
		otg_set_host(tegra->transceiver->otg, NULL);
fail_phy:
	usb_phy_shutdown(hcd->phy);
fail_io:
	clk_disable_unprepare(tegra->clk);
fail_clk:
=======
		goto cleanup_otg_set_host;
	}
	device_wakeup_enable(hcd->self.controller);

	return err;

cleanup_otg_set_host:
	otg_set_host(u_phy->otg, NULL);
cleanup_phy:
	usb_phy_shutdown(hcd->usb_phy);
cleanup_clk_en:
	clk_disable_unprepare(tegra->clk);
cleanup_hcd_create:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	usb_put_hcd(hcd);
	return err;
}

static int tegra_ehci_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct tegra_ehci_hcd *tegra = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = ehci_to_hcd(tegra->ehci);

	pm_runtime_get_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
	pm_runtime_put_noidle(&pdev->dev);

	if (!IS_ERR(tegra->transceiver))
		otg_set_host(tegra->transceiver->otg, NULL);

	usb_phy_shutdown(hcd->phy);
	usb_remove_hcd(hcd);
	usb_put_hcd(hcd);

	clk_disable_unprepare(tegra->clk);

=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct tegra_ehci_hcd *tegra =
		(struct tegra_ehci_hcd *)hcd_to_ehci(hcd)->priv;

	otg_set_host(hcd->usb_phy->otg, NULL);

	usb_phy_shutdown(hcd->usb_phy);
	usb_remove_hcd(hcd);

	clk_disable_unprepare(tegra->clk);

	usb_put_hcd(hcd);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static void tegra_ehci_hcd_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct tegra_ehci_hcd *tegra = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = ehci_to_hcd(tegra->ehci);
=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);
}

<<<<<<< HEAD
static struct of_device_id tegra_ehci_of_match[] = {
	{ .compatible = "nvidia,tegra20-ehci", },
	{ },
};

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct platform_driver tegra_ehci_driver = {
	.probe		= tegra_ehci_probe,
	.remove		= tegra_ehci_remove,
	.shutdown	= tegra_ehci_hcd_shutdown,
	.driver		= {
<<<<<<< HEAD
		.name	= "tegra-ehci",
		.of_match_table = tegra_ehci_of_match,
#ifdef CONFIG_PM
		.pm	= &tegra_ehci_pm_ops,
#endif
	}
};
=======
		.name	= DRV_NAME,
		.of_match_table = tegra_ehci_of_match,
	}
};

static int tegra_ehci_reset(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval;
	int txfifothresh;

	retval = ehci_setup(hcd);
	if (retval)
		return retval;

	/*
	 * We should really pull this value out of tegra_ehci_soc_config, but
	 * to avoid needing access to it, make use of the fact that Tegra20 is
	 * the only one so far that needs a value of 10, and Tegra20 is the
	 * only one which doesn't set has_hostpc.
	 */
	txfifothresh = ehci->has_hostpc ? 0x10 : 10;
	ehci_writel(ehci, txfifothresh << 16, &ehci->regs->txfill_tuning);

	return 0;
}

static const struct ehci_driver_overrides tegra_overrides __initconst = {
	.extra_priv_size	= sizeof(struct tegra_ehci_hcd),
	.reset			= tegra_ehci_reset,
};

static int __init ehci_tegra_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info(DRV_NAME ": " DRIVER_DESC "\n");

	ehci_init_driver(&tegra_ehci_hc_driver, &tegra_overrides);

	/*
	 * The Tegra HW has some unusual quirks, which require Tegra-specific
	 * workarounds. We override certain hc_driver functions here to
	 * achieve that. We explicitly do not enhance ehci_driver_overrides to
	 * allow this more easily, since this is an unusual case, and we don't
	 * want to encourage others to override these functions by making it
	 * too easy.
	 */

	tegra_ehci_hc_driver.map_urb_for_dma = tegra_ehci_map_urb_for_dma;
	tegra_ehci_hc_driver.unmap_urb_for_dma = tegra_ehci_unmap_urb_for_dma;
	tegra_ehci_hc_driver.hub_control = tegra_ehci_hub_control;

	return platform_driver_register(&tegra_ehci_driver);
}
module_init(ehci_tegra_init);

static void __exit ehci_tegra_cleanup(void)
{
	platform_driver_unregister(&tegra_ehci_driver);
}
module_exit(ehci_tegra_cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
MODULE_DEVICE_TABLE(of, tegra_ehci_of_match);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
