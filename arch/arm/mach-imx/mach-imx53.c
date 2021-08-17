/*
 * Copyright 2011 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2011 Linaro Ltd.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>

#include "common.h"
<<<<<<< HEAD
#include "mx53.h"

static void __init imx53_qsb_init(void)
{
	struct clk *clk;

	clk = clk_get_sys(NULL, "ssi_ext1");
	if (IS_ERR(clk)) {
		pr_err("failed to get clk ssi_ext1\n");
		return;
	}

	clk_register_clkdev(clk, NULL, "0-000a");
=======
#include "hardware.h"

static void __init imx53_init_early(void)
{
	mxc_set_cpu_type(MXC_CPU_MX53);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void __init imx53_dt_init(void)
{
<<<<<<< HEAD
	if (of_machine_is_compatible("fsl,imx53-qsb"))
		imx53_qsb_init();

	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

static const char *imx53_dt_board_compat[] __initdata = {
=======
	imx_src_init();

	imx_aips_allow_unprivileged_access("fsl,imx53-aipstz");
}

static void __init imx53_init_late(void)
{
	imx53_pm_init();
}

static const char * const imx53_dt_board_compat[] __initconst = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	"fsl,imx53",
	NULL
};

<<<<<<< HEAD
static void __init imx53_timer_init(void)
{
	mx53_clocks_init_dt();
}

DT_MACHINE_START(IMX53_DT, "Freescale i.MX53 (Device Tree Support)")
	.map_io		= mx53_map_io,
	.init_early	= imx53_init_early,
	.init_irq	= mx53_init_irq,
	.handle_irq	= imx53_handle_irq,
	.init_time	= imx53_timer_init,
	.init_machine	= imx53_dt_init,
	.init_late	= imx53_init_late,
	.dt_compat	= imx53_dt_board_compat,
	.restart	= mxc_restart,
=======
DT_MACHINE_START(IMX53_DT, "Freescale i.MX53 (Device Tree Support)")
	.init_early	= imx53_init_early,
	.init_machine	= imx53_dt_init,
	.init_late	= imx53_init_late,
	.dt_compat	= imx53_dt_board_compat,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MACHINE_END
