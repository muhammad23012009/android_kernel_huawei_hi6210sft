/*
 * Copyright 2012 Sascha Hauer, Pengutronix
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/irq.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>

#include "common.h"
#include "mx31.h"

<<<<<<< HEAD
static void __init imx31_dt_init(void)
{
	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

static const char *imx31_dt_board_compat[] __initdata = {
=======
static const char * const imx31_dt_board_compat[] __initconst = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	"fsl,imx31",
	NULL
};

static void __init imx31_dt_timer_init(void)
{
	mx31_clocks_init_dt();
}

<<<<<<< HEAD
=======
/* FIXME: replace with DT binding */
static const struct resource imx31_rnga_res[] __initconst = {
	DEFINE_RES_MEM(MX31_RNGA_BASE_ADDR, SZ_16K),
};

static void __init imx31_dt_mach_init(void)
{
	platform_device_register_simple("mxc_rnga", -1, imx31_rnga_res,
					ARRAY_SIZE(imx31_rnga_res));
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
DT_MACHINE_START(IMX31_DT, "Freescale i.MX31 (Device Tree Support)")
	.map_io		= mx31_map_io,
	.init_early	= imx31_init_early,
	.init_irq	= mx31_init_irq,
<<<<<<< HEAD
	.handle_irq	= imx31_handle_irq,
	.init_time	= imx31_dt_timer_init,
	.init_machine	= imx31_dt_init,
	.dt_compat	= imx31_dt_board_compat,
	.restart	= mxc_restart,
=======
	.init_time	= imx31_dt_timer_init,
	.init_machine	= imx31_dt_mach_init,
	.dt_compat	= imx31_dt_board_compat,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MACHINE_END
