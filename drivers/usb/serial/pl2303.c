/*
 * Prolific PL2303 USB to serial adaptor driver
 *
 * Copyright (C) 2001-2007 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (C) 2003 IBM Corp.
 *
 * Original driver for 2.2.x by anonymous
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License version
 *	2 as published by the Free Software Foundation.
 *
 * See Documentation/usb/usb-serial.txt for more information on using this
 * driver
<<<<<<< HEAD
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#include <linux/kernel.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>
<<<<<<< HEAD
#include "pl2303.h"

/*
 * Version Information
 */
#define DRIVER_DESC "Prolific PL2303 USB to serial adaptor driver"
=======
#include <asm/unaligned.h>
#include "pl2303.h"


#define PL2303_QUIRK_UART_STATE_IDX0		BIT(0)
#define PL2303_QUIRK_LEGACY			BIT(1)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static const struct usb_device_id id_table[] = {
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID) },
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_RSAQ2) },
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_DCU11) },
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_RSAQ3) },
<<<<<<< HEAD
=======
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_CHILITAG) },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_PHAROS) },
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_ALDIGA) },
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_MMX) },
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_GPRS) },
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_HCR331) },
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_MOTOROLA) },
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_ZTEK) },
<<<<<<< HEAD
	{ USB_DEVICE(IODATA_VENDOR_ID, IODATA_PRODUCT_ID) },
	{ USB_DEVICE(IODATA_VENDOR_ID, IODATA_PRODUCT_ID_RSAQ5) },
	{ USB_DEVICE(ATEN_VENDOR_ID, ATEN_PRODUCT_ID) },
=======
	{ USB_DEVICE(PL2303_VENDOR_ID, PL2303_PRODUCT_ID_TB) },
	{ USB_DEVICE(IODATA_VENDOR_ID, IODATA_PRODUCT_ID) },
	{ USB_DEVICE(IODATA_VENDOR_ID, IODATA_PRODUCT_ID_RSAQ5) },
	{ USB_DEVICE(ATEN_VENDOR_ID, ATEN_PRODUCT_ID) },
	{ USB_DEVICE(ATEN_VENDOR_ID, ATEN_PRODUCT_UC485) },
	{ USB_DEVICE(ATEN_VENDOR_ID, ATEN_PRODUCT_ID2) },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ USB_DEVICE(ATEN_VENDOR_ID2, ATEN_PRODUCT_ID) },
	{ USB_DEVICE(ELCOM_VENDOR_ID, ELCOM_PRODUCT_ID) },
	{ USB_DEVICE(ELCOM_VENDOR_ID, ELCOM_PRODUCT_ID_UCSGT) },
	{ USB_DEVICE(ITEGNO_VENDOR_ID, ITEGNO_PRODUCT_ID) },
	{ USB_DEVICE(ITEGNO_VENDOR_ID, ITEGNO_PRODUCT_ID_2080) },
	{ USB_DEVICE(MA620_VENDOR_ID, MA620_PRODUCT_ID) },
	{ USB_DEVICE(RATOC_VENDOR_ID, RATOC_PRODUCT_ID) },
	{ USB_DEVICE(TRIPP_VENDOR_ID, TRIPP_PRODUCT_ID) },
	{ USB_DEVICE(RADIOSHACK_VENDOR_ID, RADIOSHACK_PRODUCT_ID) },
	{ USB_DEVICE(DCU10_VENDOR_ID, DCU10_PRODUCT_ID) },
	{ USB_DEVICE(SITECOM_VENDOR_ID, SITECOM_PRODUCT_ID) },
	{ USB_DEVICE(ALCATEL_VENDOR_ID, ALCATEL_PRODUCT_ID) },
<<<<<<< HEAD
	{ USB_DEVICE(SAMSUNG_VENDOR_ID, SAMSUNG_PRODUCT_ID) },
	{ USB_DEVICE(SIEMENS_VENDOR_ID, SIEMENS_PRODUCT_ID_SX1) },
	{ USB_DEVICE(SIEMENS_VENDOR_ID, SIEMENS_PRODUCT_ID_X65) },
	{ USB_DEVICE(SIEMENS_VENDOR_ID, SIEMENS_PRODUCT_ID_X75) },
=======
	{ USB_DEVICE(SIEMENS_VENDOR_ID, SIEMENS_PRODUCT_ID_SX1),
		.driver_info = PL2303_QUIRK_UART_STATE_IDX0 },
	{ USB_DEVICE(SIEMENS_VENDOR_ID, SIEMENS_PRODUCT_ID_X65),
		.driver_info = PL2303_QUIRK_UART_STATE_IDX0 },
	{ USB_DEVICE(SIEMENS_VENDOR_ID, SIEMENS_PRODUCT_ID_X75),
		.driver_info = PL2303_QUIRK_UART_STATE_IDX0 },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ USB_DEVICE(SIEMENS_VENDOR_ID, SIEMENS_PRODUCT_ID_EF81) },
	{ USB_DEVICE(BENQ_VENDOR_ID, BENQ_PRODUCT_ID_S81) }, /* Benq/Siemens S81 */
	{ USB_DEVICE(SYNTECH_VENDOR_ID, SYNTECH_PRODUCT_ID) },
	{ USB_DEVICE(NOKIA_CA42_VENDOR_ID, NOKIA_CA42_PRODUCT_ID) },
	{ USB_DEVICE(CA_42_CA42_VENDOR_ID, CA_42_CA42_PRODUCT_ID) },
	{ USB_DEVICE(SAGEM_VENDOR_ID, SAGEM_PRODUCT_ID) },
	{ USB_DEVICE(LEADTEK_VENDOR_ID, LEADTEK_9531_PRODUCT_ID) },
	{ USB_DEVICE(SPEEDDRAGON_VENDOR_ID, SPEEDDRAGON_PRODUCT_ID) },
	{ USB_DEVICE(DATAPILOT_U2_VENDOR_ID, DATAPILOT_U2_PRODUCT_ID) },
	{ USB_DEVICE(BELKIN_VENDOR_ID, BELKIN_PRODUCT_ID) },
	{ USB_DEVICE(ALCOR_VENDOR_ID, ALCOR_PRODUCT_ID) },
	{ USB_DEVICE(WS002IN_VENDOR_ID, WS002IN_PRODUCT_ID) },
	{ USB_DEVICE(COREGA_VENDOR_ID, COREGA_PRODUCT_ID) },
	{ USB_DEVICE(YCCABLE_VENDOR_ID, YCCABLE_PRODUCT_ID) },
	{ USB_DEVICE(SUPERIAL_VENDOR_ID, SUPERIAL_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LD220_PRODUCT_ID) },
<<<<<<< HEAD
	{ USB_DEVICE(HP_VENDOR_ID, HP_LD960_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LCM220_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LCM960_PRODUCT_ID) },
=======
	{ USB_DEVICE(HP_VENDOR_ID, HP_LD220TA_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LD381_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LD381GC_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LD960_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LD960TA_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LCM220_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LCM960_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LM920_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_LM940_PRODUCT_ID) },
	{ USB_DEVICE(HP_VENDOR_ID, HP_TD620_PRODUCT_ID) },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ USB_DEVICE(CRESSI_VENDOR_ID, CRESSI_EDY_PRODUCT_ID) },
	{ USB_DEVICE(ZEAGLE_VENDOR_ID, ZEAGLE_N2ITION3_PRODUCT_ID) },
	{ USB_DEVICE(SONY_VENDOR_ID, SONY_QN3USB_PRODUCT_ID) },
	{ USB_DEVICE(SANWA_VENDOR_ID, SANWA_PRODUCT_ID) },
	{ USB_DEVICE(ADLINK_VENDOR_ID, ADLINK_ND6530_PRODUCT_ID) },
<<<<<<< HEAD
	{ USB_DEVICE(SMART_VENDOR_ID, SMART_PRODUCT_ID) },
=======
	{ USB_DEVICE(ADLINK_VENDOR_ID, ADLINK_ND6530GC_PRODUCT_ID) },
	{ USB_DEVICE(SMART_VENDOR_ID, SMART_PRODUCT_ID) },
	{ USB_DEVICE(AT_VENDOR_ID, AT_VTKIT3_PRODUCT_ID) },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ }					/* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, id_table);

#define SET_LINE_REQUEST_TYPE		0x21
#define SET_LINE_REQUEST		0x20

#define SET_CONTROL_REQUEST_TYPE	0x21
#define SET_CONTROL_REQUEST		0x22
#define CONTROL_DTR			0x01
#define CONTROL_RTS			0x02

#define BREAK_REQUEST_TYPE		0x21
#define BREAK_REQUEST			0x23
#define BREAK_ON			0xffff
#define BREAK_OFF			0x0000

#define GET_LINE_REQUEST_TYPE		0xa1
#define GET_LINE_REQUEST		0x21

#define VENDOR_WRITE_REQUEST_TYPE	0x40
#define VENDOR_WRITE_REQUEST		0x01

#define VENDOR_READ_REQUEST_TYPE	0xc0
#define VENDOR_READ_REQUEST		0x01

<<<<<<< HEAD
#define UART_STATE			0x08
=======
#define UART_STATE_INDEX		8
#define UART_STATE_MSR_MASK		0x8b
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define UART_STATE_TRANSIENT_MASK	0x74
#define UART_DCD			0x01
#define UART_DSR			0x02
#define UART_BREAK_ERROR		0x04
#define UART_RING			0x08
#define UART_FRAME_ERROR		0x10
#define UART_PARITY_ERROR		0x20
#define UART_OVERRUN_ERROR		0x40
#define UART_CTS			0x80

<<<<<<< HEAD

enum pl2303_type {
	type_0,		/* don't know the difference between type 0 and */
	type_1,		/* type 1, until someone from prolific tells us... */
	HX,		/* HX version of the pl2303 chip */
};

struct pl2303_serial_private {
	enum pl2303_type type;
=======
static void pl2303_set_break(struct usb_serial_port *port, bool enable);

enum pl2303_type {
	TYPE_01,	/* Type 0 and 1 (difference unknown) */
	TYPE_HX,	/* HX version of the pl2303 chip */
	TYPE_COUNT
};

struct pl2303_type_data {
	speed_t max_baud_rate;
	unsigned long quirks;
};

struct pl2303_serial_private {
	const struct pl2303_type_data *type;
	unsigned long quirks;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct pl2303_private {
	spinlock_t lock;
	u8 line_control;
	u8 line_status;

	u8 line_settings[7];
};

<<<<<<< HEAD
static int pl2303_vendor_read(__u16 value, __u16 index,
		struct usb_serial *serial, unsigned char *buf)
{
	int res = usb_control_msg(serial->dev, usb_rcvctrlpipe(serial->dev, 0),
			VENDOR_READ_REQUEST, VENDOR_READ_REQUEST_TYPE,
			value, index, buf, 1, 100);
	dev_dbg(&serial->interface->dev, "0x%x:0x%x:0x%x:0x%x  %d - %x\n",
		VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, value, index,
		res, buf[0]);
	return res;
}

static int pl2303_vendor_write(__u16 value, __u16 index,
		struct usb_serial *serial)
{
	int res = usb_control_msg(serial->dev, usb_sndctrlpipe(serial->dev, 0),
			VENDOR_WRITE_REQUEST, VENDOR_WRITE_REQUEST_TYPE,
			value, index, NULL, 0, 100);
	dev_dbg(&serial->interface->dev, "0x%x:0x%x:0x%x:0x%x  %d\n",
		VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, value, index,
		res);
	return res;
=======
static const struct pl2303_type_data pl2303_type_data[TYPE_COUNT] = {
	[TYPE_01] = {
		.max_baud_rate =	1228800,
		.quirks =		PL2303_QUIRK_LEGACY,
	},
	[TYPE_HX] = {
		.max_baud_rate =	12000000,
	},
};

static int pl2303_vendor_read(struct usb_serial *serial, u16 value,
							unsigned char buf[1])
{
	struct device *dev = &serial->interface->dev;
	int res;

	res = usb_control_msg(serial->dev, usb_rcvctrlpipe(serial->dev, 0),
			VENDOR_READ_REQUEST, VENDOR_READ_REQUEST_TYPE,
			value, 0, buf, 1, 100);
	if (res != 1) {
		dev_err(dev, "%s - failed to read [%04x]: %d\n", __func__,
								value, res);
		if (res >= 0)
			res = -EIO;

		return res;
	}

	dev_dbg(dev, "%s - [%04x] = %02x\n", __func__, value, buf[0]);

	return 0;
}

static int pl2303_vendor_write(struct usb_serial *serial, u16 value, u16 index)
{
	struct device *dev = &serial->interface->dev;
	int res;

	dev_dbg(dev, "%s - [%04x] = %02x\n", __func__, value, index);

	res = usb_control_msg(serial->dev, usb_sndctrlpipe(serial->dev, 0),
			VENDOR_WRITE_REQUEST, VENDOR_WRITE_REQUEST_TYPE,
			value, index, NULL, 0, 100);
	if (res) {
		dev_err(dev, "%s - failed to write [%04x]: %d\n", __func__,
								value, res);
		return res;
	}

	return 0;
}

static int pl2303_probe(struct usb_serial *serial,
					const struct usb_device_id *id)
{
	usb_set_serial_data(serial, (void *)id->driver_info);

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int pl2303_startup(struct usb_serial *serial)
{
	struct pl2303_serial_private *spriv;
	unsigned char num_ports = serial->num_ports;
<<<<<<< HEAD
	enum pl2303_type type = type_0;
=======
	enum pl2303_type type = TYPE_01;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned char *buf;

	if (serial->num_bulk_in < num_ports ||
			serial->num_bulk_out < num_ports ||
			serial->num_interrupt_in < num_ports) {
		dev_err(&serial->interface->dev, "missing endpoints\n");
		return -ENODEV;
	}

	spriv = kzalloc(sizeof(*spriv), GFP_KERNEL);
	if (!spriv)
		return -ENOMEM;

<<<<<<< HEAD
	buf = kmalloc(10, GFP_KERNEL);
=======
	buf = kmalloc(1, GFP_KERNEL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!buf) {
		kfree(spriv);
		return -ENOMEM;
	}

	if (serial->dev->descriptor.bDeviceClass == 0x02)
<<<<<<< HEAD
		type = type_0;
	else if (serial->dev->descriptor.bMaxPacketSize0 == 0x40)
		type = HX;
	else if (serial->dev->descriptor.bDeviceClass == 0x00)
		type = type_1;
	else if (serial->dev->descriptor.bDeviceClass == 0xFF)
		type = type_1;
	dev_dbg(&serial->interface->dev, "device type: %d\n", type);

	spriv->type = type;
	usb_set_serial_data(serial, spriv);

	pl2303_vendor_read(0x8484, 0, serial, buf);
	pl2303_vendor_write(0x0404, 0, serial);
	pl2303_vendor_read(0x8484, 0, serial, buf);
	pl2303_vendor_read(0x8383, 0, serial, buf);
	pl2303_vendor_read(0x8484, 0, serial, buf);
	pl2303_vendor_write(0x0404, 1, serial);
	pl2303_vendor_read(0x8484, 0, serial, buf);
	pl2303_vendor_read(0x8383, 0, serial, buf);
	pl2303_vendor_write(0, 1, serial);
	pl2303_vendor_write(1, 0, serial);
	if (type == HX)
		pl2303_vendor_write(2, 0x44, serial);
	else
		pl2303_vendor_write(2, 0x24, serial);

	kfree(buf);
=======
		type = TYPE_01;		/* type 0 */
	else if (serial->dev->descriptor.bMaxPacketSize0 == 0x40)
		type = TYPE_HX;
	else if (serial->dev->descriptor.bDeviceClass == 0x00)
		type = TYPE_01;		/* type 1 */
	else if (serial->dev->descriptor.bDeviceClass == 0xFF)
		type = TYPE_01;		/* type 1 */
	dev_dbg(&serial->interface->dev, "device type: %d\n", type);

	spriv->type = &pl2303_type_data[type];
	spriv->quirks = (unsigned long)usb_get_serial_data(serial);
	spriv->quirks |= spriv->type->quirks;

	usb_set_serial_data(serial, spriv);

	pl2303_vendor_read(serial, 0x8484, buf);
	pl2303_vendor_write(serial, 0x0404, 0);
	pl2303_vendor_read(serial, 0x8484, buf);
	pl2303_vendor_read(serial, 0x8383, buf);
	pl2303_vendor_read(serial, 0x8484, buf);
	pl2303_vendor_write(serial, 0x0404, 1);
	pl2303_vendor_read(serial, 0x8484, buf);
	pl2303_vendor_read(serial, 0x8383, buf);
	pl2303_vendor_write(serial, 0, 1);
	pl2303_vendor_write(serial, 1, 0);
	if (spriv->quirks & PL2303_QUIRK_LEGACY)
		pl2303_vendor_write(serial, 2, 0x24);
	else
		pl2303_vendor_write(serial, 2, 0x44);

	kfree(buf);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static void pl2303_release(struct usb_serial *serial)
{
<<<<<<< HEAD
	struct pl2303_serial_private *spriv;

	spriv = usb_get_serial_data(serial);
=======
	struct pl2303_serial_private *spriv = usb_get_serial_data(serial);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(spriv);
}

static int pl2303_port_probe(struct usb_serial_port *port)
{
	struct pl2303_private *priv;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	spin_lock_init(&priv->lock);

	usb_set_serial_port_data(port, priv);

<<<<<<< HEAD
=======
	port->port.drain_delay = 256;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static int pl2303_port_remove(struct usb_serial_port *port)
{
<<<<<<< HEAD
	struct pl2303_private *priv;

	priv = usb_get_serial_port_data(port);
=======
	struct pl2303_private *priv = usb_get_serial_port_data(port);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(priv);

	return 0;
}

static int pl2303_set_control_lines(struct usb_serial_port *port, u8 value)
{
	struct usb_device *dev = port->serial->dev;
	int retval;

<<<<<<< HEAD
	retval = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
				 SET_CONTROL_REQUEST, SET_CONTROL_REQUEST_TYPE,
				 value, 0, NULL, 0, 100);
	dev_dbg(&port->dev, "%s - value = %d, retval = %d\n", __func__,
		value, retval);
	return retval;
}

=======
	dev_dbg(&port->dev, "%s - %02x\n", __func__, value);

	retval = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
				 SET_CONTROL_REQUEST, SET_CONTROL_REQUEST_TYPE,
				 value, 0, NULL, 0, 100);
	if (retval)
		dev_err(&port->dev, "%s - failed: %d\n", __func__, retval);

	return retval;
}

/*
 * Returns the nearest supported baud rate that can be set directly without
 * using divisors.
 */
static speed_t pl2303_get_supported_baud_rate(speed_t baud)
{
	static const speed_t baud_sup[] = {
		75, 150, 300, 600, 1200, 1800, 2400, 3600, 4800, 7200, 9600,
		14400, 19200, 28800, 38400, 57600, 115200, 230400, 460800,
		614400, 921600, 1228800, 2457600, 3000000, 6000000
	};

	unsigned i;

	for (i = 0; i < ARRAY_SIZE(baud_sup); ++i) {
		if (baud_sup[i] > baud)
			break;
	}

	if (i == ARRAY_SIZE(baud_sup))
		baud = baud_sup[i - 1];
	else if (i > 0 && (baud_sup[i] - baud) > (baud - baud_sup[i - 1]))
		baud = baud_sup[i - 1];
	else
		baud = baud_sup[i];

	return baud;
}

/*
 * NOTE: If unsupported baud rates are set directly, the PL2303 seems to
 *       use 9600 baud.
 */
static speed_t pl2303_encode_baud_rate_direct(unsigned char buf[4],
								speed_t baud)
{
	put_unaligned_le32(baud, buf);

	return baud;
}

static speed_t pl2303_encode_baud_rate_divisor(unsigned char buf[4],
								speed_t baud)
{
	unsigned int baseline, mantissa, exponent;

	/*
	 * Apparently the formula is:
	 *   baudrate = 12M * 32 / (mantissa * 4^exponent)
	 * where
	 *   mantissa = buf[8:0]
	 *   exponent = buf[11:9]
	 */
	baseline = 12000000 * 32;
	mantissa = baseline / baud;
	if (mantissa == 0)
		mantissa = 1;	/* Avoid dividing by zero if baud > 32*12M. */
	exponent = 0;
	while (mantissa >= 512) {
		if (exponent < 7) {
			mantissa >>= 2;	/* divide by 4 */
			exponent++;
		} else {
			/* Exponent is maxed. Trim mantissa and leave. */
			mantissa = 511;
			break;
		}
	}

	buf[3] = 0x80;
	buf[2] = 0;
	buf[1] = exponent << 1 | mantissa >> 8;
	buf[0] = mantissa & 0xff;

	/* Calculate and return the exact baud rate. */
	baud = (baseline / mantissa) >> (exponent << 1);

	return baud;
}

static void pl2303_encode_baud_rate(struct tty_struct *tty,
					struct usb_serial_port *port,
					u8 buf[4])
{
	struct usb_serial *serial = port->serial;
	struct pl2303_serial_private *spriv = usb_get_serial_data(serial);
	speed_t	baud_sup;
	speed_t baud;

	baud = tty_get_baud_rate(tty);
	dev_dbg(&port->dev, "baud requested = %u\n", baud);
	if (!baud)
		return;

	if (spriv->type->max_baud_rate)
		baud = min_t(speed_t, baud, spriv->type->max_baud_rate);
	/*
	 * Use direct method for supported baud rates, otherwise use divisors.
	 */
	baud_sup = pl2303_get_supported_baud_rate(baud);

	if (baud == baud_sup)
		baud = pl2303_encode_baud_rate_direct(buf, baud);
	else
		baud = pl2303_encode_baud_rate_divisor(buf, baud);

	/* Save resulting baud rate */
	tty_encode_baud_rate(tty, baud, baud);
	dev_dbg(&port->dev, "baud set = %u\n", baud);
}

static int pl2303_get_line_request(struct usb_serial_port *port,
							unsigned char buf[7])
{
	struct usb_device *udev = port->serial->dev;
	int ret;

	ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
				GET_LINE_REQUEST, GET_LINE_REQUEST_TYPE,
				0, 0, buf, 7, 100);
	if (ret != 7) {
		dev_err(&port->dev, "%s - failed: %d\n", __func__, ret);

		if (ret > 0)
			ret = -EIO;

		return ret;
	}

	dev_dbg(&port->dev, "%s - %7ph\n", __func__, buf);

	return 0;
}

static int pl2303_set_line_request(struct usb_serial_port *port,
							unsigned char buf[7])
{
	struct usb_device *udev = port->serial->dev;
	int ret;

	ret = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
				SET_LINE_REQUEST, SET_LINE_REQUEST_TYPE,
				0, 0, buf, 7, 100);
	if (ret != 7) {
		dev_err(&port->dev, "%s - failed: %d\n", __func__, ret);

		if (ret > 0)
			ret = -EIO;

		return ret;
	}

	dev_dbg(&port->dev, "%s - %7ph\n", __func__, buf);

	return 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void pl2303_set_termios(struct tty_struct *tty,
		struct usb_serial_port *port, struct ktermios *old_termios)
{
	struct usb_serial *serial = port->serial;
	struct pl2303_serial_private *spriv = usb_get_serial_data(serial);
	struct pl2303_private *priv = usb_get_serial_port_data(port);
	unsigned long flags;
<<<<<<< HEAD
	unsigned int cflag;
	unsigned char *buf;
	int baud;
	int i;
	u8 control;
	const int baud_sup[] = { 75, 150, 300, 600, 1200, 1800, 2400, 3600,
	                         4800, 7200, 9600, 14400, 19200, 28800, 38400,
	                         57600, 115200, 230400, 460800, 614400,
	                         921600, 1228800, 2457600, 3000000, 6000000 };
	int baud_floor, baud_ceil;
	int k;
=======
	unsigned char *buf;
	int ret;
	u8 control;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (old_termios && !tty_termios_hw_change(&tty->termios, old_termios))
		return;

<<<<<<< HEAD
	cflag = tty->termios.c_cflag;

	buf = kzalloc(7, GFP_KERNEL);
	if (!buf) {
		dev_err(&port->dev, "%s - out of memory.\n", __func__);
=======
	buf = kzalloc(7, GFP_KERNEL);
	if (!buf) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* Report back no change occurred */
		if (old_termios)
			tty->termios = *old_termios;
		return;
	}

<<<<<<< HEAD
	i = usb_control_msg(serial->dev, usb_rcvctrlpipe(serial->dev, 0),
			    GET_LINE_REQUEST, GET_LINE_REQUEST_TYPE,
			    0, 0, buf, 7, 100);
	dev_dbg(&port->dev, "0xa1:0x21:0:0  %d - %x %x %x %x %x %x %x\n", i,
	    buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);

	switch (cflag & CSIZE) {
=======
	pl2303_get_line_request(port, buf);

	switch (C_CSIZE(tty)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case CS5:
		buf[6] = 5;
		break;
	case CS6:
		buf[6] = 6;
		break;
	case CS7:
		buf[6] = 7;
		break;
	default:
	case CS8:
		buf[6] = 8;
<<<<<<< HEAD
		break;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	dev_dbg(&port->dev, "data bits = %d\n", buf[6]);

	/* For reference buf[0]:buf[3] baud rate value */
<<<<<<< HEAD
	/* NOTE: Only the values defined in baud_sup are supported !
	 *       => if unsupported values are set, the PL2303 seems to use
	 *          9600 baud (at least my PL2303X always does)
	 */
	baud = tty_get_baud_rate(tty);
	dev_dbg(&port->dev, "baud requested = %d\n", baud);
	if (baud) {
		/* Set baudrate to nearest supported value */
		for (k=0; k<ARRAY_SIZE(baud_sup); k++) {
			if (baud_sup[k] / baud) {
				baud_ceil = baud_sup[k];
				if (k==0) {
					baud = baud_ceil;
				} else {
					baud_floor = baud_sup[k-1];
					if ((baud_ceil % baud)
					    > (baud % baud_floor))
						baud = baud_floor;
					else
						baud = baud_ceil;
				}
				break;
			}
		}
		if (baud > 1228800) {
			/* type_0, type_1 only support up to 1228800 baud */
			if (spriv->type != HX)
				baud = 1228800;
			else if (baud > 6000000)
				baud = 6000000;
		}
		dev_dbg(&port->dev, "baud set = %d\n", baud);
		if (baud <= 115200) {
			buf[0] = baud & 0xff;
			buf[1] = (baud >> 8) & 0xff;
			buf[2] = (baud >> 16) & 0xff;
			buf[3] = (baud >> 24) & 0xff;
		} else {
			/* apparently the formula for higher speeds is:
			 * baudrate = 12M * 32 / (2^buf[1]) / buf[0]
			 */
			unsigned tmp = 12*1000*1000*32 / baud;
			buf[3] = 0x80;
			buf[2] = 0;
			buf[1] = (tmp >= 256);
			while (tmp >= 256) {
				tmp >>= 2;
				buf[1] <<= 1;
			}
			buf[0] = tmp;
		}
	}
=======
	pl2303_encode_baud_rate(tty, port, &buf[0]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* For reference buf[4]=0 is 1 stop bits */
	/* For reference buf[4]=1 is 1.5 stop bits */
	/* For reference buf[4]=2 is 2 stop bits */
<<<<<<< HEAD
	if (cflag & CSTOPB) {
		/* NOTE: Comply with "real" UARTs / RS232:
		 *       use 1.5 instead of 2 stop bits with 5 data bits
		 */
		if ((cflag & CSIZE) == CS5) {
=======
	if (C_CSTOPB(tty)) {
		/*
		 * NOTE: Comply with "real" UARTs / RS232:
		 *       use 1.5 instead of 2 stop bits with 5 data bits
		 */
		if (C_CSIZE(tty) == CS5) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			buf[4] = 1;
			dev_dbg(&port->dev, "stop bits = 1.5\n");
		} else {
			buf[4] = 2;
			dev_dbg(&port->dev, "stop bits = 2\n");
		}
	} else {
		buf[4] = 0;
		dev_dbg(&port->dev, "stop bits = 1\n");
	}

<<<<<<< HEAD
	if (cflag & PARENB) {
=======
	if (C_PARENB(tty)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* For reference buf[5]=0 is none parity */
		/* For reference buf[5]=1 is odd parity */
		/* For reference buf[5]=2 is even parity */
		/* For reference buf[5]=3 is mark parity */
		/* For reference buf[5]=4 is space parity */
<<<<<<< HEAD
		if (cflag & PARODD) {
			if (cflag & CMSPAR) {
=======
		if (C_PARODD(tty)) {
			if (C_CMSPAR(tty)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				buf[5] = 3;
				dev_dbg(&port->dev, "parity = mark\n");
			} else {
				buf[5] = 1;
				dev_dbg(&port->dev, "parity = odd\n");
			}
		} else {
<<<<<<< HEAD
			if (cflag & CMSPAR) {
=======
			if (C_CMSPAR(tty)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				buf[5] = 4;
				dev_dbg(&port->dev, "parity = space\n");
			} else {
				buf[5] = 2;
				dev_dbg(&port->dev, "parity = even\n");
			}
		}
	} else {
		buf[5] = 0;
		dev_dbg(&port->dev, "parity = none\n");
	}

	/*
	 * Some PL2303 are known to lose bytes if you change serial settings
	 * even to the same values as before. Thus we actually need to filter
	 * in this specific case.
	 *
	 * Note that the tty_termios_hw_change check above is not sufficient
	 * as a previously requested baud rate may differ from the one
	 * actually used (and stored in old_termios).
	 *
	 * NOTE: No additional locking needed for line_settings as it is
	 *       only used in set_termios, which is serialised against itself.
	 */
	if (!old_termios || memcmp(buf, priv->line_settings, 7)) {
<<<<<<< HEAD
		i = usb_control_msg(serial->dev,
				    usb_sndctrlpipe(serial->dev, 0),
				    SET_LINE_REQUEST, SET_LINE_REQUEST_TYPE,
				    0, 0, buf, 7, 100);

		dev_dbg(&port->dev, "0x21:0x20:0:0  %d\n", i);

		if (i == 7)
=======
		ret = pl2303_set_line_request(port, buf);
		if (!ret)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			memcpy(priv->line_settings, buf, 7);
	}

	/* change control lines if we are switching to or from B0 */
	spin_lock_irqsave(&priv->lock, flags);
	control = priv->line_control;
<<<<<<< HEAD
	if ((cflag & CBAUD) == B0)
=======
	if (C_BAUD(tty) == B0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		priv->line_control &= ~(CONTROL_DTR | CONTROL_RTS);
	else if (old_termios && (old_termios->c_cflag & CBAUD) == B0)
		priv->line_control |= (CONTROL_DTR | CONTROL_RTS);
	if (control != priv->line_control) {
		control = priv->line_control;
		spin_unlock_irqrestore(&priv->lock, flags);
		pl2303_set_control_lines(port, control);
	} else {
		spin_unlock_irqrestore(&priv->lock, flags);
	}

<<<<<<< HEAD
	buf[0] = buf[1] = buf[2] = buf[3] = buf[4] = buf[5] = buf[6] = 0;

	i = usb_control_msg(serial->dev, usb_rcvctrlpipe(serial->dev, 0),
			    GET_LINE_REQUEST, GET_LINE_REQUEST_TYPE,
			    0, 0, buf, 7, 100);
	dev_dbg(&port->dev, "0xa1:0x21:0:0  %d - %x %x %x %x %x %x %x\n", i,
	     buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);

	if (cflag & CRTSCTS) {
		if (spriv->type == HX)
			pl2303_vendor_write(0x0, 0x61, serial);
		else
			pl2303_vendor_write(0x0, 0x41, serial);
	} else {
		pl2303_vendor_write(0x0, 0x0, serial);
	}

	/* Save resulting baud rate */
	if (baud)
		tty_encode_baud_rate(tty, baud, baud);

=======
	if (C_CRTSCTS(tty)) {
		if (spriv->quirks & PL2303_QUIRK_LEGACY)
			pl2303_vendor_write(serial, 0x0, 0x41);
		else
			pl2303_vendor_write(serial, 0x0, 0x61);
	} else {
		pl2303_vendor_write(serial, 0x0, 0x0);
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(buf);
}

static void pl2303_dtr_rts(struct usb_serial_port *port, int on)
{
	struct pl2303_private *priv = usb_get_serial_port_data(port);
	unsigned long flags;
	u8 control;

	spin_lock_irqsave(&priv->lock, flags);
<<<<<<< HEAD
	/* Change DTR and RTS */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (on)
		priv->line_control |= (CONTROL_DTR | CONTROL_RTS);
	else
		priv->line_control &= ~(CONTROL_DTR | CONTROL_RTS);
	control = priv->line_control;
	spin_unlock_irqrestore(&priv->lock, flags);
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pl2303_set_control_lines(port, control);
}

static void pl2303_close(struct usb_serial_port *port)
{
	usb_serial_generic_close(port);
	usb_kill_urb(port->interrupt_in_urb);
<<<<<<< HEAD
=======
	pl2303_set_break(port, false);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int pl2303_open(struct tty_struct *tty, struct usb_serial_port *port)
{
	struct usb_serial *serial = port->serial;
	struct pl2303_serial_private *spriv = usb_get_serial_data(serial);
	int result;

<<<<<<< HEAD
	if (spriv->type != HX) {
=======
	if (spriv->quirks & PL2303_QUIRK_LEGACY) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		usb_clear_halt(serial->dev, port->write_urb->pipe);
		usb_clear_halt(serial->dev, port->read_urb->pipe);
	} else {
		/* reset upstream data pipes */
<<<<<<< HEAD
		pl2303_vendor_write(8, 0, serial);
		pl2303_vendor_write(9, 0, serial);
=======
		pl2303_vendor_write(serial, 8, 0);
		pl2303_vendor_write(serial, 9, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Setup termios */
	if (tty)
		pl2303_set_termios(tty, port, NULL);

	result = usb_submit_urb(port->interrupt_in_urb, GFP_KERNEL);
	if (result) {
<<<<<<< HEAD
		dev_err(&port->dev, "%s - failed submitting interrupt urb,"
			" error %d\n", __func__, result);
=======
		dev_err(&port->dev, "failed to submit interrupt urb: %d\n",
			result);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return result;
	}

	result = usb_serial_generic_open(tty, port);
	if (result) {
		usb_kill_urb(port->interrupt_in_urb);
		return result;
	}

<<<<<<< HEAD
	port->port.drain_delay = 256;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static int pl2303_tiocmset(struct tty_struct *tty,
			   unsigned int set, unsigned int clear)
{
	struct usb_serial_port *port = tty->driver_data;
	struct pl2303_private *priv = usb_get_serial_port_data(port);
	unsigned long flags;
	u8 control;
	int ret;

	spin_lock_irqsave(&priv->lock, flags);
	if (set & TIOCM_RTS)
		priv->line_control |= CONTROL_RTS;
	if (set & TIOCM_DTR)
		priv->line_control |= CONTROL_DTR;
	if (clear & TIOCM_RTS)
		priv->line_control &= ~CONTROL_RTS;
	if (clear & TIOCM_DTR)
		priv->line_control &= ~CONTROL_DTR;
	control = priv->line_control;
	spin_unlock_irqrestore(&priv->lock, flags);

	ret = pl2303_set_control_lines(port, control);
	if (ret)
		return usb_translate_errors(ret);

	return 0;
}

static int pl2303_tiocmget(struct tty_struct *tty)
{
	struct usb_serial_port *port = tty->driver_data;
	struct pl2303_private *priv = usb_get_serial_port_data(port);
	unsigned long flags;
	unsigned int mcr;
	unsigned int status;
	unsigned int result;

	spin_lock_irqsave(&priv->lock, flags);
	mcr = priv->line_control;
	status = priv->line_status;
	spin_unlock_irqrestore(&priv->lock, flags);

	result = ((mcr & CONTROL_DTR)		? TIOCM_DTR : 0)
		  | ((mcr & CONTROL_RTS)	? TIOCM_RTS : 0)
		  | ((status & UART_CTS)	? TIOCM_CTS : 0)
		  | ((status & UART_DSR)	? TIOCM_DSR : 0)
		  | ((status & UART_RING)	? TIOCM_RI  : 0)
		  | ((status & UART_DCD)	? TIOCM_CD  : 0);

	dev_dbg(&port->dev, "%s - result = %x\n", __func__, result);

	return result;
}

static int pl2303_carrier_raised(struct usb_serial_port *port)
{
	struct pl2303_private *priv = usb_get_serial_port_data(port);
<<<<<<< HEAD
	if (priv->line_status & UART_DCD)
		return 1;
	return 0;
}

static int pl2303_tiocmiwait(struct tty_struct *tty, unsigned long arg)
{
	struct usb_serial_port *port = tty->driver_data;
	struct pl2303_private *priv = usb_get_serial_port_data(port);
	unsigned long flags;
	unsigned int prevstatus;
	unsigned int status;
	unsigned int changed;

	spin_lock_irqsave(&priv->lock, flags);
	prevstatus = priv->line_status;
	spin_unlock_irqrestore(&priv->lock, flags);

	while (1) {
		interruptible_sleep_on(&port->port.delta_msr_wait);
		/* see if a signal did it */
		if (signal_pending(current))
			return -ERESTARTSYS;

		if (port->serial->disconnected)
			return -EIO;

		spin_lock_irqsave(&priv->lock, flags);
		status = priv->line_status;
		spin_unlock_irqrestore(&priv->lock, flags);

		changed = prevstatus ^ status;

		if (((arg & TIOCM_RNG) && (changed & UART_RING)) ||
		    ((arg & TIOCM_DSR) && (changed & UART_DSR)) ||
		    ((arg & TIOCM_CD)  && (changed & UART_DCD)) ||
		    ((arg & TIOCM_CTS) && (changed & UART_CTS))) {
			return 0;
		}
		prevstatus = status;
	}
	/* NOTREACHED */
=======

	if (priv->line_status & UART_DCD)
		return 1;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static int pl2303_ioctl(struct tty_struct *tty,
			unsigned int cmd, unsigned long arg)
{
	struct serial_struct ser;
	struct usb_serial_port *port = tty->driver_data;

<<<<<<< HEAD
	dev_dbg(&port->dev, "%s cmd = 0x%04x\n", __func__, cmd);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	switch (cmd) {
	case TIOCGSERIAL:
		memset(&ser, 0, sizeof ser);
		ser.type = PORT_16654;
<<<<<<< HEAD
		ser.line = port->serial->minor;
		ser.port = port->number;
=======
		ser.line = port->minor;
		ser.port = port->port_number;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ser.baud_base = 460800;

		if (copy_to_user((void __user *)arg, &ser, sizeof ser))
			return -EFAULT;

		return 0;
	default:
<<<<<<< HEAD
		dev_dbg(&port->dev, "%s not supported = 0x%04x\n", __func__, cmd);
		break;
	}
	return -ENOIOCTLCMD;
}

static void pl2303_break_ctl(struct tty_struct *tty, int break_state)
{
	struct usb_serial_port *port = tty->driver_data;
=======
		break;
	}

	return -ENOIOCTLCMD;
}

static void pl2303_set_break(struct usb_serial_port *port, bool enable)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct usb_serial *serial = port->serial;
	u16 state;
	int result;

<<<<<<< HEAD
	if (break_state == 0)
		state = BREAK_OFF;
	else
		state = BREAK_ON;
=======
	if (enable)
		state = BREAK_ON;
	else
		state = BREAK_OFF;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dev_dbg(&port->dev, "%s - turning break %s\n", __func__,
			state == BREAK_OFF ? "off" : "on");

	result = usb_control_msg(serial->dev, usb_sndctrlpipe(serial->dev, 0),
				 BREAK_REQUEST, BREAK_REQUEST_TYPE, state,
				 0, NULL, 0, 100);
	if (result)
		dev_err(&port->dev, "error sending break = %d\n", result);
}

<<<<<<< HEAD
=======
static void pl2303_break_ctl(struct tty_struct *tty, int state)
{
	struct usb_serial_port *port = tty->driver_data;

	pl2303_set_break(port, state);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void pl2303_update_line_status(struct usb_serial_port *port,
				      unsigned char *data,
				      unsigned int actual_length)
{
<<<<<<< HEAD

	struct pl2303_private *priv = usb_get_serial_port_data(port);
	struct tty_struct *tty;
	unsigned long flags;
	u8 status_idx = UART_STATE;
	u8 length = UART_STATE + 1;
	u8 prev_line_status;
	u16 idv, idp;

	idv = le16_to_cpu(port->serial->dev->descriptor.idVendor);
	idp = le16_to_cpu(port->serial->dev->descriptor.idProduct);


	if (idv == SIEMENS_VENDOR_ID) {
		if (idp == SIEMENS_PRODUCT_ID_X65 ||
		    idp == SIEMENS_PRODUCT_ID_SX1 ||
		    idp == SIEMENS_PRODUCT_ID_X75) {

			length = 1;
			status_idx = 0;
		}
	}

	if (actual_length < length)
		return;

	/* Save off the uart status for others to look at */
	spin_lock_irqsave(&priv->lock, flags);
	prev_line_status = priv->line_status;
	priv->line_status = data[status_idx];
	spin_unlock_irqrestore(&priv->lock, flags);
	if (priv->line_status & UART_BREAK_ERROR)
		usb_serial_handle_break(port);
	wake_up_interruptible(&port->port.delta_msr_wait);

	tty = tty_port_tty_get(&port->port);
	if (!tty)
		return;
	if ((priv->line_status ^ prev_line_status) & UART_DCD)
		usb_serial_handle_dcd_change(port, tty,
				priv->line_status & UART_DCD);
	tty_kref_put(tty);
=======
	struct usb_serial *serial = port->serial;
	struct pl2303_serial_private *spriv = usb_get_serial_data(serial);
	struct pl2303_private *priv = usb_get_serial_port_data(port);
	struct tty_struct *tty;
	unsigned long flags;
	unsigned int status_idx = UART_STATE_INDEX;
	u8 status;
	u8 delta;

	if (spriv->quirks & PL2303_QUIRK_UART_STATE_IDX0)
		status_idx = 0;

	if (actual_length < status_idx + 1)
		return;

	status = data[status_idx];

	/* Save off the uart status for others to look at */
	spin_lock_irqsave(&priv->lock, flags);
	delta = priv->line_status ^ status;
	priv->line_status = status;
	spin_unlock_irqrestore(&priv->lock, flags);

	if (status & UART_BREAK_ERROR)
		usb_serial_handle_break(port);

	if (delta & UART_STATE_MSR_MASK) {
		if (delta & UART_CTS)
			port->icount.cts++;
		if (delta & UART_DSR)
			port->icount.dsr++;
		if (delta & UART_RING)
			port->icount.rng++;
		if (delta & UART_DCD) {
			port->icount.dcd++;
			tty = tty_port_tty_get(&port->port);
			if (tty) {
				usb_serial_handle_dcd_change(port, tty,
							status & UART_DCD);
				tty_kref_put(tty);
			}
		}

		wake_up_interruptible(&port->port.delta_msr_wait);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void pl2303_read_int_callback(struct urb *urb)
{
	struct usb_serial_port *port =  urb->context;
	unsigned char *data = urb->transfer_buffer;
	unsigned int actual_length = urb->actual_length;
	int status = urb->status;
	int retval;

	switch (status) {
	case 0:
		/* success */
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		/* this urb is terminated, clean up */
		dev_dbg(&port->dev, "%s - urb shutting down with status: %d\n",
			__func__, status);
		return;
	default:
		dev_dbg(&port->dev, "%s - nonzero urb status received: %d\n",
			__func__, status);
		goto exit;
	}

	usb_serial_debug_data(&port->dev, __func__,
			      urb->actual_length, urb->transfer_buffer);

	pl2303_update_line_status(port, data, actual_length);

exit:
	retval = usb_submit_urb(urb, GFP_ATOMIC);
<<<<<<< HEAD
	if (retval)
		dev_err(&port->dev,
			"%s - usb_submit_urb failed with result %d\n",
			__func__, retval);
=======
	if (retval) {
		dev_err(&port->dev,
			"%s - usb_submit_urb failed with result %d\n",
			__func__, retval);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void pl2303_process_read_urb(struct urb *urb)
{
	struct usb_serial_port *port = urb->context;
	struct pl2303_private *priv = usb_get_serial_port_data(port);
	unsigned char *data = urb->transfer_buffer;
	char tty_flag = TTY_NORMAL;
	unsigned long flags;
	u8 line_status;
	int i;

	/* update line status */
	spin_lock_irqsave(&priv->lock, flags);
	line_status = priv->line_status;
	priv->line_status &= ~UART_STATE_TRANSIENT_MASK;
	spin_unlock_irqrestore(&priv->lock, flags);
<<<<<<< HEAD
	wake_up_interruptible(&port->port.delta_msr_wait);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!urb->actual_length)
		return;

<<<<<<< HEAD
	/* break takes precedence over parity, */
	/* which takes precedence over framing errors */
=======
	/*
	 * Break takes precedence over parity, which takes precedence over
	 * framing errors.
	 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (line_status & UART_BREAK_ERROR)
		tty_flag = TTY_BREAK;
	else if (line_status & UART_PARITY_ERROR)
		tty_flag = TTY_PARITY;
	else if (line_status & UART_FRAME_ERROR)
		tty_flag = TTY_FRAME;
<<<<<<< HEAD
	dev_dbg(&port->dev, "%s - tty_flag = %d\n", __func__, tty_flag);

=======

	if (tty_flag != TTY_NORMAL)
		dev_dbg(&port->dev, "%s - tty_flag = %d\n", __func__,
								tty_flag);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* overrun is special, not associated with a char */
	if (line_status & UART_OVERRUN_ERROR)
		tty_insert_flip_char(&port->port, 0, TTY_OVERRUN);

	if (port->port.console && port->sysrq) {
		for (i = 0; i < urb->actual_length; ++i)
			if (!usb_serial_handle_sysrq_char(port, data[i]))
				tty_insert_flip_char(&port->port, data[i],
						tty_flag);
	} else {
		tty_insert_flip_string_fixed_flag(&port->port, data, tty_flag,
							urb->actual_length);
	}

	tty_flip_buffer_push(&port->port);
}

<<<<<<< HEAD
/* All of the device info needed for the PL2303 SIO serial converter */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct usb_serial_driver pl2303_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"pl2303",
	},
	.id_table =		id_table,
	.num_ports =		1,
	.bulk_in_size =		256,
	.bulk_out_size =	256,
	.open =			pl2303_open,
	.close =		pl2303_close,
<<<<<<< HEAD
	.dtr_rts = 		pl2303_dtr_rts,
=======
	.dtr_rts =		pl2303_dtr_rts,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.carrier_raised =	pl2303_carrier_raised,
	.ioctl =		pl2303_ioctl,
	.break_ctl =		pl2303_break_ctl,
	.set_termios =		pl2303_set_termios,
	.tiocmget =		pl2303_tiocmget,
	.tiocmset =		pl2303_tiocmset,
<<<<<<< HEAD
	.tiocmiwait =		pl2303_tiocmiwait,
	.process_read_urb =	pl2303_process_read_urb,
	.read_int_callback =	pl2303_read_int_callback,
=======
	.tiocmiwait =		usb_serial_generic_tiocmiwait,
	.process_read_urb =	pl2303_process_read_urb,
	.read_int_callback =	pl2303_read_int_callback,
	.probe =		pl2303_probe,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.attach =		pl2303_startup,
	.release =		pl2303_release,
	.port_probe =		pl2303_port_probe,
	.port_remove =		pl2303_port_remove,
};

static struct usb_serial_driver * const serial_drivers[] = {
	&pl2303_device, NULL
};

module_usb_serial_driver(serial_drivers, id_table);

<<<<<<< HEAD
MODULE_DESCRIPTION(DRIVER_DESC);
=======
MODULE_DESCRIPTION("Prolific PL2303 USB to serial adaptor driver");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_LICENSE("GPL");
