/*
 * am33xx-restart.c - Code common to all AM33xx machines.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
<<<<<<< HEAD

#include "common.h"
#include "prm-regbits-33xx.h"
#include "prm33xx.h"
=======
#include <linux/reboot.h>

#include "common.h"
#include "prm.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * am3xx_restart - trigger a software restart of the SoC
 * @mode: the "reboot mode", see arch/arm/kernel/{setup,process}.c
 * @cmd: passed from the userspace program rebooting the system (if provided)
 *
 * Resets the SoC.  For @cmd, see the 'reboot' syscall in
 * kernel/sys.c.  No return value.
 */
<<<<<<< HEAD
void am33xx_restart(char mode, const char *cmd)
{
	/* TODO: Handle mode and cmd if necessary */

	am33xx_prm_rmw_reg_bits(AM33XX_GLOBAL_WARM_SW_RST_MASK,
				AM33XX_GLOBAL_WARM_SW_RST_MASK,
				AM33XX_PRM_DEVICE_MOD,
				AM33XX_PRM_RSTCTRL_OFFSET);

	/* OCP barrier */
	(void)am33xx_prm_read_reg(AM33XX_PRM_DEVICE_MOD,
				  AM33XX_PRM_RSTCTRL_OFFSET);
=======
void am33xx_restart(enum reboot_mode mode, const char *cmd)
{
	/* TODO: Handle mode and cmd if necessary */

	omap_prm_reset_system();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
