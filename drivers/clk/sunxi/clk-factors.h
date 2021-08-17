#ifndef __MACH_SUNXI_CLK_FACTORS_H
#define __MACH_SUNXI_CLK_FACTORS_H

#include <linux/clk-provider.h>
<<<<<<< HEAD
#include <linux/clkdev.h>
=======
#include <linux/spinlock.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define SUNXI_FACTORS_NOT_APPLICABLE	(0)

struct clk_factors_config {
	u8 nshift;
	u8 nwidth;
	u8 kshift;
	u8 kwidth;
	u8 mshift;
	u8 mwidth;
	u8 pshift;
	u8 pwidth;
	u8 n_start;
};

<<<<<<< HEAD
struct clk *clk_register_factors(struct device *dev, const char *name,
				 const char *parent_name,
				 unsigned long flags, void __iomem *reg,
				 struct clk_factors_config *config,
				 void (*get_factors) (u32 *rate, u32 parent_rate,
						      u8 *n, u8 *k, u8 *m, u8 *p),
				 spinlock_t *lock);
=======
struct factors_request {
	unsigned long rate;
	unsigned long parent_rate;
	u8 parent_index;
	u8 n;
	u8 k;
	u8 m;
	u8 p;
};

struct factors_data {
	int enable;
	int mux;
	int muxmask;
	const struct clk_factors_config *table;
	void (*getter)(struct factors_request *req);
	void (*recalc)(struct factors_request *req);
	const char *name;
};

struct clk_factors {
	struct clk_hw hw;
	void __iomem *reg;
	const struct clk_factors_config *config;
	void (*get_factors)(struct factors_request *req);
	void (*recalc)(struct factors_request *req);
	spinlock_t *lock;
	/* for cleanup */
	struct clk_mux *mux;
	struct clk_gate *gate;
};

struct clk *sunxi_factors_register(struct device_node *node,
				   const struct factors_data *data,
				   spinlock_t *lock,
				   void __iomem *reg);

void sunxi_factors_unregister(struct device_node *node, struct clk *clk);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
