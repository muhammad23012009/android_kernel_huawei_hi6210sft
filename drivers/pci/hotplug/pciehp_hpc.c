/*
 * PCI Express PCI Hot Plug Driver
 *
 * Copyright (C) 1995,2001 Compaq Computer Corporation
 * Copyright (C) 2001 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (C) 2001 IBM Corp.
 * Copyright (C) 2003-2004 Intel Corporation
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Send feedback to <greg@kroah.com>,<kristen.c.accardi@intel.com>
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/signal.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/slab.h>

#include "../pci.h"
#include "pciehp.h"

<<<<<<< HEAD
static inline int pciehp_readw(struct controller *ctrl, int reg, u16 *value)
{
	struct pci_dev *dev = ctrl->pcie->port;
	return pcie_capability_read_word(dev, reg, value);
}

static inline int pciehp_readl(struct controller *ctrl, int reg, u32 *value)
{
	struct pci_dev *dev = ctrl->pcie->port;
	return pcie_capability_read_dword(dev, reg, value);
}

static inline int pciehp_writew(struct controller *ctrl, int reg, u16 value)
{
	struct pci_dev *dev = ctrl->pcie->port;
	return pcie_capability_write_word(dev, reg, value);
}

static inline int pciehp_writel(struct controller *ctrl, int reg, u32 value)
{
	struct pci_dev *dev = ctrl->pcie->port;
	return pcie_capability_write_dword(dev, reg, value);
}

/* Power Control Command */
#define POWER_ON	0
#define POWER_OFF	PCI_EXP_SLTCTL_PCC

=======
static inline struct pci_dev *ctrl_dev(struct controller *ctrl)
{
	return ctrl->pcie->port;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static irqreturn_t pcie_isr(int irq, void *dev_id);
static void start_int_poll_timer(struct controller *ctrl, int sec);

/* This is the interrupt polling timeout function. */
static void int_poll_timeout(unsigned long data)
{
	struct controller *ctrl = (struct controller *)data;

	/* Poll for interrupt events.  regs == NULL => polling */
	pcie_isr(0, ctrl);

	init_timer(&ctrl->poll_timer);
	if (!pciehp_poll_time)
		pciehp_poll_time = 2; /* default polling interval is 2 sec */

	start_int_poll_timer(ctrl, pciehp_poll_time);
}

/* This function starts the interrupt polling timer. */
static void start_int_poll_timer(struct controller *ctrl, int sec)
{
	/* Clamp to sane value */
	if ((sec <= 0) || (sec > 60))
<<<<<<< HEAD
        	sec = 2;
=======
		sec = 2;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ctrl->poll_timer.function = &int_poll_timeout;
	ctrl->poll_timer.data = (unsigned long)ctrl;
	ctrl->poll_timer.expires = jiffies + sec * HZ;
	add_timer(&ctrl->poll_timer);
}

static inline int pciehp_request_irq(struct controller *ctrl)
{
	int retval, irq = ctrl->pcie->irq;

	/* Install interrupt polling timer. Start with 10 sec delay */
	if (pciehp_poll_mode) {
		init_timer(&ctrl->poll_timer);
		start_int_poll_timer(ctrl, 10);
		return 0;
	}

	/* Installs the interrupt handler */
	retval = request_irq(irq, pcie_isr, IRQF_SHARED, MY_NAME, ctrl);
	if (retval)
		ctrl_err(ctrl, "Cannot get irq %d for the hotplug controller\n",
			 irq);
	return retval;
}

static inline void pciehp_free_irq(struct controller *ctrl)
{
	if (pciehp_poll_mode)
		del_timer_sync(&ctrl->poll_timer);
	else
		free_irq(ctrl->pcie->irq, ctrl);
}

<<<<<<< HEAD
static int pcie_poll_cmd(struct controller *ctrl)
{
	u16 slot_status;
	int err, timeout = 1000;

	err = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (!err && (slot_status & PCI_EXP_SLTSTA_CC)) {
		pciehp_writew(ctrl, PCI_EXP_SLTSTA, PCI_EXP_SLTSTA_CC);
		return 1;
	}
	while (timeout > 0) {
		msleep(10);
		timeout -= 10;
		err = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
		if (!err && (slot_status & PCI_EXP_SLTSTA_CC)) {
			pciehp_writew(ctrl, PCI_EXP_SLTSTA, PCI_EXP_SLTSTA_CC);
			return 1;
		}
=======
static int pcie_poll_cmd(struct controller *ctrl, int timeout)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_status;

	while (true) {
		pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
		if (slot_status == (u16) ~0) {
			ctrl_info(ctrl, "%s: no response from device\n",
				  __func__);
			return 0;
		}

		if (slot_status & PCI_EXP_SLTSTA_CC) {
			pcie_capability_write_word(pdev, PCI_EXP_SLTSTA,
						   PCI_EXP_SLTSTA_CC);
			return 1;
		}
		if (timeout < 0)
			break;
		msleep(10);
		timeout -= 10;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return 0;	/* timeout */
}

<<<<<<< HEAD
static void pcie_wait_cmd(struct controller *ctrl, int poll)
{
	unsigned int msecs = pciehp_poll_mode ? 2500 : 1000;
	unsigned long timeout = msecs_to_jiffies(msecs);
	int rc;

	if (poll)
		rc = pcie_poll_cmd(ctrl);
	else
		rc = wait_event_timeout(ctrl->queue, !ctrl->cmd_busy, timeout);
	if (!rc)
		ctrl_dbg(ctrl, "Command not completed in 1000 msec\n");
}

/**
 * pcie_write_cmd - Issue controller command
 * @ctrl: controller to which the command is issued
 * @cmd:  command value written to slot control register
 * @mask: bitmask of slot control register to be modified
 */
static int pcie_write_cmd(struct controller *ctrl, u16 cmd, u16 mask)
{
	int retval = 0;
	u16 slot_status;
=======
static void pcie_wait_cmd(struct controller *ctrl)
{
	unsigned int msecs = pciehp_poll_mode ? 2500 : 1000;
	unsigned long duration = msecs_to_jiffies(msecs);
	unsigned long cmd_timeout = ctrl->cmd_started + duration;
	unsigned long now, timeout;
	int rc;

	/*
	 * If the controller does not generate notifications for command
	 * completions, we never need to wait between writes.
	 */
	if (NO_CMD_CMPL(ctrl))
		return;

	if (!ctrl->cmd_busy)
		return;

	/*
	 * Even if the command has already timed out, we want to call
	 * pcie_poll_cmd() so it can clear PCI_EXP_SLTSTA_CC.
	 */
	now = jiffies;
	if (time_before_eq(cmd_timeout, now))
		timeout = 1;
	else
		timeout = cmd_timeout - now;

	if (ctrl->slot_ctrl & PCI_EXP_SLTCTL_HPIE &&
	    ctrl->slot_ctrl & PCI_EXP_SLTCTL_CCIE)
		rc = wait_event_timeout(ctrl->queue, !ctrl->cmd_busy, timeout);
	else
		rc = pcie_poll_cmd(ctrl, jiffies_to_msecs(timeout));

	/*
	 * Controllers with errata like Intel CF118 don't generate
	 * completion notifications unless the power/indicator/interlock
	 * control bits are changed.  On such controllers, we'll emit this
	 * timeout message when we wait for completion of commands that
	 * don't change those bits, e.g., commands that merely enable
	 * interrupts.
	 */
	if (!rc)
		ctrl_info(ctrl, "Timeout on hotplug command %#06x (issued %u msec ago)\n",
			  ctrl->slot_ctrl,
			  jiffies_to_msecs(jiffies - ctrl->cmd_started));
}

static void pcie_do_write_cmd(struct controller *ctrl, u16 cmd,
			      u16 mask, bool wait)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u16 slot_ctrl;

	mutex_lock(&ctrl->ctrl_lock);

<<<<<<< HEAD
	retval = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTSTATUS register\n",
			 __func__);
		goto out;
	}

	if (slot_status & PCI_EXP_SLTSTA_CC) {
		if (!ctrl->no_cmd_complete) {
			/*
			 * After 1 sec and CMD_COMPLETED still not set, just
			 * proceed forward to issue the next command according
			 * to spec. Just print out the error message.
			 */
			ctrl_dbg(ctrl, "CMD_COMPLETED not clear after 1 sec\n");
		} else if (!NO_CMD_CMPL(ctrl)) {
			/*
			 * This controller semms to notify of command completed
			 * event even though it supports none of power
			 * controller, attention led, power led and EMI.
			 */
			ctrl_dbg(ctrl, "Unexpected CMD_COMPLETED. Need to "
				 "wait for command completed event.\n");
			ctrl->no_cmd_complete = 0;
		} else {
			ctrl_dbg(ctrl, "Unexpected CMD_COMPLETED. Maybe "
				 "the controller is broken.\n");
		}
	}

	retval = pciehp_readw(ctrl, PCI_EXP_SLTCTL, &slot_ctrl);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTCTRL register\n", __func__);
=======
	/*
	 * Always wait for any previous command that might still be in progress
	 */
	pcie_wait_cmd(ctrl);

	pcie_capability_read_word(pdev, PCI_EXP_SLTCTL, &slot_ctrl);
	if (slot_ctrl == (u16) ~0) {
		ctrl_info(ctrl, "%s: no response from device\n", __func__);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto out;
	}

	slot_ctrl &= ~mask;
	slot_ctrl |= (cmd & mask);
	ctrl->cmd_busy = 1;
	smp_mb();
<<<<<<< HEAD
	retval = pciehp_writew(ctrl, PCI_EXP_SLTCTL, slot_ctrl);
	if (retval)
		ctrl_err(ctrl, "Cannot write to SLOTCTRL register\n");

	/*
	 * Wait for command completion.
	 */
	if (!retval && !ctrl->no_cmd_complete) {
		int poll = 0;
		/*
		 * if hotplug interrupt is not enabled or command
		 * completed interrupt is not enabled, we need to poll
		 * command completed event.
		 */
		if (!(slot_ctrl & PCI_EXP_SLTCTL_HPIE) ||
		    !(slot_ctrl & PCI_EXP_SLTCTL_CCIE))
			poll = 1;
                pcie_wait_cmd(ctrl, poll);
	}
 out:
	mutex_unlock(&ctrl->ctrl_lock);
	return retval;
}

static bool check_link_active(struct controller *ctrl)
{
	bool ret = false;
	u16 lnk_status;

	if (pciehp_readw(ctrl, PCI_EXP_LNKSTA, &lnk_status))
		return ret;

=======
	pcie_capability_write_word(pdev, PCI_EXP_SLTCTL, slot_ctrl);
	ctrl->cmd_started = jiffies;
	ctrl->slot_ctrl = slot_ctrl;

	/*
	 * Optionally wait for the hardware to be ready for a new command,
	 * indicating completion of the above issued command.
	 */
	if (wait)
		pcie_wait_cmd(ctrl);

out:
	mutex_unlock(&ctrl->ctrl_lock);
}

/**
 * pcie_write_cmd - Issue controller command
 * @ctrl: controller to which the command is issued
 * @cmd:  command value written to slot control register
 * @mask: bitmask of slot control register to be modified
 */
static void pcie_write_cmd(struct controller *ctrl, u16 cmd, u16 mask)
{
	pcie_do_write_cmd(ctrl, cmd, mask, true);
}

/* Same as above without waiting for the hardware to latch */
static void pcie_write_cmd_nowait(struct controller *ctrl, u16 cmd, u16 mask)
{
	pcie_do_write_cmd(ctrl, cmd, mask, false);
}

bool pciehp_check_link_active(struct controller *ctrl)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 lnk_status;
	bool ret;

	pcie_capability_read_word(pdev, PCI_EXP_LNKSTA, &lnk_status);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ret = !!(lnk_status & PCI_EXP_LNKSTA_DLLLA);

	if (ret)
		ctrl_dbg(ctrl, "%s: lnk_status = %x\n", __func__, lnk_status);

	return ret;
}

static void __pcie_wait_link_active(struct controller *ctrl, bool active)
{
	int timeout = 1000;

<<<<<<< HEAD
	if (check_link_active(ctrl) == active)
=======
	if (pciehp_check_link_active(ctrl) == active)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;
	while (timeout > 0) {
		msleep(10);
		timeout -= 10;
<<<<<<< HEAD
		if (check_link_active(ctrl) == active)
=======
		if (pciehp_check_link_active(ctrl) == active)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return;
	}
	ctrl_dbg(ctrl, "Data Link Layer Link Active not %s in 1000 msec\n",
			active ? "set" : "cleared");
}

static void pcie_wait_link_active(struct controller *ctrl)
{
	__pcie_wait_link_active(ctrl, true);
}

<<<<<<< HEAD
static void pcie_wait_link_not_active(struct controller *ctrl)
{
	__pcie_wait_link_active(ctrl, false);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static bool pci_bus_check_dev(struct pci_bus *bus, int devfn)
{
	u32 l;
	int count = 0;
	int delay = 1000, step = 20;
	bool found = false;

	do {
		found = pci_bus_read_dev_vendor_id(bus, devfn, &l, 0);
		count++;

		if (found)
			break;

		msleep(step);
		delay -= step;
	} while (delay > 0);

	if (count > 1 && pciehp_debug)
		printk(KERN_DEBUG "pci %04x:%02x:%02x.%d id reading try %d times with interval %d ms to get %08x\n",
			pci_domain_nr(bus), bus->number, PCI_SLOT(devfn),
			PCI_FUNC(devfn), count, step, l);

	return found;
}

int pciehp_check_link_status(struct controller *ctrl)
{
<<<<<<< HEAD
	u16 lnk_status;
	int retval = 0;
	bool found = false;

        /*
         * Data Link Layer Link Active Reporting must be capable for
         * hot-plug capable downstream port. But old controller might
         * not implement it. In this case, we wait for 1000 ms.
         */
        if (ctrl->link_active_reporting)
                pcie_wait_link_active(ctrl);
        else
                msleep(1000);
=======
	struct pci_dev *pdev = ctrl_dev(ctrl);
	bool found;
	u16 lnk_status;

	/*
	 * Data Link Layer Link Active Reporting must be capable for
	 * hot-plug capable downstream port. But old controller might
	 * not implement it. In this case, we wait for 1000 ms.
	*/
	if (ctrl->link_active_reporting)
		pcie_wait_link_active(ctrl);
	else
		msleep(1000);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* wait 100ms before read pci conf, and try in 1s */
	msleep(100);
	found = pci_bus_check_dev(ctrl->pcie->port->subordinate,
					PCI_DEVFN(0, 0));

<<<<<<< HEAD
	retval = pciehp_readw(ctrl, PCI_EXP_LNKSTA, &lnk_status);
	if (retval) {
		ctrl_err(ctrl, "Cannot read LNKSTATUS register\n");
		return retval;
	}

	ctrl_dbg(ctrl, "%s: lnk_status = %x\n", __func__, lnk_status);
	if ((lnk_status & PCI_EXP_LNKSTA_LT) ||
	    !(lnk_status & PCI_EXP_LNKSTA_NLW)) {
		ctrl_err(ctrl, "Link Training Error occurs \n");
		retval = -1;
		return retval;
=======
	pcie_capability_read_word(pdev, PCI_EXP_LNKSTA, &lnk_status);
	ctrl_dbg(ctrl, "%s: lnk_status = %x\n", __func__, lnk_status);
	if ((lnk_status & PCI_EXP_LNKSTA_LT) ||
	    !(lnk_status & PCI_EXP_LNKSTA_NLW)) {
		ctrl_err(ctrl, "link training error: status %#06x\n",
			 lnk_status);
		return -1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	pcie_update_link_speed(ctrl->pcie->port->subordinate, lnk_status);

<<<<<<< HEAD
	if (!found && !retval)
		retval = -1;

	return retval;
=======
	if (!found)
		return -1;

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int __pciehp_link_set(struct controller *ctrl, bool enable)
{
<<<<<<< HEAD
	u16 lnk_ctrl;
	int retval = 0;

	retval = pciehp_readw(ctrl, PCI_EXP_LNKCTL, &lnk_ctrl);
	if (retval) {
		ctrl_err(ctrl, "Cannot read LNKCTRL register\n");
		return retval;
	}
=======
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 lnk_ctrl;

	pcie_capability_read_word(pdev, PCI_EXP_LNKCTL, &lnk_ctrl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (enable)
		lnk_ctrl &= ~PCI_EXP_LNKCTL_LD;
	else
		lnk_ctrl |= PCI_EXP_LNKCTL_LD;

<<<<<<< HEAD
	retval = pciehp_writew(ctrl, PCI_EXP_LNKCTL, lnk_ctrl);
	if (retval) {
		ctrl_err(ctrl, "Cannot write LNKCTRL register\n");
		return retval;
	}
	ctrl_dbg(ctrl, "%s: lnk_ctrl = %x\n", __func__, lnk_ctrl);

	return retval;
=======
	pcie_capability_write_word(pdev, PCI_EXP_LNKCTL, lnk_ctrl);
	ctrl_dbg(ctrl, "%s: lnk_ctrl = %x\n", __func__, lnk_ctrl);
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int pciehp_link_enable(struct controller *ctrl)
{
	return __pciehp_link_set(ctrl, true);
}

<<<<<<< HEAD
static int pciehp_link_disable(struct controller *ctrl)
{
	return __pciehp_link_set(ctrl, false);
}

int pciehp_get_attention_status(struct slot *slot, u8 *status)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_ctrl;
	u8 atten_led_state;
	int retval = 0;

	retval = pciehp_readw(ctrl, PCI_EXP_SLTCTL, &slot_ctrl);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTCTRL register\n", __func__);
		return retval;
	}

	ctrl_dbg(ctrl, "%s: SLOTCTRL %x, value read %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_ctrl);

	atten_led_state = (slot_ctrl & PCI_EXP_SLTCTL_AIC) >> 6;

	switch (atten_led_state) {
	case 0:
		*status = 0xFF;	/* Reserved */
		break;
	case 1:
		*status = 1;	/* On */
		break;
	case 2:
		*status = 2;	/* Blink */
		break;
	case 3:
=======
int pciehp_get_raw_indicator_status(struct hotplug_slot *hotplug_slot,
				    u8 *status)
{
	struct slot *slot = hotplug_slot->private;
	struct pci_dev *pdev = ctrl_dev(slot->ctrl);
	u16 slot_ctrl;

	pcie_capability_read_word(pdev, PCI_EXP_SLTCTL, &slot_ctrl);
	*status = (slot_ctrl & (PCI_EXP_SLTCTL_AIC | PCI_EXP_SLTCTL_PIC)) >> 6;
	return 0;
}

void pciehp_get_attention_status(struct slot *slot, u8 *status)
{
	struct controller *ctrl = slot->ctrl;
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_ctrl;

	pcie_capability_read_word(pdev, PCI_EXP_SLTCTL, &slot_ctrl);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x, value read %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_ctrl);

	switch (slot_ctrl & PCI_EXP_SLTCTL_AIC) {
	case PCI_EXP_SLTCTL_ATTN_IND_ON:
		*status = 1;	/* On */
		break;
	case PCI_EXP_SLTCTL_ATTN_IND_BLINK:
		*status = 2;	/* Blink */
		break;
	case PCI_EXP_SLTCTL_ATTN_IND_OFF:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		*status = 0;	/* Off */
		break;
	default:
		*status = 0xFF;
		break;
	}
<<<<<<< HEAD

	return 0;
}

int pciehp_get_power_status(struct slot *slot, u8 *status)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_ctrl;
	u8 pwr_state;
	int	retval = 0;

	retval = pciehp_readw(ctrl, PCI_EXP_SLTCTL, &slot_ctrl);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTCTRL register\n", __func__);
		return retval;
	}
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x value read %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_ctrl);

	pwr_state = (slot_ctrl & PCI_EXP_SLTCTL_PCC) >> 10;

	switch (pwr_state) {
	case 0:
		*status = 1;
		break;
	case 1:
		*status = 0;
=======
}

void pciehp_get_power_status(struct slot *slot, u8 *status)
{
	struct controller *ctrl = slot->ctrl;
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_ctrl;

	pcie_capability_read_word(pdev, PCI_EXP_SLTCTL, &slot_ctrl);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x value read %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_ctrl);

	switch (slot_ctrl & PCI_EXP_SLTCTL_PCC) {
	case PCI_EXP_SLTCTL_PWR_ON:
		*status = 1;	/* On */
		break;
	case PCI_EXP_SLTCTL_PWR_OFF:
		*status = 0;	/* Off */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	default:
		*status = 0xFF;
		break;
	}
<<<<<<< HEAD

	return retval;
}

int pciehp_get_latch_status(struct slot *slot, u8 *status)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_status;
	int retval;

	retval = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTSTATUS register\n",
			 __func__);
		return retval;
	}
	*status = !!(slot_status & PCI_EXP_SLTSTA_MRLSS);
	return 0;
}

int pciehp_get_adapter_status(struct slot *slot, u8 *status)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_status;
	int retval;

	retval = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTSTATUS register\n",
			 __func__);
		return retval;
	}
	*status = !!(slot_status & PCI_EXP_SLTSTA_PDS);
	return 0;
=======
}

void pciehp_get_latch_status(struct slot *slot, u8 *status)
{
	struct pci_dev *pdev = ctrl_dev(slot->ctrl);
	u16 slot_status;

	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
	*status = !!(slot_status & PCI_EXP_SLTSTA_MRLSS);
}

void pciehp_get_adapter_status(struct slot *slot, u8 *status)
{
	struct pci_dev *pdev = ctrl_dev(slot->ctrl);
	u16 slot_status;

	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
	*status = !!(slot_status & PCI_EXP_SLTSTA_PDS);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

int pciehp_query_power_fault(struct slot *slot)
{
<<<<<<< HEAD
	struct controller *ctrl = slot->ctrl;
	u16 slot_status;
	int retval;

	retval = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (retval) {
		ctrl_err(ctrl, "Cannot check for power fault\n");
		return retval;
	}
	return !!(slot_status & PCI_EXP_SLTSTA_PFD);
}

int pciehp_set_attention_status(struct slot *slot, u8 value)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_cmd;
	u16 cmd_mask;

	cmd_mask = PCI_EXP_SLTCTL_AIC;
	switch (value) {
	case 0 :	/* turn off */
		slot_cmd = 0x00C0;
		break;
	case 1:		/* turn on */
		slot_cmd = 0x0040;
		break;
	case 2:		/* turn blink */
		slot_cmd = 0x0080;
		break;
	default:
		return -EINVAL;
	}
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
	return pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
=======
	struct pci_dev *pdev = ctrl_dev(slot->ctrl);
	u16 slot_status;

	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
	return !!(slot_status & PCI_EXP_SLTSTA_PFD);
}

int pciehp_set_raw_indicator_status(struct hotplug_slot *hotplug_slot,
				    u8 status)
{
	struct slot *slot = hotplug_slot->private;
	struct controller *ctrl = slot->ctrl;

	pcie_write_cmd_nowait(ctrl, status << 6,
			      PCI_EXP_SLTCTL_AIC | PCI_EXP_SLTCTL_PIC);
	return 0;
}

void pciehp_set_attention_status(struct slot *slot, u8 value)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_cmd;

	if (!ATTN_LED(ctrl))
		return;

	switch (value) {
	case 0:		/* turn off */
		slot_cmd = PCI_EXP_SLTCTL_ATTN_IND_OFF;
		break;
	case 1:		/* turn on */
		slot_cmd = PCI_EXP_SLTCTL_ATTN_IND_ON;
		break;
	case 2:		/* turn blink */
		slot_cmd = PCI_EXP_SLTCTL_ATTN_IND_BLINK;
		break;
	default:
		return;
	}
	pcie_write_cmd_nowait(ctrl, slot_cmd, PCI_EXP_SLTCTL_AIC);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void pciehp_green_led_on(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;
<<<<<<< HEAD
	u16 slot_cmd;
	u16 cmd_mask;

	slot_cmd = 0x0100;
	cmd_mask = PCI_EXP_SLTCTL_PIC;
	pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
=======

	if (!PWR_LED(ctrl))
		return;

	pcie_write_cmd_nowait(ctrl, PCI_EXP_SLTCTL_PWR_IND_ON,
			      PCI_EXP_SLTCTL_PIC);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL,
		 PCI_EXP_SLTCTL_PWR_IND_ON);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void pciehp_green_led_off(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;
<<<<<<< HEAD
	u16 slot_cmd;
	u16 cmd_mask;

	slot_cmd = 0x0300;
	cmd_mask = PCI_EXP_SLTCTL_PIC;
	pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
=======

	if (!PWR_LED(ctrl))
		return;

	pcie_write_cmd_nowait(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
			      PCI_EXP_SLTCTL_PIC);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL,
		 PCI_EXP_SLTCTL_PWR_IND_OFF);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void pciehp_green_led_blink(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;
<<<<<<< HEAD
	u16 slot_cmd;
	u16 cmd_mask;

	slot_cmd = 0x0200;
	cmd_mask = PCI_EXP_SLTCTL_PIC;
	pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
}

int pciehp_power_on_slot(struct slot * slot)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_cmd;
	u16 cmd_mask;
	u16 slot_status;
	int retval = 0;

	/* Clear sticky power-fault bit from previous power failures */
	retval = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTSTATUS register\n",
			 __func__);
		return retval;
	}
	slot_status &= PCI_EXP_SLTSTA_PFD;
	if (slot_status) {
		retval = pciehp_writew(ctrl, PCI_EXP_SLTSTA, slot_status);
		if (retval) {
			ctrl_err(ctrl,
				 "%s: Cannot write to SLOTSTATUS register\n",
				 __func__);
			return retval;
		}
	}
	ctrl->power_fault_detected = 0;

	slot_cmd = POWER_ON;
	cmd_mask = PCI_EXP_SLTCTL_PCC;
	retval = pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
	if (retval) {
		ctrl_err(ctrl, "Write %x command failed!\n", slot_cmd);
		return retval;
	}
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
=======

	if (!PWR_LED(ctrl))
		return;

	pcie_write_cmd_nowait(ctrl, PCI_EXP_SLTCTL_PWR_IND_BLINK,
			      PCI_EXP_SLTCTL_PIC);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL,
		 PCI_EXP_SLTCTL_PWR_IND_BLINK);
}

int pciehp_power_on_slot(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_status;
	int retval;

	/* Clear sticky power-fault bit from previous power failures */
	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
	if (slot_status & PCI_EXP_SLTSTA_PFD)
		pcie_capability_write_word(pdev, PCI_EXP_SLTSTA,
					   PCI_EXP_SLTSTA_PFD);
	ctrl->power_fault_detected = 0;

	pcie_write_cmd(ctrl, PCI_EXP_SLTCTL_PWR_ON, PCI_EXP_SLTCTL_PCC);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL,
		 PCI_EXP_SLTCTL_PWR_ON);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	retval = pciehp_link_enable(ctrl);
	if (retval)
		ctrl_err(ctrl, "%s: Can not enable the link!\n", __func__);

	return retval;
}

<<<<<<< HEAD
int pciehp_power_off_slot(struct slot * slot)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_cmd;
	u16 cmd_mask;
	int retval;

	/* Disable the link at first */
	pciehp_link_disable(ctrl);
	/* wait the link is down */
	if (ctrl->link_active_reporting)
		pcie_wait_link_not_active(ctrl);
	else
		msleep(1000);

	slot_cmd = POWER_OFF;
	cmd_mask = PCI_EXP_SLTCTL_PCC;
	retval = pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
	if (retval) {
		ctrl_err(ctrl, "Write command failed!\n");
		return retval;
	}
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
	return 0;
}

static irqreturn_t pcie_isr(int irq, void *dev_id)
{
	struct controller *ctrl = (struct controller *)dev_id;
	struct slot *slot = ctrl->slot;
	u16 detected, intr_loc;

	/*
	 * In order to guarantee that all interrupt events are
	 * serviced, we need to re-inspect Slot Status register after
	 * clearing what is presumed to be the last pending interrupt.
	 */
	intr_loc = 0;
	do {
		if (pciehp_readw(ctrl, PCI_EXP_SLTSTA, &detected)) {
			ctrl_err(ctrl, "%s: Cannot read SLOTSTATUS\n",
				 __func__);
			return IRQ_NONE;
		}

		detected &= (PCI_EXP_SLTSTA_ABP | PCI_EXP_SLTSTA_PFD |
			     PCI_EXP_SLTSTA_MRLSC | PCI_EXP_SLTSTA_PDC |
			     PCI_EXP_SLTSTA_CC);
		detected &= ~intr_loc;
		intr_loc |= detected;
		if (!intr_loc)
			return IRQ_NONE;
		if (detected && pciehp_writew(ctrl, PCI_EXP_SLTSTA, intr_loc)) {
			ctrl_err(ctrl, "%s: Cannot write to SLOTSTATUS\n",
				 __func__);
			return IRQ_NONE;
		}
	} while (detected);

	ctrl_dbg(ctrl, "%s: intr_loc %x\n", __func__, intr_loc);

	/* Check Command Complete Interrupt Pending */
	if (intr_loc & PCI_EXP_SLTSTA_CC) {
=======
void pciehp_power_off_slot(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;

	pcie_write_cmd(ctrl, PCI_EXP_SLTCTL_PWR_OFF, PCI_EXP_SLTCTL_PCC);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL,
		 PCI_EXP_SLTCTL_PWR_OFF);
}

static irqreturn_t pciehp_isr(int irq, void *dev_id)
{
	struct controller *ctrl = (struct controller *)dev_id;
	struct pci_dev *pdev = ctrl_dev(ctrl);
	struct slot *slot = ctrl->slot;
	u16 status, events;
	u8 present;
	bool link;

	/* Interrupts cannot originate from a controller that's asleep */
	if (pdev->current_state == PCI_D3cold)
		return IRQ_NONE;

	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &status);
	if (status == (u16) ~0) {
		ctrl_info(ctrl, "%s: no response from device\n", __func__);
		return IRQ_NONE;
	}

	/*
	 * Slot Status contains plain status bits as well as event
	 * notification bits; right now we only want the event bits.
	 */
	events = status & (PCI_EXP_SLTSTA_ABP | PCI_EXP_SLTSTA_PFD |
			   PCI_EXP_SLTSTA_PDC | PCI_EXP_SLTSTA_CC |
			   PCI_EXP_SLTSTA_DLLSC);

	/*
	 * If we've already reported a power fault, don't report it again
	 * until we've done something to handle it.
	 */
	if (ctrl->power_fault_detected)
		events &= ~PCI_EXP_SLTSTA_PFD;

	if (!events)
		return IRQ_NONE;

	/* Capture link status before clearing interrupts */
	if (events & PCI_EXP_SLTSTA_DLLSC)
		link = pciehp_check_link_active(ctrl);

	pcie_capability_write_word(pdev, PCI_EXP_SLTSTA, events);
	ctrl_dbg(ctrl, "pending interrupts %#06x from Slot Status\n", events);

	/* Check Command Complete Interrupt Pending */
	if (events & PCI_EXP_SLTSTA_CC) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ctrl->cmd_busy = 0;
		smp_mb();
		wake_up(&ctrl->queue);
	}

<<<<<<< HEAD
	if (!(intr_loc & ~PCI_EXP_SLTSTA_CC))
		return IRQ_HANDLED;

	/* Check MRL Sensor Changed */
	if (intr_loc & PCI_EXP_SLTSTA_MRLSC)
		pciehp_handle_switch_change(slot);

	/* Check Attention Button Pressed */
	if (intr_loc & PCI_EXP_SLTSTA_ABP)
		pciehp_handle_attention_button(slot);

	/* Check Presence Detect Changed */
	if (intr_loc & PCI_EXP_SLTSTA_PDC)
		pciehp_handle_presence_change(slot);

	/* Check Power Fault Detected */
	if ((intr_loc & PCI_EXP_SLTSTA_PFD) && !ctrl->power_fault_detected) {
		ctrl->power_fault_detected = 1;
		pciehp_handle_power_fault(slot);
	}
	return IRQ_HANDLED;
}

int pcie_enable_notification(struct controller *ctrl)
=======
	if (pdev->ignore_hotplug) {
		ctrl_dbg(ctrl, "ignoring hotplug event %#06x\n", events);
		return IRQ_HANDLED;
	}

	/* Check Attention Button Pressed */
	if (events & PCI_EXP_SLTSTA_ABP) {
		ctrl_info(ctrl, "Slot(%s): Attention button pressed\n",
			  slot_name(slot));
		pciehp_queue_interrupt_event(slot, INT_BUTTON_PRESS);
	}

	/* Check Presence Detect Changed */
	if (events & PCI_EXP_SLTSTA_PDC) {
		present = !!(status & PCI_EXP_SLTSTA_PDS);
		ctrl_info(ctrl, "Slot(%s): Card %spresent\n", slot_name(slot),
			  present ? "" : "not ");
		pciehp_queue_interrupt_event(slot, present ? INT_PRESENCE_ON :
					     INT_PRESENCE_OFF);
	}

	/* Check Power Fault Detected */
	if ((events & PCI_EXP_SLTSTA_PFD) && !ctrl->power_fault_detected) {
		ctrl->power_fault_detected = 1;
		ctrl_err(ctrl, "Slot(%s): Power fault\n", slot_name(slot));
		pciehp_queue_interrupt_event(slot, INT_POWER_FAULT);
	}

	if (events & PCI_EXP_SLTSTA_DLLSC) {
		ctrl_info(ctrl, "Slot(%s): Link %s\n", slot_name(slot),
			  link ? "Up" : "Down");
		pciehp_queue_interrupt_event(slot, link ? INT_LINK_UP :
					     INT_LINK_DOWN);
	}

	return IRQ_HANDLED;
}

static irqreturn_t pcie_isr(int irq, void *dev_id)
{
	irqreturn_t rc, handled = IRQ_NONE;

	/*
	 * To guarantee that all interrupt events are serviced, we need to
	 * re-inspect Slot Status register after clearing what is presumed
	 * to be the last pending interrupt.
	 */
	do {
		rc = pciehp_isr(irq, dev_id);
		if (rc == IRQ_HANDLED)
			handled = IRQ_HANDLED;
	} while (rc == IRQ_HANDLED);

	/* Return IRQ_HANDLED if we handled one or more events */
	return handled;
}

static void pcie_enable_notification(struct controller *ctrl)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	u16 cmd, mask;

	/*
	 * TBD: Power fault detected software notification support.
	 *
	 * Power fault detected software notification is not enabled
	 * now, because it caused power fault detected interrupt storm
	 * on some machines. On those machines, power fault detected
	 * bit in the slot status register was set again immediately
	 * when it is cleared in the interrupt service routine, and
	 * next power fault detected interrupt was notified again.
	 */
<<<<<<< HEAD
	cmd = PCI_EXP_SLTCTL_PDCE;
	if (ATTN_BUTTN(ctrl))
		cmd |= PCI_EXP_SLTCTL_ABPE;
	if (MRL_SENS(ctrl))
		cmd |= PCI_EXP_SLTCTL_MRLSCE;
=======

	/*
	 * Always enable link events: thus link-up and link-down shall
	 * always be treated as hotplug and unplug respectively. Enable
	 * presence detect only if Attention Button is not present.
	 */
	cmd = PCI_EXP_SLTCTL_DLLSCE;
	if (ATTN_BUTTN(ctrl))
		cmd |= PCI_EXP_SLTCTL_ABPE;
	else
		cmd |= PCI_EXP_SLTCTL_PDCE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!pciehp_poll_mode)
		cmd |= PCI_EXP_SLTCTL_HPIE | PCI_EXP_SLTCTL_CCIE;

	mask = (PCI_EXP_SLTCTL_PDCE | PCI_EXP_SLTCTL_ABPE |
<<<<<<< HEAD
		PCI_EXP_SLTCTL_MRLSCE | PCI_EXP_SLTCTL_PFDE |
		PCI_EXP_SLTCTL_HPIE | PCI_EXP_SLTCTL_CCIE);

	if (pcie_write_cmd(ctrl, cmd, mask)) {
		ctrl_err(ctrl, "Cannot enable software notification\n");
		return -1;
	}
	return 0;
=======
		PCI_EXP_SLTCTL_PFDE |
		PCI_EXP_SLTCTL_HPIE | PCI_EXP_SLTCTL_CCIE |
		PCI_EXP_SLTCTL_DLLSCE);

	pcie_write_cmd_nowait(ctrl, cmd, mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, cmd);
}

void pcie_reenable_notification(struct controller *ctrl)
{
	/*
	 * Clear both Presence and Data Link Layer Changed to make sure
	 * those events still fire after we have re-enabled them.
	 */
	pcie_capability_write_word(ctrl->pcie->port, PCI_EXP_SLTSTA,
				   PCI_EXP_SLTSTA_PDC | PCI_EXP_SLTSTA_DLLSC);
	pcie_enable_notification(ctrl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void pcie_disable_notification(struct controller *ctrl)
{
	u16 mask;
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mask = (PCI_EXP_SLTCTL_PDCE | PCI_EXP_SLTCTL_ABPE |
		PCI_EXP_SLTCTL_MRLSCE | PCI_EXP_SLTCTL_PFDE |
		PCI_EXP_SLTCTL_HPIE | PCI_EXP_SLTCTL_CCIE |
		PCI_EXP_SLTCTL_DLLSCE);
<<<<<<< HEAD
	if (pcie_write_cmd(ctrl, 0, mask))
		ctrl_warn(ctrl, "Cannot disable software notification\n");
=======
	pcie_write_cmd(ctrl, 0, mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, 0);
}

/*
 * pciehp has a 1:1 bus:slot relationship so we ultimately want a secondary
 * bus reset of the bridge, but at the same time we want to ensure that it is
 * not seen as a hot-unplug, followed by the hot-plug of the device. Thus,
 * disable link state notification and presence detection change notification
 * momentarily, if we see that they could interfere. Also, clear any spurious
 * events after.
 */
int pciehp_reset_slot(struct slot *slot, int probe)
{
	struct controller *ctrl = slot->ctrl;
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 stat_mask = 0, ctrl_mask = 0;

	if (probe)
		return 0;

	if (!ATTN_BUTTN(ctrl)) {
		ctrl_mask |= PCI_EXP_SLTCTL_PDCE;
		stat_mask |= PCI_EXP_SLTSTA_PDC;
	}
	ctrl_mask |= PCI_EXP_SLTCTL_DLLSCE;
	stat_mask |= PCI_EXP_SLTSTA_DLLSC;

	pcie_write_cmd(ctrl, 0, ctrl_mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, 0);
	if (pciehp_poll_mode)
		del_timer_sync(&ctrl->poll_timer);

	pci_reset_bridge_secondary_bus(ctrl->pcie->port);

	pcie_capability_write_word(pdev, PCI_EXP_SLTSTA, stat_mask);
	pcie_write_cmd_nowait(ctrl, ctrl_mask, ctrl_mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, ctrl_mask);
	if (pciehp_poll_mode)
		int_poll_timeout(ctrl->poll_timer.data);

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

int pcie_init_notification(struct controller *ctrl)
{
	if (pciehp_request_irq(ctrl))
		return -1;
<<<<<<< HEAD
	if (pcie_enable_notification(ctrl)) {
		pciehp_free_irq(ctrl);
		return -1;
	}
=======
	pcie_enable_notification(ctrl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ctrl->notification_enabled = 1;
	return 0;
}

<<<<<<< HEAD
static void pcie_shutdown_notification(struct controller *ctrl)
=======
void pcie_shutdown_notification(struct controller *ctrl)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (ctrl->notification_enabled) {
		pcie_disable_notification(ctrl);
		pciehp_free_irq(ctrl);
		ctrl->notification_enabled = 0;
	}
}

static int pcie_init_slot(struct controller *ctrl)
{
	struct slot *slot;
<<<<<<< HEAD
	char name[32];
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	slot = kzalloc(sizeof(*slot), GFP_KERNEL);
	if (!slot)
		return -ENOMEM;

<<<<<<< HEAD
	snprintf(name, sizeof(name), "pciehp-%u", PSN(ctrl));
	slot->wq = alloc_workqueue(name, 0, 0);
=======
	slot->wq = alloc_workqueue("pciehp-%u", 0, 0, PSN(ctrl));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!slot->wq)
		goto abort;

	slot->ctrl = ctrl;
	mutex_init(&slot->lock);
<<<<<<< HEAD
=======
	mutex_init(&slot->hotplug_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	INIT_DELAYED_WORK(&slot->work, pciehp_queue_pushbutton_work);
	ctrl->slot = slot;
	return 0;
abort:
	kfree(slot);
	return -ENOMEM;
}

static void pcie_cleanup_slot(struct controller *ctrl)
{
	struct slot *slot = ctrl->slot;
<<<<<<< HEAD
	cancel_delayed_work(&slot->work);
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	destroy_workqueue(slot->wq);
	kfree(slot);
}

static inline void dbg_ctrl(struct controller *ctrl)
{
<<<<<<< HEAD
	int i;
	u16 reg16;
	struct pci_dev *pdev = ctrl->pcie->port;
=======
	struct pci_dev *pdev = ctrl->pcie->port;
	u16 reg16;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!pciehp_debug)
		return;

<<<<<<< HEAD
	ctrl_info(ctrl, "Hotplug Controller:\n");
	ctrl_info(ctrl, "  Seg/Bus/Dev/Func/IRQ : %s IRQ %d\n",
		  pci_name(pdev), pdev->irq);
	ctrl_info(ctrl, "  Vendor ID            : 0x%04x\n", pdev->vendor);
	ctrl_info(ctrl, "  Device ID            : 0x%04x\n", pdev->device);
	ctrl_info(ctrl, "  Subsystem ID         : 0x%04x\n",
		  pdev->subsystem_device);
	ctrl_info(ctrl, "  Subsystem Vendor ID  : 0x%04x\n",
		  pdev->subsystem_vendor);
	ctrl_info(ctrl, "  PCIe Cap offset      : 0x%02x\n",
		  pci_pcie_cap(pdev));
	for (i = 0; i < DEVICE_COUNT_RESOURCE; i++) {
		if (!pci_resource_len(pdev, i))
			continue;
		ctrl_info(ctrl, "  PCI resource [%d]     : %pR\n",
			  i, &pdev->resource[i]);
	}
	ctrl_info(ctrl, "Slot Capabilities      : 0x%08x\n", ctrl->slot_cap);
	ctrl_info(ctrl, "  Physical Slot Number : %d\n", PSN(ctrl));
	ctrl_info(ctrl, "  Attention Button     : %3s\n",
		  ATTN_BUTTN(ctrl) ? "yes" : "no");
	ctrl_info(ctrl, "  Power Controller     : %3s\n",
		  POWER_CTRL(ctrl) ? "yes" : "no");
	ctrl_info(ctrl, "  MRL Sensor           : %3s\n",
		  MRL_SENS(ctrl)   ? "yes" : "no");
	ctrl_info(ctrl, "  Attention Indicator  : %3s\n",
		  ATTN_LED(ctrl)   ? "yes" : "no");
	ctrl_info(ctrl, "  Power Indicator      : %3s\n",
		  PWR_LED(ctrl)    ? "yes" : "no");
	ctrl_info(ctrl, "  Hot-Plug Surprise    : %3s\n",
		  HP_SUPR_RM(ctrl) ? "yes" : "no");
	ctrl_info(ctrl, "  EMI Present          : %3s\n",
		  EMI(ctrl)        ? "yes" : "no");
	ctrl_info(ctrl, "  Command Completed    : %3s\n",
		  NO_CMD_CMPL(ctrl) ? "no" : "yes");
	pciehp_readw(ctrl, PCI_EXP_SLTSTA, &reg16);
	ctrl_info(ctrl, "Slot Status            : 0x%04x\n", reg16);
	pciehp_readw(ctrl, PCI_EXP_SLTCTL, &reg16);
	ctrl_info(ctrl, "Slot Control           : 0x%04x\n", reg16);
}

=======
	ctrl_info(ctrl, "Slot Capabilities      : 0x%08x\n", ctrl->slot_cap);
	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &reg16);
	ctrl_info(ctrl, "Slot Status            : 0x%04x\n", reg16);
	pcie_capability_read_word(pdev, PCI_EXP_SLTCTL, &reg16);
	ctrl_info(ctrl, "Slot Control           : 0x%04x\n", reg16);
}

#define FLAG(x, y)	(((x) & (y)) ? '+' : '-')

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct controller *pcie_init(struct pcie_device *dev)
{
	struct controller *ctrl;
	u32 slot_cap, link_cap;
	struct pci_dev *pdev = dev->port;

	ctrl = kzalloc(sizeof(*ctrl), GFP_KERNEL);
	if (!ctrl) {
		dev_err(&dev->device, "%s: Out of memory\n", __func__);
		goto abort;
	}
	ctrl->pcie = dev;
<<<<<<< HEAD
	if (pciehp_readl(ctrl, PCI_EXP_SLTCAP, &slot_cap)) {
		ctrl_err(ctrl, "Cannot read SLOTCAP register\n");
		goto abort_ctrl;
	}
=======
	pcie_capability_read_dword(pdev, PCI_EXP_SLTCAP, &slot_cap);

	if (pdev->hotplug_user_indicators)
		slot_cap &= ~(PCI_EXP_SLTCAP_AIP | PCI_EXP_SLTCAP_PIP);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ctrl->slot_cap = slot_cap;
	mutex_init(&ctrl->ctrl_lock);
	init_waitqueue_head(&ctrl->queue);
	dbg_ctrl(ctrl);
<<<<<<< HEAD
	/*
	 * Controller doesn't notify of command completion if the "No
	 * Command Completed Support" bit is set in Slot Capability
	 * register or the controller supports none of power
	 * controller, attention led, power led and EMI.
	 */
	if (NO_CMD_CMPL(ctrl) ||
	    !(POWER_CTRL(ctrl) | ATTN_LED(ctrl) | PWR_LED(ctrl) | EMI(ctrl)))
	    ctrl->no_cmd_complete = 1;

        /* Check if Data Link Layer Link Active Reporting is implemented */
        if (pciehp_readl(ctrl, PCI_EXP_LNKCAP, &link_cap)) {
                ctrl_err(ctrl, "%s: Cannot read LNKCAP register\n", __func__);
                goto abort_ctrl;
        }
        if (link_cap & PCI_EXP_LNKCAP_DLLLARC) {
                ctrl_dbg(ctrl, "Link Active Reporting supported\n");
                ctrl->link_active_reporting = 1;
        }

	/* Clear all remaining event bits in Slot Status register */
	if (pciehp_writew(ctrl, PCI_EXP_SLTSTA, 0x1f))
		goto abort_ctrl;

	/* Disable sotfware notification */
	pcie_disable_notification(ctrl);

	ctrl_info(ctrl, "HPC vendor_id %x device_id %x ss_vid %x ss_did %x\n",
		  pdev->vendor, pdev->device, pdev->subsystem_vendor,
		  pdev->subsystem_device);
=======

	/* Check if Data Link Layer Link Active Reporting is implemented */
	pcie_capability_read_dword(pdev, PCI_EXP_LNKCAP, &link_cap);
	if (link_cap & PCI_EXP_LNKCAP_DLLLARC)
		ctrl->link_active_reporting = 1;

	/* Clear all remaining event bits in Slot Status register */
	pcie_capability_write_word(pdev, PCI_EXP_SLTSTA,
		PCI_EXP_SLTSTA_ABP | PCI_EXP_SLTSTA_PFD |
		PCI_EXP_SLTSTA_MRLSC | PCI_EXP_SLTSTA_PDC |
		PCI_EXP_SLTSTA_CC | PCI_EXP_SLTSTA_DLLSC);

	ctrl_info(ctrl, "Slot #%d AttnBtn%c PwrCtrl%c MRL%c AttnInd%c PwrInd%c HotPlug%c Surprise%c Interlock%c NoCompl%c LLActRep%c\n",
		(slot_cap & PCI_EXP_SLTCAP_PSN) >> 19,
		FLAG(slot_cap, PCI_EXP_SLTCAP_ABP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_PCP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_MRLSP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_AIP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_PIP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_HPC),
		FLAG(slot_cap, PCI_EXP_SLTCAP_HPS),
		FLAG(slot_cap, PCI_EXP_SLTCAP_EIP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_NCCS),
		FLAG(link_cap, PCI_EXP_LNKCAP_DLLLARC));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (pcie_init_slot(ctrl))
		goto abort_ctrl;

	return ctrl;

abort_ctrl:
	kfree(ctrl);
abort:
	return NULL;
}

void pciehp_release_ctrl(struct controller *ctrl)
{
<<<<<<< HEAD
	pcie_shutdown_notification(ctrl);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pcie_cleanup_slot(ctrl);
	kfree(ctrl);
}
