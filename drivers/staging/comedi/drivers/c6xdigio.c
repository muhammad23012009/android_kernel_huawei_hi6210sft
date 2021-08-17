/*
<<<<<<< HEAD
   comedi/drivers/c6xdigio.c

   Hardware driver for Mechatronic Systems Inc. C6x_DIGIO DSP daughter card.
   (http://robot0.ge.uiuc.edu/~spong/mecha/)

   COMEDI - Linux Control and Measurement Device Interface
   Copyright (C) 1999 Dan Block

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
Driver: c6xdigio
Description: Mechatronic Systems Inc. C6x_DIGIO DSP daughter card
Author: Dan Block
Status: unknown
Devices: [Mechatronic Systems Inc.] C6x_DIGIO DSP daughter card (c6xdigio)
Updated: Sun Nov 20 20:18:34 EST 2005

This driver will not work with a 2.4 kernel.
http://robot0.ge.uiuc.edu/~spong/mecha/

*/
=======
 * c6xdigio.c
 * Hardware driver for Mechatronic Systems Inc. C6x_DIGIO DSP daughter card.
 * http://web.archive.org/web/%2A/http://robot0.ge.uiuc.edu/~spong/mecha/
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 1999 Dan Block
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
 * Driver: c6xdigio
 * Description: Mechatronic Systems Inc. C6x_DIGIO DSP daughter card
 * Author: Dan Block
 * Status: unknown
 * Devices: [Mechatronic Systems Inc.] C6x_DIGIO DSP daughter card (c6xdigio)
 * Updated: Sun Nov 20 20:18:34 EST 2005
 *
 * Configuration Options:
 *	[0] - base address
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/ioport.h>
#include <linux/delay.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/interrupt.h>
#include <linux/timex.h>
#include <linux/timer.h>
#include <linux/io.h>
#include <linux/pnp.h>

#include "../comedidev.h"

<<<<<<< HEAD
static u8 ReadByteFromHwPort(unsigned long addr)
{
	u8 result = inb(addr);
	return result;
}

static void WriteByteToHwPort(unsigned long addr, u8 val)
{
	outb_p(val, addr);
}

#define C6XDIGIO_SIZE 3

/*
 * port offsets
 */
#define C6XDIGIO_PARALLEL_DATA 0
#define C6XDIGIO_PARALLEL_STATUS 1
#define C6XDIGIO_PARALLEL_CONTROL 2
struct pwmbitstype {
	unsigned sb0:2;
	unsigned sb1:2;
	unsigned sb2:2;
	unsigned sb3:2;
	unsigned sb4:2;
};
union pwmcmdtype {
	unsigned cmd;		/*  assuming here that int is 32bit */
	struct pwmbitstype bits;
};
struct encbitstype {
	unsigned sb0:3;
	unsigned sb1:3;
	unsigned sb2:3;
	unsigned sb3:3;
	unsigned sb4:3;
	unsigned sb5:3;
	unsigned sb6:3;
	unsigned sb7:3;
};
union encvaluetype {
	unsigned value;
	struct encbitstype bits;
};

#define C6XDIGIO_TIME_OUT 20

static void C6X_pwmInit(unsigned long baseAddr)
{
	int timeout = 0;

/* printk("Inside C6X_pwmInit\n"); */

	WriteByteToHwPort(baseAddr, 0x70);
	while (((ReadByteFromHwPort(baseAddr + 1) & 0x80) == 0)
	       && (timeout < C6XDIGIO_TIME_OUT)) {
		timeout++;
	}

	WriteByteToHwPort(baseAddr, 0x74);
	timeout = 0;
	while (((ReadByteFromHwPort(baseAddr + 1) & 0x80) == 0x80)
	       && (timeout < C6XDIGIO_TIME_OUT)) {
		timeout++;
	}

	WriteByteToHwPort(baseAddr, 0x70);
	timeout = 0;
	while (((ReadByteFromHwPort(baseAddr + 1) & 0x80) == 0x0)
	       && (timeout < C6XDIGIO_TIME_OUT)) {
		timeout++;
	}

	WriteByteToHwPort(baseAddr, 0x0);
	timeout = 0;
	while (((ReadByteFromHwPort(baseAddr + 1) & 0x80) == 0x80)
	       && (timeout < C6XDIGIO_TIME_OUT)) {
		timeout++;
	}

}

static void C6X_pwmOutput(unsigned long baseAddr, unsigned channel, int value)
{
	unsigned ppcmd;
	union pwmcmdtype pwm;
	int timeout = 0;
	unsigned tmp;

	/* printk("Inside C6X_pwmOutput\n"); */

	pwm.cmd = value;
	if (pwm.cmd > 498)
		pwm.cmd = 498;
	if (pwm.cmd < 2)
		pwm.cmd = 2;

	if (channel == 0) {
		ppcmd = 0x28;
	} else {		/*  if channel == 1 */
		ppcmd = 0x30;
	}			/* endif */

	WriteByteToHwPort(baseAddr, ppcmd + pwm.bits.sb0);
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}

	WriteByteToHwPort(baseAddr, ppcmd + pwm.bits.sb1 + 0x4);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0x80) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}

	WriteByteToHwPort(baseAddr, ppcmd + pwm.bits.sb2);
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}

	WriteByteToHwPort(baseAddr, ppcmd + pwm.bits.sb3 + 0x4);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0x80) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}

	WriteByteToHwPort(baseAddr, ppcmd + pwm.bits.sb4);
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}

	WriteByteToHwPort(baseAddr, 0x0);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0x80) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}

}

static int C6X_encInput(unsigned long baseAddr, unsigned channel)
{
	unsigned ppcmd;
	union encvaluetype enc;
	int timeout = 0;
	int tmp;

	/* printk("Inside C6X_encInput\n"); */

	enc.value = 0;
	if (channel == 0)
		ppcmd = 0x48;
	else
		ppcmd = 0x50;

	WriteByteToHwPort(baseAddr, ppcmd);
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}

	enc.bits.sb0 = ((ReadByteFromHwPort(baseAddr + 1) >> 3) & 0x7);
	WriteByteToHwPort(baseAddr, ppcmd + 0x4);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0x80) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}
	enc.bits.sb1 = ((ReadByteFromHwPort(baseAddr + 1) >> 3) & 0x7);
	WriteByteToHwPort(baseAddr, ppcmd);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}
	enc.bits.sb2 = ((ReadByteFromHwPort(baseAddr + 1) >> 3) & 0x7);
	WriteByteToHwPort(baseAddr, ppcmd + 0x4);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0x80) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}
	enc.bits.sb3 = ((ReadByteFromHwPort(baseAddr + 1) >> 3) & 0x7);
	WriteByteToHwPort(baseAddr, ppcmd);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}
	enc.bits.sb4 = ((ReadByteFromHwPort(baseAddr + 1) >> 3) & 0x7);
	WriteByteToHwPort(baseAddr, ppcmd + 0x4);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0x80) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}
	enc.bits.sb5 = ((ReadByteFromHwPort(baseAddr + 1) >> 3) & 0x7);
	WriteByteToHwPort(baseAddr, ppcmd);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0x0) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}
	enc.bits.sb6 = ((ReadByteFromHwPort(baseAddr + 1) >> 3) & 0x7);
	WriteByteToHwPort(baseAddr, ppcmd + 0x4);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0x80) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}
	enc.bits.sb7 = ((ReadByteFromHwPort(baseAddr + 1) >> 3) & 0x7);
	WriteByteToHwPort(baseAddr, ppcmd);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0x0) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}

	WriteByteToHwPort(baseAddr, 0x0);
	timeout = 0;
	tmp = ReadByteFromHwPort(baseAddr + 1);
	while (((tmp & 0x80) == 0x80) && (timeout < C6XDIGIO_TIME_OUT)) {
		tmp = ReadByteFromHwPort(baseAddr + 1);
		timeout++;
	}

	return enc.value ^ 0x800000;
}

static void C6X_encResetAll(unsigned long baseAddr)
{
	unsigned timeout = 0;

/* printk("Inside C6X_encResetAll\n"); */

	WriteByteToHwPort(baseAddr, 0x68);
	while (((ReadByteFromHwPort(baseAddr + 1) & 0x80) == 0)
	       && (timeout < C6XDIGIO_TIME_OUT)) {
		timeout++;
	}
	WriteByteToHwPort(baseAddr, 0x6C);
	timeout = 0;
	while (((ReadByteFromHwPort(baseAddr + 1) & 0x80) == 0x80)
	       && (timeout < C6XDIGIO_TIME_OUT)) {
		timeout++;
	}
	WriteByteToHwPort(baseAddr, 0x68);
	timeout = 0;
	while (((ReadByteFromHwPort(baseAddr + 1) & 0x80) == 0x0)
	       && (timeout < C6XDIGIO_TIME_OUT)) {
		timeout++;
	}
	WriteByteToHwPort(baseAddr, 0x0);
	timeout = 0;
	while (((ReadByteFromHwPort(baseAddr + 1) & 0x80) == 0x80)
	       && (timeout < C6XDIGIO_TIME_OUT)) {
		timeout++;
	}
}

static int c6xdigio_pwmo_insn_read(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn, unsigned int *data)
{
	printk(KERN_DEBUG "c6xdigio_pwmo_insn_read %x\n", insn->n);
	return insn->n;
}

static int c6xdigio_pwmo_insn_write(struct comedi_device *dev,
				    struct comedi_subdevice *s,
				    struct comedi_insn *insn,
				    unsigned int *data)
{
	int i;
	int chan = CR_CHAN(insn->chanspec);

	/*   printk("c6xdigio_pwmo_insn_write %x\n", insn->n); */
	for (i = 0; i < insn->n; i++) {
		C6X_pwmOutput(dev->iobase, chan, data[i]);
		/*    devpriv->ao_readback[chan] = data[i]; */
	}
	return i;
}

/* static int c6xdigio_ei_init_insn_read(struct comedi_device *dev, */
/* struct comedi_subdevice *s, */
/* struct comedi_insn *insn, */
/* unsigned int *data) */
/* { */
/* printk("c6xdigio_ei_init_insn_read %x\n", insn->n); */
/* return insn->n; */
/* } */

/* static int c6xdigio_ei_init_insn_write(struct comedi_device *dev, */
/* struct comedi_subdevice *s, */
/* struct comedi_insn *insn, */
/* unsigned int *data) */
/* { */
/* int i; */
/* int chan = CR_CHAN(insn->chanspec); */
      /*  *//* C6X_encResetAll( dev->iobase ); */
      /*  *//* return insn->n; */
/* } */

static int c6xdigio_ei_insn_read(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 struct comedi_insn *insn, unsigned int *data)
{
	/*   printk("c6xdigio_ei__insn_read %x\n", insn->n); */
	int n;
	int chan = CR_CHAN(insn->chanspec);

	for (n = 0; n < insn->n; n++)
		data[n] = (C6X_encInput(dev->iobase, chan) & 0xffffff);

	return n;
}

static void board_init(struct comedi_device *dev)
{

	/* printk("Inside board_init\n"); */

	C6X_pwmInit(dev->iobase);
	C6X_encResetAll(dev->iobase);

=======
/*
 * Register I/O map
 */
#define C6XDIGIO_DATA_REG	0x00
#define C6XDIGIO_DATA_CHAN(x)	(((x) + 1) << 4)
#define C6XDIGIO_DATA_PWM	BIT(5)
#define C6XDIGIO_DATA_ENCODER	BIT(6)
#define C6XDIGIO_STATUS_REG	0x01
#define C6XDIGIO_CTRL_REG	0x02

#define C6XDIGIO_TIME_OUT 20

static int c6xdigio_chk_status(struct comedi_device *dev, unsigned long context)
{
	unsigned int status;
	int timeout = 0;

	do {
		status = inb(dev->iobase + C6XDIGIO_STATUS_REG);
		if ((status & 0x80) != context)
			return 0;
		timeout++;
	} while  (timeout < C6XDIGIO_TIME_OUT);

	return -EBUSY;
}

static int c6xdigio_write_data(struct comedi_device *dev,
			       unsigned int val, unsigned int status)
{
	outb_p(val, dev->iobase + C6XDIGIO_DATA_REG);
	return c6xdigio_chk_status(dev, status);
}

static int c6xdigio_get_encoder_bits(struct comedi_device *dev,
				     unsigned int *bits,
				     unsigned int cmd,
				     unsigned int status)
{
	unsigned int val;

	val = inb(dev->iobase + C6XDIGIO_STATUS_REG);
	val >>= 3;
	val &= 0x07;

	*bits = val;

	return c6xdigio_write_data(dev, cmd, status);
}

static void c6xdigio_pwm_write(struct comedi_device *dev,
			       unsigned int chan, unsigned int val)
{
	unsigned int cmd = C6XDIGIO_DATA_PWM | C6XDIGIO_DATA_CHAN(chan);
	unsigned int bits;

	if (val > 498)
		val = 498;
	if (val < 2)
		val = 2;

	bits = (val >> 0) & 0x03;
	c6xdigio_write_data(dev, cmd | bits | (0 << 2), 0x00);
	bits = (val >> 2) & 0x03;
	c6xdigio_write_data(dev, cmd | bits | (1 << 2), 0x80);
	bits = (val >> 4) & 0x03;
	c6xdigio_write_data(dev, cmd | bits | (0 << 2), 0x00);
	bits = (val >> 6) & 0x03;
	c6xdigio_write_data(dev, cmd | bits | (1 << 2), 0x80);
	bits = (val >> 8) & 0x03;
	c6xdigio_write_data(dev, cmd | bits | (0 << 2), 0x00);

	c6xdigio_write_data(dev, 0x00, 0x80);
}

static int c6xdigio_encoder_read(struct comedi_device *dev,
				 unsigned int chan)
{
	unsigned int cmd = C6XDIGIO_DATA_ENCODER | C6XDIGIO_DATA_CHAN(chan);
	unsigned int val = 0;
	unsigned int bits;

	c6xdigio_write_data(dev, cmd, 0x00);

	c6xdigio_get_encoder_bits(dev, &bits, cmd | (1 << 2), 0x80);
	val |= (bits << 0);

	c6xdigio_get_encoder_bits(dev, &bits, cmd | (0 << 2), 0x00);
	val |= (bits << 3);

	c6xdigio_get_encoder_bits(dev, &bits, cmd | (1 << 2), 0x80);
	val |= (bits << 6);

	c6xdigio_get_encoder_bits(dev, &bits, cmd | (0 << 2), 0x00);
	val |= (bits << 9);

	c6xdigio_get_encoder_bits(dev, &bits, cmd | (1 << 2), 0x80);
	val |= (bits << 12);

	c6xdigio_get_encoder_bits(dev, &bits, cmd | (0 << 2), 0x00);
	val |= (bits << 15);

	c6xdigio_get_encoder_bits(dev, &bits, cmd | (1 << 2), 0x80);
	val |= (bits << 18);

	c6xdigio_get_encoder_bits(dev, &bits, cmd | (0 << 2), 0x00);
	val |= (bits << 21);

	c6xdigio_write_data(dev, 0x00, 0x80);

	return val;
}

static int c6xdigio_pwm_insn_write(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn,
				   unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int val = (s->state >> (16 * chan)) & 0xffff;
	int i;

	for (i = 0; i < insn->n; i++) {
		val = data[i];
		c6xdigio_pwm_write(dev, chan, val);
	}

	/*
	 * There are only 2 PWM channels and they have a maxdata of 500.
	 * Instead of allocating private data to save the values in for
	 * readback this driver just packs the values for the two channels
	 * in the s->state.
	 */
	s->state &= (0xffff << (16 * chan));
	s->state |= (val << (16 * chan));

	return insn->n;
}

static int c6xdigio_pwm_insn_read(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn,
				  unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int val;
	int i;

	val = (s->state >> (16 * chan)) & 0xffff;

	for (i = 0; i < insn->n; i++)
		data[i] = val;

	return insn->n;
}

static int c6xdigio_encoder_insn_read(struct comedi_device *dev,
				      struct comedi_subdevice *s,
				      struct comedi_insn *insn,
				      unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int val;
	int i;

	for (i = 0; i < insn->n; i++) {
		val = c6xdigio_encoder_read(dev, chan);

		/* munge two's complement value to offset binary */
		data[i] = comedi_offset_munge(s, val);
	}

	return insn->n;
}

static void c6xdigio_init(struct comedi_device *dev)
{
	/* Initialize the PWM */
	c6xdigio_write_data(dev, 0x70, 0x00);
	c6xdigio_write_data(dev, 0x74, 0x80);
	c6xdigio_write_data(dev, 0x70, 0x00);
	c6xdigio_write_data(dev, 0x00, 0x80);

	/* Reset the encoders */
	c6xdigio_write_data(dev, 0x68, 0x00);
	c6xdigio_write_data(dev, 0x6c, 0x80);
	c6xdigio_write_data(dev, 0x68, 0x00);
	c6xdigio_write_data(dev, 0x00, 0x80);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static const struct pnp_device_id c6xdigio_pnp_tbl[] = {
	/* Standard LPT Printer Port */
	{.id = "PNP0400", .driver_data = 0},
	/* ECP Printer Port */
	{.id = "PNP0401", .driver_data = 0},
	{}
};

static struct pnp_driver c6xdigio_pnp_driver = {
	.name = "c6xdigio",
	.id_table = c6xdigio_pnp_tbl,
};

static int c6xdigio_attach(struct comedi_device *dev,
			   struct comedi_devconfig *it)
{
	struct comedi_subdevice *s;
	int ret;

<<<<<<< HEAD
	ret = comedi_request_region(dev, it->options[0], C6XDIGIO_SIZE);
=======
	ret = comedi_request_region(dev, it->options[0], 0x03);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret)
		return ret;

	ret = comedi_alloc_subdevices(dev, 2);
	if (ret)
		return ret;

	/*  Make sure that PnP ports get activated */
	pnp_register_driver(&c6xdigio_pnp_driver);

	s = &dev->subdevices[0];
	/* pwm output subdevice */
<<<<<<< HEAD
	s->type = COMEDI_SUBD_AO;	/*  Not sure what to put here */
	s->subdev_flags = SDF_WRITEABLE;
	s->n_chan = 2;
	/*      s->trig[0] = c6xdigio_pwmo; */
	s->insn_read = c6xdigio_pwmo_insn_read;
	s->insn_write = c6xdigio_pwmo_insn_write;
	s->maxdata = 500;
	s->range_table = &range_bipolar10;	/*  A suitable lie */

	s = &dev->subdevices[1];
	/* encoder (counter) subdevice */
	s->type = COMEDI_SUBD_COUNTER;
	s->subdev_flags = SDF_READABLE | SDF_LSAMPL;
	s->n_chan = 2;
	/* s->trig[0] = c6xdigio_ei; */
	s->insn_read = c6xdigio_ei_insn_read;
	s->maxdata = 0xffffff;
	s->range_table = &range_unknown;

	/*	s = &dev->subdevices[2]; */
	/* pwm output subdevice */
	/*	s->type = COMEDI_SUBD_COUNTER;  // Not sure what to put here */
	/*	s->subdev_flags = SDF_WRITEABLE; */
	/*	s->n_chan = 1; */
	/*	s->trig[0] = c6xdigio_ei_init; */
	/*	s->insn_read = c6xdigio_ei_init_insn_read; */
	/*	s->insn_write = c6xdigio_ei_init_insn_write; */
	/*	s->maxdata = 0xFFFF;  // Really just a don't care */
	/*	s->range_table = &range_unknown; // Not sure what to put here */

	/*  I will call this init anyway but more than likely the DSP board */
	/*  will not be connected when device driver is loaded. */
	board_init(dev);
=======
	s->type		= COMEDI_SUBD_PWM;
	s->subdev_flags	= SDF_WRITABLE;
	s->n_chan	= 2;
	s->maxdata	= 500;
	s->range_table	= &range_unknown;
	s->insn_write	= c6xdigio_pwm_insn_write;
	s->insn_read	= c6xdigio_pwm_insn_read;

	s = &dev->subdevices[1];
	/* encoder (counter) subdevice */
	s->type		= COMEDI_SUBD_COUNTER;
	s->subdev_flags	= SDF_READABLE | SDF_LSAMPL;
	s->n_chan	= 2;
	s->maxdata	= 0xffffff;
	s->range_table	= &range_unknown;
	s->insn_read	= c6xdigio_encoder_insn_read;

	/*  I will call this init anyway but more than likely the DSP board */
	/*  will not be connected when device driver is loaded. */
	c6xdigio_init(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static void c6xdigio_detach(struct comedi_device *dev)
{
	comedi_legacy_detach(dev);
	pnp_unregister_driver(&c6xdigio_pnp_driver);
}

static struct comedi_driver c6xdigio_driver = {
	.driver_name	= "c6xdigio",
	.module		= THIS_MODULE,
	.attach		= c6xdigio_attach,
	.detach		= c6xdigio_detach,
};
module_comedi_driver(c6xdigio_driver);

MODULE_AUTHOR("Comedi http://www.comedi.org");
<<<<<<< HEAD
MODULE_DESCRIPTION("Comedi low-level driver");
=======
MODULE_DESCRIPTION("Comedi driver for the C6x_DIGIO DSP daughter card");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_LICENSE("GPL");
