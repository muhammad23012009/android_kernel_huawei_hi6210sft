/*
 * udc.c - ChipIdea UDC driver
 *
 * Copyright (C) 2008 Chipidea - MIPS Technologies, Inc. All rights reserved.
 *
 * Author: David Lopo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/dmapool.h>
#include <linux/err.h>
#include <linux/irqreturn.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
<<<<<<< HEAD
#include <linux/usb/otg.h>
=======
#include <linux/usb/otg-fsm.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/usb/chipidea.h>

#include "ci.h"
#include "udc.h"
#include "bits.h"
<<<<<<< HEAD
#include "debug.h"
=======
#include "otg.h"
#include "otg_fsm.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* control endpoint description */
static const struct usb_endpoint_descriptor
ctrl_endpt_out_desc = {
	.bLength         = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bEndpointAddress = USB_DIR_OUT,
	.bmAttributes    = USB_ENDPOINT_XFER_CONTROL,
	.wMaxPacketSize  = cpu_to_le16(CTRL_PAYLOAD_MAX),
};

static const struct usb_endpoint_descriptor
ctrl_endpt_in_desc = {
	.bLength         = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes    = USB_ENDPOINT_XFER_CONTROL,
	.wMaxPacketSize  = cpu_to_le16(CTRL_PAYLOAD_MAX),
};

/**
 * hw_ep_bit: calculates the bit number
 * @num: endpoint number
 * @dir: endpoint direction
 *
 * This function returns bit number
 */
static inline int hw_ep_bit(int num, int dir)
{
<<<<<<< HEAD
	return num + (dir ? 16 : 0);
}

static inline int ep_to_bit(struct ci13xxx *ci, int n)
=======
	return num + ((dir == TX) ? 16 : 0);
}

static inline int ep_to_bit(struct ci_hdrc *ci, int n)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int fill = 16 - ci->hw_ep_max / 2;

	if (n >= ci->hw_ep_max / 2)
		n += fill;

	return n;
}

/**
 * hw_device_state: enables/disables interrupts (execute without interruption)
 * @dma: 0 => disable, !0 => enable and set dma engine
 *
 * This function returns an error code
 */
<<<<<<< HEAD
static int hw_device_state(struct ci13xxx *ci, u32 dma)
=======
static int hw_device_state(struct ci_hdrc *ci, u32 dma)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (dma) {
		hw_write(ci, OP_ENDPTLISTADDR, ~0, dma);
		/* interrupt, error, port change, reset, sleep/suspend */
		hw_write(ci, OP_USBINTR, ~0,
			     USBi_UI|USBi_UEI|USBi_PCI|USBi_URI|USBi_SLI);
	} else {
		hw_write(ci, OP_USBINTR, ~0, 0);
	}
	return 0;
}

/**
 * hw_ep_flush: flush endpoint fifo (execute without interruption)
 * @num: endpoint number
 * @dir: endpoint direction
 *
 * This function returns an error code
 */
<<<<<<< HEAD
static int hw_ep_flush(struct ci13xxx *ci, int num, int dir)
=======
static int hw_ep_flush(struct ci_hdrc *ci, int num, int dir)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int n = hw_ep_bit(num, dir);

	do {
		/* flush any pending transfer */
		hw_write(ci, OP_ENDPTFLUSH, ~0, BIT(n));
		while (hw_read(ci, OP_ENDPTFLUSH, BIT(n)))
			cpu_relax();
	} while (hw_read(ci, OP_ENDPTSTAT, BIT(n)));

	return 0;
}

/**
 * hw_ep_disable: disables endpoint (execute without interruption)
 * @num: endpoint number
 * @dir: endpoint direction
 *
 * This function returns an error code
 */
<<<<<<< HEAD
static int hw_ep_disable(struct ci13xxx *ci, int num, int dir)
{
	hw_ep_flush(ci, num, dir);
	hw_write(ci, OP_ENDPTCTRL + num,
		 dir ? ENDPTCTRL_TXE : ENDPTCTRL_RXE, 0);
=======
static int hw_ep_disable(struct ci_hdrc *ci, int num, int dir)
{
	hw_write(ci, OP_ENDPTCTRL + num,
		 (dir == TX) ? ENDPTCTRL_TXE : ENDPTCTRL_RXE, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

/**
 * hw_ep_enable: enables endpoint (execute without interruption)
 * @num:  endpoint number
 * @dir:  endpoint direction
 * @type: endpoint type
 *
 * This function returns an error code
 */
<<<<<<< HEAD
static int hw_ep_enable(struct ci13xxx *ci, int num, int dir, int type)
{
	u32 mask, data;

	if (dir) {
=======
static int hw_ep_enable(struct ci_hdrc *ci, int num, int dir, int type)
{
	u32 mask, data;

	if (dir == TX) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		mask  = ENDPTCTRL_TXT;  /* type    */
		data  = type << __ffs(mask);

		mask |= ENDPTCTRL_TXS;  /* unstall */
		mask |= ENDPTCTRL_TXR;  /* reset data toggle */
		data |= ENDPTCTRL_TXR;
		mask |= ENDPTCTRL_TXE;  /* enable  */
		data |= ENDPTCTRL_TXE;
	} else {
		mask  = ENDPTCTRL_RXT;  /* type    */
		data  = type << __ffs(mask);

		mask |= ENDPTCTRL_RXS;  /* unstall */
		mask |= ENDPTCTRL_RXR;  /* reset data toggle */
		data |= ENDPTCTRL_RXR;
		mask |= ENDPTCTRL_RXE;  /* enable  */
		data |= ENDPTCTRL_RXE;
	}
	hw_write(ci, OP_ENDPTCTRL + num, mask, data);
	return 0;
}

/**
 * hw_ep_get_halt: return endpoint halt status
 * @num: endpoint number
 * @dir: endpoint direction
 *
 * This function returns 1 if endpoint halted
 */
<<<<<<< HEAD
static int hw_ep_get_halt(struct ci13xxx *ci, int num, int dir)
{
	u32 mask = dir ? ENDPTCTRL_TXS : ENDPTCTRL_RXS;
=======
static int hw_ep_get_halt(struct ci_hdrc *ci, int num, int dir)
{
	u32 mask = (dir == TX) ? ENDPTCTRL_TXS : ENDPTCTRL_RXS;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return hw_read(ci, OP_ENDPTCTRL + num, mask) ? 1 : 0;
}

/**
<<<<<<< HEAD
 * hw_test_and_clear_setup_status: test & clear setup status (execute without
 *                                 interruption)
 * @n: endpoint number
 *
 * This function returns setup status
 */
static int hw_test_and_clear_setup_status(struct ci13xxx *ci, int n)
{
	n = ep_to_bit(ci, n);
	return hw_test_and_clear(ci, OP_ENDPTSETUPSTAT, BIT(n));
}

/**
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * hw_ep_prime: primes endpoint (execute without interruption)
 * @num:     endpoint number
 * @dir:     endpoint direction
 * @is_ctrl: true if control endpoint
 *
 * This function returns an error code
 */
<<<<<<< HEAD
static int hw_ep_prime(struct ci13xxx *ci, int num, int dir, int is_ctrl)
{
	int n = hw_ep_bit(num, dir);

=======
static int hw_ep_prime(struct ci_hdrc *ci, int num, int dir, int is_ctrl)
{
	int n = hw_ep_bit(num, dir);

	/* Synchronize before ep prime */
	wmb();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (is_ctrl && dir == RX && hw_read(ci, OP_ENDPTSETUPSTAT, BIT(num)))
		return -EAGAIN;

	hw_write(ci, OP_ENDPTPRIME, ~0, BIT(n));

	while (hw_read(ci, OP_ENDPTPRIME, BIT(n)))
		cpu_relax();
	if (is_ctrl && dir == RX && hw_read(ci, OP_ENDPTSETUPSTAT, BIT(num)))
		return -EAGAIN;

	/* status shoult be tested according with manual but it doesn't work */
	return 0;
}

/**
 * hw_ep_set_halt: configures ep halt & resets data toggle after clear (execute
 *                 without interruption)
 * @num:   endpoint number
 * @dir:   endpoint direction
 * @value: true => stall, false => unstall
 *
 * This function returns an error code
 */
<<<<<<< HEAD
static int hw_ep_set_halt(struct ci13xxx *ci, int num, int dir, int value)
=======
static int hw_ep_set_halt(struct ci_hdrc *ci, int num, int dir, int value)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (value != 0 && value != 1)
		return -EINVAL;

	do {
<<<<<<< HEAD
		enum ci13xxx_regs reg = OP_ENDPTCTRL + num;
		u32 mask_xs = dir ? ENDPTCTRL_TXS : ENDPTCTRL_RXS;
		u32 mask_xr = dir ? ENDPTCTRL_TXR : ENDPTCTRL_RXR;
=======
		enum ci_hw_regs reg = OP_ENDPTCTRL + num;
		u32 mask_xs = (dir == TX) ? ENDPTCTRL_TXS : ENDPTCTRL_RXS;
		u32 mask_xr = (dir == TX) ? ENDPTCTRL_TXR : ENDPTCTRL_RXR;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* data toggle - reserved for EP0 but it's in ESS */
		hw_write(ci, reg, mask_xs|mask_xr,
			  value ? mask_xs : mask_xr);
	} while (value != hw_ep_get_halt(ci, num, dir));

	return 0;
}

/**
 * hw_is_port_high_speed: test if port is high speed
 *
 * This function returns true if high speed port
 */
<<<<<<< HEAD
static int hw_port_is_high_speed(struct ci13xxx *ci)
=======
static int hw_port_is_high_speed(struct ci_hdrc *ci)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return ci->hw_bank.lpm ? hw_read(ci, OP_DEVLC, DEVLC_PSPD) :
		hw_read(ci, OP_PORTSC, PORTSC_HSP);
}

/**
<<<<<<< HEAD
 * hw_read_intr_enable: returns interrupt enable register
 *
 * This function returns register data
 */
static u32 hw_read_intr_enable(struct ci13xxx *ci)
{
	return hw_read(ci, OP_USBINTR, ~0);
}

/**
 * hw_read_intr_status: returns interrupt status register
 *
 * This function returns register data
 */
static u32 hw_read_intr_status(struct ci13xxx *ci)
{
	return hw_read(ci, OP_USBSTS, ~0);
}

/**
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * hw_test_and_clear_complete: test & clear complete status (execute without
 *                             interruption)
 * @n: endpoint number
 *
 * This function returns complete status
 */
<<<<<<< HEAD
static int hw_test_and_clear_complete(struct ci13xxx *ci, int n)
=======
static int hw_test_and_clear_complete(struct ci_hdrc *ci, int n)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	n = ep_to_bit(ci, n);
	return hw_test_and_clear(ci, OP_ENDPTCOMPLETE, BIT(n));
}

/**
 * hw_test_and_clear_intr_active: test & clear active interrupts (execute
 *                                without interruption)
 *
 * This function returns active interrutps
 */
<<<<<<< HEAD
static u32 hw_test_and_clear_intr_active(struct ci13xxx *ci)
=======
static u32 hw_test_and_clear_intr_active(struct ci_hdrc *ci)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	u32 reg = hw_read_intr_status(ci) & hw_read_intr_enable(ci);

	hw_write(ci, OP_USBSTS, ~0, reg);
	return reg;
}

/**
 * hw_test_and_clear_setup_guard: test & clear setup guard (execute without
 *                                interruption)
 *
 * This function returns guard value
 */
<<<<<<< HEAD
static int hw_test_and_clear_setup_guard(struct ci13xxx *ci)
=======
static int hw_test_and_clear_setup_guard(struct ci_hdrc *ci)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return hw_test_and_write(ci, OP_USBCMD, USBCMD_SUTW, 0);
}

/**
 * hw_test_and_set_setup_guard: test & set setup guard (execute without
 *                              interruption)
 *
 * This function returns guard value
 */
<<<<<<< HEAD
static int hw_test_and_set_setup_guard(struct ci13xxx *ci)
=======
static int hw_test_and_set_setup_guard(struct ci_hdrc *ci)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return hw_test_and_write(ci, OP_USBCMD, USBCMD_SUTW, USBCMD_SUTW);
}

/**
 * hw_usb_set_address: configures USB address (execute without interruption)
 * @value: new USB address
 *
 * This function explicitly sets the address, without the "USBADRA" (advance)
 * feature, which is not supported by older versions of the controller.
 */
<<<<<<< HEAD
static void hw_usb_set_address(struct ci13xxx *ci, u8 value)
=======
static void hw_usb_set_address(struct ci_hdrc *ci, u8 value)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	hw_write(ci, OP_DEVICEADDR, DEVICEADDR_USBADR,
		 value << __ffs(DEVICEADDR_USBADR));
}

/**
 * hw_usb_reset: restart device after a bus reset (execute without
 *               interruption)
 *
 * This function returns an error code
 */
<<<<<<< HEAD
static int hw_usb_reset(struct ci13xxx *ci)
=======
static int hw_usb_reset(struct ci_hdrc *ci)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	hw_usb_set_address(ci, 0);

	/* ESS flushes only at end?!? */
	hw_write(ci, OP_ENDPTFLUSH,    ~0, ~0);

	/* clear setup token semaphores */
	hw_write(ci, OP_ENDPTSETUPSTAT, 0,  0);

	/* clear complete status */
	hw_write(ci, OP_ENDPTCOMPLETE,  0,  0);

	/* wait until all bits cleared */
	while (hw_read(ci, OP_ENDPTPRIME, ~0))
		udelay(10);             /* not RTOS friendly */

	/* reset all endpoints ? */

	/* reset internal status and wait for further instructions
	   no need to verify the port reset status (ESS does it) */

	return 0;
}

/******************************************************************************
 * UTIL block
 *****************************************************************************/
<<<<<<< HEAD
=======

static int add_td_to_list(struct ci_hw_ep *hwep, struct ci_hw_req *hwreq,
			  unsigned length)
{
	int i;
	u32 temp;
	struct td_node *lastnode, *node = kzalloc(sizeof(struct td_node),
						  GFP_ATOMIC);

	if (node == NULL)
		return -ENOMEM;

	node->ptr = dma_pool_zalloc(hwep->td_pool, GFP_ATOMIC, &node->dma);
	if (node->ptr == NULL) {
		kfree(node);
		return -ENOMEM;
	}

	node->ptr->token = cpu_to_le32(length << __ffs(TD_TOTAL_BYTES));
	node->ptr->token &= cpu_to_le32(TD_TOTAL_BYTES);
	node->ptr->token |= cpu_to_le32(TD_STATUS_ACTIVE);
	if (hwep->type == USB_ENDPOINT_XFER_ISOC && hwep->dir == TX) {
		u32 mul = hwreq->req.length / hwep->ep.maxpacket;

		if (hwreq->req.length == 0
				|| hwreq->req.length % hwep->ep.maxpacket)
			mul++;
		node->ptr->token |= mul << __ffs(TD_MULTO);
	}

	temp = (u32) (hwreq->req.dma + hwreq->req.actual);
	if (length) {
		node->ptr->page[0] = cpu_to_le32(temp);
		for (i = 1; i < TD_PAGE_COUNT; i++) {
			u32 page = temp + i * CI_HDRC_PAGE_SIZE;
			page &= ~TD_RESERVED_MASK;
			node->ptr->page[i] = cpu_to_le32(page);
		}
	}

	hwreq->req.actual += length;

	if (!list_empty(&hwreq->tds)) {
		/* get the last entry */
		lastnode = list_entry(hwreq->tds.prev,
				struct td_node, td);
		lastnode->ptr->next = cpu_to_le32(node->dma);
	}

	INIT_LIST_HEAD(&node->td);
	list_add_tail(&node->td, &hwreq->tds);

	return 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * _usb_addr: calculates endpoint address from direction & number
 * @ep:  endpoint
 */
<<<<<<< HEAD
static inline u8 _usb_addr(struct ci13xxx_ep *ep)
=======
static inline u8 _usb_addr(struct ci_hw_ep *ep)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return ((ep->dir == TX) ? USB_ENDPOINT_DIR_MASK : 0) | ep->num;
}

/**
<<<<<<< HEAD
 * _hardware_queue: configures a request at hardware level
 * @gadget: gadget
 * @mEp:    endpoint
 *
 * This function returns an error code
 */
static int _hardware_enqueue(struct ci13xxx_ep *mEp, struct ci13xxx_req *mReq)
{
	struct ci13xxx *ci = mEp->ci;
	unsigned i;
	int ret = 0;
	unsigned length = mReq->req.length;

	/* don't queue twice */
	if (mReq->req.status == -EALREADY)
		return -EALREADY;

	mReq->req.status = -EALREADY;

	if (mReq->req.zero && length && (length % mEp->ep.maxpacket == 0)) {
		mReq->zptr = dma_pool_alloc(mEp->td_pool, GFP_ATOMIC,
					   &mReq->zdma);
		if (mReq->zptr == NULL)
			return -ENOMEM;

		memset(mReq->zptr, 0, sizeof(*mReq->zptr));
		mReq->zptr->next    = cpu_to_le32(TD_TERMINATE);
		mReq->zptr->token   = cpu_to_le32(TD_STATUS_ACTIVE);
		if (!mReq->req.no_interrupt)
			mReq->zptr->token   |= cpu_to_le32(TD_IOC);
	}
	ret = usb_gadget_map_request(&ci->gadget, &mReq->req, mEp->dir);
=======
 * _hardware_enqueue: configures a request at hardware level
 * @hwep:   endpoint
 * @hwreq:  request
 *
 * This function returns an error code
 */
static int _hardware_enqueue(struct ci_hw_ep *hwep, struct ci_hw_req *hwreq)
{
	struct ci_hdrc *ci = hwep->ci;
	int ret = 0;
	unsigned rest = hwreq->req.length;
	int pages = TD_PAGE_COUNT;
	struct td_node *firstnode, *lastnode;

	/* don't queue twice */
	if (hwreq->req.status == -EALREADY)
		return -EALREADY;

	hwreq->req.status = -EALREADY;

	ret = usb_gadget_map_request(&ci->gadget, &hwreq->req, hwep->dir);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret)
		return ret;

	/*
<<<<<<< HEAD
	 * TD configuration
	 * TODO - handle requests which spawns into several TDs
	 */
	memset(mReq->ptr, 0, sizeof(*mReq->ptr));
	mReq->ptr->token    = cpu_to_le32(length << __ffs(TD_TOTAL_BYTES));
	mReq->ptr->token   &= cpu_to_le32(TD_TOTAL_BYTES);
	mReq->ptr->token   |= cpu_to_le32(TD_STATUS_ACTIVE);
	if (mReq->zptr) {
		mReq->ptr->next    = cpu_to_le32(mReq->zdma);
	} else {
		mReq->ptr->next    = cpu_to_le32(TD_TERMINATE);
		if (!mReq->req.no_interrupt)
			mReq->ptr->token  |= cpu_to_le32(TD_IOC);
	}
	mReq->ptr->page[0]  = cpu_to_le32(mReq->req.dma);
	for (i = 1; i < TD_PAGE_COUNT; i++) {
		u32 page = mReq->req.dma + i * CI13XXX_PAGE_SIZE;
		page &= ~TD_RESERVED_MASK;
		mReq->ptr->page[i] = cpu_to_le32(page);
	}

	wmb();

	if (!list_empty(&mEp->qh.queue)) {
		struct ci13xxx_req *mReqPrev;
		int n = hw_ep_bit(mEp->num, mEp->dir);
		int tmp_stat;
		u32 next = mReq->dma & TD_ADDR_MASK;

		mReqPrev = list_entry(mEp->qh.queue.prev,
				struct ci13xxx_req, queue);
		if (mReqPrev->zptr)
			mReqPrev->zptr->next = cpu_to_le32(next);
		else
			mReqPrev->ptr->next = cpu_to_le32(next);
=======
	 * The first buffer could be not page aligned.
	 * In that case we have to span into one extra td.
	 */
	if (hwreq->req.dma % PAGE_SIZE)
		pages--;

	if (rest == 0) {
		ret = add_td_to_list(hwep, hwreq, 0);
		if (ret < 0)
			goto done;
	}

	while (rest > 0) {
		unsigned count = min(hwreq->req.length - hwreq->req.actual,
					(unsigned)(pages * CI_HDRC_PAGE_SIZE));
		ret = add_td_to_list(hwep, hwreq, count);
		if (ret < 0)
			goto done;

		rest -= count;
	}

	if (hwreq->req.zero && hwreq->req.length && hwep->dir == TX
	    && (hwreq->req.length % hwep->ep.maxpacket == 0)) {
		ret = add_td_to_list(hwep, hwreq, 0);
		if (ret < 0)
			goto done;
	}

	firstnode = list_first_entry(&hwreq->tds, struct td_node, td);

	lastnode = list_entry(hwreq->tds.prev,
		struct td_node, td);

	lastnode->ptr->next = cpu_to_le32(TD_TERMINATE);
	if (!hwreq->req.no_interrupt)
		lastnode->ptr->token |= cpu_to_le32(TD_IOC);
	wmb();

	hwreq->req.actual = 0;
	if (!list_empty(&hwep->qh.queue)) {
		struct ci_hw_req *hwreqprev;
		int n = hw_ep_bit(hwep->num, hwep->dir);
		int tmp_stat;
		struct td_node *prevlastnode;
		u32 next = firstnode->dma & TD_ADDR_MASK;

		hwreqprev = list_entry(hwep->qh.queue.prev,
				struct ci_hw_req, queue);
		prevlastnode = list_entry(hwreqprev->tds.prev,
				struct td_node, td);

		prevlastnode->ptr->next = cpu_to_le32(next);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		wmb();
		if (hw_read(ci, OP_ENDPTPRIME, BIT(n)))
			goto done;
		do {
			hw_write(ci, OP_USBCMD, USBCMD_ATDTW, USBCMD_ATDTW);
			tmp_stat = hw_read(ci, OP_ENDPTSTAT, BIT(n));
		} while (!hw_read(ci, OP_USBCMD, USBCMD_ATDTW));
		hw_write(ci, OP_USBCMD, USBCMD_ATDTW, 0);
		if (tmp_stat)
			goto done;
	}

	/*  QH configuration */
<<<<<<< HEAD
	mEp->qh.ptr->td.next   = cpu_to_le32(mReq->dma);    /* TERMINATE = 0 */
	mEp->qh.ptr->td.token &=
		cpu_to_le32(~(TD_STATUS_HALTED|TD_STATUS_ACTIVE));

	wmb();   /* synchronize before ep prime */

	ret = hw_ep_prime(ci, mEp->num, mEp->dir,
			   mEp->type == USB_ENDPOINT_XFER_CONTROL);
=======
	hwep->qh.ptr->td.next = cpu_to_le32(firstnode->dma);
	hwep->qh.ptr->td.token &=
		cpu_to_le32(~(TD_STATUS_HALTED|TD_STATUS_ACTIVE));

	if (hwep->type == USB_ENDPOINT_XFER_ISOC && hwep->dir == RX) {
		u32 mul = hwreq->req.length / hwep->ep.maxpacket;

		if (hwreq->req.length == 0
				|| hwreq->req.length % hwep->ep.maxpacket)
			mul++;
		hwep->qh.ptr->cap |= mul << __ffs(QH_MULT);
	}

	ret = hw_ep_prime(ci, hwep->num, hwep->dir,
			   hwep->type == USB_ENDPOINT_XFER_CONTROL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
done:
	return ret;
}

<<<<<<< HEAD
/**
 * _hardware_dequeue: handles a request at hardware level
 * @gadget: gadget
 * @mEp:    endpoint
 *
 * This function returns an error code
 */
static int _hardware_dequeue(struct ci13xxx_ep *mEp, struct ci13xxx_req *mReq)
{
	u32 tmptoken = le32_to_cpu(mReq->ptr->token);

	if (mReq->req.status != -EALREADY)
		return -EINVAL;

	if ((TD_STATUS_ACTIVE & tmptoken) != 0)
		return -EBUSY;

	if (mReq->zptr) {
		if ((cpu_to_le32(TD_STATUS_ACTIVE) & mReq->zptr->token) != 0)
			return -EBUSY;
		dma_pool_free(mEp->td_pool, mReq->zptr, mReq->zdma);
		mReq->zptr = NULL;
	}

	mReq->req.status = 0;

	usb_gadget_unmap_request(&mEp->ci->gadget, &mReq->req, mEp->dir);

	mReq->req.status = tmptoken & TD_STATUS;
	if ((TD_STATUS_HALTED & mReq->req.status) != 0)
		mReq->req.status = -1;
	else if ((TD_STATUS_DT_ERR & mReq->req.status) != 0)
		mReq->req.status = -1;
	else if ((TD_STATUS_TR_ERR & mReq->req.status) != 0)
		mReq->req.status = -1;

	mReq->req.actual   = tmptoken & TD_TOTAL_BYTES;
	mReq->req.actual >>= __ffs(TD_TOTAL_BYTES);
	mReq->req.actual   = mReq->req.length - mReq->req.actual;
	mReq->req.actual   = mReq->req.status ? 0 : mReq->req.actual;

	return mReq->req.actual;
=======
/*
 * free_pending_td: remove a pending request for the endpoint
 * @hwep: endpoint
 */
static void free_pending_td(struct ci_hw_ep *hwep)
{
	struct td_node *pending = hwep->pending_td;

	dma_pool_free(hwep->td_pool, pending->ptr, pending->dma);
	hwep->pending_td = NULL;
	kfree(pending);
}

static int reprime_dtd(struct ci_hdrc *ci, struct ci_hw_ep *hwep,
					   struct td_node *node)
{
	hwep->qh.ptr->td.next = node->dma;
	hwep->qh.ptr->td.token &=
		cpu_to_le32(~(TD_STATUS_HALTED | TD_STATUS_ACTIVE));

	return hw_ep_prime(ci, hwep->num, hwep->dir,
				hwep->type == USB_ENDPOINT_XFER_CONTROL);
}

/**
 * _hardware_dequeue: handles a request at hardware level
 * @gadget: gadget
 * @hwep:   endpoint
 *
 * This function returns an error code
 */
static int _hardware_dequeue(struct ci_hw_ep *hwep, struct ci_hw_req *hwreq)
{
	u32 tmptoken;
	struct td_node *node, *tmpnode;
	unsigned remaining_length;
	unsigned actual = hwreq->req.length;
	struct ci_hdrc *ci = hwep->ci;

	if (hwreq->req.status != -EALREADY)
		return -EINVAL;

	hwreq->req.status = 0;

	list_for_each_entry_safe(node, tmpnode, &hwreq->tds, td) {
		tmptoken = le32_to_cpu(node->ptr->token);
		if ((TD_STATUS_ACTIVE & tmptoken) != 0) {
			int n = hw_ep_bit(hwep->num, hwep->dir);

			if (ci->rev == CI_REVISION_24)
				if (!hw_read(ci, OP_ENDPTSTAT, BIT(n)))
					reprime_dtd(ci, hwep, node);
			hwreq->req.status = -EALREADY;
			return -EBUSY;
		}

		remaining_length = (tmptoken & TD_TOTAL_BYTES);
		remaining_length >>= __ffs(TD_TOTAL_BYTES);
		actual -= remaining_length;

		hwreq->req.status = tmptoken & TD_STATUS;
		if ((TD_STATUS_HALTED & hwreq->req.status)) {
			hwreq->req.status = -EPIPE;
			break;
		} else if ((TD_STATUS_DT_ERR & hwreq->req.status)) {
			hwreq->req.status = -EPROTO;
			break;
		} else if ((TD_STATUS_TR_ERR & hwreq->req.status)) {
			hwreq->req.status = -EILSEQ;
			break;
		}

		if (remaining_length) {
			if (hwep->dir == TX) {
				hwreq->req.status = -EPROTO;
				break;
			}
		}
		/*
		 * As the hardware could still address the freed td
		 * which will run the udc unusable, the cleanup of the
		 * td has to be delayed by one.
		 */
		if (hwep->pending_td)
			free_pending_td(hwep);

		hwep->pending_td = node;
		list_del_init(&node->td);
	}

	usb_gadget_unmap_request(&hwep->ci->gadget, &hwreq->req, hwep->dir);

	hwreq->req.actual += actual;

	if (hwreq->req.status)
		return hwreq->req.status;

	return hwreq->req.actual;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * _ep_nuke: dequeues all endpoint requests
<<<<<<< HEAD
 * @mEp: endpoint
=======
 * @hwep: endpoint
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This function returns an error code
 * Caller must hold lock
 */
<<<<<<< HEAD
static int _ep_nuke(struct ci13xxx_ep *mEp)
__releases(mEp->lock)
__acquires(mEp->lock)
{
	if (mEp == NULL)
		return -EINVAL;

	hw_ep_flush(mEp->ci, mEp->num, mEp->dir);

	while (!list_empty(&mEp->qh.queue)) {

		/* pop oldest request */
		struct ci13xxx_req *mReq = \
			list_entry(mEp->qh.queue.next,
				   struct ci13xxx_req, queue);

		if (mReq->zptr) {
			dma_pool_free(mEp->td_pool, mReq->zptr, mReq->zdma);
			mReq->zptr = NULL;
		}

		list_del_init(&mReq->queue);
		mReq->req.status = -ESHUTDOWN;

		if (mReq->req.complete != NULL) {
			spin_unlock(mEp->lock);
			mReq->req.complete(&mEp->ep, &mReq->req);
			spin_lock(mEp->lock);
		}
	}
	return 0;
}

=======
static int _ep_nuke(struct ci_hw_ep *hwep)
__releases(hwep->lock)
__acquires(hwep->lock)
{
	struct td_node *node, *tmpnode;
	if (hwep == NULL)
		return -EINVAL;

	hw_ep_flush(hwep->ci, hwep->num, hwep->dir);

	while (!list_empty(&hwep->qh.queue)) {

		/* pop oldest request */
		struct ci_hw_req *hwreq = list_entry(hwep->qh.queue.next,
						     struct ci_hw_req, queue);

		list_for_each_entry_safe(node, tmpnode, &hwreq->tds, td) {
			dma_pool_free(hwep->td_pool, node->ptr, node->dma);
			list_del_init(&node->td);
			node->ptr = NULL;
			kfree(node);
		}

		list_del_init(&hwreq->queue);
		hwreq->req.status = -ESHUTDOWN;

		if (hwreq->req.complete != NULL) {
			spin_unlock(hwep->lock);
			usb_gadget_giveback_request(&hwep->ep, &hwreq->req);
			spin_lock(hwep->lock);
		}
	}

	if (hwep->pending_td)
		free_pending_td(hwep);

	return 0;
}

static int _ep_set_halt(struct usb_ep *ep, int value, bool check_transfer)
{
	struct ci_hw_ep *hwep = container_of(ep, struct ci_hw_ep, ep);
	int direction, retval = 0;
	unsigned long flags;

	if (ep == NULL || hwep->ep.desc == NULL)
		return -EINVAL;

	if (usb_endpoint_xfer_isoc(hwep->ep.desc))
		return -EOPNOTSUPP;

	spin_lock_irqsave(hwep->lock, flags);

	if (value && hwep->dir == TX && check_transfer &&
		!list_empty(&hwep->qh.queue) &&
			!usb_endpoint_xfer_control(hwep->ep.desc)) {
		spin_unlock_irqrestore(hwep->lock, flags);
		return -EAGAIN;
	}

	direction = hwep->dir;
	do {
		retval |= hw_ep_set_halt(hwep->ci, hwep->num, hwep->dir, value);

		if (!value)
			hwep->wedge = 0;

		if (hwep->type == USB_ENDPOINT_XFER_CONTROL)
			hwep->dir = (hwep->dir == TX) ? RX : TX;

	} while (hwep->dir != direction);

	spin_unlock_irqrestore(hwep->lock, flags);
	return retval;
}


>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * _gadget_stop_activity: stops all USB activity, flushes & disables all endpts
 * @gadget: gadget
 *
 * This function returns an error code
 */
static int _gadget_stop_activity(struct usb_gadget *gadget)
{
	struct usb_ep *ep;
<<<<<<< HEAD
	struct ci13xxx    *ci = container_of(gadget, struct ci13xxx, gadget);
	unsigned long flags;

	spin_lock_irqsave(&ci->lock, flags);
	ci->gadget.speed = USB_SPEED_UNKNOWN;
	ci->remote_wakeup = 0;
	ci->suspended = 0;
	spin_unlock_irqrestore(&ci->lock, flags);

=======
	struct ci_hdrc    *ci = container_of(gadget, struct ci_hdrc, gadget);
	unsigned long flags;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* flush all endpoints */
	gadget_for_each_ep(ep, gadget) {
		usb_ep_fifo_flush(ep);
	}
	usb_ep_fifo_flush(&ci->ep0out->ep);
	usb_ep_fifo_flush(&ci->ep0in->ep);

<<<<<<< HEAD
	if (ci->driver)
		ci->driver->disconnect(gadget);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* make sure to disable all endpoints */
	gadget_for_each_ep(ep, gadget) {
		usb_ep_disable(ep);
	}

	if (ci->status != NULL) {
		usb_ep_free_request(&ci->ep0in->ep, ci->status);
		ci->status = NULL;
	}

<<<<<<< HEAD
=======
	spin_lock_irqsave(&ci->lock, flags);
	ci->gadget.speed = USB_SPEED_UNKNOWN;
	ci->remote_wakeup = 0;
	ci->suspended = 0;
	spin_unlock_irqrestore(&ci->lock, flags);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

/******************************************************************************
 * ISR block
 *****************************************************************************/
/**
 * isr_reset_handler: USB reset interrupt handler
 * @ci: UDC device
 *
 * This function resets USB engine after a bus reset occurred
 */
<<<<<<< HEAD
static void isr_reset_handler(struct ci13xxx *ci)
=======
static void isr_reset_handler(struct ci_hdrc *ci)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
__releases(ci->lock)
__acquires(ci->lock)
{
	int retval;

	spin_unlock(&ci->lock);
<<<<<<< HEAD
=======
	if (ci->gadget.speed != USB_SPEED_UNKNOWN)
		usb_gadget_udc_reset(&ci->gadget, ci->driver);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	retval = _gadget_stop_activity(&ci->gadget);
	if (retval)
		goto done;

	retval = hw_usb_reset(ci);
	if (retval)
		goto done;

	ci->status = usb_ep_alloc_request(&ci->ep0in->ep, GFP_ATOMIC);
	if (ci->status == NULL)
		retval = -ENOMEM;

done:
	spin_lock(&ci->lock);

	if (retval)
		dev_err(ci->dev, "error: %i\n", retval);
}

/**
 * isr_get_status_complete: get_status request complete function
 * @ep:  endpoint
 * @req: request handled
 *
 * Caller must release lock
 */
static void isr_get_status_complete(struct usb_ep *ep, struct usb_request *req)
{
	if (ep == NULL || req == NULL)
		return;

	kfree(req->buf);
	usb_ep_free_request(ep, req);
}

/**
 * _ep_queue: queues (submits) an I/O request to an endpoint
<<<<<<< HEAD
 *
 * Caller must hold lock
=======
 * @ep:        endpoint
 * @req:       request
 * @gfp_flags: GFP flags (not used)
 *
 * Caller must hold lock
 * This function returns an error code
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static int _ep_queue(struct usb_ep *ep, struct usb_request *req,
		    gfp_t __maybe_unused gfp_flags)
{
<<<<<<< HEAD
	struct ci13xxx_ep  *mEp  = container_of(ep,  struct ci13xxx_ep, ep);
	struct ci13xxx_req *mReq = container_of(req, struct ci13xxx_req, req);
	struct ci13xxx *ci = mEp->ci;
	int retval = 0;

	if (ep == NULL || req == NULL || mEp->ep.desc == NULL)
		return -EINVAL;

	if (mEp->type == USB_ENDPOINT_XFER_CONTROL) {
		if (req->length)
			mEp = (ci->ep0_dir == RX) ?
			       ci->ep0out : ci->ep0in;
		if (!list_empty(&mEp->qh.queue)) {
			_ep_nuke(mEp);
			retval = -EOVERFLOW;
			dev_warn(mEp->ci->dev, "endpoint ctrl %X nuked\n",
				 _usb_addr(mEp));
		}
	}

	/* first nuke then test link, e.g. previous status has not sent */
	if (!list_empty(&mReq->queue)) {
		dev_err(mEp->ci->dev, "request already in queue\n");
		return -EBUSY;
	}

	if (req->length > (TD_PAGE_COUNT - 1) * CI13XXX_PAGE_SIZE) {
		dev_err(mEp->ci->dev, "request bigger than one td\n");
		return -EMSGSIZE;
	}

	/* push request */
	mReq->req.status = -EINPROGRESS;
	mReq->req.actual = 0;

	retval = _hardware_enqueue(mEp, mReq);
=======
	struct ci_hw_ep  *hwep  = container_of(ep,  struct ci_hw_ep, ep);
	struct ci_hw_req *hwreq = container_of(req, struct ci_hw_req, req);
	struct ci_hdrc *ci = hwep->ci;
	int retval = 0;

	if (ep == NULL || req == NULL || hwep->ep.desc == NULL)
		return -EINVAL;

	if (hwep->type == USB_ENDPOINT_XFER_CONTROL) {
		if (req->length)
			hwep = (ci->ep0_dir == RX) ?
			       ci->ep0out : ci->ep0in;
		if (!list_empty(&hwep->qh.queue)) {
			_ep_nuke(hwep);
			dev_warn(hwep->ci->dev, "endpoint ctrl %X nuked\n",
				 _usb_addr(hwep));
		}
	}

	if (usb_endpoint_xfer_isoc(hwep->ep.desc) &&
	    hwreq->req.length > (1 + hwep->ep.mult) * hwep->ep.maxpacket) {
		dev_err(hwep->ci->dev, "request length too big for isochronous\n");
		return -EMSGSIZE;
	}

	/* first nuke then test link, e.g. previous status has not sent */
	if (!list_empty(&hwreq->queue)) {
		dev_err(hwep->ci->dev, "request already in queue\n");
		return -EBUSY;
	}

	/* push request */
	hwreq->req.status = -EINPROGRESS;
	hwreq->req.actual = 0;

	retval = _hardware_enqueue(hwep, hwreq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (retval == -EALREADY)
		retval = 0;
	if (!retval)
<<<<<<< HEAD
		list_add_tail(&mReq->queue, &mEp->qh.queue);
=======
		list_add_tail(&hwreq->queue, &hwep->qh.queue);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return retval;
}

/**
 * isr_get_status_response: get_status request response
 * @ci: ci struct
 * @setup: setup request packet
 *
 * This function returns an error code
 */
<<<<<<< HEAD
static int isr_get_status_response(struct ci13xxx *ci,
				   struct usb_ctrlrequest *setup)
__releases(mEp->lock)
__acquires(mEp->lock)
{
	struct ci13xxx_ep *mEp = ci->ep0in;
=======
static int isr_get_status_response(struct ci_hdrc *ci,
				   struct usb_ctrlrequest *setup)
__releases(hwep->lock)
__acquires(hwep->lock)
{
	struct ci_hw_ep *hwep = ci->ep0in;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct usb_request *req = NULL;
	gfp_t gfp_flags = GFP_ATOMIC;
	int dir, num, retval;

<<<<<<< HEAD
	if (mEp == NULL || setup == NULL)
		return -EINVAL;

	spin_unlock(mEp->lock);
	req = usb_ep_alloc_request(&mEp->ep, gfp_flags);
	spin_lock(mEp->lock);
=======
	if (hwep == NULL || setup == NULL)
		return -EINVAL;

	spin_unlock(hwep->lock);
	req = usb_ep_alloc_request(&hwep->ep, gfp_flags);
	spin_lock(hwep->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (req == NULL)
		return -ENOMEM;

	req->complete = isr_get_status_complete;
	req->length   = 2;
	req->buf      = kzalloc(req->length, gfp_flags);
	if (req->buf == NULL) {
		retval = -ENOMEM;
		goto err_free_req;
	}

	if ((setup->bRequestType & USB_RECIP_MASK) == USB_RECIP_DEVICE) {
<<<<<<< HEAD
		/* Assume that device is bus powered for now. */
		*(u16 *)req->buf = ci->remote_wakeup << 1;
		retval = 0;
=======
		*(u16 *)req->buf = (ci->remote_wakeup << 1) |
			ci->gadget.is_selfpowered;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else if ((setup->bRequestType & USB_RECIP_MASK) \
		   == USB_RECIP_ENDPOINT) {
		dir = (le16_to_cpu(setup->wIndex) & USB_ENDPOINT_DIR_MASK) ?
			TX : RX;
		num =  le16_to_cpu(setup->wIndex) & USB_ENDPOINT_NUMBER_MASK;
		*(u16 *)req->buf = hw_ep_get_halt(ci, num, dir);
	}
	/* else do nothing; reserved for future use */

<<<<<<< HEAD
	retval = _ep_queue(&mEp->ep, req, gfp_flags);
=======
	retval = _ep_queue(&hwep->ep, req, gfp_flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (retval)
		goto err_free_buf;

	return 0;

 err_free_buf:
	kfree(req->buf);
 err_free_req:
<<<<<<< HEAD
	spin_unlock(mEp->lock);
	usb_ep_free_request(&mEp->ep, req);
	spin_lock(mEp->lock);
=======
	spin_unlock(hwep->lock);
	usb_ep_free_request(&hwep->ep, req);
	spin_lock(hwep->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return retval;
}

/**
 * isr_setup_status_complete: setup_status request complete function
 * @ep:  endpoint
 * @req: request handled
 *
 * Caller must release lock. Put the port in test mode if test mode
 * feature is selected.
 */
static void
isr_setup_status_complete(struct usb_ep *ep, struct usb_request *req)
{
<<<<<<< HEAD
	struct ci13xxx *ci = req->context;
=======
	struct ci_hdrc *ci = req->context;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long flags;

	if (ci->setaddr) {
		hw_usb_set_address(ci, ci->address);
		ci->setaddr = false;
<<<<<<< HEAD
=======
		if (ci->address)
			usb_gadget_set_state(&ci->gadget, USB_STATE_ADDRESS);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	spin_lock_irqsave(&ci->lock, flags);
	if (ci->test_mode)
		hw_port_test_set(ci, ci->test_mode);
	spin_unlock_irqrestore(&ci->lock, flags);
}

/**
 * isr_setup_status_phase: queues the status phase of a setup transation
 * @ci: ci struct
 *
 * This function returns an error code
 */
<<<<<<< HEAD
static int isr_setup_status_phase(struct ci13xxx *ci)
{
	int retval;
	struct ci13xxx_ep *mEp;

	mEp = (ci->ep0_dir == TX) ? ci->ep0out : ci->ep0in;
	ci->status->context = ci;
	ci->status->complete = isr_setup_status_complete;

	retval = _ep_queue(&mEp->ep, ci->status, GFP_ATOMIC);
=======
static int isr_setup_status_phase(struct ci_hdrc *ci)
{
	int retval;
	struct ci_hw_ep *hwep;

	/*
	 * Unexpected USB controller behavior, caused by bad signal integrity
	 * or ground reference problems, can lead to isr_setup_status_phase
	 * being called with ci->status equal to NULL.
	 * If this situation occurs, you should review your USB hardware design.
	 */
	if (WARN_ON_ONCE(!ci->status))
		return -EPIPE;

	hwep = (ci->ep0_dir == TX) ? ci->ep0out : ci->ep0in;
	ci->status->context = ci;
	ci->status->complete = isr_setup_status_complete;

	retval = _ep_queue(&hwep->ep, ci->status, GFP_ATOMIC);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return retval;
}

/**
 * isr_tr_complete_low: transaction complete low level handler
<<<<<<< HEAD
 * @mEp: endpoint
=======
 * @hwep: endpoint
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This function returns an error code
 * Caller must hold lock
 */
<<<<<<< HEAD
static int isr_tr_complete_low(struct ci13xxx_ep *mEp)
__releases(mEp->lock)
__acquires(mEp->lock)
{
	struct ci13xxx_req *mReq, *mReqTemp;
	struct ci13xxx_ep *mEpTemp = mEp;
	int retval = 0;

	list_for_each_entry_safe(mReq, mReqTemp, &mEp->qh.queue,
			queue) {
		retval = _hardware_dequeue(mEp, mReq);
		if (retval < 0)
			break;
		list_del_init(&mReq->queue);
		if (mReq->req.complete != NULL) {
			spin_unlock(mEp->lock);
			if ((mEp->type == USB_ENDPOINT_XFER_CONTROL) &&
					mReq->req.length)
				mEpTemp = mEp->ci->ep0in;
			mReq->req.complete(&mEpTemp->ep, &mReq->req);
			spin_lock(mEp->lock);
=======
static int isr_tr_complete_low(struct ci_hw_ep *hwep)
__releases(hwep->lock)
__acquires(hwep->lock)
{
	struct ci_hw_req *hwreq, *hwreqtemp;
	struct ci_hw_ep *hweptemp = hwep;
	int retval = 0;

	list_for_each_entry_safe(hwreq, hwreqtemp, &hwep->qh.queue,
			queue) {
		retval = _hardware_dequeue(hwep, hwreq);
		if (retval < 0)
			break;
		list_del_init(&hwreq->queue);
		if (hwreq->req.complete != NULL) {
			spin_unlock(hwep->lock);
			if ((hwep->type == USB_ENDPOINT_XFER_CONTROL) &&
					hwreq->req.length)
				hweptemp = hwep->ci->ep0in;
			usb_gadget_giveback_request(&hweptemp->ep, &hwreq->req);
			spin_lock(hwep->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	if (retval == -EBUSY)
		retval = 0;

	return retval;
}

<<<<<<< HEAD
/**
 * isr_tr_complete_handler: transaction complete interrupt handler
 * @ci: UDC descriptor
 *
 * This function handles traffic events
 */
static void isr_tr_complete_handler(struct ci13xxx *ci)
__releases(ci->lock)
__acquires(ci->lock)
{
	unsigned i;
	u8 tmode = 0;

	for (i = 0; i < ci->hw_ep_max; i++) {
		struct ci13xxx_ep *mEp  = &ci->ci13xxx_ep[i];
		int type, num, dir, err = -EINVAL;
		struct usb_ctrlrequest req;

		if (mEp->ep.desc == NULL)
			continue;   /* not configured */

		if (hw_test_and_clear_complete(ci, i)) {
			err = isr_tr_complete_low(mEp);
			if (mEp->type == USB_ENDPOINT_XFER_CONTROL) {
				if (err > 0)   /* needs status phase */
					err = isr_setup_status_phase(ci);
				if (err < 0) {
					spin_unlock(&ci->lock);
					if (usb_ep_set_halt(&mEp->ep))
						dev_err(ci->dev,
							"error: ep_set_halt\n");
					spin_lock(&ci->lock);
				}
			}
		}

		if (mEp->type != USB_ENDPOINT_XFER_CONTROL ||
		    !hw_test_and_clear_setup_status(ci, i))
			continue;

		if (i != 0) {
			dev_warn(ci->dev, "ctrl traffic at endpoint %d\n", i);
			continue;
		}

		/*
		 * Flush data and handshake transactions of previous
		 * setup packet.
		 */
		_ep_nuke(ci->ep0out);
		_ep_nuke(ci->ep0in);

		/* read_setup_packet */
		do {
			hw_test_and_set_setup_guard(ci);
			memcpy(&req, &mEp->qh.ptr->setup, sizeof(req));
		} while (!hw_test_and_clear_setup_guard(ci));

		type = req.bRequestType;

		ci->ep0_dir = (type & USB_DIR_IN) ? TX : RX;

		switch (req.bRequest) {
		case USB_REQ_CLEAR_FEATURE:
			if (type == (USB_DIR_OUT|USB_RECIP_ENDPOINT) &&
					le16_to_cpu(req.wValue) ==
					USB_ENDPOINT_HALT) {
				if (req.wLength != 0)
					break;
				num  = le16_to_cpu(req.wIndex);
				dir = num & USB_ENDPOINT_DIR_MASK;
				num &= USB_ENDPOINT_NUMBER_MASK;
				if (dir) /* TX */
					num += ci->hw_ep_max/2;
				if (!ci->ci13xxx_ep[num].wedge) {
					spin_unlock(&ci->lock);
					err = usb_ep_clear_halt(
						&ci->ci13xxx_ep[num].ep);
					spin_lock(&ci->lock);
					if (err)
						break;
				}
				err = isr_setup_status_phase(ci);
			} else if (type == (USB_DIR_OUT|USB_RECIP_DEVICE) &&
					le16_to_cpu(req.wValue) ==
					USB_DEVICE_REMOTE_WAKEUP) {
				if (req.wLength != 0)
					break;
				ci->remote_wakeup = 0;
				err = isr_setup_status_phase(ci);
			} else {
				goto delegate;
			}
			break;
		case USB_REQ_GET_STATUS:
			if (type != (USB_DIR_IN|USB_RECIP_DEVICE)   &&
			    type != (USB_DIR_IN|USB_RECIP_ENDPOINT) &&
			    type != (USB_DIR_IN|USB_RECIP_INTERFACE))
				goto delegate;
			if (le16_to_cpu(req.wLength) != 2 ||
			    le16_to_cpu(req.wValue)  != 0)
				break;
			err = isr_get_status_response(ci, &req);
			break;
		case USB_REQ_SET_ADDRESS:
			if (type != (USB_DIR_OUT|USB_RECIP_DEVICE))
				goto delegate;
			if (le16_to_cpu(req.wLength) != 0 ||
			    le16_to_cpu(req.wIndex)  != 0)
				break;
			ci->address = (u8)le16_to_cpu(req.wValue);
			ci->setaddr = true;
			err = isr_setup_status_phase(ci);
			break;
		case USB_REQ_SET_FEATURE:
			if (type == (USB_DIR_OUT|USB_RECIP_ENDPOINT) &&
					le16_to_cpu(req.wValue) ==
					USB_ENDPOINT_HALT) {
				if (req.wLength != 0)
					break;
				num  = le16_to_cpu(req.wIndex);
				dir = num & USB_ENDPOINT_DIR_MASK;
				num &= USB_ENDPOINT_NUMBER_MASK;
				if (dir) /* TX */
					num += ci->hw_ep_max/2;

				spin_unlock(&ci->lock);
				err = usb_ep_set_halt(&ci->ci13xxx_ep[num].ep);
				spin_lock(&ci->lock);
				if (!err)
					isr_setup_status_phase(ci);
			} else if (type == (USB_DIR_OUT|USB_RECIP_DEVICE)) {
				if (req.wLength != 0)
					break;
				switch (le16_to_cpu(req.wValue)) {
				case USB_DEVICE_REMOTE_WAKEUP:
					ci->remote_wakeup = 1;
					err = isr_setup_status_phase(ci);
					break;
				case USB_DEVICE_TEST_MODE:
					tmode = le16_to_cpu(req.wIndex) >> 8;
					switch (tmode) {
					case TEST_J:
					case TEST_K:
					case TEST_SE0_NAK:
					case TEST_PACKET:
					case TEST_FORCE_EN:
						ci->test_mode = tmode;
						err = isr_setup_status_phase(
								ci);
						break;
					default:
						break;
					}
				default:
					goto delegate;
				}
			} else {
				goto delegate;
			}
			break;
		default:
delegate:
			if (req.wLength == 0)   /* no data phase */
				ci->ep0_dir = TX;

			spin_unlock(&ci->lock);
			err = ci->driver->setup(&ci->gadget, &req);
			spin_lock(&ci->lock);
			break;
		}

		if (err < 0) {
			spin_unlock(&ci->lock);
			if (usb_ep_set_halt(&mEp->ep))
				dev_err(ci->dev, "error: ep_set_halt\n");
			spin_lock(&ci->lock);
		}
=======
static int otg_a_alt_hnp_support(struct ci_hdrc *ci)
{
	dev_warn(&ci->gadget.dev,
		"connect the device to an alternate port if you want HNP\n");
	return isr_setup_status_phase(ci);
}

/**
 * isr_setup_packet_handler: setup packet handler
 * @ci: UDC descriptor
 *
 * This function handles setup packet 
 */
static void isr_setup_packet_handler(struct ci_hdrc *ci)
__releases(ci->lock)
__acquires(ci->lock)
{
	struct ci_hw_ep *hwep = &ci->ci_hw_ep[0];
	struct usb_ctrlrequest req;
	int type, num, dir, err = -EINVAL;
	u8 tmode = 0;

	/*
	 * Flush data and handshake transactions of previous
	 * setup packet.
	 */
	_ep_nuke(ci->ep0out);
	_ep_nuke(ci->ep0in);

	/* read_setup_packet */
	do {
		hw_test_and_set_setup_guard(ci);
		memcpy(&req, &hwep->qh.ptr->setup, sizeof(req));
	} while (!hw_test_and_clear_setup_guard(ci));

	type = req.bRequestType;

	ci->ep0_dir = (type & USB_DIR_IN) ? TX : RX;

	switch (req.bRequest) {
	case USB_REQ_CLEAR_FEATURE:
		if (type == (USB_DIR_OUT|USB_RECIP_ENDPOINT) &&
				le16_to_cpu(req.wValue) ==
				USB_ENDPOINT_HALT) {
			if (req.wLength != 0)
				break;
			num  = le16_to_cpu(req.wIndex);
			dir = (num & USB_ENDPOINT_DIR_MASK) ? TX : RX;
			num &= USB_ENDPOINT_NUMBER_MASK;
			if (dir == TX)
				num += ci->hw_ep_max / 2;
			if (!ci->ci_hw_ep[num].wedge) {
				spin_unlock(&ci->lock);
				err = usb_ep_clear_halt(
					&ci->ci_hw_ep[num].ep);
				spin_lock(&ci->lock);
				if (err)
					break;
			}
			err = isr_setup_status_phase(ci);
		} else if (type == (USB_DIR_OUT|USB_RECIP_DEVICE) &&
				le16_to_cpu(req.wValue) ==
				USB_DEVICE_REMOTE_WAKEUP) {
			if (req.wLength != 0)
				break;
			ci->remote_wakeup = 0;
			err = isr_setup_status_phase(ci);
		} else {
			goto delegate;
		}
		break;
	case USB_REQ_GET_STATUS:
		if ((type != (USB_DIR_IN|USB_RECIP_DEVICE) ||
			le16_to_cpu(req.wIndex) == OTG_STS_SELECTOR) &&
		    type != (USB_DIR_IN|USB_RECIP_ENDPOINT) &&
		    type != (USB_DIR_IN|USB_RECIP_INTERFACE))
			goto delegate;
		if (le16_to_cpu(req.wLength) != 2 ||
		    le16_to_cpu(req.wValue)  != 0)
			break;
		err = isr_get_status_response(ci, &req);
		break;
	case USB_REQ_SET_ADDRESS:
		if (type != (USB_DIR_OUT|USB_RECIP_DEVICE))
			goto delegate;
		if (le16_to_cpu(req.wLength) != 0 ||
		    le16_to_cpu(req.wIndex)  != 0)
			break;
		ci->address = (u8)le16_to_cpu(req.wValue);
		ci->setaddr = true;
		err = isr_setup_status_phase(ci);
		break;
	case USB_REQ_SET_FEATURE:
		if (type == (USB_DIR_OUT|USB_RECIP_ENDPOINT) &&
				le16_to_cpu(req.wValue) ==
				USB_ENDPOINT_HALT) {
			if (req.wLength != 0)
				break;
			num  = le16_to_cpu(req.wIndex);
			dir = (num & USB_ENDPOINT_DIR_MASK) ? TX : RX;
			num &= USB_ENDPOINT_NUMBER_MASK;
			if (dir == TX)
				num += ci->hw_ep_max / 2;

			spin_unlock(&ci->lock);
			err = _ep_set_halt(&ci->ci_hw_ep[num].ep, 1, false);
			spin_lock(&ci->lock);
			if (!err)
				isr_setup_status_phase(ci);
		} else if (type == (USB_DIR_OUT|USB_RECIP_DEVICE)) {
			if (req.wLength != 0)
				break;
			switch (le16_to_cpu(req.wValue)) {
			case USB_DEVICE_REMOTE_WAKEUP:
				ci->remote_wakeup = 1;
				err = isr_setup_status_phase(ci);
				break;
			case USB_DEVICE_TEST_MODE:
				tmode = le16_to_cpu(req.wIndex) >> 8;
				switch (tmode) {
				case TEST_J:
				case TEST_K:
				case TEST_SE0_NAK:
				case TEST_PACKET:
				case TEST_FORCE_EN:
					ci->test_mode = tmode;
					err = isr_setup_status_phase(
							ci);
					break;
				default:
					break;
				}
				break;
			case USB_DEVICE_B_HNP_ENABLE:
				if (ci_otg_is_fsm_mode(ci)) {
					ci->gadget.b_hnp_enable = 1;
					err = isr_setup_status_phase(
							ci);
				}
				break;
			case USB_DEVICE_A_ALT_HNP_SUPPORT:
				if (ci_otg_is_fsm_mode(ci))
					err = otg_a_alt_hnp_support(ci);
				break;
			case USB_DEVICE_A_HNP_SUPPORT:
				if (ci_otg_is_fsm_mode(ci)) {
					ci->gadget.a_hnp_support = 1;
					err = isr_setup_status_phase(
							ci);
				}
				break;
			default:
				goto delegate;
			}
		} else {
			goto delegate;
		}
		break;
	default:
delegate:
		if (req.wLength == 0)   /* no data phase */
			ci->ep0_dir = TX;

		spin_unlock(&ci->lock);
		err = ci->driver->setup(&ci->gadget, &req);
		spin_lock(&ci->lock);
		break;
	}

	if (err < 0) {
		spin_unlock(&ci->lock);
		if (_ep_set_halt(&hwep->ep, 1, false))
			dev_err(ci->dev, "error: _ep_set_halt\n");
		spin_lock(&ci->lock);
	}
}

/**
 * isr_tr_complete_handler: transaction complete interrupt handler
 * @ci: UDC descriptor
 *
 * This function handles traffic events
 */
static void isr_tr_complete_handler(struct ci_hdrc *ci)
__releases(ci->lock)
__acquires(ci->lock)
{
	unsigned i;
	int err;

	for (i = 0; i < ci->hw_ep_max; i++) {
		struct ci_hw_ep *hwep  = &ci->ci_hw_ep[i];

		if (hwep->ep.desc == NULL)
			continue;   /* not configured */

		if (hw_test_and_clear_complete(ci, i)) {
			err = isr_tr_complete_low(hwep);
			if (hwep->type == USB_ENDPOINT_XFER_CONTROL) {
				if (err > 0)   /* needs status phase */
					err = isr_setup_status_phase(ci);
				if (err < 0) {
					spin_unlock(&ci->lock);
					if (_ep_set_halt(&hwep->ep, 1, false))
						dev_err(ci->dev,
						"error: _ep_set_halt\n");
					spin_lock(&ci->lock);
				}
			}
		}

		/* Only handle setup packet below */
		if (i == 0 &&
			hw_test_and_clear(ci, OP_ENDPTSETUPSTAT, BIT(0)))
			isr_setup_packet_handler(ci);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

/******************************************************************************
 * ENDPT block
 *****************************************************************************/
/**
 * ep_enable: configure endpoint, making it usable
 *
 * Check usb_ep_enable() at "usb_gadget.h" for details
 */
static int ep_enable(struct usb_ep *ep,
		     const struct usb_endpoint_descriptor *desc)
{
<<<<<<< HEAD
	struct ci13xxx_ep *mEp = container_of(ep, struct ci13xxx_ep, ep);
=======
	struct ci_hw_ep *hwep = container_of(ep, struct ci_hw_ep, ep);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int retval = 0;
	unsigned long flags;
	u32 cap = 0;

	if (ep == NULL || desc == NULL)
		return -EINVAL;

<<<<<<< HEAD
	spin_lock_irqsave(mEp->lock, flags);

	/* only internal SW should enable ctrl endpts */

	mEp->ep.desc = desc;

	if (!list_empty(&mEp->qh.queue))
		dev_warn(mEp->ci->dev, "enabling a non-empty endpoint!\n");

	mEp->dir  = usb_endpoint_dir_in(desc) ? TX : RX;
	mEp->num  = usb_endpoint_num(desc);
	mEp->type = usb_endpoint_type(desc);

	mEp->ep.maxpacket = usb_endpoint_maxp(desc);

	if (mEp->type == USB_ENDPOINT_XFER_CONTROL)
		cap |= QH_IOS;
	if (mEp->num)
		cap |= QH_ZLT;
	cap |= (mEp->ep.maxpacket << __ffs(QH_MAX_PKT)) & QH_MAX_PKT;

	mEp->qh.ptr->cap = cpu_to_le32(cap);

	mEp->qh.ptr->td.next |= cpu_to_le32(TD_TERMINATE);   /* needed? */
=======
	spin_lock_irqsave(hwep->lock, flags);

	/* only internal SW should enable ctrl endpts */

	if (!list_empty(&hwep->qh.queue)) {
		dev_warn(hwep->ci->dev, "enabling a non-empty endpoint!\n");
		spin_unlock_irqrestore(hwep->lock, flags);
		return -EBUSY;
	}

	hwep->ep.desc = desc;

	hwep->dir  = usb_endpoint_dir_in(desc) ? TX : RX;
	hwep->num  = usb_endpoint_num(desc);
	hwep->type = usb_endpoint_type(desc);

	hwep->ep.maxpacket = usb_endpoint_maxp(desc) & 0x07ff;
	hwep->ep.mult = QH_ISO_MULT(usb_endpoint_maxp(desc));

	if (hwep->type == USB_ENDPOINT_XFER_CONTROL)
		cap |= QH_IOS;

	cap |= QH_ZLT;
	cap |= (hwep->ep.maxpacket << __ffs(QH_MAX_PKT)) & QH_MAX_PKT;
	/*
	 * For ISO-TX, we set mult at QH as the largest value, and use
	 * MultO at TD as real mult value.
	 */
	if (hwep->type == USB_ENDPOINT_XFER_ISOC && hwep->dir == TX)
		cap |= 3 << __ffs(QH_MULT);

	hwep->qh.ptr->cap = cpu_to_le32(cap);

	hwep->qh.ptr->td.next |= cpu_to_le32(TD_TERMINATE);   /* needed? */

	if (hwep->num != 0 && hwep->type == USB_ENDPOINT_XFER_CONTROL) {
		dev_err(hwep->ci->dev, "Set control xfer at non-ep0\n");
		retval = -EINVAL;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Enable endpoints in the HW other than ep0 as ep0
	 * is always enabled
	 */
<<<<<<< HEAD
	if (mEp->num)
		retval |= hw_ep_enable(mEp->ci, mEp->num, mEp->dir, mEp->type);

	spin_unlock_irqrestore(mEp->lock, flags);
=======
	if (hwep->num)
		retval |= hw_ep_enable(hwep->ci, hwep->num, hwep->dir,
				       hwep->type);

	spin_unlock_irqrestore(hwep->lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return retval;
}

/**
 * ep_disable: endpoint is no longer usable
 *
 * Check usb_ep_disable() at "usb_gadget.h" for details
 */
static int ep_disable(struct usb_ep *ep)
{
<<<<<<< HEAD
	struct ci13xxx_ep *mEp = container_of(ep, struct ci13xxx_ep, ep);
=======
	struct ci_hw_ep *hwep = container_of(ep, struct ci_hw_ep, ep);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int direction, retval = 0;
	unsigned long flags;

	if (ep == NULL)
		return -EINVAL;
<<<<<<< HEAD
	else if (mEp->ep.desc == NULL)
		return -EBUSY;

	spin_lock_irqsave(mEp->lock, flags);

	/* only internal SW should disable ctrl endpts */

	direction = mEp->dir;
	do {
		retval |= _ep_nuke(mEp);
		retval |= hw_ep_disable(mEp->ci, mEp->num, mEp->dir);

		if (mEp->type == USB_ENDPOINT_XFER_CONTROL)
			mEp->dir = (mEp->dir == TX) ? RX : TX;

	} while (mEp->dir != direction);

	mEp->ep.desc = NULL;

	spin_unlock_irqrestore(mEp->lock, flags);
=======
	else if (hwep->ep.desc == NULL)
		return -EBUSY;

	spin_lock_irqsave(hwep->lock, flags);
	if (hwep->ci->gadget.speed == USB_SPEED_UNKNOWN) {
		spin_unlock_irqrestore(hwep->lock, flags);
		return 0;
	}

	/* only internal SW should disable ctrl endpts */

	direction = hwep->dir;
	do {
		retval |= _ep_nuke(hwep);
		retval |= hw_ep_disable(hwep->ci, hwep->num, hwep->dir);

		if (hwep->type == USB_ENDPOINT_XFER_CONTROL)
			hwep->dir = (hwep->dir == TX) ? RX : TX;

	} while (hwep->dir != direction);

	hwep->ep.desc = NULL;

	spin_unlock_irqrestore(hwep->lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return retval;
}

/**
 * ep_alloc_request: allocate a request object to use with this endpoint
 *
 * Check usb_ep_alloc_request() at "usb_gadget.h" for details
 */
static struct usb_request *ep_alloc_request(struct usb_ep *ep, gfp_t gfp_flags)
{
<<<<<<< HEAD
	struct ci13xxx_ep  *mEp  = container_of(ep, struct ci13xxx_ep, ep);
	struct ci13xxx_req *mReq = NULL;
=======
	struct ci_hw_req *hwreq = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (ep == NULL)
		return NULL;

<<<<<<< HEAD
	mReq = kzalloc(sizeof(struct ci13xxx_req), gfp_flags);
	if (mReq != NULL) {
		INIT_LIST_HEAD(&mReq->queue);

		mReq->ptr = dma_pool_alloc(mEp->td_pool, gfp_flags,
					   &mReq->dma);
		if (mReq->ptr == NULL) {
			kfree(mReq);
			mReq = NULL;
		}
	}

	return (mReq == NULL) ? NULL : &mReq->req;
=======
	hwreq = kzalloc(sizeof(struct ci_hw_req), gfp_flags);
	if (hwreq != NULL) {
		INIT_LIST_HEAD(&hwreq->queue);
		INIT_LIST_HEAD(&hwreq->tds);
	}

	return (hwreq == NULL) ? NULL : &hwreq->req;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * ep_free_request: frees a request object
 *
 * Check usb_ep_free_request() at "usb_gadget.h" for details
 */
static void ep_free_request(struct usb_ep *ep, struct usb_request *req)
{
<<<<<<< HEAD
	struct ci13xxx_ep  *mEp  = container_of(ep,  struct ci13xxx_ep, ep);
	struct ci13xxx_req *mReq = container_of(req, struct ci13xxx_req, req);
=======
	struct ci_hw_ep  *hwep  = container_of(ep,  struct ci_hw_ep, ep);
	struct ci_hw_req *hwreq = container_of(req, struct ci_hw_req, req);
	struct td_node *node, *tmpnode;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long flags;

	if (ep == NULL || req == NULL) {
		return;
<<<<<<< HEAD
	} else if (!list_empty(&mReq->queue)) {
		dev_err(mEp->ci->dev, "freeing queued request\n");
		return;
	}

	spin_lock_irqsave(mEp->lock, flags);

	if (mReq->ptr)
		dma_pool_free(mEp->td_pool, mReq->ptr, mReq->dma);
	kfree(mReq);

	spin_unlock_irqrestore(mEp->lock, flags);
=======
	} else if (!list_empty(&hwreq->queue)) {
		dev_err(hwep->ci->dev, "freeing queued request\n");
		return;
	}

	spin_lock_irqsave(hwep->lock, flags);

	list_for_each_entry_safe(node, tmpnode, &hwreq->tds, td) {
		dma_pool_free(hwep->td_pool, node->ptr, node->dma);
		list_del_init(&node->td);
		node->ptr = NULL;
		kfree(node);
	}

	kfree(hwreq);

	spin_unlock_irqrestore(hwep->lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * ep_queue: queues (submits) an I/O request to an endpoint
 *
 * Check usb_ep_queue()* at usb_gadget.h" for details
 */
static int ep_queue(struct usb_ep *ep, struct usb_request *req,
		    gfp_t __maybe_unused gfp_flags)
{
<<<<<<< HEAD
	struct ci13xxx_ep  *mEp  = container_of(ep,  struct ci13xxx_ep, ep);
	int retval = 0;
	unsigned long flags;

	if (ep == NULL || req == NULL || mEp->ep.desc == NULL)
		return -EINVAL;

	spin_lock_irqsave(mEp->lock, flags);
	retval = _ep_queue(ep, req, gfp_flags);
	spin_unlock_irqrestore(mEp->lock, flags);
=======
	struct ci_hw_ep  *hwep  = container_of(ep,  struct ci_hw_ep, ep);
	int retval = 0;
	unsigned long flags;

	if (ep == NULL || req == NULL || hwep->ep.desc == NULL)
		return -EINVAL;

	spin_lock_irqsave(hwep->lock, flags);
	if (hwep->ci->gadget.speed == USB_SPEED_UNKNOWN) {
		spin_unlock_irqrestore(hwep->lock, flags);
		return 0;
	}
	retval = _ep_queue(ep, req, gfp_flags);
	spin_unlock_irqrestore(hwep->lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return retval;
}

/**
 * ep_dequeue: dequeues (cancels, unlinks) an I/O request from an endpoint
 *
 * Check usb_ep_dequeue() at "usb_gadget.h" for details
 */
static int ep_dequeue(struct usb_ep *ep, struct usb_request *req)
{
<<<<<<< HEAD
	struct ci13xxx_ep  *mEp  = container_of(ep,  struct ci13xxx_ep, ep);
	struct ci13xxx_req *mReq = container_of(req, struct ci13xxx_req, req);
	unsigned long flags;

	if (ep == NULL || req == NULL || mReq->req.status != -EALREADY ||
		mEp->ep.desc == NULL || list_empty(&mReq->queue) ||
		list_empty(&mEp->qh.queue))
		return -EINVAL;

	spin_lock_irqsave(mEp->lock, flags);

	hw_ep_flush(mEp->ci, mEp->num, mEp->dir);

	/* pop request */
	list_del_init(&mReq->queue);

	usb_gadget_unmap_request(&mEp->ci->gadget, req, mEp->dir);

	req->status = -ECONNRESET;

	if (mReq->req.complete != NULL) {
		spin_unlock(mEp->lock);
		mReq->req.complete(&mEp->ep, &mReq->req);
		spin_lock(mEp->lock);
	}

	spin_unlock_irqrestore(mEp->lock, flags);
=======
	struct ci_hw_ep  *hwep  = container_of(ep,  struct ci_hw_ep, ep);
	struct ci_hw_req *hwreq = container_of(req, struct ci_hw_req, req);
	unsigned long flags;
	struct td_node *node, *tmpnode;

	if (ep == NULL || req == NULL || hwreq->req.status != -EALREADY ||
		hwep->ep.desc == NULL || list_empty(&hwreq->queue) ||
		list_empty(&hwep->qh.queue))
		return -EINVAL;

	spin_lock_irqsave(hwep->lock, flags);
	if (hwep->ci->gadget.speed != USB_SPEED_UNKNOWN)
		hw_ep_flush(hwep->ci, hwep->num, hwep->dir);

	list_for_each_entry_safe(node, tmpnode, &hwreq->tds, td) {
		dma_pool_free(hwep->td_pool, node->ptr, node->dma);
		list_del(&node->td);
		kfree(node);
	}

	/* pop request */
	list_del_init(&hwreq->queue);

	usb_gadget_unmap_request(&hwep->ci->gadget, req, hwep->dir);

	req->status = -ECONNRESET;

	if (hwreq->req.complete != NULL) {
		spin_unlock(hwep->lock);
		usb_gadget_giveback_request(&hwep->ep, &hwreq->req);
		spin_lock(hwep->lock);
	}

	spin_unlock_irqrestore(hwep->lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

/**
 * ep_set_halt: sets the endpoint halt feature
 *
 * Check usb_ep_set_halt() at "usb_gadget.h" for details
 */
static int ep_set_halt(struct usb_ep *ep, int value)
{
<<<<<<< HEAD
	struct ci13xxx_ep *mEp = container_of(ep, struct ci13xxx_ep, ep);
	int direction, retval = 0;
	unsigned long flags;

	if (ep == NULL || mEp->ep.desc == NULL)
		return -EINVAL;

	spin_lock_irqsave(mEp->lock, flags);

#ifndef STALL_IN
	/* g_file_storage MS compliant but g_zero fails chapter 9 compliance */
	if (value && mEp->type == USB_ENDPOINT_XFER_BULK && mEp->dir == TX &&
	    !list_empty(&mEp->qh.queue)) {
		spin_unlock_irqrestore(mEp->lock, flags);
		return -EAGAIN;
	}
#endif

	direction = mEp->dir;
	do {
		retval |= hw_ep_set_halt(mEp->ci, mEp->num, mEp->dir, value);

		if (!value)
			mEp->wedge = 0;

		if (mEp->type == USB_ENDPOINT_XFER_CONTROL)
			mEp->dir = (mEp->dir == TX) ? RX : TX;

	} while (mEp->dir != direction);

	spin_unlock_irqrestore(mEp->lock, flags);
	return retval;
=======
	return _ep_set_halt(ep, value, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * ep_set_wedge: sets the halt feature and ignores clear requests
 *
 * Check usb_ep_set_wedge() at "usb_gadget.h" for details
 */
static int ep_set_wedge(struct usb_ep *ep)
{
<<<<<<< HEAD
	struct ci13xxx_ep *mEp = container_of(ep, struct ci13xxx_ep, ep);
	unsigned long flags;

	if (ep == NULL || mEp->ep.desc == NULL)
		return -EINVAL;

	spin_lock_irqsave(mEp->lock, flags);
	mEp->wedge = 1;
	spin_unlock_irqrestore(mEp->lock, flags);
=======
	struct ci_hw_ep *hwep = container_of(ep, struct ci_hw_ep, ep);
	unsigned long flags;

	if (ep == NULL || hwep->ep.desc == NULL)
		return -EINVAL;

	spin_lock_irqsave(hwep->lock, flags);
	hwep->wedge = 1;
	spin_unlock_irqrestore(hwep->lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return usb_ep_set_halt(ep);
}

/**
 * ep_fifo_flush: flushes contents of a fifo
 *
 * Check usb_ep_fifo_flush() at "usb_gadget.h" for details
 */
static void ep_fifo_flush(struct usb_ep *ep)
{
<<<<<<< HEAD
	struct ci13xxx_ep *mEp = container_of(ep, struct ci13xxx_ep, ep);
	unsigned long flags;

	if (ep == NULL) {
		dev_err(mEp->ci->dev, "%02X: -EINVAL\n", _usb_addr(mEp));
		return;
	}

	spin_lock_irqsave(mEp->lock, flags);

	hw_ep_flush(mEp->ci, mEp->num, mEp->dir);

	spin_unlock_irqrestore(mEp->lock, flags);
=======
	struct ci_hw_ep *hwep = container_of(ep, struct ci_hw_ep, ep);
	unsigned long flags;

	if (ep == NULL) {
		dev_err(hwep->ci->dev, "%02X: -EINVAL\n", _usb_addr(hwep));
		return;
	}

	spin_lock_irqsave(hwep->lock, flags);
	if (hwep->ci->gadget.speed == USB_SPEED_UNKNOWN) {
		spin_unlock_irqrestore(hwep->lock, flags);
		return;
	}

	hw_ep_flush(hwep->ci, hwep->num, hwep->dir);

	spin_unlock_irqrestore(hwep->lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * Endpoint-specific part of the API to the USB controller hardware
 * Check "usb_gadget.h" for details
 */
static const struct usb_ep_ops usb_ep_ops = {
	.enable	       = ep_enable,
	.disable       = ep_disable,
	.alloc_request = ep_alloc_request,
	.free_request  = ep_free_request,
	.queue	       = ep_queue,
	.dequeue       = ep_dequeue,
	.set_halt      = ep_set_halt,
	.set_wedge     = ep_set_wedge,
	.fifo_flush    = ep_fifo_flush,
};

/******************************************************************************
 * GADGET block
 *****************************************************************************/
<<<<<<< HEAD
static int ci13xxx_vbus_session(struct usb_gadget *_gadget, int is_active)
{
	struct ci13xxx *ci = container_of(_gadget, struct ci13xxx, gadget);
	unsigned long flags;
	int gadget_ready = 0;

	if (!(ci->platdata->flags & CI13XXX_PULLUP_ON_VBUS))
		return -EOPNOTSUPP;

=======
static int ci_udc_vbus_session(struct usb_gadget *_gadget, int is_active)
{
	struct ci_hdrc *ci = container_of(_gadget, struct ci_hdrc, gadget);
	unsigned long flags;
	int gadget_ready = 0;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_lock_irqsave(&ci->lock, flags);
	ci->vbus_active = is_active;
	if (ci->driver)
		gadget_ready = 1;
	spin_unlock_irqrestore(&ci->lock, flags);

	if (gadget_ready) {
		if (is_active) {
			pm_runtime_get_sync(&_gadget->dev);
<<<<<<< HEAD
			hw_device_reset(ci, USBMODE_CM_DC);
			hw_device_state(ci, ci->ep0out->qh.dma);
		} else {
			hw_device_state(ci, 0);
			if (ci->platdata->notify_event)
				ci->platdata->notify_event(ci,
				CI13XXX_CONTROLLER_STOPPED_EVENT);
			_gadget_stop_activity(&ci->gadget);
			pm_runtime_put_sync(&_gadget->dev);
=======
			hw_device_reset(ci);
			hw_device_state(ci, ci->ep0out->qh.dma);
			usb_gadget_set_state(_gadget, USB_STATE_POWERED);
			usb_udc_vbus_handler(_gadget, true);
		} else {
			usb_udc_vbus_handler(_gadget, false);
			if (ci->driver)
				ci->driver->disconnect(&ci->gadget);
			hw_device_state(ci, 0);
			if (ci->platdata->notify_event)
				ci->platdata->notify_event(ci,
				CI_HDRC_CONTROLLER_STOPPED_EVENT);
			_gadget_stop_activity(&ci->gadget);
			pm_runtime_put_sync(&_gadget->dev);
			usb_gadget_set_state(_gadget, USB_STATE_NOTATTACHED);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	return 0;
}

<<<<<<< HEAD
static int ci13xxx_wakeup(struct usb_gadget *_gadget)
{
	struct ci13xxx *ci = container_of(_gadget, struct ci13xxx, gadget);
=======
static int ci_udc_wakeup(struct usb_gadget *_gadget)
{
	struct ci_hdrc *ci = container_of(_gadget, struct ci_hdrc, gadget);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&ci->lock, flags);
<<<<<<< HEAD
=======
	if (ci->gadget.speed == USB_SPEED_UNKNOWN) {
		spin_unlock_irqrestore(&ci->lock, flags);
		return 0;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!ci->remote_wakeup) {
		ret = -EOPNOTSUPP;
		goto out;
	}
	if (!hw_read(ci, OP_PORTSC, PORTSC_SUSP)) {
		ret = -EINVAL;
		goto out;
	}
	hw_write(ci, OP_PORTSC, PORTSC_FPR, PORTSC_FPR);
out:
	spin_unlock_irqrestore(&ci->lock, flags);
	return ret;
}

<<<<<<< HEAD
static int ci13xxx_vbus_draw(struct usb_gadget *_gadget, unsigned mA)
{
	struct ci13xxx *ci = container_of(_gadget, struct ci13xxx, gadget);

	if (ci->transceiver)
		return usb_phy_set_power(ci->transceiver, mA);
	return -ENOTSUPP;
}

/* Change Data+ pullup status
 * this func is used by usb_gadget_connect/disconnet
 */
static int ci13xxx_pullup(struct usb_gadget *_gadget, int is_on)
{
	struct ci13xxx *ci = container_of(_gadget, struct ci13xxx, gadget);

=======
static int ci_udc_vbus_draw(struct usb_gadget *_gadget, unsigned ma)
{
	struct ci_hdrc *ci = container_of(_gadget, struct ci_hdrc, gadget);

	if (ci->usb_phy)
		return usb_phy_set_power(ci->usb_phy, ma);
	return -ENOTSUPP;
}

static int ci_udc_selfpowered(struct usb_gadget *_gadget, int is_on)
{
	struct ci_hdrc *ci = container_of(_gadget, struct ci_hdrc, gadget);
	struct ci_hw_ep *hwep = ci->ep0in;
	unsigned long flags;

	spin_lock_irqsave(hwep->lock, flags);
	_gadget->is_selfpowered = (is_on != 0);
	spin_unlock_irqrestore(hwep->lock, flags);

	return 0;
}

/* Change Data+ pullup status
 * this func is used by usb_gadget_connect/disconnet
 */
static int ci_udc_pullup(struct usb_gadget *_gadget, int is_on)
{
	struct ci_hdrc *ci = container_of(_gadget, struct ci_hdrc, gadget);

	/*
	 * Data+ pullup controlled by OTG state machine in OTG fsm mode;
	 * and don't touch Data+ in host mode for dual role config.
	 */
	if (ci_otg_is_fsm_mode(ci) || ci->role == CI_ROLE_HOST)
		return 0;

	pm_runtime_get_sync(&ci->gadget.dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (is_on)
		hw_write(ci, OP_USBCMD, USBCMD_RS, USBCMD_RS);
	else
		hw_write(ci, OP_USBCMD, USBCMD_RS, 0);
<<<<<<< HEAD
=======
	pm_runtime_put_sync(&ci->gadget.dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static int ci13xxx_start(struct usb_gadget *gadget,
			 struct usb_gadget_driver *driver);
static int ci13xxx_stop(struct usb_gadget *gadget,
			struct usb_gadget_driver *driver);
=======
static int ci_udc_start(struct usb_gadget *gadget,
			 struct usb_gadget_driver *driver);
static int ci_udc_stop(struct usb_gadget *gadget);

/* Match ISOC IN from the highest endpoint */
static struct usb_ep *ci_udc_match_ep(struct usb_gadget *gadget,
			      struct usb_endpoint_descriptor *desc,
			      struct usb_ss_ep_comp_descriptor *comp_desc)
{
	struct ci_hdrc *ci = container_of(gadget, struct ci_hdrc, gadget);
	struct usb_ep *ep;

	if (usb_endpoint_xfer_isoc(desc) && usb_endpoint_dir_in(desc)) {
		list_for_each_entry_reverse(ep, &ci->gadget.ep_list, ep_list) {
			if (ep->caps.dir_in && !ep->claimed)
				return ep;
		}
	}

	return NULL;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * Device operations part of the API to the USB controller hardware,
 * which don't involve endpoints (or i/o)
 * Check  "usb_gadget.h" for details
 */
static const struct usb_gadget_ops usb_gadget_ops = {
<<<<<<< HEAD
	.vbus_session	= ci13xxx_vbus_session,
	.wakeup		= ci13xxx_wakeup,
	.pullup		= ci13xxx_pullup,
	.vbus_draw	= ci13xxx_vbus_draw,
	.udc_start	= ci13xxx_start,
	.udc_stop	= ci13xxx_stop,
};

static int init_eps(struct ci13xxx *ci)
=======
	.vbus_session	= ci_udc_vbus_session,
	.wakeup		= ci_udc_wakeup,
	.set_selfpowered	= ci_udc_selfpowered,
	.pullup		= ci_udc_pullup,
	.vbus_draw	= ci_udc_vbus_draw,
	.udc_start	= ci_udc_start,
	.udc_stop	= ci_udc_stop,
	.match_ep 	= ci_udc_match_ep,
};

static int init_eps(struct ci_hdrc *ci)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int retval = 0, i, j;

	for (i = 0; i < ci->hw_ep_max/2; i++)
		for (j = RX; j <= TX; j++) {
			int k = i + j * ci->hw_ep_max/2;
<<<<<<< HEAD
			struct ci13xxx_ep *mEp = &ci->ci13xxx_ep[k];

			scnprintf(mEp->name, sizeof(mEp->name), "ep%i%s", i,
					(j == TX)  ? "in" : "out");

			mEp->ci          = ci;
			mEp->lock         = &ci->lock;
			mEp->td_pool      = ci->td_pool;

			mEp->ep.name      = mEp->name;
			mEp->ep.ops       = &usb_ep_ops;
=======
			struct ci_hw_ep *hwep = &ci->ci_hw_ep[k];

			scnprintf(hwep->name, sizeof(hwep->name), "ep%i%s", i,
					(j == TX)  ? "in" : "out");

			hwep->ci          = ci;
			hwep->lock         = &ci->lock;
			hwep->td_pool      = ci->td_pool;

			hwep->ep.name      = hwep->name;
			hwep->ep.ops       = &usb_ep_ops;

			if (i == 0) {
				hwep->ep.caps.type_control = true;
			} else {
				hwep->ep.caps.type_iso = true;
				hwep->ep.caps.type_bulk = true;
				hwep->ep.caps.type_int = true;
			}

			if (j == TX)
				hwep->ep.caps.dir_in = true;
			else
				hwep->ep.caps.dir_out = true;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/*
			 * for ep0: maxP defined in desc, for other
			 * eps, maxP is set by epautoconfig() called
			 * by gadget layer
			 */
<<<<<<< HEAD
			mEp->ep.maxpacket = (unsigned short)~0;

			INIT_LIST_HEAD(&mEp->qh.queue);
			mEp->qh.ptr = dma_pool_alloc(ci->qh_pool, GFP_KERNEL,
						     &mEp->qh.dma);
			if (mEp->qh.ptr == NULL)
				retval = -ENOMEM;
			else
				memset(mEp->qh.ptr, 0, sizeof(*mEp->qh.ptr));
=======
			usb_ep_set_maxpacket_limit(&hwep->ep, (unsigned short)~0);

			INIT_LIST_HEAD(&hwep->qh.queue);
			hwep->qh.ptr = dma_pool_zalloc(ci->qh_pool, GFP_KERNEL,
						       &hwep->qh.dma);
			if (hwep->qh.ptr == NULL)
				retval = -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			/*
			 * set up shorthands for ep0 out and in endpoints,
			 * don't add to gadget's ep_list
			 */
			if (i == 0) {
				if (j == RX)
<<<<<<< HEAD
					ci->ep0out = mEp;
				else
					ci->ep0in = mEp;

				mEp->ep.maxpacket = CTRL_PAYLOAD_MAX;
				continue;
			}

			list_add_tail(&mEp->ep.ep_list, &ci->gadget.ep_list);
=======
					ci->ep0out = hwep;
				else
					ci->ep0in = hwep;

				usb_ep_set_maxpacket_limit(&hwep->ep, CTRL_PAYLOAD_MAX);
				continue;
			}

			list_add_tail(&hwep->ep.ep_list, &ci->gadget.ep_list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

	return retval;
}

<<<<<<< HEAD
static void destroy_eps(struct ci13xxx *ci)
=======
static void destroy_eps(struct ci_hdrc *ci)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int i;

	for (i = 0; i < ci->hw_ep_max; i++) {
<<<<<<< HEAD
		struct ci13xxx_ep *mEp = &ci->ci13xxx_ep[i];

		dma_pool_free(ci->qh_pool, mEp->qh.ptr, mEp->qh.dma);
=======
		struct ci_hw_ep *hwep = &ci->ci_hw_ep[i];

		if (hwep->pending_td)
			free_pending_td(hwep);
		dma_pool_free(ci->qh_pool, hwep->qh.ptr, hwep->qh.dma);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

/**
<<<<<<< HEAD
 * ci13xxx_start: register a gadget driver
=======
 * ci_udc_start: register a gadget driver
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @gadget: our gadget
 * @driver: the driver being registered
 *
 * Interrupts are enabled here.
 */
<<<<<<< HEAD
static int ci13xxx_start(struct usb_gadget *gadget,
			 struct usb_gadget_driver *driver)
{
	struct ci13xxx *ci = container_of(gadget, struct ci13xxx, gadget);
=======
static int ci_udc_start(struct usb_gadget *gadget,
			 struct usb_gadget_driver *driver)
{
	struct ci_hdrc *ci = container_of(gadget, struct ci_hdrc, gadget);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long flags;
	int retval = -ENOMEM;

	if (driver->disconnect == NULL)
		return -EINVAL;


	ci->ep0out->ep.desc = &ctrl_endpt_out_desc;
	retval = usb_ep_enable(&ci->ep0out->ep);
	if (retval)
		return retval;

	ci->ep0in->ep.desc = &ctrl_endpt_in_desc;
	retval = usb_ep_enable(&ci->ep0in->ep);
	if (retval)
		return retval;
<<<<<<< HEAD
	spin_lock_irqsave(&ci->lock, flags);

	ci->driver = driver;
	pm_runtime_get_sync(&ci->gadget.dev);
	if (ci->platdata->flags & CI13XXX_PULLUP_ON_VBUS) {
		if (ci->vbus_active) {
			if (ci->platdata->flags & CI13XXX_REGS_SHARED)
				hw_device_reset(ci, USBMODE_CM_DC);
		} else {
			pm_runtime_put_sync(&ci->gadget.dev);
			goto done;
		}
	}

	retval = hw_device_state(ci, ci->ep0out->qh.dma);
	if (retval)
		pm_runtime_put_sync(&ci->gadget.dev);

 done:
	spin_unlock_irqrestore(&ci->lock, flags);
	return retval;
}

/**
 * ci13xxx_stop: unregister a gadget driver
 */
static int ci13xxx_stop(struct usb_gadget *gadget,
			struct usb_gadget_driver *driver)
{
	struct ci13xxx *ci = container_of(gadget, struct ci13xxx, gadget);
=======

	ci->driver = driver;

	/* Start otg fsm for B-device */
	if (ci_otg_is_fsm_mode(ci) && ci->fsm.id) {
		ci_hdrc_otg_fsm_start(ci);
		return retval;
	}

	pm_runtime_get_sync(&ci->gadget.dev);
	if (ci->vbus_active) {
		spin_lock_irqsave(&ci->lock, flags);
		hw_device_reset(ci);
	} else {
		usb_udc_vbus_handler(&ci->gadget, false);
		pm_runtime_put_sync(&ci->gadget.dev);
		return retval;
	}

	retval = hw_device_state(ci, ci->ep0out->qh.dma);
	spin_unlock_irqrestore(&ci->lock, flags);
	if (retval)
		pm_runtime_put_sync(&ci->gadget.dev);

	return retval;
}

static void ci_udc_stop_for_otg_fsm(struct ci_hdrc *ci)
{
	if (!ci_otg_is_fsm_mode(ci))
		return;

	mutex_lock(&ci->fsm.lock);
	if (ci->fsm.otg->state == OTG_STATE_A_PERIPHERAL) {
		ci->fsm.a_bidl_adis_tmout = 1;
		ci_hdrc_otg_fsm_start(ci);
	} else if (ci->fsm.otg->state == OTG_STATE_B_PERIPHERAL) {
		ci->fsm.protocol = PROTO_UNDEF;
		ci->fsm.otg->state = OTG_STATE_UNDEFINED;
	}
	mutex_unlock(&ci->fsm.lock);
}

/**
 * ci_udc_stop: unregister a gadget driver
 */
static int ci_udc_stop(struct usb_gadget *gadget)
{
	struct ci_hdrc *ci = container_of(gadget, struct ci_hdrc, gadget);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long flags;

	spin_lock_irqsave(&ci->lock, flags);

<<<<<<< HEAD
	if (!(ci->platdata->flags & CI13XXX_PULLUP_ON_VBUS) ||
			ci->vbus_active) {
		hw_device_state(ci, 0);
		if (ci->platdata->notify_event)
			ci->platdata->notify_event(ci,
			CI13XXX_CONTROLLER_STOPPED_EVENT);
		ci->driver = NULL;
=======
	if (ci->vbus_active) {
		hw_device_state(ci, 0);
		if (ci->platdata->notify_event)
			ci->platdata->notify_event(ci,
			CI_HDRC_CONTROLLER_STOPPED_EVENT);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		spin_unlock_irqrestore(&ci->lock, flags);
		_gadget_stop_activity(&ci->gadget);
		spin_lock_irqsave(&ci->lock, flags);
		pm_runtime_put(&ci->gadget.dev);
	}

<<<<<<< HEAD
	spin_unlock_irqrestore(&ci->lock, flags);

=======
	ci->driver = NULL;
	spin_unlock_irqrestore(&ci->lock, flags);

	ci_udc_stop_for_otg_fsm(ci);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

/******************************************************************************
 * BUS block
 *****************************************************************************/
/**
 * udc_irq: ci interrupt handler
 *
 * This function returns IRQ_HANDLED if the IRQ has been handled
 * It locks access to registers
 */
<<<<<<< HEAD
static irqreturn_t udc_irq(struct ci13xxx *ci)
=======
static irqreturn_t udc_irq(struct ci_hdrc *ci)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	irqreturn_t retval;
	u32 intr;

	if (ci == NULL)
		return IRQ_HANDLED;

	spin_lock(&ci->lock);

<<<<<<< HEAD
	if (ci->platdata->flags & CI13XXX_REGS_SHARED) {
=======
	if (ci->platdata->flags & CI_HDRC_REGS_SHARED) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (hw_read(ci, OP_USBMODE, USBMODE_CM) !=
				USBMODE_CM_DC) {
			spin_unlock(&ci->lock);
			return IRQ_NONE;
		}
	}
	intr = hw_test_and_clear_intr_active(ci);

	if (intr) {
		/* order defines priority - do NOT change it */
		if (USBi_URI & intr)
			isr_reset_handler(ci);

		if (USBi_PCI & intr) {
			ci->gadget.speed = hw_port_is_high_speed(ci) ?
				USB_SPEED_HIGH : USB_SPEED_FULL;
			if (ci->suspended && ci->driver->resume) {
				spin_unlock(&ci->lock);
				ci->driver->resume(&ci->gadget);
				spin_lock(&ci->lock);
				ci->suspended = 0;
			}
		}

		if (USBi_UI  & intr)
			isr_tr_complete_handler(ci);

		if (USBi_SLI & intr) {
			if (ci->gadget.speed != USB_SPEED_UNKNOWN &&
			    ci->driver->suspend) {
				ci->suspended = 1;
				spin_unlock(&ci->lock);
				ci->driver->suspend(&ci->gadget);
<<<<<<< HEAD
=======
				usb_gadget_set_state(&ci->gadget,
						USB_STATE_SUSPENDED);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				spin_lock(&ci->lock);
			}
		}
		retval = IRQ_HANDLED;
	} else {
		retval = IRQ_NONE;
	}
	spin_unlock(&ci->lock);

	return retval;
}

/**
 * udc_start: initialize gadget role
 * @ci: chipidea controller
 */
<<<<<<< HEAD
static int udc_start(struct ci13xxx *ci)
{
	struct device *dev = ci->dev;
=======
static int udc_start(struct ci_hdrc *ci)
{
	struct device *dev = ci->dev;
	struct usb_otg_caps *otg_caps = &ci->platdata->ci_otg_caps;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int retval = 0;

	ci->gadget.ops          = &usb_gadget_ops;
	ci->gadget.speed        = USB_SPEED_UNKNOWN;
	ci->gadget.max_speed    = USB_SPEED_HIGH;
<<<<<<< HEAD
	ci->gadget.is_otg       = 0;
	ci->gadget.name         = ci->platdata->name;
=======
	ci->gadget.name         = ci->platdata->name;
	ci->gadget.otg_caps	= otg_caps;

	if (ci->is_otg && (otg_caps->hnp_support || otg_caps->srp_support ||
						otg_caps->adp_support))
		ci->gadget.is_otg = 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	INIT_LIST_HEAD(&ci->gadget.ep_list);

	/* alloc resources */
<<<<<<< HEAD
	ci->qh_pool = dma_pool_create("ci13xxx_qh", dev,
				       sizeof(struct ci13xxx_qh),
				       64, CI13XXX_PAGE_SIZE);
	if (ci->qh_pool == NULL)
		return -ENOMEM;

	ci->td_pool = dma_pool_create("ci13xxx_td", dev,
				       sizeof(struct ci13xxx_td),
				       64, CI13XXX_PAGE_SIZE);
=======
	ci->qh_pool = dma_pool_create("ci_hw_qh", dev,
				       sizeof(struct ci_hw_qh),
				       64, CI_HDRC_PAGE_SIZE);
	if (ci->qh_pool == NULL)
		return -ENOMEM;

	ci->td_pool = dma_pool_create("ci_hw_td", dev,
				       sizeof(struct ci_hw_td),
				       64, CI_HDRC_PAGE_SIZE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ci->td_pool == NULL) {
		retval = -ENOMEM;
		goto free_qh_pool;
	}

	retval = init_eps(ci);
	if (retval)
		goto free_pools;

	ci->gadget.ep0 = &ci->ep0in->ep;

<<<<<<< HEAD
	if (ci->global_phy) {
		ci->transceiver = usb_get_phy(USB_PHY_TYPE_USB2);
		if (IS_ERR(ci->transceiver))
			ci->transceiver = NULL;
	}

	if (ci->platdata->flags & CI13XXX_REQUIRE_TRANSCEIVER) {
		if (ci->transceiver == NULL) {
			retval = -ENODEV;
			goto destroy_eps;
		}
	}

	if (!(ci->platdata->flags & CI13XXX_REGS_SHARED)) {
		retval = hw_device_reset(ci, USBMODE_CM_DC);
		if (retval)
			goto put_transceiver;
	}

	if (ci->transceiver) {
		retval = otg_set_peripheral(ci->transceiver->otg,
						&ci->gadget);
		if (retval)
			goto put_transceiver;
	}

	retval = usb_add_gadget_udc(dev, &ci->gadget);
	if (retval)
		goto remove_trans;
=======
	retval = usb_add_gadget_udc(dev, &ci->gadget);
	if (retval)
		goto destroy_eps;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	pm_runtime_no_callbacks(&ci->gadget.dev);
	pm_runtime_enable(&ci->gadget.dev);

	return retval;

<<<<<<< HEAD
remove_trans:
	if (ci->transceiver) {
		otg_set_peripheral(ci->transceiver->otg, NULL);
		if (ci->global_phy)
			usb_put_phy(ci->transceiver);
	}

	dev_err(dev, "error = %i\n", retval);
put_transceiver:
	if (ci->transceiver && ci->global_phy)
		usb_put_phy(ci->transceiver);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
destroy_eps:
	destroy_eps(ci);
free_pools:
	dma_pool_destroy(ci->td_pool);
free_qh_pool:
	dma_pool_destroy(ci->qh_pool);
	return retval;
}

/**
<<<<<<< HEAD
 * udc_remove: parent remove must call this to remove UDC
 *
 * No interrupts active, the IRQ has been released
 */
static void udc_stop(struct ci13xxx *ci)
{
	if (ci == NULL)
=======
 * ci_hdrc_gadget_destroy: parent remove must call this to remove UDC
 *
 * No interrupts active, the IRQ has been released
 */
void ci_hdrc_gadget_destroy(struct ci_hdrc *ci)
{
	if (!ci->roles[CI_ROLE_GADGET])
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;

	usb_del_gadget_udc(&ci->gadget);

	destroy_eps(ci);

	dma_pool_destroy(ci->td_pool);
	dma_pool_destroy(ci->qh_pool);
<<<<<<< HEAD

	if (ci->transceiver) {
		otg_set_peripheral(ci->transceiver->otg, NULL);
		if (ci->global_phy)
			usb_put_phy(ci->transceiver);
	}
	/* my kobject is dynamic, I swear! */
	memset(&ci->gadget, 0, sizeof(ci->gadget));
=======
}

static int udc_id_switch_for_device(struct ci_hdrc *ci)
{
	if (ci->is_otg)
		/* Clear and enable BSV irq */
		hw_write_otgsc(ci, OTGSC_BSVIS | OTGSC_BSVIE,
					OTGSC_BSVIS | OTGSC_BSVIE);

	return 0;
}

static void udc_id_switch_for_host(struct ci_hdrc *ci)
{
	/*
	 * host doesn't care B_SESSION_VALID event
	 * so clear and disbale BSV irq
	 */
	if (ci->is_otg)
		hw_write_otgsc(ci, OTGSC_BSVIE | OTGSC_BSVIS, OTGSC_BSVIS);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * ci_hdrc_gadget_init - initialize device related bits
 * ci: the controller
 *
<<<<<<< HEAD
 * This function enables the gadget role, if the device is "device capable".
 */
int ci_hdrc_gadget_init(struct ci13xxx *ci)
{
	struct ci_role_driver *rdrv;
=======
 * This function initializes the gadget, if the device is "device capable".
 */
int ci_hdrc_gadget_init(struct ci_hdrc *ci)
{
	struct ci_role_driver *rdrv;
	int ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!hw_read(ci, CAP_DCCPARAMS, DCCPARAMS_DC))
		return -ENXIO;

<<<<<<< HEAD
	rdrv = devm_kzalloc(ci->dev, sizeof(struct ci_role_driver), GFP_KERNEL);
	if (!rdrv)
		return -ENOMEM;

	rdrv->start	= udc_start;
	rdrv->stop	= udc_stop;
	rdrv->irq	= udc_irq;
	rdrv->name	= "gadget";
	ci->roles[CI_ROLE_GADGET] = rdrv;

	return 0;
=======
	rdrv = devm_kzalloc(ci->dev, sizeof(*rdrv), GFP_KERNEL);
	if (!rdrv)
		return -ENOMEM;

	rdrv->start	= udc_id_switch_for_device;
	rdrv->stop	= udc_id_switch_for_host;
	rdrv->irq	= udc_irq;
	rdrv->name	= "gadget";

	ret = udc_start(ci);
	if (!ret)
		ci->roles[CI_ROLE_GADGET] = rdrv;

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
