/*
 * Copyright (C) 2010-2011 Canonical Ltd <jeremy.kerr@canonical.com>
 * Copyright (C) 2011-2012 Mike Turquette, Linaro Ltd <mturquette@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Fixed rate clock implementation
 */

#include <linux/clk-provider.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/of.h>
<<<<<<< HEAD
#ifdef CONFIG_HI3XXX_CLK
#include <linux/clkdev.h>
#endif
#ifdef CONFIG_HI6XXX_CLK
#include <soc_baseaddr_interface.h>
#include <soc_smart_interface.h>
#endif
=======
#include <linux/platform_device.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * DOC: basic fixed-rate clock that cannot gate
 *
 * Traits of this clock:
 * prepare - clk_(un)prepare only ensures parents are prepared
 * enable - clk_enable only ensures parents are enabled
 * rate - rate is always a fixed value.  No clk_set_rate support
 * parent - fixed parent.  No clk_set_parent support
 */

<<<<<<< HEAD
#define to_clk_fixed_rate(_hw) container_of(_hw, struct clk_fixed_rate, hw)
#ifdef CONFIG_HI6XXX_CLK
extern unsigned char hi6xxx_pmic_reg_read_ex (void *pmu_base, unsigned int reg_addr);
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static unsigned long clk_fixed_rate_recalc_rate(struct clk_hw *hw,
		unsigned long parent_rate)
{
	return to_clk_fixed_rate(hw)->fixed_rate;
}

<<<<<<< HEAD
const struct clk_ops clk_fixed_rate_ops = {
	.recalc_rate = clk_fixed_rate_recalc_rate,
=======
static unsigned long clk_fixed_rate_recalc_accuracy(struct clk_hw *hw,
		unsigned long parent_accuracy)
{
	return to_clk_fixed_rate(hw)->fixed_accuracy;
}

const struct clk_ops clk_fixed_rate_ops = {
	.recalc_rate = clk_fixed_rate_recalc_rate,
	.recalc_accuracy = clk_fixed_rate_recalc_accuracy,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
EXPORT_SYMBOL_GPL(clk_fixed_rate_ops);

/**
<<<<<<< HEAD
 * clk_register_fixed_rate - register fixed-rate clock with the clock framework
=======
 * clk_hw_register_fixed_rate_with_accuracy - register fixed-rate clock with
 * the clock framework
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @dev: device that is registering this clock
 * @name: name of this clock
 * @parent_name: name of clock's parent
 * @flags: framework-specific flags
 * @fixed_rate: non-adjustable clock rate
<<<<<<< HEAD
 */
struct clk *clk_register_fixed_rate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		unsigned long fixed_rate)
{
	struct clk_fixed_rate *fixed;
	struct clk *clk;
	struct clk_init_data init;

	/* allocate fixed-rate clock */
	fixed = kzalloc(sizeof(struct clk_fixed_rate), GFP_KERNEL);
	if (!fixed) {
		pr_err("%s: could not allocate fixed clk\n", __func__);
		return ERR_PTR(-ENOMEM);
	}
=======
 * @fixed_accuracy: non-adjustable clock rate
 */
struct clk_hw *clk_hw_register_fixed_rate_with_accuracy(struct device *dev,
		const char *name, const char *parent_name, unsigned long flags,
		unsigned long fixed_rate, unsigned long fixed_accuracy)
{
	struct clk_fixed_rate *fixed;
	struct clk_hw *hw;
	struct clk_init_data init;
	int ret;

	/* allocate fixed-rate clock */
	fixed = kzalloc(sizeof(*fixed), GFP_KERNEL);
	if (!fixed)
		return ERR_PTR(-ENOMEM);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	init.name = name;
	init.ops = &clk_fixed_rate_ops;
	init.flags = flags | CLK_IS_BASIC;
	init.parent_names = (parent_name ? &parent_name: NULL);
	init.num_parents = (parent_name ? 1 : 0);

	/* struct clk_fixed_rate assignments */
	fixed->fixed_rate = fixed_rate;
<<<<<<< HEAD
	fixed->hw.init = &init;

	/* register the clock */
	clk = clk_register(dev, &fixed->hw);

	if (IS_ERR(clk))
		kfree(fixed);

	return clk;
}
EXPORT_SYMBOL_GPL(clk_register_fixed_rate);

#ifdef CONFIG_HI3XXX_CLK
int IS_FPGA(void)
{
	if (IS_ERR_OR_NULL(__clk_lookup("fpga")))
		return 0;
	else
		return 1;
}
EXPORT_SYMBOL_GPL(IS_FPGA);
#endif

#ifdef CONFIG_OF
/**
 * of_fixed_clk_setup() - Setup function for simple fixed rate clock
 */
void of_fixed_clk_setup(struct device_node *node)
=======
	fixed->fixed_accuracy = fixed_accuracy;
	fixed->hw.init = &init;

	/* register the clock */
	hw = &fixed->hw;
	ret = clk_hw_register(dev, hw);
	if (ret) {
		kfree(fixed);
		hw = ERR_PTR(ret);
	}

	return hw;
}
EXPORT_SYMBOL_GPL(clk_hw_register_fixed_rate_with_accuracy);

struct clk *clk_register_fixed_rate_with_accuracy(struct device *dev,
		const char *name, const char *parent_name, unsigned long flags,
		unsigned long fixed_rate, unsigned long fixed_accuracy)
{
	struct clk_hw *hw;

	hw = clk_hw_register_fixed_rate_with_accuracy(dev, name, parent_name,
			flags, fixed_rate, fixed_accuracy);
	if (IS_ERR(hw))
		return ERR_CAST(hw);
	return hw->clk;
}
EXPORT_SYMBOL_GPL(clk_register_fixed_rate_with_accuracy);

/**
 * clk_hw_register_fixed_rate - register fixed-rate clock with the clock
 * framework
 * @dev: device that is registering this clock
 * @name: name of this clock
 * @parent_name: name of clock's parent
 * @flags: framework-specific flags
 * @fixed_rate: non-adjustable clock rate
 */
struct clk_hw *clk_hw_register_fixed_rate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		unsigned long fixed_rate)
{
	return clk_hw_register_fixed_rate_with_accuracy(dev, name, parent_name,
						     flags, fixed_rate, 0);
}
EXPORT_SYMBOL_GPL(clk_hw_register_fixed_rate);

struct clk *clk_register_fixed_rate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		unsigned long fixed_rate)
{
	return clk_register_fixed_rate_with_accuracy(dev, name, parent_name,
						     flags, fixed_rate, 0);
}
EXPORT_SYMBOL_GPL(clk_register_fixed_rate);

void clk_unregister_fixed_rate(struct clk *clk)
{
	struct clk_hw *hw;

	hw = __clk_get_hw(clk);
	if (!hw)
		return;

	clk_unregister(clk);
	kfree(to_clk_fixed_rate(hw));
}
EXPORT_SYMBOL_GPL(clk_unregister_fixed_rate);

void clk_hw_unregister_fixed_rate(struct clk_hw *hw)
{
	struct clk_fixed_rate *fixed;

	fixed = to_clk_fixed_rate(hw);

	clk_hw_unregister(hw);
	kfree(fixed);
}
EXPORT_SYMBOL_GPL(clk_hw_unregister_fixed_rate);

#ifdef CONFIG_OF
static struct clk *_of_fixed_clk_setup(struct device_node *node)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct clk *clk;
	const char *clk_name = node->name;
	u32 rate;
<<<<<<< HEAD
#ifdef CONFIG_HI6XXX_CLK
      void __iomem *pmu_baseaddr = NULL;
      u32 pmu_version = 0;
#endif

	if (of_property_read_u32(node, "clock-frequency", &rate))
		return;
	/*PMU 32k-clk: hi6553(32764), hi6552(32000) */
#ifdef CONFIG_HI6XXX_CLK
	pmu_baseaddr = ioremap(SOC_PMUSSI_BASE_ADDR, SZ_4K);
       pmu_version = hi6xxx_pmic_reg_read_ex(pmu_baseaddr, SOC_SMART_VERSION_ADDR(0));
	/*PMU chipid equals to hi6553 */
       if((pmu_version == 0x10) || (pmu_version == 0x11)){
             if(NULL != of_find_property(node, "clock-frequency-hi6553", NULL))
			 if (of_property_read_u32(node, "clock-frequency-hi6553", &rate))
				return;
       }
#endif
	of_property_read_string(node, "clock-output-names", &clk_name);
	clk = clk_register_fixed_rate(NULL, clk_name, NULL, CLK_IS_ROOT, rate);
	if (!IS_ERR(clk))
		of_clk_add_provider(node, of_clk_src_simple_get, clk);
#ifdef CONFIG_HI3XXX_CLK
	clk_register_clkdev(clk, clk_name, NULL);
#endif
}
EXPORT_SYMBOL_GPL(of_fixed_clk_setup);
CLK_OF_DECLARE(fixed_clk, "fixed-clock", of_fixed_clk_setup);
=======
	u32 accuracy = 0;
	int ret;

	if (of_property_read_u32(node, "clock-frequency", &rate))
		return ERR_PTR(-EIO);

	of_property_read_u32(node, "clock-accuracy", &accuracy);

	of_property_read_string(node, "clock-output-names", &clk_name);

	clk = clk_register_fixed_rate_with_accuracy(NULL, clk_name, NULL,
						    0, rate, accuracy);
	if (IS_ERR(clk))
		return clk;

	ret = of_clk_add_provider(node, of_clk_src_simple_get, clk);
	if (ret) {
		clk_unregister(clk);
		return ERR_PTR(ret);
	}

	return clk;
}

/**
 * of_fixed_clk_setup() - Setup function for simple fixed rate clock
 */
void __init of_fixed_clk_setup(struct device_node *node)
{
	_of_fixed_clk_setup(node);
}
CLK_OF_DECLARE(fixed_clk, "fixed-clock", of_fixed_clk_setup);

static int of_fixed_clk_remove(struct platform_device *pdev)
{
	struct clk *clk = platform_get_drvdata(pdev);

	of_clk_del_provider(pdev->dev.of_node);
	clk_unregister_fixed_rate(clk);

	return 0;
}

static int of_fixed_clk_probe(struct platform_device *pdev)
{
	struct clk *clk;

	/*
	 * This function is not executed when of_fixed_clk_setup
	 * succeeded.
	 */
	clk = _of_fixed_clk_setup(pdev->dev.of_node);
	if (IS_ERR(clk))
		return PTR_ERR(clk);

	platform_set_drvdata(pdev, clk);

	return 0;
}

static const struct of_device_id of_fixed_clk_ids[] = {
	{ .compatible = "fixed-clock" },
	{ }
};
MODULE_DEVICE_TABLE(of, of_fixed_clk_ids);

static struct platform_driver of_fixed_clk_driver = {
	.driver = {
		.name = "of_fixed_clk",
		.of_match_table = of_fixed_clk_ids,
	},
	.probe = of_fixed_clk_probe,
	.remove = of_fixed_clk_remove,
};
builtin_platform_driver(of_fixed_clk_driver);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
