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
 * File: int.h
 *
 * Purpose:
 *
 * Author: Jerry Chen
 *
 * Date: Apr. 2, 2004
 *
 */

#ifndef __INT_H__
#define __INT_H__

#include "device.h"

<<<<<<< HEAD
typedef struct tagSINTData {
	u8 byTSR0;
	u8 byPkt0;
	u16 wTime0;
	u8 byTSR1;
	u8 byPkt1;
	u16 wTime1;
	u8 byTSR2;
	u8 byPkt2;
	u16 wTime2;
	u8 byTSR3;
	u8 byPkt3;
	u16 wTime3;
	u64 qwTSF;
	u8 byISR0;
	u8 byISR1;
	u8 byRTSSuccess;
	u8 byRTSFail;
	u8 byACKFail;
	u8 byFCSErr;
	u8 abySW[2];
} __attribute__ ((__packed__))
SINTData, *PSINTData;

void INTvWorkItem(struct vnt_private *);
void INTnsProcessData(struct vnt_private *);
=======
struct vnt_interrupt_data {
	u8 tsr0;
	u8 pkt0;
	u16 time0;
	u8 tsr1;
	u8 pkt1;
	u16 time1;
	u8 tsr2;
	u8 pkt2;
	u16 time2;
	u8 tsr3;
	u8 pkt3;
	u16 time3;
	__le64 tsf;
	u8 isr0;
	u8 isr1;
	u8 rts_success;
	u8 rts_fail;
	u8 ack_fail;
	u8 fcs_err;
	u8 sw[2];
} __packed;

void vnt_int_start_interrupt(struct vnt_private *);
void vnt_int_process_data(struct vnt_private *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __INT_H__ */
