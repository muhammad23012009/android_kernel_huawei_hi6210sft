/*
<<<<<<< HEAD
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
 *
 * ########################################################################
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 *
 * Reset the MIPS boards.
 *
 */
#include <linux/init.h>
#include <linux/pm.h>

#include <asm/io.h>
#include <asm/reboot.h>
#include <asm/mips-boards/generic.h>

static void mips_machine_restart(char *command)
{
	unsigned int __iomem *softres_reg =
		ioremap(SOFTRES_REG, sizeof(unsigned int));

	__raw_writel(GORESET, softres_reg);
}

static void mips_machine_halt(void)
{
	unsigned int __iomem *softres_reg =
		ioremap(SOFTRES_REG, sizeof(unsigned int));

	__raw_writel(GORESET, softres_reg);
}


static int __init mips_reboot_setup(void)
{
	_machine_restart = mips_machine_restart;
	_machine_halt = mips_machine_halt;
	pm_power_off = mips_machine_halt;

	return 0;
}

=======
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
 */
#include <linux/io.h>
#include <linux/pm.h>
#include <linux/reboot.h>

#include <asm/reboot.h>
#include <asm/mach-malta/malta-pm.h>

static void mips_machine_power_off(void)
{
	mips_pm_suspend(PIIX4_FUNC3IO_PMCNTRL_SUS_TYP_SOFF);

	pr_info("Failed to power down, resetting\n");
	machine_restart(NULL);
}

static int __init mips_reboot_setup(void)
{
	pm_power_off = mips_machine_power_off;

	return 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
arch_initcall(mips_reboot_setup);
