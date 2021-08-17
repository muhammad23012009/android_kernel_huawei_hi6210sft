/*
 * Copyright (C) 2012 Altera Corporation
 * Copyright (c) 2011 Picochip Ltd., Jamie Iles
 *
 * Modified from mach-picoxcell/time.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
<<<<<<< HEAD
=======
#include <linux/delay.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/dw_apb_timer.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
<<<<<<< HEAD

#include <asm/mach/time.h>
#include <asm/sched_clock.h>

static void timer_get_base_and_rate(struct device_node *np,
				    void __iomem **base, u32 *rate)
{
=======
#include <linux/clk.h>
#include <linux/sched_clock.h>

static void __init timer_get_base_and_rate(struct device_node *np,
				    void __iomem **base, u32 *rate)
{
	struct clk *timer_clk;
	struct clk *pclk;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	*base = of_iomap(np, 0);

	if (!*base)
		panic("Unable to map regs for %s", np->name);

<<<<<<< HEAD
	if (of_property_read_u32(np, "clock-freq", rate) &&
		of_property_read_u32(np, "clock-frequency", rate))
		panic("No clock-frequency property for %s", np->name);
}

static void add_clockevent(struct device_node *event_timer)
=======
	/*
	 * Not all implementations use a periphal clock, so don't panic
	 * if it's not present
	 */
	pclk = of_clk_get_by_name(np, "pclk");
	if (!IS_ERR(pclk))
		if (clk_prepare_enable(pclk))
			pr_warn("pclk for %s is present, but could not be activated\n",
				np->name);

	timer_clk = of_clk_get_by_name(np, "timer");
	if (IS_ERR(timer_clk))
		goto try_clock_freq;

	if (!clk_prepare_enable(timer_clk)) {
		*rate = clk_get_rate(timer_clk);
		return;
	}

try_clock_freq:
	if (of_property_read_u32(np, "clock-freq", rate) &&
	    of_property_read_u32(np, "clock-frequency", rate))
		panic("No clock nor clock-frequency property for %s", np->name);
}

static void __init add_clockevent(struct device_node *event_timer)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	void __iomem *iobase;
	struct dw_apb_clock_event_device *ced;
	u32 irq, rate;

	irq = irq_of_parse_and_map(event_timer, 0);
	if (irq == 0)
		panic("No IRQ for clock event timer");

	timer_get_base_and_rate(event_timer, &iobase, &rate);

	ced = dw_apb_clockevent_init(0, event_timer->name, 300, iobase, irq,
				     rate);
	if (!ced)
		panic("Unable to initialise clockevent device");

	dw_apb_clockevent_register(ced);
}

<<<<<<< HEAD
static void add_clocksource(struct device_node *source_timer)
=======
static void __iomem *sched_io_base;
static u32 sched_rate;

static void __init add_clocksource(struct device_node *source_timer)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	void __iomem *iobase;
	struct dw_apb_clocksource *cs;
	u32 rate;

	timer_get_base_and_rate(source_timer, &iobase, &rate);

	cs = dw_apb_clocksource_init(300, source_timer->name, iobase, rate);
	if (!cs)
		panic("Unable to initialise clocksource device");

	dw_apb_clocksource_start(cs);
	dw_apb_clocksource_register(cs);
<<<<<<< HEAD
}

static void __iomem *sched_io_base;

static u32 read_sched_clock(void)
{
	return ~__raw_readl(sched_io_base);
=======

	/*
	 * Fallback to use the clocksource as sched_clock if no separate
	 * timer is found. sched_io_base then points to the current_value
	 * register of the clocksource timer.
	 */
	sched_io_base = iobase + 0x04;
	sched_rate = rate;
}

static u64 notrace read_sched_clock(void)
{
	return ~readl_relaxed(sched_io_base);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static const struct of_device_id sptimer_ids[] __initconst = {
	{ .compatible = "picochip,pc3x2-rtc" },
<<<<<<< HEAD
	{ .compatible = "snps,dw-apb-timer-sp" },
	{ /* Sentinel */ },
};

static void init_sched_clock(void)
{
	struct device_node *sched_timer;
	u32 rate;

	sched_timer = of_find_matching_node(NULL, sptimer_ids);
	if (!sched_timer)
		panic("No RTC for sched clock to use");

	timer_get_base_and_rate(sched_timer, &sched_io_base, &rate);
	of_node_put(sched_timer);

	setup_sched_clock(read_sched_clock, 32, rate);
}

static const struct of_device_id osctimer_ids[] __initconst = {
	{ .compatible = "picochip,pc3x2-timer" },
	{ .compatible = "snps,dw-apb-timer-osc" },
	{},
};

void __init dw_apb_timer_init(void)
{
	struct device_node *event_timer, *source_timer;

	event_timer = of_find_matching_node(NULL, osctimer_ids);
	if (!event_timer)
		panic("No timer for clockevent");
	add_clockevent(event_timer);

	source_timer = of_find_matching_node(event_timer, osctimer_ids);
	if (!source_timer)
		panic("No timer for clocksource");
	add_clocksource(source_timer);

	of_node_put(source_timer);

	init_sched_clock();
}
=======
	{ /* Sentinel */ },
};

static void __init init_sched_clock(void)
{
	struct device_node *sched_timer;

	sched_timer = of_find_matching_node(NULL, sptimer_ids);
	if (sched_timer) {
		timer_get_base_and_rate(sched_timer, &sched_io_base,
					&sched_rate);
		of_node_put(sched_timer);
	}

	sched_clock_register(read_sched_clock, 32, sched_rate);
}

#ifdef CONFIG_ARM
static unsigned long dw_apb_delay_timer_read(void)
{
	return ~readl_relaxed(sched_io_base);
}

static struct delay_timer dw_apb_delay_timer = {
	.read_current_timer	= dw_apb_delay_timer_read,
};
#endif

static int num_called;
static int __init dw_apb_timer_init(struct device_node *timer)
{
	switch (num_called) {
	case 1:
		pr_debug("%s: found clocksource timer\n", __func__);
		add_clocksource(timer);
		init_sched_clock();
#ifdef CONFIG_ARM
		dw_apb_delay_timer.freq = sched_rate;
		register_current_timer_delay(&dw_apb_delay_timer);
#endif
		break;
	default:
		pr_debug("%s: found clockevent timer\n", __func__);
		add_clockevent(timer);
		break;
	}

	num_called++;

	return 0;
}
CLOCKSOURCE_OF_DECLARE(pc3x2_timer, "picochip,pc3x2-timer", dw_apb_timer_init);
CLOCKSOURCE_OF_DECLARE(apb_timer_osc, "snps,dw-apb-timer-osc", dw_apb_timer_init);
CLOCKSOURCE_OF_DECLARE(apb_timer_sp, "snps,dw-apb-timer-sp", dw_apb_timer_init);
CLOCKSOURCE_OF_DECLARE(apb_timer, "snps,dw-apb-timer", dw_apb_timer_init);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
