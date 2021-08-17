/*
<<<<<<< HEAD
    comedi/drivers/amplc_dio200_common.c

    Common support code for "amplc_dio200" and "amplc_dio200_pci".

    Copyright (C) 2005-2013 MEV Ltd. <http://www.mev.co.uk/>

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 1998,2000 David A. Schleef <ds@schleef.org>

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

#include <linux/interrupt.h>
#include <linux/slab.h>
=======
 * comedi/drivers/amplc_dio200_common.c
 *
 * Common support code for "amplc_dio200" and "amplc_dio200_pci".
 *
 * Copyright (C) 2005-2013 MEV Ltd. <http://www.mev.co.uk/>
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 1998,2000 David A. Schleef <ds@schleef.org>
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

#include <linux/module.h>
#include <linux/interrupt.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "../comedidev.h"

#include "amplc_dio200.h"
<<<<<<< HEAD
#include "comedi_fc.h"
#include "8253.h"

/* 8255 control register bits */
#define CR_C_LO_IO	0x01
#define CR_B_IO		0x02
#define CR_B_MODE	0x04
#define CR_C_HI_IO	0x08
#define CR_A_IO		0x10
#define CR_A_MODE(a)	((a)<<5)
#define CR_CW		0x80
=======
#include "comedi_8254.h"
#include "8255.h"		/* only for register defines */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* 200 series registers */
#define DIO200_IO_SIZE		0x20
#define DIO200_PCIE_IO_SIZE	0x4000
<<<<<<< HEAD
#define DIO200_XCLK_SCE		0x18	/* Group X clock selection register */
#define DIO200_YCLK_SCE		0x19	/* Group Y clock selection register */
#define DIO200_ZCLK_SCE		0x1a	/* Group Z clock selection register */
#define DIO200_XGAT_SCE		0x1b	/* Group X gate selection register */
#define DIO200_YGAT_SCE		0x1c	/* Group Y gate selection register */
#define DIO200_ZGAT_SCE		0x1d	/* Group Z gate selection register */
=======
#define DIO200_CLK_SCE(x)	(0x18 + (x))	/* Group X/Y/Z clock sel reg */
#define DIO200_GAT_SCE(x)	(0x1b + (x))	/* Group X/Y/Z gate sel reg */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define DIO200_INT_SCE		0x1e	/* Interrupt enable/status register */
/* Extra registers for new PCIe boards */
#define DIO200_ENHANCE		0x20	/* 1 to enable enhanced features */
#define DIO200_VERSION		0x24	/* Hardware version register */
#define DIO200_TS_CONFIG	0x600	/* Timestamp timer config register */
#define DIO200_TS_COUNT		0x602	/* Timestamp timer count register */

/*
 * Functions for constructing value for DIO_200_?CLK_SCE and
 * DIO_200_?GAT_SCE registers:
 *
 * 'which' is: 0 for CTR-X1, CTR-Y1, CTR-Z1; 1 for CTR-X2, CTR-Y2 or CTR-Z2.
 * 'chan' is the channel: 0, 1 or 2.
 * 'source' is the signal source: 0 to 7, or 0 to 31 for "enhanced" boards.
 */
static unsigned char clk_gat_sce(unsigned int which, unsigned int chan,
				 unsigned int source)
{
	return (which << 5) | (chan << 3) |
	       ((source & 030) << 3) | (source & 007);
}

static unsigned char clk_sce(unsigned int which, unsigned int chan,
			     unsigned int source)
{
	return clk_gat_sce(which, chan, source);
}

static unsigned char gat_sce(unsigned int which, unsigned int chan,
			     unsigned int source)
{
	return clk_gat_sce(which, chan, source);
}

/*
 * Periods of the internal clock sources in nanoseconds.
 */
static const unsigned int clock_period[32] = {
	[1] = 100,		/* 10 MHz */
	[2] = 1000,		/* 1 MHz */
	[3] = 10000,		/* 100 kHz */
	[4] = 100000,		/* 10 kHz */
	[5] = 1000000,		/* 1 kHz */
	[11] = 50,		/* 20 MHz (enhanced boards) */
	/* clock sources 12 and later reserved for enhanced boards */
};

/*
 * Timestamp timer configuration register (for new PCIe boards).
 */
#define TS_CONFIG_RESET		0x100	/* Reset counter to zero. */
#define TS_CONFIG_CLK_SRC_MASK	0x0FF	/* Clock source. */
#define TS_CONFIG_MAX_CLK_SRC	2	/* Maximum clock source value. */

/*
 * Periods of the timestamp timer clock sources in nanoseconds.
 */
static const unsigned int ts_clock_period[TS_CONFIG_MAX_CLK_SRC + 1] = {
	1,			/* 1 nanosecond (but with 20 ns granularity). */
	1000,			/* 1 microsecond. */
	1000000,		/* 1 millisecond. */
};

<<<<<<< HEAD
struct dio200_subdev_8254 {
	unsigned int ofs;		/* Counter base offset */
	unsigned int clk_sce_ofs;	/* CLK_SCE base address */
	unsigned int gat_sce_ofs;	/* GAT_SCE base address */
	int which;			/* Bit 5 of CLK_SCE or GAT_SCE */
	unsigned int clock_src[3];	/* Current clock sources */
	unsigned int gate_src[3];	/* Current gate sources */
	spinlock_t spinlock;
};

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct dio200_subdev_8255 {
	unsigned int ofs;		/* DIO base offset */
};

struct dio200_subdev_intr {
<<<<<<< HEAD
	spinlock_t spinlock;
	unsigned int ofs;
	unsigned int valid_isns;
	unsigned int enabled_isns;
	unsigned int stopcount;
	bool active:1;
	bool continuous:1;
};

static inline const struct dio200_layout *
dio200_board_layout(const struct dio200_board *board)
{
	return &board->layout;
}

static inline const struct dio200_layout *
dio200_dev_layout(struct comedi_device *dev)
{
	return dio200_board_layout(comedi_board(dev));
}

/*
 * Read 8-bit register.
 */
static unsigned char dio200_read8(struct comedi_device *dev,
				  unsigned int offset)
{
	const struct dio200_board *thisboard = comedi_board(dev);
	struct dio200_private *devpriv = dev->private;

	offset <<= thisboard->mainshift;
	if (devpriv->io.regtype == io_regtype)
		return inb(devpriv->io.u.iobase + offset);
	else
		return readb(devpriv->io.u.membase + offset);
}

/*
 * Write 8-bit register.
 */
static void dio200_write8(struct comedi_device *dev, unsigned int offset,
			  unsigned char val)
{
	const struct dio200_board *thisboard = comedi_board(dev);
	struct dio200_private *devpriv = dev->private;

	offset <<= thisboard->mainshift;
	if (devpriv->io.regtype == io_regtype)
		outb(val, devpriv->io.u.iobase + offset);
	else
		writeb(val, devpriv->io.u.membase + offset);
}

/*
 * Read 32-bit register.
 */
static unsigned int dio200_read32(struct comedi_device *dev,
				  unsigned int offset)
{
	const struct dio200_board *thisboard = comedi_board(dev);
	struct dio200_private *devpriv = dev->private;

	offset <<= thisboard->mainshift;
	if (devpriv->io.regtype == io_regtype)
		return inl(devpriv->io.u.iobase + offset);
	else
		return readl(devpriv->io.u.membase + offset);
}

/*
 * Write 32-bit register.
 */
static void dio200_write32(struct comedi_device *dev, unsigned int offset,
			   unsigned int val)
{
	const struct dio200_board *thisboard = comedi_board(dev);
	struct dio200_private *devpriv = dev->private;

	offset <<= thisboard->mainshift;
	if (devpriv->io.regtype == io_regtype)
		outl(val, devpriv->io.u.iobase + offset);
	else
		writel(val, devpriv->io.u.membase + offset);
}

/*
 * 'insn_bits' function for an 'INTERRUPT' subdevice.
 */
static int
dio200_subdev_intr_insn_bits(struct comedi_device *dev,
			     struct comedi_subdevice *s,
			     struct comedi_insn *insn, unsigned int *data)
{
	const struct dio200_layout *layout = dio200_dev_layout(dev);
	struct dio200_subdev_intr *subpriv = s->private;

	if (layout->has_int_sce) {
=======
	spinlock_t spinlock;	/* protects the 'active' flag */
	unsigned int ofs;
	unsigned int valid_isns;
	unsigned int enabled_isns;
	bool active:1;
};

static unsigned char dio200_read8(struct comedi_device *dev,
				  unsigned int offset)
{
	const struct dio200_board *board = dev->board_ptr;

	if (board->is_pcie)
		offset <<= 3;

	if (dev->mmio)
		return readb(dev->mmio + offset);
	return inb(dev->iobase + offset);
}

static void dio200_write8(struct comedi_device *dev,
			  unsigned int offset, unsigned char val)
{
	const struct dio200_board *board = dev->board_ptr;

	if (board->is_pcie)
		offset <<= 3;

	if (dev->mmio)
		writeb(val, dev->mmio + offset);
	else
		outb(val, dev->iobase + offset);
}

static unsigned int dio200_read32(struct comedi_device *dev,
				  unsigned int offset)
{
	const struct dio200_board *board = dev->board_ptr;

	if (board->is_pcie)
		offset <<= 3;

	if (dev->mmio)
		return readl(dev->mmio + offset);
	return inl(dev->iobase + offset);
}

static void dio200_write32(struct comedi_device *dev,
			   unsigned int offset, unsigned int val)
{
	const struct dio200_board *board = dev->board_ptr;

	if (board->is_pcie)
		offset <<= 3;

	if (dev->mmio)
		writel(val, dev->mmio + offset);
	else
		outl(val, dev->iobase + offset);
}

static unsigned int dio200_subdev_8254_offset(struct comedi_device *dev,
					      struct comedi_subdevice *s)
{
	const struct dio200_board *board = dev->board_ptr;
	struct comedi_8254 *i8254 = s->private;
	unsigned int offset;

	/* get the offset that was passed to comedi_8254_*_init() */
	if (dev->mmio)
		offset = i8254->mmio - dev->mmio;
	else
		offset = i8254->iobase - dev->iobase;

	/* remove the shift that was added for PCIe boards */
	if (board->is_pcie)
		offset >>= 3;

	/* this offset now works for the dio200_{read,write} helpers */
	return offset;
}

static int dio200_subdev_intr_insn_bits(struct comedi_device *dev,
					struct comedi_subdevice *s,
					struct comedi_insn *insn,
					unsigned int *data)
{
	const struct dio200_board *board = dev->board_ptr;
	struct dio200_subdev_intr *subpriv = s->private;

	if (board->has_int_sce) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* Just read the interrupt status register.  */
		data[1] = dio200_read8(dev, subpriv->ofs) & subpriv->valid_isns;
	} else {
		/* No interrupt status register. */
		data[0] = 0;
	}

	return insn->n;
}

<<<<<<< HEAD
/*
 * Called to stop acquisition for an 'INTERRUPT' subdevice.
 */
static void dio200_stop_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	const struct dio200_layout *layout = dio200_dev_layout(dev);
=======
static void dio200_stop_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	const struct dio200_board *board = dev->board_ptr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct dio200_subdev_intr *subpriv = s->private;

	subpriv->active = false;
	subpriv->enabled_isns = 0;
<<<<<<< HEAD
	if (layout->has_int_sce)
		dio200_write8(dev, subpriv->ofs, 0);
}

/*
 * Called to start acquisition for an 'INTERRUPT' subdevice.
 */
static int dio200_start_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	unsigned int n;
	unsigned isn_bits;
	const struct dio200_layout *layout = dio200_dev_layout(dev);
	struct dio200_subdev_intr *subpriv = s->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	int retval = 0;

	if (!subpriv->continuous && subpriv->stopcount == 0) {
		/* An empty acquisition! */
		s->async->events |= COMEDI_CB_EOA;
		subpriv->active = false;
		retval = 1;
	} else {
		/* Determine interrupt sources to enable. */
		isn_bits = 0;
		if (cmd->chanlist) {
			for (n = 0; n < cmd->chanlist_len; n++)
				isn_bits |= (1U << CR_CHAN(cmd->chanlist[n]));
		}
		isn_bits &= subpriv->valid_isns;
		/* Enable interrupt sources. */
		subpriv->enabled_isns = isn_bits;
		if (layout->has_int_sce)
			dio200_write8(dev, subpriv->ofs, isn_bits);
	}

	return retval;
}

/*
 * Internal trigger function to start acquisition for an 'INTERRUPT' subdevice.
 */
static int
dio200_inttrig_start_intr(struct comedi_device *dev, struct comedi_subdevice *s,
			  unsigned int trignum)
{
	struct dio200_subdev_intr *subpriv;
	unsigned long flags;
	int event = 0;

	if (trignum != 0)
		return -EINVAL;

	subpriv = s->private;

	spin_lock_irqsave(&subpriv->spinlock, flags);
	s->async->inttrig = NULL;
	if (subpriv->active)
		event = dio200_start_intr(dev, s);

	spin_unlock_irqrestore(&subpriv->spinlock, flags);

	if (event)
		comedi_event(dev, s);

=======
	if (board->has_int_sce)
		dio200_write8(dev, subpriv->ofs, 0);
}

static void dio200_start_intr(struct comedi_device *dev,
			      struct comedi_subdevice *s)
{
	const struct dio200_board *board = dev->board_ptr;
	struct dio200_subdev_intr *subpriv = s->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	unsigned int n;
	unsigned int isn_bits;

	/* Determine interrupt sources to enable. */
	isn_bits = 0;
	if (cmd->chanlist) {
		for (n = 0; n < cmd->chanlist_len; n++)
			isn_bits |= (1U << CR_CHAN(cmd->chanlist[n]));
	}
	isn_bits &= subpriv->valid_isns;
	/* Enable interrupt sources. */
	subpriv->enabled_isns = isn_bits;
	if (board->has_int_sce)
		dio200_write8(dev, subpriv->ofs, isn_bits);
}

static int dio200_inttrig_start_intr(struct comedi_device *dev,
				     struct comedi_subdevice *s,
				     unsigned int trig_num)
{
	struct dio200_subdev_intr *subpriv = s->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	unsigned long flags;

	if (trig_num != cmd->start_arg)
		return -EINVAL;

	spin_lock_irqsave(&subpriv->spinlock, flags);
	s->async->inttrig = NULL;
	if (subpriv->active)
		dio200_start_intr(dev, s);

	spin_unlock_irqrestore(&subpriv->spinlock, flags);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 1;
}

static void dio200_read_scan_intr(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  unsigned int triggered)
{
<<<<<<< HEAD
	struct dio200_subdev_intr *subpriv = s->private;
	unsigned short val;
	unsigned int n, ch, len;

	val = 0;
	len = s->async->cmd.chanlist_len;
	for (n = 0; n < len; n++) {
		ch = CR_CHAN(s->async->cmd.chanlist[n]);
		if (triggered & (1U << ch))
			val |= (1U << n);
	}
	/* Write the scan to the buffer. */
	if (comedi_buf_put(s->async, val)) {
		s->async->events |= (COMEDI_CB_BLOCK | COMEDI_CB_EOS);
	} else {
		/* Error!  Stop acquisition.  */
		dio200_stop_intr(dev, s);
		s->async->events |= COMEDI_CB_ERROR | COMEDI_CB_OVERFLOW;
		comedi_error(dev, "buffer overflow");
	}

	/* Check for end of acquisition. */
	if (!subpriv->continuous) {
		/* stop_src == TRIG_COUNT */
		if (subpriv->stopcount > 0) {
			subpriv->stopcount--;
			if (subpriv->stopcount == 0) {
				s->async->events |= COMEDI_CB_EOA;
				dio200_stop_intr(dev, s);
			}
		}
	}
}

/*
 * This is called from the interrupt service routine to handle a read
 * scan on an 'INTERRUPT' subdevice.
 */
static int dio200_handle_read_intr(struct comedi_device *dev,
				   struct comedi_subdevice *s)
{
	const struct dio200_layout *layout = dio200_dev_layout(dev);
	struct dio200_subdev_intr *subpriv = s->private;
	unsigned triggered;
	unsigned intstat;
	unsigned cur_enabled;
	unsigned int oldevents;
=======
	struct comedi_cmd *cmd = &s->async->cmd;
	unsigned short val;
	unsigned int n, ch;

	val = 0;
	for (n = 0; n < cmd->chanlist_len; n++) {
		ch = CR_CHAN(cmd->chanlist[n]);
		if (triggered & (1U << ch))
			val |= (1U << n);
	}

	comedi_buf_write_samples(s, &val, 1);

	if (cmd->stop_src == TRIG_COUNT &&
	    s->async->scans_done >= cmd->stop_arg)
		s->async->events |= COMEDI_CB_EOA;
}

static int dio200_handle_read_intr(struct comedi_device *dev,
				   struct comedi_subdevice *s)
{
	const struct dio200_board *board = dev->board_ptr;
	struct dio200_subdev_intr *subpriv = s->private;
	unsigned int triggered;
	unsigned int intstat;
	unsigned int cur_enabled;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long flags;

	triggered = 0;

	spin_lock_irqsave(&subpriv->spinlock, flags);
<<<<<<< HEAD
	oldevents = s->async->events;
	if (layout->has_int_sce) {
=======
	if (board->has_int_sce) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * Collect interrupt sources that have triggered and disable
		 * them temporarily.  Loop around until no extra interrupt
		 * sources have triggered, at which point, the valid part of
		 * the interrupt status register will read zero, clearing the
		 * cause of the interrupt.
		 *
		 * Mask off interrupt sources already seen to avoid infinite
		 * loop in case of misconfiguration.
		 */
		cur_enabled = subpriv->enabled_isns;
		while ((intstat = (dio200_read8(dev, subpriv->ofs) &
				   subpriv->valid_isns & ~triggered)) != 0) {
			triggered |= intstat;
			cur_enabled &= ~triggered;
			dio200_write8(dev, subpriv->ofs, cur_enabled);
		}
	} else {
		/*
		 * No interrupt status register.  Assume the single interrupt
		 * source has triggered.
		 */
		triggered = subpriv->enabled_isns;
	}

	if (triggered) {
		/*
		 * Some interrupt sources have triggered and have been
		 * temporarily disabled to clear the cause of the interrupt.
		 *
		 * Reenable them NOW to minimize the time they are disabled.
		 */
		cur_enabled = subpriv->enabled_isns;
<<<<<<< HEAD
		if (layout->has_int_sce)
=======
		if (board->has_int_sce)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			dio200_write8(dev, subpriv->ofs, cur_enabled);

		if (subpriv->active) {
			/*
			 * The command is still active.
			 *
			 * Ignore interrupt sources that the command isn't
			 * interested in (just in case there's a race
			 * condition).
			 */
<<<<<<< HEAD
			if (triggered & subpriv->enabled_isns)
				/* Collect scan data. */
				dio200_read_scan_intr(dev, s, triggered);
=======
			if (triggered & subpriv->enabled_isns) {
				/* Collect scan data. */
				dio200_read_scan_intr(dev, s, triggered);
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
	spin_unlock_irqrestore(&subpriv->spinlock, flags);

<<<<<<< HEAD
	if (oldevents != s->async->events)
		comedi_event(dev, s);
=======
	comedi_handle_events(dev, s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return (triggered != 0);
}

<<<<<<< HEAD
/*
 * 'cancel' function for an 'INTERRUPT' subdevice.
 */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int dio200_subdev_intr_cancel(struct comedi_device *dev,
				     struct comedi_subdevice *s)
{
	struct dio200_subdev_intr *subpriv = s->private;
	unsigned long flags;

	spin_lock_irqsave(&subpriv->spinlock, flags);
	if (subpriv->active)
		dio200_stop_intr(dev, s);

	spin_unlock_irqrestore(&subpriv->spinlock, flags);

	return 0;
}

<<<<<<< HEAD
/*
 * 'do_cmdtest' function for an 'INTERRUPT' subdevice.
 */
static int
dio200_subdev_intr_cmdtest(struct comedi_device *dev,
			   struct comedi_subdevice *s, struct comedi_cmd *cmd)
=======
static int dio200_subdev_intr_cmdtest(struct comedi_device *dev,
				      struct comedi_subdevice *s,
				      struct comedi_cmd *cmd)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int err = 0;

	/* Step 1 : check if triggers are trivially valid */

<<<<<<< HEAD
	err |= cfc_check_trigger_src(&cmd->start_src, TRIG_NOW | TRIG_INT);
	err |= cfc_check_trigger_src(&cmd->scan_begin_src, TRIG_EXT);
	err |= cfc_check_trigger_src(&cmd->convert_src, TRIG_NOW);
	err |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= cfc_check_trigger_src(&cmd->stop_src, TRIG_COUNT | TRIG_NONE);
=======
	err |= comedi_check_trigger_src(&cmd->start_src, TRIG_NOW | TRIG_INT);
	err |= comedi_check_trigger_src(&cmd->scan_begin_src, TRIG_EXT);
	err |= comedi_check_trigger_src(&cmd->convert_src, TRIG_NOW);
	err |= comedi_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= comedi_check_trigger_src(&cmd->stop_src, TRIG_COUNT | TRIG_NONE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (err)
		return 1;

	/* Step 2a : make sure trigger sources are unique */

<<<<<<< HEAD
	err |= cfc_check_trigger_is_unique(cmd->start_src);
	err |= cfc_check_trigger_is_unique(cmd->stop_src);
=======
	err |= comedi_check_trigger_is_unique(cmd->start_src);
	err |= comedi_check_trigger_is_unique(cmd->stop_src);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Step 2b : and mutually compatible */

	if (err)
		return 2;

	/* Step 3: check if arguments are trivially valid */

<<<<<<< HEAD
	err |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);
	err |= cfc_check_trigger_arg_is(&cmd->scan_begin_arg, 0);
	err |= cfc_check_trigger_arg_is(&cmd->convert_arg, 0);
	err |= cfc_check_trigger_arg_is(&cmd->scan_end_arg, cmd->chanlist_len);

	switch (cmd->stop_src) {
	case TRIG_COUNT:
		/* any count allowed */
		break;
	case TRIG_NONE:
		err |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);
		break;
	default:
		break;
	}
=======
	err |= comedi_check_trigger_arg_is(&cmd->start_arg, 0);
	err |= comedi_check_trigger_arg_is(&cmd->scan_begin_arg, 0);
	err |= comedi_check_trigger_arg_is(&cmd->convert_arg, 0);
	err |= comedi_check_trigger_arg_is(&cmd->scan_end_arg,
					   cmd->chanlist_len);

	if (cmd->stop_src == TRIG_COUNT)
		err |= comedi_check_trigger_arg_min(&cmd->stop_arg, 1);
	else	/* TRIG_NONE */
		err |= comedi_check_trigger_arg_is(&cmd->stop_arg, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (err)
		return 3;

	/* step 4: fix up any arguments */

	/* if (err) return 4; */

	return 0;
}

<<<<<<< HEAD
/*
 * 'do_cmd' function for an 'INTERRUPT' subdevice.
 */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int dio200_subdev_intr_cmd(struct comedi_device *dev,
				  struct comedi_subdevice *s)
{
	struct comedi_cmd *cmd = &s->async->cmd;
	struct dio200_subdev_intr *subpriv = s->private;
	unsigned long flags;
<<<<<<< HEAD
	int event = 0;

	spin_lock_irqsave(&subpriv->spinlock, flags);
	subpriv->active = 1;

	/* Set up end of acquisition. */
	switch (cmd->stop_src) {
	case TRIG_COUNT:
		subpriv->continuous = false;
		subpriv->stopcount = cmd->stop_arg;
		break;
	default:
		/* TRIG_NONE */
		subpriv->continuous = true;
		subpriv->stopcount = 0;
		break;
	}

	/* Set up start of acquisition. */
	switch (cmd->start_src) {
	case TRIG_INT:
		s->async->inttrig = dio200_inttrig_start_intr;
		break;
	default:
		/* TRIG_NOW */
		event = dio200_start_intr(dev, s);
		break;
	}
	spin_unlock_irqrestore(&subpriv->spinlock, flags);

	if (event)
		comedi_event(dev, s);
=======

	spin_lock_irqsave(&subpriv->spinlock, flags);

	subpriv->active = true;

	if (cmd->start_src == TRIG_INT)
		s->async->inttrig = dio200_inttrig_start_intr;
	else	/* TRIG_NOW */
		dio200_start_intr(dev, s);

	spin_unlock_irqrestore(&subpriv->spinlock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
/*
 * This function initializes an 'INTERRUPT' subdevice.
 */
static int
dio200_subdev_intr_init(struct comedi_device *dev, struct comedi_subdevice *s,
			unsigned int offset, unsigned valid_isns)
{
	const struct dio200_layout *layout = dio200_dev_layout(dev);
	struct dio200_subdev_intr *subpriv;

	subpriv = kzalloc(sizeof(*subpriv), GFP_KERNEL);
=======
static int dio200_subdev_intr_init(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   unsigned int offset,
				   unsigned int valid_isns)
{
	const struct dio200_board *board = dev->board_ptr;
	struct dio200_subdev_intr *subpriv;

	subpriv = comedi_alloc_spriv(s, sizeof(*subpriv));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!subpriv)
		return -ENOMEM;

	subpriv->ofs = offset;
	subpriv->valid_isns = valid_isns;
	spin_lock_init(&subpriv->spinlock);

<<<<<<< HEAD
	if (layout->has_int_sce)
		/* Disable interrupt sources. */
		dio200_write8(dev, subpriv->ofs, 0);

	s->private = subpriv;
	s->type = COMEDI_SUBD_DI;
	s->subdev_flags = SDF_READABLE | SDF_CMD_READ;
	if (layout->has_int_sce) {
=======
	if (board->has_int_sce)
		/* Disable interrupt sources. */
		dio200_write8(dev, subpriv->ofs, 0);

	s->type = COMEDI_SUBD_DI;
	s->subdev_flags = SDF_READABLE | SDF_CMD_READ | SDF_PACKED;
	if (board->has_int_sce) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		s->n_chan = DIO200_MAX_ISNS;
		s->len_chanlist = DIO200_MAX_ISNS;
	} else {
		/* No interrupt source register.  Support single channel. */
		s->n_chan = 1;
		s->len_chanlist = 1;
	}
	s->range_table = &range_digital;
	s->maxdata = 1;
	s->insn_bits = dio200_subdev_intr_insn_bits;
	s->do_cmdtest = dio200_subdev_intr_cmdtest;
	s->do_cmd = dio200_subdev_intr_cmd;
	s->cancel = dio200_subdev_intr_cancel;

	return 0;
}

<<<<<<< HEAD
/*
 * Interrupt service routine.
 */
static irqreturn_t dio200_interrupt(int irq, void *d)
{
	struct comedi_device *dev = d;
	struct dio200_private *devpriv = dev->private;
	struct comedi_subdevice *s;
=======
static irqreturn_t dio200_interrupt(int irq, void *d)
{
	struct comedi_device *dev = d;
	struct comedi_subdevice *s = dev->read_subdev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int handled;

	if (!dev->attached)
		return IRQ_NONE;

<<<<<<< HEAD
	if (devpriv->intr_sd >= 0) {
		s = &dev->subdevices[devpriv->intr_sd];
		handled = dio200_handle_read_intr(dev, s);
	} else {
		handled = 0;
	}
=======
	handled = dio200_handle_read_intr(dev, s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return IRQ_RETVAL(handled);
}

<<<<<<< HEAD
/*
 * Read an '8254' counter subdevice channel.
 */
static unsigned int
dio200_subdev_8254_read_chan(struct comedi_device *dev,
			     struct comedi_subdevice *s, unsigned int chan)
{
	struct dio200_subdev_8254 *subpriv = s->private;
	unsigned int val;

	/* latch counter */
	val = chan << 6;
	dio200_write8(dev, subpriv->ofs + i8254_control_reg, val);
	/* read lsb, msb */
	val = dio200_read8(dev, subpriv->ofs + chan);
	val += dio200_read8(dev, subpriv->ofs + chan) << 8;
	return val;
}

/*
 * Write an '8254' subdevice channel.
 */
static void
dio200_subdev_8254_write_chan(struct comedi_device *dev,
			      struct comedi_subdevice *s, unsigned int chan,
			      unsigned int count)
{
	struct dio200_subdev_8254 *subpriv = s->private;

	/* write lsb, msb */
	dio200_write8(dev, subpriv->ofs + chan, count & 0xff);
	dio200_write8(dev, subpriv->ofs + chan, (count >> 8) & 0xff);
}

/*
 * Set mode of an '8254' subdevice channel.
 */
static void
dio200_subdev_8254_set_mode(struct comedi_device *dev,
			    struct comedi_subdevice *s, unsigned int chan,
			    unsigned int mode)
{
	struct dio200_subdev_8254 *subpriv = s->private;
	unsigned int byte;

	byte = chan << 6;
	byte |= 0x30;		/* access order: lsb, msb */
	byte |= (mode & 0xf);	/* counter mode and BCD|binary */
	dio200_write8(dev, subpriv->ofs + i8254_control_reg, byte);
}

/*
 * Read status byte of an '8254' counter subdevice channel.
 */
static unsigned int
dio200_subdev_8254_status(struct comedi_device *dev,
			  struct comedi_subdevice *s, unsigned int chan)
{
	struct dio200_subdev_8254 *subpriv = s->private;

	/* latch status */
	dio200_write8(dev, subpriv->ofs + i8254_control_reg,
		      0xe0 | (2 << chan));
	/* read status */
	return dio200_read8(dev, subpriv->ofs + chan);
}

/*
 * Handle 'insn_read' for an '8254' counter subdevice.
 */
static int
dio200_subdev_8254_read(struct comedi_device *dev, struct comedi_subdevice *s,
			struct comedi_insn *insn, unsigned int *data)
{
	struct dio200_subdev_8254 *subpriv = s->private;
	int chan = CR_CHAN(insn->chanspec);
	unsigned int n;
	unsigned long flags;

	for (n = 0; n < insn->n; n++) {
		spin_lock_irqsave(&subpriv->spinlock, flags);
		data[n] = dio200_subdev_8254_read_chan(dev, s, chan);
		spin_unlock_irqrestore(&subpriv->spinlock, flags);
	}
	return insn->n;
}

/*
 * Handle 'insn_write' for an '8254' counter subdevice.
 */
static int
dio200_subdev_8254_write(struct comedi_device *dev, struct comedi_subdevice *s,
			 struct comedi_insn *insn, unsigned int *data)
{
	struct dio200_subdev_8254 *subpriv = s->private;
	int chan = CR_CHAN(insn->chanspec);
	unsigned int n;
	unsigned long flags;

	for (n = 0; n < insn->n; n++) {
		spin_lock_irqsave(&subpriv->spinlock, flags);
		dio200_subdev_8254_write_chan(dev, s, chan, data[n]);
		spin_unlock_irqrestore(&subpriv->spinlock, flags);
	}
	return insn->n;
}

/*
 * Set gate source for an '8254' counter subdevice channel.
 */
static int
dio200_subdev_8254_set_gate_src(struct comedi_device *dev,
				struct comedi_subdevice *s,
				unsigned int counter_number,
				unsigned int gate_src)
{
	const struct dio200_layout *layout = dio200_dev_layout(dev);
	struct dio200_subdev_8254 *subpriv = s->private;
	unsigned char byte;

	if (!layout->has_clk_gat_sce)
		return -1;
	if (counter_number > 2)
		return -1;
	if (gate_src > (layout->has_enhancements ? 31 : 7))
		return -1;

	subpriv->gate_src[counter_number] = gate_src;
	byte = gat_sce(subpriv->which, counter_number, gate_src);
	dio200_write8(dev, subpriv->gat_sce_ofs, byte);

	return 0;
}

/*
 * Get gate source for an '8254' counter subdevice channel.
 */
static int
dio200_subdev_8254_get_gate_src(struct comedi_device *dev,
				struct comedi_subdevice *s,
				unsigned int counter_number)
{
	const struct dio200_layout *layout = dio200_dev_layout(dev);
	struct dio200_subdev_8254 *subpriv = s->private;

	if (!layout->has_clk_gat_sce)
		return -1;
	if (counter_number > 2)
		return -1;

	return subpriv->gate_src[counter_number];
}

/*
 * Set clock source for an '8254' counter subdevice channel.
 */
static int
dio200_subdev_8254_set_clock_src(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 unsigned int counter_number,
				 unsigned int clock_src)
{
	const struct dio200_layout *layout = dio200_dev_layout(dev);
	struct dio200_subdev_8254 *subpriv = s->private;
	unsigned char byte;

	if (!layout->has_clk_gat_sce)
		return -1;
	if (counter_number > 2)
		return -1;
	if (clock_src > (layout->has_enhancements ? 31 : 7))
		return -1;

	subpriv->clock_src[counter_number] = clock_src;
	byte = clk_sce(subpriv->which, counter_number, clock_src);
	dio200_write8(dev, subpriv->clk_sce_ofs, byte);

	return 0;
}

/*
 * Get clock source for an '8254' counter subdevice channel.
 */
static int
dio200_subdev_8254_get_clock_src(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 unsigned int counter_number,
				 unsigned int *period_ns)
{
	const struct dio200_layout *layout = dio200_dev_layout(dev);
	struct dio200_subdev_8254 *subpriv = s->private;
	unsigned clock_src;

	if (!layout->has_clk_gat_sce)
		return -1;
	if (counter_number > 2)
		return -1;

	clock_src = subpriv->clock_src[counter_number];
	*period_ns = clock_period[clock_src];
	return clock_src;
}

/*
 * Handle 'insn_config' for an '8254' counter subdevice.
 */
static int
dio200_subdev_8254_config(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_insn *insn, unsigned int *data)
{
	struct dio200_subdev_8254 *subpriv = s->private;
	int ret = 0;
	int chan = CR_CHAN(insn->chanspec);
	unsigned long flags;

	spin_lock_irqsave(&subpriv->spinlock, flags);
	switch (data[0]) {
	case INSN_CONFIG_SET_COUNTER_MODE:
		if (data[1] > (I8254_MODE5 | I8254_BINARY))
			ret = -EINVAL;
		else
			dio200_subdev_8254_set_mode(dev, s, chan, data[1]);
		break;
	case INSN_CONFIG_8254_READ_STATUS:
		data[1] = dio200_subdev_8254_status(dev, s, chan);
		break;
	case INSN_CONFIG_SET_GATE_SRC:
		ret = dio200_subdev_8254_set_gate_src(dev, s, chan, data[2]);
		if (ret < 0)
			ret = -EINVAL;
		break;
	case INSN_CONFIG_GET_GATE_SRC:
		ret = dio200_subdev_8254_get_gate_src(dev, s, chan);
		if (ret < 0) {
			ret = -EINVAL;
			break;
		}
		data[2] = ret;
		break;
	case INSN_CONFIG_SET_CLOCK_SRC:
		ret = dio200_subdev_8254_set_clock_src(dev, s, chan, data[1]);
		if (ret < 0)
			ret = -EINVAL;
		break;
	case INSN_CONFIG_GET_CLOCK_SRC:
		ret = dio200_subdev_8254_get_clock_src(dev, s, chan, &data[2]);
		if (ret < 0) {
			ret = -EINVAL;
			break;
		}
		data[1] = ret;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	spin_unlock_irqrestore(&subpriv->spinlock, flags);
	return ret < 0 ? ret : insn->n;
}

/*
 * This function initializes an '8254' counter subdevice.
 */
static int
dio200_subdev_8254_init(struct comedi_device *dev, struct comedi_subdevice *s,
			unsigned int offset)
{
	const struct dio200_layout *layout = dio200_dev_layout(dev);
	struct dio200_subdev_8254 *subpriv;
	unsigned int chan;

	subpriv = kzalloc(sizeof(*subpriv), GFP_KERNEL);
	if (!subpriv)
		return -ENOMEM;

	s->private = subpriv;
	s->type = COMEDI_SUBD_COUNTER;
	s->subdev_flags = SDF_WRITABLE | SDF_READABLE;
	s->n_chan = 3;
	s->maxdata = 0xFFFF;
	s->insn_read = dio200_subdev_8254_read;
	s->insn_write = dio200_subdev_8254_write;
	s->insn_config = dio200_subdev_8254_config;

	spin_lock_init(&subpriv->spinlock);
	subpriv->ofs = offset;
	if (layout->has_clk_gat_sce) {
		/* Derive CLK_SCE and GAT_SCE register offsets from
		 * 8254 offset. */
		subpriv->clk_sce_ofs = DIO200_XCLK_SCE + (offset >> 3);
		subpriv->gat_sce_ofs = DIO200_XGAT_SCE + (offset >> 3);
		subpriv->which = (offset >> 2) & 1;
	}

	/* Initialize channels. */
	for (chan = 0; chan < 3; chan++) {
		dio200_subdev_8254_set_mode(dev, s, chan,
					    I8254_MODE0 | I8254_BINARY);
		if (layout->has_clk_gat_sce) {
=======
static void dio200_subdev_8254_set_gate_src(struct comedi_device *dev,
					    struct comedi_subdevice *s,
					    unsigned int chan,
					    unsigned int src)
{
	unsigned int offset = dio200_subdev_8254_offset(dev, s);

	dio200_write8(dev, DIO200_GAT_SCE(offset >> 3),
		      gat_sce((offset >> 2) & 1, chan, src));
}

static void dio200_subdev_8254_set_clock_src(struct comedi_device *dev,
					     struct comedi_subdevice *s,
					     unsigned int chan,
					     unsigned int src)
{
	unsigned int offset = dio200_subdev_8254_offset(dev, s);

	dio200_write8(dev, DIO200_CLK_SCE(offset >> 3),
		      clk_sce((offset >> 2) & 1, chan, src));
}

static int dio200_subdev_8254_config(struct comedi_device *dev,
				     struct comedi_subdevice *s,
				     struct comedi_insn *insn,
				     unsigned int *data)
{
	const struct dio200_board *board = dev->board_ptr;
	struct comedi_8254 *i8254 = s->private;
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int max_src = board->is_pcie ? 31 : 7;
	unsigned int src;

	if (!board->has_clk_gat_sce)
		return -EINVAL;

	switch (data[0]) {
	case INSN_CONFIG_SET_GATE_SRC:
		src = data[2];
		if (src > max_src)
			return -EINVAL;

		dio200_subdev_8254_set_gate_src(dev, s, chan, src);
		i8254->gate_src[chan] = src;
		break;
	case INSN_CONFIG_GET_GATE_SRC:
		data[2] = i8254->gate_src[chan];
		break;
	case INSN_CONFIG_SET_CLOCK_SRC:
		src = data[1];
		if (src > max_src)
			return -EINVAL;

		dio200_subdev_8254_set_clock_src(dev, s, chan, src);
		i8254->clock_src[chan] = src;
		break;
	case INSN_CONFIG_GET_CLOCK_SRC:
		data[1] = i8254->clock_src[chan];
		data[2] = clock_period[i8254->clock_src[chan]];
		break;
	default:
		return -EINVAL;
	}

	return insn->n;
}

static int dio200_subdev_8254_init(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   unsigned int offset)
{
	const struct dio200_board *board = dev->board_ptr;
	struct comedi_8254 *i8254;
	unsigned int regshift;
	int chan;

	/*
	 * PCIe boards need the offset shifted in order to get the
	 * correct base address of the timer.
	 */
	if (board->is_pcie) {
		offset <<= 3;
		regshift = 3;
	} else {
		regshift = 0;
	}

	if (dev->mmio) {
		i8254 = comedi_8254_mm_init(dev->mmio + offset,
					    0, I8254_IO8, regshift);
	} else {
		i8254 = comedi_8254_init(dev->iobase + offset,
					 0, I8254_IO8, regshift);
	}
	if (!i8254)
		return -ENOMEM;

	comedi_8254_subdevice_init(s, i8254);

	i8254->insn_config = dio200_subdev_8254_config;

	/*
	 * There could be multiple timers so this driver does not
	 * use dev->pacer to save the i8254 pointer. Instead,
	 * comedi_8254_subdevice_init() saved the i8254 pointer in
	 * s->private.  Mark the subdevice as having private data
	 * to be automatically freed when the device is detached.
	 */
	comedi_set_spriv_auto_free(s);

	/* Initialize channels. */
	if (board->has_clk_gat_sce) {
		for (chan = 0; chan < 3; chan++) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* Gate source 0 is VCC (logic 1). */
			dio200_subdev_8254_set_gate_src(dev, s, chan, 0);
			/* Clock source 0 is the dedicated clock input. */
			dio200_subdev_8254_set_clock_src(dev, s, chan, 0);
		}
	}

	return 0;
}

<<<<<<< HEAD
/*
 * This function sets I/O directions for an '8255' DIO subdevice.
 */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void dio200_subdev_8255_set_dir(struct comedi_device *dev,
				       struct comedi_subdevice *s)
{
	struct dio200_subdev_8255 *subpriv = s->private;
	int config;

<<<<<<< HEAD
	config = CR_CW;
	/* 1 in io_bits indicates output, 1 in config indicates input */
	if (!(s->io_bits & 0x0000ff))
		config |= CR_A_IO;
	if (!(s->io_bits & 0x00ff00))
		config |= CR_B_IO;
	if (!(s->io_bits & 0x0f0000))
		config |= CR_C_LO_IO;
	if (!(s->io_bits & 0xf00000))
		config |= CR_C_HI_IO;
	dio200_write8(dev, subpriv->ofs + 3, config);
}

/*
 * Handle 'insn_bits' for an '8255' DIO subdevice.
 */
static int dio200_subdev_8255_bits(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn, unsigned int *data)
{
	struct dio200_subdev_8255 *subpriv = s->private;

	if (data[0]) {
		s->state &= ~data[0];
		s->state |= (data[0] & data[1]);
		if (data[0] & 0xff)
			dio200_write8(dev, subpriv->ofs, s->state & 0xff);
		if (data[0] & 0xff00)
			dio200_write8(dev, subpriv->ofs + 1,
				      (s->state >> 8) & 0xff);
		if (data[0] & 0xff0000)
			dio200_write8(dev, subpriv->ofs + 2,
				      (s->state >> 16) & 0xff);
	}
	data[1] = dio200_read8(dev, subpriv->ofs);
	data[1] |= dio200_read8(dev, subpriv->ofs + 1) << 8;
	data[1] |= dio200_read8(dev, subpriv->ofs + 2) << 16;
	return 2;
}

/*
 * Handle 'insn_config' for an '8255' DIO subdevice.
 */
=======
	config = I8255_CTRL_CW;
	/* 1 in io_bits indicates output, 1 in config indicates input */
	if (!(s->io_bits & 0x0000ff))
		config |= I8255_CTRL_A_IO;
	if (!(s->io_bits & 0x00ff00))
		config |= I8255_CTRL_B_IO;
	if (!(s->io_bits & 0x0f0000))
		config |= I8255_CTRL_C_LO_IO;
	if (!(s->io_bits & 0xf00000))
		config |= I8255_CTRL_C_HI_IO;
	dio200_write8(dev, subpriv->ofs + I8255_CTRL_REG, config);
}

static int dio200_subdev_8255_bits(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn,
				   unsigned int *data)
{
	struct dio200_subdev_8255 *subpriv = s->private;
	unsigned int mask;
	unsigned int val;

	mask = comedi_dio_update_state(s, data);
	if (mask) {
		if (mask & 0xff) {
			dio200_write8(dev, subpriv->ofs + I8255_DATA_A_REG,
				      s->state & 0xff);
		}
		if (mask & 0xff00) {
			dio200_write8(dev, subpriv->ofs + I8255_DATA_B_REG,
				      (s->state >> 8) & 0xff);
		}
		if (mask & 0xff0000) {
			dio200_write8(dev, subpriv->ofs + I8255_DATA_C_REG,
				      (s->state >> 16) & 0xff);
		}
	}

	val = dio200_read8(dev, subpriv->ofs + I8255_DATA_A_REG);
	val |= dio200_read8(dev, subpriv->ofs + I8255_DATA_B_REG) << 8;
	val |= dio200_read8(dev, subpriv->ofs + I8255_DATA_C_REG) << 16;

	data[1] = val;

	return insn->n;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int dio200_subdev_8255_config(struct comedi_device *dev,
				     struct comedi_subdevice *s,
				     struct comedi_insn *insn,
				     unsigned int *data)
{
<<<<<<< HEAD
	unsigned int mask;
	unsigned int bits;

	mask = 1 << CR_CHAN(insn->chanspec);
	if (mask & 0x0000ff)
		bits = 0x0000ff;
	else if (mask & 0x00ff00)
		bits = 0x00ff00;
	else if (mask & 0x0f0000)
		bits = 0x0f0000;
	else
		bits = 0xf00000;
	switch (data[0]) {
	case INSN_CONFIG_DIO_INPUT:
		s->io_bits &= ~bits;
		break;
	case INSN_CONFIG_DIO_OUTPUT:
		s->io_bits |= bits;
		break;
	case INSN_CONFIG_DIO_QUERY:
		data[1] = (s->io_bits & bits) ? COMEDI_OUTPUT : COMEDI_INPUT;
		return insn->n;
		break;
	default:
		return -EINVAL;
	}
	dio200_subdev_8255_set_dir(dev, s);
	return 1;
}

/*
 * This function initializes an '8255' DIO subdevice.
 *
 * offset is the offset to the 8255 chip.
 */
=======
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int mask;
	int ret;

	if (chan < 8)
		mask = 0x0000ff;
	else if (chan < 16)
		mask = 0x00ff00;
	else if (chan < 20)
		mask = 0x0f0000;
	else
		mask = 0xf00000;

	ret = comedi_dio_insn_config(dev, s, insn, data, mask);
	if (ret)
		return ret;

	dio200_subdev_8255_set_dir(dev, s);

	return insn->n;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int dio200_subdev_8255_init(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   unsigned int offset)
{
	struct dio200_subdev_8255 *subpriv;

<<<<<<< HEAD
	subpriv = kzalloc(sizeof(*subpriv), GFP_KERNEL);
	if (!subpriv)
		return -ENOMEM;
	subpriv->ofs = offset;
	s->private = subpriv;
=======
	subpriv = comedi_alloc_spriv(s, sizeof(*subpriv));
	if (!subpriv)
		return -ENOMEM;

	subpriv->ofs = offset;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	s->type = COMEDI_SUBD_DIO;
	s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
	s->n_chan = 24;
	s->range_table = &range_digital;
	s->maxdata = 1;
	s->insn_bits = dio200_subdev_8255_bits;
	s->insn_config = dio200_subdev_8255_config;
<<<<<<< HEAD
	s->state = 0;
	s->io_bits = 0;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dio200_subdev_8255_set_dir(dev, s);
	return 0;
}

<<<<<<< HEAD
/*
 * Handle 'insn_read' for a timer subdevice.
 */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int dio200_subdev_timer_read(struct comedi_device *dev,
				    struct comedi_subdevice *s,
				    struct comedi_insn *insn,
				    unsigned int *data)
{
	unsigned int n;

	for (n = 0; n < insn->n; n++)
		data[n] = dio200_read32(dev, DIO200_TS_COUNT);
	return n;
}

<<<<<<< HEAD
/*
 * Reset timer subdevice.
 */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void dio200_subdev_timer_reset(struct comedi_device *dev,
				      struct comedi_subdevice *s)
{
	unsigned int clock;

	clock = dio200_read32(dev, DIO200_TS_CONFIG) & TS_CONFIG_CLK_SRC_MASK;
	dio200_write32(dev, DIO200_TS_CONFIG, clock | TS_CONFIG_RESET);
	dio200_write32(dev, DIO200_TS_CONFIG, clock);
}

<<<<<<< HEAD
/*
 * Get timer subdevice clock source and period.
 */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void dio200_subdev_timer_get_clock_src(struct comedi_device *dev,
					      struct comedi_subdevice *s,
					      unsigned int *src,
					      unsigned int *period)
{
	unsigned int clk;

	clk = dio200_read32(dev, DIO200_TS_CONFIG) & TS_CONFIG_CLK_SRC_MASK;
	*src = clk;
	*period = (clk < ARRAY_SIZE(ts_clock_period)) ?
		  ts_clock_period[clk] : 0;
}

<<<<<<< HEAD
/*
 * Set timer subdevice clock source.
 */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int dio200_subdev_timer_set_clock_src(struct comedi_device *dev,
					     struct comedi_subdevice *s,
					     unsigned int src)
{
	if (src > TS_CONFIG_MAX_CLK_SRC)
		return -EINVAL;
	dio200_write32(dev, DIO200_TS_CONFIG, src);
	return 0;
}

<<<<<<< HEAD
/*
 * Handle 'insn_config' for a timer subdevice.
 */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int dio200_subdev_timer_config(struct comedi_device *dev,
				      struct comedi_subdevice *s,
				      struct comedi_insn *insn,
				      unsigned int *data)
{
	int ret = 0;

	switch (data[0]) {
	case INSN_CONFIG_RESET:
		dio200_subdev_timer_reset(dev, s);
		break;
	case INSN_CONFIG_SET_CLOCK_SRC:
		ret = dio200_subdev_timer_set_clock_src(dev, s, data[1]);
		if (ret < 0)
			ret = -EINVAL;
		break;
	case INSN_CONFIG_GET_CLOCK_SRC:
		dio200_subdev_timer_get_clock_src(dev, s, &data[1], &data[2]);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	return ret < 0 ? ret : insn->n;
}

<<<<<<< HEAD
/*
 * This function initializes a timer subdevice.
 *
 * Uses the timestamp timer registers.  There is only one timestamp timer.
 */
static int dio200_subdev_timer_init(struct comedi_device *dev,
				    struct comedi_subdevice *s)
{
	s->type = COMEDI_SUBD_TIMER;
	s->subdev_flags = SDF_READABLE | SDF_LSAMPL;
	s->n_chan = 1;
	s->maxdata = 0xFFFFFFFF;
	s->insn_read = dio200_subdev_timer_read;
	s->insn_config = dio200_subdev_timer_config;
	return 0;
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void amplc_dio200_set_enhance(struct comedi_device *dev, unsigned char val)
{
	dio200_write8(dev, DIO200_ENHANCE, val);
}
EXPORT_SYMBOL_GPL(amplc_dio200_set_enhance);

int amplc_dio200_common_attach(struct comedi_device *dev, unsigned int irq,
			       unsigned long req_irq_flags)
{
<<<<<<< HEAD
	const struct dio200_board *thisboard = comedi_board(dev);
	struct dio200_private *devpriv = dev->private;
	const struct dio200_layout *layout = dio200_board_layout(thisboard);
	struct comedi_subdevice *s;
	int sdx;
	unsigned int n;
	int ret;

	devpriv->intr_sd = -1;

	ret = comedi_alloc_subdevices(dev, layout->n_subdevs);
=======
	const struct dio200_board *board = dev->board_ptr;
	struct comedi_subdevice *s;
	unsigned int n;
	int ret;

	ret = comedi_alloc_subdevices(dev, board->n_subdevs);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret)
		return ret;

	for (n = 0; n < dev->n_subdevices; n++) {
		s = &dev->subdevices[n];
<<<<<<< HEAD
		switch (layout->sdtype[n]) {
		case sd_8254:
			/* counter subdevice (8254) */
			ret = dio200_subdev_8254_init(dev, s,
						      layout->sdinfo[n]);
=======
		switch (board->sdtype[n]) {
		case sd_8254:
			/* counter subdevice (8254) */
			ret = dio200_subdev_8254_init(dev, s,
						      board->sdinfo[n]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (ret < 0)
				return ret;
			break;
		case sd_8255:
			/* digital i/o subdevice (8255) */
			ret = dio200_subdev_8255_init(dev, s,
<<<<<<< HEAD
						      layout->sdinfo[n]);
=======
						      board->sdinfo[n]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (ret < 0)
				return ret;
			break;
		case sd_intr:
			/* 'INTERRUPT' subdevice */
<<<<<<< HEAD
			if (irq) {
				ret = dio200_subdev_intr_init(dev, s,
							      DIO200_INT_SCE,
							      layout->sdinfo[n]
							     );
				if (ret < 0)
					return ret;
				devpriv->intr_sd = n;
=======
			if (irq && !dev->read_subdev) {
				ret = dio200_subdev_intr_init(dev, s,
							      DIO200_INT_SCE,
							      board->sdinfo[n]);
				if (ret < 0)
					return ret;
				dev->read_subdev = s;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			} else {
				s->type = COMEDI_SUBD_UNUSED;
			}
			break;
		case sd_timer:
<<<<<<< HEAD
			ret = dio200_subdev_timer_init(dev, s);
			if (ret < 0)
				return ret;
=======
			s->type		= COMEDI_SUBD_TIMER;
			s->subdev_flags	= SDF_READABLE | SDF_LSAMPL;
			s->n_chan	= 1;
			s->maxdata	= 0xffffffff;
			s->insn_read	= dio200_subdev_timer_read;
			s->insn_config	= dio200_subdev_timer_config;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		default:
			s->type = COMEDI_SUBD_UNUSED;
			break;
		}
	}
<<<<<<< HEAD
	sdx = devpriv->intr_sd;
	if (sdx >= 0 && sdx < dev->n_subdevices)
		dev->read_subdev = &dev->subdevices[sdx];
	if (irq) {
=======

	if (irq && dev->read_subdev) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (request_irq(irq, dio200_interrupt, req_irq_flags,
				dev->board_name, dev) >= 0) {
			dev->irq = irq;
		} else {
			dev_warn(dev->class_dev,
				 "warning! irq %u unavailable!\n", irq);
		}
	}
<<<<<<< HEAD
	dev_info(dev->class_dev, "attached\n");
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}
EXPORT_SYMBOL_GPL(amplc_dio200_common_attach);

<<<<<<< HEAD
void amplc_dio200_common_detach(struct comedi_device *dev)
{
	const struct dio200_board *thisboard = comedi_board(dev);
	struct dio200_private *devpriv = dev->private;
	const struct dio200_layout *layout;
	unsigned n;

	if (!thisboard || !devpriv)
		return;
	if (dev->irq)
		free_irq(dev->irq, dev);
	if (dev->subdevices) {
		layout = dio200_board_layout(thisboard);
		for (n = 0; n < dev->n_subdevices; n++) {
			switch (layout->sdtype[n]) {
			case sd_8254:
			case sd_8255:
			case sd_intr:
				comedi_spriv_free(dev, n);
				break;
			case sd_timer:
			default:
				break;
			}
		}
	}
}
EXPORT_SYMBOL_GPL(amplc_dio200_common_detach);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int __init amplc_dio200_common_init(void)
{
	return 0;
}
module_init(amplc_dio200_common_init);

static void __exit amplc_dio200_common_exit(void)
{
}
module_exit(amplc_dio200_common_exit);

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi helper for amplc_dio200 and amplc_dio200_pci");
MODULE_LICENSE("GPL");
