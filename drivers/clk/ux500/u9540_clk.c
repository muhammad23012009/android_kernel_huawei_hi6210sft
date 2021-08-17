/*
 * Clock definitions for u9540 platform.
 *
 * Copyright (C) 2012 ST-Ericsson SA
 * Author: Ulf Hansson <ulf.hansson@linaro.org>
 *
 * License terms: GNU General Public License (GPL) version 2
 */

<<<<<<< HEAD
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/clk-provider.h>
#include <linux/mfd/dbx500-prcmu.h>
#include <linux/platform_data/clk-ux500.h>

#include "clk.h"

void u9540_clk_init(void)
{
	/* register clocks here */
}
=======
#include <linux/clk-provider.h>
#include <linux/mfd/dbx500-prcmu.h>
#include "clk.h"

static void u9540_clk_init(struct device_node *np)
{
	/* register clocks here */
}
CLK_OF_DECLARE(u9540_clks, "stericsson,u9540-clks", u9540_clk_init);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
