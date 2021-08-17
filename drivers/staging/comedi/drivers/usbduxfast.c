/*
<<<<<<< HEAD
 *  Copyright (C) 2004 Bernd Porr, Bernd.Porr@f2s.com
=======
 *  Copyright (C) 2004-2019 Bernd Porr, mail@berndporr.me.uk
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
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
<<<<<<< HEAD
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
 */

/*
 * Driver: usbduxfast
 * Description: University of Stirling USB DAQ & INCITE Technology Limited
 * Devices: [ITL] USB-DUX-FAST (usbduxfast)
 * Author: Bernd Porr <mail@berndporr.me.uk>
 * Updated: 16 Nov 2019
 * Status: stable
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

/*
 * I must give credit here to Chris Baugher who
 * wrote the driver for AT-MIO-16d. I used some parts of this
 * driver. I also must give credits to David Brownell
 * who supported me with the USB development.
 *
 * Bernd Porr
 *
 *
 * Revision history:
<<<<<<< HEAD
=======
 * 1.0: Fixed a rounding error in usbduxfast_ai_cmdtest
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * 0.9: Dropping the first data packet which seems to be from the last transfer.
 *      Buffer overflows in the FX2 are handed over to comedi.
 * 0.92: Dropping now 4 packets. The quad buffer has to be emptied.
 *       Added insn command basically for testing. Sample rate is
 *       1MHz/16ch=62.5kHz
 * 0.99: Ian Abbott pointed out a bug which has been corrected. Thanks!
 * 0.99a: added external trigger.
 * 1.00: added firmware kernel request to the driver which fixed
 *       udev coldplug problem
 */

<<<<<<< HEAD
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/firmware.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/usb.h>
#include <linux/fcntl.h>
#include <linux/compiler.h>
#include "comedi_fc.h"
#include "../comedidev.h"
=======
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/fcntl.h>
#include <linux/compiler.h>
#include "../comedi_usb.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * timeout for the USB-transfer
 */
#define EZTIMEOUT	30

/*
 * constants for "firmware" upload and download
 */
#define FIRMWARE		"usbduxfast_firmware.bin"
<<<<<<< HEAD
=======
#define FIRMWARE_MAX_LEN	0x2000
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define USBDUXFASTSUB_FIRMWARE	0xA0
#define VENDOR_DIR_IN		0xC0
#define VENDOR_DIR_OUT		0x40

/*
 * internal addresses of the 8051 processor
 */
#define USBDUXFASTSUB_CPUCS	0xE600

/*
 * max lenghth of the transfer-buffer for software upload
 */
#define TB_LEN	0x2000

/*
 * input endpoint number
 */
#define BULKINEP	6

/*
 * endpoint for the A/D channellist: bulk OUT
 */
#define CHANNELLISTEP	4

/*
 * number of channels
 */
#define NUMCHANNELS	32

/*
 * size of the waveform descriptor
 */
#define WAVESIZE	0x20

/*
 * size of one A/D value
 */
<<<<<<< HEAD
#define SIZEADIN	(sizeof(int16_t))
=======
#define SIZEADIN	(sizeof(s16))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * size of the input-buffer IN BYTES
 */
#define SIZEINBUF	512

/*
 * 16 bytes
 */
#define SIZEINSNBUF	512

/*
 * size of the buffer for the dux commands in bytes
 */
<<<<<<< HEAD
#define SIZEOFDUXBUFFER	256
=======
#define SIZEOFDUXBUF	256
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * number of in-URBs which receive the data: min=5
 */
#define NUMOFINBUFFERSHIGH	10

/*
<<<<<<< HEAD
 * total number of usbduxfast devices
 */
#define NUMUSBDUXFAST	16

/*
 * analogue in subdevice
 */
#define SUBDEV_AD	0

/*
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * min delay steps for more than one channel
 * basically when the mux gives up ;-)
 *
 * steps at 30MHz in the FX2
 */
#define MIN_SAMPLING_PERIOD	9

/*
 * max number of 1/30MHz delay steps
 */
#define MAX_SAMPLING_PERIOD	500

/*
 * number of received packets to ignore before we start handing data
 * over to comedi, it's quad buffering and we have to ignore 4 packets
 */
#define PACKETS_TO_IGNORE	4

/*
 * comedi constants
 */
static const struct comedi_lrange range_usbduxfast_ai_range = {
<<<<<<< HEAD
	2, {BIP_RANGE(0.75), BIP_RANGE(0.5)}
=======
	2, {
		BIP_RANGE(0.75),
		BIP_RANGE(0.5)
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/*
 * private structure of one subdevice
 *
 * this is the structure which holds all the data of this driver
 * one sub device just now: A/D
 */
<<<<<<< HEAD
struct usbduxfastsub_s {
	int attached;		/* is attached? */
	int probed;		/* is it associated with a subdevice? */
	struct usb_device *usbdev;	/* pointer to the usb-device */
	struct urb *urbIn;	/* BULK-transfer handling: urb */
	int8_t *transfer_buffer;
	int16_t *insnBuffer;	/* input buffer for single insn */
	int ifnum;		/* interface number */
	struct usb_interface *interface;	/* interface structure */
	/* comedi device for the interrupt context */
	struct comedi_device *comedidev;
	short int ai_cmd_running;	/* asynchronous command is running */
	short int ai_continous;	/* continous acquisition */
	long int ai_sample_count;	/* number of samples to acquire */
	uint8_t *dux_commands;	/* commands */
	int ignore;		/* counter which ignores the first
				   buffers */
	struct semaphore sem;
};

/*
 * The pointer to the private usb-data of the driver
 * is also the private data for the comedi-device.
 * This has to be global as the usb subsystem needs
 * global variables. The other reason is that this
 * structure must be there _before_ any comedi
 * command is issued. The usb subsystem must be
 * initialised before comedi can access it.
 */
static struct usbduxfastsub_s usbduxfastsub[NUMUSBDUXFAST];

static DEFINE_SEMAPHORE(start_stop_sem);

/*
=======
struct usbduxfast_private {
	struct urb *urb;	/* BULK-transfer handling: urb */
	u8 *duxbuf;
	s8 *inbuf;
	short int ai_cmd_running;	/* asynchronous command is running */
	int ignore;		/* counter which ignores the first buffers */
	struct mutex mut;
};

/*
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * bulk transfers to usbduxfast
 */
#define SENDADCOMMANDS            0
#define SENDINITEP6               1

<<<<<<< HEAD
static int send_dux_commands(struct usbduxfastsub_s *udfs, int cmd_type)
{
	int tmp, nsent;

	udfs->dux_commands[0] = cmd_type;

#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi%d: usbduxfast: dux_commands: ",
	       udfs->comedidev->minor);
	for (tmp = 0; tmp < SIZEOFDUXBUFFER; tmp++)
		printk(" %02x", udfs->dux_commands[tmp]);
	printk("\n");
#endif

	tmp = usb_bulk_msg(udfs->usbdev,
			   usb_sndbulkpipe(udfs->usbdev, CHANNELLISTEP),
			   udfs->dux_commands, SIZEOFDUXBUFFER, &nsent, 10000);
	if (tmp < 0)
		dev_err(&udfs->interface->dev,
			"could not transmit dux_commands to the usb-device, err=%d\n",
			tmp);
	return tmp;
}

/*
 * Stops the data acquision.
 * It should be safe to call this function from any context.
 */
static int usbduxfastsub_unlink_InURBs(struct usbduxfastsub_s *udfs)
{
	int j = 0;
	int err = 0;

	if (udfs && udfs->urbIn) {
		udfs->ai_cmd_running = 0;
		/* waits until a running transfer is over */
		usb_kill_urb(udfs->urbIn);
		j = 0;
	}
#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi: usbduxfast: unlinked InURB: res=%d\n", j);
#endif
	return err;
}

/*
 * This will stop a running acquisition operation.
 * Is called from within this driver from both the
 * interrupt context and from comedi.
 */
static int usbduxfast_ai_stop(struct usbduxfastsub_s *udfs, int do_unlink)
{
	int ret = 0;

	if (!udfs) {
		pr_err("%s: udfs=NULL!\n", __func__);
		return -EFAULT;
	}
#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi: usbduxfast_ai_stop\n");
#endif

	udfs->ai_cmd_running = 0;

	if (do_unlink)
		/* stop aquistion */
		ret = usbduxfastsub_unlink_InURBs(udfs);

	return ret;
}

/*
 * This will cancel a running acquisition operation.
 * This is called by comedi but never from inside the driver.
 */
static int usbduxfast_ai_cancel(struct comedi_device *dev,
				struct comedi_subdevice *s)
{
	struct usbduxfastsub_s *udfs;
	int ret;

	/* force unlink of all urbs */
#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi: usbduxfast_ai_cancel\n");
#endif
	udfs = dev->private;
	if (!udfs) {
		dev_err(dev->class_dev, "%s: udfs=NULL\n", __func__);
		return -EFAULT;
	}
	down(&udfs->sem);
	if (!udfs->probed) {
		up(&udfs->sem);
		return -ENODEV;
	}
	/* unlink */
	ret = usbduxfast_ai_stop(udfs, 1);
	up(&udfs->sem);
=======
static int usbduxfast_send_cmd(struct comedi_device *dev, int cmd_type)
{
	struct usb_device *usb = comedi_to_usb_dev(dev);
	struct usbduxfast_private *devpriv = dev->private;
	int nsent;
	int ret;

	devpriv->duxbuf[0] = cmd_type;

	ret = usb_bulk_msg(usb, usb_sndbulkpipe(usb, CHANNELLISTEP),
			   devpriv->duxbuf, SIZEOFDUXBUF,
			   &nsent, 10000);
	if (ret < 0)
		dev_err(dev->class_dev,
			"could not transmit command to the usb-device, err=%d\n",
			ret);
	return ret;
}

static void usbduxfast_cmd_data(struct comedi_device *dev, int index,
				u8 len, u8 op, u8 out, u8 log)
{
	struct usbduxfast_private *devpriv = dev->private;

	/* Set the GPIF bytes, the first byte is the command byte */
	devpriv->duxbuf[1 + 0x00 + index] = len;
	devpriv->duxbuf[1 + 0x08 + index] = op;
	devpriv->duxbuf[1 + 0x10 + index] = out;
	devpriv->duxbuf[1 + 0x18 + index] = log;
}

static int usbduxfast_ai_stop(struct comedi_device *dev, int do_unlink)
{
	struct usbduxfast_private *devpriv = dev->private;

	/* stop aquistion */
	devpriv->ai_cmd_running = 0;

	if (do_unlink && devpriv->urb) {
		/* kill the running transfer */
		usb_kill_urb(devpriv->urb);
	}

	return 0;
}

static int usbduxfast_ai_cancel(struct comedi_device *dev,
				struct comedi_subdevice *s)
{
	struct usbduxfast_private *devpriv = dev->private;
	int ret;

	mutex_lock(&devpriv->mut);
	ret = usbduxfast_ai_stop(dev, 1);
	mutex_unlock(&devpriv->mut);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ret;
}

<<<<<<< HEAD
/*
 * analogue IN
 * interrupt service routine
 */
static void usbduxfastsub_ai_Irq(struct urb *urb)
{
	int n, err;
	struct usbduxfastsub_s *udfs;
	struct comedi_device *this_comedidev;
	struct comedi_subdevice *s;

	/* sanity checks - is the urb there? */
	if (!urb) {
		pr_err("ao int-handler called with urb=NULL!\n");
		return;
	}
	/* the context variable points to the subdevice */
	this_comedidev = urb->context;
	if (!this_comedidev) {
		pr_err("urb context is a NULL pointer!\n");
		return;
	}
	/* the private structure of the subdevice is usbduxfastsub_s */
	udfs = this_comedidev->private;
	if (!udfs) {
		pr_err("private of comedi subdev is a NULL pointer!\n");
		return;
	}
	/* are we running a command? */
	if (unlikely(!udfs->ai_cmd_running)) {
		/*
		 * not running a command
		 * do not continue execution if no asynchronous command
		 * is running in particular not resubmit
		 */
		return;
	}

	if (unlikely(!udfs->attached)) {
		/* no comedi device there */
		return;
	}
	/* subdevice which is the AD converter */
	s = &this_comedidev->subdevices[SUBDEV_AD];

	/* first we test if something unusual has just happened */
	switch (urb->status) {
	case 0:
		break;

		/*
		 * happens after an unlink command or when the device
		 * is plugged out
		 */
=======
static void usbduxfast_ai_handle_urb(struct comedi_device *dev,
				     struct comedi_subdevice *s,
				     struct urb *urb)
{
	struct usbduxfast_private *devpriv = dev->private;
	struct comedi_async *async = s->async;
	struct comedi_cmd *cmd = &async->cmd;
	int ret;

	if (devpriv->ignore) {
		devpriv->ignore--;
	} else {
		unsigned int nsamples;

		nsamples = comedi_bytes_to_samples(s, urb->actual_length);
		nsamples = comedi_nsamples_left(s, nsamples);
		comedi_buf_write_samples(s, urb->transfer_buffer, nsamples);

		if (cmd->stop_src == TRIG_COUNT &&
		    async->scans_done >= cmd->stop_arg)
			async->events |= COMEDI_CB_EOA;
	}

	/* if command is still running, resubmit urb for BULK transfer */
	if (!(async->events & COMEDI_CB_CANCEL_MASK)) {
		urb->dev = comedi_to_usb_dev(dev);
		urb->status = 0;
		ret = usb_submit_urb(urb, GFP_ATOMIC);
		if (ret < 0) {
			dev_err(dev->class_dev, "urb resubm failed: %d", ret);
			async->events |= COMEDI_CB_ERROR;
		}
	}
}

static void usbduxfast_ai_interrupt(struct urb *urb)
{
	struct comedi_device *dev = urb->context;
	struct comedi_subdevice *s = dev->read_subdev;
	struct comedi_async *async = s->async;
	struct usbduxfast_private *devpriv = dev->private;

	/* exit if not running a command, do not resubmit urb */
	if (!devpriv->ai_cmd_running)
		return;

	switch (urb->status) {
	case 0:
		usbduxfast_ai_handle_urb(dev, s, urb);
		break;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
	case -ECONNABORTED:
<<<<<<< HEAD
		/* tell this comedi */
		s->async->events |= COMEDI_CB_EOA;
		s->async->events |= COMEDI_CB_ERROR;
		comedi_event(udfs->comedidev, s);
		/* stop the transfer w/o unlink */
		usbduxfast_ai_stop(udfs, 0);
		return;

	default:
		pr_err("non-zero urb status received in ai intr context: %d\n",
		       urb->status);
		s->async->events |= COMEDI_CB_EOA;
		s->async->events |= COMEDI_CB_ERROR;
		comedi_event(udfs->comedidev, s);
		usbduxfast_ai_stop(udfs, 0);
		return;
	}

	if (!udfs->ignore) {
		if (!udfs->ai_continous) {
			/* not continuous, fixed number of samples */
			n = urb->actual_length / sizeof(uint16_t);
			if (unlikely(udfs->ai_sample_count < n)) {
				/*
				 * we have send only a fraction of the bytes
				 * received
				 */
				cfc_write_array_to_buffer(s,
							  urb->transfer_buffer,
							  udfs->ai_sample_count
							  * sizeof(uint16_t));
				usbduxfast_ai_stop(udfs, 0);
				/* tell comedi that the acquistion is over */
				s->async->events |= COMEDI_CB_EOA;
				comedi_event(udfs->comedidev, s);
				return;
			}
			udfs->ai_sample_count -= n;
		}
		/* write the full buffer to comedi */
		err = cfc_write_array_to_buffer(s, urb->transfer_buffer,
						urb->actual_length);
		if (unlikely(err == 0)) {
			/* buffer overflow */
			usbduxfast_ai_stop(udfs, 0);
			return;
		}

		/* tell comedi that data is there */
		comedi_event(udfs->comedidev, s);

	} else {
		/* ignore this packet */
		udfs->ignore--;
	}

	/*
	 * command is still running
	 * resubmit urb for BULK transfer
	 */
	urb->dev = udfs->usbdev;
	urb->status = 0;
	err = usb_submit_urb(urb, GFP_ATOMIC);
	if (err < 0) {
		dev_err(&urb->dev->dev,
			"urb resubm failed: %d", err);
		s->async->events |= COMEDI_CB_EOA;
		s->async->events |= COMEDI_CB_ERROR;
		comedi_event(udfs->comedidev, s);
		usbduxfast_ai_stop(udfs, 0);
	}
}

static int usbduxfastsub_start(struct usbduxfastsub_s *udfs)
{
	int ret;
	unsigned char *local_transfer_buffer;

	local_transfer_buffer = kmalloc(1, GFP_KERNEL);
	if (!local_transfer_buffer)
		return -ENOMEM;

	/* 7f92 to zero */
	*local_transfer_buffer = 0;
	/* bRequest, "Firmware" */
	ret = usb_control_msg(udfs->usbdev, usb_sndctrlpipe(udfs->usbdev, 0),
			      USBDUXFASTSUB_FIRMWARE,
			      VENDOR_DIR_OUT,	  /* bmRequestType */
			      USBDUXFASTSUB_CPUCS,    /* Value */
			      0x0000,	/* Index */
			      /* address of the transfer buffer */
			      local_transfer_buffer,
			      1,      /* Length */
			      EZTIMEOUT);    /* Timeout */
	if (ret < 0)
		dev_err(&udfs->interface->dev,
			"control msg failed (start)\n");

	kfree(local_transfer_buffer);
	return ret;
}

static int usbduxfastsub_stop(struct usbduxfastsub_s *udfs)
{
	int ret;
	unsigned char *local_transfer_buffer;

	local_transfer_buffer = kmalloc(1, GFP_KERNEL);
	if (!local_transfer_buffer)
		return -ENOMEM;

	/* 7f92 to one */
	*local_transfer_buffer = 1;
	/* bRequest, "Firmware" */
	ret = usb_control_msg(udfs->usbdev, usb_sndctrlpipe(udfs->usbdev, 0),
			      USBDUXFASTSUB_FIRMWARE,
			      VENDOR_DIR_OUT,	/* bmRequestType */
			      USBDUXFASTSUB_CPUCS,	/* Value */
			      0x0000,	/* Index */
			      local_transfer_buffer, 1,	/* Length */
			      EZTIMEOUT);	/* Timeout */
	if (ret < 0)
		dev_err(&udfs->interface->dev,
			"control msg failed (stop)\n");

	kfree(local_transfer_buffer);
	return ret;
}

static int usbduxfastsub_upload(struct usbduxfastsub_s *udfs,
				unsigned char *local_transfer_buffer,
				unsigned int startAddr, unsigned int len)
{
	int ret;

#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi: usbduxfast: uploading %d bytes", len);
	printk(KERN_DEBUG " to addr %d, first byte=%d.\n",
	       startAddr, local_transfer_buffer[0]);
#endif
	/* brequest, firmware */
	ret = usb_control_msg(udfs->usbdev, usb_sndctrlpipe(udfs->usbdev, 0),
			      USBDUXFASTSUB_FIRMWARE,
			      VENDOR_DIR_OUT,	/* bmRequestType */
			      startAddr,	/* value */
			      0x0000,	 /* index */
			      /* our local safe buffer */
			      local_transfer_buffer,
			      len,	/* length */
			      EZTIMEOUT);      /* timeout */

#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi_: usbduxfast: result=%d\n", ret);
#endif

	if (ret < 0) {
		dev_err(&udfs->interface->dev, "uppload failed\n");
		return ret;
	}

	return 0;
}

static int usbduxfastsub_submit_InURBs(struct usbduxfastsub_s *udfs)
{
	int ret;

	if (!udfs)
		return -EFAULT;

	usb_fill_bulk_urb(udfs->urbIn, udfs->usbdev,
			  usb_rcvbulkpipe(udfs->usbdev, BULKINEP),
			  udfs->transfer_buffer,
			  SIZEINBUF, usbduxfastsub_ai_Irq, udfs->comedidev);

#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi%d: usbduxfast: submitting in-urb: "
	       "0x%p,0x%p\n", udfs->comedidev->minor, udfs->urbIn->context,
	       udfs->urbIn->dev);
#endif
	ret = usb_submit_urb(udfs->urbIn, GFP_ATOMIC);
	if (ret) {
		dev_err(&udfs->interface->dev,
			"ai: usb_submit_urb error %d\n", ret);
		return ret;
=======
		/* after an unlink command, unplug, ... etc */
		async->events |= COMEDI_CB_ERROR;
		break;

	default:
		/* a real error */
		dev_err(dev->class_dev,
			"non-zero urb status received in ai intr context: %d\n",
			urb->status);
		async->events |= COMEDI_CB_ERROR;
		break;
	}

	/*
	 * comedi_handle_events() cannot be used in this driver. The (*cancel)
	 * operation would unlink the urb.
	 */
	if (async->events & COMEDI_CB_CANCEL_MASK)
		usbduxfast_ai_stop(dev, 0);

	comedi_event(dev, s);
}

static int usbduxfast_submit_urb(struct comedi_device *dev)
{
	struct usb_device *usb = comedi_to_usb_dev(dev);
	struct usbduxfast_private *devpriv = dev->private;
	int ret;

	usb_fill_bulk_urb(devpriv->urb, usb, usb_rcvbulkpipe(usb, BULKINEP),
			  devpriv->inbuf, SIZEINBUF,
			  usbduxfast_ai_interrupt, dev);

	ret = usb_submit_urb(devpriv->urb, GFP_ATOMIC);
	if (ret) {
		dev_err(dev->class_dev, "usb_submit_urb error %d\n", ret);
		return ret;
	}
	return 0;
}

static int usbduxfast_ai_check_chanlist(struct comedi_device *dev,
					struct comedi_subdevice *s,
					struct comedi_cmd *cmd)
{
	unsigned int gain0 = CR_RANGE(cmd->chanlist[0]);
	int i;

	if (cmd->chanlist_len > 3 && cmd->chanlist_len != 16) {
		dev_err(dev->class_dev, "unsupported combination of channels\n");
		return -EINVAL;
	}

	for (i = 0; i < cmd->chanlist_len; ++i) {
		unsigned int chan = CR_CHAN(cmd->chanlist[i]);
		unsigned int gain = CR_RANGE(cmd->chanlist[i]);

		if (chan != i) {
			dev_err(dev->class_dev,
				"channels are not consecutive\n");
			return -EINVAL;
		}
		if (gain != gain0 && cmd->chanlist_len > 3) {
			dev_err(dev->class_dev,
				"gain must be the same for all channels\n");
			return -EINVAL;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return 0;
}

static int usbduxfast_ai_cmdtest(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 struct comedi_cmd *cmd)
{
<<<<<<< HEAD
	struct usbduxfastsub_s *udfs = dev->private;
	int err = 0;
	long int steps, tmp;
	int minSamplPer;

	if (!udfs->probed)
		return -ENODEV;

	/* Step 1 : check if triggers are trivially valid */

	err |= cfc_check_trigger_src(&cmd->start_src,
					TRIG_NOW | TRIG_EXT | TRIG_INT);
	err |= cfc_check_trigger_src(&cmd->scan_begin_src,
					TRIG_TIMER | TRIG_FOLLOW | TRIG_EXT);
	err |= cfc_check_trigger_src(&cmd->convert_src, TRIG_TIMER | TRIG_EXT);
	err |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= cfc_check_trigger_src(&cmd->stop_src, TRIG_COUNT | TRIG_NONE);
=======
	int err = 0;
	int err2 = 0;
	unsigned int steps;
	unsigned int arg;

	/* Step 1 : check if triggers are trivially valid */

	err |= comedi_check_trigger_src(&cmd->start_src,
					TRIG_NOW | TRIG_EXT | TRIG_INT);
	err |= comedi_check_trigger_src(&cmd->scan_begin_src, TRIG_FOLLOW);
	err |= comedi_check_trigger_src(&cmd->convert_src, TRIG_TIMER);
	err |= comedi_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= comedi_check_trigger_src(&cmd->stop_src, TRIG_COUNT | TRIG_NONE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (err)
		return 1;

	/* Step 2a : make sure trigger sources are unique */

<<<<<<< HEAD
	err |= cfc_check_trigger_is_unique(cmd->start_src);
	err |= cfc_check_trigger_is_unique(cmd->scan_begin_src);
	err |= cfc_check_trigger_is_unique(cmd->convert_src);
	err |= cfc_check_trigger_is_unique(cmd->stop_src);

	/* Step 2b : and mutually compatible */

	/* can't have external stop and start triggers at once */
	if (cmd->start_src == TRIG_EXT && cmd->stop_src == TRIG_EXT)
		err |= -EINVAL;

=======
	err |= comedi_check_trigger_is_unique(cmd->start_src);
	err |= comedi_check_trigger_is_unique(cmd->stop_src);

	/* Step 2b : and mutually compatible */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err)
		return 2;

	/* Step 3: check if arguments are trivially valid */

<<<<<<< HEAD
	if (cmd->start_src == TRIG_NOW)
		err |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);
=======
	err |= comedi_check_trigger_arg_is(&cmd->start_arg, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!cmd->chanlist_len)
		err |= -EINVAL;

<<<<<<< HEAD
	err |= cfc_check_trigger_arg_is(&cmd->scan_end_arg, cmd->chanlist_len);

	if (cmd->chanlist_len == 1)
		minSamplPer = 1;
	else
		minSamplPer = MIN_SAMPLING_PERIOD;

	if (cmd->convert_src == TRIG_TIMER) {
		steps = cmd->convert_arg * 30;
		if (steps < (minSamplPer * 1000))
			steps = minSamplPer * 1000;

		if (steps > (MAX_SAMPLING_PERIOD * 1000))
			steps = MAX_SAMPLING_PERIOD * 1000;

		/* calc arg again */
		tmp = steps / 30;
		err |= cfc_check_trigger_arg_is(&cmd->convert_arg, tmp);
	}

	if (cmd->scan_begin_src == TRIG_TIMER)
		err |= -EINVAL;

	/* stop source */
	switch (cmd->stop_src) {
	case TRIG_COUNT:
		err |= cfc_check_trigger_arg_min(&cmd->stop_arg, 1);
		break;
	case TRIG_NONE:
		err |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);
		break;
		/*
		 * TRIG_EXT doesn't care since it doesn't trigger
		 * off a numbered channel
		 */
	default:
		break;
	}
=======
	/* external start trigger is only valid for 1 or 16 channels */
	if (cmd->start_src == TRIG_EXT &&
	    cmd->chanlist_len != 1 && cmd->chanlist_len != 16)
		err |= -EINVAL;

	err |= comedi_check_trigger_arg_is(&cmd->scan_end_arg,
					   cmd->chanlist_len);

	/*
	 * Validate the conversion timing:
	 * for 1 channel the timing in 30MHz "steps" is:
	 *	steps <= MAX_SAMPLING_PERIOD
	 * for all other chanlist_len it is:
	 *	MIN_SAMPLING_PERIOD <= steps <= MAX_SAMPLING_PERIOD
	 */
	steps = (cmd->convert_arg * 30) / 1000;
	if (cmd->chanlist_len !=  1)
		err2 |= comedi_check_trigger_arg_min(&steps,
						     MIN_SAMPLING_PERIOD);
	else
		err2 |= comedi_check_trigger_arg_min(&steps, 1);
	err2 |= comedi_check_trigger_arg_max(&steps, MAX_SAMPLING_PERIOD);
	if (err2) {
		err |= err2;
		arg = (steps * 1000) / 30;
		err |= comedi_check_trigger_arg_is(&cmd->convert_arg, arg);
	}

	if (cmd->stop_src == TRIG_COUNT)
		err |= comedi_check_trigger_arg_min(&cmd->stop_arg, 1);
	else	/* TRIG_NONE */
		err |= comedi_check_trigger_arg_is(&cmd->stop_arg, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (err)
		return 3;

<<<<<<< HEAD
	/* step 4: fix up any arguments */

	return 0;

=======
	/* Step 4: fix up any arguments */

	/* Step 5: check channel list if it exists */
	if (cmd->chanlist && cmd->chanlist_len > 0)
		err |= usbduxfast_ai_check_chanlist(dev, s, cmd);
	if (err)
		return 5;

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int usbduxfast_ai_inttrig(struct comedi_device *dev,
				 struct comedi_subdevice *s,
<<<<<<< HEAD
				 unsigned int trignum)
{
	int ret;
	struct usbduxfastsub_s *udfs = dev->private;

	if (!udfs)
		return -EFAULT;

	down(&udfs->sem);
	if (!udfs->probed) {
		up(&udfs->sem);
		return -ENODEV;
	}
#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi%d: usbduxfast_ai_inttrig\n", dev->minor);
#endif

	if (trignum != 0) {
		dev_err(dev->class_dev, "%s: invalid trignum\n", __func__);
		up(&udfs->sem);
		return -EINVAL;
	}
	if (!udfs->ai_cmd_running) {
		udfs->ai_cmd_running = 1;
		ret = usbduxfastsub_submit_InURBs(udfs);
		if (ret < 0) {
			dev_err(dev->class_dev,
				"%s: urbSubmit: err=%d\n", __func__, ret);
			udfs->ai_cmd_running = 0;
			up(&udfs->sem);
=======
				 unsigned int trig_num)
{
	struct usbduxfast_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	int ret;

	if (trig_num != cmd->start_arg)
		return -EINVAL;

	mutex_lock(&devpriv->mut);

	if (!devpriv->ai_cmd_running) {
		devpriv->ai_cmd_running = 1;
		ret = usbduxfast_submit_urb(dev);
		if (ret < 0) {
			dev_err(dev->class_dev, "urbSubmit: err=%d\n", ret);
			devpriv->ai_cmd_running = 0;
			mutex_unlock(&devpriv->mut);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return ret;
		}
		s->async->inttrig = NULL;
	} else {
<<<<<<< HEAD
		dev_err(dev->class_dev,
			"ai_inttrig but acqu is already running\n");
	}
	up(&udfs->sem);
	return 1;
}

/*
 * offsets for the GPIF bytes
 * the first byte is the command byte
 */
#define LENBASE	(1+0x00)
#define OPBASE	(1+0x08)
#define OUTBASE	(1+0x10)
#define LOGBASE	(1+0x18)

static int usbduxfast_ai_cmd(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	struct comedi_cmd *cmd = &s->async->cmd;
	unsigned int chan, gain, rngmask = 0xff;
	int i, j, ret;
	struct usbduxfastsub_s *udfs;
	int result;
	long steps, steps_tmp;

#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi%d: usbduxfast_ai_cmd\n", dev->minor);
#endif
	udfs = dev->private;
	if (!udfs)
		return -EFAULT;

	down(&udfs->sem);
	if (!udfs->probed) {
		up(&udfs->sem);
		return -ENODEV;
	}
	if (udfs->ai_cmd_running) {
		dev_err(dev->class_dev,
			"ai_cmd not possible. Another ai_cmd is running.\n");
		up(&udfs->sem);
		return -EBUSY;
	}
	/* set current channel of the running acquisition to zero */
	s->async->cur_chan = 0;
=======
		dev_err(dev->class_dev, "ai is already running\n");
	}
	mutex_unlock(&devpriv->mut);
	return 1;
}

static int usbduxfast_ai_cmd(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	struct usbduxfast_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	unsigned int rngmask = 0xff;
	int j, ret;
	long steps, steps_tmp;

	mutex_lock(&devpriv->mut);
	if (devpriv->ai_cmd_running) {
		ret = -EBUSY;
		goto cmd_exit;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * ignore the first buffers from the device if there
	 * is an error condition
	 */
<<<<<<< HEAD
	udfs->ignore = PACKETS_TO_IGNORE;

	if (cmd->chanlist_len > 0) {
		gain = CR_RANGE(cmd->chanlist[0]);
		for (i = 0; i < cmd->chanlist_len; ++i) {
			chan = CR_CHAN(cmd->chanlist[i]);
			if (chan != i) {
				dev_err(dev->class_dev,
					"cmd is accepting only consecutive channels.\n");
				up(&udfs->sem);
				return -EINVAL;
			}
			if ((gain != CR_RANGE(cmd->chanlist[i]))
			    && (cmd->chanlist_len > 3)) {
				dev_err(dev->class_dev,
					"the gain must be the same for all channels.\n");
				up(&udfs->sem);
				return -EINVAL;
			}
			if (i >= NUMCHANNELS) {
				dev_err(dev->class_dev,
					"channel list too long\n");
				break;
			}
		}
	}
	steps = 0;
	if (cmd->scan_begin_src == TRIG_TIMER) {
		dev_err(dev->class_dev,
			"scan_begin_src==TRIG_TIMER not valid.\n");
		up(&udfs->sem);
		return -EINVAL;
	}
	if (cmd->convert_src == TRIG_TIMER)
		steps = (cmd->convert_arg * 30) / 1000;

	if ((steps < MIN_SAMPLING_PERIOD) && (cmd->chanlist_len != 1)) {
		dev_err(dev->class_dev,
			"ai_cmd: steps=%ld, scan_begin_arg=%d. Not properly tested by cmdtest?\n",
			steps, cmd->scan_begin_arg);
		up(&udfs->sem);
		return -EINVAL;
	}
	if (steps > MAX_SAMPLING_PERIOD) {
		dev_err(dev->class_dev, "ai_cmd: sampling rate too low.\n");
		up(&udfs->sem);
		return -EINVAL;
	}
	if ((cmd->start_src == TRIG_EXT) && (cmd->chanlist_len != 1)
	    && (cmd->chanlist_len != 16)) {
		dev_err(dev->class_dev,
			"ai_cmd: TRIG_EXT only with 1 or 16 channels possible.\n");
		up(&udfs->sem);
		return -EINVAL;
	}
#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi%d: usbduxfast: steps=%ld, convert_arg=%u\n",
	       dev->minor, steps, cmd->convert_arg);
#endif
=======
	devpriv->ignore = PACKETS_TO_IGNORE;

	steps = (cmd->convert_arg * 30) / 1000;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	switch (cmd->chanlist_len) {
	case 1:
		/*
		 * one channel
		 */

		if (CR_RANGE(cmd->chanlist[0]) > 0)
			rngmask = 0xff - 0x04;
		else
			rngmask = 0xff;

		/*
		 * for external trigger: looping in this state until
		 * the RDY0 pin becomes zero
		 */

		/* we loop here until ready has been set */
		if (cmd->start_src == TRIG_EXT) {
			/* branch back to state 0 */
<<<<<<< HEAD
			udfs->dux_commands[LENBASE + 0] = 0x01;
			/* deceision state w/o data */
			udfs->dux_commands[OPBASE + 0] = 0x01;
			udfs->dux_commands[OUTBASE + 0] = 0xFF & rngmask;
			/* RDY0 = 0 */
			udfs->dux_commands[LOGBASE + 0] = 0x00;
		} else {	/* we just proceed to state 1 */
			udfs->dux_commands[LENBASE + 0] = 1;
			udfs->dux_commands[OPBASE + 0] = 0;
			udfs->dux_commands[OUTBASE + 0] = 0xFF & rngmask;
			udfs->dux_commands[LOGBASE + 0] = 0;
=======
			/* deceision state w/o data */
			/* RDY0 = 0 */
			usbduxfast_cmd_data(dev, 0, 0x01, 0x01, rngmask, 0x00);
		} else {	/* we just proceed to state 1 */
			usbduxfast_cmd_data(dev, 0, 0x01, 0x00, rngmask, 0x00);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		if (steps < MIN_SAMPLING_PERIOD) {
			/* for fast single channel aqu without mux */
			if (steps <= 1) {
				/*
				 * we just stay here at state 1 and rexecute
				 * the same state this gives us 30MHz sampling
				 * rate
				 */

				/* branch back to state 1 */
<<<<<<< HEAD
				udfs->dux_commands[LENBASE + 1] = 0x89;
				/* deceision state with data */
				udfs->dux_commands[OPBASE + 1] = 0x03;
				udfs->dux_commands[OUTBASE + 1] =
				    0xFF & rngmask;
				/* doesn't matter */
				udfs->dux_commands[LOGBASE + 1] = 0xFF;
=======
				/* deceision state with data */
				/* doesn't matter */
				usbduxfast_cmd_data(dev, 1,
						    0x89, 0x03, rngmask, 0xff);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			} else {
				/*
				 * we loop through two states: data and delay
				 * max rate is 15MHz
				 */
<<<<<<< HEAD
				udfs->dux_commands[LENBASE + 1] = steps - 1;
				/* data */
				udfs->dux_commands[OPBASE + 1] = 0x02;
				udfs->dux_commands[OUTBASE + 1] =
				    0xFF & rngmask;
				/* doesn't matter */
				udfs->dux_commands[LOGBASE + 1] = 0;
				/* branch back to state 1 */
				udfs->dux_commands[LENBASE + 2] = 0x09;
				/* deceision state w/o data */
				udfs->dux_commands[OPBASE + 2] = 0x01;
				udfs->dux_commands[OUTBASE + 2] =
				    0xFF & rngmask;
				/* doesn't matter */
				udfs->dux_commands[LOGBASE + 2] = 0xFF;
=======
				/* data */
				/* doesn't matter */
				usbduxfast_cmd_data(dev, 1, steps - 1,
						    0x02, rngmask, 0x00);

				/* branch back to state 1 */
				/* deceision state w/o data */
				/* doesn't matter */
				usbduxfast_cmd_data(dev, 2,
						    0x09, 0x01, rngmask, 0xff);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			}
		} else {
			/*
			 * we loop through 3 states: 2x delay and 1x data
			 * this gives a min sampling rate of 60kHz
			 */

			/* we have 1 state with duration 1 */
			steps = steps - 1;

			/* do the first part of the delay */
<<<<<<< HEAD
			udfs->dux_commands[LENBASE + 1] = steps / 2;
			udfs->dux_commands[OPBASE + 1] = 0;
			udfs->dux_commands[OUTBASE + 1] = 0xFF & rngmask;
			udfs->dux_commands[LOGBASE + 1] = 0;

			/* and the second part */
			udfs->dux_commands[LENBASE + 2] = steps - steps / 2;
			udfs->dux_commands[OPBASE + 2] = 0;
			udfs->dux_commands[OUTBASE + 2] = 0xFF & rngmask;
			udfs->dux_commands[LOGBASE + 2] = 0;
=======
			usbduxfast_cmd_data(dev, 1,
					    steps / 2, 0x00, rngmask, 0x00);

			/* and the second part */
			usbduxfast_cmd_data(dev, 2, steps - steps / 2,
					    0x00, rngmask, 0x00);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			/* get the data and branch back */

			/* branch back to state 1 */
<<<<<<< HEAD
			udfs->dux_commands[LENBASE + 3] = 0x09;
			/* deceision state w data */
			udfs->dux_commands[OPBASE + 3] = 0x03;
			udfs->dux_commands[OUTBASE + 3] = 0xFF & rngmask;
			/* doesn't matter */
			udfs->dux_commands[LOGBASE + 3] = 0xFF;
=======
			/* deceision state w data */
			/* doesn't matter */
			usbduxfast_cmd_data(dev, 3,
					    0x09, 0x03, rngmask, 0xff);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		break;

	case 2:
		/*
		 * two channels
		 * commit data to the FIFO
		 */

		if (CR_RANGE(cmd->chanlist[0]) > 0)
			rngmask = 0xff - 0x04;
		else
			rngmask = 0xff;

<<<<<<< HEAD
		udfs->dux_commands[LENBASE + 0] = 1;
		/* data */
		udfs->dux_commands[OPBASE + 0] = 0x02;
		udfs->dux_commands[OUTBASE + 0] = 0xFF & rngmask;
		udfs->dux_commands[LOGBASE + 0] = 0;
=======
		/* data */
		usbduxfast_cmd_data(dev, 0, 0x01, 0x02, rngmask, 0x00);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* we have 1 state with duration 1: state 0 */
		steps_tmp = steps - 1;

		if (CR_RANGE(cmd->chanlist[1]) > 0)
			rngmask = 0xff - 0x04;
		else
			rngmask = 0xff;

		/* do the first part of the delay */
<<<<<<< HEAD
		udfs->dux_commands[LENBASE + 1] = steps_tmp / 2;
		udfs->dux_commands[OPBASE + 1] = 0;
		/* count */
		udfs->dux_commands[OUTBASE + 1] = 0xFE & rngmask;
		udfs->dux_commands[LOGBASE + 1] = 0;

		/* and the second part */
		udfs->dux_commands[LENBASE + 2] = steps_tmp - steps_tmp / 2;
		udfs->dux_commands[OPBASE + 2] = 0;
		udfs->dux_commands[OUTBASE + 2] = 0xFF & rngmask;
		udfs->dux_commands[LOGBASE + 2] = 0;

		udfs->dux_commands[LENBASE + 3] = 1;
		/* data */
		udfs->dux_commands[OPBASE + 3] = 0x02;
		udfs->dux_commands[OUTBASE + 3] = 0xFF & rngmask;
		udfs->dux_commands[LOGBASE + 3] = 0;
=======
		/* count */
		usbduxfast_cmd_data(dev, 1, steps_tmp / 2,
				    0x00, 0xfe & rngmask, 0x00);

		/* and the second part */
		usbduxfast_cmd_data(dev, 2, steps_tmp  - steps_tmp / 2,
				    0x00, rngmask, 0x00);

		/* data */
		usbduxfast_cmd_data(dev, 3, 0x01, 0x02, rngmask, 0x00);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/*
		 * we have 2 states with duration 1: step 6 and
		 * the IDLE state
		 */
		steps_tmp = steps - 2;

		if (CR_RANGE(cmd->chanlist[0]) > 0)
			rngmask = 0xff - 0x04;
		else
			rngmask = 0xff;

		/* do the first part of the delay */
<<<<<<< HEAD
		udfs->dux_commands[LENBASE + 4] = steps_tmp / 2;
		udfs->dux_commands[OPBASE + 4] = 0;
		/* reset */
		udfs->dux_commands[OUTBASE + 4] = (0xFF - 0x02) & rngmask;
		udfs->dux_commands[LOGBASE + 4] = 0;

		/* and the second part */
		udfs->dux_commands[LENBASE + 5] = steps_tmp - steps_tmp / 2;
		udfs->dux_commands[OPBASE + 5] = 0;
		udfs->dux_commands[OUTBASE + 5] = 0xFF & rngmask;
		udfs->dux_commands[LOGBASE + 5] = 0;

		udfs->dux_commands[LENBASE + 6] = 1;
		udfs->dux_commands[OPBASE + 6] = 0;
		udfs->dux_commands[OUTBASE + 6] = 0xFF & rngmask;
		udfs->dux_commands[LOGBASE + 6] = 0;
=======
		/* reset */
		usbduxfast_cmd_data(dev, 4, steps_tmp / 2,
				    0x00, (0xff - 0x02) & rngmask, 0x00);

		/* and the second part */
		usbduxfast_cmd_data(dev, 5, steps_tmp - steps_tmp / 2,
				    0x00, rngmask, 0x00);

		usbduxfast_cmd_data(dev, 6, 0x01, 0x00, rngmask, 0x00);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;

	case 3:
		/*
		 * three channels
		 */
		for (j = 0; j < 1; j++) {
<<<<<<< HEAD
=======
			int index = j * 2;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (CR_RANGE(cmd->chanlist[j]) > 0)
				rngmask = 0xff - 0x04;
			else
				rngmask = 0xff;
			/*
			 * commit data to the FIFO and do the first part
			 * of the delay
			 */
<<<<<<< HEAD
			udfs->dux_commands[LENBASE + j * 2] = steps / 2;
			/* data */
			udfs->dux_commands[OPBASE + j * 2] = 0x02;
			/* no change */
			udfs->dux_commands[OUTBASE + j * 2] = 0xFF & rngmask;
			udfs->dux_commands[LOGBASE + j * 2] = 0;
=======
			/* data */
			/* no change */
			usbduxfast_cmd_data(dev, index, steps / 2,
					    0x02, rngmask, 0x00);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			if (CR_RANGE(cmd->chanlist[j + 1]) > 0)
				rngmask = 0xff - 0x04;
			else
				rngmask = 0xff;

			/* do the second part of the delay */
<<<<<<< HEAD
			udfs->dux_commands[LENBASE + j * 2 + 1] =
			    steps - steps / 2;
			/* no data */
			udfs->dux_commands[OPBASE + j * 2 + 1] = 0;
			/* count */
			udfs->dux_commands[OUTBASE + j * 2 + 1] =
			    0xFE & rngmask;
			udfs->dux_commands[LOGBASE + j * 2 + 1] = 0;
=======
			/* no data */
			/* count */
			usbduxfast_cmd_data(dev, index + 1, steps - steps / 2,
					    0x00, 0xfe & rngmask, 0x00);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/* 2 steps with duration 1: the idele step and step 6: */
		steps_tmp = steps - 2;

		/* commit data to the FIFO and do the first part of the delay */
<<<<<<< HEAD
		udfs->dux_commands[LENBASE + 4] = steps_tmp / 2;
		/* data */
		udfs->dux_commands[OPBASE + 4] = 0x02;
		udfs->dux_commands[OUTBASE + 4] = 0xFF & rngmask;
		udfs->dux_commands[LOGBASE + 4] = 0;
=======
		/* data */
		usbduxfast_cmd_data(dev, 4, steps_tmp / 2,
				    0x02, rngmask, 0x00);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (CR_RANGE(cmd->chanlist[0]) > 0)
			rngmask = 0xff - 0x04;
		else
			rngmask = 0xff;

		/* do the second part of the delay */
<<<<<<< HEAD
		udfs->dux_commands[LENBASE + 5] = steps_tmp - steps_tmp / 2;
		/* no data */
		udfs->dux_commands[OPBASE + 5] = 0;
		/* reset */
		udfs->dux_commands[OUTBASE + 5] = (0xFF - 0x02) & rngmask;
		udfs->dux_commands[LOGBASE + 5] = 0;

		udfs->dux_commands[LENBASE + 6] = 1;
		udfs->dux_commands[OPBASE + 6] = 0;
		udfs->dux_commands[OUTBASE + 6] = 0xFF & rngmask;
		udfs->dux_commands[LOGBASE + 6] = 0;
=======
		/* no data */
		/* reset */
		usbduxfast_cmd_data(dev, 5, steps_tmp - steps_tmp / 2,
				    0x00, (0xff - 0x02) & rngmask, 0x00);

		usbduxfast_cmd_data(dev, 6, 0x01, 0x00, rngmask, 0x00);
		break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	case 16:
		if (CR_RANGE(cmd->chanlist[0]) > 0)
			rngmask = 0xff - 0x04;
		else
			rngmask = 0xff;

		if (cmd->start_src == TRIG_EXT) {
			/*
			 * we loop here until ready has been set
			 */

			/* branch back to state 0 */
<<<<<<< HEAD
			udfs->dux_commands[LENBASE + 0] = 0x01;
			/* deceision state w/o data */
			udfs->dux_commands[OPBASE + 0] = 0x01;
			/* reset */
			udfs->dux_commands[OUTBASE + 0] =
			    (0xFF - 0x02) & rngmask;
			/* RDY0 = 0 */
			udfs->dux_commands[LOGBASE + 0] = 0x00;
=======
			/* deceision state w/o data */
			/* reset */
			/* RDY0 = 0 */
			usbduxfast_cmd_data(dev, 0, 0x01, 0x01,
					    (0xff - 0x02) & rngmask, 0x00);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		} else {
			/*
			 * we just proceed to state 1
			 */

			/* 30us reset pulse */
<<<<<<< HEAD
			udfs->dux_commands[LENBASE + 0] = 255;
			udfs->dux_commands[OPBASE + 0] = 0;
			/* reset */
			udfs->dux_commands[OUTBASE + 0] =
			    (0xFF - 0x02) & rngmask;
			udfs->dux_commands[LOGBASE + 0] = 0;
		}

		/* commit data to the FIFO */
		udfs->dux_commands[LENBASE + 1] = 1;
		/* data */
		udfs->dux_commands[OPBASE + 1] = 0x02;
		udfs->dux_commands[OUTBASE + 1] = 0xFF & rngmask;
		udfs->dux_commands[LOGBASE + 1] = 0;
=======
			/* reset */
			usbduxfast_cmd_data(dev, 0, 0xff, 0x00,
					    (0xff - 0x02) & rngmask, 0x00);
		}

		/* commit data to the FIFO */
		/* data */
		usbduxfast_cmd_data(dev, 1, 0x01, 0x02, rngmask, 0x00);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* we have 2 states with duration 1 */
		steps = steps - 2;

		/* do the first part of the delay */
<<<<<<< HEAD
		udfs->dux_commands[LENBASE + 2] = steps / 2;
		udfs->dux_commands[OPBASE + 2] = 0;
		udfs->dux_commands[OUTBASE + 2] = 0xFE & rngmask;
		udfs->dux_commands[LOGBASE + 2] = 0;

		/* and the second part */
		udfs->dux_commands[LENBASE + 3] = steps - steps / 2;
		udfs->dux_commands[OPBASE + 3] = 0;
		udfs->dux_commands[OUTBASE + 3] = 0xFF & rngmask;
		udfs->dux_commands[LOGBASE + 3] = 0;

		/* branch back to state 1 */
		udfs->dux_commands[LENBASE + 4] = 0x09;
		/* deceision state w/o data */
		udfs->dux_commands[OPBASE + 4] = 0x01;
		udfs->dux_commands[OUTBASE + 4] = 0xFF & rngmask;
		/* doesn't matter */
		udfs->dux_commands[LOGBASE + 4] = 0xFF;

		break;

	default:
		dev_err(dev->class_dev, "unsupported combination of channels\n");
		up(&udfs->sem);
		return -EFAULT;
	}

#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi %d: sending commands to the usb device\n",
	       dev->minor);
#endif
	/* 0 means that the AD commands are sent */
	result = send_dux_commands(udfs, SENDADCOMMANDS);
	if (result < 0) {
		dev_err(dev->class_dev,
			"adc command could not be submitted. Aborting...\n");
		up(&udfs->sem);
		return result;
	}
	if (cmd->stop_src == TRIG_COUNT) {
		udfs->ai_sample_count = cmd->stop_arg * cmd->scan_end_arg;
		if (udfs->ai_sample_count < 1) {
			dev_err(dev->class_dev,
				"(cmd->stop_arg)*(cmd->scan_end_arg)<1, aborting.\n");
			up(&udfs->sem);
			return -EFAULT;
		}
		udfs->ai_continous = 0;
	} else {
		/* continous acquisition */
		udfs->ai_continous = 1;
		udfs->ai_sample_count = 0;
	}

	if ((cmd->start_src == TRIG_NOW) || (cmd->start_src == TRIG_EXT)) {
		/* enable this acquisition operation */
		udfs->ai_cmd_running = 1;
		ret = usbduxfastsub_submit_InURBs(udfs);
		if (ret < 0) {
			udfs->ai_cmd_running = 0;
			/* fixme: unlink here?? */
			up(&udfs->sem);
			return ret;
		}
		s->async->inttrig = NULL;
	} else {
		/*
		 * TRIG_INT
		 * don't enable the acquision operation
		 * wait for an internal signal
		 */
		s->async->inttrig = usbduxfast_ai_inttrig;
	}
	up(&udfs->sem);

	return 0;
=======
		usbduxfast_cmd_data(dev, 2, steps / 2,
				    0x00, 0xfe & rngmask, 0x00);

		/* and the second part */
		usbduxfast_cmd_data(dev, 3, steps - steps / 2,
				    0x00, rngmask, 0x00);

		/* branch back to state 1 */
		/* deceision state w/o data */
		/* doesn't matter */
		usbduxfast_cmd_data(dev, 4, 0x09, 0x01, rngmask, 0xff);

		break;
	}

	/* 0 means that the AD commands are sent */
	ret = usbduxfast_send_cmd(dev, SENDADCOMMANDS);
	if (ret < 0)
		goto cmd_exit;

	if ((cmd->start_src == TRIG_NOW) || (cmd->start_src == TRIG_EXT)) {
		/* enable this acquisition operation */
		devpriv->ai_cmd_running = 1;
		ret = usbduxfast_submit_urb(dev);
		if (ret < 0) {
			devpriv->ai_cmd_running = 0;
			/* fixme: unlink here?? */
			goto cmd_exit;
		}
		s->async->inttrig = NULL;
	} else {	/* TRIG_INT */
		s->async->inttrig = usbduxfast_ai_inttrig;
	}

cmd_exit:
	mutex_unlock(&devpriv->mut);

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Mode 0 is used to get a single conversion on demand.
 */
static int usbduxfast_ai_insn_read(struct comedi_device *dev,
				   struct comedi_subdevice *s,
<<<<<<< HEAD
				   struct comedi_insn *insn, unsigned int *data)
{
	int i, j, n, actual_length;
	int chan, range, rngmask;
	int err;
	struct usbduxfastsub_s *udfs;

	udfs = dev->private;
	if (!udfs) {
		dev_err(dev->class_dev, "%s: no usb dev.\n", __func__);
		return -ENODEV;
	}
#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi%d: ai_insn_read, insn->n=%d, "
	       "insn->subdev=%d\n", dev->minor, insn->n, insn->subdev);
#endif
	down(&udfs->sem);
	if (!udfs->probed) {
		up(&udfs->sem);
		return -ENODEV;
	}
	if (udfs->ai_cmd_running) {
		dev_err(dev->class_dev,
			"ai_insn_read not possible. Async Command is running.\n");
		up(&udfs->sem);
		return -EBUSY;
	}
	/* sample one channel */
	chan = CR_CHAN(insn->chanspec);
	range = CR_RANGE(insn->chanspec);
	/* set command for the first channel */

	if (range > 0)
		rngmask = 0xff - 0x04;
	else
		rngmask = 0xff;

	/* commit data to the FIFO */
	udfs->dux_commands[LENBASE + 0] = 1;
	/* data */
	udfs->dux_commands[OPBASE + 0] = 0x02;
	udfs->dux_commands[OUTBASE + 0] = 0xFF & rngmask;
	udfs->dux_commands[LOGBASE + 0] = 0;

	/* do the first part of the delay */
	udfs->dux_commands[LENBASE + 1] = 12;
	udfs->dux_commands[OPBASE + 1] = 0;
	udfs->dux_commands[OUTBASE + 1] = 0xFE & rngmask;
	udfs->dux_commands[LOGBASE + 1] = 0;

	udfs->dux_commands[LENBASE + 2] = 1;
	udfs->dux_commands[OPBASE + 2] = 0;
	udfs->dux_commands[OUTBASE + 2] = 0xFE & rngmask;
	udfs->dux_commands[LOGBASE + 2] = 0;

	udfs->dux_commands[LENBASE + 3] = 1;
	udfs->dux_commands[OPBASE + 3] = 0;
	udfs->dux_commands[OUTBASE + 3] = 0xFE & rngmask;
	udfs->dux_commands[LOGBASE + 3] = 0;

	udfs->dux_commands[LENBASE + 4] = 1;
	udfs->dux_commands[OPBASE + 4] = 0;
	udfs->dux_commands[OUTBASE + 4] = 0xFE & rngmask;
	udfs->dux_commands[LOGBASE + 4] = 0;

	/* second part */
	udfs->dux_commands[LENBASE + 5] = 12;
	udfs->dux_commands[OPBASE + 5] = 0;
	udfs->dux_commands[OUTBASE + 5] = 0xFF & rngmask;
	udfs->dux_commands[LOGBASE + 5] = 0;

	udfs->dux_commands[LENBASE + 6] = 1;
	udfs->dux_commands[OPBASE + 6] = 0;
	udfs->dux_commands[OUTBASE + 6] = 0xFF & rngmask;
	udfs->dux_commands[LOGBASE + 0] = 0;

#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi %d: sending commands to the usb device\n",
	       dev->minor);
#endif
	/* 0 means that the AD commands are sent */
	err = send_dux_commands(udfs, SENDADCOMMANDS);
	if (err < 0) {
		dev_err(dev->class_dev,
			"adc command could not be submitted. Aborting...\n");
		up(&udfs->sem);
		return err;
	}
#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi%d: usbduxfast: submitting in-urb: "
	       "0x%p,0x%p\n", udfs->comedidev->minor, udfs->urbIn->context,
	       udfs->urbIn->dev);
#endif
	for (i = 0; i < PACKETS_TO_IGNORE; i++) {
		err = usb_bulk_msg(udfs->usbdev,
				   usb_rcvbulkpipe(udfs->usbdev, BULKINEP),
				   udfs->transfer_buffer, SIZEINBUF,
				   &actual_length, 10000);
		if (err < 0) {
			dev_err(dev->class_dev, "insn timeout. No data.\n");
			up(&udfs->sem);
			return err;
		}
	}
	/* data points */
	for (i = 0; i < insn->n;) {
		err = usb_bulk_msg(udfs->usbdev,
				   usb_rcvbulkpipe(udfs->usbdev, BULKINEP),
				   udfs->transfer_buffer, SIZEINBUF,
				   &actual_length, 10000);
		if (err < 0) {
			dev_err(dev->class_dev, "insn data error: %d\n", err);
			up(&udfs->sem);
			return err;
		}
		n = actual_length / sizeof(uint16_t);
		if ((n % 16) != 0) {
			dev_err(dev->class_dev, "insn data packet corrupted.\n");
			up(&udfs->sem);
			return -EINVAL;
		}
		for (j = chan; (j < n) && (i < insn->n); j = j + 16) {
			data[i] = ((uint16_t *) (udfs->transfer_buffer))[j];
			i++;
		}
	}
	up(&udfs->sem);
	return i;
}

#define FIRMWARE_MAX_LEN 0x2000

static int firmwareUpload(struct usbduxfastsub_s *usbduxfastsub,
			  const u8 *firmwareBinary, int sizeFirmware)
{
	int ret;
	uint8_t *fwBuf;

	if (!firmwareBinary)
		return 0;

	if (sizeFirmware > FIRMWARE_MAX_LEN) {
		dev_err(&usbduxfastsub->interface->dev,
			"comedi_: usbduxfast firmware binary it too large for FX2.\n");
=======
				   struct comedi_insn *insn,
				   unsigned int *data)
{
	struct usb_device *usb = comedi_to_usb_dev(dev);
	struct usbduxfast_private *devpriv = dev->private;
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int range = CR_RANGE(insn->chanspec);
	u8 rngmask = range ? (0xff - 0x04) : 0xff;
	int i, j, n, actual_length;
	int ret;

	mutex_lock(&devpriv->mut);

	if (devpriv->ai_cmd_running) {
		dev_err(dev->class_dev,
			"ai_insn_read not possible, async cmd is running\n");
		mutex_unlock(&devpriv->mut);
		return -EBUSY;
	}

	/* set command for the first channel */

	/* commit data to the FIFO */
	/* data */
	usbduxfast_cmd_data(dev, 0, 0x01, 0x02, rngmask, 0x00);

	/* do the first part of the delay */
	usbduxfast_cmd_data(dev, 1, 0x0c, 0x00, 0xfe & rngmask, 0x00);
	usbduxfast_cmd_data(dev, 2, 0x01, 0x00, 0xfe & rngmask, 0x00);
	usbduxfast_cmd_data(dev, 3, 0x01, 0x00, 0xfe & rngmask, 0x00);
	usbduxfast_cmd_data(dev, 4, 0x01, 0x00, 0xfe & rngmask, 0x00);

	/* second part */
	usbduxfast_cmd_data(dev, 5, 0x0c, 0x00, rngmask, 0x00);
	usbduxfast_cmd_data(dev, 6, 0x01, 0x00, rngmask, 0x00);

	ret = usbduxfast_send_cmd(dev, SENDADCOMMANDS);
	if (ret < 0) {
		mutex_unlock(&devpriv->mut);
		return ret;
	}

	for (i = 0; i < PACKETS_TO_IGNORE; i++) {
		ret = usb_bulk_msg(usb, usb_rcvbulkpipe(usb, BULKINEP),
				   devpriv->inbuf, SIZEINBUF,
				   &actual_length, 10000);
		if (ret < 0) {
			dev_err(dev->class_dev, "insn timeout, no data\n");
			mutex_unlock(&devpriv->mut);
			return ret;
		}
	}

	for (i = 0; i < insn->n;) {
		ret = usb_bulk_msg(usb, usb_rcvbulkpipe(usb, BULKINEP),
				   devpriv->inbuf, SIZEINBUF,
				   &actual_length, 10000);
		if (ret < 0) {
			dev_err(dev->class_dev, "insn data error: %d\n", ret);
			mutex_unlock(&devpriv->mut);
			return ret;
		}
		n = actual_length / sizeof(u16);
		if ((n % 16) != 0) {
			dev_err(dev->class_dev, "insn data packet corrupted\n");
			mutex_unlock(&devpriv->mut);
			return -EINVAL;
		}
		for (j = chan; (j < n) && (i < insn->n); j = j + 16) {
			data[i] = ((u16 *)(devpriv->inbuf))[j];
			i++;
		}
	}

	mutex_unlock(&devpriv->mut);

	return insn->n;
}

static int usbduxfast_upload_firmware(struct comedi_device *dev,
				      const u8 *data, size_t size,
				      unsigned long context)
{
	struct usb_device *usb = comedi_to_usb_dev(dev);
	u8 *buf;
	unsigned char *tmp;
	int ret;

	if (!data)
		return 0;

	if (size > FIRMWARE_MAX_LEN) {
		dev_err(dev->class_dev, "firmware binary too large for FX2\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -ENOMEM;
	}

	/* we generate a local buffer for the firmware */
<<<<<<< HEAD
	fwBuf = kmemdup(firmwareBinary, sizeFirmware, GFP_KERNEL);
	if (!fwBuf) {
		dev_err(&usbduxfastsub->interface->dev,
			"comedi_: mem alloc for firmware failed\n");
		return -ENOMEM;
	}

	ret = usbduxfastsub_stop(usbduxfastsub);
	if (ret < 0) {
		dev_err(&usbduxfastsub->interface->dev,
			"comedi_: can not stop firmware\n");
		kfree(fwBuf);
		return ret;
	}

	ret = usbduxfastsub_upload(usbduxfastsub, fwBuf, 0, sizeFirmware);
	if (ret < 0) {
		dev_err(&usbduxfastsub->interface->dev,
			"comedi_: firmware upload failed\n");
		kfree(fwBuf);
		return ret;
	}
	ret = usbduxfastsub_start(usbduxfastsub);
	if (ret < 0) {
		dev_err(&usbduxfastsub->interface->dev,
			"comedi_: can not start firmware\n");
		kfree(fwBuf);
		return ret;
	}
	kfree(fwBuf);
	return 0;
}

static void tidy_up(struct usbduxfastsub_s *udfs)
{
#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi_: usbduxfast: tiding up\n");
#endif

	if (!udfs)
		return;

	/* shows the usb subsystem that the driver is down */
	if (udfs->interface)
		usb_set_intfdata(udfs->interface, NULL);

	udfs->probed = 0;

	if (udfs->urbIn) {
		/* waits until a running transfer is over */
		usb_kill_urb(udfs->urbIn);

		kfree(udfs->transfer_buffer);
		udfs->transfer_buffer = NULL;

		usb_free_urb(udfs->urbIn);
		udfs->urbIn = NULL;
	}

	kfree(udfs->insnBuffer);
	udfs->insnBuffer = NULL;

	kfree(udfs->dux_commands);
	udfs->dux_commands = NULL;

	udfs->ai_cmd_running = 0;
}

static int usbduxfast_attach_common(struct comedi_device *dev,
				    struct usbduxfastsub_s *udfs)
{
	int ret;
	struct comedi_subdevice *s;

	down(&udfs->sem);
	/* pointer back to the corresponding comedi device */
	udfs->comedidev = dev;

	ret = comedi_alloc_subdevices(dev, 1);
	if (ret) {
		up(&udfs->sem);
		return ret;
	}
	/* private structure is also simply the usb-structure */
	dev->private = udfs;
	/* the first subdevice is the A/D converter */
	s = &dev->subdevices[SUBDEV_AD];
	/*
	 * the URBs get the comedi subdevice which is responsible for reading
	 * this is the subdevice which reads data
	 */
	dev->read_subdev = s;
	/* the subdevice receives as private structure the usb-structure */
	s->private = NULL;
	/* analog input */
	s->type = COMEDI_SUBD_AI;
	/* readable and ref is to ground */
	s->subdev_flags = SDF_READABLE | SDF_GROUND | SDF_CMD_READ;
	/* 16 channels */
	s->n_chan = 16;
	/* length of the channellist */
	s->len_chanlist = 16;
	/* callback functions */
	s->insn_read = usbduxfast_ai_insn_read;
	s->do_cmdtest = usbduxfast_ai_cmdtest;
	s->do_cmd = usbduxfast_ai_cmd;
	s->cancel = usbduxfast_ai_cancel;
	/* max value from the A/D converter (12bit+1 bit for overflow) */
	s->maxdata = 0x1000;
	/* range table to convert to physical units */
	s->range_table = &range_usbduxfast_ai_range;
	/* finally decide that it's attached */
	udfs->attached = 1;
	up(&udfs->sem);
	dev_info(dev->class_dev, "successfully attached to usbduxfast.\n");
	return 0;
}

static int usbduxfast_auto_attach(struct comedi_device *dev,
				  unsigned long context_unused)
{
	struct usb_interface *uinterf = comedi_to_usb_interface(dev);
	int ret;
	struct usbduxfastsub_s *udfs;

	dev->private = NULL;
	down(&start_stop_sem);
	udfs = usb_get_intfdata(uinterf);
	if (!udfs || !udfs->probed) {
		dev_err(dev->class_dev,
			"usbduxfast: error: auto_attach failed, not connected\n");
		ret = -ENODEV;
	} else if (udfs->attached) {
		dev_err(dev->class_dev,
		       "usbduxfast: error: auto_attach failed, already attached\n");
		ret = -ENODEV;
	} else
		ret = usbduxfast_attach_common(dev, udfs);
	up(&start_stop_sem);
	return ret;
}

static void usbduxfast_detach(struct comedi_device *dev)
{
	struct usbduxfastsub_s *usb = dev->private;

	if (usb) {
		down(&usb->sem);
		down(&start_stop_sem);
		dev->private = NULL;
		usb->attached = 0;
		usb->comedidev = NULL;
		up(&start_stop_sem);
		up(&usb->sem);
	}
=======
	buf = kmemdup(data, size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	/* we need a malloc'ed buffer for usb_control_msg() */
	tmp = kmalloc(1, GFP_KERNEL);
	if (!tmp) {
		kfree(buf);
		return -ENOMEM;
	}

	/* stop the current firmware on the device */
	*tmp = 1;	/* 7f92 to one */
	ret = usb_control_msg(usb, usb_sndctrlpipe(usb, 0),
			      USBDUXFASTSUB_FIRMWARE,
			      VENDOR_DIR_OUT,
			      USBDUXFASTSUB_CPUCS, 0x0000,
			      tmp, 1,
			      EZTIMEOUT);
	if (ret < 0) {
		dev_err(dev->class_dev, "can not stop firmware\n");
		goto done;
	}

	/* upload the new firmware to the device */
	ret = usb_control_msg(usb, usb_sndctrlpipe(usb, 0),
			      USBDUXFASTSUB_FIRMWARE,
			      VENDOR_DIR_OUT,
			      0, 0x0000,
			      buf, size,
			      EZTIMEOUT);
	if (ret < 0) {
		dev_err(dev->class_dev, "firmware upload failed\n");
		goto done;
	}

	/* start the new firmware on the device */
	*tmp = 0;	/* 7f92 to zero */
	ret = usb_control_msg(usb, usb_sndctrlpipe(usb, 0),
			      USBDUXFASTSUB_FIRMWARE,
			      VENDOR_DIR_OUT,
			      USBDUXFASTSUB_CPUCS, 0x0000,
			      tmp, 1,
			      EZTIMEOUT);
	if (ret < 0)
		dev_err(dev->class_dev, "can not start firmware\n");

done:
	kfree(tmp);
	kfree(buf);
	return ret;
}

static int usbduxfast_auto_attach(struct comedi_device *dev,
				  unsigned long context_unused)
{
	struct usb_interface *intf = comedi_to_usb_interface(dev);
	struct usb_device *usb = comedi_to_usb_dev(dev);
	struct usbduxfast_private *devpriv;
	struct comedi_subdevice *s;
	int ret;

	if (usb->speed != USB_SPEED_HIGH) {
		dev_err(dev->class_dev,
			"This driver needs USB 2.0 to operate. Aborting...\n");
		return -ENODEV;
	}

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv)
		return -ENOMEM;

	mutex_init(&devpriv->mut);
	usb_set_intfdata(intf, devpriv);

	devpriv->duxbuf = kmalloc(SIZEOFDUXBUF, GFP_KERNEL);
	if (!devpriv->duxbuf)
		return -ENOMEM;

	ret = usb_set_interface(usb,
				intf->altsetting->desc.bInterfaceNumber, 1);
	if (ret < 0) {
		dev_err(dev->class_dev,
			"could not switch to alternate setting 1\n");
		return -ENODEV;
	}

	devpriv->urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!devpriv->urb)
		return -ENOMEM;

	devpriv->inbuf = kmalloc(SIZEINBUF, GFP_KERNEL);
	if (!devpriv->inbuf)
		return -ENOMEM;

	ret = comedi_load_firmware(dev, &usb->dev, FIRMWARE,
				   usbduxfast_upload_firmware, 0);
	if (ret)
		return ret;

	ret = comedi_alloc_subdevices(dev, 1);
	if (ret)
		return ret;

	/* Analog Input subdevice */
	s = &dev->subdevices[0];
	dev->read_subdev = s;
	s->type		= COMEDI_SUBD_AI;
	s->subdev_flags	= SDF_READABLE | SDF_GROUND | SDF_CMD_READ;
	s->n_chan	= 16;
	s->maxdata	= 0x1000;	/* 12-bit + 1 overflow bit */
	s->range_table	= &range_usbduxfast_ai_range;
	s->insn_read	= usbduxfast_ai_insn_read;
	s->len_chanlist	= s->n_chan;
	s->do_cmdtest	= usbduxfast_ai_cmdtest;
	s->do_cmd	= usbduxfast_ai_cmd;
	s->cancel	= usbduxfast_ai_cancel;

	return 0;
}

static void usbduxfast_detach(struct comedi_device *dev)
{
	struct usb_interface *intf = comedi_to_usb_interface(dev);
	struct usbduxfast_private *devpriv = dev->private;

	if (!devpriv)
		return;

	mutex_lock(&devpriv->mut);

	usb_set_intfdata(intf, NULL);

	if (devpriv->urb) {
		/* waits until a running transfer is over */
		usb_kill_urb(devpriv->urb);

		kfree(devpriv->inbuf);
		usb_free_urb(devpriv->urb);
	}

	kfree(devpriv->duxbuf);

	mutex_unlock(&devpriv->mut);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static struct comedi_driver usbduxfast_driver = {
	.driver_name	= "usbduxfast",
	.module		= THIS_MODULE,
	.auto_attach	= usbduxfast_auto_attach,
	.detach		= usbduxfast_detach,
};

<<<<<<< HEAD
static void usbduxfast_firmware_request_complete_handler(const struct firmware
							 *fw, void *context)
{
	struct usbduxfastsub_s *usbduxfastsub_tmp = context;
	struct usb_interface *uinterf = usbduxfastsub_tmp->interface;
	int ret;

	if (fw == NULL)
		return;

	/*
	 * we need to upload the firmware here because fw will be
	 * freed once we've left this function
	 */
	ret = firmwareUpload(usbduxfastsub_tmp, fw->data, fw->size);

	if (ret) {
		dev_err(&uinterf->dev,
			"Could not upload firmware (err=%d)\n", ret);
		goto out;
	}

	comedi_usb_auto_config(uinterf, &usbduxfast_driver, 0);
 out:
	release_firmware(fw);
}

static int usbduxfast_usb_probe(struct usb_interface *uinterf,
				const struct usb_device_id *id)
{
	struct usb_device *udev = interface_to_usbdev(uinterf);
	int i;
	int index;
	int ret;

	if (udev->speed != USB_SPEED_HIGH) {
		dev_err(&uinterf->dev,
			"This driver needs USB 2.0 to operate. Aborting...\n");
		return -ENODEV;
	}
#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi_: usbduxfast_: finding a free structure for "
	       "the usb-device\n");
#endif
	down(&start_stop_sem);
	/* look for a free place in the usbduxfast array */
	index = -1;
	for (i = 0; i < NUMUSBDUXFAST; i++) {
		if (!usbduxfastsub[i].probed) {
			index = i;
			break;
		}
	}

	/* no more space */
	if (index == -1) {
		dev_err(&uinterf->dev,
			"Too many usbduxfast-devices connected.\n");
		up(&start_stop_sem);
		return -EMFILE;
	}
#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi_: usbduxfast: usbduxfastsub[%d] is ready to "
	       "connect to comedi.\n", index);
#endif

	sema_init(&(usbduxfastsub[index].sem), 1);
	/* save a pointer to the usb device */
	usbduxfastsub[index].usbdev = udev;

	/* save the interface itself */
	usbduxfastsub[index].interface = uinterf;
	/* get the interface number from the interface */
	usbduxfastsub[index].ifnum = uinterf->altsetting->desc.bInterfaceNumber;
	/*
	 * hand the private data over to the usb subsystem
	 * will be needed for disconnect
	 */
	usb_set_intfdata(uinterf, &(usbduxfastsub[index]));

#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi_: usbduxfast: ifnum=%d\n",
	       usbduxfastsub[index].ifnum);
#endif
	/* create space for the commands going to the usb device */
	usbduxfastsub[index].dux_commands = kmalloc(SIZEOFDUXBUFFER,
						    GFP_KERNEL);
	if (!usbduxfastsub[index].dux_commands) {
		tidy_up(&(usbduxfastsub[index]));
		up(&start_stop_sem);
		return -ENOMEM;
	}
	/* create space of the instruction buffer */
	usbduxfastsub[index].insnBuffer = kmalloc(SIZEINSNBUF, GFP_KERNEL);
	if (!usbduxfastsub[index].insnBuffer) {
		tidy_up(&(usbduxfastsub[index]));
		up(&start_stop_sem);
		return -ENOMEM;
	}
	/* setting to alternate setting 1: enabling bulk ep */
	i = usb_set_interface(usbduxfastsub[index].usbdev,
			      usbduxfastsub[index].ifnum, 1);
	if (i < 0) {
		dev_err(&uinterf->dev,
			"usbduxfast%d: could not switch to alternate setting 1.\n",
			index);
		tidy_up(&(usbduxfastsub[index]));
		up(&start_stop_sem);
		return -ENODEV;
	}
	usbduxfastsub[index].urbIn = usb_alloc_urb(0, GFP_KERNEL);
	if (!usbduxfastsub[index].urbIn) {
		dev_err(&uinterf->dev,
			"usbduxfast%d: Could not alloc. urb\n", index);
		tidy_up(&(usbduxfastsub[index]));
		up(&start_stop_sem);
		return -ENOMEM;
	}
	usbduxfastsub[index].transfer_buffer = kmalloc(SIZEINBUF, GFP_KERNEL);
	if (!usbduxfastsub[index].transfer_buffer) {
		tidy_up(&(usbduxfastsub[index]));
		up(&start_stop_sem);
		return -ENOMEM;
	}
	/* we've reached the bottom of the function */
	usbduxfastsub[index].probed = 1;
	up(&start_stop_sem);

	ret = request_firmware_nowait(THIS_MODULE,
				      FW_ACTION_HOTPLUG,
				      FIRMWARE,
				      &udev->dev,
				      GFP_KERNEL,
				      usbduxfastsub + index,
				      usbduxfast_firmware_request_complete_handler);

	if (ret) {
		dev_err(&uinterf->dev, "could not load firmware (err=%d)\n", ret);
		return ret;
	}

	dev_info(&uinterf->dev,
		 "usbduxfast%d has been successfully initialized.\n", index);
	/* success */
	return 0;
}

static void usbduxfast_usb_disconnect(struct usb_interface *intf)
{
	struct usbduxfastsub_s *udfs = usb_get_intfdata(intf);
	struct usb_device *udev = interface_to_usbdev(intf);

	if (!udfs) {
		dev_err(&intf->dev, "disconnect called with null pointer.\n");
		return;
	}
	if (udfs->usbdev != udev) {
		dev_err(&intf->dev, "BUG! called with wrong ptr!!!\n");
		return;
	}

	comedi_usb_auto_unconfig(intf);

	down(&start_stop_sem);
	down(&udfs->sem);
	tidy_up(udfs);
	up(&udfs->sem);
	up(&start_stop_sem);

#ifdef CONFIG_COMEDI_DEBUG
	printk(KERN_DEBUG "comedi_: usbduxfast: disconnected from the usb\n");
#endif
=======
static int usbduxfast_usb_probe(struct usb_interface *intf,
				const struct usb_device_id *id)
{
	return comedi_usb_auto_config(intf, &usbduxfast_driver, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static const struct usb_device_id usbduxfast_usb_table[] = {
	/* { USB_DEVICE(0x4b4, 0x8613) }, testing */
	{ USB_DEVICE(0x13d8, 0x0010) },	/* real ID */
	{ USB_DEVICE(0x13d8, 0x0011) },	/* real ID */
	{ }
};
MODULE_DEVICE_TABLE(usb, usbduxfast_usb_table);

static struct usb_driver usbduxfast_usb_driver = {
<<<<<<< HEAD
#ifdef COMEDI_HAVE_USB_DRIVER_OWNER
	.owner		= THIS_MODULE,
#endif
	.name		= "usbduxfast",
	.probe		= usbduxfast_usb_probe,
	.disconnect	= usbduxfast_usb_disconnect,
=======
	.name		= "usbduxfast",
	.probe		= usbduxfast_usb_probe,
	.disconnect	= comedi_usb_auto_unconfig,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.id_table	= usbduxfast_usb_table,
};
module_comedi_usb_driver(usbduxfast_driver, usbduxfast_usb_driver);

MODULE_AUTHOR("Bernd Porr, BerndPorr@f2s.com");
MODULE_DESCRIPTION("USB-DUXfast, BerndPorr@f2s.com");
MODULE_LICENSE("GPL");
MODULE_FIRMWARE(FIRMWARE);
