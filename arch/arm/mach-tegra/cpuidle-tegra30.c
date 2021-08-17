/*
 * CPU idle driver for Tegra CPUs
 *
 * Copyright (c) 2010-2012, NVIDIA Corporation.
 * Copyright (c) 2011 Google, Inc.
 * Author: Colin Cross <ccross@android.com>
 *         Gary King <gking@nvidia.com>
 *
 * Rework for 3.3 by Peter De Schrijver <pdeschrijver@nvidia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cpuidle.h>
#include <linux/cpu_pm.h>
#include <linux/clockchips.h>
#include <linux/clk/tegra.h>

#include <asm/cpuidle.h>
#include <asm/proc-fns.h>
#include <asm/suspend.h>
#include <asm/smp_plat.h>

=======
#include <linux/clk/tegra.h>
#include <linux/tick.h>
#include <linux/cpuidle.h>
#include <linux/cpu_pm.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <asm/cpuidle.h>
#include <asm/smp_plat.h>
#include <asm/suspend.h>

#include "cpuidle.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "pm.h"
#include "sleep.h"

#ifdef CONFIG_PM_SLEEP
static int tegra30_idle_lp2(struct cpuidle_device *dev,
			    struct cpuidle_driver *drv,
			    int index);
#endif

static struct cpuidle_driver tegra_idle_driver = {
	.name = "tegra_idle",
	.owner = THIS_MODULE,
#ifdef CONFIG_PM_SLEEP
	.state_count = 2,
#else
	.state_count = 1,
#endif
	.states = {
		[0] = ARM_CPUIDLE_WFI_STATE_PWR(600),
#ifdef CONFIG_PM_SLEEP
		[1] = {
			.enter			= tegra30_idle_lp2,
			.exit_latency		= 2000,
			.target_residency	= 2200,
			.power_usage		= 0,
<<<<<<< HEAD
			.flags			= CPUIDLE_FLAG_TIME_VALID,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			.name			= "powered-down",
			.desc			= "CPU power gated",
		},
#endif
	},
};

#ifdef CONFIG_PM_SLEEP
static bool tegra30_cpu_cluster_power_down(struct cpuidle_device *dev,
					   struct cpuidle_driver *drv,
					   int index)
{
	/* All CPUs entering LP2 is not working.
	 * Don't let CPU0 enter LP2 when any secondary CPU is online.
	 */
	if (num_online_cpus() > 1 || !tegra_cpu_rail_off_ready()) {
		cpu_do_idle();
		return false;
	}

<<<<<<< HEAD
	clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_ENTER, &dev->cpu);

	tegra_idle_lp2_last();

	clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_EXIT, &dev->cpu);
=======
	tick_broadcast_enter();

	tegra_idle_lp2_last();

	tick_broadcast_exit();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return true;
}

#ifdef CONFIG_SMP
static bool tegra30_cpu_core_power_down(struct cpuidle_device *dev,
					struct cpuidle_driver *drv,
					int index)
{
<<<<<<< HEAD
	clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_ENTER, &dev->cpu);
=======
	tick_broadcast_enter();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	smp_wmb();

	cpu_suspend(0, tegra30_sleep_cpu_secondary_finish);

<<<<<<< HEAD
	clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_EXIT, &dev->cpu);
=======
	tick_broadcast_exit();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return true;
}
#else
static inline bool tegra30_cpu_core_power_down(struct cpuidle_device *dev,
					       struct cpuidle_driver *drv,
					       int index)
{
	return true;
}
#endif

static int tegra30_idle_lp2(struct cpuidle_device *dev,
			    struct cpuidle_driver *drv,
			    int index)
{
<<<<<<< HEAD
	u32 cpu = is_smp() ? cpu_logical_map(dev->cpu) : dev->cpu;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bool entered_lp2 = false;
	bool last_cpu;

	local_fiq_disable();

<<<<<<< HEAD
	last_cpu = tegra_set_cpu_in_lp2(cpu);
	cpu_pm_enter();

	if (cpu == 0) {
=======
	last_cpu = tegra_set_cpu_in_lp2();
	cpu_pm_enter();

	if (dev->cpu == 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (last_cpu)
			entered_lp2 = tegra30_cpu_cluster_power_down(dev, drv,
								     index);
		else
			cpu_do_idle();
	} else {
		entered_lp2 = tegra30_cpu_core_power_down(dev, drv, index);
	}

	cpu_pm_exit();
<<<<<<< HEAD
	tegra_clear_cpu_in_lp2(cpu);
=======
	tegra_clear_cpu_in_lp2();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	local_fiq_enable();

	smp_rmb();

	return (entered_lp2) ? index : 0;
}
#endif

int __init tegra30_cpuidle_init(void)
{
<<<<<<< HEAD
#ifdef CONFIG_PM_SLEEP
	tegra_tear_down_cpu = tegra30_tear_down_cpu;
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return cpuidle_register(&tegra_idle_driver, NULL);
}
