/*
 * Copyright (C) 2013 Emilio López <emilio@elopez.com.ar>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Adjustable factor-based clock implementation
 */

#include <linux/clk-provider.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/string.h>

#include <linux/delay.h>

#include "clk-factors.h"

/*
 * DOC: basic adjustable factor-based clock that cannot gate
=======
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/of_address.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "clk-factors.h"

/*
 * DOC: basic adjustable factor-based clock
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Traits of this clock:
 * prepare - clk_prepare only ensures that parents are prepared
 * enable - clk_enable only ensures that parents are enabled
 * rate - rate is adjustable.
 *        clk->rate = (parent->rate * N * (K + 1) >> P) / (M + 1)
 * parent - fixed parent.  No clk_set_parent support
 */

<<<<<<< HEAD
struct clk_factors {
	struct clk_hw hw;
	void __iomem *reg;
	struct clk_factors_config *config;
	void (*get_factors) (u32 *rate, u32 parent, u8 *n, u8 *k, u8 *m, u8 *p);
	spinlock_t *lock;
};

#define to_clk_factors(_hw) container_of(_hw, struct clk_factors, hw)

#define SETMASK(len, pos)		(((-1U) >> (31-len))  << (pos))
=======
#define to_clk_factors(_hw) container_of(_hw, struct clk_factors, hw)

#define FACTORS_MAX_PARENTS		5

#define SETMASK(len, pos)		(((1U << (len)) - 1) << (pos))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define CLRMASK(len, pos)		(~(SETMASK(len, pos)))
#define FACTOR_GET(bit, len, reg)	(((reg) & SETMASK(len, bit)) >> (bit))

#define FACTOR_SET(bit, len, reg, val) \
	(((reg) & CLRMASK(len, bit)) | (val << (bit)))

static unsigned long clk_factors_recalc_rate(struct clk_hw *hw,
					     unsigned long parent_rate)
{
	u8 n = 1, k = 0, p = 0, m = 0;
	u32 reg;
	unsigned long rate;
	struct clk_factors *factors = to_clk_factors(hw);
<<<<<<< HEAD
	struct clk_factors_config *config = factors->config;
=======
	const struct clk_factors_config *config = factors->config;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Fetch the register value */
	reg = readl(factors->reg);

	/* Get each individual factor if applicable */
	if (config->nwidth != SUNXI_FACTORS_NOT_APPLICABLE)
		n = FACTOR_GET(config->nshift, config->nwidth, reg);
	if (config->kwidth != SUNXI_FACTORS_NOT_APPLICABLE)
		k = FACTOR_GET(config->kshift, config->kwidth, reg);
	if (config->mwidth != SUNXI_FACTORS_NOT_APPLICABLE)
		m = FACTOR_GET(config->mshift, config->mwidth, reg);
	if (config->pwidth != SUNXI_FACTORS_NOT_APPLICABLE)
		p = FACTOR_GET(config->pshift, config->pwidth, reg);

<<<<<<< HEAD
=======
	if (factors->recalc) {
		struct factors_request factors_req = {
			.parent_rate = parent_rate,
			.n = n,
			.k = k,
			.m = m,
			.p = p,
		};

		/* get mux details from mux clk structure */
		if (factors->mux)
			factors_req.parent_index =
				(reg >> factors->mux->shift) &
				factors->mux->mask;

		factors->recalc(&factors_req);

		return factors_req.rate;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Calculate the rate */
	rate = (parent_rate * (n + config->n_start) * (k + 1) >> p) / (m + 1);

	return rate;
}

<<<<<<< HEAD
static long clk_factors_round_rate(struct clk_hw *hw, unsigned long rate,
				   unsigned long *parent_rate)
{
	struct clk_factors *factors = to_clk_factors(hw);
	factors->get_factors((u32 *)&rate, (u32)*parent_rate,
			     NULL, NULL, NULL, NULL);

	return rate;
=======
static int clk_factors_determine_rate(struct clk_hw *hw,
				      struct clk_rate_request *req)
{
	struct clk_factors *factors = to_clk_factors(hw);
	struct clk_hw *parent, *best_parent = NULL;
	int i, num_parents;
	unsigned long parent_rate, best = 0, child_rate, best_child_rate = 0;

	/* find the parent that can help provide the fastest rate <= rate */
	num_parents = clk_hw_get_num_parents(hw);
	for (i = 0; i < num_parents; i++) {
		struct factors_request factors_req = {
			.rate = req->rate,
			.parent_index = i,
		};
		parent = clk_hw_get_parent_by_index(hw, i);
		if (!parent)
			continue;
		if (clk_hw_get_flags(hw) & CLK_SET_RATE_PARENT)
			parent_rate = clk_hw_round_rate(parent, req->rate);
		else
			parent_rate = clk_hw_get_rate(parent);

		factors_req.parent_rate = parent_rate;
		factors->get_factors(&factors_req);
		child_rate = factors_req.rate;

		if (child_rate <= req->rate && child_rate > best_child_rate) {
			best_parent = parent;
			best = parent_rate;
			best_child_rate = child_rate;
		}
	}

	if (!best_parent)
		return -EINVAL;

	req->best_parent_hw = best_parent;
	req->best_parent_rate = best;
	req->rate = best_child_rate;

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int clk_factors_set_rate(struct clk_hw *hw, unsigned long rate,
				unsigned long parent_rate)
{
<<<<<<< HEAD
	u8 n, k, m, p;
	u32 reg;
	struct clk_factors *factors = to_clk_factors(hw);
	struct clk_factors_config *config = factors->config;
	unsigned long flags = 0;

	factors->get_factors((u32 *)&rate, (u32)parent_rate, &n, &k, &m, &p);
=======
	struct factors_request req = {
		.rate = rate,
		.parent_rate = parent_rate,
	};
	u32 reg;
	struct clk_factors *factors = to_clk_factors(hw);
	const struct clk_factors_config *config = factors->config;
	unsigned long flags = 0;

	factors->get_factors(&req);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (factors->lock)
		spin_lock_irqsave(factors->lock, flags);

	/* Fetch the register value */
	reg = readl(factors->reg);

	/* Set up the new factors - macros do not do anything if width is 0 */
<<<<<<< HEAD
	reg = FACTOR_SET(config->nshift, config->nwidth, reg, n);
	reg = FACTOR_SET(config->kshift, config->kwidth, reg, k);
	reg = FACTOR_SET(config->mshift, config->mwidth, reg, m);
	reg = FACTOR_SET(config->pshift, config->pwidth, reg, p);
=======
	reg = FACTOR_SET(config->nshift, config->nwidth, reg, req.n);
	reg = FACTOR_SET(config->kshift, config->kwidth, reg, req.k);
	reg = FACTOR_SET(config->mshift, config->mwidth, reg, req.m);
	reg = FACTOR_SET(config->pshift, config->pwidth, reg, req.p);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Apply them now */
	writel(reg, factors->reg);

	/* delay 500us so pll stabilizes */
	__delay((rate >> 20) * 500 / 2);

	if (factors->lock)
		spin_unlock_irqrestore(factors->lock, flags);

	return 0;
}

static const struct clk_ops clk_factors_ops = {
<<<<<<< HEAD
	.recalc_rate = clk_factors_recalc_rate,
	.round_rate = clk_factors_round_rate,
	.set_rate = clk_factors_set_rate,
};

/**
 * clk_register_factors - register a factors clock with
 * the clock framework
 * @dev: device registering this clock
 * @name: name of this clock
 * @parent_name: name of clock's parent
 * @flags: framework-specific flags
 * @reg: register address to adjust factors
 * @config: shift and width of factors n, k, m and p
 * @get_factors: function to calculate the factors for a given frequency
 * @lock: shared register lock for this clock
 */
struct clk *clk_register_factors(struct device *dev, const char *name,
				 const char *parent_name,
				 unsigned long flags, void __iomem *reg,
				 struct clk_factors_config *config,
				 void (*get_factors)(u32 *rate, u32 parent,
						     u8 *n, u8 *k, u8 *m, u8 *p),
				 spinlock_t *lock)
{
	struct clk_factors *factors;
	struct clk *clk;
	struct clk_init_data init;

	/* allocate the factors */
	factors = kzalloc(sizeof(struct clk_factors), GFP_KERNEL);
	if (!factors) {
		pr_err("%s: could not allocate factors clk\n", __func__);
		return ERR_PTR(-ENOMEM);
	}

	init.name = name;
	init.ops = &clk_factors_ops;
	init.flags = flags;
	init.parent_names = (parent_name ? &parent_name : NULL);
	init.num_parents = (parent_name ? 1 : 0);

	/* struct clk_factors assignments */
	factors->reg = reg;
	factors->config = config;
	factors->lock = lock;
	factors->hw.init = &init;
	factors->get_factors = get_factors;

	/* register the clock */
	clk = clk_register(dev, &factors->hw);

	if (IS_ERR(clk))
		kfree(factors);

	return clk;
=======
	.determine_rate = clk_factors_determine_rate,
	.recalc_rate = clk_factors_recalc_rate,
	.set_rate = clk_factors_set_rate,
};

struct clk *sunxi_factors_register(struct device_node *node,
				   const struct factors_data *data,
				   spinlock_t *lock,
				   void __iomem *reg)
{
	struct clk *clk;
	struct clk_factors *factors;
	struct clk_gate *gate = NULL;
	struct clk_mux *mux = NULL;
	struct clk_hw *gate_hw = NULL;
	struct clk_hw *mux_hw = NULL;
	const char *clk_name = node->name;
	const char *parents[FACTORS_MAX_PARENTS];
	int ret, i = 0;

	/* if we have a mux, we will have >1 parents */
	i = of_clk_parent_fill(node, parents, FACTORS_MAX_PARENTS);

	/*
	 * some factor clocks, such as pll5 and pll6, may have multiple
	 * outputs, and have their name designated in factors_data
	 */
	if (data->name)
		clk_name = data->name;
	else
		of_property_read_string(node, "clock-output-names", &clk_name);

	factors = kzalloc(sizeof(struct clk_factors), GFP_KERNEL);
	if (!factors)
		goto err_factors;

	/* set up factors properties */
	factors->reg = reg;
	factors->config = data->table;
	factors->get_factors = data->getter;
	factors->recalc = data->recalc;
	factors->lock = lock;

	/* Add a gate if this factor clock can be gated */
	if (data->enable) {
		gate = kzalloc(sizeof(struct clk_gate), GFP_KERNEL);
		if (!gate)
			goto err_gate;

		factors->gate = gate;

		/* set up gate properties */
		gate->reg = reg;
		gate->bit_idx = data->enable;
		gate->lock = factors->lock;
		gate_hw = &gate->hw;
	}

	/* Add a mux if this factor clock can be muxed */
	if (data->mux) {
		mux = kzalloc(sizeof(struct clk_mux), GFP_KERNEL);
		if (!mux)
			goto err_mux;

		factors->mux = mux;

		/* set up gate properties */
		mux->reg = reg;
		mux->shift = data->mux;
		mux->mask = data->muxmask;
		mux->lock = factors->lock;
		mux_hw = &mux->hw;
	}

	clk = clk_register_composite(NULL, clk_name,
			parents, i,
			mux_hw, &clk_mux_ops,
			&factors->hw, &clk_factors_ops,
			gate_hw, &clk_gate_ops, 0);
	if (IS_ERR(clk))
		goto err_register;

	ret = of_clk_add_provider(node, of_clk_src_simple_get, clk);
	if (ret)
		goto err_provider;

	return clk;

err_provider:
	/* TODO: The composite clock stuff will leak a bit here. */
	clk_unregister(clk);
err_register:
	kfree(mux);
err_mux:
	kfree(gate);
err_gate:
	kfree(factors);
err_factors:
	return NULL;
}

void sunxi_factors_unregister(struct device_node *node, struct clk *clk)
{
	struct clk_hw *hw = __clk_get_hw(clk);
	struct clk_factors *factors;
	const char *name;

	if (!hw)
		return;

	factors = to_clk_factors(hw);
	name = clk_hw_get_name(hw);

	of_clk_del_provider(node);
	/* TODO: The composite clock stuff will leak a bit here. */
	clk_unregister(clk);
	kfree(factors->mux);
	kfree(factors->gate);
	kfree(factors);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
