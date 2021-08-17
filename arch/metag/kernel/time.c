/*
 * Copyright (C) 2005-2013 Imagination Technologies Ltd.
 *
 * This file contains the Meta-specific time handling details.
 *
 */

<<<<<<< HEAD
#include <linux/init.h>

#include <clocksource/metag_generic.h>

void __init time_init(void)
{
=======
#include <clocksource/metag_generic.h>
#include <linux/clk-provider.h>
#include <linux/init.h>
#include <asm/clock.h>

void __init time_init(void)
{
#ifdef CONFIG_COMMON_CLK
	/* Init clocks from device tree */
	of_clk_init(NULL);
#endif

	/* Init meta clocks, particularly the core clock */
	init_metag_clocks();

	/* Set up the timer clock sources */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	metag_generic_timer_init();
}
