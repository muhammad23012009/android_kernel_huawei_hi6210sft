/*
 * soc-io.c  --  ASoC register I/O helpers
 *
 * Copyright 2009-2011 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#include <linux/i2c.h>
#include <linux/spi/spi.h>
#include <linux/regmap.h>
#include <linux/export.h>
#include <sound/soc.h>

<<<<<<< HEAD
#include <trace/events/asoc.h>

#ifdef CONFIG_REGMAP
static int hw_write(struct snd_soc_codec *codec, unsigned int reg,
		    unsigned int value)
{
	int ret;

	if (!snd_soc_codec_volatile_register(codec, reg) &&
	    reg < codec->driver->reg_cache_size &&
	    !codec->cache_bypass) {
		ret = snd_soc_cache_write(codec, reg, value);
		if (ret < 0)
			return -1;
	}

	if (codec->cache_only) {
		codec->cache_sync = 1;
		return 0;
	}

	return regmap_write(codec->control_data, reg, value);
}

static unsigned int hw_read(struct snd_soc_codec *codec, unsigned int reg)
{
	int ret;
	unsigned int val;

	if (reg >= codec->driver->reg_cache_size ||
	    snd_soc_codec_volatile_register(codec, reg) ||
	    codec->cache_bypass) {
		if (codec->cache_only)
			return -1;

		ret = regmap_read(codec->control_data, reg, &val);
		if (ret == 0)
			return val;
		else
			return -1;
	}

	ret = snd_soc_cache_read(codec, reg, &val);
	if (ret < 0)
		return -1;
	return val;
}

/* Primitive bulk write support for soc-cache.  The data pointed to by
 * `data' needs to already be in the form the hardware expects.  Any
 * data written through this function will not go through the cache as
 * it only handles writing to volatile or out of bounds registers.
 *
 * This is currently only supported for devices using the regmap API
 * wrappers.
 */
static int snd_soc_hw_bulk_write_raw(struct snd_soc_codec *codec,
				     unsigned int reg,
				     const void *data, size_t len)
{
	/* To ensure that we don't get out of sync with the cache, check
	 * whether the base register is volatile or if we've directly asked
	 * to bypass the cache.  Out of bounds registers are considered
	 * volatile.
	 */
	if (!codec->cache_bypass
	    && !snd_soc_codec_volatile_register(codec, reg)
	    && reg < codec->driver->reg_cache_size)
		return -EINVAL;

	return regmap_raw_write(codec->control_data, reg, data, len);
}

/**
 * snd_soc_codec_set_cache_io: Set up standard I/O functions.
 *
 * @codec: CODEC to configure.
 * @addr_bits: Number of bits of register address data.
 * @data_bits: Number of bits of data per register.
 * @control: Control bus used.
 *
 * Register formats are frequently shared between many I2C and SPI
 * devices.  In order to promote code reuse the ASoC core provides
 * some standard implementations of CODEC read and write operations
 * which can be set up using this function.
 *
 * The caller is responsible for allocating and initialising the
 * actual cache.
 *
 * Note that at present this code cannot be used by CODECs with
 * volatile registers.
 */
int snd_soc_codec_set_cache_io(struct snd_soc_codec *codec,
			       int addr_bits, int data_bits,
			       enum snd_soc_control_type control)
{
	struct regmap_config config;
	int ret;

	memset(&config, 0, sizeof(config));
	codec->write = hw_write;
	codec->read = hw_read;
	codec->bulk_write_raw = snd_soc_hw_bulk_write_raw;

	config.reg_bits = addr_bits;
	config.val_bits = data_bits;

	switch (control) {
#if defined(CONFIG_REGMAP_I2C) || defined(CONFIG_REGMAP_I2C_MODULE)
	case SND_SOC_I2C:
		codec->control_data = regmap_init_i2c(to_i2c_client(codec->dev),
						      &config);
		break;
#endif

#if defined(CONFIG_REGMAP_SPI) || defined(CONFIG_REGMAP_SPI_MODULE)
	case SND_SOC_SPI:
		codec->control_data = regmap_init_spi(to_spi_device(codec->dev),
						      &config);
		break;
#endif

	case SND_SOC_REGMAP:
		/* Device has made its own regmap arrangements */
		codec->using_regmap = true;
		if (!codec->control_data)
			codec->control_data = dev_get_regmap(codec->dev, NULL);

		if (codec->control_data) {
			ret = regmap_get_val_bytes(codec->control_data);
			/* Errors are legitimate for non-integer byte
			 * multiples */
			if (ret > 0)
				codec->val_bytes = ret;
		}
		break;

	default:
		return -EINVAL;
	}

	return PTR_RET(codec->control_data);
}
EXPORT_SYMBOL_GPL(snd_soc_codec_set_cache_io);
#else
int snd_soc_codec_set_cache_io(struct snd_soc_codec *codec,
			       int addr_bits, int data_bits,
			       enum snd_soc_control_type control)
{
	return -ENOTSUPP;
}
EXPORT_SYMBOL_GPL(snd_soc_codec_set_cache_io);
#endif
=======
/**
 * snd_soc_component_read() - Read register value
 * @component: Component to read from
 * @reg: Register to read
 * @val: Pointer to where the read value is stored
 *
 * Return: 0 on success, a negative error code otherwise.
 */
int snd_soc_component_read(struct snd_soc_component *component,
	unsigned int reg, unsigned int *val)
{
	int ret;

	if (component->regmap)
		ret = regmap_read(component->regmap, reg, val);
	else if (component->read)
		ret = component->read(component, reg, val);
	else
		ret = -EIO;

	return ret;
}
EXPORT_SYMBOL_GPL(snd_soc_component_read);

/**
 * snd_soc_component_write() - Write register value
 * @component: Component to write to
 * @reg: Register to write
 * @val: Value to write to the register
 *
 * Return: 0 on success, a negative error code otherwise.
 */
int snd_soc_component_write(struct snd_soc_component *component,
	unsigned int reg, unsigned int val)
{
	if (component->regmap)
		return regmap_write(component->regmap, reg, val);
	else if (component->write)
		return component->write(component, reg, val);
	else
		return -EIO;
}
EXPORT_SYMBOL_GPL(snd_soc_component_write);

static int snd_soc_component_update_bits_legacy(
	struct snd_soc_component *component, unsigned int reg,
	unsigned int mask, unsigned int val, bool *change)
{
	unsigned int old, new;
	int ret;

	if (!component->read || !component->write)
		return -EIO;

	mutex_lock(&component->io_mutex);

	ret = component->read(component, reg, &old);
	if (ret < 0)
		goto out_unlock;

	new = (old & ~mask) | (val & mask);
	*change = old != new;
	if (*change)
		ret = component->write(component, reg, new);
out_unlock:
	mutex_unlock(&component->io_mutex);

	return ret;
}

/**
 * snd_soc_component_update_bits() - Perform read/modify/write cycle
 * @component: Component to update
 * @reg: Register to update
 * @mask: Mask that specifies which bits to update
 * @val: New value for the bits specified by mask
 *
 * Return: 1 if the operation was successful and the value of the register
 * changed, 0 if the operation was successful, but the value did not change.
 * Returns a negative error code otherwise.
 */
int snd_soc_component_update_bits(struct snd_soc_component *component,
	unsigned int reg, unsigned int mask, unsigned int val)
{
	bool change;
	int ret;

	if (component->regmap)
		ret = regmap_update_bits_check(component->regmap, reg, mask,
			val, &change);
	else
		ret = snd_soc_component_update_bits_legacy(component, reg,
			mask, val, &change);

	if (ret < 0)
		return ret;
	return change;
}
EXPORT_SYMBOL_GPL(snd_soc_component_update_bits);

/**
 * snd_soc_component_update_bits_async() - Perform asynchronous
 *  read/modify/write cycle
 * @component: Component to update
 * @reg: Register to update
 * @mask: Mask that specifies which bits to update
 * @val: New value for the bits specified by mask
 *
 * This function is similar to snd_soc_component_update_bits(), but the update
 * operation is scheduled asynchronously. This means it may not be completed
 * when the function returns. To make sure that all scheduled updates have been
 * completed snd_soc_component_async_complete() must be called.
 *
 * Return: 1 if the operation was successful and the value of the register
 * changed, 0 if the operation was successful, but the value did not change.
 * Returns a negative error code otherwise.
 */
int snd_soc_component_update_bits_async(struct snd_soc_component *component,
	unsigned int reg, unsigned int mask, unsigned int val)
{
	bool change;
	int ret;

	if (component->regmap)
		ret = regmap_update_bits_check_async(component->regmap, reg,
			mask, val, &change);
	else
		ret = snd_soc_component_update_bits_legacy(component, reg,
			mask, val, &change);

	if (ret < 0)
		return ret;
	return change;
}
EXPORT_SYMBOL_GPL(snd_soc_component_update_bits_async);

/**
 * snd_soc_component_async_complete() - Ensure asynchronous I/O has completed
 * @component: Component for which to wait
 *
 * This function blocks until all asynchronous I/O which has previously been
 * scheduled using snd_soc_component_update_bits_async() has completed.
 */
void snd_soc_component_async_complete(struct snd_soc_component *component)
{
	if (component->regmap)
		regmap_async_complete(component->regmap);
}
EXPORT_SYMBOL_GPL(snd_soc_component_async_complete);

/**
 * snd_soc_component_test_bits - Test register for change
 * @component: component
 * @reg: Register to test
 * @mask: Mask that specifies which bits to test
 * @value: Value to test against
 *
 * Tests a register with a new value and checks if the new value is
 * different from the old value.
 *
 * Return: 1 for change, otherwise 0.
 */
int snd_soc_component_test_bits(struct snd_soc_component *component,
	unsigned int reg, unsigned int mask, unsigned int value)
{
	unsigned int old, new;
	int ret;

	ret = snd_soc_component_read(component, reg, &old);
	if (ret < 0)
		return ret;
	new = (old & ~mask) | value;
	return old != new;
}
EXPORT_SYMBOL_GPL(snd_soc_component_test_bits);

unsigned int snd_soc_read(struct snd_soc_codec *codec, unsigned int reg)
{
	unsigned int val;
	int ret;

	ret = snd_soc_component_read(&codec->component, reg, &val);
	if (ret < 0)
		return -1;

	return val;
}
EXPORT_SYMBOL_GPL(snd_soc_read);

int snd_soc_write(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int val)
{
	return snd_soc_component_write(&codec->component, reg, val);
}
EXPORT_SYMBOL_GPL(snd_soc_write);

/**
 * snd_soc_update_bits - update codec register bits
 * @codec: audio codec
 * @reg: codec register
 * @mask: register mask
 * @value: new value
 *
 * Writes new register value.
 *
 * Returns 1 for change, 0 for no change, or negative error code.
 */
int snd_soc_update_bits(struct snd_soc_codec *codec, unsigned int reg,
				unsigned int mask, unsigned int value)
{
	return snd_soc_component_update_bits(&codec->component, reg, mask,
		value);
}
EXPORT_SYMBOL_GPL(snd_soc_update_bits);

/**
 * snd_soc_test_bits - test register for change
 * @codec: audio codec
 * @reg: codec register
 * @mask: register mask
 * @value: new value
 *
 * Tests a register with a new value and checks if the new value is
 * different from the old value.
 *
 * Returns 1 for change else 0.
 */
int snd_soc_test_bits(struct snd_soc_codec *codec, unsigned int reg,
				unsigned int mask, unsigned int value)
{
	return snd_soc_component_test_bits(&codec->component, reg, mask, value);
}
EXPORT_SYMBOL_GPL(snd_soc_test_bits);

int snd_soc_platform_read(struct snd_soc_platform *platform,
					unsigned int reg)
{
	unsigned int val;
	int ret;

	ret = snd_soc_component_read(&platform->component, reg, &val);
	if (ret < 0)
		return -1;

	return val;
}
EXPORT_SYMBOL_GPL(snd_soc_platform_read);

int snd_soc_platform_write(struct snd_soc_platform *platform,
					 unsigned int reg, unsigned int val)
{
	return snd_soc_component_write(&platform->component, reg, val);
}
EXPORT_SYMBOL_GPL(snd_soc_platform_write);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
