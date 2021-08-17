/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
<<<<<<< HEAD
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * File: usbpipe.h
 *
 * Purpose:
 *
 * Author: Warren Hsu
 *
 * Date: Mar. 30, 2005
 *
 */

#ifndef __USBPIPE_H__
#define __USBPIPE_H__

#include "device.h"

<<<<<<< HEAD
int PIPEnsControlOut(struct vnt_private *, u8 byRequest, u16 wValue,
		u16 wIndex, u16 wLength, u8 *pbyBuffer);
int PIPEnsControlOutAsyn(struct vnt_private *, u8 byRequest,
	u16 wValue, u16 wIndex, u16 wLength, u8 *pbyBuffer);
int PIPEnsControlIn(struct vnt_private *, u8 byRequest, u16 wValue,
	u16 wIndex, u16 wLength,  u8 *pbyBuffer);

int PIPEnsInterruptRead(struct vnt_private *);
int PIPEnsBulkInUsbRead(struct vnt_private *, PRCB pRCB);
int PIPEnsSendBulkOut(struct vnt_private *, PUSB_SEND_CONTEXT pContext);
=======
int vnt_control_out(struct vnt_private *, u8, u16, u16, u16, u8 *);
int vnt_control_in(struct vnt_private *, u8, u16, u16, u16,  u8 *);

void vnt_control_out_u8(struct vnt_private *, u8, u8, u8);
void vnt_control_in_u8(struct vnt_private *, u8, u8, u8 *);

int vnt_start_interrupt_urb(struct vnt_private *);
int vnt_submit_rx_urb(struct vnt_private *, struct vnt_rcb *);
int vnt_tx_context(struct vnt_private *, struct vnt_usb_send_context *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __USBPIPE_H__ */
