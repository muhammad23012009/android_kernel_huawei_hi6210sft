/*
<<<<<<< HEAD
    comedi/drivers/pcm724.c

    Drew Csillag <drew_csillag@yahoo.com>

    hardware driver for Advantech card:
     card:   PCM-3724
     driver: pcm3724

    Options for PCM-3724
     [0] - IO Base
*/
/*
Driver: pcm3724
Description: Advantech PCM-3724
Author: Drew Csillag <drew_csillag@yahoo.com>
Devices: [Advantech] PCM-3724 (pcm724)
Status: tested

This is driver for digital I/O boards PCM-3724 with 48 DIO.
It needs 8255.o for operations and only immediate mode is supported.
See the source for configuration details.

Copy/pasted/hacked from pcm724.c
*/
/*
 * check_driver overrides:
 *   struct comedi_insn
 */

#include "../comedidev.h"

#include <linux/ioport.h>
#include <linux/delay.h>

#include "8255.h"

#define PCM3724_SIZE   16
#define SIZE_8255	4

#define BUF_C0 0x1
#define BUF_B0 0x2
#define BUF_A0 0x4
#define BUF_C1 0x8
#define BUF_B1 0x10
#define BUF_A1 0x20

#define GATE_A0 0x4
#define GATE_B0	0x2
#define GATE_C0	0x1
#define GATE_A1	0x20
#define GATE_B1	0x10
#define GATE_C1 0x8

/* from 8255.c */
#define CR_CW		0x80
#define _8255_CR 3
#define CR_B_IO		0x02
#define CR_B_MODE	0x04
#define CR_C_IO	        0x09
#define CR_A_IO		0x10
#define CR_A_MODE(a)	((a)<<5)
#define CR_CW		0x80
=======
 * pcm3724.c
 * Comedi driver for Advantech PCM-3724 Digital I/O board
 *
 * Drew Csillag <drew_csillag@yahoo.com>
 */

/*
 * Driver: pcm3724
 * Description: Advantech PCM-3724
 * Devices: [Advantech] PCM-3724 (pcm3724)
 * Author: Drew Csillag <drew_csillag@yahoo.com>
 * Status: tested
 *
 * This is driver for digital I/O boards PCM-3724 with 48 DIO.
 * It needs 8255.o for operations and only immediate mode is supported.
 * See the source for configuration details.
 *
 * Copy/pasted/hacked from pcm724.c
 *
 * Configuration Options:
 *   [0] - I/O port base address
 */

#include <linux/module.h>
#include "../comedidev.h"

#include "8255.h"

/*
 * Register I/O Map
 *
 * This board has two standard 8255 devices that provide six 8-bit DIO ports
 * (48 channels total). Six 74HCT245 chips (one for each port) buffer the
 * I/O lines to increase driving capability. Because the 74HCT245 is a
 * bidirectional, tri-state line buffer, two additional I/O ports are used
 * to control the direction of data and the enable of each port.
 */
#define PCM3724_8255_0_BASE		0x00
#define PCM3724_8255_1_BASE		0x04
#define PCM3724_DIO_DIR_REG		0x08
#define PCM3724_DIO_DIR_C0_OUT		BIT(0)
#define PCM3724_DIO_DIR_B0_OUT		BIT(1)
#define PCM3724_DIO_DIR_A0_OUT		BIT(2)
#define PCM3724_DIO_DIR_C1_OUT		BIT(3)
#define PCM3724_DIO_DIR_B1_OUT		BIT(4)
#define PCM3724_DIO_DIR_A1_OUT		BIT(5)
#define PCM3724_GATE_CTRL_REG		0x09
#define PCM3724_GATE_CTRL_C0_ENA	BIT(0)
#define PCM3724_GATE_CTRL_B0_ENA	BIT(1)
#define PCM3724_GATE_CTRL_A0_ENA	BIT(2)
#define PCM3724_GATE_CTRL_C1_ENA	BIT(3)
#define PCM3724_GATE_CTRL_B1_ENA	BIT(4)
#define PCM3724_GATE_CTRL_A1_ENA	BIT(5)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* used to track configured dios */
struct priv_pcm3724 {
	int dio_1;
	int dio_2;
};

<<<<<<< HEAD
static int subdev_8255_cb(int dir, int port, int data, unsigned long arg)
{
	unsigned long iobase = arg;
	unsigned char inbres;
	/* printk("8255cb %d %d %d %lx\n", dir,port,data,arg); */
	if (dir) {
		/* printk("8255 cb   outb(%x, %lx)\n", data, iobase+port); */
		outb(data, iobase + port);
		return 0;
	} else {
		inbres = inb(iobase + port);
		/* printk("8255 cb   inb(%lx) = %x\n", iobase+port, inbres); */
		return inbres;
	}
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int compute_buffer(int config, int devno, struct comedi_subdevice *s)
{
	/* 1 in io_bits indicates output */
	if (s->io_bits & 0x0000ff) {
		if (devno == 0)
<<<<<<< HEAD
			config |= BUF_A0;
		else
			config |= BUF_A1;
	}
	if (s->io_bits & 0x00ff00) {
		if (devno == 0)
			config |= BUF_B0;
		else
			config |= BUF_B1;
	}
	if (s->io_bits & 0xff0000) {
		if (devno == 0)
			config |= BUF_C0;
		else
			config |= BUF_C1;
=======
			config |= PCM3724_DIO_DIR_A0_OUT;
		else
			config |= PCM3724_DIO_DIR_A1_OUT;
	}
	if (s->io_bits & 0x00ff00) {
		if (devno == 0)
			config |= PCM3724_DIO_DIR_B0_OUT;
		else
			config |= PCM3724_DIO_DIR_B1_OUT;
	}
	if (s->io_bits & 0xff0000) {
		if (devno == 0)
			config |= PCM3724_DIO_DIR_C0_OUT;
		else
			config |= PCM3724_DIO_DIR_C1_OUT;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return config;
}

static void do_3724_config(struct comedi_device *dev,
			   struct comedi_subdevice *s, int chanspec)
{
	struct comedi_subdevice *s_dio1 = &dev->subdevices[0];
	struct comedi_subdevice *s_dio2 = &dev->subdevices[1];
	int config;
	int buffer_config;
	unsigned long port_8255_cfg;

<<<<<<< HEAD
	config = CR_CW;
=======
	config = I8255_CTRL_CW;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	buffer_config = 0;

	/* 1 in io_bits indicates output, 1 in config indicates input */
	if (!(s->io_bits & 0x0000ff))
<<<<<<< HEAD
		config |= CR_A_IO;

	if (!(s->io_bits & 0x00ff00))
		config |= CR_B_IO;

	if (!(s->io_bits & 0xff0000))
		config |= CR_C_IO;
=======
		config |= I8255_CTRL_A_IO;

	if (!(s->io_bits & 0x00ff00))
		config |= I8255_CTRL_B_IO;

	if (!(s->io_bits & 0xff0000))
		config |= I8255_CTRL_C_HI_IO | I8255_CTRL_C_LO_IO;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	buffer_config = compute_buffer(0, 0, s_dio1);
	buffer_config = compute_buffer(buffer_config, 1, s_dio2);

	if (s == s_dio1)
<<<<<<< HEAD
		port_8255_cfg = dev->iobase + _8255_CR;
	else
		port_8255_cfg = dev->iobase + SIZE_8255 + _8255_CR;

	outb(buffer_config, dev->iobase + 8);	/* update buffer register */
	/* printk("pcm3724 buffer_config (%lx) %d, %x\n",
	       dev->iobase + _8255_CR, chanspec, buffer_config); */
=======
		port_8255_cfg = dev->iobase + I8255_CTRL_REG;
	else
		port_8255_cfg = dev->iobase + I8255_SIZE + I8255_CTRL_REG;

	outb(buffer_config, dev->iobase + PCM3724_DIO_DIR_REG);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	outb(config, port_8255_cfg);
}

static void enable_chan(struct comedi_device *dev, struct comedi_subdevice *s,
			int chanspec)
{
	struct priv_pcm3724 *priv = dev->private;
	struct comedi_subdevice *s_dio1 = &dev->subdevices[0];
	unsigned int mask;
	int gatecfg;

	gatecfg = 0;

	mask = 1 << CR_CHAN(chanspec);
	if (s == s_dio1)
		priv->dio_1 |= mask;
	else
		priv->dio_2 |= mask;

	if (priv->dio_1 & 0xff0000)
<<<<<<< HEAD
		gatecfg |= GATE_C0;

	if (priv->dio_1 & 0xff00)
		gatecfg |= GATE_B0;

	if (priv->dio_1 & 0xff)
		gatecfg |= GATE_A0;

	if (priv->dio_2 & 0xff0000)
		gatecfg |= GATE_C1;

	if (priv->dio_2 & 0xff00)
		gatecfg |= GATE_B1;

	if (priv->dio_2 & 0xff)
		gatecfg |= GATE_A1;

	/*       printk("gate control %x\n", gatecfg); */
	outb(gatecfg, dev->iobase + 9);
=======
		gatecfg |= PCM3724_GATE_CTRL_C0_ENA;

	if (priv->dio_1 & 0xff00)
		gatecfg |= PCM3724_GATE_CTRL_B0_ENA;

	if (priv->dio_1 & 0xff)
		gatecfg |= PCM3724_GATE_CTRL_A0_ENA;

	if (priv->dio_2 & 0xff0000)
		gatecfg |= PCM3724_GATE_CTRL_C1_ENA;

	if (priv->dio_2 & 0xff00)
		gatecfg |= PCM3724_GATE_CTRL_B1_ENA;

	if (priv->dio_2 & 0xff)
		gatecfg |= PCM3724_GATE_CTRL_A1_ENA;

	outb(gatecfg, dev->iobase + PCM3724_GATE_CTRL_REG);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* overriding the 8255 insn config */
static int subdev_3724_insn_config(struct comedi_device *dev,
				   struct comedi_subdevice *s,
<<<<<<< HEAD
				   struct comedi_insn *insn, unsigned int *data)
{
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

	do_3724_config(dev, s, insn->chanspec);
	enable_chan(dev, s, insn->chanspec);
	return 1;
=======
				   struct comedi_insn *insn,
				   unsigned int *data)
{
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

	do_3724_config(dev, s, insn->chanspec);
	enable_chan(dev, s, insn->chanspec);

	return insn->n;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int pcm3724_attach(struct comedi_device *dev,
			  struct comedi_devconfig *it)
{
	struct priv_pcm3724 *priv;
	struct comedi_subdevice *s;
	int ret, i;

<<<<<<< HEAD
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
	dev->private = priv;

	ret = comedi_request_region(dev, it->options[0], PCM3724_SIZE);
=======
	priv = comedi_alloc_devpriv(dev, sizeof(*priv));
	if (!priv)
		return -ENOMEM;

	ret = comedi_request_region(dev, it->options[0], 0x10);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret)
		return ret;

	ret = comedi_alloc_subdevices(dev, 2);
	if (ret)
		return ret;

	for (i = 0; i < dev->n_subdevices; i++) {
		s = &dev->subdevices[i];
<<<<<<< HEAD
		subdev_8255_init(dev, s, subdev_8255_cb,
				 (unsigned long)(dev->iobase + SIZE_8255 * i));
=======
		ret = subdev_8255_init(dev, s, NULL, i * I8255_SIZE);
		if (ret)
			return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		s->insn_config = subdev_3724_insn_config;
	}
	return 0;
}

<<<<<<< HEAD
static void pcm3724_detach(struct comedi_device *dev)
{
	int i;

	for (i = 0; i < dev->n_subdevices; i++)
		comedi_spriv_free(dev, i);
	comedi_legacy_detach(dev);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct comedi_driver pcm3724_driver = {
	.driver_name	= "pcm3724",
	.module		= THIS_MODULE,
	.attach		= pcm3724_attach,
<<<<<<< HEAD
	.detach		= pcm3724_detach,
=======
	.detach		= comedi_legacy_detach,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
module_comedi_driver(pcm3724_driver);

MODULE_AUTHOR("Comedi http://www.comedi.org");
<<<<<<< HEAD
MODULE_DESCRIPTION("Comedi low-level driver");
=======
MODULE_DESCRIPTION("Comedi driver for Advantech PCM-3724 Digital I/O board");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_LICENSE("GPL");
