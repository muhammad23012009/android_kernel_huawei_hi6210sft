/*
<<<<<<< HEAD
 * Xilinx gpio driver
 *
 * Copyright 2008 Xilinx, Inc.
=======
 * Xilinx gpio driver for xps/axi_gpio IP.
 *
 * Copyright 2008 - 2013 Xilinx, Inc.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

<<<<<<< HEAD
=======
#include <linux/bitops.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/slab.h>

/* Register Offset Definitions */
#define XGPIO_DATA_OFFSET   (0x0)	/* Data register  */
#define XGPIO_TRI_OFFSET    (0x4)	/* I/O direction register  */

<<<<<<< HEAD
struct xgpio_instance {
	struct of_mm_gpio_chip mmchip;
	u32 gpio_state;		/* GPIO state shadow register */
	u32 gpio_dir;		/* GPIO direction shadow register */
	spinlock_t gpio_lock;	/* Lock used for synchronization */
};

=======
#define XGPIO_CHANNEL_OFFSET	0x8

/* Read/Write access to the GPIO registers */
#if defined(CONFIG_ARCH_ZYNQ) || defined(CONFIG_X86)
# define xgpio_readreg(offset)		readl(offset)
# define xgpio_writereg(offset, val)	writel(val, offset)
#else
# define xgpio_readreg(offset)		__raw_readl(offset)
# define xgpio_writereg(offset, val)	__raw_writel(val, offset)
#endif

/**
 * struct xgpio_instance - Stores information about GPIO device
 * @mmchip: OF GPIO chip for memory mapped banks
 * @gpio_width: GPIO width for every channel
 * @gpio_state: GPIO state shadow register
 * @gpio_dir: GPIO direction shadow register
 * @gpio_lock: Lock used for synchronization
 */
struct xgpio_instance {
	struct of_mm_gpio_chip mmchip;
	unsigned int gpio_width[2];
	u32 gpio_state[2];
	u32 gpio_dir[2];
	spinlock_t gpio_lock[2];
};

static inline int xgpio_index(struct xgpio_instance *chip, int gpio)
{
	if (gpio >= chip->gpio_width[0])
		return 1;

	return 0;
}

static inline int xgpio_regoffset(struct xgpio_instance *chip, int gpio)
{
	if (xgpio_index(chip, gpio))
		return XGPIO_CHANNEL_OFFSET;

	return 0;
}

static inline int xgpio_offset(struct xgpio_instance *chip, int gpio)
{
	if (xgpio_index(chip, gpio))
		return gpio - chip->gpio_width[0];

	return gpio;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * xgpio_get - Read the specified signal of the GPIO device.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 *
<<<<<<< HEAD
 * This function reads the specified signal of the GPIO device. It returns 0 if
 * the signal clear, 1 if signal is set or negative value on error.
=======
 * This function reads the specified signal of the GPIO device.
 *
 * Return:
 * 0 if direction of GPIO signals is set as input otherwise it
 * returns negative error value.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static int xgpio_get(struct gpio_chip *gc, unsigned int gpio)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
<<<<<<< HEAD

	return (in_be32(mm_gc->regs + XGPIO_DATA_OFFSET) >> gpio) & 1;
=======
	struct xgpio_instance *chip = gpiochip_get_data(gc);
	u32 val;

	val = xgpio_readreg(mm_gc->regs + XGPIO_DATA_OFFSET +
			    xgpio_regoffset(chip, gpio));

	return !!(val & BIT(xgpio_offset(chip, gpio)));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * xgpio_set - Write the specified signal of the GPIO device.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 * @val:    Value to be written to specified signal.
 *
 * This function writes the specified value in to the specified signal of the
 * GPIO device.
 */
static void xgpio_set(struct gpio_chip *gc, unsigned int gpio, int val)
{
	unsigned long flags;
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
<<<<<<< HEAD
	struct xgpio_instance *chip =
	    container_of(mm_gc, struct xgpio_instance, mmchip);

	spin_lock_irqsave(&chip->gpio_lock, flags);

	/* Write to GPIO signal and set its direction to output */
	if (val)
		chip->gpio_state |= 1 << gpio;
	else
		chip->gpio_state &= ~(1 << gpio);
	out_be32(mm_gc->regs + XGPIO_DATA_OFFSET, chip->gpio_state);

	spin_unlock_irqrestore(&chip->gpio_lock, flags);
=======
	struct xgpio_instance *chip = gpiochip_get_data(gc);
	int index =  xgpio_index(chip, gpio);
	int offset =  xgpio_offset(chip, gpio);

	spin_lock_irqsave(&chip->gpio_lock[index], flags);

	/* Write to GPIO signal and set its direction to output */
	if (val)
		chip->gpio_state[index] |= BIT(offset);
	else
		chip->gpio_state[index] &= ~BIT(offset);

	xgpio_writereg(mm_gc->regs + XGPIO_DATA_OFFSET +
		       xgpio_regoffset(chip, gpio), chip->gpio_state[index]);

	spin_unlock_irqrestore(&chip->gpio_lock[index], flags);
}

/**
 * xgpio_set_multiple - Write the specified signals of the GPIO device.
 * @gc:     Pointer to gpio_chip device structure.
 * @mask:   Mask of the GPIOS to modify.
 * @bits:   Value to be wrote on each GPIO
 *
 * This function writes the specified values into the specified signals of the
 * GPIO devices.
 */
static void xgpio_set_multiple(struct gpio_chip *gc, unsigned long *mask,
			       unsigned long *bits)
{
	unsigned long flags;
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct xgpio_instance *chip = gpiochip_get_data(gc);
	int index = xgpio_index(chip, 0);
	int offset, i;

	spin_lock_irqsave(&chip->gpio_lock[index], flags);

	/* Write to GPIO signals */
	for (i = 0; i < gc->ngpio; i++) {
		if (*mask == 0)
			break;
		if (index !=  xgpio_index(chip, i)) {
			xgpio_writereg(mm_gc->regs + XGPIO_DATA_OFFSET +
				       xgpio_regoffset(chip, i),
				       chip->gpio_state[index]);
			spin_unlock_irqrestore(&chip->gpio_lock[index], flags);
			index =  xgpio_index(chip, i);
			spin_lock_irqsave(&chip->gpio_lock[index], flags);
		}
		if (__test_and_clear_bit(i, mask)) {
			offset =  xgpio_offset(chip, i);
			if (test_bit(i, bits))
				chip->gpio_state[index] |= BIT(offset);
			else
				chip->gpio_state[index] &= ~BIT(offset);
		}
	}

	xgpio_writereg(mm_gc->regs + XGPIO_DATA_OFFSET +
		       xgpio_regoffset(chip, i), chip->gpio_state[index]);

	spin_unlock_irqrestore(&chip->gpio_lock[index], flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * xgpio_dir_in - Set the direction of the specified GPIO signal as input.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 *
<<<<<<< HEAD
 * This function sets the direction of specified GPIO signal as input.
 * It returns 0 if direction of GPIO signals is set as input otherwise it
 * returns negative error value.
=======
 * Return:
 * 0 - if direction of GPIO signals is set as input
 * otherwise it returns negative error value.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static int xgpio_dir_in(struct gpio_chip *gc, unsigned int gpio)
{
	unsigned long flags;
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
<<<<<<< HEAD
	struct xgpio_instance *chip =
	    container_of(mm_gc, struct xgpio_instance, mmchip);

	spin_lock_irqsave(&chip->gpio_lock, flags);

	/* Set the GPIO bit in shadow register and set direction as input */
	chip->gpio_dir |= (1 << gpio);
	out_be32(mm_gc->regs + XGPIO_TRI_OFFSET, chip->gpio_dir);

	spin_unlock_irqrestore(&chip->gpio_lock, flags);
=======
	struct xgpio_instance *chip = gpiochip_get_data(gc);
	int index =  xgpio_index(chip, gpio);
	int offset =  xgpio_offset(chip, gpio);

	spin_lock_irqsave(&chip->gpio_lock[index], flags);

	/* Set the GPIO bit in shadow register and set direction as input */
	chip->gpio_dir[index] |= BIT(offset);
	xgpio_writereg(mm_gc->regs + XGPIO_TRI_OFFSET +
		       xgpio_regoffset(chip, gpio), chip->gpio_dir[index]);

	spin_unlock_irqrestore(&chip->gpio_lock[index], flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

/**
 * xgpio_dir_out - Set the direction of the specified GPIO signal as output.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 * @val:    Value to be written to specified signal.
 *
<<<<<<< HEAD
 * This function sets the direction of specified GPIO signal as output. If all
 * GPIO signals of GPIO chip is configured as input then it returns
=======
 * This function sets the direction of specified GPIO signal as output.
 *
 * Return:
 * If all GPIO signals of GPIO chip is configured as input then it returns
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * error otherwise it returns 0.
 */
static int xgpio_dir_out(struct gpio_chip *gc, unsigned int gpio, int val)
{
	unsigned long flags;
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
<<<<<<< HEAD
	struct xgpio_instance *chip =
	    container_of(mm_gc, struct xgpio_instance, mmchip);

	spin_lock_irqsave(&chip->gpio_lock, flags);

	/* Write state of GPIO signal */
	if (val)
		chip->gpio_state |= 1 << gpio;
	else
		chip->gpio_state &= ~(1 << gpio);
	out_be32(mm_gc->regs + XGPIO_DATA_OFFSET, chip->gpio_state);

	/* Clear the GPIO bit in shadow register and set direction as output */
	chip->gpio_dir &= (~(1 << gpio));
	out_be32(mm_gc->regs + XGPIO_TRI_OFFSET, chip->gpio_dir);

	spin_unlock_irqrestore(&chip->gpio_lock, flags);
=======
	struct xgpio_instance *chip = gpiochip_get_data(gc);
	int index =  xgpio_index(chip, gpio);
	int offset =  xgpio_offset(chip, gpio);

	spin_lock_irqsave(&chip->gpio_lock[index], flags);

	/* Write state of GPIO signal */
	if (val)
		chip->gpio_state[index] |= BIT(offset);
	else
		chip->gpio_state[index] &= ~BIT(offset);
	xgpio_writereg(mm_gc->regs + XGPIO_DATA_OFFSET +
			xgpio_regoffset(chip, gpio), chip->gpio_state[index]);

	/* Clear the GPIO bit in shadow register and set direction as output */
	chip->gpio_dir[index] &= ~BIT(offset);
	xgpio_writereg(mm_gc->regs + XGPIO_TRI_OFFSET +
			xgpio_regoffset(chip, gpio), chip->gpio_dir[index]);

	spin_unlock_irqrestore(&chip->gpio_lock[index], flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

/**
 * xgpio_save_regs - Set initial values of GPIO pins
<<<<<<< HEAD
 * @mm_gc: pointer to memory mapped GPIO chip structure
=======
 * @mm_gc: Pointer to memory mapped GPIO chip structure
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static void xgpio_save_regs(struct of_mm_gpio_chip *mm_gc)
{
	struct xgpio_instance *chip =
<<<<<<< HEAD
	    container_of(mm_gc, struct xgpio_instance, mmchip);

	out_be32(mm_gc->regs + XGPIO_DATA_OFFSET, chip->gpio_state);
	out_be32(mm_gc->regs + XGPIO_TRI_OFFSET, chip->gpio_dir);
=======
		container_of(mm_gc, struct xgpio_instance, mmchip);

	xgpio_writereg(mm_gc->regs + XGPIO_DATA_OFFSET,	chip->gpio_state[0]);
	xgpio_writereg(mm_gc->regs + XGPIO_TRI_OFFSET, chip->gpio_dir[0]);

	if (!chip->gpio_width[1])
		return;

	xgpio_writereg(mm_gc->regs + XGPIO_DATA_OFFSET + XGPIO_CHANNEL_OFFSET,
		       chip->gpio_state[1]);
	xgpio_writereg(mm_gc->regs + XGPIO_TRI_OFFSET + XGPIO_CHANNEL_OFFSET,
		       chip->gpio_dir[1]);
}

/**
 * xgpio_remove - Remove method for the GPIO device.
 * @pdev: pointer to the platform device
 *
 * This function remove gpiochips and frees all the allocated resources.
 *
 * Return: 0 always
 */
static int xgpio_remove(struct platform_device *pdev)
{
	struct xgpio_instance *chip = platform_get_drvdata(pdev);

	of_mm_gpiochip_remove(&chip->mmchip);

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * xgpio_of_probe - Probe method for the GPIO device.
<<<<<<< HEAD
 * @np: pointer to device tree node
 *
 * This function probes the GPIO device in the device tree. It initializes the
 * driver data structure. It returns 0, if the driver is bound to the GPIO
 * device, or a negative value if there is an error.
 */
static int xgpio_of_probe(struct device_node *np)
{
	struct xgpio_instance *chip;
	int status = 0;
	const u32 *tree_info;

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	/* Update GPIO state shadow register with default value */
	tree_info = of_get_property(np, "xlnx,dout-default", NULL);
	if (tree_info)
		chip->gpio_state = be32_to_cpup(tree_info);

	/* Update GPIO direction shadow register with default value */
	chip->gpio_dir = 0xFFFFFFFF; /* By default, all pins are inputs */
	tree_info = of_get_property(np, "xlnx,tri-default", NULL);
	if (tree_info)
		chip->gpio_dir = be32_to_cpup(tree_info);

	/* Check device node and parent device node for device width */
	chip->mmchip.gc.ngpio = 32; /* By default assume full GPIO controller */
	tree_info = of_get_property(np, "xlnx,gpio-width", NULL);
	if (!tree_info)
		tree_info = of_get_property(np->parent,
					    "xlnx,gpio-width", NULL);
	if (tree_info)
		chip->mmchip.gc.ngpio = be32_to_cpup(tree_info);

	spin_lock_init(&chip->gpio_lock);

=======
 * @pdev: pointer to the platform device
 *
 * Return:
 * It returns 0, if the driver is bound to the GPIO device, or
 * a negative value if there is an error.
 */
static int xgpio_probe(struct platform_device *pdev)
{
	struct xgpio_instance *chip;
	int status = 0;
	struct device_node *np = pdev->dev.of_node;
	u32 is_dual;

	chip = devm_kzalloc(&pdev->dev, sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	platform_set_drvdata(pdev, chip);

	/* Update GPIO state shadow register with default value */
	of_property_read_u32(np, "xlnx,dout-default", &chip->gpio_state[0]);

	/* Update GPIO direction shadow register with default value */
	if (of_property_read_u32(np, "xlnx,tri-default", &chip->gpio_dir[0]))
		chip->gpio_dir[0] = 0xFFFFFFFF;

	/*
	 * Check device node and parent device node for device width
	 * and assume default width of 32
	 */
	if (of_property_read_u32(np, "xlnx,gpio-width", &chip->gpio_width[0]))
		chip->gpio_width[0] = 32;

	spin_lock_init(&chip->gpio_lock[0]);

	if (of_property_read_u32(np, "xlnx,is-dual", &is_dual))
		is_dual = 0;

	if (is_dual) {
		/* Update GPIO state shadow register with default value */
		of_property_read_u32(np, "xlnx,dout-default-2",
				     &chip->gpio_state[1]);

		/* Update GPIO direction shadow register with default value */
		if (of_property_read_u32(np, "xlnx,tri-default-2",
					 &chip->gpio_dir[1]))
			chip->gpio_dir[1] = 0xFFFFFFFF;

		/*
		 * Check device node and parent device node for device width
		 * and assume default width of 32
		 */
		if (of_property_read_u32(np, "xlnx,gpio2-width",
					 &chip->gpio_width[1]))
			chip->gpio_width[1] = 32;

		spin_lock_init(&chip->gpio_lock[1]);
	}

	chip->mmchip.gc.ngpio = chip->gpio_width[0] + chip->gpio_width[1];
	chip->mmchip.gc.parent = &pdev->dev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	chip->mmchip.gc.direction_input = xgpio_dir_in;
	chip->mmchip.gc.direction_output = xgpio_dir_out;
	chip->mmchip.gc.get = xgpio_get;
	chip->mmchip.gc.set = xgpio_set;
<<<<<<< HEAD
=======
	chip->mmchip.gc.set_multiple = xgpio_set_multiple;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	chip->mmchip.save_regs = xgpio_save_regs;

	/* Call the OF gpio helper to setup and register the GPIO device */
<<<<<<< HEAD
	status = of_mm_gpiochip_add(np, &chip->mmchip);
	if (status) {
		kfree(chip);
=======
	status = of_mm_gpiochip_add_data(np, &chip->mmchip, chip);
	if (status) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		pr_err("%s: error in probe function with status %d\n",
		       np->full_name, status);
		return status;
	}
<<<<<<< HEAD
	return 0;
}

static struct of_device_id xgpio_of_match[] = {
=======

	return 0;
}

static const struct of_device_id xgpio_of_match[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ .compatible = "xlnx,xps-gpio-1.00.a", },
	{ /* end of list */ },
};

<<<<<<< HEAD
static int __init xgpio_init(void)
{
	struct device_node *np;

	for_each_matching_node(np, xgpio_of_match)
		xgpio_of_probe(np);

	return 0;
}

/* Make sure we get initialized before anyone else tries to use us */
subsys_initcall(xgpio_init);
/* No exit call at the moment as we cannot unregister of GPIO chips */
=======
MODULE_DEVICE_TABLE(of, xgpio_of_match);

static struct platform_driver xgpio_plat_driver = {
	.probe		= xgpio_probe,
	.remove		= xgpio_remove,
	.driver		= {
			.name = "gpio-xilinx",
			.of_match_table	= xgpio_of_match,
	},
};

static int __init xgpio_init(void)
{
	return platform_driver_register(&xgpio_plat_driver);
}

subsys_initcall(xgpio_init);

static void __exit xgpio_exit(void)
{
	platform_driver_unregister(&xgpio_plat_driver);
}
module_exit(xgpio_exit);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

MODULE_AUTHOR("Xilinx, Inc.");
MODULE_DESCRIPTION("Xilinx GPIO driver");
MODULE_LICENSE("GPL");
