/*
 * Device Tree support for Allwinner A1X SoCs
 *
 * Copyright (C) 2012 Maxime Ripard
 *
 * Maxime Ripard <maxime.ripard@free-electrons.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

<<<<<<< HEAD
#include <linux/clocksource.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/irqchip.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/io.h>

#include <linux/clk/sunxi.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/system_misc.h>

#include "sunxi.h"

#define SUN4I_WATCHDOG_CTRL_REG		0x00
#define SUN4I_WATCHDOG_CTRL_RESTART		(1 << 0)
#define SUN4I_WATCHDOG_MODE_REG		0x04
#define SUN4I_WATCHDOG_MODE_ENABLE		(1 << 0)
#define SUN4I_WATCHDOG_MODE_RESET_ENABLE	(1 << 1)

static void __iomem *wdt_base;

static void sun4i_restart(char mode, const char *cmd)
{
	if (!wdt_base)
		return;

	/* Enable timer and set reset bit in the watchdog */
	writel(SUN4I_WATCHDOG_MODE_ENABLE | SUN4I_WATCHDOG_MODE_RESET_ENABLE,
	       wdt_base + SUN4I_WATCHDOG_MODE_REG);

	/*
	 * Restart the watchdog. The default (and lowest) interval
	 * value for the watchdog is 0.5s.
	 */
	writel(SUN4I_WATCHDOG_CTRL_RESTART, wdt_base + SUN4I_WATCHDOG_CTRL_REG);

	while (1) {
		mdelay(5);
		writel(SUN4I_WATCHDOG_MODE_ENABLE | SUN4I_WATCHDOG_MODE_RESET_ENABLE,
		       wdt_base + SUN4I_WATCHDOG_MODE_REG);
	}
}

static struct of_device_id sunxi_restart_ids[] = {
	{ .compatible = "allwinner,sun4i-wdt", .data = sun4i_restart },
	{ /*sentinel*/ }
};

static void sunxi_setup_restart(void)
{
	const struct of_device_id *of_id;
	struct device_node *np;

	np = of_find_matching_node(NULL, sunxi_restart_ids);
	if (WARN(!np, "unable to setup watchdog restart"))
		return;

	wdt_base = of_iomap(np, 0);
	WARN(!wdt_base, "failed to map watchdog base address");

	of_id = of_match_node(sunxi_restart_ids, np);
	WARN(!of_id, "restart function not available");

	arm_pm_restart = of_id->data;
}

static struct map_desc sunxi_io_desc[] __initdata = {
	{
		.virtual	= (unsigned long) SUNXI_REGS_VIRT_BASE,
		.pfn		= __phys_to_pfn(SUNXI_REGS_PHYS_BASE),
		.length		= SUNXI_REGS_SIZE,
		.type		= MT_DEVICE,
	},
};

void __init sunxi_map_io(void)
{
	iotable_init(sunxi_io_desc, ARRAY_SIZE(sunxi_io_desc));
}

static void __init sunxi_timer_init(void)
{
	sunxi_init_clocks();
	clocksource_of_init();
}

static void __init sunxi_dt_init(void)
{
	sunxi_setup_restart();

	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

static const char * const sunxi_board_dt_compat[] = {
	"allwinner,sun4i-a10",
	"allwinner,sun5i-a13",
	NULL,
};

DT_MACHINE_START(SUNXI_DT, "Allwinner A1X (Device Tree)")
	.init_machine	= sunxi_dt_init,
	.map_io		= sunxi_map_io,
	.init_irq	= irqchip_init,
	.init_time	= sunxi_timer_init,
	.dt_compat	= sunxi_board_dt_compat,
=======
#include <linux/clk-provider.h>
#include <linux/clocksource.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>

static const char * const sunxi_board_dt_compat[] = {
	"allwinner,sun4i-a10",
	"allwinner,sun5i-a10s",
	"allwinner,sun5i-a13",
	"allwinner,sun5i-r8",
	"nextthing,gr8",
	NULL,
};

DT_MACHINE_START(SUNXI_DT, "Allwinner sun4i/sun5i Families")
	.dt_compat	= sunxi_board_dt_compat,
MACHINE_END

static const char * const sun6i_board_dt_compat[] = {
	"allwinner,sun6i-a31",
	"allwinner,sun6i-a31s",
	NULL,
};

extern void __init sun6i_reset_init(void);
static void __init sun6i_timer_init(void)
{
	of_clk_init(NULL);
	if (IS_ENABLED(CONFIG_RESET_CONTROLLER))
		sun6i_reset_init();
	clocksource_probe();
}

DT_MACHINE_START(SUN6I_DT, "Allwinner sun6i (A31) Family")
	.init_time	= sun6i_timer_init,
	.dt_compat	= sun6i_board_dt_compat,
MACHINE_END

static const char * const sun7i_board_dt_compat[] = {
	"allwinner,sun7i-a20",
	NULL,
};

DT_MACHINE_START(SUN7I_DT, "Allwinner sun7i (A20) Family")
	.dt_compat	= sun7i_board_dt_compat,
MACHINE_END

static const char * const sun8i_board_dt_compat[] = {
	"allwinner,sun8i-a23",
	"allwinner,sun8i-a33",
	"allwinner,sun8i-a83t",
	"allwinner,sun8i-h3",
	NULL,
};

DT_MACHINE_START(SUN8I_DT, "Allwinner sun8i Family")
	.init_time	= sun6i_timer_init,
	.dt_compat	= sun8i_board_dt_compat,
MACHINE_END

static const char * const sun9i_board_dt_compat[] = {
	"allwinner,sun9i-a80",
	NULL,
};

DT_MACHINE_START(SUN9I_DT, "Allwinner sun9i Family")
	.dt_compat	= sun9i_board_dt_compat,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MACHINE_END
