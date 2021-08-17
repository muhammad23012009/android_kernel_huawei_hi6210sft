/*
 *  linux/arch/arm/mach-mmp/mmp-dt.c
 *
 *  Copyright (C) 2012 Marvell Technology Group Ltd.
 *  Author: Haojian Zhuang <haojian.zhuang@marvell.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  publishhed by the Free Software Foundation.
 */

<<<<<<< HEAD
#include <linux/irq.h>
#include <linux/irqdomain.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <mach/irqs.h>

#include "common.h"

extern void __init mmp_dt_irq_init(void);
extern void __init mmp_dt_init_timer(void);

static const struct of_dev_auxdata pxa168_auxdata_lookup[] __initconst = {
	OF_DEV_AUXDATA("mrvl,mmp-uart", 0xd4017000, "pxa2xx-uart.0", NULL),
	OF_DEV_AUXDATA("mrvl,mmp-uart", 0xd4018000, "pxa2xx-uart.1", NULL),
	OF_DEV_AUXDATA("mrvl,mmp-uart", 0xd4026000, "pxa2xx-uart.2", NULL),
	OF_DEV_AUXDATA("mrvl,mmp-twsi", 0xd4011000, "pxa2xx-i2c.0", NULL),
	OF_DEV_AUXDATA("mrvl,mmp-twsi", 0xd4025000, "pxa2xx-i2c.1", NULL),
	OF_DEV_AUXDATA("marvell,mmp-gpio", 0xd4019000, "mmp-gpio", NULL),
	OF_DEV_AUXDATA("mrvl,mmp-rtc", 0xd4010000, "sa1100-rtc", NULL),
	{}
};

static const struct of_dev_auxdata pxa910_auxdata_lookup[] __initconst = {
	OF_DEV_AUXDATA("mrvl,mmp-uart", 0xd4017000, "pxa2xx-uart.0", NULL),
	OF_DEV_AUXDATA("mrvl,mmp-uart", 0xd4018000, "pxa2xx-uart.1", NULL),
	OF_DEV_AUXDATA("mrvl,mmp-uart", 0xd4036000, "pxa2xx-uart.2", NULL),
	OF_DEV_AUXDATA("mrvl,mmp-twsi", 0xd4011000, "pxa2xx-i2c.0", NULL),
	OF_DEV_AUXDATA("mrvl,mmp-twsi", 0xd4037000, "pxa2xx-i2c.1", NULL),
	OF_DEV_AUXDATA("marvell,mmp-gpio", 0xd4019000, "mmp-gpio", NULL),
	OF_DEV_AUXDATA("mrvl,mmp-rtc", 0xd4010000, "sa1100-rtc", NULL),
	{}
};

static void __init pxa168_dt_init(void)
{
	of_platform_populate(NULL, of_default_bus_match_table,
			     pxa168_auxdata_lookup, NULL);
}

static void __init pxa910_dt_init(void)
{
	of_platform_populate(NULL, of_default_bus_match_table,
			     pxa910_auxdata_lookup, NULL);
}

static const char *mmp_dt_board_compat[] __initdata = {
	"mrvl,pxa168-aspenite",
	"mrvl,pxa910-dkb",
	NULL,
};

DT_MACHINE_START(PXA168_DT, "Marvell PXA168 (Device Tree Support)")
	.map_io		= mmp_map_io,
	.init_irq	= mmp_dt_irq_init,
	.init_time	= mmp_dt_init_timer,
	.init_machine	= pxa168_dt_init,
	.dt_compat	= mmp_dt_board_compat,
=======
#include <linux/irqchip.h>
#include <linux/of_platform.h>
#include <linux/clk-provider.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/hardware/cache-tauros2.h>

#include "common.h"

extern void __init mmp_dt_init_timer(void);

static const char *const pxa168_dt_board_compat[] __initconst = {
	"mrvl,pxa168-aspenite",
	NULL,
};

static const char *const pxa910_dt_board_compat[] __initconst = {
	"mrvl,pxa910-dkb",
	NULL,
};

static void __init mmp_init_time(void)
{
#ifdef CONFIG_CACHE_TAUROS2
	tauros2_init(0);
#endif
	mmp_dt_init_timer();
	of_clk_init(NULL);
}

DT_MACHINE_START(PXA168_DT, "Marvell PXA168 (Device Tree Support)")
	.map_io		= mmp_map_io,
	.init_time	= mmp_init_time,
	.dt_compat	= pxa168_dt_board_compat,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MACHINE_END

DT_MACHINE_START(PXA910_DT, "Marvell PXA910 (Device Tree Support)")
	.map_io		= mmp_map_io,
<<<<<<< HEAD
	.init_irq	= mmp_dt_irq_init,
	.init_time	= mmp_dt_init_timer,
	.init_machine	= pxa910_dt_init,
	.dt_compat	= mmp_dt_board_compat,
=======
	.init_time	= mmp_init_time,
	.dt_compat	= pxa910_dt_board_compat,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MACHINE_END
