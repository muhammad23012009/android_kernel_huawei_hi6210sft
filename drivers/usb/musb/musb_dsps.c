/*
 * Texas Instruments DSPS platforms "glue layer"
 *
 * Copyright (C) 2012, by Texas Instruments
 *
 * Based on the am35x "glue layer" code.
 *
 * This file is part of the Inventra Controller Driver for Linux.
 *
 * The Inventra Controller Driver for Linux is free software; you
 * can redistribute it and/or modify it under the terms of the GNU
 * General Public License version 2 as published by the Free Software
 * Foundation.
 *
 * The Inventra Controller Driver for Linux is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Inventra Controller Driver for Linux ; if not,
 * write to the Free Software Foundation, Inc., 59 Temple Place,
 * Suite 330, Boston, MA  02111-1307  USA
 *
 * musb_dsps.c will be a common file for all the TI DSPS platforms
 * such as dm64x, dm36x, dm35x, da8x, am35x and ti81x.
 * For now only ti81x is using this and in future davinci.c, am35x.c
 * da8xx.c would be merged to this file after testing.
 */

<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/io.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/pm_runtime.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/usb/nop-usb-xceiv.h>
=======
#include <linux/usb/usb_phy_generic.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/platform_data/usb-omap.h>
#include <linux/sizes.h>

#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
<<<<<<< HEAD

#include "musb_core.h"

#ifdef CONFIG_OF
static const struct of_device_id musb_dsps_of_match[];
#endif

/**
 * avoid using musb_readx()/musb_writex() as glue layer should not be
 * dependent on musb core layer symbols.
 */
static inline u8 dsps_readb(const void __iomem *addr, unsigned offset)
	{ return __raw_readb(addr + offset); }

static inline u32 dsps_readl(const void __iomem *addr, unsigned offset)
	{ return __raw_readl(addr + offset); }

static inline void dsps_writeb(void __iomem *addr, unsigned offset, u8 data)
	{ __raw_writeb(data, addr + offset); }

static inline void dsps_writel(void __iomem *addr, unsigned offset, u32 data)
	{ __raw_writel(data, addr + offset); }
=======
#include <linux/of_irq.h>
#include <linux/usb/of.h>

#include <linux/debugfs.h>

#include "musb_core.h"

static const struct of_device_id musb_dsps_of_match[];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * DSPS musb wrapper register offset.
 * FIXME: This should be expanded to have all the wrapper registers from TI DSPS
 * musb ips.
 */
struct dsps_musb_wrapper {
	u16	revision;
	u16	control;
	u16	status;
<<<<<<< HEAD
	u16	eoi;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u16	epintr_set;
	u16	epintr_clear;
	u16	epintr_status;
	u16	coreintr_set;
	u16	coreintr_clear;
	u16	coreintr_status;
	u16	phy_utmi;
	u16	mode;
<<<<<<< HEAD
=======
	u16	tx_mode;
	u16	rx_mode;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* bit positions for control */
	unsigned	reset:5;

	/* bit positions for interrupt */
	unsigned	usb_shift:5;
	u32		usb_mask;
	u32		usb_bitmap;
	unsigned	drvvbus:5;

	unsigned	txep_shift:5;
	u32		txep_mask;
	u32		txep_bitmap;

	unsigned	rxep_shift:5;
	u32		rxep_mask;
	u32		rxep_bitmap;

	/* bit positions for phy_utmi */
	unsigned	otg_disable:5;

	/* bit positions for mode */
	unsigned	iddig:5;
<<<<<<< HEAD
	/* miscellaneous stuff */
	u32		musb_core_offset;
	u8		poll_seconds;
	/* number of musb instances */
	u8		instances;
=======
	unsigned	iddig_mux:5;
	/* miscellaneous stuff */
	unsigned	poll_timeout;
};

/*
 * register shadow for suspend
 */
struct dsps_context {
	u32 control;
	u32 epintr;
	u32 coreintr;
	u32 phy_utmi;
	u32 mode;
	u32 tx_mode;
	u32 rx_mode;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/**
 * DSPS glue structure.
 */
struct dsps_glue {
	struct device *dev;
<<<<<<< HEAD
	struct platform_device *musb[2];	/* child musb pdev */
	const struct dsps_musb_wrapper *wrp; /* wrapper register offsets */
	struct timer_list timer[2];	/* otg_workaround timer */
	unsigned long last_timer[2];    /* last timer data for each instance */
	u32 __iomem *usb_ctrl[2];
};

#define	DSPS_AM33XX_CONTROL_MODULE_PHYS_0	0x44e10620
#define	DSPS_AM33XX_CONTROL_MODULE_PHYS_1	0x44e10628

static const resource_size_t dsps_control_module_phys[] = {
	DSPS_AM33XX_CONTROL_MODULE_PHYS_0,
	DSPS_AM33XX_CONTROL_MODULE_PHYS_1,
};

#define USBPHY_CM_PWRDN		(1 << 0)
#define USBPHY_OTG_PWRDN	(1 << 1)
#define USBPHY_OTGVDET_EN	(1 << 19)
#define USBPHY_OTGSESSEND_EN	(1 << 20)

/**
 * musb_dsps_phy_control - phy on/off
 * @glue: struct dsps_glue *
 * @id: musb instance
 * @on: flag for phy to be switched on or off
 *
 * This is to enable the PHY using usb_ctrl register in system control
 * module space.
 *
 * XXX: This function will be removed once we have a seperate driver for
 * control module
 */
static void musb_dsps_phy_control(struct dsps_glue *glue, u8 id, u8 on)
{
	u32 usbphycfg;

	usbphycfg = readl(glue->usb_ctrl[id]);

	if (on) {
		usbphycfg &= ~(USBPHY_CM_PWRDN | USBPHY_OTG_PWRDN);
		usbphycfg |= USBPHY_OTGVDET_EN | USBPHY_OTGSESSEND_EN;
	} else {
		usbphycfg |= USBPHY_CM_PWRDN | USBPHY_OTG_PWRDN;
	}

	writel(usbphycfg, glue->usb_ctrl[id]);
}
=======
	struct platform_device *musb;	/* child musb pdev */
	const struct dsps_musb_wrapper *wrp; /* wrapper register offsets */
	struct timer_list timer;	/* otg_workaround timer */
	unsigned long last_timer;    /* last timer data for each instance */
	bool sw_babble_enabled;

	struct dsps_context context;
	struct debugfs_regset32 regset;
	struct dentry *dbgfs_root;
};

static const struct debugfs_reg32 dsps_musb_regs[] = {
	{ "revision",		0x00 },
	{ "control",		0x14 },
	{ "status",		0x18 },
	{ "eoi",		0x24 },
	{ "intr0_stat",		0x30 },
	{ "intr1_stat",		0x34 },
	{ "intr0_set",		0x38 },
	{ "intr1_set",		0x3c },
	{ "txmode",		0x70 },
	{ "rxmode",		0x74 },
	{ "autoreq",		0xd0 },
	{ "srpfixtime",		0xd4 },
	{ "tdown",		0xd8 },
	{ "phy_utmi",		0xe0 },
	{ "mode",		0xe8 },
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * dsps_musb_enable - enable interrupts
 */
static void dsps_musb_enable(struct musb *musb)
{
	struct device *dev = musb->controller;
	struct platform_device *pdev = to_platform_device(dev->parent);
	struct dsps_glue *glue = platform_get_drvdata(pdev);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	void __iomem *reg_base = musb->ctrl_base;
	u32 epmask, coremask;

	/* Workaround: setup IRQs through both register sets. */
	epmask = ((musb->epmask & wrp->txep_mask) << wrp->txep_shift) |
	       ((musb->epmask & wrp->rxep_mask) << wrp->rxep_shift);
	coremask = (wrp->usb_bitmap & ~MUSB_INTR_SOF);

<<<<<<< HEAD
	dsps_writel(reg_base, wrp->epintr_set, epmask);
	dsps_writel(reg_base, wrp->coreintr_set, coremask);
	/* Force the DRVVBUS IRQ so we can start polling for ID change. */
	dsps_writel(reg_base, wrp->coreintr_set,
		    (1 << wrp->drvvbus) << wrp->usb_shift);
=======
	musb_writel(reg_base, wrp->epintr_set, epmask);
	musb_writel(reg_base, wrp->coreintr_set, coremask);
	/* start polling for ID change in dual-role idle mode */
	if (musb->xceiv->otg->state == OTG_STATE_B_IDLE &&
			musb->port_mode == MUSB_PORT_MODE_DUAL_ROLE)
		mod_timer(&glue->timer, jiffies +
				msecs_to_jiffies(wrp->poll_timeout));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * dsps_musb_disable - disable HDRC and flush interrupts
 */
static void dsps_musb_disable(struct musb *musb)
{
	struct device *dev = musb->controller;
	struct platform_device *pdev = to_platform_device(dev->parent);
	struct dsps_glue *glue = platform_get_drvdata(pdev);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	void __iomem *reg_base = musb->ctrl_base;

<<<<<<< HEAD
	dsps_writel(reg_base, wrp->coreintr_clear, wrp->usb_bitmap);
	dsps_writel(reg_base, wrp->epintr_clear,
			 wrp->txep_bitmap | wrp->rxep_bitmap);
	dsps_writeb(musb->mregs, MUSB_DEVCTL, 0);
	dsps_writel(reg_base, wrp->eoi, 0);
}

static void otg_timer(unsigned long _musb)
{
	struct musb *musb = (void *)_musb;
	void __iomem *mregs = musb->mregs;
	struct device *dev = musb->controller;
	struct platform_device *pdev = to_platform_device(dev);
	struct dsps_glue *glue = dev_get_drvdata(dev->parent);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	u8 devctl;
	unsigned long flags;
=======
	musb_writel(reg_base, wrp->coreintr_clear, wrp->usb_bitmap);
	musb_writel(reg_base, wrp->epintr_clear,
			 wrp->txep_bitmap | wrp->rxep_bitmap);
	del_timer_sync(&glue->timer);
	musb_writeb(musb->mregs, MUSB_DEVCTL, 0);
}

/* Caller must take musb->lock */
static int dsps_check_status(struct musb *musb, void *unused)
{
	void __iomem *mregs = musb->mregs;
	struct device *dev = musb->controller;
	struct dsps_glue *glue = dev_get_drvdata(dev->parent);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	u8 devctl;
	int skip_session = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * We poll because DSPS IP's won't expose several OTG-critical
	 * status change events (from the transceiver) otherwise.
	 */
<<<<<<< HEAD
	devctl = dsps_readb(mregs, MUSB_DEVCTL);
	dev_dbg(musb->controller, "Poll devctl %02x (%s)\n", devctl,
				usb_otg_state_string(musb->xceiv->state));

	spin_lock_irqsave(&musb->lock, flags);
	switch (musb->xceiv->state) {
	case OTG_STATE_A_WAIT_BCON:
		devctl &= ~MUSB_DEVCTL_SESSION;
		dsps_writeb(musb->mregs, MUSB_DEVCTL, devctl);

		devctl = dsps_readb(musb->mregs, MUSB_DEVCTL);
		if (devctl & MUSB_DEVCTL_BDEVICE) {
			musb->xceiv->state = OTG_STATE_B_IDLE;
			MUSB_DEV_MODE(musb);
		} else {
			musb->xceiv->state = OTG_STATE_A_IDLE;
			MUSB_HST_MODE(musb);
		}
		break;
	case OTG_STATE_A_WAIT_VFALL:
		musb->xceiv->state = OTG_STATE_A_WAIT_VRISE;
		dsps_writel(musb->ctrl_base, wrp->coreintr_set,
			    MUSB_INTR_VBUSERROR << wrp->usb_shift);
		break;
	case OTG_STATE_B_IDLE:
		devctl = dsps_readb(mregs, MUSB_DEVCTL);
		if (devctl & MUSB_DEVCTL_BDEVICE)
			mod_timer(&glue->timer[pdev->id],
					jiffies + wrp->poll_seconds * HZ);
		else
			musb->xceiv->state = OTG_STATE_A_IDLE;
		break;
	default:
		break;
	}
	spin_unlock_irqrestore(&musb->lock, flags);
}

static void dsps_musb_try_idle(struct musb *musb, unsigned long timeout)
{
	struct device *dev = musb->controller;
	struct platform_device *pdev = to_platform_device(dev);
	struct dsps_glue *glue = dev_get_drvdata(dev->parent);

	if (timeout == 0)
		timeout = jiffies + msecs_to_jiffies(3);

	/* Never idle if active, or when VBUS timeout is not set as host */
	if (musb->is_active || (musb->a_wait_bcon == 0 &&
				musb->xceiv->state == OTG_STATE_A_WAIT_BCON)) {
		dev_dbg(musb->controller, "%s active, deleting timer\n",
				usb_otg_state_string(musb->xceiv->state));
		del_timer(&glue->timer[pdev->id]);
		glue->last_timer[pdev->id] = jiffies;
		return;
	}

	if (time_after(glue->last_timer[pdev->id], timeout) &&
				timer_pending(&glue->timer[pdev->id])) {
		dev_dbg(musb->controller,
			"Longer idle timer already pending, ignoring...\n");
		return;
	}
	glue->last_timer[pdev->id] = timeout;

	dev_dbg(musb->controller, "%s inactive, starting idle timer for %u ms\n",
		usb_otg_state_string(musb->xceiv->state),
			jiffies_to_msecs(timeout - jiffies));
	mod_timer(&glue->timer[pdev->id], timeout);
=======
	devctl = musb_readb(mregs, MUSB_DEVCTL);
	dev_dbg(musb->controller, "Poll devctl %02x (%s)\n", devctl,
				usb_otg_state_string(musb->xceiv->otg->state));

	switch (musb->xceiv->otg->state) {
	case OTG_STATE_A_WAIT_VRISE:
		mod_timer(&glue->timer, jiffies +
				msecs_to_jiffies(wrp->poll_timeout));
		break;
	case OTG_STATE_A_WAIT_BCON:
		/* keep VBUS on for host-only mode */
		if (musb->port_mode == MUSB_PORT_MODE_HOST) {
			mod_timer(&glue->timer, jiffies +
					msecs_to_jiffies(wrp->poll_timeout));
			break;
		}
		musb_writeb(musb->mregs, MUSB_DEVCTL, 0);
		skip_session = 1;
		/* fall */

	case OTG_STATE_A_IDLE:
	case OTG_STATE_B_IDLE:
		if (devctl & MUSB_DEVCTL_BDEVICE) {
			musb->xceiv->otg->state = OTG_STATE_B_IDLE;
			MUSB_DEV_MODE(musb);
		} else {
			musb->xceiv->otg->state = OTG_STATE_A_IDLE;
			MUSB_HST_MODE(musb);
		}
		if (!(devctl & MUSB_DEVCTL_SESSION) && !skip_session)
			musb_writeb(mregs, MUSB_DEVCTL, MUSB_DEVCTL_SESSION);
		mod_timer(&glue->timer, jiffies +
				msecs_to_jiffies(wrp->poll_timeout));
		break;
	case OTG_STATE_A_WAIT_VFALL:
		musb->xceiv->otg->state = OTG_STATE_A_WAIT_VRISE;
		musb_writel(musb->ctrl_base, wrp->coreintr_set,
			    MUSB_INTR_VBUSERROR << wrp->usb_shift);
		break;
	default:
		break;
	}

	return 0;
}

static void otg_timer(unsigned long _musb)
{
	struct musb *musb = (void *)_musb;
	struct device *dev = musb->controller;
	unsigned long flags;
	int err;

	err = pm_runtime_get(dev);
	if ((err != -EINPROGRESS) && err < 0) {
		dev_err(dev, "Poll could not pm_runtime_get: %i\n", err);
		pm_runtime_put_noidle(dev);

		return;
	}

	spin_lock_irqsave(&musb->lock, flags);
	err = musb_queue_resume_work(musb, dsps_check_status, NULL);
	if (err < 0)
		dev_err(dev, "%s resume work: %i\n", __func__, err);
	spin_unlock_irqrestore(&musb->lock, flags);
	pm_runtime_mark_last_busy(dev);
	pm_runtime_put_autosuspend(dev);
}

void dsps_musb_clear_ep_rxintr(struct musb *musb, int epnum)
{
	u32 epintr;
	struct dsps_glue *glue = dev_get_drvdata(musb->controller->parent);
	const struct dsps_musb_wrapper *wrp = glue->wrp;

	/* musb->lock might already been held */
	epintr = (1 << epnum) << wrp->rxep_shift;
	musb_writel(musb->ctrl_base, wrp->epintr_status, epintr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static irqreturn_t dsps_interrupt(int irq, void *hci)
{
	struct musb  *musb = hci;
	void __iomem *reg_base = musb->ctrl_base;
	struct device *dev = musb->controller;
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct dsps_glue *glue = dev_get_drvdata(dev->parent);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	unsigned long flags;
	irqreturn_t ret = IRQ_NONE;
	u32 epintr, usbintr;

	spin_lock_irqsave(&musb->lock, flags);

	/* Get endpoint interrupts */
<<<<<<< HEAD
	epintr = dsps_readl(reg_base, wrp->epintr_status);
=======
	epintr = musb_readl(reg_base, wrp->epintr_status);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	musb->int_rx = (epintr & wrp->rxep_bitmap) >> wrp->rxep_shift;
	musb->int_tx = (epintr & wrp->txep_bitmap) >> wrp->txep_shift;

	if (epintr)
<<<<<<< HEAD
		dsps_writel(reg_base, wrp->epintr_status, epintr);

	/* Get usb core interrupts */
	usbintr = dsps_readl(reg_base, wrp->coreintr_status);
	if (!usbintr && !epintr)
		goto eoi;

	musb->int_usb =	(usbintr & wrp->usb_bitmap) >> wrp->usb_shift;
	if (usbintr)
		dsps_writel(reg_base, wrp->coreintr_status, usbintr);

	dev_dbg(musb->controller, "usbintr (%x) epintr(%x)\n",
			usbintr, epintr);
	/*
	 * DRVVBUS IRQs are the only proxy we have (a very poor one!) for
	 * DSPS IP's missing ID change IRQ.  We need an ID change IRQ to
	 * switch appropriately between halves of the OTG state machine.
	 * Managing DEVCTL.SESSION per Mentor docs requires that we know its
	 * value but DEVCTL.BDEVICE is invalid without DEVCTL.SESSION set.
	 * Also, DRVVBUS pulses for SRP (but not at 5V) ...
	 */
	if (is_host_active(musb) && usbintr & MUSB_INTR_BABBLE)
		pr_info("CAUTION: musb: Babble Interrupt Occurred\n");

	if (usbintr & ((1 << wrp->drvvbus) << wrp->usb_shift)) {
		int drvvbus = dsps_readl(reg_base, wrp->status);
		void __iomem *mregs = musb->mregs;
		u8 devctl = dsps_readb(mregs, MUSB_DEVCTL);
=======
		musb_writel(reg_base, wrp->epintr_status, epintr);

	/* Get usb core interrupts */
	usbintr = musb_readl(reg_base, wrp->coreintr_status);
	if (!usbintr && !epintr)
		goto out;

	musb->int_usb =	(usbintr & wrp->usb_bitmap) >> wrp->usb_shift;
	if (usbintr)
		musb_writel(reg_base, wrp->coreintr_status, usbintr);

	dev_dbg(musb->controller, "usbintr (%x) epintr(%x)\n",
			usbintr, epintr);

	if (usbintr & ((1 << wrp->drvvbus) << wrp->usb_shift)) {
		int drvvbus = musb_readl(reg_base, wrp->status);
		void __iomem *mregs = musb->mregs;
		u8 devctl = musb_readb(mregs, MUSB_DEVCTL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		int err;

		err = musb->int_usb & MUSB_INTR_VBUSERROR;
		if (err) {
			/*
			 * The Mentor core doesn't debounce VBUS as needed
			 * to cope with device connect current spikes. This
			 * means it's not uncommon for bus-powered devices
			 * to get VBUS errors during enumeration.
			 *
			 * This is a workaround, but newer RTL from Mentor
			 * seems to allow a better one: "re"-starting sessions
			 * without waiting for VBUS to stop registering in
			 * devctl.
			 */
			musb->int_usb &= ~MUSB_INTR_VBUSERROR;
<<<<<<< HEAD
			musb->xceiv->state = OTG_STATE_A_WAIT_VFALL;
			mod_timer(&glue->timer[pdev->id],
					jiffies + wrp->poll_seconds * HZ);
			WARNING("VBUS error workaround (delay coming)\n");
		} else if (drvvbus) {
			musb->is_active = 1;
			MUSB_HST_MODE(musb);
			musb->xceiv->otg->default_a = 1;
			musb->xceiv->state = OTG_STATE_A_WAIT_VRISE;
			del_timer(&glue->timer[pdev->id]);
=======
			musb->xceiv->otg->state = OTG_STATE_A_WAIT_VFALL;
			mod_timer(&glue->timer, jiffies +
					msecs_to_jiffies(wrp->poll_timeout));
			WARNING("VBUS error workaround (delay coming)\n");
		} else if (drvvbus) {
			MUSB_HST_MODE(musb);
			musb->xceiv->otg->default_a = 1;
			musb->xceiv->otg->state = OTG_STATE_A_WAIT_VRISE;
			mod_timer(&glue->timer, jiffies +
				  msecs_to_jiffies(wrp->poll_timeout));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		} else {
			musb->is_active = 0;
			MUSB_DEV_MODE(musb);
			musb->xceiv->otg->default_a = 0;
<<<<<<< HEAD
			musb->xceiv->state = OTG_STATE_B_IDLE;
=======
			musb->xceiv->otg->state = OTG_STATE_B_IDLE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/* NOTE: this must complete power-on within 100 ms. */
		dev_dbg(musb->controller, "VBUS %s (%s)%s, devctl %02x\n",
				drvvbus ? "on" : "off",
<<<<<<< HEAD
				usb_otg_state_string(musb->xceiv->state),
=======
				usb_otg_state_string(musb->xceiv->otg->state),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				err ? " ERROR" : "",
				devctl);
		ret = IRQ_HANDLED;
	}

	if (musb->int_tx || musb->int_rx || musb->int_usb)
		ret |= musb_interrupt(musb);

<<<<<<< HEAD
 eoi:
	/* EOI needs to be written for the IRQ to be re-asserted. */
	if (ret == IRQ_HANDLED || epintr || usbintr)
		dsps_writel(reg_base, wrp->eoi, 1);

	/* Poll for ID change */
	if (musb->xceiv->state == OTG_STATE_B_IDLE)
		mod_timer(&glue->timer[pdev->id],
			 jiffies + wrp->poll_seconds * HZ);

=======
	/* Poll for ID change and connect */
	switch (musb->xceiv->otg->state) {
	case OTG_STATE_B_IDLE:
	case OTG_STATE_A_WAIT_BCON:
		mod_timer(&glue->timer, jiffies +
				msecs_to_jiffies(wrp->poll_timeout));
		break;
	default:
		break;
	}

out:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_unlock_irqrestore(&musb->lock, flags);

	return ret;
}

<<<<<<< HEAD
static int dsps_musb_init(struct musb *musb)
{
	struct device *dev = musb->controller;
	struct platform_device *pdev = to_platform_device(dev);
	struct dsps_glue *glue = dev_get_drvdata(dev->parent);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	void __iomem *reg_base = musb->ctrl_base;
	u32 rev, val;
	int status;

	/* mentor core register starts at offset of 0x400 from musb base */
	musb->mregs += wrp->musb_core_offset;

	/* NOP driver needs change if supporting dual instance */
	usb_nop_xceiv_register();
	musb->xceiv = usb_get_phy(USB_PHY_TYPE_USB2);
	if (IS_ERR_OR_NULL(musb->xceiv))
		return -EPROBE_DEFER;

	/* Returns zero if e.g. not clocked */
	rev = dsps_readl(reg_base, wrp->revision);
	if (!rev) {
		status = -ENODEV;
		goto err0;
	}

	setup_timer(&glue->timer[pdev->id], otg_timer, (unsigned long) musb);

	/* Reset the musb */
	dsps_writel(reg_base, wrp->control, (1 << wrp->reset));

	/* Start the on-chip PHY and its PLL. */
	musb_dsps_phy_control(glue, pdev->id, 1);
=======
static int dsps_musb_dbg_init(struct musb *musb, struct dsps_glue *glue)
{
	struct dentry *root;
	struct dentry *file;
	char buf[128];

	sprintf(buf, "%s.dsps", dev_name(musb->controller));
	root = debugfs_create_dir(buf, NULL);
	if (!root)
		return -ENOMEM;
	glue->dbgfs_root = root;

	glue->regset.regs = dsps_musb_regs;
	glue->regset.nregs = ARRAY_SIZE(dsps_musb_regs);
	glue->regset.base = musb->ctrl_base;

	file = debugfs_create_regset32("regdump", S_IRUGO, root, &glue->regset);
	if (!file) {
		debugfs_remove_recursive(root);
		return -ENOMEM;
	}
	return 0;
}

static int dsps_musb_init(struct musb *musb)
{
	struct device *dev = musb->controller;
	struct dsps_glue *glue = dev_get_drvdata(dev->parent);
	struct platform_device *parent = to_platform_device(dev->parent);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	void __iomem *reg_base;
	struct resource *r;
	u32 rev, val;
	int ret;

	r = platform_get_resource_byname(parent, IORESOURCE_MEM, "control");
	reg_base = devm_ioremap_resource(dev, r);
	if (IS_ERR(reg_base))
		return PTR_ERR(reg_base);
	musb->ctrl_base = reg_base;

	/* NOP driver needs change if supporting dual instance */
	musb->xceiv = devm_usb_get_phy_by_phandle(dev->parent, "phys", 0);
	if (IS_ERR(musb->xceiv))
		return PTR_ERR(musb->xceiv);

	musb->phy = devm_phy_get(dev->parent, "usb2-phy");

	/* Returns zero if e.g. not clocked */
	rev = musb_readl(reg_base, wrp->revision);
	if (!rev)
		return -ENODEV;

	usb_phy_init(musb->xceiv);
	if (IS_ERR(musb->phy))  {
		musb->phy = NULL;
	} else {
		ret = phy_init(musb->phy);
		if (ret < 0)
			return ret;
		ret = phy_power_on(musb->phy);
		if (ret) {
			phy_exit(musb->phy);
			return ret;
		}
	}

	setup_timer(&glue->timer, otg_timer, (unsigned long) musb);

	/* Reset the musb */
	musb_writel(reg_base, wrp->control, (1 << wrp->reset));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	musb->isr = dsps_interrupt;

	/* reset the otgdisable bit, needed for host mode to work */
<<<<<<< HEAD
	val = dsps_readl(reg_base, wrp->phy_utmi);
	val &= ~(1 << wrp->otg_disable);
	dsps_writel(musb->ctrl_base, wrp->phy_utmi, val);

	/* clear level interrupt */
	dsps_writel(reg_base, wrp->eoi, 0);

	return 0;
err0:
	usb_put_phy(musb->xceiv);
	usb_nop_xceiv_unregister();
	return status;
=======
	val = musb_readl(reg_base, wrp->phy_utmi);
	val &= ~(1 << wrp->otg_disable);
	musb_writel(musb->ctrl_base, wrp->phy_utmi, val);

	/*
	 *  Check whether the dsps version has babble control enabled.
	 * In latest silicon revision the babble control logic is enabled.
	 * If MUSB_BABBLE_CTL returns 0x4 then we have the babble control
	 * logic enabled.
	 */
	val = musb_readb(musb->mregs, MUSB_BABBLE_CTL);
	if (val & MUSB_BABBLE_RCV_DISABLE) {
		glue->sw_babble_enabled = true;
		val |= MUSB_BABBLE_SW_SESSION_CTRL;
		musb_writeb(musb->mregs, MUSB_BABBLE_CTL, val);
	}

	mod_timer(&glue->timer, jiffies +
		  msecs_to_jiffies(glue->wrp->poll_timeout));

	return dsps_musb_dbg_init(musb, glue);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int dsps_musb_exit(struct musb *musb)
{
	struct device *dev = musb->controller;
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct dsps_glue *glue = dev_get_drvdata(dev->parent);

	del_timer_sync(&glue->timer[pdev->id]);

	/* Shutdown the on-chip PHY and its PLL. */
	musb_dsps_phy_control(glue, pdev->id, 0);

	/* NOP driver needs change if supporting dual instance */
	usb_put_phy(musb->xceiv);
	usb_nop_xceiv_unregister();
=======
	struct dsps_glue *glue = dev_get_drvdata(dev->parent);

	del_timer_sync(&glue->timer);
	usb_phy_shutdown(musb->xceiv);
	phy_power_off(musb->phy);
	phy_exit(musb->phy);
	debugfs_remove_recursive(glue->dbgfs_root);

	return 0;
}

static int dsps_musb_set_mode(struct musb *musb, u8 mode)
{
	struct device *dev = musb->controller;
	struct dsps_glue *glue = dev_get_drvdata(dev->parent);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	void __iomem *ctrl_base = musb->ctrl_base;
	u32 reg;

	reg = musb_readl(ctrl_base, wrp->mode);

	switch (mode) {
	case MUSB_HOST:
		reg &= ~(1 << wrp->iddig);

		/*
		 * if we're setting mode to host-only or device-only, we're
		 * going to ignore whatever the PHY sends us and just force
		 * ID pin status by SW
		 */
		reg |= (1 << wrp->iddig_mux);

		musb_writel(ctrl_base, wrp->mode, reg);
		musb_writel(ctrl_base, wrp->phy_utmi, 0x02);
		break;
	case MUSB_PERIPHERAL:
		reg |= (1 << wrp->iddig);

		/*
		 * if we're setting mode to host-only or device-only, we're
		 * going to ignore whatever the PHY sends us and just force
		 * ID pin status by SW
		 */
		reg |= (1 << wrp->iddig_mux);

		musb_writel(ctrl_base, wrp->mode, reg);
		break;
	case MUSB_OTG:
		musb_writel(ctrl_base, wrp->phy_utmi, 0x02);
		break;
	default:
		dev_err(glue->dev, "unsupported mode %d\n", mode);
		return -EINVAL;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static struct musb_platform_ops dsps_ops = {
	.init		= dsps_musb_init,
	.exit		= dsps_musb_exit,

	.enable		= dsps_musb_enable,
	.disable	= dsps_musb_disable,

	.try_idle	= dsps_musb_try_idle,
=======
static bool dsps_sw_babble_control(struct musb *musb)
{
	u8 babble_ctl;
	bool session_restart =  false;

	babble_ctl = musb_readb(musb->mregs, MUSB_BABBLE_CTL);
	dev_dbg(musb->controller, "babble: MUSB_BABBLE_CTL value %x\n",
		babble_ctl);
	/*
	 * check line monitor flag to check whether babble is
	 * due to noise
	 */
	dev_dbg(musb->controller, "STUCK_J is %s\n",
		babble_ctl & MUSB_BABBLE_STUCK_J ? "set" : "reset");

	if (babble_ctl & MUSB_BABBLE_STUCK_J) {
		int timeout = 10;

		/*
		 * babble is due to noise, then set transmit idle (d7 bit)
		 * to resume normal operation
		 */
		babble_ctl = musb_readb(musb->mregs, MUSB_BABBLE_CTL);
		babble_ctl |= MUSB_BABBLE_FORCE_TXIDLE;
		musb_writeb(musb->mregs, MUSB_BABBLE_CTL, babble_ctl);

		/* wait till line monitor flag cleared */
		dev_dbg(musb->controller, "Set TXIDLE, wait J to clear\n");
		do {
			babble_ctl = musb_readb(musb->mregs, MUSB_BABBLE_CTL);
			udelay(1);
		} while ((babble_ctl & MUSB_BABBLE_STUCK_J) && timeout--);

		/* check whether stuck_at_j bit cleared */
		if (babble_ctl & MUSB_BABBLE_STUCK_J) {
			/*
			 * real babble condition has occurred
			 * restart the controller to start the
			 * session again
			 */
			dev_dbg(musb->controller, "J not cleared, misc (%x)\n",
				babble_ctl);
			session_restart = true;
		}
	} else {
		session_restart = true;
	}

	return session_restart;
}

static int dsps_musb_recover(struct musb *musb)
{
	struct device *dev = musb->controller;
	struct dsps_glue *glue = dev_get_drvdata(dev->parent);
	int session_restart = 0;

	if (glue->sw_babble_enabled)
		session_restart = dsps_sw_babble_control(musb);
	else
		session_restart = 1;

	return session_restart ? 0 : -EPIPE;
}

/* Similar to am35x, dm81xx support only 32-bit read operation */
static void dsps_read_fifo32(struct musb_hw_ep *hw_ep, u16 len, u8 *dst)
{
	void __iomem *fifo = hw_ep->fifo;

	if (len >= 4) {
		ioread32_rep(fifo, dst, len >> 2);
		dst += len & ~0x03;
		len &= 0x03;
	}

	/* Read any remaining 1 to 3 bytes */
	if (len > 0) {
		u32 val = musb_readl(fifo, 0);
		memcpy(dst, &val, len);
	}
}

static struct musb_platform_ops dsps_ops = {
	.quirks		= MUSB_DMA_CPPI41 | MUSB_INDEXED_EP,
	.init		= dsps_musb_init,
	.exit		= dsps_musb_exit,

#ifdef CONFIG_USB_TI_CPPI41_DMA
	.dma_init	= cppi41_dma_controller_create,
	.dma_exit	= cppi41_dma_controller_destroy,
#endif
	.enable		= dsps_musb_enable,
	.disable	= dsps_musb_disable,

	.set_mode	= dsps_musb_set_mode,
	.recover	= dsps_musb_recover,
	.clear_ep_rxintr = dsps_musb_clear_ep_rxintr,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static u64 musb_dmamask = DMA_BIT_MASK(32);

<<<<<<< HEAD
static int dsps_create_musb_pdev(struct dsps_glue *glue, u8 id)
{
	struct device *dev = glue->dev;
	struct platform_device *pdev = to_platform_device(dev);
	struct musb_hdrc_platform_data  *pdata = dev->platform_data;
	struct device_node *np = pdev->dev.of_node;
	struct musb_hdrc_config	*config;
	struct platform_device	*musb;
	struct resource *res;
	struct resource	resources[2];
	char res_name[11];
	int ret;

	resources[0].start = dsps_control_module_phys[id];
	resources[0].end = resources[0].start + SZ_4 - 1;
	resources[0].flags = IORESOURCE_MEM;

	glue->usb_ctrl[id] = devm_ioremap_resource(&pdev->dev, resources);
	if (IS_ERR(glue->usb_ctrl[id])) {
		ret = PTR_ERR(glue->usb_ctrl[id]);
		goto err0;
	}

	/* first resource is for usbss, so start index from 1 */
	res = platform_get_resource(pdev, IORESOURCE_MEM, id + 1);
	if (!res) {
		dev_err(dev, "failed to get memory for instance %d\n", id);
		ret = -ENODEV;
		goto err0;
	}
	res->parent = NULL;
	resources[0] = *res;

	/* first resource is for usbss, so start index from 1 */
	res = platform_get_resource(pdev, IORESOURCE_IRQ, id + 1);
	if (!res) {
		dev_err(dev, "failed to get irq for instance %d\n", id);
		ret = -ENODEV;
		goto err0;
	}
	res->parent = NULL;
	resources[1] = *res;
	resources[1].name = "mc";
=======
static int get_int_prop(struct device_node *dn, const char *s)
{
	int ret;
	u32 val;

	ret = of_property_read_u32(dn, s, &val);
	if (ret)
		return 0;
	return val;
}

static int get_musb_port_mode(struct device *dev)
{
	enum usb_dr_mode mode;

	mode = usb_get_dr_mode(dev);
	switch (mode) {
	case USB_DR_MODE_HOST:
		return MUSB_PORT_MODE_HOST;

	case USB_DR_MODE_PERIPHERAL:
		return MUSB_PORT_MODE_GADGET;

	case USB_DR_MODE_UNKNOWN:
	case USB_DR_MODE_OTG:
	default:
		return MUSB_PORT_MODE_DUAL_ROLE;
	}
}

static int dsps_create_musb_pdev(struct dsps_glue *glue,
		struct platform_device *parent)
{
	struct musb_hdrc_platform_data pdata;
	struct resource	resources[2];
	struct resource	*res;
	struct device *dev = &parent->dev;
	struct musb_hdrc_config	*config;
	struct platform_device *musb;
	struct device_node *dn = parent->dev.of_node;
	int ret, val;

	memset(resources, 0, sizeof(resources));
	res = platform_get_resource_byname(parent, IORESOURCE_MEM, "mc");
	if (!res) {
		dev_err(dev, "failed to get memory.\n");
		return -EINVAL;
	}
	resources[0] = *res;

	res = platform_get_resource_byname(parent, IORESOURCE_IRQ, "mc");
	if (!res) {
		dev_err(dev, "failed to get irq.\n");
		return -EINVAL;
	}
	resources[1] = *res;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* allocate the child platform device */
	musb = platform_device_alloc("musb-hdrc", PLATFORM_DEVID_AUTO);
	if (!musb) {
		dev_err(dev, "failed to allocate musb device\n");
<<<<<<< HEAD
		ret = -ENOMEM;
		goto err0;
=======
		return -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	musb->dev.parent		= dev;
	musb->dev.dma_mask		= &musb_dmamask;
	musb->dev.coherent_dma_mask	= musb_dmamask;

<<<<<<< HEAD
	glue->musb[id]			= musb;

	ret = platform_device_add_resources(musb, resources, 2);
	if (ret) {
		dev_err(dev, "failed to add resources\n");
		goto err2;
	}

	if (np) {
		pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata) {
			dev_err(&pdev->dev,
				"failed to allocate musb platfrom data\n");
			ret = -ENOMEM;
			goto err2;
		}

		config = devm_kzalloc(&pdev->dev, sizeof(*config), GFP_KERNEL);
		if (!config) {
			dev_err(&pdev->dev,
				"failed to allocate musb hdrc config\n");
			ret = -ENOMEM;
			goto err2;
		}

		of_property_read_u32(np, "num-eps", (u32 *)&config->num_eps);
		of_property_read_u32(np, "ram-bits", (u32 *)&config->ram_bits);
		snprintf(res_name, sizeof(res_name), "port%d-mode", id);
		of_property_read_u32(np, res_name, (u32 *)&pdata->mode);
		of_property_read_u32(np, "power", (u32 *)&pdata->power);
		config->multipoint = of_property_read_bool(np, "multipoint");

		pdata->config		= config;
	}

	pdata->platform_ops		= &dsps_ops;

	ret = platform_device_add_data(musb, pdata, sizeof(*pdata));
	if (ret) {
		dev_err(dev, "failed to add platform_data\n");
		goto err2;
=======
	glue->musb = musb;

	ret = platform_device_add_resources(musb, resources,
			ARRAY_SIZE(resources));
	if (ret) {
		dev_err(dev, "failed to add resources\n");
		goto err;
	}

	config = devm_kzalloc(&parent->dev, sizeof(*config), GFP_KERNEL);
	if (!config) {
		ret = -ENOMEM;
		goto err;
	}
	pdata.config = config;
	pdata.platform_ops = &dsps_ops;

	config->num_eps = get_int_prop(dn, "mentor,num-eps");
	config->ram_bits = get_int_prop(dn, "mentor,ram-bits");
	config->host_port_deassert_reset_at_resume = 1;
	pdata.mode = get_musb_port_mode(dev);
	/* DT keeps this entry in mA, musb expects it as per USB spec */
	pdata.power = get_int_prop(dn, "mentor,power") / 2;

	ret = of_property_read_u32(dn, "mentor,multipoint", &val);
	if (!ret && val)
		config->multipoint = true;

	config->maximum_speed = usb_get_maximum_speed(&parent->dev);
	switch (config->maximum_speed) {
	case USB_SPEED_LOW:
	case USB_SPEED_FULL:
		break;
	case USB_SPEED_SUPER:
		dev_warn(dev, "ignore incorrect maximum_speed "
				"(super-speed) setting in dts");
		/* fall through */
	default:
		config->maximum_speed = USB_SPEED_HIGH;
	}

	ret = platform_device_add_data(musb, &pdata, sizeof(pdata));
	if (ret) {
		dev_err(dev, "failed to add platform_data\n");
		goto err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	ret = platform_device_add(musb);
	if (ret) {
		dev_err(dev, "failed to register musb device\n");
<<<<<<< HEAD
		goto err2;
	}

	return 0;

err2:
	platform_device_put(musb);
err0:
=======
		goto err;
	}
	return 0;

err:
	platform_device_put(musb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static int dsps_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;
	const struct dsps_musb_wrapper *wrp;
	struct dsps_glue *glue;
<<<<<<< HEAD
	struct resource *iomem;
	int ret, i;
=======
	int ret;

	if (!strcmp(pdev->name, "musb-hdrc"))
		return -ENODEV;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	match = of_match_node(musb_dsps_of_match, pdev->dev.of_node);
	if (!match) {
		dev_err(&pdev->dev, "fail to get matching of_match struct\n");
<<<<<<< HEAD
		ret = -EINVAL;
		goto err0;
	}
	wrp = match->data;

	/* allocate glue */
	glue = kzalloc(sizeof(*glue), GFP_KERNEL);
	if (!glue) {
		dev_err(&pdev->dev, "unable to allocate glue memory\n");
		ret = -ENOMEM;
		goto err0;
	}

	/* get memory resource */
	iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!iomem) {
		dev_err(&pdev->dev, "failed to get usbss mem resourse\n");
		ret = -ENODEV;
		goto err1;
	}

	glue->dev = &pdev->dev;

	glue->wrp = kmemdup(wrp, sizeof(*wrp), GFP_KERNEL);
	if (!glue->wrp) {
		dev_err(&pdev->dev, "failed to duplicate wrapper struct memory\n");
		ret = -ENOMEM;
		goto err1;
	}
	platform_set_drvdata(pdev, glue);

	/* enable the usbss clocks */
	pm_runtime_enable(&pdev->dev);

	ret = pm_runtime_get_sync(&pdev->dev);
	if (ret < 0) {
		dev_err(&pdev->dev, "pm_runtime_get_sync FAILED");
		goto err2;
	}

	/* create the child platform device for all instances of musb */
	for (i = 0; i < wrp->instances ; i++) {
		ret = dsps_create_musb_pdev(glue, i);
		if (ret != 0) {
			dev_err(&pdev->dev, "failed to create child pdev\n");
			/* release resources of previously created instances */
			for (i--; i >= 0 ; i--)
				platform_device_unregister(glue->musb[i]);
			goto err3;
		}
	}

	return 0;

err3:
	pm_runtime_put(&pdev->dev);
err2:
	pm_runtime_disable(&pdev->dev);
	kfree(glue->wrp);
err1:
	kfree(glue);
err0:
	return ret;
}
static int dsps_remove(struct platform_device *pdev)
{
	struct dsps_glue *glue = platform_get_drvdata(pdev);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	int i;

	/* delete the child platform device */
	for (i = 0; i < wrp->instances ; i++)
		platform_device_unregister(glue->musb[i]);

	/* disable usbss clocks */
	pm_runtime_put(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
	kfree(glue->wrp);
	kfree(glue);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int dsps_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev->parent);
	struct dsps_glue *glue = platform_get_drvdata(pdev);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	int i;

	for (i = 0; i < wrp->instances; i++)
		musb_dsps_phy_control(glue, i, 0);
=======
		return -EINVAL;
	}
	wrp = match->data;

	if (of_device_is_compatible(pdev->dev.of_node, "ti,musb-dm816"))
		dsps_ops.read_fifo = dsps_read_fifo32;

	/* allocate glue */
	glue = devm_kzalloc(&pdev->dev, sizeof(*glue), GFP_KERNEL);
	if (!glue)
		return -ENOMEM;

	glue->dev = &pdev->dev;
	glue->wrp = wrp;

	platform_set_drvdata(pdev, glue);
	pm_runtime_enable(&pdev->dev);
	ret = dsps_create_musb_pdev(glue, pdev);
	if (ret)
		goto err;

	return 0;

err:
	pm_runtime_disable(&pdev->dev);
	return ret;
}

static int dsps_remove(struct platform_device *pdev)
{
	struct dsps_glue *glue = platform_get_drvdata(pdev);

	platform_device_unregister(glue->musb);

	pm_runtime_disable(&pdev->dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static int dsps_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev->parent);
	struct dsps_glue *glue = platform_get_drvdata(pdev);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	int i;

	for (i = 0; i < wrp->instances; i++)
		musb_dsps_phy_control(glue, i, 1);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(dsps_pm_ops, dsps_suspend, dsps_resume);

static const struct dsps_musb_wrapper ti81xx_driver_data = {
	.revision		= 0x00,
	.control		= 0x14,
	.status			= 0x18,
	.eoi			= 0x24,
=======
static const struct dsps_musb_wrapper am33xx_driver_data = {
	.revision		= 0x00,
	.control		= 0x14,
	.status			= 0x18,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.epintr_set		= 0x38,
	.epintr_clear		= 0x40,
	.epintr_status		= 0x30,
	.coreintr_set		= 0x3c,
	.coreintr_clear		= 0x44,
	.coreintr_status	= 0x34,
	.phy_utmi		= 0xe0,
	.mode			= 0xe8,
<<<<<<< HEAD
	.reset			= 0,
	.otg_disable		= 21,
	.iddig			= 8,
=======
	.tx_mode		= 0x70,
	.rx_mode		= 0x74,
	.reset			= 0,
	.otg_disable		= 21,
	.iddig			= 8,
	.iddig_mux		= 7,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.usb_shift		= 0,
	.usb_mask		= 0x1ff,
	.usb_bitmap		= (0x1ff << 0),
	.drvvbus		= 8,
	.txep_shift		= 0,
	.txep_mask		= 0xffff,
	.txep_bitmap		= (0xffff << 0),
	.rxep_shift		= 16,
	.rxep_mask		= 0xfffe,
	.rxep_bitmap		= (0xfffe << 16),
<<<<<<< HEAD
	.musb_core_offset	= 0x400,
	.poll_seconds		= 2,
	.instances		= 1,
};

static const struct platform_device_id musb_dsps_id_table[] = {
	{
		.name	= "musb-ti81xx",
		.driver_data	= (kernel_ulong_t) &ti81xx_driver_data,
	},
	{  },	/* Terminating Entry */
};
MODULE_DEVICE_TABLE(platform, musb_dsps_id_table);

#ifdef CONFIG_OF
static const struct of_device_id musb_dsps_of_match[] = {
	{ .compatible = "ti,musb-am33xx",
		.data = (void *) &ti81xx_driver_data, },
	{  },
};
MODULE_DEVICE_TABLE(of, musb_dsps_of_match);
#endif

=======
	.poll_timeout		= 2000, /* ms */
};

static const struct of_device_id musb_dsps_of_match[] = {
	{ .compatible = "ti,musb-am33xx",
		.data = &am33xx_driver_data, },
	{ .compatible = "ti,musb-dm816",
		.data = &am33xx_driver_data, },
	{  },
};
MODULE_DEVICE_TABLE(of, musb_dsps_of_match);

#ifdef CONFIG_PM_SLEEP
static int dsps_suspend(struct device *dev)
{
	struct dsps_glue *glue = dev_get_drvdata(dev);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	struct musb *musb = platform_get_drvdata(glue->musb);
	void __iomem *mbase;

	del_timer_sync(&glue->timer);

	if (!musb)
		/* This can happen if the musb device is in -EPROBE_DEFER */
		return 0;

	mbase = musb->ctrl_base;
	glue->context.control = musb_readl(mbase, wrp->control);
	glue->context.epintr = musb_readl(mbase, wrp->epintr_set);
	glue->context.coreintr = musb_readl(mbase, wrp->coreintr_set);
	glue->context.phy_utmi = musb_readl(mbase, wrp->phy_utmi);
	glue->context.mode = musb_readl(mbase, wrp->mode);
	glue->context.tx_mode = musb_readl(mbase, wrp->tx_mode);
	glue->context.rx_mode = musb_readl(mbase, wrp->rx_mode);

	return 0;
}

static int dsps_resume(struct device *dev)
{
	struct dsps_glue *glue = dev_get_drvdata(dev);
	const struct dsps_musb_wrapper *wrp = glue->wrp;
	struct musb *musb = platform_get_drvdata(glue->musb);
	void __iomem *mbase;

	if (!musb)
		return 0;

	mbase = musb->ctrl_base;
	musb_writel(mbase, wrp->control, glue->context.control);
	musb_writel(mbase, wrp->epintr_set, glue->context.epintr);
	musb_writel(mbase, wrp->coreintr_set, glue->context.coreintr);
	musb_writel(mbase, wrp->phy_utmi, glue->context.phy_utmi);
	musb_writel(mbase, wrp->mode, glue->context.mode);
	musb_writel(mbase, wrp->tx_mode, glue->context.tx_mode);
	musb_writel(mbase, wrp->rx_mode, glue->context.rx_mode);
	if (musb->xceiv->otg->state == OTG_STATE_B_IDLE &&
	    musb->port_mode == MUSB_PORT_MODE_DUAL_ROLE)
		mod_timer(&glue->timer, jiffies +
				msecs_to_jiffies(wrp->poll_timeout));

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(dsps_pm_ops, dsps_suspend, dsps_resume);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct platform_driver dsps_usbss_driver = {
	.probe		= dsps_probe,
	.remove         = dsps_remove,
	.driver         = {
		.name   = "musb-dsps",
		.pm	= &dsps_pm_ops,
<<<<<<< HEAD
		.of_match_table	= of_match_ptr(musb_dsps_of_match),
	},
	.id_table	= musb_dsps_id_table,
=======
		.of_match_table	= musb_dsps_of_match,
	},
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

MODULE_DESCRIPTION("TI DSPS MUSB Glue Layer");
MODULE_AUTHOR("Ravi B <ravibabu@ti.com>");
MODULE_AUTHOR("Ajay Kumar Gupta <ajay.gupta@ti.com>");
MODULE_LICENSE("GPL v2");

<<<<<<< HEAD
static int __init dsps_init(void)
{
	return platform_driver_register(&dsps_usbss_driver);
}
subsys_initcall(dsps_init);

static void __exit dsps_exit(void)
{
	platform_driver_unregister(&dsps_usbss_driver);
}
module_exit(dsps_exit);
=======
module_platform_driver(dsps_usbss_driver);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
