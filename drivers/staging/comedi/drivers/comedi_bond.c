/*
<<<<<<< HEAD
    comedi/drivers/comedi_bond.c
    A Comedi driver to 'bond' or merge multiple drivers and devices as one.

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2000 David A. Schleef <ds@schleef.org>
    Copyright (C) 2005 Calin A. Culianu <calin@ajvar.org>

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
Driver: comedi_bond
Description: A driver to 'bond' (merge) multiple subdevices from multiple
	     devices together as one.
Devices:
Author: ds
Updated: Mon, 10 Oct 00:18:25 -0500
Status: works

This driver allows you to 'bond' (merge) multiple comedi subdevices
(coming from possibly difference boards and/or drivers) together.  For
example, if you had a board with 2 different DIO subdevices, and
another with 1 DIO subdevice, you could 'bond' them with this driver
so that they look like one big fat DIO subdevice.  This makes writing
applications slightly easier as you don't have to worry about managing
different subdevices in the application -- you just worry about
indexing one linear array of channel id's.

Right now only DIO subdevices are supported as that's the personal itch
I am scratching with this driver.  If you want to add support for AI and AO
subdevs, go right on ahead and do so!

Commands aren't supported -- although it would be cool if they were.

Configuration Options:
  List of comedi-minors to bond.  All subdevices of the same type
  within each minor will be concatenated together in the order given here.
*/

=======
 * comedi_bond.c
 * A Comedi driver to 'bond' or merge multiple drivers and devices as one.
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 2000 David A. Schleef <ds@schleef.org>
 * Copyright (C) 2005 Calin A. Culianu <calin@ajvar.org>
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
 * Driver: comedi_bond
 * Description: A driver to 'bond' (merge) multiple subdevices from multiple
 * devices together as one.
 * Devices:
 * Author: ds
 * Updated: Mon, 10 Oct 00:18:25 -0500
 * Status: works
 *
 * This driver allows you to 'bond' (merge) multiple comedi subdevices
 * (coming from possibly difference boards and/or drivers) together.  For
 * example, if you had a board with 2 different DIO subdevices, and
 * another with 1 DIO subdevice, you could 'bond' them with this driver
 * so that they look like one big fat DIO subdevice.  This makes writing
 * applications slightly easier as you don't have to worry about managing
 * different subdevices in the application -- you just worry about
 * indexing one linear array of channel id's.
 *
 * Right now only DIO subdevices are supported as that's the personal itch
 * I am scratching with this driver.  If you want to add support for AI and AO
 * subdevs, go right on ahead and do so!
 *
 * Commands aren't supported -- although it would be cool if they were.
 *
 * Configuration Options:
 *   List of comedi-minors to bond.  All subdevices of the same type
 *   within each minor will be concatenated together in the order given here.
 */

#include <linux/module.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/string.h>
#include <linux/slab.h>
#include "../comedi.h"
#include "../comedilib.h"
#include "../comedidev.h"

<<<<<<< HEAD
/* The maxiumum number of channels per subdevice. */
#define MAX_CHANS 256

struct BondedDevice {
	struct comedi_device *dev;
	unsigned minor;
	unsigned subdev;
	unsigned subdev_type;
	unsigned nchans;
	unsigned chanid_offset;	/* The offset into our unified linear
				   channel-id's of chanid 0 on this
				   subdevice. */
};

/* this structure is for data unique to this hardware driver.  If
   several hardware drivers keep similar information in this structure,
   feel free to suggest moving the variable to the struct comedi_device struct.  */
struct comedi_bond_private {
# define MAX_BOARD_NAME 256
	char name[MAX_BOARD_NAME];
	struct BondedDevice **devs;
	unsigned ndevs;
	struct BondedDevice *chanIdDevMap[MAX_CHANS];
	unsigned nchans;
};

/* DIO devices are slightly special.  Although it is possible to
 * implement the insn_read/insn_write interface, it is much more
 * useful to applications if you implement the insn_bits interface.
 * This allows packed reading/writing of the DIO channels.  The
 * comedi core can convert between insn_bits and insn_read/write */
=======
struct bonded_device {
	struct comedi_device *dev;
	unsigned int minor;
	unsigned int subdev;
	unsigned int nchans;
};

struct comedi_bond_private {
	char name[256];
	struct bonded_device **devs;
	unsigned int ndevs;
	unsigned int nchans;
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int bonding_dio_insn_bits(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 struct comedi_insn *insn, unsigned int *data)
{
	struct comedi_bond_private *devpriv = dev->private;
<<<<<<< HEAD
#define LSAMPL_BITS (sizeof(unsigned int)*8)
	unsigned nchans = LSAMPL_BITS, num_done = 0, i;

	if (devpriv->nchans < nchans)
		nchans = devpriv->nchans;

	/* The insn data is a mask in data[0] and the new data
	 * in data[1], each channel cooresponding to a bit. */
	for (i = 0; num_done < nchans && i < devpriv->ndevs; ++i) {
		struct BondedDevice *bdev = devpriv->devs[i];
		/* Grab the channel mask and data of only the bits corresponding
		   to this subdevice.. need to shift them to zero position of
		   course. */
		/* Bits corresponding to this subdev. */
		unsigned int subdevMask = ((1 << bdev->nchans) - 1);
		unsigned int writeMask, dataBits;

		/* Argh, we have >= LSAMPL_BITS chans.. take all bits */
		if (bdev->nchans >= LSAMPL_BITS)
			subdevMask = (unsigned int)(-1);

		writeMask = (data[0] >> num_done) & subdevMask;
		dataBits = (data[1] >> num_done) & subdevMask;

		/* Read/Write the new digital lines */
		if (comedi_dio_bitfield(bdev->dev, bdev->subdev, writeMask,
					&dataBits) != 2)
			return -EINVAL;

		/* Make room for the new bits in data[1], the return value */
		data[1] &= ~(subdevMask << num_done);
		/* Put the bits in the return value */
		data[1] |= (dataBits & subdevMask) << num_done;
		/* Save the new bits to the saved state.. */
		s->state = data[1];

		num_done += bdev->nchans;
	}
=======
	unsigned int n_left, n_done, base_chan;
	unsigned int write_mask, data_bits;
	struct bonded_device **devs;

	write_mask = data[0];
	data_bits = data[1];
	base_chan = CR_CHAN(insn->chanspec);
	/* do a maximum of 32 channels, starting from base_chan. */
	n_left = devpriv->nchans - base_chan;
	if (n_left > 32)
		n_left = 32;

	n_done = 0;
	devs = devpriv->devs;
	do {
		struct bonded_device *bdev = *devs++;

		if (base_chan < bdev->nchans) {
			/* base channel falls within bonded device */
			unsigned int b_chans, b_mask, b_write_mask, b_data_bits;
			int ret;

			/*
			 * Get num channels to do for bonded device and set
			 * up mask and data bits for bonded device.
			 */
			b_chans = bdev->nchans - base_chan;
			if (b_chans > n_left)
				b_chans = n_left;
			b_mask = (b_chans < 32) ? ((1 << b_chans) - 1)
						: 0xffffffff;
			b_write_mask = (write_mask >> n_done) & b_mask;
			b_data_bits = (data_bits >> n_done) & b_mask;
			/* Read/Write the new digital lines. */
			ret = comedi_dio_bitfield2(bdev->dev, bdev->subdev,
						   b_write_mask, &b_data_bits,
						   base_chan);
			if (ret < 0)
				return ret;
			/* Place read bits into data[1]. */
			data[1] &= ~(b_mask << n_done);
			data[1] |= (b_data_bits & b_mask) << n_done;
			/*
			 * Set up for following bonded device (if still have
			 * channels to read/write).
			 */
			base_chan = 0;
			n_done += b_chans;
			n_left -= b_chans;
		} else {
			/* Skip bonded devices before base channel. */
			base_chan -= bdev->nchans;
		}
	} while (n_left);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return insn->n;
}

static int bonding_dio_insn_config(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn, unsigned int *data)
{
	struct comedi_bond_private *devpriv = dev->private;
<<<<<<< HEAD
	int chan = CR_CHAN(insn->chanspec), ret, io_bits = s->io_bits;
	unsigned int io;
	struct BondedDevice *bdev;

	if (chan < 0 || chan >= devpriv->nchans)
		return -EINVAL;
	bdev = devpriv->chanIdDevMap[chan];

	/* The input or output configuration of each digital line is
	 * configured by a special insn_config instruction.  chanspec
	 * contains the channel to be changed, and data[0] contains the
	 * value COMEDI_INPUT or COMEDI_OUTPUT. */
	switch (data[0]) {
	case INSN_CONFIG_DIO_OUTPUT:
		io = COMEDI_OUTPUT;	/* is this really necessary? */
		io_bits |= 1 << chan;
		break;
	case INSN_CONFIG_DIO_INPUT:
		io = COMEDI_INPUT;	/* is this really necessary? */
		io_bits &= ~(1 << chan);
		break;
	case INSN_CONFIG_DIO_QUERY:
		data[1] =
		    (io_bits & (1 << chan)) ? COMEDI_OUTPUT : COMEDI_INPUT;
		return insn->n;
		break;
	default:
		return -EINVAL;
		break;
	}
	/* 'real' channel id for this subdev.. */
	chan -= bdev->chanid_offset;
	ret = comedi_dio_config(bdev->dev, bdev->subdev, chan, io);
	if (ret != 1)
		return -EINVAL;
	/* Finally, save the new io_bits values since we didn't get
	   an error above. */
	s->io_bits = io_bits;
	return insn->n;
}

static void *Realloc(const void *oldmem, size_t newlen, size_t oldlen)
{
	void *newmem = kmalloc(newlen, GFP_KERNEL);

	if (newmem && oldmem)
		memcpy(newmem, oldmem, min(oldlen, newlen));
	kfree(oldmem);
	return newmem;
}

static int doDevConfig(struct comedi_device *dev, struct comedi_devconfig *it)
{
	struct comedi_bond_private *devpriv = dev->private;
	int i;
	struct comedi_device *devs_opened[COMEDI_NUM_BOARD_MINORS];

	memset(devs_opened, 0, sizeof(devs_opened));
	devpriv->name[0] = 0;
	/* Loop through all comedi devices specified on the command-line,
	   building our device list */
	for (i = 0; i < COMEDI_NDEVCONFOPTS && (!i || it->options[i]); ++i) {
		char file[] = "/dev/comediXXXXXX";
		int minor = it->options[i];
		struct comedi_device *d;
		int sdev = -1, nchans, tmp;
		struct BondedDevice *bdev = NULL;
=======
	unsigned int chan = CR_CHAN(insn->chanspec);
	int ret;
	struct bonded_device *bdev;
	struct bonded_device **devs;

	/*
	 * Locate bonded subdevice and adjust channel.
	 */
	devs = devpriv->devs;
	for (bdev = *devs++; chan >= bdev->nchans; bdev = *devs++)
		chan -= bdev->nchans;

	/*
	 * The input or output configuration of each digital line is
	 * configured by a special insn_config instruction.  chanspec
	 * contains the channel to be changed, and data[0] contains the
	 * configuration instruction INSN_CONFIG_DIO_OUTPUT,
	 * INSN_CONFIG_DIO_INPUT or INSN_CONFIG_DIO_QUERY.
	 *
	 * Note that INSN_CONFIG_DIO_OUTPUT == COMEDI_OUTPUT,
	 * and INSN_CONFIG_DIO_INPUT == COMEDI_INPUT.  This is deliberate ;)
	 */
	switch (data[0]) {
	case INSN_CONFIG_DIO_OUTPUT:
	case INSN_CONFIG_DIO_INPUT:
		ret = comedi_dio_config(bdev->dev, bdev->subdev, chan, data[0]);
		break;
	case INSN_CONFIG_DIO_QUERY:
		ret = comedi_dio_get_config(bdev->dev, bdev->subdev, chan,
					    &data[1]);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	if (ret >= 0)
		ret = insn->n;
	return ret;
}

static int do_dev_config(struct comedi_device *dev, struct comedi_devconfig *it)
{
	struct comedi_bond_private *devpriv = dev->private;
	DECLARE_BITMAP(devs_opened, COMEDI_NUM_BOARD_MINORS);
	int i;

	memset(&devs_opened, 0, sizeof(devs_opened));
	devpriv->name[0] = 0;
	/*
	 * Loop through all comedi devices specified on the command-line,
	 * building our device list.
	 */
	for (i = 0; i < COMEDI_NDEVCONFOPTS && (!i || it->options[i]); ++i) {
		char file[sizeof("/dev/comediXXXXXX")];
		int minor = it->options[i];
		struct comedi_device *d;
		int sdev = -1, nchans;
		struct bonded_device *bdev;
		struct bonded_device **devs;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (minor < 0 || minor >= COMEDI_NUM_BOARD_MINORS) {
			dev_err(dev->class_dev,
				"Minor %d is invalid!\n", minor);
<<<<<<< HEAD
			return 0;
=======
			return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		if (minor == dev->minor) {
			dev_err(dev->class_dev,
				"Cannot bond this driver to itself!\n");
<<<<<<< HEAD
			return 0;
		}
		if (devs_opened[minor]) {
			dev_err(dev->class_dev,
				"Minor %d specified more than once!\n", minor);
			return 0;
		}

		snprintf(file, sizeof(file), "/dev/comedi%u", minor);
		file[sizeof(file) - 1] = 0;

		d = devs_opened[minor] = comedi_open(file);
=======
			return -EINVAL;
		}
		if (test_and_set_bit(minor, devs_opened)) {
			dev_err(dev->class_dev,
				"Minor %d specified more than once!\n", minor);
			return -EINVAL;
		}

		snprintf(file, sizeof(file), "/dev/comedi%d", minor);
		file[sizeof(file) - 1] = 0;

		d = comedi_open(file);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (!d) {
			dev_err(dev->class_dev,
				"Minor %u could not be opened\n", minor);
<<<<<<< HEAD
			return 0;
=======
			return -ENODEV;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/* Do DIO, as that's all we support now.. */
		while ((sdev = comedi_find_subdevice_by_type(d, COMEDI_SUBD_DIO,
							     sdev + 1)) > -1) {
			nchans = comedi_get_n_channels(d, sdev);
			if (nchans <= 0) {
				dev_err(dev->class_dev,
					"comedi_get_n_channels() returned %d on minor %u subdev %d!\n",
					nchans, minor, sdev);
<<<<<<< HEAD
				return 0;
			}
			bdev = kmalloc(sizeof(*bdev), GFP_KERNEL);
			if (!bdev)
				return 0;
=======
				return -EINVAL;
			}
			bdev = kmalloc(sizeof(*bdev), GFP_KERNEL);
			if (!bdev)
				return -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			bdev->dev = d;
			bdev->minor = minor;
			bdev->subdev = sdev;
<<<<<<< HEAD
			bdev->subdev_type = COMEDI_SUBD_DIO;
			bdev->nchans = nchans;
			bdev->chanid_offset = devpriv->nchans;

			/* map channel id's to BondedDevice * pointer.. */
			while (nchans--)
				devpriv->chanIdDevMap[devpriv->nchans++] = bdev;

			/* Now put bdev pointer at end of devpriv->devs array
			 * list.. */

			/* ergh.. ugly.. we need to realloc :(  */
			tmp = devpriv->ndevs * sizeof(bdev);
			devpriv->devs =
			    Realloc(devpriv->devs,
				    ++devpriv->ndevs * sizeof(bdev), tmp);
			if (!devpriv->devs) {
				dev_err(dev->class_dev,
					"Could not allocate memory. Out of memory?\n");
				return 0;
			}

			devpriv->devs[devpriv->ndevs - 1] = bdev;
			{
	/** Append dev:subdev to devpriv->name */
				char buf[20];
				int left =
				    MAX_BOARD_NAME - strlen(devpriv->name) - 1;
				snprintf(buf, sizeof(buf), "%d:%d ", dev->minor,
					 bdev->subdev);
				buf[sizeof(buf) - 1] = 0;
				strncat(devpriv->name, buf, left);
			}

=======
			bdev->nchans = nchans;
			devpriv->nchans += nchans;

			/*
			 * Now put bdev pointer at end of devpriv->devs array
			 * list..
			 */

			/* ergh.. ugly.. we need to realloc :(  */
			devs = krealloc(devpriv->devs,
					(devpriv->ndevs + 1) * sizeof(*devs),
					GFP_KERNEL);
			if (!devs) {
				dev_err(dev->class_dev,
					"Could not allocate memory. Out of memory?\n");
				kfree(bdev);
				return -ENOMEM;
			}
			devpriv->devs = devs;
			devpriv->devs[devpriv->ndevs++] = bdev;
			{
				/* Append dev:subdev to devpriv->name */
				char buf[20];

				snprintf(buf, sizeof(buf), "%u:%u ",
					 bdev->minor, bdev->subdev);
				strlcat(devpriv->name, buf,
					sizeof(devpriv->name));
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	if (!devpriv->nchans) {
		dev_err(dev->class_dev, "No channels found!\n");
<<<<<<< HEAD
		return 0;
	}

	return 1;
=======
		return -EINVAL;
	}

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int bonding_attach(struct comedi_device *dev,
			  struct comedi_devconfig *it)
{
	struct comedi_bond_private *devpriv;
	struct comedi_subdevice *s;
	int ret;

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

	/*
	 * Setup our bonding from config params.. sets up our private struct..
	 */
<<<<<<< HEAD
	if (!doDevConfig(dev, it))
		return -EINVAL;
=======
	ret = do_dev_config(dev, it);
	if (ret)
		return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	dev->board_name = devpriv->name;

	ret = comedi_alloc_subdevices(dev, 1);
	if (ret)
		return ret;

	s = &dev->subdevices[0];
	s->type = COMEDI_SUBD_DIO;
	s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
	s->n_chan = devpriv->nchans;
	s->maxdata = 1;
	s->range_table = &range_digital;
	s->insn_bits = bonding_dio_insn_bits;
	s->insn_config = bonding_dio_insn_config;

	dev_info(dev->class_dev,
<<<<<<< HEAD
		"%s: %s attached, %u channels from %u devices\n",
		dev->driver->driver_name, dev->board_name,
		devpriv->nchans, devpriv->ndevs);

	return 1;
=======
		 "%s: %s attached, %u channels from %u devices\n",
		 dev->driver->driver_name, dev->board_name,
		 devpriv->nchans, devpriv->ndevs);

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void bonding_detach(struct comedi_device *dev)
{
	struct comedi_bond_private *devpriv = dev->private;
<<<<<<< HEAD
	unsigned long devs_closed = 0;

	if (devpriv) {
		while (devpriv->ndevs-- && devpriv->devs) {
			struct BondedDevice *bdev;
=======

	if (devpriv && devpriv->devs) {
		DECLARE_BITMAP(devs_closed, COMEDI_NUM_BOARD_MINORS);

		memset(&devs_closed, 0, sizeof(devs_closed));
		while (devpriv->ndevs--) {
			struct bonded_device *bdev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			bdev = devpriv->devs[devpriv->ndevs];
			if (!bdev)
				continue;
<<<<<<< HEAD
			if (!(devs_closed & (0x1 << bdev->minor))) {
				comedi_close(bdev->dev);
				devs_closed |= (0x1 << bdev->minor);
			}
=======
			if (!test_and_set_bit(bdev->minor, devs_closed))
				comedi_close(bdev->dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			kfree(bdev);
		}
		kfree(devpriv->devs);
		devpriv->devs = NULL;
<<<<<<< HEAD
		kfree(devpriv);
		dev->private = NULL;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

static struct comedi_driver bonding_driver = {
	.driver_name	= "comedi_bond",
	.module		= THIS_MODULE,
	.attach		= bonding_attach,
	.detach		= bonding_detach,
};
module_comedi_driver(bonding_driver);

MODULE_AUTHOR("Calin A. Culianu");
<<<<<<< HEAD
MODULE_DESCRIPTION("comedi_bond: A driver for COMEDI to bond multiple COMEDI "
		   "devices together as one.  In the words of John Lennon: "
		   "'And the world will live as one...'");
=======
MODULE_DESCRIPTION("comedi_bond: A driver for COMEDI to bond multiple COMEDI devices together as one.");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_LICENSE("GPL");
