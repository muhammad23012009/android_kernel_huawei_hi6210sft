/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Copyright (C) 2012 ARM Limited
 */

<<<<<<< HEAD
#include <linux/jiffies.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/stat.h>
#include <linux/vexpress.h>

#include <asm/system_misc.h>

static void vexpress_reset_do(struct device *dev, const char *what)
{
	int err = -ENOENT;
	struct vexpress_config_func *func =
			vexpress_config_func_get_by_dev(dev);

	if (func) {
		unsigned long timeout;

		err = vexpress_config_write(func, 0, 0);

		timeout = jiffies + HZ;
		while (time_before(jiffies, timeout))
			cpu_relax();
=======
#include <linux/delay.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/reboot.h>
#include <linux/stat.h>
#include <linux/vexpress.h>

static void vexpress_reset_do(struct device *dev, const char *what)
{
	int err = -ENOENT;
	struct regmap *reg = dev_get_drvdata(dev);

	if (reg) {
		err = regmap_write(reg, 0, 0);
		if (!err)
			mdelay(1000);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	dev_emerg(dev, "Unable to %s (%d)\n", what, err);
}

static struct device *vexpress_power_off_device;
<<<<<<< HEAD
=======
static atomic_t vexpress_restart_nb_refcnt = ATOMIC_INIT(0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void vexpress_power_off(void)
{
	vexpress_reset_do(vexpress_power_off_device, "power off");
}

static struct device *vexpress_restart_device;

<<<<<<< HEAD
static void vexpress_restart(char str, const char *cmd)
{
	vexpress_reset_do(vexpress_restart_device, "restart");
}

=======
static int vexpress_restart(struct notifier_block *this, unsigned long mode,
			     void *cmd)
{
	vexpress_reset_do(vexpress_restart_device, "restart");

	return NOTIFY_DONE;
}

static struct notifier_block vexpress_restart_nb = {
	.notifier_call = vexpress_restart,
	.priority = 128,
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static ssize_t vexpress_reset_active_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", vexpress_restart_device == dev);
}

static ssize_t vexpress_reset_active_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	long value;
	int err = kstrtol(buf, 0, &value);

	if (!err && value)
		vexpress_restart_device = dev;

	return err ? err : count;
}

<<<<<<< HEAD
DEVICE_ATTR(active, S_IRUGO | S_IWUSR, vexpress_reset_active_show,
		vexpress_reset_active_store);
=======
static DEVICE_ATTR(active, S_IRUGO | S_IWUSR, vexpress_reset_active_show,
		   vexpress_reset_active_store);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414


enum vexpress_reset_func { FUNC_RESET, FUNC_SHUTDOWN, FUNC_REBOOT };

<<<<<<< HEAD
static struct of_device_id vexpress_reset_of_match[] = {
=======
static const struct of_device_id vexpress_reset_of_match[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{
		.compatible = "arm,vexpress-reset",
		.data = (void *)FUNC_RESET,
	}, {
		.compatible = "arm,vexpress-shutdown",
		.data = (void *)FUNC_SHUTDOWN
	}, {
		.compatible = "arm,vexpress-reboot",
		.data = (void *)FUNC_REBOOT
	},
	{}
};

<<<<<<< HEAD
static int vexpress_reset_probe(struct platform_device *pdev)
{
	enum vexpress_reset_func func;
	const struct of_device_id *match =
			of_match_device(vexpress_reset_of_match, &pdev->dev);

	if (match)
		func = (enum vexpress_reset_func)match->data;
	else
		func = pdev->id_entry->driver_data;

	switch (func) {
=======
static int _vexpress_register_restart_handler(struct device *dev)
{
	int err;

	vexpress_restart_device = dev;
	if (atomic_inc_return(&vexpress_restart_nb_refcnt) == 1) {
		err = register_restart_handler(&vexpress_restart_nb);
		if (err) {
			dev_err(dev, "cannot register restart handler (err=%d)\n", err);
			atomic_dec(&vexpress_restart_nb_refcnt);
			return err;
		}
	}
	device_create_file(dev, &dev_attr_active);

	return 0;
}

static int vexpress_reset_probe(struct platform_device *pdev)
{
	const struct of_device_id *match =
			of_match_device(vexpress_reset_of_match, &pdev->dev);
	struct regmap *regmap;
	int ret = 0;

	if (!match)
		return -EINVAL;

	regmap = devm_regmap_init_vexpress_config(&pdev->dev);
	if (IS_ERR(regmap))
		return PTR_ERR(regmap);
	dev_set_drvdata(&pdev->dev, regmap);

	switch ((enum vexpress_reset_func)match->data) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case FUNC_SHUTDOWN:
		vexpress_power_off_device = &pdev->dev;
		pm_power_off = vexpress_power_off;
		break;
	case FUNC_RESET:
		if (!vexpress_restart_device)
<<<<<<< HEAD
			vexpress_restart_device = &pdev->dev;
		arm_pm_restart = vexpress_restart;
		device_create_file(&pdev->dev, &dev_attr_active);
		break;
	case FUNC_REBOOT:
		vexpress_restart_device = &pdev->dev;
		arm_pm_restart = vexpress_restart;
		device_create_file(&pdev->dev, &dev_attr_active);
		break;
	};

	return 0;
}

static const struct platform_device_id vexpress_reset_id_table[] = {
	{ .name = "vexpress-reset", .driver_data = FUNC_RESET, },
	{ .name = "vexpress-shutdown", .driver_data = FUNC_SHUTDOWN, },
	{ .name = "vexpress-reboot", .driver_data = FUNC_REBOOT, },
	{}
};

=======
			ret = _vexpress_register_restart_handler(&pdev->dev);
		break;
	case FUNC_REBOOT:
		ret = _vexpress_register_restart_handler(&pdev->dev);
		break;
	};

	return ret;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct platform_driver vexpress_reset_driver = {
	.probe = vexpress_reset_probe,
	.driver = {
		.name = "vexpress-reset",
		.of_match_table = vexpress_reset_of_match,
<<<<<<< HEAD
	},
	.id_table = vexpress_reset_id_table,
=======
		.suppress_bind_attrs = true,
	},
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int __init vexpress_reset_init(void)
{
	return platform_driver_register(&vexpress_reset_driver);
}
device_initcall(vexpress_reset_init);
