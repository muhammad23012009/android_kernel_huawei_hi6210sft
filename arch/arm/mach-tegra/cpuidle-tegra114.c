/*
 * Copyright (c) 2013, NVIDIA Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cpuidle.h>

#include <asm/cpuidle.h>
=======
#include <asm/firmware.h>
#include <linux/tick.h>
#include <linux/cpuidle.h>
#include <linux/cpu_pm.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <asm/cpuidle.h>
#include <asm/smp_plat.h>
#include <asm/suspend.h>
#include <asm/psci.h>

#include "cpuidle.h"
#include "pm.h"
#include "sleep.h"

#ifdef CONFIG_PM_SLEEP
#define TEGRA114_MAX_STATES 2
#else
#define TEGRA114_MAX_STATES 1
#endif

#ifdef CONFIG_PM_SLEEP
static int tegra114_idle_power_down(struct cpuidle_device *dev,
				    struct cpuidle_driver *drv,
				    int index)
{
	local_fiq_disable();

	tegra_set_cpu_in_lp2();
	cpu_pm_enter();

	call_firmware_op(prepare_idle);

	/* Do suspend by ourselves if the firmware does not implement it */
	if (call_firmware_op(do_idle, 0) == -ENOSYS)
		cpu_suspend(0, tegra30_sleep_cpu_secondary_finish);

	cpu_pm_exit();
	tegra_clear_cpu_in_lp2();

	local_fiq_enable();

	return index;
}

static void tegra114_idle_enter_freeze(struct cpuidle_device *dev,
				       struct cpuidle_driver *drv,
				       int index)
{
       tegra114_idle_power_down(dev, drv, index);
}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static struct cpuidle_driver tegra_idle_driver = {
	.name = "tegra_idle",
	.owner = THIS_MODULE,
<<<<<<< HEAD
	.state_count = 1,
	.states = {
		[0] = ARM_CPUIDLE_WFI_STATE_PWR(600),
=======
	.state_count = TEGRA114_MAX_STATES,
	.states = {
		[0] = ARM_CPUIDLE_WFI_STATE_PWR(600),
#ifdef CONFIG_PM_SLEEP
		[1] = {
			.enter			= tegra114_idle_power_down,
			.enter_freeze		= tegra114_idle_enter_freeze,
			.exit_latency		= 500,
			.target_residency	= 1000,
			.flags			= CPUIDLE_FLAG_TIMER_STOP,
			.power_usage		= 0,
			.name			= "powered-down",
			.desc			= "CPU power gated",
		},
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
};

int __init tegra114_cpuidle_init(void)
{
<<<<<<< HEAD
	return cpuidle_register(&tegra_idle_driver, NULL);
=======
	if (!psci_smp_available())
		return cpuidle_register(&tegra_idle_driver, NULL);

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
