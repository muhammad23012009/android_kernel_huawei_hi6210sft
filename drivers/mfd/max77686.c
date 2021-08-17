/*
<<<<<<< HEAD
 * max77686.c - mfd core driver for the Maxim 77686
 *
 * Copyright (C) 2012 Samsung Electronics
 * Chiwoong Byun <woong.byun@smasung.com>
=======
 * max77686.c - mfd core driver for the Maxim 77686/802
 *
 * Copyright (C) 2012 Samsung Electronics
 * Chiwoong Byun <woong.byun@samsung.com>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Jonghwa Lee <jonghwa3.lee@samsung.com>
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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * This driver is based on max8997.c
 */

#include <linux/export.h>
#include <linux/slab.h>
#include <linux/i2c.h>
<<<<<<< HEAD
=======
#include <linux/irq.h>
#include <linux/interrupt.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/pm_runtime.h>
#include <linux/module.h>
#include <linux/mfd/core.h>
#include <linux/mfd/max77686.h>
#include <linux/mfd/max77686-private.h>
#include <linux/err.h>
<<<<<<< HEAD

#define I2C_ADDR_RTC	(0x0C >> 1)

static struct mfd_cell max77686_devs[] = {
	{ .name = "max77686-pmic", },
	{ .name = "max77686-rtc", },
};

static struct regmap_config max77686_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
};

#ifdef CONFIG_OF
static struct of_device_id max77686_pmic_dt_match[] = {
	{.compatible = "maxim,max77686", .data = NULL},
	{},
};

static struct max77686_platform_data *max77686_i2c_parse_dt_pdata(struct device
								  *dev)
{
	struct max77686_platform_data *pd;

	pd = devm_kzalloc(dev, sizeof(*pd), GFP_KERNEL);
	if (!pd) {
		dev_err(dev, "could not allocate memory for pdata\n");
		return NULL;
	}

	dev->platform_data = pd;
	return pd;
}
#else
static struct max77686_platform_data *max77686_i2c_parse_dt_pdata(struct device
								  *dev)
{
	return 0;
}
#endif
=======
#include <linux/of.h>

static const struct mfd_cell max77686_devs[] = {
	{ .name = "max77686-pmic", },
	{ .name = "max77686-rtc", },
	{ .name = "max77686-clk", },
};

static const struct mfd_cell max77802_devs[] = {
	{ .name = "max77802-pmic", },
	{ .name = "max77802-clk", },
	{ .name = "max77802-rtc", },
};

static bool max77802_pmic_is_accessible_reg(struct device *dev,
					    unsigned int reg)
{
	return reg < MAX77802_REG_PMIC_END;
}

static bool max77802_rtc_is_accessible_reg(struct device *dev,
					   unsigned int reg)
{
	return (reg >= MAX77802_RTC_INT && reg < MAX77802_RTC_END);
}

static bool max77802_is_accessible_reg(struct device *dev, unsigned int reg)
{
	return (max77802_pmic_is_accessible_reg(dev, reg) ||
		max77802_rtc_is_accessible_reg(dev, reg));
}

static bool max77802_pmic_is_precious_reg(struct device *dev, unsigned int reg)
{
	return (reg == MAX77802_REG_INTSRC || reg == MAX77802_REG_INT1 ||
		reg == MAX77802_REG_INT2);
}

static bool max77802_rtc_is_precious_reg(struct device *dev, unsigned int reg)
{
	return (reg == MAX77802_RTC_INT ||
		reg == MAX77802_RTC_UPDATE0 ||
		reg == MAX77802_RTC_UPDATE1);
}

static bool max77802_is_precious_reg(struct device *dev, unsigned int reg)
{
	return (max77802_pmic_is_precious_reg(dev, reg) ||
		max77802_rtc_is_precious_reg(dev, reg));
}

static bool max77802_pmic_is_volatile_reg(struct device *dev, unsigned int reg)
{
	return (max77802_is_precious_reg(dev, reg) ||
		reg == MAX77802_REG_STATUS1 || reg == MAX77802_REG_STATUS2 ||
		reg == MAX77802_REG_PWRON);
}

static bool max77802_rtc_is_volatile_reg(struct device *dev, unsigned int reg)
{
	return (max77802_rtc_is_precious_reg(dev, reg) ||
		reg == MAX77802_RTC_SEC ||
		reg == MAX77802_RTC_MIN ||
		reg == MAX77802_RTC_HOUR ||
		reg == MAX77802_RTC_WEEKDAY ||
		reg == MAX77802_RTC_MONTH ||
		reg == MAX77802_RTC_YEAR ||
		reg == MAX77802_RTC_DATE);
}

static bool max77802_is_volatile_reg(struct device *dev, unsigned int reg)
{
	return (max77802_pmic_is_volatile_reg(dev, reg) ||
		max77802_rtc_is_volatile_reg(dev, reg));
}

static const struct regmap_config max77686_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
};

static const struct regmap_config max77802_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.writeable_reg = max77802_is_accessible_reg,
	.readable_reg = max77802_is_accessible_reg,
	.precious_reg = max77802_is_precious_reg,
	.volatile_reg = max77802_is_volatile_reg,
	.name = "max77802-pmic",
	.cache_type = REGCACHE_RBTREE,
};

static const struct regmap_irq max77686_irqs[] = {
	/* INT1 interrupts */
	{ .reg_offset = 0, .mask = MAX77686_INT1_PWRONF_MSK, },
	{ .reg_offset = 0, .mask = MAX77686_INT1_PWRONR_MSK, },
	{ .reg_offset = 0, .mask = MAX77686_INT1_JIGONBF_MSK, },
	{ .reg_offset = 0, .mask = MAX77686_INT1_JIGONBR_MSK, },
	{ .reg_offset = 0, .mask = MAX77686_INT1_ACOKBF_MSK, },
	{ .reg_offset = 0, .mask = MAX77686_INT1_ACOKBR_MSK, },
	{ .reg_offset = 0, .mask = MAX77686_INT1_ONKEY1S_MSK, },
	{ .reg_offset = 0, .mask = MAX77686_INT1_MRSTB_MSK, },
	/* INT2 interrupts */
	{ .reg_offset = 1, .mask = MAX77686_INT2_140C_MSK, },
	{ .reg_offset = 1, .mask = MAX77686_INT2_120C_MSK, },
};

static const struct regmap_irq_chip max77686_irq_chip = {
	.name			= "max77686-pmic",
	.status_base		= MAX77686_REG_INT1,
	.mask_base		= MAX77686_REG_INT1MSK,
	.num_regs		= 2,
	.irqs			= max77686_irqs,
	.num_irqs		= ARRAY_SIZE(max77686_irqs),
};

static const struct regmap_irq_chip max77802_irq_chip = {
	.name			= "max77802-pmic",
	.status_base		= MAX77802_REG_INT1,
	.mask_base		= MAX77802_REG_INT1MSK,
	.num_regs		= 2,
	.irqs			= max77686_irqs, /* same masks as 77686 */
	.num_irqs		= ARRAY_SIZE(max77686_irqs),
};

static const struct of_device_id max77686_pmic_dt_match[] = {
	{
		.compatible = "maxim,max77686",
		.data = (void *)TYPE_MAX77686,
	},
	{
		.compatible = "maxim,max77802",
		.data = (void *)TYPE_MAX77802,
	},
	{ },
};
MODULE_DEVICE_TABLE(of, max77686_pmic_dt_match);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int max77686_i2c_probe(struct i2c_client *i2c,
			      const struct i2c_device_id *id)
{
	struct max77686_dev *max77686 = NULL;
<<<<<<< HEAD
	struct max77686_platform_data *pdata = i2c->dev.platform_data;
	unsigned int data;
	int ret = 0;

	if (i2c->dev.of_node)
		pdata = max77686_i2c_parse_dt_pdata(&i2c->dev);

	if (!pdata) {
		ret = -EIO;
		dev_err(&i2c->dev, "No platform data found.\n");
		goto err;
	}

	max77686 = kzalloc(sizeof(struct max77686_dev), GFP_KERNEL);
	if (max77686 == NULL)
		return -ENOMEM;
=======
	const struct of_device_id *match;
	unsigned int data;
	int ret = 0;
	const struct regmap_config *config;
	const struct regmap_irq_chip *irq_chip;
	const struct mfd_cell *cells;
	int n_devs;

	max77686 = devm_kzalloc(&i2c->dev,
				sizeof(struct max77686_dev), GFP_KERNEL);
	if (!max77686)
		return -ENOMEM;

	if (i2c->dev.of_node) {
		match = of_match_node(max77686_pmic_dt_match, i2c->dev.of_node);
		if (!match)
			return -EINVAL;

		max77686->type = (unsigned long)match->data;
	} else
		max77686->type = id->driver_data;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	i2c_set_clientdata(i2c, max77686);
	max77686->dev = &i2c->dev;
	max77686->i2c = i2c;
<<<<<<< HEAD
	max77686->type = id->driver_data;

	max77686->wakeup = pdata->wakeup;
	max77686->irq_gpio = pdata->irq_gpio;
	max77686->irq = i2c->irq;

	max77686->regmap = devm_regmap_init_i2c(i2c, &max77686_regmap_config);
=======

	max77686->irq = i2c->irq;

	if (max77686->type == TYPE_MAX77686) {
		config = &max77686_regmap_config;
		irq_chip = &max77686_irq_chip;
		cells =  max77686_devs;
		n_devs = ARRAY_SIZE(max77686_devs);
	} else {
		config = &max77802_regmap_config;
		irq_chip = &max77802_irq_chip;
		cells =  max77802_devs;
		n_devs = ARRAY_SIZE(max77802_devs);
	}

	max77686->regmap = devm_regmap_init_i2c(i2c, config);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (IS_ERR(max77686->regmap)) {
		ret = PTR_ERR(max77686->regmap);
		dev_err(max77686->dev, "Failed to allocate register map: %d\n",
				ret);
<<<<<<< HEAD
		kfree(max77686);
		return ret;
	}

	if (regmap_read(max77686->regmap,
			 MAX77686_REG_DEVICE_ID, &data) < 0) {
		dev_err(max77686->dev,
			"device not found on this channel (this is not an error)\n");
		ret = -ENODEV;
		goto err;
	} else
		dev_info(max77686->dev, "device found\n");

	max77686->rtc = i2c_new_dummy(i2c->adapter, I2C_ADDR_RTC);
	if (!max77686->rtc) {
		dev_err(max77686->dev, "Failed to allocate I2C device for RTC\n");
		return -ENODEV;
	}
	i2c_set_clientdata(max77686->rtc, max77686);

	max77686_irq_init(max77686);

	ret = mfd_add_devices(max77686->dev, -1, max77686_devs,
			      ARRAY_SIZE(max77686_devs), NULL, 0, NULL);

	if (ret < 0)
		goto err_mfd;

	return ret;

err_mfd:
	mfd_remove_devices(max77686->dev);
	i2c_unregister_device(max77686->rtc);
err:
	kfree(max77686);
	return ret;
}

static int max77686_i2c_remove(struct i2c_client *i2c)
{
	struct max77686_dev *max77686 = i2c_get_clientdata(i2c);

	mfd_remove_devices(max77686->dev);
	i2c_unregister_device(max77686->rtc);
	kfree(max77686);

	return 0;
}

static const struct i2c_device_id max77686_i2c_id[] = {
	{ "max77686", TYPE_MAX77686 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, max77686_i2c_id);
=======
		return ret;
	}

	ret = regmap_read(max77686->regmap, MAX77686_REG_DEVICE_ID, &data);
	if (ret < 0) {
		dev_err(max77686->dev,
			"device not found on this channel (this is not an error)\n");
		return -ENODEV;
	}

	ret = devm_regmap_add_irq_chip(&i2c->dev, max77686->regmap,
				       max77686->irq,
				       IRQF_TRIGGER_FALLING | IRQF_ONESHOT |
				       IRQF_SHARED, 0, irq_chip,
				       &max77686->irq_data);
	if (ret < 0) {
		dev_err(&i2c->dev, "failed to add PMIC irq chip: %d\n", ret);
		return ret;
	}

	ret = devm_mfd_add_devices(max77686->dev, -1, cells, n_devs, NULL,
				   0, NULL);
	if (ret < 0) {
		dev_err(&i2c->dev, "failed to add MFD devices: %d\n", ret);
		return ret;
	}

	return 0;
}

static const struct i2c_device_id max77686_i2c_id[] = {
	{ "max77686", TYPE_MAX77686 },
	{ "max77802", TYPE_MAX77802 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, max77686_i2c_id);

#ifdef CONFIG_PM_SLEEP
static int max77686_suspend(struct device *dev)
{
	struct i2c_client *i2c = to_i2c_client(dev);
	struct max77686_dev *max77686 = i2c_get_clientdata(i2c);

	if (device_may_wakeup(dev))
		enable_irq_wake(max77686->irq);

	/*
	 * IRQ must be disabled during suspend because if it happens
	 * while suspended it will be handled before resuming I2C.
	 *
	 * When device is woken up from suspend (e.g. by RTC wake alarm),
	 * an interrupt occurs before resuming I2C bus controller.
	 * Interrupt handler tries to read registers but this read
	 * will fail because I2C is still suspended.
	 */
	disable_irq(max77686->irq);

	return 0;
}

static int max77686_resume(struct device *dev)
{
	struct i2c_client *i2c = to_i2c_client(dev);
	struct max77686_dev *max77686 = i2c_get_clientdata(i2c);

	if (device_may_wakeup(dev))
		disable_irq_wake(max77686->irq);

	enable_irq(max77686->irq);

	return 0;
}
#endif /* CONFIG_PM_SLEEP */

static SIMPLE_DEV_PM_OPS(max77686_pm, max77686_suspend, max77686_resume);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static struct i2c_driver max77686_i2c_driver = {
	.driver = {
		   .name = "max77686",
<<<<<<< HEAD
		   .owner = THIS_MODULE,
		   .of_match_table = of_match_ptr(max77686_pmic_dt_match),
	},
	.probe = max77686_i2c_probe,
	.remove = max77686_i2c_remove,
	.id_table = max77686_i2c_id,
};

static int __init max77686_i2c_init(void)
{
	return i2c_add_driver(&max77686_i2c_driver);
}
/* init early so consumer devices can complete system boot */
subsys_initcall(max77686_i2c_init);

static void __exit max77686_i2c_exit(void)
{
	i2c_del_driver(&max77686_i2c_driver);
}
module_exit(max77686_i2c_exit);

MODULE_DESCRIPTION("MAXIM 77686 multi-function core driver");
=======
		   .pm = &max77686_pm,
		   .of_match_table = of_match_ptr(max77686_pmic_dt_match),
	},
	.probe = max77686_i2c_probe,
	.id_table = max77686_i2c_id,
};

module_i2c_driver(max77686_i2c_driver);

MODULE_DESCRIPTION("MAXIM 77686/802 multi-function core driver");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_AUTHOR("Chiwoong Byun <woong.byun@samsung.com>");
MODULE_LICENSE("GPL");
