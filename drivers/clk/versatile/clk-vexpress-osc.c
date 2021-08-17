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
#define pr_fmt(fmt) "vexpress-osc: " fmt

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/clkdev.h>
#include <linux/clk-provider.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/vexpress.h>

struct vexpress_osc {
<<<<<<< HEAD
	struct vexpress_config_func *func;
=======
	struct regmap *reg;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct clk_hw hw;
	unsigned long rate_min;
	unsigned long rate_max;
};

#define to_vexpress_osc(osc) container_of(osc, struct vexpress_osc, hw)

static unsigned long vexpress_osc_recalc_rate(struct clk_hw *hw,
		unsigned long parent_rate)
{
	struct vexpress_osc *osc = to_vexpress_osc(hw);
	u32 rate;

<<<<<<< HEAD
	vexpress_config_read(osc->func, 0, &rate);
=======
	regmap_read(osc->reg, 0, &rate);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return rate;
}

static long vexpress_osc_round_rate(struct clk_hw *hw, unsigned long rate,
		unsigned long *parent_rate)
{
	struct vexpress_osc *osc = to_vexpress_osc(hw);

	if (WARN_ON(osc->rate_min && rate < osc->rate_min))
		rate = osc->rate_min;

	if (WARN_ON(osc->rate_max && rate > osc->rate_max))
		rate = osc->rate_max;

	return rate;
}

static int vexpress_osc_set_rate(struct clk_hw *hw, unsigned long rate,
		unsigned long parent_rate)
{
	struct vexpress_osc *osc = to_vexpress_osc(hw);

<<<<<<< HEAD
	return vexpress_config_write(osc->func, 0, rate);
=======
	return regmap_write(osc->reg, 0, rate);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static struct clk_ops vexpress_osc_ops = {
	.recalc_rate = vexpress_osc_recalc_rate,
	.round_rate = vexpress_osc_round_rate,
	.set_rate = vexpress_osc_set_rate,
};


<<<<<<< HEAD
struct clk * __init vexpress_osc_setup(struct device *dev)
{
	struct clk_init_data init;
	struct vexpress_osc *osc = kzalloc(sizeof(*osc), GFP_KERNEL);

	if (!osc)
		return NULL;

	osc->func = vexpress_config_func_get_by_dev(dev);
	if (!osc->func) {
		kfree(osc);
		return NULL;
	}

	init.name = dev_name(dev);
	init.ops = &vexpress_osc_ops;
	init.flags = CLK_IS_ROOT;
	init.num_parents = 0;
	osc->hw.init = &init;

	return clk_register(NULL, &osc->hw);
}

void __init vexpress_osc_of_setup(struct device_node *node)
=======
static int vexpress_osc_probe(struct platform_device *pdev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct clk_init_data init;
	struct vexpress_osc *osc;
	struct clk *clk;
	u32 range[2];

<<<<<<< HEAD
	osc = kzalloc(sizeof(*osc), GFP_KERNEL);
	if (!osc)
		return;

	osc->func = vexpress_config_func_get_by_node(node);
	if (!osc->func) {
		pr_err("Failed to obtain config func for node '%s'!\n",
				node->full_name);
		goto error;
	}

	if (of_property_read_u32_array(node, "freq-range", range,
=======
	osc = devm_kzalloc(&pdev->dev, sizeof(*osc), GFP_KERNEL);
	if (!osc)
		return -ENOMEM;

	osc->reg = devm_regmap_init_vexpress_config(&pdev->dev);
	if (IS_ERR(osc->reg))
		return PTR_ERR(osc->reg);

	if (of_property_read_u32_array(pdev->dev.of_node, "freq-range", range,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ARRAY_SIZE(range)) == 0) {
		osc->rate_min = range[0];
		osc->rate_max = range[1];
	}

<<<<<<< HEAD
	of_property_read_string(node, "clock-output-names", &init.name);
	if (!init.name)
		init.name = node->full_name;

	init.ops = &vexpress_osc_ops;
	init.flags = CLK_IS_ROOT;
=======
	if (of_property_read_string(pdev->dev.of_node, "clock-output-names",
			&init.name) != 0)
		init.name = dev_name(&pdev->dev);

	init.ops = &vexpress_osc_ops;
	init.flags = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	init.num_parents = 0;

	osc->hw.init = &init;

	clk = clk_register(NULL, &osc->hw);
<<<<<<< HEAD
	if (IS_ERR(clk)) {
		pr_err("Failed to register clock '%s'!\n", init.name);
		goto error;
	}

	of_clk_add_provider(node, of_clk_src_simple_get, clk);

	pr_debug("Registered clock '%s'\n", init.name);

	return;

error:
	if (osc->func)
		vexpress_config_func_put(osc->func);
	kfree(osc);
}
CLK_OF_DECLARE(vexpress_soc, "arm,vexpress-osc", vexpress_osc_of_setup);
=======
	if (IS_ERR(clk))
		return PTR_ERR(clk);

	of_clk_add_provider(pdev->dev.of_node, of_clk_src_simple_get, clk);

	dev_dbg(&pdev->dev, "Registered clock '%s'\n", init.name);

	return 0;
}

static const struct of_device_id vexpress_osc_of_match[] = {
	{ .compatible = "arm,vexpress-osc", },
	{}
};

static struct platform_driver vexpress_osc_driver = {
	.driver	= {
		.name = "vexpress-osc",
		.of_match_table = vexpress_osc_of_match,
	},
	.probe = vexpress_osc_probe,
};

static int __init vexpress_osc_init(void)
{
	return platform_driver_register(&vexpress_osc_driver);
}
core_initcall(vexpress_osc_init);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
