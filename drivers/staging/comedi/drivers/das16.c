/*
<<<<<<< HEAD
    comedi/drivers/das16.c
    DAS16 driver

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2000 David A. Schleef <ds@schleef.org>
    Copyright (C) 2000 Chris R. Baugher <baugher@enteract.com>
    Copyright (C) 2001,2002 Frank Mori Hess <fmhess@users.sourceforge.net>

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

************************************************************************
*/
/*
Driver: das16
Description: DAS16 compatible boards
Author: Sam Moore, Warren Jasper, ds, Chris Baugher, Frank Hess, Roman Fietze
Devices: [Keithley Metrabyte] DAS-16 (das-16), DAS-16G (das-16g),
  DAS-16F (das-16f), DAS-1201 (das-1201), DAS-1202 (das-1202),
  DAS-1401 (das-1401), DAS-1402 (das-1402), DAS-1601 (das-1601),
  DAS-1602 (das-1602),
  [ComputerBoards] PC104-DAS16/JR (pc104-das16jr),
  PC104-DAS16JR/16 (pc104-das16jr/16),
  CIO-DAS16JR/16 (cio-das16jr/16),
  CIO-DAS16/JR (cio-das16/jr), CIO-DAS1401/12 (cio-das1401/12),
  CIO-DAS1402/12 (cio-das1402/12), CIO-DAS1402/16 (cio-das1402/16),
  CIO-DAS1601/12 (cio-das1601/12), CIO-DAS1602/12 (cio-das1602/12),
  CIO-DAS1602/16 (cio-das1602/16), CIO-DAS16/330 (cio-das16/330)
Status: works
Updated: 2003-10-12

A rewrite of the das16 and das1600 drivers.
Options:
	[0] - base io address
	[1] - irq (does nothing, irq is not used anymore)
	[2] - dma (optional, required for comedi_command support)
	[3] - master clock speed in MHz (optional, 1 or 10, ignored if
		board can probe clock, defaults to 1)
	[4] - analog input range lowest voltage in microvolts (optional,
		only useful if your board does not have software
		programmable gain)
	[5] - analog input range highest voltage in microvolts (optional,
		only useful if board does not have software programmable
		gain)
	[6] - analog output range lowest voltage in microvolts (optional)
	[7] - analog output range highest voltage in microvolts (optional)
	[8] - use timer mode for DMA.  Timer mode is needed e.g. for
		buggy DMA controllers in NS CS5530A (Geode Companion), and for
		'jr' cards that lack a hardware fifo.  This option is no
		longer needed, since timer mode is _always_ used.

Passing a zero for an option is the same as leaving it unspecified.

*/
/*

Testing and debugging help provided by Daniel Koch.

Keithley Manuals:
	2309.PDF (das16)
	4919.PDF (das1400, 1600)
	4922.PDF (das-1400)
	4923.PDF (das1200, 1400, 1600)

Computer boards manuals also available from their website
www.measurementcomputing.com

*/

#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/interrupt.h>

#include <asm/dma.h>

#include "../comedidev.h"

#include "8253.h"
#include "8255.h"
#include "comedi_fc.h"

#undef DEBUG
/* #define DEBUG */

#ifdef DEBUG
#define DEBUG_PRINT(format, args...)	\
	printk(KERN_DEBUG "das16: " format, ## args)
#else
#define DEBUG_PRINT(format, args...)
#endif

#define DAS16_SIZE 20		/*  number of ioports */
#define DAS16_DMA_SIZE 0xff00	/*  size in bytes of allocated dma buffer */

/*
    cio-das16.pdf

    "das16"
    "das16/f"

  0	a/d bits 0-3		start 12 bit
  1	a/d bits 4-11		unused
  2	mux read		mux set
  3	di 4 bit		do 4 bit
  4	unused			ao0_lsb
  5	unused			ao0_msb
  6	unused			ao1_lsb
  7	unused			ao1_msb
  8	status eoc uni/bip	interrupt reset
  9	dma, int, trig ctrl	set dma, int
  a	pacer control		unused
  b	reserved		reserved
  cdef	8254
  0123	8255

*/

/*
    cio-das16jr.pdf

    "das16jr"

  0	a/d bits 0-3		start 12 bit
  1	a/d bits 4-11		unused
  2	mux read		mux set
  3	di 4 bit		do 4 bit
  4567	unused			unused
  8	status eoc uni/bip	interrupt reset
  9	dma, int, trig ctrl	set dma, int
  a	pacer control		unused
  b	gain status		gain control
  cdef	8254

*/

/*
    cio-das16jr_16.pdf

    "das16jr_16"

  0	a/d bits 0-7		start 16 bit
  1	a/d bits 8-15		unused
  2	mux read		mux set
  3	di 4 bit		do 4 bit
  4567	unused			unused
  8	status eoc uni/bip	interrupt reset
  9	dma, int, trig ctrl	set dma, int
  a	pacer control		unused
  b	gain status		gain control
  cdef	8254

*/
/*
    cio-das160x-1x.pdf

    "das1601/12"
    "das1602/12"
    "das1602/16"

  0	a/d bits 0-3		start 12 bit
  1	a/d bits 4-11		unused
  2	mux read		mux set
  3	di 4 bit		do 4 bit
  4	unused			ao0_lsb
  5	unused			ao0_msb
  6	unused			ao1_lsb
  7	unused			ao1_msb
  8	status eoc uni/bip	interrupt reset
  9	dma, int, trig ctrl	set dma, int
  a	pacer control		unused
  b	gain status		gain control
  cdef	8254
  400	8255
  404	unused			conversion enable
  405	unused			burst enable
  406	unused			das1600 enable
  407	status

*/

/*  size in bytes of a sample from board */
static const int sample_size = 2;

#define DAS16_TRIG		0
#define DAS16_AI_LSB		0
#define DAS16_AI_MSB		1
#define DAS16_MUX		2
#define DAS16_DIO		3
#define DAS16_AO_LSB(x)	((x) ? 6 : 4)
#define DAS16_AO_MSB(x)	((x) ? 7 : 5)
#define DAS16_STATUS		8
#define   BUSY			(1<<7)
#define   UNIPOLAR			(1<<6)
#define   DAS16_MUXBIT			(1<<5)
#define   DAS16_INT			(1<<4)
#define DAS16_CONTROL		9
#define   DAS16_INTE			(1<<7)
#define   DAS16_IRQ(x)			(((x) & 0x7) << 4)
#define   DMA_ENABLE			(1<<2)
#define   PACING_MASK	0x3
#define   INT_PACER		0x03
#define   EXT_PACER			0x02
#define   DAS16_SOFT		0x00
#define DAS16_PACER		0x0A
#define   DAS16_CTR0			(1<<1)
#define   DAS16_TRIG0			(1<<0)
#define   BURST_LEN_BITS(x)			(((x) & 0xf) << 4)
#define DAS16_GAIN		0x0B
#define DAS16_CNTR0_DATA		0x0C
#define DAS16_CNTR1_DATA		0x0D
#define DAS16_CNTR2_DATA		0x0E
#define DAS16_CNTR_CONTROL	0x0F
#define   DAS16_TERM_CNT	0x00
#define   DAS16_ONE_SHOT	0x02
#define   DAS16_RATE_GEN	0x04
#define   DAS16_CNTR_LSB_MSB	0x30
#define   DAS16_CNTR0		0x00
#define   DAS16_CNTR1		0x40
#define   DAS16_CNTR2		0x80

#define DAS1600_CONV		0x404
#define   DAS1600_CONV_DISABLE		0x40
#define DAS1600_BURST		0x405
#define   DAS1600_BURST_VAL		0x40
#define DAS1600_ENABLE		0x406
#define   DAS1600_ENABLE_VAL		0x40
#define DAS1600_STATUS_B	0x407
#define   DAS1600_BME		0x40
#define   DAS1600_ME		0x20
#define   DAS1600_CD			0x10
#define   DAS1600_WS			0x02
#define   DAS1600_CLK_10MHZ		0x01

static const struct comedi_lrange range_das1x01_bip = { 4, {
							    BIP_RANGE(10),
							    BIP_RANGE(1),
							    BIP_RANGE(0.1),
							    BIP_RANGE(0.01),
							    }
};

static const struct comedi_lrange range_das1x01_unip = { 4, {
							     UNI_RANGE(10),
							     UNI_RANGE(1),
							     UNI_RANGE(0.1),
							     UNI_RANGE(0.01),
							     }
};

static const struct comedi_lrange range_das1x02_bip = { 4, {
							    BIP_RANGE(10),
							    BIP_RANGE(5),
							    BIP_RANGE(2.5),
							    BIP_RANGE(1.25),
							    }
};

static const struct comedi_lrange range_das1x02_unip = { 4, {
							     UNI_RANGE(10),
							     UNI_RANGE(5),
							     UNI_RANGE(2.5),
							     UNI_RANGE(1.25),
							     }
};

static const struct comedi_lrange range_das16jr = { 9, {
						/*  also used by 16/330 */
							BIP_RANGE(10),
							BIP_RANGE(5),
							BIP_RANGE(2.5),
							BIP_RANGE(1.25),
							BIP_RANGE(0.625),
							UNI_RANGE(10),
							UNI_RANGE(5),
							UNI_RANGE(2.5),
							UNI_RANGE(1.25),
							}
};

static const struct comedi_lrange range_das16jr_16 = { 8, {
							   BIP_RANGE(10),
							   BIP_RANGE(5),
							   BIP_RANGE(2.5),
							   BIP_RANGE(1.25),
							   UNI_RANGE(10),
							   UNI_RANGE(5),
							   UNI_RANGE(2.5),
							   UNI_RANGE(1.25),
							   }
=======
 * das16.c
 * DAS16 driver
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 2000 David A. Schleef <ds@schleef.org>
 * Copyright (C) 2000 Chris R. Baugher <baugher@enteract.com>
 * Copyright (C) 2001,2002 Frank Mori Hess <fmhess@users.sourceforge.net>
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
 * Driver: das16
 * Description: DAS16 compatible boards
 * Author: Sam Moore, Warren Jasper, ds, Chris Baugher, Frank Hess, Roman Fietze
 * Devices: [Keithley Metrabyte] DAS-16 (das-16), DAS-16G (das-16g),
 *   DAS-16F (das-16f), DAS-1201 (das-1201), DAS-1202 (das-1202),
 *   DAS-1401 (das-1401), DAS-1402 (das-1402), DAS-1601 (das-1601),
 *   DAS-1602 (das-1602),
 *   [ComputerBoards] PC104-DAS16/JR (pc104-das16jr),
 *   PC104-DAS16JR/16 (pc104-das16jr/16), CIO-DAS16 (cio-das16),
 *   CIO-DAS16F (cio-das16/f), CIO-DAS16/JR (cio-das16/jr),
 *   CIO-DAS16JR/16 (cio-das16jr/16), CIO-DAS1401/12 (cio-das1401/12),
 *   CIO-DAS1402/12 (cio-das1402/12), CIO-DAS1402/16 (cio-das1402/16),
 *   CIO-DAS1601/12 (cio-das1601/12), CIO-DAS1602/12 (cio-das1602/12),
 *   CIO-DAS1602/16 (cio-das1602/16), CIO-DAS16/330 (cio-das16/330)
 * Status: works
 * Updated: 2003-10-12
 *
 * A rewrite of the das16 and das1600 drivers.
 *
 * Options:
 *	[0] - base io address
 *	[1] - irq (does nothing, irq is not used anymore)
 *	[2] - dma channel (optional, required for comedi_command support)
 *	[3] - master clock speed in MHz (optional, 1 or 10, ignored if
 *		board can probe clock, defaults to 1)
 *	[4] - analog input range lowest voltage in microvolts (optional,
 *		only useful if your board does not have software
 *		programmable gain)
 *	[5] - analog input range highest voltage in microvolts (optional,
 *		only useful if board does not have software programmable
 *		gain)
 *	[6] - analog output range lowest voltage in microvolts (optional)
 *	[7] - analog output range highest voltage in microvolts (optional)
 *
 * Passing a zero for an option is the same as leaving it unspecified.
 */

/*
 * Testing and debugging help provided by Daniel Koch.
 *
 * Keithley Manuals:
 *	2309.PDF (das16)
 *	4919.PDF (das1400, 1600)
 *	4922.PDF (das-1400)
 *	4923.PDF (das1200, 1400, 1600)
 *
 * Computer boards manuals also available from their website
 * www.measurementcomputing.com
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/interrupt.h>

#include "../comedidev.h"

#include "comedi_isadma.h"
#include "comedi_8254.h"
#include "8255.h"

#define DAS16_DMA_SIZE 0xff00	/*  size in bytes of allocated dma buffer */

/*
 * Register I/O map
 */
#define DAS16_TRIG_REG			0x00
#define DAS16_AI_LSB_REG		0x00
#define DAS16_AI_MSB_REG		0x01
#define DAS16_MUX_REG			0x02
#define DAS16_DIO_REG			0x03
#define DAS16_AO_LSB_REG(x)		((x) ? 0x06 : 0x04)
#define DAS16_AO_MSB_REG(x)		((x) ? 0x07 : 0x05)
#define DAS16_STATUS_REG		0x08
#define DAS16_STATUS_BUSY		BIT(7)
#define DAS16_STATUS_UNIPOLAR		BIT(6)
#define DAS16_STATUS_MUXBIT		BIT(5)
#define DAS16_STATUS_INT		BIT(4)
#define DAS16_CTRL_REG			0x09
#define DAS16_CTRL_INTE			BIT(7)
#define DAS16_CTRL_IRQ(x)		(((x) & 0x7) << 4)
#define DAS16_CTRL_DMAE			BIT(2)
#define DAS16_CTRL_PACING_MASK		(3 << 0)
#define DAS16_CTRL_INT_PACER		(3 << 0)
#define DAS16_CTRL_EXT_PACER		(2 << 0)
#define DAS16_CTRL_SOFT_PACER		(0 << 0)
#define DAS16_PACER_REG			0x0a
#define DAS16_PACER_BURST_LEN(x)	(((x) & 0xf) << 4)
#define DAS16_PACER_CTR0		BIT(1)
#define DAS16_PACER_TRIG0		BIT(0)
#define DAS16_GAIN_REG			0x0b
#define DAS16_TIMER_BASE_REG		0x0c	/* to 0x0f */

#define DAS1600_CONV_REG		0x404
#define DAS1600_CONV_DISABLE		BIT(6)
#define DAS1600_BURST_REG		0x405
#define DAS1600_BURST_VAL		BIT(6)
#define DAS1600_ENABLE_REG		0x406
#define DAS1600_ENABLE_VAL		BIT(6)
#define DAS1600_STATUS_REG		0x407
#define DAS1600_STATUS_BME		BIT(6)
#define DAS1600_STATUS_ME		BIT(5)
#define DAS1600_STATUS_CD		BIT(4)
#define DAS1600_STATUS_WS		BIT(1)
#define DAS1600_STATUS_CLK_10MHZ	BIT(0)

static const struct comedi_lrange range_das1x01_bip = {
	4, {
		BIP_RANGE(10),
		BIP_RANGE(1),
		BIP_RANGE(0.1),
		BIP_RANGE(0.01)
	}
};

static const struct comedi_lrange range_das1x01_unip = {
	4, {
		UNI_RANGE(10),
		UNI_RANGE(1),
		UNI_RANGE(0.1),
		UNI_RANGE(0.01)
	}
};

static const struct comedi_lrange range_das1x02_bip = {
	4, {
		BIP_RANGE(10),
		BIP_RANGE(5),
		BIP_RANGE(2.5),
		BIP_RANGE(1.25)
	}
};

static const struct comedi_lrange range_das1x02_unip = {
	4, {
		UNI_RANGE(10),
		UNI_RANGE(5),
		UNI_RANGE(2.5),
		UNI_RANGE(1.25)
	}
};

static const struct comedi_lrange range_das16jr = {
	9, {
		BIP_RANGE(10),
		BIP_RANGE(5),
		BIP_RANGE(2.5),
		BIP_RANGE(1.25),
		BIP_RANGE(0.625),
		UNI_RANGE(10),
		UNI_RANGE(5),
		UNI_RANGE(2.5),
		UNI_RANGE(1.25)
	}
};

static const struct comedi_lrange range_das16jr_16 = {
	8, {
		BIP_RANGE(10),
		BIP_RANGE(5),
		BIP_RANGE(2.5),
		BIP_RANGE(1.25),
		UNI_RANGE(10),
		UNI_RANGE(5),
		UNI_RANGE(2.5),
		UNI_RANGE(1.25)
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static const int das16jr_gainlist[] = { 8, 0, 1, 2, 3, 4, 5, 6, 7 };
static const int das16jr_16_gainlist[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
static const int das1600_gainlist[] = { 0, 1, 2, 3 };

enum {
	das16_pg_none = 0,
	das16_pg_16jr,
	das16_pg_16jr_16,
	das16_pg_1601,
	das16_pg_1602,
};
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static const int *const das16_gainlists[] = {
	NULL,
	das16jr_gainlist,
	das16jr_16_gainlist,
	das1600_gainlist,
	das1600_gainlist,
};

static const struct comedi_lrange *const das16_ai_uni_lranges[] = {
	&range_unknown,
	&range_das16jr,
	&range_das16jr_16,
	&range_das1x01_unip,
	&range_das1x02_unip,
};

static const struct comedi_lrange *const das16_ai_bip_lranges[] = {
	&range_unknown,
	&range_das16jr,
	&range_das16jr_16,
	&range_das1x01_bip,
	&range_das1x02_bip,
};

<<<<<<< HEAD
struct munge_info {
	uint8_t byte;
	unsigned have_byte:1;
};

struct das16_board {
	const char *name;
	void *ai;
	unsigned int ai_nbits;
	unsigned int ai_speed;	/*  max conversion speed in nanosec */
	unsigned int ai_pg;
	void *ao;
	unsigned int ao_nbits;
	void *di;
	void *do_;

	unsigned int i8255_offset;
	unsigned int i8254_offset;
=======
struct das16_board {
	const char *name;
	unsigned int ai_maxdata;
	unsigned int ai_speed;	/*  max conversion speed in nanosec */
	unsigned int ai_pg;
	unsigned int has_ao:1;
	unsigned int has_8255:1;

	unsigned int i8255_offset;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	unsigned int size;
	unsigned int id;
};

<<<<<<< HEAD
#define DAS16_TIMEOUT 1000

/* Period for timer interrupt in jiffies.  It's a function
 * to deal with possibility of dynamic HZ patches  */
=======
static const struct das16_board das16_boards[] = {
	{
		.name		= "das-16",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 15000,
		.ai_pg		= das16_pg_none,
		.has_ao		= 1,
		.has_8255	= 1,
		.i8255_offset	= 0x10,
		.size		= 0x14,
		.id		= 0x00,
	}, {
		.name		= "das-16g",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 15000,
		.ai_pg		= das16_pg_none,
		.has_ao		= 1,
		.has_8255	= 1,
		.i8255_offset	= 0x10,
		.size		= 0x14,
		.id		= 0x00,
	}, {
		.name		= "das-16f",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 8500,
		.ai_pg		= das16_pg_none,
		.has_ao		= 1,
		.has_8255	= 1,
		.i8255_offset	= 0x10,
		.size		= 0x14,
		.id		= 0x00,
	}, {
		.name		= "cio-das16",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 20000,
		.ai_pg		= das16_pg_none,
		.has_ao		= 1,
		.has_8255	= 1,
		.i8255_offset	= 0x10,
		.size		= 0x14,
		.id		= 0x80,
	}, {
		.name		= "cio-das16/f",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_none,
		.has_ao		= 1,
		.has_8255	= 1,
		.i8255_offset	= 0x10,
		.size		= 0x14,
		.id		= 0x80,
	}, {
		.name		= "cio-das16/jr",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 7692,
		.ai_pg		= das16_pg_16jr,
		.size		= 0x10,
		.id		= 0x00,
	}, {
		.name		= "pc104-das16jr",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 3300,
		.ai_pg		= das16_pg_16jr,
		.size		= 0x10,
		.id		= 0x00,
	}, {
		.name		= "cio-das16jr/16",
		.ai_maxdata	= 0xffff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_16jr_16,
		.size		= 0x10,
		.id		= 0x00,
	}, {
		.name		= "pc104-das16jr/16",
		.ai_maxdata	= 0xffff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_16jr_16,
		.size		= 0x10,
		.id		= 0x00,
	}, {
		.name		= "das-1201",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 20000,
		.ai_pg		= das16_pg_none,
		.has_8255	= 1,
		.i8255_offset	= 0x400,
		.size		= 0x408,
		.id		= 0x20,
	}, {
		.name		= "das-1202",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_none,
		.has_8255	= 1,
		.i8255_offset	= 0x400,
		.size		= 0x408,
		.id		= 0x20,
	}, {
		.name		= "das-1401",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1601,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "das-1402",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1602,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "das-1601",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1601,
		.has_ao		= 1,
		.has_8255	= 1,
		.i8255_offset	= 0x400,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "das-1602",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1602,
		.has_ao		= 1,
		.has_8255	= 1,
		.i8255_offset	= 0x400,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1401/12",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 6250,
		.ai_pg		= das16_pg_1601,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1402/12",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 6250,
		.ai_pg		= das16_pg_1602,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1402/16",
		.ai_maxdata	= 0xffff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1602,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1601/12",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 6250,
		.ai_pg		= das16_pg_1601,
		.has_ao		= 1,
		.has_8255	= 1,
		.i8255_offset	= 0x400,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1602/12",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1602,
		.has_ao		= 1,
		.has_8255	= 1,
		.i8255_offset	= 0x400,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1602/16",
		.ai_maxdata	= 0xffff,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1602,
		.has_ao		= 1,
		.has_8255	= 1,
		.i8255_offset	= 0x400,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das16/330",
		.ai_maxdata	= 0x0fff,
		.ai_speed	= 3030,
		.ai_pg		= das16_pg_16jr,
		.size		= 0x14,
		.id		= 0xf0,
	},
};

/*
 * Period for timer interrupt in jiffies.  It's a function
 * to deal with possibility of dynamic HZ patches
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline int timer_period(void)
{
	return HZ / 20;
}

struct das16_private_struct {
<<<<<<< HEAD
	unsigned int ai_unipolar;	/*  unipolar flag */
	unsigned int ai_singleended;	/*  single ended flag */
	unsigned int clockbase;	/*  master clock speed in ns */
	volatile unsigned int control_state;	/*  dma, interrupt and trigger control bits */
	volatile unsigned long adc_byte_count;	/*  number of bytes remaining */
	/*  divisor dividing master clock to get conversion frequency */
	unsigned int divisor1;
	/*  divisor dividing master clock to get conversion frequency */
	unsigned int divisor2;
	unsigned int dma_chan;	/*  dma channel */
	uint16_t *dma_buffer[2];
	dma_addr_t dma_buffer_addr[2];
	unsigned int current_buffer;
	volatile unsigned int dma_transfer_size;	/*  target number of bytes to transfer per dma shot */
	/**
	 * user-defined analog input and output ranges
	 * defined from config options
	 */
	struct comedi_lrange *user_ai_range_table;
	struct comedi_lrange *user_ao_range_table;

	struct timer_list timer;	/*  for timed interrupt */
	volatile short timer_running;
	volatile short timer_mode;	/*  true if using timer mode */

	unsigned long extra_iobase;
};
=======
	struct comedi_isadma	*dma;
	unsigned int		clockbase;
	unsigned int		ctrl_reg;
	unsigned int		divisor1;
	unsigned int		divisor2;
	struct timer_list	timer;
	unsigned long		extra_iobase;
	unsigned int		can_burst:1;
	unsigned int		timer_running:1;
};

static void das16_ai_setup_dma(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       unsigned int unread_samples)
{
	struct das16_private_struct *devpriv = dev->private;
	struct comedi_isadma *dma = devpriv->dma;
	struct comedi_isadma_desc *desc = &dma->desc[dma->cur_dma];
	unsigned int max_samples = comedi_bytes_to_samples(s, desc->maxsize);
	unsigned int nsamples;

	/*
	 * Determine dma size based on the buffer size plus the number of
	 * unread samples and the number of samples remaining in the command.
	 */
	nsamples = comedi_nsamples_left(s, max_samples + unread_samples);
	if (nsamples > unread_samples) {
		nsamples -= unread_samples;
		desc->size = comedi_samples_to_bytes(s, nsamples);
		comedi_isadma_program(desc);
	}
}

static void das16_interrupt(struct comedi_device *dev)
{
	struct das16_private_struct *devpriv = dev->private;
	struct comedi_subdevice *s = dev->read_subdev;
	struct comedi_async *async = s->async;
	struct comedi_cmd *cmd = &async->cmd;
	struct comedi_isadma *dma = devpriv->dma;
	struct comedi_isadma_desc *desc = &dma->desc[dma->cur_dma];
	unsigned long spin_flags;
	unsigned int residue;
	unsigned int nbytes;
	unsigned int nsamples;

	spin_lock_irqsave(&dev->spinlock, spin_flags);
	if (!(devpriv->ctrl_reg & DAS16_CTRL_DMAE)) {
		spin_unlock_irqrestore(&dev->spinlock, spin_flags);
		return;
	}

	/*
	 * The pc104-das16jr (at least) has problems if the dma
	 * transfer is interrupted in the middle of transferring
	 * a 16 bit sample.
	 */
	residue = comedi_isadma_disable_on_sample(desc->chan,
						  comedi_bytes_per_sample(s));

	/* figure out how many samples to read */
	if (residue > desc->size) {
		dev_err(dev->class_dev, "residue > transfer size!\n");
		async->events |= COMEDI_CB_ERROR;
		nbytes = 0;
	} else {
		nbytes = desc->size - residue;
	}
	nsamples = comedi_bytes_to_samples(s, nbytes);

	/* restart DMA if more samples are needed */
	if (nsamples) {
		dma->cur_dma = 1 - dma->cur_dma;
		das16_ai_setup_dma(dev, s, nsamples);
	}

	spin_unlock_irqrestore(&dev->spinlock, spin_flags);

	comedi_buf_write_samples(s, desc->virt_addr, nsamples);

	if (cmd->stop_src == TRIG_COUNT && async->scans_done >= cmd->stop_arg)
		async->events |= COMEDI_CB_EOA;

	comedi_handle_events(dev, s);
}

static void das16_timer_interrupt(unsigned long arg)
{
	struct comedi_device *dev = (struct comedi_device *)arg;
	struct das16_private_struct *devpriv = dev->private;
	unsigned long flags;

	das16_interrupt(dev);

	spin_lock_irqsave(&dev->spinlock, flags);
	if (devpriv->timer_running)
		mod_timer(&devpriv->timer, jiffies + timer_period());
	spin_unlock_irqrestore(&dev->spinlock, flags);
}

static void das16_ai_set_mux_range(struct comedi_device *dev,
				   unsigned int first_chan,
				   unsigned int last_chan,
				   unsigned int range)
{
	const struct das16_board *board = dev->board_ptr;

	/* set multiplexer */
	outb(first_chan | (last_chan << 4), dev->iobase + DAS16_MUX_REG);

	/* some boards do not have programmable gain */
	if (board->ai_pg == das16_pg_none)
		return;

	/*
	 * Set gain (this is also burst rate register but according to
	 * computer boards manual, burst rate does nothing, even on
	 * keithley cards).
	 */
	outb((das16_gainlists[board->ai_pg])[range],
	     dev->iobase + DAS16_GAIN_REG);
}

static int das16_ai_check_chanlist(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_cmd *cmd)
{
	unsigned int chan0 = CR_CHAN(cmd->chanlist[0]);
	unsigned int range0 = CR_RANGE(cmd->chanlist[0]);
	int i;

	for (i = 1; i < cmd->chanlist_len; i++) {
		unsigned int chan = CR_CHAN(cmd->chanlist[i]);
		unsigned int range = CR_RANGE(cmd->chanlist[i]);

		if (chan != ((chan0 + i) % s->n_chan)) {
			dev_dbg(dev->class_dev,
				"entries in chanlist must be consecutive channels, counting upwards\n");
			return -EINVAL;
		}

		if (range != range0) {
			dev_dbg(dev->class_dev,
				"entries in chanlist must all have the same gain\n");
			return -EINVAL;
		}
	}

	return 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int das16_cmd_test(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_cmd *cmd)
{
<<<<<<< HEAD
	const struct das16_board *board = comedi_board(dev);
	struct das16_private_struct *devpriv = dev->private;
	int err = 0, tmp;
	int gain, start_chan, i;
	int mask;

	/* Step 1 : check if triggers are trivially valid */

	err |= cfc_check_trigger_src(&cmd->start_src, TRIG_NOW);

	mask = TRIG_FOLLOW;
	/*  if board supports burst mode */
	if (board->size > 0x400)
		mask |= TRIG_TIMER | TRIG_EXT;
	err |= cfc_check_trigger_src(&cmd->scan_begin_src, mask);

	tmp = cmd->convert_src;
	mask = TRIG_TIMER | TRIG_EXT;
	/*  if board supports burst mode */
	if (board->size > 0x400)
		mask |= TRIG_NOW;
	err |= cfc_check_trigger_src(&cmd->convert_src, mask);

	err |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= cfc_check_trigger_src(&cmd->stop_src, TRIG_COUNT | TRIG_NONE);
=======
	const struct das16_board *board = dev->board_ptr;
	struct das16_private_struct *devpriv = dev->private;
	int err = 0;
	unsigned int trig_mask;
	unsigned int arg;

	/* Step 1 : check if triggers are trivially valid */

	err |= comedi_check_trigger_src(&cmd->start_src, TRIG_NOW);

	trig_mask = TRIG_FOLLOW;
	if (devpriv->can_burst)
		trig_mask |= TRIG_TIMER | TRIG_EXT;
	err |= comedi_check_trigger_src(&cmd->scan_begin_src, trig_mask);

	trig_mask = TRIG_TIMER | TRIG_EXT;
	if (devpriv->can_burst)
		trig_mask |= TRIG_NOW;
	err |= comedi_check_trigger_src(&cmd->convert_src, trig_mask);

	err |= comedi_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= comedi_check_trigger_src(&cmd->stop_src, TRIG_COUNT | TRIG_NONE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (err)
		return 1;

	/* Step 2a : make sure trigger sources are unique */

<<<<<<< HEAD
	err |= cfc_check_trigger_is_unique(cmd->scan_begin_src);
	err |= cfc_check_trigger_is_unique(cmd->convert_src);
	err |= cfc_check_trigger_is_unique(cmd->stop_src);
=======
	err |= comedi_check_trigger_is_unique(cmd->scan_begin_src);
	err |= comedi_check_trigger_is_unique(cmd->convert_src);
	err |= comedi_check_trigger_is_unique(cmd->stop_src);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Step 2b : and mutually compatible */

	/*  make sure scan_begin_src and convert_src dont conflict */
	if (cmd->scan_begin_src == TRIG_FOLLOW && cmd->convert_src == TRIG_NOW)
		err |= -EINVAL;
	if (cmd->scan_begin_src != TRIG_FOLLOW && cmd->convert_src != TRIG_NOW)
		err |= -EINVAL;

	if (err)
		return 2;

	/* Step 3: check if arguments are trivially valid */

<<<<<<< HEAD
	err |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);

	if (cmd->scan_begin_src == TRIG_FOLLOW)	/* internal trigger */
		err |= cfc_check_trigger_arg_is(&cmd->scan_begin_arg, 0);

	err |= cfc_check_trigger_arg_is(&cmd->scan_end_arg, cmd->chanlist_len);

	/* check against maximum frequency */
	if (cmd->scan_begin_src == TRIG_TIMER)
		err |= cfc_check_trigger_arg_min(&cmd->scan_begin_arg,
					board->ai_speed * cmd->chanlist_len);

	if (cmd->convert_src == TRIG_TIMER)
		err |= cfc_check_trigger_arg_min(&cmd->convert_arg,
						 board->ai_speed);

	if (cmd->stop_src == TRIG_NONE)
		err |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);
=======
	err |= comedi_check_trigger_arg_is(&cmd->start_arg, 0);

	if (cmd->scan_begin_src == TRIG_FOLLOW)	/* internal trigger */
		err |= comedi_check_trigger_arg_is(&cmd->scan_begin_arg, 0);

	err |= comedi_check_trigger_arg_is(&cmd->scan_end_arg,
					   cmd->chanlist_len);

	/* check against maximum frequency */
	if (cmd->scan_begin_src == TRIG_TIMER) {
		err |= comedi_check_trigger_arg_min(&cmd->scan_begin_arg,
						    board->ai_speed *
						    cmd->chanlist_len);
	}

	if (cmd->convert_src == TRIG_TIMER) {
		err |= comedi_check_trigger_arg_min(&cmd->convert_arg,
						    board->ai_speed);
	}

	if (cmd->stop_src == TRIG_COUNT)
		err |= comedi_check_trigger_arg_min(&cmd->stop_arg, 1);
	else	/* TRIG_NONE */
		err |= comedi_check_trigger_arg_is(&cmd->stop_arg, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (err)
		return 3;

	/*  step 4: fix up arguments */
	if (cmd->scan_begin_src == TRIG_TIMER) {
<<<<<<< HEAD
		unsigned int tmp = cmd->scan_begin_arg;
		/*  set divisors, correct timing arguments */
		i8253_cascade_ns_to_timer_2div(devpriv->clockbase,
					       &(devpriv->divisor1),
					       &(devpriv->divisor2),
					       &(cmd->scan_begin_arg),
					       cmd->flags & TRIG_ROUND_MASK);
		err += (tmp != cmd->scan_begin_arg);
	}
	if (cmd->convert_src == TRIG_TIMER) {
		unsigned int tmp = cmd->convert_arg;
		/*  set divisors, correct timing arguments */
		i8253_cascade_ns_to_timer_2div(devpriv->clockbase,
					       &(devpriv->divisor1),
					       &(devpriv->divisor2),
					       &(cmd->convert_arg),
					       cmd->flags & TRIG_ROUND_MASK);
		err += (tmp != cmd->convert_arg);
=======
		arg = cmd->scan_begin_arg;
		comedi_8254_cascade_ns_to_timer(dev->pacer, &arg, cmd->flags);
		err |= comedi_check_trigger_arg_is(&cmd->scan_begin_arg, arg);
	}
	if (cmd->convert_src == TRIG_TIMER) {
		arg = cmd->convert_arg;
		comedi_8254_cascade_ns_to_timer(dev->pacer, &arg, cmd->flags);
		err |= comedi_check_trigger_arg_is(&cmd->convert_arg, arg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	if (err)
		return 4;

<<<<<<< HEAD
	/*  check channel/gain list against card's limitations */
	if (cmd->chanlist) {
		gain = CR_RANGE(cmd->chanlist[0]);
		start_chan = CR_CHAN(cmd->chanlist[0]);
		for (i = 1; i < cmd->chanlist_len; i++) {
			if (CR_CHAN(cmd->chanlist[i]) !=
			    (start_chan + i) % s->n_chan) {
				comedi_error(dev,
						"entries in chanlist must be "
						"consecutive channels, "
						"counting upwards\n");
				err++;
			}
			if (CR_RANGE(cmd->chanlist[i]) != gain) {
				comedi_error(dev,
						"entries in chanlist must all "
						"have the same gain\n");
				err++;
			}
		}
	}
=======
	/* Step 5: check channel list if it exists */
	if (cmd->chanlist && cmd->chanlist_len > 0)
		err |= das16_ai_check_chanlist(dev, s, cmd);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err)
		return 5;

	return 0;
}

<<<<<<< HEAD
/* utility function that suggests a dma transfer size in bytes */
static unsigned int das16_suggest_transfer_size(struct comedi_device *dev,
						const struct comedi_cmd *cmd)
{
	struct das16_private_struct *devpriv = dev->private;
	unsigned int size;
	unsigned int freq;

	/* if we are using timer interrupt, we don't care how long it
	 * will take to complete transfer since it will be interrupted
	 * by timer interrupt */
	if (devpriv->timer_mode)
		return DAS16_DMA_SIZE;

	/* otherwise, we are relying on dma terminal count interrupt,
	 * so pick a reasonable size */
	if (cmd->convert_src == TRIG_TIMER)
		freq = 1000000000 / cmd->convert_arg;
	else if (cmd->scan_begin_src == TRIG_TIMER)
		freq = (1000000000 / cmd->scan_begin_arg) * cmd->chanlist_len;
	/*  return some default value */
	else
		freq = 0xffffffff;

	if (cmd->flags & TRIG_WAKE_EOS) {
		size = sample_size * cmd->chanlist_len;
	} else {
		/*  make buffer fill in no more than 1/3 second */
		size = (freq / 3) * sample_size;
	}

	/*  set a minimum and maximum size allowed */
	if (size > DAS16_DMA_SIZE)
		size = DAS16_DMA_SIZE - DAS16_DMA_SIZE % sample_size;
	else if (size < sample_size)
		size = sample_size;

	if (cmd->stop_src == TRIG_COUNT && size > devpriv->adc_byte_count)
		size = devpriv->adc_byte_count;

	return size;
}

static unsigned int das16_set_pacer(struct comedi_device *dev, unsigned int ns,
				    int rounding_flags)
{
	struct das16_private_struct *devpriv = dev->private;

	i8253_cascade_ns_to_timer_2div(devpriv->clockbase, &(devpriv->divisor1),
				       &(devpriv->divisor2), &ns,
				       rounding_flags & TRIG_ROUND_MASK);

	/* Write the values of ctr1 and ctr2 into counters 1 and 2 */
	i8254_load(dev->iobase + DAS16_CNTR0_DATA, 0, 1, devpriv->divisor1, 2);
	i8254_load(dev->iobase + DAS16_CNTR0_DATA, 0, 2, devpriv->divisor2, 2);
=======
static unsigned int das16_set_pacer(struct comedi_device *dev, unsigned int ns,
				    unsigned int flags)
{
	comedi_8254_cascade_ns_to_timer(dev->pacer, &ns, flags);
	comedi_8254_update_divisors(dev->pacer);
	comedi_8254_pacer_enable(dev->pacer, 1, 2, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ns;
}

static int das16_cmd_exec(struct comedi_device *dev, struct comedi_subdevice *s)
{
<<<<<<< HEAD
	const struct das16_board *board = comedi_board(dev);
	struct das16_private_struct *devpriv = dev->private;
	struct comedi_async *async = s->async;
	struct comedi_cmd *cmd = &async->cmd;
	unsigned int byte;
	unsigned long flags;
	int range;

	if (devpriv->dma_chan == 0 || (dev->irq == 0
				       && devpriv->timer_mode == 0)) {
		comedi_error(dev,
				"irq (or use of 'timer mode') dma required to "
							"execute comedi_cmd");
		return -1;
	}
	if (cmd->flags & TRIG_RT) {
		comedi_error(dev, "isa dma transfers cannot be performed with "
							"TRIG_RT, aborting");
		return -1;
	}

	devpriv->adc_byte_count =
	    cmd->stop_arg * cmd->chanlist_len * sizeof(uint16_t);

	/*  disable conversions for das1600 mode */
	if (board->size > 0x400)
		outb(DAS1600_CONV_DISABLE, dev->iobase + DAS1600_CONV);

	/*  set scan limits */
	byte = CR_CHAN(cmd->chanlist[0]);
	byte |= CR_CHAN(cmd->chanlist[cmd->chanlist_len - 1]) << 4;
	outb(byte, dev->iobase + DAS16_MUX);

	/* set gain (this is also burst rate register but according to
	 * computer boards manual, burst rate does nothing, even on
	 * keithley cards) */
	if (board->ai_pg != das16_pg_none) {
		range = CR_RANGE(cmd->chanlist[0]);
		outb((das16_gainlists[board->ai_pg])[range],
		     dev->iobase + DAS16_GAIN);
	}

	/* set counter mode and counts */
	cmd->convert_arg =
	    das16_set_pacer(dev, cmd->convert_arg,
			    cmd->flags & TRIG_ROUND_MASK);
	DEBUG_PRINT("pacer period: %d ns\n", cmd->convert_arg);

	/* enable counters */
	byte = 0;
	/* Enable burst mode if appropriate. */
	if (board->size > 0x400) {
		if (cmd->convert_src == TRIG_NOW) {
			outb(DAS1600_BURST_VAL, dev->iobase + DAS1600_BURST);
			/*  set burst length */
			byte |= BURST_LEN_BITS(cmd->chanlist_len - 1);
		} else {
			outb(0, dev->iobase + DAS1600_BURST);
		}
	}
	outb(byte, dev->iobase + DAS16_PACER);

	/*  set up dma transfer */
	flags = claim_dma_lock();
	disable_dma(devpriv->dma_chan);
	/* clear flip-flop to make sure 2-byte registers for
	 * count and address get set correctly */
	clear_dma_ff(devpriv->dma_chan);
	devpriv->current_buffer = 0;
	set_dma_addr(devpriv->dma_chan,
		     devpriv->dma_buffer_addr[devpriv->current_buffer]);
	/*  set appropriate size of transfer */
	devpriv->dma_transfer_size = das16_suggest_transfer_size(dev, cmd);
	set_dma_count(devpriv->dma_chan, devpriv->dma_transfer_size);
	enable_dma(devpriv->dma_chan);
	release_dma_lock(flags);

	/*  set up interrupt */
	if (devpriv->timer_mode) {
		devpriv->timer_running = 1;
		devpriv->timer.expires = jiffies + timer_period();
		add_timer(&devpriv->timer);
		devpriv->control_state &= ~DAS16_INTE;
	} else {
		/* clear interrupt bit */
		outb(0x00, dev->iobase + DAS16_STATUS);
		/* enable interrupts */
		devpriv->control_state |= DAS16_INTE;
	}
	devpriv->control_state |= DMA_ENABLE;
	devpriv->control_state &= ~PACING_MASK;
	if (cmd->convert_src == TRIG_EXT)
		devpriv->control_state |= EXT_PACER;
	else
		devpriv->control_state |= INT_PACER;
	outb(devpriv->control_state, dev->iobase + DAS16_CONTROL);

	/* Enable conversions if using das1600 mode */
	if (board->size > 0x400)
		outb(0, dev->iobase + DAS1600_CONV);

=======
	struct das16_private_struct *devpriv = dev->private;
	struct comedi_isadma *dma = devpriv->dma;
	struct comedi_async *async = s->async;
	struct comedi_cmd *cmd = &async->cmd;
	unsigned int first_chan = CR_CHAN(cmd->chanlist[0]);
	unsigned int last_chan = CR_CHAN(cmd->chanlist[cmd->chanlist_len - 1]);
	unsigned int range = CR_RANGE(cmd->chanlist[0]);
	unsigned int byte;
	unsigned long flags;

	if (cmd->flags & CMDF_PRIORITY) {
		dev_err(dev->class_dev,
			"isa dma transfers cannot be performed with CMDF_PRIORITY, aborting\n");
		return -1;
	}

	if (devpriv->can_burst)
		outb(DAS1600_CONV_DISABLE, dev->iobase + DAS1600_CONV_REG);

	/* set mux and range for chanlist scan */
	das16_ai_set_mux_range(dev, first_chan, last_chan, range);

	/* set counter mode and counts */
	cmd->convert_arg = das16_set_pacer(dev, cmd->convert_arg, cmd->flags);

	/* enable counters */
	byte = 0;
	if (devpriv->can_burst) {
		if (cmd->convert_src == TRIG_NOW) {
			outb(DAS1600_BURST_VAL,
			     dev->iobase + DAS1600_BURST_REG);
			/*  set burst length */
			byte |= DAS16_PACER_BURST_LEN(cmd->chanlist_len - 1);
		} else {
			outb(0, dev->iobase + DAS1600_BURST_REG);
		}
	}
	outb(byte, dev->iobase + DAS16_PACER_REG);

	/* set up dma transfer */
	dma->cur_dma = 0;
	das16_ai_setup_dma(dev, s, 0);

	/*  set up timer */
	spin_lock_irqsave(&dev->spinlock, flags);
	devpriv->timer_running = 1;
	devpriv->timer.expires = jiffies + timer_period();
	add_timer(&devpriv->timer);

	/* enable DMA interrupt with external or internal pacing */
	devpriv->ctrl_reg &= ~(DAS16_CTRL_INTE | DAS16_CTRL_PACING_MASK);
	devpriv->ctrl_reg |= DAS16_CTRL_DMAE;
	if (cmd->convert_src == TRIG_EXT)
		devpriv->ctrl_reg |= DAS16_CTRL_EXT_PACER;
	else
		devpriv->ctrl_reg |= DAS16_CTRL_INT_PACER;
	outb(devpriv->ctrl_reg, dev->iobase + DAS16_CTRL_REG);

	if (devpriv->can_burst)
		outb(0, dev->iobase + DAS1600_CONV_REG);
	spin_unlock_irqrestore(&dev->spinlock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static int das16_cancel(struct comedi_device *dev, struct comedi_subdevice *s)
{
<<<<<<< HEAD
	const struct das16_board *board = comedi_board(dev);
	struct das16_private_struct *devpriv = dev->private;
	unsigned long flags;

	spin_lock_irqsave(&dev->spinlock, flags);
	/* disable interrupts, dma and pacer clocked conversions */
	devpriv->control_state &= ~DAS16_INTE & ~PACING_MASK & ~DMA_ENABLE;
	outb(devpriv->control_state, dev->iobase + DAS16_CONTROL);
	if (devpriv->dma_chan)
		disable_dma(devpriv->dma_chan);

	/*  disable SW timer */
	if (devpriv->timer_mode && devpriv->timer_running) {
=======
	struct das16_private_struct *devpriv = dev->private;
	struct comedi_isadma *dma = devpriv->dma;
	unsigned long flags;

	spin_lock_irqsave(&dev->spinlock, flags);

	/* disable interrupts, dma and pacer clocked conversions */
	devpriv->ctrl_reg &= ~(DAS16_CTRL_INTE | DAS16_CTRL_DMAE |
			       DAS16_CTRL_PACING_MASK);
	outb(devpriv->ctrl_reg, dev->iobase + DAS16_CTRL_REG);

	comedi_isadma_disable(dma->chan);

	/*  disable SW timer */
	if (devpriv->timer_running) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		devpriv->timer_running = 0;
		del_timer(&devpriv->timer);
	}

<<<<<<< HEAD
	/* disable burst mode */
	if (board->size > 0x400)
		outb(0, dev->iobase + DAS1600_BURST);

=======
	if (devpriv->can_burst)
		outb(0, dev->iobase + DAS1600_BURST_REG);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_unlock_irqrestore(&dev->spinlock, flags);

	return 0;
}

<<<<<<< HEAD
static void das16_reset(struct comedi_device *dev)
{
	outb(0, dev->iobase + DAS16_STATUS);
	outb(0, dev->iobase + DAS16_CONTROL);
	outb(0, dev->iobase + DAS16_PACER);
	outb(0, dev->iobase + DAS16_CNTR_CONTROL);
}

static int das16_ai_rinsn(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_insn *insn, unsigned int *data)
{
	const struct das16_board *board = comedi_board(dev);
	struct das16_private_struct *devpriv = dev->private;
	int i, n;
	int range;
	int chan;
	int msb, lsb;

	/*  disable interrupts and pacing */
	devpriv->control_state &= ~DAS16_INTE & ~DMA_ENABLE & ~PACING_MASK;
	outb(devpriv->control_state, dev->iobase + DAS16_CONTROL);

	/* set multiplexer */
	chan = CR_CHAN(insn->chanspec);
	chan |= CR_CHAN(insn->chanspec) << 4;
	outb(chan, dev->iobase + DAS16_MUX);

	/* set gain */
	if (board->ai_pg != das16_pg_none) {
		range = CR_RANGE(insn->chanspec);
		outb((das16_gainlists[board->ai_pg])[range],
		     dev->iobase + DAS16_GAIN);
	}

	for (n = 0; n < insn->n; n++) {
		/* trigger conversion */
		outb_p(0, dev->iobase + DAS16_TRIG);

		for (i = 0; i < DAS16_TIMEOUT; i++) {
			if (!(inb(dev->iobase + DAS16_STATUS) & BUSY))
				break;
		}
		if (i == DAS16_TIMEOUT) {
			printk("das16: timeout\n");
			return -ETIME;
		}
		msb = inb(dev->iobase + DAS16_AI_MSB);
		lsb = inb(dev->iobase + DAS16_AI_LSB);
		if (board->ai_nbits == 12)
			data[n] = ((lsb >> 4) & 0xf) | (msb << 4);
		else
			data[n] = lsb | (msb << 8);

	}

	return n;
}

static int das16_di_rbits(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_insn *insn, unsigned int *data)
{
	unsigned int bits;

	bits = inb(dev->iobase + DAS16_DIO) & 0xf;
	data[1] = bits;
	data[0] = 0;

	return insn->n;
}

static int das16_do_wbits(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_insn *insn, unsigned int *data)
{
	unsigned int wbits;

	/*  only set bits that have been masked */
	data[0] &= 0xf;
	wbits = s->state;
	/*  zero bits that have been masked */
	wbits &= ~data[0];
	/*  set masked bits */
	wbits |= data[0] & data[1];
	s->state = wbits;
	data[1] = wbits;

	outb(s->state, dev->iobase + DAS16_DIO);

	return insn->n;
}

static int das16_ao_winsn(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_insn *insn, unsigned int *data)
{
	const struct das16_board *board = comedi_board(dev);
	int i;
	int lsb, msb;
	int chan;

	chan = CR_CHAN(insn->chanspec);

	for (i = 0; i < insn->n; i++) {
		if (board->ao_nbits == 12) {
			lsb = (data[i] << 4) & 0xff;
			msb = (data[i] >> 4) & 0xff;
		} else {
			lsb = data[i] & 0xff;
			msb = (data[i] >> 8) & 0xff;
		}
		outb(lsb, dev->iobase + DAS16_AO_LSB(chan));
		outb(msb, dev->iobase + DAS16_AO_MSB(chan));
	}

	return i;
}

/* the pc104-das16jr (at least) has problems if the dma
	transfer is interrupted in the middle of transferring
	a 16 bit sample, so this function takes care to get
	an even transfer count after disabling dma
	channel.
*/
static int disable_dma_on_even(struct comedi_device *dev)
{
	struct das16_private_struct *devpriv = dev->private;
	int residue;
	int i;
	static const int disable_limit = 100;
	static const int enable_timeout = 100;

	disable_dma(devpriv->dma_chan);
	residue = get_dma_residue(devpriv->dma_chan);
	for (i = 0; i < disable_limit && (residue % 2); ++i) {
		int j;
		enable_dma(devpriv->dma_chan);
		for (j = 0; j < enable_timeout; ++j) {
			int new_residue;
			udelay(2);
			new_residue = get_dma_residue(devpriv->dma_chan);
			if (new_residue != residue)
				break;
		}
		disable_dma(devpriv->dma_chan);
		residue = get_dma_residue(devpriv->dma_chan);
	}
	if (i == disable_limit) {
		comedi_error(dev, "failed to get an even dma transfer, "
							"could be trouble.");
	}
	return residue;
}

static void das16_interrupt(struct comedi_device *dev)
{
	const struct das16_board *board = comedi_board(dev);
	struct das16_private_struct *devpriv = dev->private;
	unsigned long dma_flags, spin_flags;
	struct comedi_subdevice *s = dev->read_subdev;
	struct comedi_async *async;
	struct comedi_cmd *cmd;
	int num_bytes, residue;
	int buffer_index;

	if (!dev->attached) {
		comedi_error(dev, "premature interrupt");
		return;
	}
	/*  initialize async here to make sure it is not NULL */
	async = s->async;
	cmd = &async->cmd;

	if (devpriv->dma_chan == 0) {
		comedi_error(dev, "interrupt with no dma channel?");
		return;
	}

	spin_lock_irqsave(&dev->spinlock, spin_flags);
	if ((devpriv->control_state & DMA_ENABLE) == 0) {
		spin_unlock_irqrestore(&dev->spinlock, spin_flags);
		DEBUG_PRINT("interrupt while dma disabled?\n");
		return;
	}

	dma_flags = claim_dma_lock();
	clear_dma_ff(devpriv->dma_chan);
	residue = disable_dma_on_even(dev);

	/*  figure out how many points to read */
	if (residue > devpriv->dma_transfer_size) {
		comedi_error(dev, "residue > transfer size!\n");
		async->events |= COMEDI_CB_ERROR | COMEDI_CB_EOA;
		num_bytes = 0;
	} else
		num_bytes = devpriv->dma_transfer_size - residue;

	if (cmd->stop_src == TRIG_COUNT &&
					num_bytes >= devpriv->adc_byte_count) {
		num_bytes = devpriv->adc_byte_count;
		async->events |= COMEDI_CB_EOA;
	}

	buffer_index = devpriv->current_buffer;
	devpriv->current_buffer = (devpriv->current_buffer + 1) % 2;
	devpriv->adc_byte_count -= num_bytes;

	/*  figure out how many bytes for next transfer */
	if (cmd->stop_src == TRIG_COUNT && devpriv->timer_mode == 0 &&
	    devpriv->dma_transfer_size > devpriv->adc_byte_count)
		devpriv->dma_transfer_size = devpriv->adc_byte_count;

	/*  re-enable  dma */
	if ((async->events & COMEDI_CB_EOA) == 0) {
		set_dma_addr(devpriv->dma_chan,
			     devpriv->dma_buffer_addr[devpriv->current_buffer]);
		set_dma_count(devpriv->dma_chan, devpriv->dma_transfer_size);
		enable_dma(devpriv->dma_chan);
		/* reenable conversions for das1600 mode, (stupid hardware) */
		if (board->size > 0x400 && devpriv->timer_mode == 0)
			outb(0x00, dev->iobase + DAS1600_CONV);

	}
	release_dma_lock(dma_flags);

	spin_unlock_irqrestore(&dev->spinlock, spin_flags);

	cfc_write_array_to_buffer(s,
				  devpriv->dma_buffer[buffer_index], num_bytes);

	cfc_handle_events(dev, s);
}

static irqreturn_t das16_dma_interrupt(int irq, void *d)
{
	int status;
	struct comedi_device *dev = d;

	status = inb(dev->iobase + DAS16_STATUS);

	if ((status & DAS16_INT) == 0) {
		DEBUG_PRINT("spurious interrupt\n");
		return IRQ_NONE;
	}

	/* clear interrupt */
	outb(0x00, dev->iobase + DAS16_STATUS);
	das16_interrupt(dev);
	return IRQ_HANDLED;
}

static void das16_timer_interrupt(unsigned long arg)
{
	struct comedi_device *dev = (struct comedi_device *)arg;
	struct das16_private_struct *devpriv = dev->private;

	das16_interrupt(dev);

	if (devpriv->timer_running)
		mod_timer(&devpriv->timer, jiffies + timer_period());
}

static void reg_dump(struct comedi_device *dev)
{
	DEBUG_PRINT("********DAS1600 REGISTER DUMP********\n");
	DEBUG_PRINT("DAS16_MUX: %x\n", inb(dev->iobase + DAS16_MUX));
	DEBUG_PRINT("DAS16_DIO: %x\n", inb(dev->iobase + DAS16_DIO));
	DEBUG_PRINT("DAS16_STATUS: %x\n", inb(dev->iobase + DAS16_STATUS));
	DEBUG_PRINT("DAS16_CONTROL: %x\n", inb(dev->iobase + DAS16_CONTROL));
	DEBUG_PRINT("DAS16_PACER: %x\n", inb(dev->iobase + DAS16_PACER));
	DEBUG_PRINT("DAS16_GAIN: %x\n", inb(dev->iobase + DAS16_GAIN));
	DEBUG_PRINT("DAS16_CNTR_CONTROL: %x\n",
		    inb(dev->iobase + DAS16_CNTR_CONTROL));
	DEBUG_PRINT("DAS1600_CONV: %x\n", inb(dev->iobase + DAS1600_CONV));
	DEBUG_PRINT("DAS1600_BURST: %x\n", inb(dev->iobase + DAS1600_BURST));
	DEBUG_PRINT("DAS1600_ENABLE: %x\n", inb(dev->iobase + DAS1600_ENABLE));
	DEBUG_PRINT("DAS1600_STATUS_B: %x\n",
		    inb(dev->iobase + DAS1600_STATUS_B));
=======
static void das16_ai_munge(struct comedi_device *dev,
			   struct comedi_subdevice *s, void *array,
			   unsigned int num_bytes,
			   unsigned int start_chan_index)
{
	unsigned short *data = array;
	unsigned int num_samples = comedi_bytes_to_samples(s, num_bytes);
	unsigned int i;
	__le16 *buf = array;

	for (i = 0; i < num_samples; i++) {
		data[i] = le16_to_cpu(buf[i]);
		if (s->maxdata == 0x0fff)
			data[i] >>= 4;
		data[i] &= s->maxdata;
	}
}

static int das16_ai_eoc(struct comedi_device *dev,
			struct comedi_subdevice *s,
			struct comedi_insn *insn,
			unsigned long context)
{
	unsigned int status;

	status = inb(dev->iobase + DAS16_STATUS_REG);
	if ((status & DAS16_STATUS_BUSY) == 0)
		return 0;
	return -EBUSY;
}

static int das16_ai_insn_read(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn,
			      unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int range = CR_RANGE(insn->chanspec);
	unsigned int val;
	int ret;
	int i;

	/* set mux and range for single channel */
	das16_ai_set_mux_range(dev, chan, chan, range);

	for (i = 0; i < insn->n; i++) {
		/* trigger conversion */
		outb_p(0, dev->iobase + DAS16_TRIG_REG);

		ret = comedi_timeout(dev, s, insn, das16_ai_eoc, 0);
		if (ret)
			return ret;

		val = inb(dev->iobase + DAS16_AI_MSB_REG) << 8;
		val |= inb(dev->iobase + DAS16_AI_LSB_REG);
		if (s->maxdata == 0x0fff)
			val >>= 4;
		val &= s->maxdata;

		data[i] = val;
	}

	return insn->n;
}

static int das16_ao_insn_write(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn,
			       unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	int i;

	for (i = 0; i < insn->n; i++) {
		unsigned int val = data[i];

		s->readback[chan] = val;

		val <<= 4;

		outb(val & 0xff, dev->iobase + DAS16_AO_LSB_REG(chan));
		outb((val >> 8) & 0xff, dev->iobase + DAS16_AO_MSB_REG(chan));
	}

	return insn->n;
}

static int das16_di_insn_bits(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn,
			      unsigned int *data)
{
	data[1] = inb(dev->iobase + DAS16_DIO_REG) & 0xf;

	return insn->n;
}

static int das16_do_insn_bits(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn,
			      unsigned int *data)
{
	if (comedi_dio_update_state(s, data))
		outb(s->state, dev->iobase + DAS16_DIO_REG);

	data[1] = s->state;

	return insn->n;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int das16_probe(struct comedi_device *dev, struct comedi_devconfig *it)
{
<<<<<<< HEAD
	const struct das16_board *board = comedi_board(dev);
	struct das16_private_struct *devpriv = dev->private;
	int status;
	int diobits;

	/* status is available on all boards */

	status = inb(dev->iobase + DAS16_STATUS);

	if ((status & UNIPOLAR))
		devpriv->ai_unipolar = 1;
	else
		devpriv->ai_unipolar = 0;


	if ((status & DAS16_MUXBIT))
		devpriv->ai_singleended = 1;
	else
		devpriv->ai_singleended = 0;


	/* diobits indicates boards */

	diobits = inb(dev->iobase + DAS16_DIO) & 0xf0;

	printk(KERN_INFO " id bits are 0x%02x\n", diobits);
	if (board->id != diobits) {
		printk(KERN_INFO " requested board's id bits are 0x%x (ignore)\n",
		       board->id);
=======
	const struct das16_board *board = dev->board_ptr;
	int diobits;

	/* diobits indicates boards */
	diobits = inb(dev->iobase + DAS16_DIO_REG) & 0xf0;
	if (board->id != diobits) {
		dev_err(dev->class_dev,
			"requested board's id bits are incorrect (0x%x != 0x%x)\n",
			board->id, diobits);
		return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return 0;
}

<<<<<<< HEAD
static int das1600_mode_detect(struct comedi_device *dev)
{
	struct das16_private_struct *devpriv = dev->private;
	int status = 0;

	status = inb(dev->iobase + DAS1600_STATUS_B);

	if (status & DAS1600_CLK_10MHZ) {
		devpriv->clockbase = 100;
		printk(KERN_INFO " 10MHz pacer clock\n");
	} else {
		devpriv->clockbase = 1000;
		printk(KERN_INFO " 1MHz pacer clock\n");
	}

	reg_dump(dev);

	return 0;
}

static void das16_ai_munge(struct comedi_device *dev,
			   struct comedi_subdevice *s, void *array,
			   unsigned int num_bytes,
			   unsigned int start_chan_index)
{
	const struct das16_board *board = comedi_board(dev);
	unsigned int i, num_samples = num_bytes / sizeof(short);
	short *data = array;

	for (i = 0; i < num_samples; i++) {
		data[i] = le16_to_cpu(data[i]);
		if (board->ai_nbits == 12)
			data[i] = (data[i] >> 4) & 0xfff;

	}
}

/*
 *
 * Options list:
 *   0  I/O base
 *   1  IRQ
 *   2  DMA
 *   3  Clock speed (in MHz)
 */
static int das16_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	const struct das16_board *board = comedi_board(dev);
	struct das16_private_struct *devpriv;
	struct comedi_subdevice *s;
	int ret;
	unsigned int irq;
	unsigned int dma_chan;
	int timer_mode;
	unsigned long flags;
	struct comedi_krange *user_ai_range, *user_ao_range;

#if 0
	irq = it->options[1];
	timer_mode = it->options[8];
#endif
	/* always use time_mode since using irq can drop samples while
	 * waiting for dma done interrupt (due to hardware limitations) */
	irq = 0;
	timer_mode = 1;
	if (timer_mode)
		irq = 0;

	/*  check that clock setting is valid */
	if (it->options[3]) {
		if (it->options[3] != 0 &&
		    it->options[3] != 1 && it->options[3] != 10) {
			printk
			    ("\n Invalid option.  Master clock must be set "
							"to 1 or 10 (MHz)\n");
=======
static void das16_reset(struct comedi_device *dev)
{
	outb(0, dev->iobase + DAS16_STATUS_REG);
	outb(0, dev->iobase + DAS16_CTRL_REG);
	outb(0, dev->iobase + DAS16_PACER_REG);
}

static void das16_alloc_dma(struct comedi_device *dev, unsigned int dma_chan)
{
	struct das16_private_struct *devpriv = dev->private;

	/* only DMA channels 3 and 1 are valid */
	if (!(dma_chan == 1 || dma_chan == 3))
		return;

	/* DMA uses two buffers */
	devpriv->dma = comedi_isadma_alloc(dev, 2, dma_chan, dma_chan,
					   DAS16_DMA_SIZE, COMEDI_ISADMA_READ);
	if (devpriv->dma) {
		setup_timer(&devpriv->timer, das16_timer_interrupt,
			    (unsigned long)dev);
	}
}

static void das16_free_dma(struct comedi_device *dev)
{
	struct das16_private_struct *devpriv = dev->private;

	if (devpriv) {
		if (devpriv->timer.data)
			del_timer_sync(&devpriv->timer);
		comedi_isadma_free(devpriv->dma);
	}
}

static const struct comedi_lrange *das16_ai_range(struct comedi_device *dev,
						  struct comedi_subdevice *s,
						  struct comedi_devconfig *it,
						  unsigned int pg_type,
						  unsigned int status)
{
	unsigned int min = it->options[4];
	unsigned int max = it->options[5];

	/* get any user-defined input range */
	if (pg_type == das16_pg_none && (min || max)) {
		struct comedi_lrange *lrange;
		struct comedi_krange *krange;

		/* allocate single-range range table */
		lrange = comedi_alloc_spriv(s,
					    sizeof(*lrange) + sizeof(*krange));
		if (!lrange)
			return &range_unknown;

		/* initialize ai range */
		lrange->length = 1;
		krange = lrange->range;
		krange->min = min;
		krange->max = max;
		krange->flags = UNIT_volt;

		return lrange;
	}

	/* use software programmable range */
	if (status & DAS16_STATUS_UNIPOLAR)
		return das16_ai_uni_lranges[pg_type];
	return das16_ai_bip_lranges[pg_type];
}

static const struct comedi_lrange *das16_ao_range(struct comedi_device *dev,
						  struct comedi_subdevice *s,
						  struct comedi_devconfig *it)
{
	unsigned int min = it->options[6];
	unsigned int max = it->options[7];

	/* get any user-defined output range */
	if (min || max) {
		struct comedi_lrange *lrange;
		struct comedi_krange *krange;

		/* allocate single-range range table */
		lrange = comedi_alloc_spriv(s,
					    sizeof(*lrange) + sizeof(*krange));
		if (!lrange)
			return &range_unknown;

		/* initialize ao range */
		lrange->length = 1;
		krange = lrange->range;
		krange->min = min;
		krange->max = max;
		krange->flags = UNIT_volt;

		return lrange;
	}

	return &range_unknown;
}

static int das16_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	const struct das16_board *board = dev->board_ptr;
	struct das16_private_struct *devpriv;
	struct comedi_subdevice *s;
	unsigned int osc_base;
	unsigned int status;
	int ret;

	/*  check that clock setting is valid */
	if (it->options[3]) {
		if (it->options[3] != 1 && it->options[3] != 10) {
			dev_err(dev->class_dev,
				"Invalid option. Master clock must be set to 1 or 10 (MHz)\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -EINVAL;
		}
	}

<<<<<<< HEAD
	devpriv = kzalloc(sizeof(*devpriv), GFP_KERNEL);
	if (!devpriv)
		return -ENOMEM;
	dev->private = devpriv;
=======
	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv)
		return -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (board->size < 0x400) {
		ret = comedi_request_region(dev, it->options[0], board->size);
		if (ret)
			return ret;
	} else {
		ret = comedi_request_region(dev, it->options[0], 0x10);
		if (ret)
			return ret;
		/* Request an additional region for the 8255 */
		ret = __comedi_request_region(dev, dev->iobase + 0x400,
					      board->size & 0x3ff);
		if (ret)
			return ret;
		devpriv->extra_iobase = dev->iobase + 0x400;
<<<<<<< HEAD
	}

	/*  probe id bits to make sure they are consistent */
	if (das16_probe(dev, it)) {
		printk(KERN_ERR " id bits do not match selected board, aborting\n");
		return -EINVAL;
	}

	/*  get master clock speed */
	if (board->size < 0x400) {
		if (it->options[3])
			devpriv->clockbase = 1000 / it->options[3];
		else
			devpriv->clockbase = 1000;	/*  1 MHz default */
	} else {
		das1600_mode_detect(dev);
	}

	/* now for the irq */
	if (irq > 1 && irq < 8) {
		ret = request_irq(irq, das16_dma_interrupt, 0,
				  dev->board_name, dev);

		if (ret < 0)
			return ret;
		dev->irq = irq;
		printk(KERN_INFO " ( irq = %u )", irq);
	} else if (irq == 0) {
		printk(" ( no irq )");
	} else {
		printk(" invalid irq\n");
		return -EINVAL;
	}

	/*  initialize dma */
	dma_chan = it->options[2];
	if (dma_chan == 1 || dma_chan == 3) {
		/*  allocate dma buffers */
		int i;
		for (i = 0; i < 2; i++) {
			devpriv->dma_buffer[i] = pci_alloc_consistent(
						NULL, DAS16_DMA_SIZE,
						&devpriv->dma_buffer_addr[i]);

			if (devpriv->dma_buffer[i] == NULL)
				return -ENOMEM;
		}
		if (request_dma(dma_chan, dev->board_name)) {
			printk(KERN_ERR " failed to allocate dma channel %i\n",
			       dma_chan);
			return -EINVAL;
		}
		devpriv->dma_chan = dma_chan;
		flags = claim_dma_lock();
		disable_dma(devpriv->dma_chan);
		set_dma_mode(devpriv->dma_chan, DMA_MODE_READ);
		release_dma_lock(flags);
		printk(KERN_INFO " ( dma = %u)\n", dma_chan);
	} else if (dma_chan == 0) {
		printk(KERN_INFO " ( no dma )\n");
	} else {
		printk(KERN_ERR " invalid dma channel\n");
		return -EINVAL;
	}

	/*  get any user-defined input range */
	if (board->ai_pg == das16_pg_none &&
	    (it->options[4] || it->options[5])) {
		/*  allocate single-range range table */
		devpriv->user_ai_range_table =
		    kmalloc(sizeof(struct comedi_lrange) +
			    sizeof(struct comedi_krange), GFP_KERNEL);
		/*  initialize ai range */
		devpriv->user_ai_range_table->length = 1;
		user_ai_range = devpriv->user_ai_range_table->range;
		user_ai_range->min = it->options[4];
		user_ai_range->max = it->options[5];
		user_ai_range->flags = UNIT_volt;
	}
	/*  get any user-defined output range */
	if (it->options[6] || it->options[7]) {
		/*  allocate single-range range table */
		devpriv->user_ao_range_table =
		    kmalloc(sizeof(struct comedi_lrange) +
			    sizeof(struct comedi_krange), GFP_KERNEL);
		/*  initialize ao range */
		devpriv->user_ao_range_table->length = 1;
		user_ao_range = devpriv->user_ao_range_table->range;
		user_ao_range->min = it->options[6];
		user_ao_range->max = it->options[7];
		user_ao_range->flags = UNIT_volt;
	}

	if (timer_mode) {
		init_timer(&(devpriv->timer));
		devpriv->timer.function = das16_timer_interrupt;
		devpriv->timer.data = (unsigned long)dev;
	}
	devpriv->timer_mode = timer_mode ? 1 : 0;

	ret = comedi_alloc_subdevices(dev, 5);
	if (ret)
		return ret;

	s = &dev->subdevices[0];
	dev->read_subdev = s;
	/* ai */
	if (board->ai) {
		s->type = COMEDI_SUBD_AI;
		s->subdev_flags = SDF_READABLE | SDF_CMD_READ;
		if (devpriv->ai_singleended) {
			s->n_chan = 16;
			s->len_chanlist = 16;
			s->subdev_flags |= SDF_GROUND;
		} else {
			s->n_chan = 8;
			s->len_chanlist = 8;
			s->subdev_flags |= SDF_DIFF;
		}
		s->maxdata = (1 << board->ai_nbits) - 1;
		if (devpriv->user_ai_range_table) { /*  user defined ai range */
			s->range_table = devpriv->user_ai_range_table;
		} else if (devpriv->ai_unipolar) {
			s->range_table = das16_ai_uni_lranges[board->ai_pg];
		} else {
			s->range_table = das16_ai_bip_lranges[board->ai_pg];
		}
		s->insn_read = board->ai;
		s->do_cmdtest = das16_cmd_test;
		s->do_cmd = das16_cmd_exec;
		s->cancel = das16_cancel;
		s->munge = das16_ai_munge;
	} else {
		s->type = COMEDI_SUBD_UNUSED;
	}

	s = &dev->subdevices[1];
	/* ao */
	if (board->ao) {
		s->type = COMEDI_SUBD_AO;
		s->subdev_flags = SDF_WRITABLE;
		s->n_chan = 2;
		s->maxdata = (1 << board->ao_nbits) - 1;
		/*  user defined ao range */
		if (devpriv->user_ao_range_table)
			s->range_table = devpriv->user_ao_range_table;
		else
			s->range_table = &range_unknown;

		s->insn_write = board->ao;
	} else {
		s->type = COMEDI_SUBD_UNUSED;
	}

	s = &dev->subdevices[2];
	/* di */
	if (board->di) {
		s->type = COMEDI_SUBD_DI;
		s->subdev_flags = SDF_READABLE;
		s->n_chan = 4;
		s->maxdata = 1;
		s->range_table = &range_digital;
		s->insn_bits = board->di;
	} else {
		s->type = COMEDI_SUBD_UNUSED;
	}

	s = &dev->subdevices[3];
	/* do */
	if (board->do_) {
		s->type = COMEDI_SUBD_DO;
		s->subdev_flags = SDF_WRITABLE | SDF_READABLE;
		s->n_chan = 4;
		s->maxdata = 1;
		s->range_table = &range_digital;
		s->insn_bits = board->do_;
		/*  initialize digital output lines */
		outb(s->state, dev->iobase + DAS16_DIO);
	} else {
		s->type = COMEDI_SUBD_UNUSED;
	}

	s = &dev->subdevices[4];
	/* 8255 */
	if (board->i8255_offset != 0) {
		subdev_8255_init(dev, s, NULL, (dev->iobase +
						board->i8255_offset));
	} else {
		s->type = COMEDI_SUBD_UNUSED;
=======
		devpriv->can_burst = 1;
	}

	/*  probe id bits to make sure they are consistent */
	if (das16_probe(dev, it))
		return -EINVAL;

	/*  get master clock speed */
	osc_base = I8254_OSC_BASE_1MHZ;
	if (devpriv->can_burst) {
		status = inb(dev->iobase + DAS1600_STATUS_REG);
		if (status & DAS1600_STATUS_CLK_10MHZ)
			osc_base = I8254_OSC_BASE_10MHZ;
	} else {
		if (it->options[3])
			osc_base = I8254_OSC_BASE_1MHZ / it->options[3];
	}

	dev->pacer = comedi_8254_init(dev->iobase + DAS16_TIMER_BASE_REG,
				      osc_base, I8254_IO8, 0);
	if (!dev->pacer)
		return -ENOMEM;

	das16_alloc_dma(dev, it->options[2]);

	ret = comedi_alloc_subdevices(dev, 4 + board->has_8255);
	if (ret)
		return ret;

	status = inb(dev->iobase + DAS16_STATUS_REG);

	/* Analog Input subdevice */
	s = &dev->subdevices[0];
	s->type		= COMEDI_SUBD_AI;
	s->subdev_flags	= SDF_READABLE;
	if (status & DAS16_STATUS_MUXBIT) {
		s->subdev_flags	|= SDF_GROUND;
		s->n_chan	= 16;
	} else {
		s->subdev_flags	|= SDF_DIFF;
		s->n_chan	= 8;
	}
	s->len_chanlist	= s->n_chan;
	s->maxdata	= board->ai_maxdata;
	s->range_table	= das16_ai_range(dev, s, it, board->ai_pg, status);
	s->insn_read	= das16_ai_insn_read;
	if (devpriv->dma) {
		dev->read_subdev = s;
		s->subdev_flags	|= SDF_CMD_READ;
		s->do_cmdtest	= das16_cmd_test;
		s->do_cmd	= das16_cmd_exec;
		s->cancel	= das16_cancel;
		s->munge	= das16_ai_munge;
	}

	/* Analog Output subdevice */
	s = &dev->subdevices[1];
	if (board->has_ao) {
		s->type		= COMEDI_SUBD_AO;
		s->subdev_flags	= SDF_WRITABLE;
		s->n_chan	= 2;
		s->maxdata	= 0x0fff;
		s->range_table	= das16_ao_range(dev, s, it);
		s->insn_write	= das16_ao_insn_write;

		ret = comedi_alloc_subdev_readback(s);
		if (ret)
			return ret;
	} else {
		s->type		= COMEDI_SUBD_UNUSED;
	}

	/* Digital Input subdevice */
	s = &dev->subdevices[2];
	s->type		= COMEDI_SUBD_DI;
	s->subdev_flags	= SDF_READABLE;
	s->n_chan	= 4;
	s->maxdata	= 1;
	s->range_table	= &range_digital;
	s->insn_bits	= das16_di_insn_bits;

	/* Digital Output subdevice */
	s = &dev->subdevices[3];
	s->type		= COMEDI_SUBD_DO;
	s->subdev_flags	= SDF_WRITABLE;
	s->n_chan	= 4;
	s->maxdata	= 1;
	s->range_table	= &range_digital;
	s->insn_bits	= das16_do_insn_bits;

	/* initialize digital output lines */
	outb(s->state, dev->iobase + DAS16_DIO_REG);

	/* 8255 Digital I/O subdevice */
	if (board->has_8255) {
		s = &dev->subdevices[4];
		ret = subdev_8255_init(dev, s, NULL, board->i8255_offset);
		if (ret)
			return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	das16_reset(dev);
	/* set the interrupt level */
<<<<<<< HEAD
	devpriv->control_state = DAS16_IRQ(dev->irq);
	outb(devpriv->control_state, dev->iobase + DAS16_CONTROL);

	/*  turn on das1600 mode if available */
	if (board->size > 0x400) {
		outb(DAS1600_ENABLE_VAL, dev->iobase + DAS1600_ENABLE);
		outb(0, dev->iobase + DAS1600_CONV);
		outb(0, dev->iobase + DAS1600_BURST);
=======
	devpriv->ctrl_reg = DAS16_CTRL_IRQ(dev->irq);
	outb(devpriv->ctrl_reg, dev->iobase + DAS16_CTRL_REG);

	if (devpriv->can_burst) {
		outb(DAS1600_ENABLE_VAL, dev->iobase + DAS1600_ENABLE_REG);
		outb(0, dev->iobase + DAS1600_CONV_REG);
		outb(0, dev->iobase + DAS1600_BURST_REG);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return 0;
}

static void das16_detach(struct comedi_device *dev)
{
<<<<<<< HEAD
	const struct das16_board *board = comedi_board(dev);
	struct das16_private_struct *devpriv = dev->private;

	das16_reset(dev);
	comedi_spriv_free(dev, 4);
	if (devpriv) {
		int i;
		for (i = 0; i < 2; i++) {
			if (devpriv->dma_buffer[i])
				pci_free_consistent(NULL, DAS16_DMA_SIZE,
						    devpriv->dma_buffer[i],
						    devpriv->
						    dma_buffer_addr[i]);
		}
		if (devpriv->dma_chan)
			free_dma(devpriv->dma_chan);
		kfree(devpriv->user_ai_range_table);
		kfree(devpriv->user_ao_range_table);
	}
	if (devpriv->extra_iobase)
		release_region(devpriv->extra_iobase, board->size & 0x3ff);
	comedi_legacy_detach(dev);
}

static const struct das16_board das16_boards[] = {
	{
		.name		= "das-16",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 15000,
		.ai_pg		= das16_pg_none,
		.ao		= das16_ao_winsn,
		.ao_nbits	= 12,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x10,
		.i8254_offset	= 0x0c,
		.size		= 0x14,
		.id		= 0x00,
	}, {
		.name		= "das-16g",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 15000,
		.ai_pg		= das16_pg_none,
		.ao		= das16_ao_winsn,
		.ao_nbits	= 12,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x10,
		.i8254_offset	= 0x0c,
		.size		= 0x14,
		.id		= 0x00,
	}, {
		.name		= "das-16f",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 8500,
		.ai_pg		= das16_pg_none,
		.ao		= das16_ao_winsn,
		.ao_nbits	= 12,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x10,
		.i8254_offset	= 0x0c,
		.size		= 0x14,
		.id		= 0x00,
	}, {
		.name		= "cio-das16",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 20000,
		.ai_pg		= das16_pg_none,
		.ao		= das16_ao_winsn,
		.ao_nbits	= 12,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x10,
		.i8254_offset	= 0x0c,
		.size		= 0x14,
		.id		= 0x80,
	}, {
		.name		= "cio-das16/f",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_none,
		.ao		= das16_ao_winsn,
		.ao_nbits	= 12,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x10,
		.i8254_offset	= 0x0c,
		.size		= 0x14,
		.id		= 0x80,
	}, {
		.name		= "cio-das16/jr",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 7692,
		.ai_pg		= das16_pg_16jr,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0,
		.i8254_offset	= 0x0c,
		.size		= 0x10,
		.id		= 0x00,
	}, {
		.name		= "pc104-das16jr",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 3300,
		.ai_pg		= das16_pg_16jr,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0,
		.i8254_offset	= 0x0c,
		.size		= 0x10,
		.id		= 0x00,
	}, {
		.name		= "cio-das16jr/16",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 16,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_16jr_16,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0,
		.i8254_offset	= 0x0c,
		.size		= 0x10,
		.id		= 0x00,
	}, {
		.name		= "pc104-das16jr/16",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 16,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_16jr_16,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0,
		.i8254_offset	= 0x0c,
		.size		= 0x10,
		.id		= 0x00,
	}, {
		.name		= "das-1201",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 20000,
		.ai_pg		= das16_pg_none,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x400,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0x20,
	}, {
		.name		= "das-1202",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_none,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x400,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0x20,
	}, {
		.name		= "das-1401",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1601,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x0,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "das-1402",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1602,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x0,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "das-1601",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1601,
		.ao		= das16_ao_winsn,
		.ao_nbits	= 12,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x400,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "das-1602",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1602,
		.ao		= das16_ao_winsn,
		.ao_nbits	= 12,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x400,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1401/12",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 6250,
		.ai_pg		= das16_pg_1601,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1402/12",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 6250,
		.ai_pg		= das16_pg_1602,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1402/16",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 16,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1602,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1601/12",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 6250,
		.ai_pg		= das16_pg_1601,
		.ao		= das16_ao_winsn,
		.ao_nbits	= 12,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x400,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1602/12",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1602,
		.ao		= das16_ao_winsn,
		.ao_nbits	= 12,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x400,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das1602/16",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 16,
		.ai_speed	= 10000,
		.ai_pg		= das16_pg_1602,
		.ao		= das16_ao_winsn,
		.ao_nbits	= 12,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0x400,
		.i8254_offset	= 0x0c,
		.size		= 0x408,
		.id		= 0xc0,
	}, {
		.name		= "cio-das16/330",
		.ai		= das16_ai_rinsn,
		.ai_nbits	= 12,
		.ai_speed	= 3030,
		.ai_pg		= das16_pg_16jr,
		.ao		= NULL,
		.di		= das16_di_rbits,
		.do_		= das16_do_wbits,
		.i8255_offset	= 0,
		.i8254_offset	= 0x0c,
		.size		= 0x14,
		.id		= 0xf0,
	},
};

=======
	const struct das16_board *board = dev->board_ptr;
	struct das16_private_struct *devpriv = dev->private;

	if (devpriv) {
		if (dev->iobase)
			das16_reset(dev);
		das16_free_dma(dev);

		if (devpriv->extra_iobase)
			release_region(devpriv->extra_iobase,
				       board->size & 0x3ff);
	}

	comedi_legacy_detach(dev);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct comedi_driver das16_driver = {
	.driver_name	= "das16",
	.module		= THIS_MODULE,
	.attach		= das16_attach,
	.detach		= das16_detach,
	.board_name	= &das16_boards[0].name,
	.num_names	= ARRAY_SIZE(das16_boards),
	.offset		= sizeof(das16_boards[0]),
};
module_comedi_driver(das16_driver);

MODULE_AUTHOR("Comedi http://www.comedi.org");
<<<<<<< HEAD
MODULE_DESCRIPTION("Comedi low-level driver");
=======
MODULE_DESCRIPTION("Comedi driver for DAS16 compatible boards");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_LICENSE("GPL");
