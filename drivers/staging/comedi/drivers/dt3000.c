/*
<<<<<<< HEAD
    comedi/drivers/dt3000.c
    Data Translation DT3000 series driver

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 1999 David A. Schleef <ds@schleef.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/
/*
Driver: dt3000
Description: Data Translation DT3000 series
Author: ds
Devices: [Data Translation] DT3001 (dt3000), DT3001-PGL, DT3002, DT3003,
  DT3003-PGL, DT3004, DT3005, DT3004-200
Updated: Mon, 14 Apr 2008 15:41:24 +0100
Status: works

Configuration Options: not applicable, uses PCI auto config

There is code to support AI commands, but it may not work.

AO commands are not supported.
*/

/*
   The DT3000 series is Data Translation's attempt to make a PCI
   data acquisition board.  The design of this series is very nice,
   since each board has an on-board DSP (Texas Instruments TMS320C52).
   However, a few details are a little annoying.  The boards lack
   bus-mastering DMA, which eliminates them from serious work.
   They also are not capable of autocalibration, which is a common
   feature in modern hardware.  The default firmware is pretty bad,
   making it nearly impossible to write an RT compatible driver.
   It would make an interesting project to write a decent firmware
   for these boards.

   Data Translation originally wanted an NDA for the documentation
   for the 3k series.  However, if you ask nicely, they might send
   you the docs without one, also.
*/

#define DEBUG 1

#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include "../comedidev.h"

#include "comedi_fc.h"
=======
 * dt3000.c
 * Data Translation DT3000 series driver
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 1999 David A. Schleef <ds@schleef.org>
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
 */

/*
 * Driver: dt3000
 * Description: Data Translation DT3000 series
 * Devices: [Data Translation] DT3001 (dt3000), DT3001-PGL, DT3002, DT3003,
 *   DT3003-PGL, DT3004, DT3005, DT3004-200
 * Author: ds
 * Updated: Mon, 14 Apr 2008 15:41:24 +0100
 * Status: works
 *
 * Configuration Options: not applicable, uses PCI auto config
 *
 * There is code to support AI commands, but it may not work.
 *
 * AO commands are not supported.
 */

/*
 * The DT3000 series is Data Translation's attempt to make a PCI
 * data acquisition board.  The design of this series is very nice,
 * since each board has an on-board DSP (Texas Instruments TMS320C52).
 * However, a few details are a little annoying.  The boards lack
 * bus-mastering DMA, which eliminates them from serious work.
 * They also are not capable of autocalibration, which is a common
 * feature in modern hardware.  The default firmware is pretty bad,
 * making it nearly impossible to write an RT compatible driver.
 * It would make an interesting project to write a decent firmware
 * for these boards.
 *
 * Data Translation originally wanted an NDA for the documentation
 * for the 3k series.  However, if you ask nicely, they might send
 * you the docs without one, also.
 */

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include "../comedi_pci.h"

/*
 * PCI BAR0 - dual-ported RAM location definitions (dev->mmio)
 */
#define DPR_DAC_BUFFER		(4 * 0x000)
#define DPR_ADC_BUFFER		(4 * 0x800)
#define DPR_COMMAND		(4 * 0xfd3)
#define DPR_SUBSYS		(4 * 0xfd3)
#define DPR_SUBSYS_AI		0
#define DPR_SUBSYS_AO		1
#define DPR_SUBSYS_DIN		2
#define DPR_SUBSYS_DOUT		3
#define DPR_SUBSYS_MEM		4
#define DPR_SUBSYS_CT		5
#define DPR_ENCODE		(4 * 0xfd4)
#define DPR_PARAMS(x)		(4 * (0xfd5 + (x)))
#define DPR_TICK_REG_LO		(4 * 0xff5)
#define DPR_TICK_REG_HI		(4 * 0xff6)
#define DPR_DA_BUF_FRONT	(4 * 0xff7)
#define DPR_DA_BUF_REAR		(4 * 0xff8)
#define DPR_AD_BUF_FRONT	(4 * 0xff9)
#define DPR_AD_BUF_REAR		(4 * 0xffa)
#define DPR_INT_MASK		(4 * 0xffb)
#define DPR_INTR_FLAG		(4 * 0xffc)
#define DPR_INTR_CMDONE		BIT(7)
#define DPR_INTR_CTDONE		BIT(6)
#define DPR_INTR_DAHWERR	BIT(5)
#define DPR_INTR_DASWERR	BIT(4)
#define DPR_INTR_DAEMPTY	BIT(3)
#define DPR_INTR_ADHWERR	BIT(2)
#define DPR_INTR_ADSWERR	BIT(1)
#define DPR_INTR_ADFULL		BIT(0)
#define DPR_RESPONSE_MBX	(4 * 0xffe)
#define DPR_CMD_MBX		(4 * 0xfff)
#define DPR_CMD_COMPLETION(x)	((x) << 8)
#define DPR_CMD_NOTPROCESSED	DPR_CMD_COMPLETION(0x00)
#define DPR_CMD_NOERROR		DPR_CMD_COMPLETION(0x55)
#define DPR_CMD_ERROR		DPR_CMD_COMPLETION(0xaa)
#define DPR_CMD_NOTSUPPORTED	DPR_CMD_COMPLETION(0xff)
#define DPR_CMD_COMPLETION_MASK	DPR_CMD_COMPLETION(0xff)
#define DPR_CMD(x)		((x) << 0)
#define DPR_CMD_GETBRDINFO	DPR_CMD(0)
#define DPR_CMD_CONFIG		DPR_CMD(1)
#define DPR_CMD_GETCONFIG	DPR_CMD(2)
#define DPR_CMD_START		DPR_CMD(3)
#define DPR_CMD_STOP		DPR_CMD(4)
#define DPR_CMD_READSINGLE	DPR_CMD(5)
#define DPR_CMD_WRITESINGLE	DPR_CMD(6)
#define DPR_CMD_CALCCLOCK	DPR_CMD(7)
#define DPR_CMD_READEVENTS	DPR_CMD(8)
#define DPR_CMD_WRITECTCTRL	DPR_CMD(16)
#define DPR_CMD_READCTCTRL	DPR_CMD(17)
#define DPR_CMD_WRITECT		DPR_CMD(18)
#define DPR_CMD_READCT		DPR_CMD(19)
#define DPR_CMD_WRITEDATA	DPR_CMD(32)
#define DPR_CMD_READDATA	DPR_CMD(33)
#define DPR_CMD_WRITEIO		DPR_CMD(34)
#define DPR_CMD_READIO		DPR_CMD(35)
#define DPR_CMD_WRITECODE	DPR_CMD(36)
#define DPR_CMD_READCODE	DPR_CMD(37)
#define DPR_CMD_EXECUTE		DPR_CMD(38)
#define DPR_CMD_HALT		DPR_CMD(48)
#define DPR_CMD_MASK		DPR_CMD(0xff)

#define DPR_PARAM5_AD_TRIG(x)		(((x) & 0x7) << 2)
#define DPR_PARAM5_AD_TRIG_INT		DPR_PARAM5_AD_TRIG(0)
#define DPR_PARAM5_AD_TRIG_EXT		DPR_PARAM5_AD_TRIG(1)
#define DPR_PARAM5_AD_TRIG_INT_RETRIG	DPR_PARAM5_AD_TRIG(2)
#define DPR_PARAM5_AD_TRIG_EXT_RETRIG	DPR_PARAM5_AD_TRIG(3)
#define DPR_PARAM5_AD_TRIG_INT_RETRIG2	DPR_PARAM5_AD_TRIG(4)

#define DPR_PARAM6_AD_DIFF		BIT(0)

#define DPR_AI_FIFO_DEPTH		2003
#define DPR_AO_FIFO_DEPTH		2048

#define DPR_EXTERNAL_CLOCK		1
#define DPR_RISING_EDGE			2

#define DPR_TMODE_MASK			0x1c

#define DPR_CMD_TIMEOUT			100
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static const struct comedi_lrange range_dt3000_ai = {
	4, {
		BIP_RANGE(10),
		BIP_RANGE(5),
		BIP_RANGE(2.5),
		BIP_RANGE(1.25)
	}
};

static const struct comedi_lrange range_dt3000_ai_pgl = {
	4, {
		BIP_RANGE(10),
		BIP_RANGE(1),
		BIP_RANGE(0.1),
		BIP_RANGE(0.02)
	}
};

enum dt3k_boardid {
	BOARD_DT3001,
	BOARD_DT3001_PGL,
	BOARD_DT3002,
	BOARD_DT3003,
	BOARD_DT3003_PGL,
	BOARD_DT3004,
	BOARD_DT3005,
};

struct dt3k_boardtype {
	const char *name;
	int adchan;
<<<<<<< HEAD
	int adbits;
	int ai_speed;
	const struct comedi_lrange *adrange;
	int dachan;
	int dabits;
=======
	int ai_speed;
	const struct comedi_lrange *adrange;
	unsigned int ai_is_16bit:1;
	unsigned int has_ao:1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static const struct dt3k_boardtype dt3k_boardtypes[] = {
	[BOARD_DT3001] = {
		.name		= "dt3001",
		.adchan		= 16,
<<<<<<< HEAD
		.adbits		= 12,
		.adrange	= &range_dt3000_ai,
		.ai_speed	= 3000,
		.dachan		= 2,
		.dabits		= 12,
=======
		.adrange	= &range_dt3000_ai,
		.ai_speed	= 3000,
		.has_ao		= 1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	[BOARD_DT3001_PGL] = {
		.name		= "dt3001-pgl",
		.adchan		= 16,
<<<<<<< HEAD
		.adbits		= 12,
		.adrange	= &range_dt3000_ai_pgl,
		.ai_speed	= 3000,
		.dachan		= 2,
		.dabits		= 12,
=======
		.adrange	= &range_dt3000_ai_pgl,
		.ai_speed	= 3000,
		.has_ao		= 1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	[BOARD_DT3002] = {
		.name		= "dt3002",
		.adchan		= 32,
<<<<<<< HEAD
		.adbits		= 12,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.adrange	= &range_dt3000_ai,
		.ai_speed	= 3000,
	},
	[BOARD_DT3003] = {
		.name		= "dt3003",
		.adchan		= 64,
<<<<<<< HEAD
		.adbits		= 12,
		.adrange	= &range_dt3000_ai,
		.ai_speed	= 3000,
		.dachan		= 2,
		.dabits		= 12,
=======
		.adrange	= &range_dt3000_ai,
		.ai_speed	= 3000,
		.has_ao		= 1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	[BOARD_DT3003_PGL] = {
		.name		= "dt3003-pgl",
		.adchan		= 64,
<<<<<<< HEAD
		.adbits		= 12,
		.adrange	= &range_dt3000_ai_pgl,
		.ai_speed	= 3000,
		.dachan		= 2,
		.dabits		= 12,
=======
		.adrange	= &range_dt3000_ai_pgl,
		.ai_speed	= 3000,
		.has_ao		= 1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	[BOARD_DT3004] = {
		.name		= "dt3004",
		.adchan		= 16,
<<<<<<< HEAD
		.adbits		= 16,
		.adrange	= &range_dt3000_ai,
		.ai_speed	= 10000,
		.dachan		= 2,
		.dabits		= 12,
=======
		.adrange	= &range_dt3000_ai,
		.ai_speed	= 10000,
		.ai_is_16bit	= 1,
		.has_ao		= 1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	[BOARD_DT3005] = {
		.name		= "dt3005",	/* a.k.a. 3004-200 */
		.adchan		= 16,
<<<<<<< HEAD
		.adbits		= 16,
		.adrange	= &range_dt3000_ai,
		.ai_speed	= 5000,
		.dachan		= 2,
		.dabits		= 12,
	},
};

/* dual-ported RAM location definitions */

#define DPR_DAC_buffer		(4*0x000)
#define DPR_ADC_buffer		(4*0x800)
#define DPR_Command		(4*0xfd3)
#define DPR_SubSys		(4*0xfd3)
#define DPR_Encode		(4*0xfd4)
#define DPR_Params(a)		(4*(0xfd5+(a)))
#define DPR_Tick_Reg_Lo		(4*0xff5)
#define DPR_Tick_Reg_Hi		(4*0xff6)
#define DPR_DA_Buf_Front	(4*0xff7)
#define DPR_DA_Buf_Rear		(4*0xff8)
#define DPR_AD_Buf_Front	(4*0xff9)
#define DPR_AD_Buf_Rear		(4*0xffa)
#define DPR_Int_Mask		(4*0xffb)
#define DPR_Intr_Flag		(4*0xffc)
#define DPR_Response_Mbx	(4*0xffe)
#define DPR_Command_Mbx		(4*0xfff)

#define AI_FIFO_DEPTH	2003
#define AO_FIFO_DEPTH	2048

/* command list */

#define CMD_GETBRDINFO		0
#define CMD_CONFIG		1
#define CMD_GETCONFIG		2
#define CMD_START		3
#define CMD_STOP		4
#define CMD_READSINGLE		5
#define CMD_WRITESINGLE		6
#define CMD_CALCCLOCK		7
#define CMD_READEVENTS		8
#define CMD_WRITECTCTRL		16
#define CMD_READCTCTRL		17
#define CMD_WRITECT		18
#define CMD_READCT		19
#define CMD_WRITEDATA		32
#define CMD_READDATA		33
#define CMD_WRITEIO		34
#define CMD_READIO		35
#define CMD_WRITECODE		36
#define CMD_READCODE		37
#define CMD_EXECUTE		38
#define CMD_HALT		48

#define SUBS_AI		0
#define SUBS_AO		1
#define SUBS_DIN	2
#define SUBS_DOUT	3
#define SUBS_MEM	4
#define SUBS_CT		5

/* interrupt flags */
#define DT3000_CMDONE		0x80
#define DT3000_CTDONE		0x40
#define DT3000_DAHWERR		0x20
#define DT3000_DASWERR		0x10
#define DT3000_DAEMPTY		0x08
#define DT3000_ADHWERR		0x04
#define DT3000_ADSWERR		0x02
#define DT3000_ADFULL		0x01

#define DT3000_COMPLETION_MASK	0xff00
#define DT3000_COMMAND_MASK	0x00ff
#define DT3000_NOTPROCESSED	0x0000
#define DT3000_NOERROR		0x5500
#define DT3000_ERROR		0xaa00
#define DT3000_NOTSUPPORTED	0xff00

#define DT3000_EXTERNAL_CLOCK	1
#define DT3000_RISING_EDGE	2

#define TMODE_MASK		0x1c

#define DT3000_AD_TRIG_INTERNAL		(0<<2)
#define DT3000_AD_TRIG_EXTERNAL		(1<<2)
#define DT3000_AD_RETRIG_INTERNAL	(2<<2)
#define DT3000_AD_RETRIG_EXTERNAL	(3<<2)
#define DT3000_AD_EXTRETRIG		(4<<2)

#define DT3000_CHANNEL_MODE_SE		0
#define DT3000_CHANNEL_MODE_DI		1

struct dt3k_private {
	void __iomem *io_addr;
	unsigned int lock;
	unsigned int ao_readback[2];
=======
		.adrange	= &range_dt3000_ai,
		.ai_speed	= 5000,
		.ai_is_16bit	= 1,
		.has_ao		= 1,
	},
};

struct dt3k_private {
	unsigned int lock;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int ai_front;
	unsigned int ai_rear;
};

<<<<<<< HEAD
#ifdef DEBUG
static char *intr_flags[] = {
	"AdFull", "AdSwError", "AdHwError", "DaEmpty",
	"DaSwError", "DaHwError", "CtDone", "CmDone",
};

static void debug_intr_flags(unsigned int flags)
{
	int i;
	printk(KERN_DEBUG "dt3k: intr_flags:");
	for (i = 0; i < 8; i++) {
		if (flags & (1 << i))
			printk(KERN_CONT " %s", intr_flags[i]);
	}
	printk(KERN_CONT "\n");
}
#endif

#define TIMEOUT 100

static void dt3k_send_cmd(struct comedi_device *dev, unsigned int cmd)
{
	struct dt3k_private *devpriv = dev->private;
	int i;
	unsigned int status = 0;

	writew(cmd, devpriv->io_addr + DPR_Command_Mbx);

	for (i = 0; i < TIMEOUT; i++) {
		status = readw(devpriv->io_addr + DPR_Command_Mbx);
		if ((status & DT3000_COMPLETION_MASK) != DT3000_NOTPROCESSED)
=======
static void dt3k_send_cmd(struct comedi_device *dev, unsigned int cmd)
{
	int i;
	unsigned int status = 0;

	writew(cmd, dev->mmio + DPR_CMD_MBX);

	for (i = 0; i < DPR_CMD_TIMEOUT; i++) {
		status = readw(dev->mmio + DPR_CMD_MBX);
		status &= DPR_CMD_COMPLETION_MASK;
		if (status != DPR_CMD_NOTPROCESSED)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		udelay(1);
	}

<<<<<<< HEAD
	if ((status & DT3000_COMPLETION_MASK) != DT3000_NOERROR)
=======
	if (status != DPR_CMD_NOERROR)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_dbg(dev->class_dev, "%s: timeout/error status=0x%04x\n",
			__func__, status);
}

static unsigned int dt3k_readsingle(struct comedi_device *dev,
				    unsigned int subsys, unsigned int chan,
				    unsigned int gain)
{
<<<<<<< HEAD
	struct dt3k_private *devpriv = dev->private;

	writew(subsys, devpriv->io_addr + DPR_SubSys);

	writew(chan, devpriv->io_addr + DPR_Params(0));
	writew(gain, devpriv->io_addr + DPR_Params(1));

	dt3k_send_cmd(dev, CMD_READSINGLE);

	return readw(devpriv->io_addr + DPR_Params(2));
=======
	writew(subsys, dev->mmio + DPR_SUBSYS);

	writew(chan, dev->mmio + DPR_PARAMS(0));
	writew(gain, dev->mmio + DPR_PARAMS(1));

	dt3k_send_cmd(dev, DPR_CMD_READSINGLE);

	return readw(dev->mmio + DPR_PARAMS(2));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void dt3k_writesingle(struct comedi_device *dev, unsigned int subsys,
			     unsigned int chan, unsigned int data)
{
<<<<<<< HEAD
	struct dt3k_private *devpriv = dev->private;

	writew(subsys, devpriv->io_addr + DPR_SubSys);

	writew(chan, devpriv->io_addr + DPR_Params(0));
	writew(0, devpriv->io_addr + DPR_Params(1));
	writew(data, devpriv->io_addr + DPR_Params(2));

	dt3k_send_cmd(dev, CMD_WRITESINGLE);
=======
	writew(subsys, dev->mmio + DPR_SUBSYS);

	writew(chan, dev->mmio + DPR_PARAMS(0));
	writew(0, dev->mmio + DPR_PARAMS(1));
	writew(data, dev->mmio + DPR_PARAMS(2));

	dt3k_send_cmd(dev, DPR_CMD_WRITESINGLE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void dt3k_ai_empty_fifo(struct comedi_device *dev,
			       struct comedi_subdevice *s)
{
	struct dt3k_private *devpriv = dev->private;
	int front;
	int rear;
	int count;
	int i;
<<<<<<< HEAD
	short data;

	front = readw(devpriv->io_addr + DPR_AD_Buf_Front);
	count = front - devpriv->ai_front;
	if (count < 0)
		count += AI_FIFO_DEPTH;
=======
	unsigned short data;

	front = readw(dev->mmio + DPR_AD_BUF_FRONT);
	count = front - devpriv->ai_front;
	if (count < 0)
		count += DPR_AI_FIFO_DEPTH;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rear = devpriv->ai_rear;

	for (i = 0; i < count; i++) {
<<<<<<< HEAD
		data = readw(devpriv->io_addr + DPR_ADC_buffer + rear);
		comedi_buf_put(s->async, data);
		rear++;
		if (rear >= AI_FIFO_DEPTH)
=======
		data = readw(dev->mmio + DPR_ADC_BUFFER + rear);
		comedi_buf_write_samples(s, &data, 1);
		rear++;
		if (rear >= DPR_AI_FIFO_DEPTH)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			rear = 0;
	}

	devpriv->ai_rear = rear;
<<<<<<< HEAD
	writew(rear, devpriv->io_addr + DPR_AD_Buf_Rear);
}

static int dt3k_ai_cancel(struct comedi_device *dev, struct comedi_subdevice *s)
{
	struct dt3k_private *devpriv = dev->private;

	writew(SUBS_AI, devpriv->io_addr + DPR_SubSys);
	dt3k_send_cmd(dev, CMD_STOP);

	writew(0, devpriv->io_addr + DPR_Int_Mask);
=======
	writew(rear, dev->mmio + DPR_AD_BUF_REAR);
}

static int dt3k_ai_cancel(struct comedi_device *dev,
			  struct comedi_subdevice *s)
{
	writew(DPR_SUBSYS_AI, dev->mmio + DPR_SUBSYS);
	dt3k_send_cmd(dev, DPR_CMD_STOP);

	writew(0, dev->mmio + DPR_INT_MASK);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static int debug_n_ints;

/* FIXME! Assumes shared interrupt is for this card. */
/* What's this debug_n_ints stuff? Obviously needs some work... */
static irqreturn_t dt3k_interrupt(int irq, void *d)
{
	struct comedi_device *dev = d;
<<<<<<< HEAD
	struct dt3k_private *devpriv = dev->private;
	struct comedi_subdevice *s;
=======
	struct comedi_subdevice *s = dev->read_subdev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int status;

	if (!dev->attached)
		return IRQ_NONE;

<<<<<<< HEAD
	s = &dev->subdevices[0];
	status = readw(devpriv->io_addr + DPR_Intr_Flag);
#ifdef DEBUG
	debug_intr_flags(status);
#endif

	if (status & DT3000_ADFULL) {
		dt3k_ai_empty_fifo(dev, s);
		s->async->events |= COMEDI_CB_BLOCK;
	}

	if (status & (DT3000_ADSWERR | DT3000_ADHWERR))
		s->async->events |= COMEDI_CB_ERROR | COMEDI_CB_EOA;

	debug_n_ints++;
	if (debug_n_ints >= 10) {
		dt3k_ai_cancel(dev, s);
		s->async->events |= COMEDI_CB_EOA;
	}

	comedi_event(dev, s);
=======
	status = readw(dev->mmio + DPR_INTR_FLAG);

	if (status & DPR_INTR_ADFULL)
		dt3k_ai_empty_fifo(dev, s);

	if (status & (DPR_INTR_ADSWERR | DPR_INTR_ADHWERR))
		s->async->events |= COMEDI_CB_ERROR;

	debug_n_ints++;
	if (debug_n_ints >= 10)
		s->async->events |= COMEDI_CB_EOA;

	comedi_handle_events(dev, s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return IRQ_HANDLED;
}

static int dt3k_ns_to_timer(unsigned int timer_base, unsigned int *nanosec,
<<<<<<< HEAD
			    unsigned int round_mode)
{
	int divider, base, prescale;

	/* This function needs improvment */
=======
			    unsigned int flags)
{
	unsigned int divider, base, prescale;

	/* This function needs improvement */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Don't know if divider==0 works. */

	for (prescale = 0; prescale < 16; prescale++) {
		base = timer_base * (prescale + 1);
<<<<<<< HEAD
		switch (round_mode) {
		case TRIG_ROUND_NEAREST:
		default:
			divider = (*nanosec + base / 2) / base;
			break;
		case TRIG_ROUND_DOWN:
			divider = (*nanosec) / base;
			break;
		case TRIG_ROUND_UP:
			divider = (*nanosec) / base;
=======
		switch (flags & CMDF_ROUND_MASK) {
		case CMDF_ROUND_NEAREST:
		default:
			divider = DIV_ROUND_CLOSEST(*nanosec, base);
			break;
		case CMDF_ROUND_DOWN:
			divider = (*nanosec) / base;
			break;
		case CMDF_ROUND_UP:
			divider = DIV_ROUND_UP(*nanosec, base);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		}
		if (divider < 65536) {
			*nanosec = divider * base;
			return (prescale << 16) | (divider);
		}
	}

	prescale = 15;
<<<<<<< HEAD
	base = timer_base * (1 << prescale);
=======
	base = timer_base * (prescale + 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	divider = 65535;
	*nanosec = divider * base;
	return (prescale << 16) | (divider);
}

static int dt3k_ai_cmdtest(struct comedi_device *dev,
			   struct comedi_subdevice *s, struct comedi_cmd *cmd)
{
<<<<<<< HEAD
	const struct dt3k_boardtype *this_board = comedi_board(dev);
	int err = 0;
	int tmp;

	/* Step 1 : check if triggers are trivially valid */

	err |= cfc_check_trigger_src(&cmd->start_src, TRIG_NOW);
	err |= cfc_check_trigger_src(&cmd->scan_begin_src, TRIG_TIMER);
	err |= cfc_check_trigger_src(&cmd->convert_src, TRIG_TIMER);
	err |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= cfc_check_trigger_src(&cmd->stop_src, TRIG_COUNT);
=======
	const struct dt3k_boardtype *board = dev->board_ptr;
	int err = 0;
	unsigned int arg;

	/* Step 1 : check if triggers are trivially valid */

	err |= comedi_check_trigger_src(&cmd->start_src, TRIG_NOW);
	err |= comedi_check_trigger_src(&cmd->scan_begin_src, TRIG_TIMER);
	err |= comedi_check_trigger_src(&cmd->convert_src, TRIG_TIMER);
	err |= comedi_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= comedi_check_trigger_src(&cmd->stop_src, TRIG_COUNT);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (err)
		return 1;

	/* Step 2a : make sure trigger sources are unique */
	/* Step 2b : and mutually compatible */

<<<<<<< HEAD
	if (err)
		return 2;

	/* Step 3: check if arguments are trivially valid */

	err |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);

	if (cmd->scan_begin_src == TRIG_TIMER) {
		err |= cfc_check_trigger_arg_min(&cmd->scan_begin_arg,
						 this_board->ai_speed);
		err |= cfc_check_trigger_arg_max(&cmd->scan_begin_arg,
						 100 * 16 * 65535);
	}

	if (cmd->convert_src == TRIG_TIMER) {
		err |= cfc_check_trigger_arg_min(&cmd->convert_arg,
						 this_board->ai_speed);
		err |= cfc_check_trigger_arg_max(&cmd->convert_arg,
						 50 * 16 * 65535);
	}

	err |= cfc_check_trigger_arg_is(&cmd->scan_end_arg, cmd->chanlist_len);

	if (cmd->stop_src == TRIG_COUNT)
		err |= cfc_check_trigger_arg_max(&cmd->stop_arg, 0x00ffffff);
	else	/* TRIG_NONE */
		err |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);
=======
	/* Step 3: check if arguments are trivially valid */

	err |= comedi_check_trigger_arg_is(&cmd->start_arg, 0);

	if (cmd->scan_begin_src == TRIG_TIMER) {
		err |= comedi_check_trigger_arg_min(&cmd->scan_begin_arg,
						    board->ai_speed);
		err |= comedi_check_trigger_arg_max(&cmd->scan_begin_arg,
						    100 * 16 * 65535);
	}

	if (cmd->convert_src == TRIG_TIMER) {
		err |= comedi_check_trigger_arg_min(&cmd->convert_arg,
						    board->ai_speed);
		err |= comedi_check_trigger_arg_max(&cmd->convert_arg,
						    50 * 16 * 65535);
	}

	err |= comedi_check_trigger_arg_is(&cmd->scan_end_arg,
					   cmd->chanlist_len);

	if (cmd->stop_src == TRIG_COUNT)
		err |= comedi_check_trigger_arg_max(&cmd->stop_arg, 0x00ffffff);
	else	/* TRIG_NONE */
		err |= comedi_check_trigger_arg_is(&cmd->stop_arg, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (err)
		return 3;

	/* step 4: fix up any arguments */

	if (cmd->scan_begin_src == TRIG_TIMER) {
<<<<<<< HEAD
		tmp = cmd->scan_begin_arg;
		dt3k_ns_to_timer(100, &cmd->scan_begin_arg,
				 cmd->flags & TRIG_ROUND_MASK);
		if (tmp != cmd->scan_begin_arg)
			err++;
	}

	if (cmd->convert_src == TRIG_TIMER) {
		tmp = cmd->convert_arg;
		dt3k_ns_to_timer(50, &cmd->convert_arg,
				 cmd->flags & TRIG_ROUND_MASK);
		if (tmp != cmd->convert_arg)
			err++;
		if (cmd->scan_begin_src == TRIG_TIMER &&
		    cmd->scan_begin_arg <
		    cmd->convert_arg * cmd->scan_end_arg) {
			cmd->scan_begin_arg =
			    cmd->convert_arg * cmd->scan_end_arg;
			err++;
=======
		arg = cmd->scan_begin_arg;
		dt3k_ns_to_timer(100, &arg, cmd->flags);
		err |= comedi_check_trigger_arg_is(&cmd->scan_begin_arg, arg);
	}

	if (cmd->convert_src == TRIG_TIMER) {
		arg = cmd->convert_arg;
		dt3k_ns_to_timer(50, &arg, cmd->flags);
		err |= comedi_check_trigger_arg_is(&cmd->convert_arg, arg);

		if (cmd->scan_begin_src == TRIG_TIMER) {
			arg = cmd->convert_arg * cmd->scan_end_arg;
			err |= comedi_check_trigger_arg_min(&cmd->
							    scan_begin_arg,
							    arg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	if (err)
		return 4;

	return 0;
}

static int dt3k_ai_cmd(struct comedi_device *dev, struct comedi_subdevice *s)
{
<<<<<<< HEAD
	struct dt3k_private *devpriv = dev->private;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct comedi_cmd *cmd = &s->async->cmd;
	int i;
	unsigned int chan, range, aref;
	unsigned int divider;
	unsigned int tscandiv;
<<<<<<< HEAD
	unsigned int mode;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (i = 0; i < cmd->chanlist_len; i++) {
		chan = CR_CHAN(cmd->chanlist[i]);
		range = CR_RANGE(cmd->chanlist[i]);

<<<<<<< HEAD
		writew((range << 6) | chan,
		       devpriv->io_addr + DPR_ADC_buffer + i);
	}
	aref = CR_AREF(cmd->chanlist[0]);

	writew(cmd->scan_end_arg, devpriv->io_addr + DPR_Params(0));

	if (cmd->convert_src == TRIG_TIMER) {
		divider = dt3k_ns_to_timer(50, &cmd->convert_arg,
					   cmd->flags & TRIG_ROUND_MASK);
		writew((divider >> 16), devpriv->io_addr + DPR_Params(1));
		writew((divider & 0xffff), devpriv->io_addr + DPR_Params(2));
=======
		writew((range << 6) | chan, dev->mmio + DPR_ADC_BUFFER + i);
	}
	aref = CR_AREF(cmd->chanlist[0]);

	writew(cmd->scan_end_arg, dev->mmio + DPR_PARAMS(0));

	if (cmd->convert_src == TRIG_TIMER) {
		divider = dt3k_ns_to_timer(50, &cmd->convert_arg, cmd->flags);
		writew((divider >> 16), dev->mmio + DPR_PARAMS(1));
		writew((divider & 0xffff), dev->mmio + DPR_PARAMS(2));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (cmd->scan_begin_src == TRIG_TIMER) {
		tscandiv = dt3k_ns_to_timer(100, &cmd->scan_begin_arg,
<<<<<<< HEAD
					    cmd->flags & TRIG_ROUND_MASK);
		writew((tscandiv >> 16), devpriv->io_addr + DPR_Params(3));
		writew((tscandiv & 0xffff), devpriv->io_addr + DPR_Params(4));
	}

	mode = DT3000_AD_RETRIG_INTERNAL | 0 | 0;
	writew(mode, devpriv->io_addr + DPR_Params(5));
	writew(aref == AREF_DIFF, devpriv->io_addr + DPR_Params(6));

	writew(AI_FIFO_DEPTH / 2, devpriv->io_addr + DPR_Params(7));

	writew(SUBS_AI, devpriv->io_addr + DPR_SubSys);
	dt3k_send_cmd(dev, CMD_CONFIG);

	writew(DT3000_ADFULL | DT3000_ADSWERR | DT3000_ADHWERR,
	       devpriv->io_addr + DPR_Int_Mask);

	debug_n_ints = 0;

	writew(SUBS_AI, devpriv->io_addr + DPR_SubSys);
	dt3k_send_cmd(dev, CMD_START);
=======
					    cmd->flags);
		writew((tscandiv >> 16), dev->mmio + DPR_PARAMS(3));
		writew((tscandiv & 0xffff), dev->mmio + DPR_PARAMS(4));
	}

	writew(DPR_PARAM5_AD_TRIG_INT_RETRIG, dev->mmio + DPR_PARAMS(5));
	writew((aref == AREF_DIFF) ? DPR_PARAM6_AD_DIFF : 0,
	       dev->mmio + DPR_PARAMS(6));

	writew(DPR_AI_FIFO_DEPTH / 2, dev->mmio + DPR_PARAMS(7));

	writew(DPR_SUBSYS_AI, dev->mmio + DPR_SUBSYS);
	dt3k_send_cmd(dev, DPR_CMD_CONFIG);

	writew(DPR_INTR_ADFULL | DPR_INTR_ADSWERR | DPR_INTR_ADHWERR,
	       dev->mmio + DPR_INT_MASK);

	debug_n_ints = 0;

	writew(DPR_SUBSYS_AI, dev->mmio + DPR_SUBSYS);
	dt3k_send_cmd(dev, DPR_CMD_START);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static int dt3k_ai_insn(struct comedi_device *dev, struct comedi_subdevice *s,
			struct comedi_insn *insn, unsigned int *data)
=======
static int dt3k_ai_insn_read(struct comedi_device *dev,
			     struct comedi_subdevice *s,
			     struct comedi_insn *insn,
			     unsigned int *data)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int i;
	unsigned int chan, gain, aref;

	chan = CR_CHAN(insn->chanspec);
	gain = CR_RANGE(insn->chanspec);
	/* XXX docs don't explain how to select aref */
	aref = CR_AREF(insn->chanspec);

	for (i = 0; i < insn->n; i++)
<<<<<<< HEAD
		data[i] = dt3k_readsingle(dev, SUBS_AI, chan, gain);
=======
		data[i] = dt3k_readsingle(dev, DPR_SUBSYS_AI, chan, gain);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return i;
}

<<<<<<< HEAD
static int dt3k_ao_insn(struct comedi_device *dev, struct comedi_subdevice *s,
			struct comedi_insn *insn, unsigned int *data)
{
	struct dt3k_private *devpriv = dev->private;
	int i;
	unsigned int chan;

	chan = CR_CHAN(insn->chanspec);
	for (i = 0; i < insn->n; i++) {
		dt3k_writesingle(dev, SUBS_AO, chan, data[i]);
		devpriv->ao_readback[chan] = data[i];
	}

	return i;
}

static int dt3k_ao_insn_read(struct comedi_device *dev,
			     struct comedi_subdevice *s,
			     struct comedi_insn *insn, unsigned int *data)
{
	struct dt3k_private *devpriv = dev->private;
	int i;
	unsigned int chan;

	chan = CR_CHAN(insn->chanspec);
	for (i = 0; i < insn->n; i++)
		data[i] = devpriv->ao_readback[chan];

	return i;
=======
static int dt3k_ao_insn_write(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn,
			      unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int val = s->readback[chan];
	int i;

	for (i = 0; i < insn->n; i++) {
		val = data[i];
		dt3k_writesingle(dev, DPR_SUBSYS_AO, chan, val);
	}
	s->readback[chan] = val;

	return insn->n;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void dt3k_dio_config(struct comedi_device *dev, int bits)
{
<<<<<<< HEAD
	struct dt3k_private *devpriv = dev->private;

	/* XXX */
	writew(SUBS_DOUT, devpriv->io_addr + DPR_SubSys);

	writew(bits, devpriv->io_addr + DPR_Params(0));
#if 0
	/* don't know */
	writew(0, devpriv->io_addr + DPR_Params(1));
	writew(0, devpriv->io_addr + DPR_Params(2));
#endif

	dt3k_send_cmd(dev, CMD_CONFIG);
=======
	/* XXX */
	writew(DPR_SUBSYS_DOUT, dev->mmio + DPR_SUBSYS);

	writew(bits, dev->mmio + DPR_PARAMS(0));

	/* XXX write 0 to DPR_PARAMS(1) and DPR_PARAMS(2) ? */

	dt3k_send_cmd(dev, DPR_CMD_CONFIG);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int dt3k_dio_insn_config(struct comedi_device *dev,
				struct comedi_subdevice *s,
<<<<<<< HEAD
				struct comedi_insn *insn, unsigned int *data)
{
	int mask;

	mask = (CR_CHAN(insn->chanspec) < 4) ? 0x0f : 0xf0;

	switch (data[0]) {
	case INSN_CONFIG_DIO_OUTPUT:
		s->io_bits |= mask;
		break;
	case INSN_CONFIG_DIO_INPUT:
		s->io_bits &= ~mask;
		break;
	case INSN_CONFIG_DIO_QUERY:
		data[1] =
		    (s->
		     io_bits & (1 << CR_CHAN(insn->chanspec))) ? COMEDI_OUTPUT :
		    COMEDI_INPUT;
		return insn->n;
		break;
	default:
		return -EINVAL;
		break;
	}
	mask = (s->io_bits & 0x01) | ((s->io_bits & 0x10) >> 3);
	dt3k_dio_config(dev, mask);
=======
				struct comedi_insn *insn,
				unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int mask;
	int ret;

	if (chan < 4)
		mask = 0x0f;
	else
		mask = 0xf0;

	ret = comedi_dio_insn_config(dev, s, insn, data, mask);
	if (ret)
		return ret;

	dt3k_dio_config(dev, (s->io_bits & 0x01) | ((s->io_bits & 0x10) >> 3));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return insn->n;
}

static int dt3k_dio_insn_bits(struct comedi_device *dev,
			      struct comedi_subdevice *s,
<<<<<<< HEAD
			      struct comedi_insn *insn, unsigned int *data)
{
	if (data[0]) {
		s->state &= ~data[0];
		s->state |= data[1] & data[0];
		dt3k_writesingle(dev, SUBS_DOUT, 0, s->state);
	}
	data[1] = dt3k_readsingle(dev, SUBS_DIN, 0, 0);
=======
			      struct comedi_insn *insn,
			      unsigned int *data)
{
	if (comedi_dio_update_state(s, data))
		dt3k_writesingle(dev, DPR_SUBSYS_DOUT, 0, s->state);

	data[1] = dt3k_readsingle(dev, DPR_SUBSYS_DIN, 0, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return insn->n;
}

static int dt3k_mem_insn_read(struct comedi_device *dev,
			      struct comedi_subdevice *s,
<<<<<<< HEAD
			      struct comedi_insn *insn, unsigned int *data)
{
	struct dt3k_private *devpriv = dev->private;
=======
			      struct comedi_insn *insn,
			      unsigned int *data)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int addr = CR_CHAN(insn->chanspec);
	int i;

	for (i = 0; i < insn->n; i++) {
<<<<<<< HEAD
		writew(SUBS_MEM, devpriv->io_addr + DPR_SubSys);
		writew(addr, devpriv->io_addr + DPR_Params(0));
		writew(1, devpriv->io_addr + DPR_Params(1));

		dt3k_send_cmd(dev, CMD_READCODE);

		data[i] = readw(devpriv->io_addr + DPR_Params(2));
=======
		writew(DPR_SUBSYS_MEM, dev->mmio + DPR_SUBSYS);
		writew(addr, dev->mmio + DPR_PARAMS(0));
		writew(1, dev->mmio + DPR_PARAMS(1));

		dt3k_send_cmd(dev, DPR_CMD_READCODE);

		data[i] = readw(dev->mmio + DPR_PARAMS(2));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return i;
}

static int dt3000_auto_attach(struct comedi_device *dev,
			      unsigned long context)
{
	struct pci_dev *pcidev = comedi_to_pci_dev(dev);
<<<<<<< HEAD
	const struct dt3k_boardtype *this_board = NULL;
=======
	const struct dt3k_boardtype *board = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct dt3k_private *devpriv;
	struct comedi_subdevice *s;
	int ret = 0;

	if (context < ARRAY_SIZE(dt3k_boardtypes))
<<<<<<< HEAD
		this_board = &dt3k_boardtypes[context];
	if (!this_board)
		return -ENODEV;
	dev->board_ptr = this_board;
	dev->board_name = this_board->name;

	devpriv = kzalloc(sizeof(*devpriv), GFP_KERNEL);
	if (!devpriv)
		return -ENOMEM;
	dev->private = devpriv;
=======
		board = &dt3k_boardtypes[context];
	if (!board)
		return -ENODEV;
	dev->board_ptr = board;
	dev->board_name = board->name;

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv)
		return -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = comedi_pci_enable(dev);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	devpriv->io_addr = pci_ioremap_bar(pcidev, 0);
	if (!devpriv->io_addr)
		return -ENOMEM;

	ret = request_irq(pcidev->irq, dt3k_interrupt, IRQF_SHARED,
			  dev->board_name, dev);
	if (ret)
		return ret;
	dev->irq = pcidev->irq;
=======
	dev->mmio = pci_ioremap_bar(pcidev, 0);
	if (!dev->mmio)
		return -ENOMEM;

	if (pcidev->irq) {
		ret = request_irq(pcidev->irq, dt3k_interrupt, IRQF_SHARED,
				  dev->board_name, dev);
		if (ret == 0)
			dev->irq = pcidev->irq;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = comedi_alloc_subdevices(dev, 4);
	if (ret)
		return ret;

<<<<<<< HEAD
	s = &dev->subdevices[0];
	dev->read_subdev = s;
	/* ai subdevice */
	s->type		= COMEDI_SUBD_AI;
	s->subdev_flags	= SDF_READABLE | SDF_GROUND | SDF_DIFF | SDF_CMD_READ;
	s->n_chan	= this_board->adchan;
	s->insn_read	= dt3k_ai_insn;
	s->maxdata	= (1 << this_board->adbits) - 1;
	s->len_chanlist	= 512;
	s->range_table	= &range_dt3000_ai;	/* XXX */
	s->do_cmd	= dt3k_ai_cmd;
	s->do_cmdtest	= dt3k_ai_cmdtest;
	s->cancel	= dt3k_ai_cancel;

	s = &dev->subdevices[1];
	/* ao subsystem */
	s->type		= COMEDI_SUBD_AO;
	s->subdev_flags	= SDF_WRITABLE;
	s->n_chan	= 2;
	s->insn_read	= dt3k_ao_insn_read;
	s->insn_write	= dt3k_ao_insn;
	s->maxdata	= (1 << this_board->dabits) - 1;
	s->len_chanlist	= 1;
	s->range_table	= &range_bipolar10;

	s = &dev->subdevices[2];
	/* dio subsystem */
	s->type		= COMEDI_SUBD_DIO;
	s->subdev_flags	= SDF_READABLE | SDF_WRITABLE;
	s->n_chan	= 8;
	s->insn_config	= dt3k_dio_insn_config;
	s->insn_bits	= dt3k_dio_insn_bits;
	s->maxdata	= 1;
	s->len_chanlist	= 8;
	s->range_table	= &range_digital;

	s = &dev->subdevices[3];
	/* mem subsystem */
	s->type		= COMEDI_SUBD_MEMORY;
	s->subdev_flags	= SDF_READABLE;
	s->n_chan	= 0x1000;
	s->insn_read	= dt3k_mem_insn_read;
	s->maxdata	= 0xff;
	s->len_chanlist	= 1;
	s->range_table	= &range_unknown;

#if 0
	s = &dev->subdevices[4];
	/* proc subsystem */
	s->type = COMEDI_SUBD_PROC;
#endif

	dev_info(dev->class_dev, "%s attached\n", dev->board_name);
=======
	/* Analog Input subdevice */
	s = &dev->subdevices[0];
	s->type		= COMEDI_SUBD_AI;
	s->subdev_flags	= SDF_READABLE | SDF_GROUND | SDF_DIFF;
	s->n_chan	= board->adchan;
	s->maxdata	= board->ai_is_16bit ? 0xffff : 0x0fff;
	s->range_table	= &range_dt3000_ai;	/* XXX */
	s->insn_read	= dt3k_ai_insn_read;
	if (dev->irq) {
		dev->read_subdev = s;
		s->subdev_flags	|= SDF_CMD_READ;
		s->len_chanlist	= 512;
		s->do_cmd	= dt3k_ai_cmd;
		s->do_cmdtest	= dt3k_ai_cmdtest;
		s->cancel	= dt3k_ai_cancel;
	}

	/* Analog Output subdevice */
	s = &dev->subdevices[1];
	if (board->has_ao) {
		s->type		= COMEDI_SUBD_AO;
		s->subdev_flags	= SDF_WRITABLE;
		s->n_chan	= 2;
		s->maxdata	= 0x0fff;
		s->range_table	= &range_bipolar10;
		s->insn_write	= dt3k_ao_insn_write;

		ret = comedi_alloc_subdev_readback(s);
		if (ret)
			return ret;

	} else {
		s->type		= COMEDI_SUBD_UNUSED;
	}

	/* Digital I/O subdevice */
	s = &dev->subdevices[2];
	s->type		= COMEDI_SUBD_DIO;
	s->subdev_flags	= SDF_READABLE | SDF_WRITABLE;
	s->n_chan	= 8;
	s->maxdata	= 1;
	s->range_table	= &range_digital;
	s->insn_config	= dt3k_dio_insn_config;
	s->insn_bits	= dt3k_dio_insn_bits;

	/* Memory subdevice */
	s = &dev->subdevices[3];
	s->type		= COMEDI_SUBD_MEMORY;
	s->subdev_flags	= SDF_READABLE;
	s->n_chan	= 0x1000;
	s->maxdata	= 0xff;
	s->range_table	= &range_unknown;
	s->insn_read	= dt3k_mem_insn_read;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static void dt3000_detach(struct comedi_device *dev)
{
	struct dt3k_private *devpriv = dev->private;

	if (dev->irq)
		free_irq(dev->irq, dev);
	if (devpriv) {
		if (devpriv->io_addr)
			iounmap(devpriv->io_addr);
	}
	comedi_pci_disable(dev);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct comedi_driver dt3000_driver = {
	.driver_name	= "dt3000",
	.module		= THIS_MODULE,
	.auto_attach	= dt3000_auto_attach,
<<<<<<< HEAD
	.detach		= dt3000_detach,
=======
	.detach		= comedi_pci_detach,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int dt3000_pci_probe(struct pci_dev *dev,
			    const struct pci_device_id *id)
{
	return comedi_pci_auto_config(dev, &dt3000_driver, id->driver_data);
}

<<<<<<< HEAD
static DEFINE_PCI_DEVICE_TABLE(dt3000_pci_table) = {
=======
static const struct pci_device_id dt3000_pci_table[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ PCI_VDEVICE(DT, 0x0022), BOARD_DT3001 },
	{ PCI_VDEVICE(DT, 0x0023), BOARD_DT3002 },
	{ PCI_VDEVICE(DT, 0x0024), BOARD_DT3003 },
	{ PCI_VDEVICE(DT, 0x0025), BOARD_DT3004 },
	{ PCI_VDEVICE(DT, 0x0026), BOARD_DT3005 },
	{ PCI_VDEVICE(DT, 0x0027), BOARD_DT3001_PGL },
	{ PCI_VDEVICE(DT, 0x0028), BOARD_DT3003_PGL },
	{ 0 }
};
MODULE_DEVICE_TABLE(pci, dt3000_pci_table);

static struct pci_driver dt3000_pci_driver = {
	.name		= "dt3000",
	.id_table	= dt3000_pci_table,
	.probe		= dt3000_pci_probe,
	.remove		= comedi_pci_auto_unconfig,
};
module_comedi_pci_driver(dt3000_driver, dt3000_pci_driver);

MODULE_AUTHOR("Comedi http://www.comedi.org");
<<<<<<< HEAD
MODULE_DESCRIPTION("Comedi low-level driver");
=======
MODULE_DESCRIPTION("Comedi driver for Data Translation DT3000 series boards");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_LICENSE("GPL");
