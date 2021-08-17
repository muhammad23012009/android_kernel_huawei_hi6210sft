/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/mfd/pm8xxx/core.h>
=======
#include <linux/regmap.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define VIB_DRV			0x4A

#define VIB_DRV_SEL_MASK	0xf8
#define VIB_DRV_SEL_SHIFT	0x03
#define VIB_DRV_EN_MANUAL_MASK	0xfc

#define VIB_MAX_LEVEL_mV	(3100)
#define VIB_MIN_LEVEL_mV	(1200)
#define VIB_MAX_LEVELS		(VIB_MAX_LEVEL_mV - VIB_MIN_LEVEL_mV)

#define MAX_FF_SPEED		0xff

/**
 * struct pm8xxx_vib - structure to hold vibrator data
 * @vib_input_dev: input device supporting force feedback
 * @work: work structure to set the vibration parameters
<<<<<<< HEAD
 * @dev: device supporting force feedback
=======
 * @regmap: regmap for register read/write
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @speed: speed of vibration set from userland
 * @active: state of vibrator
 * @level: level of vibration to set in the chip
 * @reg_vib_drv: VIB_DRV register value
 */
struct pm8xxx_vib {
	struct input_dev *vib_input_dev;
	struct work_struct work;
<<<<<<< HEAD
	struct device *dev;
=======
	struct regmap *regmap;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int speed;
	int level;
	bool active;
	u8  reg_vib_drv;
};

/**
<<<<<<< HEAD
 * pm8xxx_vib_read_u8 - helper to read a byte from pmic chip
 * @vib: pointer to vibrator structure
 * @data: placeholder for data to be read
 * @reg: register address
 */
static int pm8xxx_vib_read_u8(struct pm8xxx_vib *vib,
				 u8 *data, u16 reg)
{
	int rc;

	rc = pm8xxx_readb(vib->dev->parent, reg, data);
	if (rc < 0)
		dev_warn(vib->dev, "Error reading pm8xxx reg 0x%x(0x%x)\n",
				reg, rc);
	return rc;
}

/**
 * pm8xxx_vib_write_u8 - helper to write a byte to pmic chip
 * @vib: pointer to vibrator structure
 * @data: data to write
 * @reg: register address
 */
static int pm8xxx_vib_write_u8(struct pm8xxx_vib *vib,
				 u8 data, u16 reg)
{
	int rc;

	rc = pm8xxx_writeb(vib->dev->parent, reg, data);
	if (rc < 0)
		dev_warn(vib->dev, "Error writing pm8xxx reg 0x%x(0x%x)\n",
				reg, rc);
	return rc;
}

/**
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * pm8xxx_vib_set - handler to start/stop vibration
 * @vib: pointer to vibrator structure
 * @on: state to set
 */
static int pm8xxx_vib_set(struct pm8xxx_vib *vib, bool on)
{
	int rc;
<<<<<<< HEAD
	u8 val = vib->reg_vib_drv;
=======
	unsigned int val = vib->reg_vib_drv;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (on)
		val |= ((vib->level << VIB_DRV_SEL_SHIFT) & VIB_DRV_SEL_MASK);
	else
		val &= ~VIB_DRV_SEL_MASK;

<<<<<<< HEAD
	rc = pm8xxx_vib_write_u8(vib, val, VIB_DRV);
=======
	rc = regmap_write(vib->regmap, VIB_DRV, val);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (rc < 0)
		return rc;

	vib->reg_vib_drv = val;
	return 0;
}

/**
 * pm8xxx_work_handler - worker to set vibration level
 * @work: pointer to work_struct
 */
static void pm8xxx_work_handler(struct work_struct *work)
{
	struct pm8xxx_vib *vib = container_of(work, struct pm8xxx_vib, work);
	int rc;
<<<<<<< HEAD
	u8 val;

	rc = pm8xxx_vib_read_u8(vib, &val, VIB_DRV);
=======
	unsigned int val;

	rc = regmap_read(vib->regmap, VIB_DRV, &val);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (rc < 0)
		return;

	/*
	 * pmic vibrator supports voltage ranges from 1.2 to 3.1V, so
	 * scale the level to fit into these ranges.
	 */
	if (vib->speed) {
		vib->active = true;
		vib->level = ((VIB_MAX_LEVELS * vib->speed) / MAX_FF_SPEED) +
						VIB_MIN_LEVEL_mV;
		vib->level /= 100;
	} else {
		vib->active = false;
		vib->level = VIB_MIN_LEVEL_mV / 100;
	}

	pm8xxx_vib_set(vib, vib->active);
}

/**
 * pm8xxx_vib_close - callback of input close callback
 * @dev: input device pointer
 *
 * Turns off the vibrator.
 */
static void pm8xxx_vib_close(struct input_dev *dev)
{
	struct pm8xxx_vib *vib = input_get_drvdata(dev);

	cancel_work_sync(&vib->work);
	if (vib->active)
		pm8xxx_vib_set(vib, false);
}

/**
 * pm8xxx_vib_play_effect - function to handle vib effects.
 * @dev: input device pointer
 * @data: data of effect
 * @effect: effect to play
 *
 * Currently this driver supports only rumble effects.
 */
static int pm8xxx_vib_play_effect(struct input_dev *dev, void *data,
				  struct ff_effect *effect)
{
	struct pm8xxx_vib *vib = input_get_drvdata(dev);

	vib->speed = effect->u.rumble.strong_magnitude >> 8;
	if (!vib->speed)
		vib->speed = effect->u.rumble.weak_magnitude >> 9;

	schedule_work(&vib->work);

	return 0;
}

static int pm8xxx_vib_probe(struct platform_device *pdev)
<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct pm8xxx_vib *vib;
	struct input_dev *input_dev;
	int error;
<<<<<<< HEAD
	u8 val;

	vib = kzalloc(sizeof(*vib), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!vib || !input_dev) {
		dev_err(&pdev->dev, "couldn't allocate memory\n");
		error = -ENOMEM;
		goto err_free_mem;
	}

	INIT_WORK(&vib->work, pm8xxx_work_handler);
	vib->dev = &pdev->dev;
	vib->vib_input_dev = input_dev;

	/* operate in manual mode */
	error = pm8xxx_vib_read_u8(vib, &val, VIB_DRV);
	if (error < 0)
		goto err_free_mem;
	val &= ~VIB_DRV_EN_MANUAL_MASK;
	error = pm8xxx_vib_write_u8(vib, val, VIB_DRV);
	if (error < 0)
		goto err_free_mem;
=======
	unsigned int val;

	vib = devm_kzalloc(&pdev->dev, sizeof(*vib), GFP_KERNEL);
	if (!vib)
		return -ENOMEM;

	vib->regmap = dev_get_regmap(pdev->dev.parent, NULL);
	if (!vib->regmap)
		return -ENODEV;

	input_dev = devm_input_allocate_device(&pdev->dev);
	if (!input_dev)
		return -ENOMEM;

	INIT_WORK(&vib->work, pm8xxx_work_handler);
	vib->vib_input_dev = input_dev;

	/* operate in manual mode */
	error = regmap_read(vib->regmap, VIB_DRV, &val);
	if (error < 0)
		return error;

	val &= ~VIB_DRV_EN_MANUAL_MASK;
	error = regmap_write(vib->regmap, VIB_DRV, val);
	if (error < 0)
		return error;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	vib->reg_vib_drv = val;

	input_dev->name = "pm8xxx_vib_ffmemless";
	input_dev->id.version = 1;
<<<<<<< HEAD
	input_dev->dev.parent = &pdev->dev;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	input_dev->close = pm8xxx_vib_close;
	input_set_drvdata(input_dev, vib);
	input_set_capability(vib->vib_input_dev, EV_FF, FF_RUMBLE);

	error = input_ff_create_memless(input_dev, NULL,
					pm8xxx_vib_play_effect);
	if (error) {
		dev_err(&pdev->dev,
			"couldn't register vibrator as FF device\n");
<<<<<<< HEAD
		goto err_free_mem;
=======
		return error;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	error = input_register_device(input_dev);
	if (error) {
		dev_err(&pdev->dev, "couldn't register input device\n");
<<<<<<< HEAD
		goto err_destroy_memless;
=======
		return error;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	platform_set_drvdata(pdev, vib);
	return 0;
<<<<<<< HEAD

err_destroy_memless:
	input_ff_destroy(input_dev);
err_free_mem:
	input_free_device(input_dev);
	kfree(vib);

	return error;
}

static int pm8xxx_vib_remove(struct platform_device *pdev)
{
	struct pm8xxx_vib *vib = platform_get_drvdata(pdev);

	input_unregister_device(vib->vib_input_dev);
	kfree(vib);

	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int pm8xxx_vib_suspend(struct device *dev)
=======
}

static int __maybe_unused pm8xxx_vib_suspend(struct device *dev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct pm8xxx_vib *vib = dev_get_drvdata(dev);

	/* Turn off the vibrator */
	pm8xxx_vib_set(vib, false);

	return 0;
}
<<<<<<< HEAD
#endif

static SIMPLE_DEV_PM_OPS(pm8xxx_vib_pm_ops, pm8xxx_vib_suspend, NULL);

static struct platform_driver pm8xxx_vib_driver = {
	.probe		= pm8xxx_vib_probe,
	.remove		= pm8xxx_vib_remove,
	.driver		= {
		.name	= "pm8xxx-vib",
		.owner	= THIS_MODULE,
		.pm	= &pm8xxx_vib_pm_ops,
=======

static SIMPLE_DEV_PM_OPS(pm8xxx_vib_pm_ops, pm8xxx_vib_suspend, NULL);

static const struct of_device_id pm8xxx_vib_id_table[] = {
	{ .compatible = "qcom,pm8058-vib" },
	{ .compatible = "qcom,pm8921-vib" },
	{ }
};
MODULE_DEVICE_TABLE(of, pm8xxx_vib_id_table);

static struct platform_driver pm8xxx_vib_driver = {
	.probe		= pm8xxx_vib_probe,
	.driver		= {
		.name	= "pm8xxx-vib",
		.pm	= &pm8xxx_vib_pm_ops,
		.of_match_table = pm8xxx_vib_id_table,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
};
module_platform_driver(pm8xxx_vib_driver);

MODULE_ALIAS("platform:pm8xxx_vib");
MODULE_DESCRIPTION("PMIC8xxx vibrator driver based on ff-memless framework");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Amy Maloche <amaloche@codeaurora.org>");
