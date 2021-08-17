#ifndef __ASM_CLKDEV__H_
#define __ASM_CLKDEV__H_

#include <linux/slab.h>

static inline struct clk_lookup_alloc *__clkdev_alloc(size_t size)
{
	return kzalloc(size, GFP_KERNEL);
}

<<<<<<< HEAD
#define __clk_put(clk)
#define __clk_get(clk) ({ 1; })
=======
#ifndef CONFIG_COMMON_CLK
#define __clk_put(clk)
#define __clk_get(clk) ({ 1; })
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
