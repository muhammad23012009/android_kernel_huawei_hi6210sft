/*
 * arch/arm/plat-spear/restart.c
 *
 * SPEAr platform specific restart functions
 *
 * Copyright (C) 2009 ST Microelectronics
<<<<<<< HEAD
 * Viresh Kumar <viresh.linux@gmail.com>
=======
 * Viresh Kumar <vireshk@kernel.org>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <linux/io.h>
#include <linux/amba/sp810.h>
<<<<<<< HEAD
=======
#include <linux/reboot.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/system_misc.h>
#include <mach/spear.h>
#include "generic.h"

#define SPEAR13XX_SYS_SW_RES			(VA_MISC_BASE + 0x204)
<<<<<<< HEAD
void spear_restart(char mode, const char *cmd)
{
	if (mode == 's') {
=======
void spear_restart(enum reboot_mode mode, const char *cmd)
{
	if (mode == REBOOT_SOFT) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* software reset, Jump into ROM at address 0 */
		soft_restart(0);
	} else {
		/* hardware reset, Use on-chip reset capability */
#ifdef CONFIG_ARCH_SPEAR13XX
		writel_relaxed(0x01, SPEAR13XX_SYS_SW_RES);
#endif
#if defined(CONFIG_ARCH_SPEAR3XX) || defined(CONFIG_ARCH_SPEAR6XX)
		sysctl_soft_reset((void __iomem *)VA_SPEAR_SYS_CTRL_BASE);
#endif
	}
}
