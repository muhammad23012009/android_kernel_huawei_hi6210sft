/*
 * at25.c -- support most SPI EEPROMs, such as Atmel AT25 models
 *
 * Copyright (C) 2006 David Brownell
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/sched.h>

<<<<<<< HEAD
#include <linux/spi/spi.h>
#include <linux/spi/eeprom.h>
#include <linux/of.h>
=======
#include <linux/nvmem-provider.h>
#include <linux/spi/spi.h>
#include <linux/spi/eeprom.h>
#include <linux/property.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * NOTE: this is an *EEPROM* driver.  The vagaries of product naming
 * mean that some AT25 products are EEPROMs, and others are FLASH.
 * Handle FLASH chips with the drivers/mtd/devices/m25p80.c driver,
 * not this one!
 */

struct at25_data {
	struct spi_device	*spi;
<<<<<<< HEAD
	struct memory_accessor	mem;
	struct mutex		lock;
	struct spi_eeprom	chip;
	struct bin_attribute	bin;
	unsigned		addrlen;
=======
	struct mutex		lock;
	struct spi_eeprom	chip;
	unsigned		addrlen;
	struct nvmem_config	nvmem_config;
	struct nvmem_device	*nvmem;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#define	AT25_WREN	0x06		/* latch the write enable */
#define	AT25_WRDI	0x04		/* reset the write enable */
#define	AT25_RDSR	0x05		/* read status register */
#define	AT25_WRSR	0x01		/* write status register */
#define	AT25_READ	0x03		/* read byte(s) */
#define	AT25_WRITE	0x02		/* write byte(s)/sector */

#define	AT25_SR_nRDY	0x01		/* nRDY = write-in-progress */
#define	AT25_SR_WEN	0x02		/* write enable (latched) */
#define	AT25_SR_BP0	0x04		/* BP for software writeprotect */
#define	AT25_SR_BP1	0x08
#define	AT25_SR_WPEN	0x80		/* writeprotect enable */

#define	AT25_INSTR_BIT3	0x08		/* Additional address bit in instr */

#define EE_MAXADDRLEN	3		/* 24 bit addresses, up to 2 MBytes */

/* Specs often allow 5 msec for a page write, sometimes 20 msec;
 * it's important to recover from write timeouts.
 */
#define	EE_TIMEOUT	25

/*-------------------------------------------------------------------------*/

#define	io_limit	PAGE_SIZE	/* bytes */

<<<<<<< HEAD
static ssize_t
at25_ee_read(
	struct at25_data	*at25,
	char			*buf,
	unsigned		offset,
	size_t			count
)
{
=======
static int at25_ee_read(void *priv, unsigned int offset,
			void *val, size_t count)
{
	struct at25_data *at25 = priv;
	char *buf = val;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u8			command[EE_MAXADDRLEN + 1];
	u8			*cp;
	ssize_t			status;
	struct spi_transfer	t[2];
	struct spi_message	m;
	u8			instr;

<<<<<<< HEAD
	if (unlikely(offset >= at25->bin.size))
		return 0;
	if ((offset + count) > at25->bin.size)
		count = at25->bin.size - offset;
	if (unlikely(!count))
		return count;
=======
	if (unlikely(offset >= at25->chip.byte_len))
		return -EINVAL;
	if ((offset + count) > at25->chip.byte_len)
		count = at25->chip.byte_len - offset;
	if (unlikely(!count))
		return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	cp = command;

	instr = AT25_READ;
	if (at25->chip.flags & EE_INSTR_BIT3_IS_ADDR)
		if (offset >= (1U << (at25->addrlen * 8)))
			instr |= AT25_INSTR_BIT3;
	*cp++ = instr;

	/* 8/16/24-bit address is written MSB first */
	switch (at25->addrlen) {
	default:	/* case 3 */
		*cp++ = offset >> 16;
	case 2:
		*cp++ = offset >> 8;
	case 1:
	case 0:	/* can't happen: for better codegen */
		*cp++ = offset >> 0;
	}

	spi_message_init(&m);
	memset(t, 0, sizeof t);

	t[0].tx_buf = command;
	t[0].len = at25->addrlen + 1;
	spi_message_add_tail(&t[0], &m);

	t[1].rx_buf = buf;
	t[1].len = count;
	spi_message_add_tail(&t[1], &m);

	mutex_lock(&at25->lock);

	/* Read it all at once.
	 *
	 * REVISIT that's potentially a problem with large chips, if
	 * other devices on the bus need to be accessed regularly or
	 * this chip is clocked very slowly
	 */
	status = spi_sync(at25->spi, &m);
<<<<<<< HEAD
	dev_dbg(&at25->spi->dev,
		"read %Zd bytes at %d --> %d\n",
		count, offset, (int) status);

	mutex_unlock(&at25->lock);
	return status ? status : count;
}

static ssize_t
at25_bin_read(struct file *filp, struct kobject *kobj,
	      struct bin_attribute *bin_attr,
	      char *buf, loff_t off, size_t count)
{
	struct device		*dev;
	struct at25_data	*at25;

	dev = container_of(kobj, struct device, kobj);
	at25 = dev_get_drvdata(dev);

	return at25_ee_read(at25, buf, off, count);
}


static ssize_t
at25_ee_write(struct at25_data *at25, const char *buf, loff_t off,
	      size_t count)
{
	ssize_t			status = 0;
	unsigned		written = 0;
	unsigned		buf_size;
	u8			*bounce;

	if (unlikely(off >= at25->bin.size))
		return -EFBIG;
	if ((off + count) > at25->bin.size)
		count = at25->bin.size - off;
	if (unlikely(!count))
		return count;
=======
	dev_dbg(&at25->spi->dev, "read %zu bytes at %d --> %zd\n",
		count, offset, status);

	mutex_unlock(&at25->lock);
	return status;
}

static int at25_ee_write(void *priv, unsigned int off, void *val, size_t count)
{
	struct at25_data *at25 = priv;
	const char *buf = val;
	int			status = 0;
	unsigned		buf_size;
	u8			*bounce;

	if (unlikely(off >= at25->chip.byte_len))
		return -EFBIG;
	if ((off + count) > at25->chip.byte_len)
		count = at25->chip.byte_len - off;
	if (unlikely(!count))
		return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Temp buffer starts with command and address */
	buf_size = at25->chip.page_size;
	if (buf_size > io_limit)
		buf_size = io_limit;
	bounce = kmalloc(buf_size + at25->addrlen + 1, GFP_KERNEL);
	if (!bounce)
		return -ENOMEM;

	/* For write, rollover is within the page ... so we write at
	 * most one page, then manually roll over to the next page.
	 */
	mutex_lock(&at25->lock);
	do {
		unsigned long	timeout, retries;
		unsigned	segment;
		unsigned	offset = (unsigned) off;
		u8		*cp = bounce;
		int		sr;
		u8		instr;

		*cp = AT25_WREN;
		status = spi_write(at25->spi, cp, 1);
		if (status < 0) {
<<<<<<< HEAD
			dev_dbg(&at25->spi->dev, "WREN --> %d\n",
					(int) status);
=======
			dev_dbg(&at25->spi->dev, "WREN --> %d\n", status);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		}

		instr = AT25_WRITE;
		if (at25->chip.flags & EE_INSTR_BIT3_IS_ADDR)
			if (offset >= (1U << (at25->addrlen * 8)))
				instr |= AT25_INSTR_BIT3;
		*cp++ = instr;

		/* 8/16/24-bit address is written MSB first */
		switch (at25->addrlen) {
		default:	/* case 3 */
			*cp++ = offset >> 16;
		case 2:
			*cp++ = offset >> 8;
		case 1:
		case 0:	/* can't happen: for better codegen */
			*cp++ = offset >> 0;
		}

		/* Write as much of a page as we can */
		segment = buf_size - (offset % buf_size);
		if (segment > count)
			segment = count;
		memcpy(cp, buf, segment);
		status = spi_write(at25->spi, bounce,
				segment + at25->addrlen + 1);
<<<<<<< HEAD
		dev_dbg(&at25->spi->dev,
				"write %u bytes at %u --> %d\n",
				segment, offset, (int) status);
=======
		dev_dbg(&at25->spi->dev, "write %u bytes at %u --> %d\n",
			segment, offset, status);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (status < 0)
			break;

		/* REVISIT this should detect (or prevent) failed writes
		 * to readonly sections of the EEPROM...
		 */

		/* Wait for non-busy status */
		timeout = jiffies + msecs_to_jiffies(EE_TIMEOUT);
		retries = 0;
		do {

			sr = spi_w8r8(at25->spi, AT25_RDSR);
			if (sr < 0 || (sr & AT25_SR_nRDY)) {
				dev_dbg(&at25->spi->dev,
					"rdsr --> %d (%02x)\n", sr, sr);
				/* at HZ=100, this is sloooow */
				msleep(1);
				continue;
			}
			if (!(sr & AT25_SR_nRDY))
				break;
		} while (retries++ < 3 || time_before_eq(jiffies, timeout));

		if ((sr < 0) || (sr & AT25_SR_nRDY)) {
			dev_err(&at25->spi->dev,
<<<<<<< HEAD
				"write %d bytes offset %d, "
				"timeout after %u msecs\n",
=======
				"write %u bytes offset %u, timeout after %u msecs\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				segment, offset,
				jiffies_to_msecs(jiffies -
					(timeout - EE_TIMEOUT)));
			status = -ETIMEDOUT;
			break;
		}

		off += segment;
		buf += segment;
		count -= segment;
<<<<<<< HEAD
		written += segment;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	} while (count > 0);

	mutex_unlock(&at25->lock);

	kfree(bounce);
<<<<<<< HEAD
	return written ? written : status;
}

static ssize_t
at25_bin_write(struct file *filp, struct kobject *kobj,
	       struct bin_attribute *bin_attr,
	       char *buf, loff_t off, size_t count)
{
	struct device		*dev;
	struct at25_data	*at25;

	dev = container_of(kobj, struct device, kobj);
	at25 = dev_get_drvdata(dev);

	return at25_ee_write(at25, buf, off, count);
}

/*-------------------------------------------------------------------------*/

/* Let in-kernel code access the eeprom data. */

static ssize_t at25_mem_read(struct memory_accessor *mem, char *buf,
			 off_t offset, size_t count)
{
	struct at25_data *at25 = container_of(mem, struct at25_data, mem);

	return at25_ee_read(at25, buf, offset, count);
}

static ssize_t at25_mem_write(struct memory_accessor *mem, const char *buf,
			  off_t offset, size_t count)
{
	struct at25_data *at25 = container_of(mem, struct at25_data, mem);

	return at25_ee_write(at25, buf, offset, count);
=======
	return status;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
static int at25_np_to_chip(struct device *dev,
			   struct device_node *np,
			   struct spi_eeprom *chip)
=======
static int at25_fw_to_chip(struct device *dev, struct spi_eeprom *chip)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	u32 val;

	memset(chip, 0, sizeof(*chip));
<<<<<<< HEAD
	strncpy(chip->name, np->name, sizeof(chip->name));

	if (of_property_read_u32(np, "size", &val) == 0 ||
	    of_property_read_u32(np, "at25,byte-len", &val) == 0) {
=======
	strncpy(chip->name, "at25", sizeof(chip->name));

	if (device_property_read_u32(dev, "size", &val) == 0 ||
	    device_property_read_u32(dev, "at25,byte-len", &val) == 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		chip->byte_len = val;
	} else {
		dev_err(dev, "Error: missing \"size\" property\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	if (of_property_read_u32(np, "pagesize", &val) == 0 ||
	    of_property_read_u32(np, "at25,page-size", &val) == 0) {
=======
	if (device_property_read_u32(dev, "pagesize", &val) == 0 ||
	    device_property_read_u32(dev, "at25,page-size", &val) == 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		chip->page_size = (u16)val;
	} else {
		dev_err(dev, "Error: missing \"pagesize\" property\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	if (of_property_read_u32(np, "at25,addr-mode", &val) == 0) {
		chip->flags = (u16)val;
	} else {
		if (of_property_read_u32(np, "address-width", &val)) {
=======
	if (device_property_read_u32(dev, "at25,addr-mode", &val) == 0) {
		chip->flags = (u16)val;
	} else {
		if (device_property_read_u32(dev, "address-width", &val)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			dev_err(dev,
				"Error: missing \"address-width\" property\n");
			return -ENODEV;
		}
		switch (val) {
		case 8:
			chip->flags |= EE_ADDR1;
			break;
		case 16:
			chip->flags |= EE_ADDR2;
			break;
		case 24:
			chip->flags |= EE_ADDR3;
			break;
		default:
			dev_err(dev,
				"Error: bad \"address-width\" property: %u\n",
				val);
			return -ENODEV;
		}
<<<<<<< HEAD
		if (of_find_property(np, "read-only", NULL))
=======
		if (device_property_present(dev, "read-only"))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			chip->flags |= EE_READONLY;
	}
	return 0;
}

static int at25_probe(struct spi_device *spi)
{
	struct at25_data	*at25 = NULL;
	struct spi_eeprom	chip;
<<<<<<< HEAD
	struct device_node	*np = spi->dev.of_node;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int			err;
	int			sr;
	int			addrlen;

	/* Chip description */
	if (!spi->dev.platform_data) {
<<<<<<< HEAD
		if (np) {
			err = at25_np_to_chip(&spi->dev, np, &chip);
			if (err)
				goto fail;
		} else {
			dev_err(&spi->dev, "Error: no chip description\n");
			err = -ENODEV;
			goto fail;
		}
=======
		err = at25_fw_to_chip(&spi->dev, &chip);
		if (err)
			return err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else
		chip = *(struct spi_eeprom *)spi->dev.platform_data;

	/* For now we only support 8/16/24 bit addressing */
	if (chip.flags & EE_ADDR1)
		addrlen = 1;
	else if (chip.flags & EE_ADDR2)
		addrlen = 2;
	else if (chip.flags & EE_ADDR3)
		addrlen = 3;
	else {
		dev_dbg(&spi->dev, "unsupported address type\n");
<<<<<<< HEAD
		err = -EINVAL;
		goto fail;
=======
		return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Ping the chip ... the status register is pretty portable,
	 * unlike probing manufacturer IDs.  We do expect that system
	 * firmware didn't write it in the past few milliseconds!
	 */
	sr = spi_w8r8(spi, AT25_RDSR);
	if (sr < 0 || sr & AT25_SR_nRDY) {
		dev_dbg(&spi->dev, "rdsr --> %d (%02x)\n", sr, sr);
<<<<<<< HEAD
		err = -ENXIO;
		goto fail;
	}

	if (!(at25 = kzalloc(sizeof *at25, GFP_KERNEL))) {
		err = -ENOMEM;
		goto fail;
	}

	mutex_init(&at25->lock);
	at25->chip = chip;
	at25->spi = spi_dev_get(spi);
	spi_set_drvdata(spi, at25);
	at25->addrlen = addrlen;

	/* Export the EEPROM bytes through sysfs, since that's convenient.
	 * And maybe to other kernel code; it might hold a board's Ethernet
	 * address, or board-specific calibration data generated on the
	 * manufacturing floor.
	 *
	 * Default to root-only access to the data; EEPROMs often hold data
	 * that's sensitive for read and/or write, like ethernet addresses,
	 * security codes, board-specific manufacturing calibrations, etc.
	 */
	sysfs_bin_attr_init(&at25->bin);
	at25->bin.attr.name = "eeprom";
	at25->bin.attr.mode = S_IRUSR;
	at25->bin.read = at25_bin_read;
	at25->mem.read = at25_mem_read;

	at25->bin.size = at25->chip.byte_len;
	if (!(chip.flags & EE_READONLY)) {
		at25->bin.write = at25_bin_write;
		at25->bin.attr.mode |= S_IWUSR;
		at25->mem.write = at25_mem_write;
	}

	err = sysfs_create_bin_file(&spi->dev.kobj, &at25->bin);
	if (err)
		goto fail;

	if (chip.setup)
		chip.setup(&at25->mem, chip.context);

	dev_info(&spi->dev, "%Zd %s %s eeprom%s, pagesize %u\n",
		(at25->bin.size < 1024)
			? at25->bin.size
			: (at25->bin.size / 1024),
		(at25->bin.size < 1024) ? "Byte" : "KByte",
=======
		return -ENXIO;
	}

	at25 = devm_kzalloc(&spi->dev, sizeof(struct at25_data), GFP_KERNEL);
	if (!at25)
		return -ENOMEM;

	mutex_init(&at25->lock);
	at25->chip = chip;
	at25->spi = spi;
	spi_set_drvdata(spi, at25);
	at25->addrlen = addrlen;

	at25->nvmem_config.name = dev_name(&spi->dev);
	at25->nvmem_config.dev = &spi->dev;
	at25->nvmem_config.read_only = chip.flags & EE_READONLY;
	at25->nvmem_config.root_only = true;
	at25->nvmem_config.owner = THIS_MODULE;
	at25->nvmem_config.compat = true;
	at25->nvmem_config.base_dev = &spi->dev;
	at25->nvmem_config.reg_read = at25_ee_read;
	at25->nvmem_config.reg_write = at25_ee_write;
	at25->nvmem_config.priv = at25;
	at25->nvmem_config.stride = 1;
	at25->nvmem_config.word_size = 1;
	at25->nvmem_config.size = chip.byte_len;

	at25->nvmem = nvmem_register(&at25->nvmem_config);
	if (IS_ERR(at25->nvmem))
		return PTR_ERR(at25->nvmem);

	dev_info(&spi->dev, "%d %s %s eeprom%s, pagesize %u\n",
		(chip.byte_len < 1024) ? chip.byte_len : (chip.byte_len / 1024),
		(chip.byte_len < 1024) ? "Byte" : "KByte",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		at25->chip.name,
		(chip.flags & EE_READONLY) ? " (readonly)" : "",
		at25->chip.page_size);
	return 0;
<<<<<<< HEAD
fail:
	dev_dbg(&spi->dev, "probe err %d\n", err);
	kfree(at25);
	return err;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int at25_remove(struct spi_device *spi)
{
	struct at25_data	*at25;

	at25 = spi_get_drvdata(spi);
<<<<<<< HEAD
	sysfs_remove_bin_file(&spi->dev.kobj, &at25->bin);
	kfree(at25);
=======
	nvmem_unregister(at25->nvmem);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
static struct spi_driver at25_driver = {
	.driver = {
		.name		= "at25",
		.owner		= THIS_MODULE,
=======
static const struct of_device_id at25_of_match[] = {
	{ .compatible = "atmel,at25", },
	{ }
};
MODULE_DEVICE_TABLE(of, at25_of_match);

static struct spi_driver at25_driver = {
	.driver = {
		.name		= "at25",
		.of_match_table = at25_of_match,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	.probe		= at25_probe,
	.remove		= at25_remove,
};

module_spi_driver(at25_driver);

MODULE_DESCRIPTION("Driver for most SPI EEPROMs");
MODULE_AUTHOR("David Brownell");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:at25");
