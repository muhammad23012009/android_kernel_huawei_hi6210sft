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

#include "pm.h"
#include "sleep.h"
#include "iomap.h"
#include "irq.h"
#include "flowctrl.h"
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
#include "flowctrl.h"
#include "iomap.h"
#include "irq.h"
#include "pm.h"
#include "reset.h"
#include "sleep.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_PM_SLEEP
static bool abort_flag;
static atomic_t abort_barrier;
static int tegra20_idle_lp2_coupled(struct cpuidle_device *dev,
				    struct cpuidle_driver *drv,
				    int index);
#define TEGRA20_MAX_STATES 2
#else
#define TEGRA20_MAX_STATES 1
#endif

static struct cpuidle_driver tegra_idle_driver = {
	.name = "tegra_idle",
	.owner = THIS_MODULE,
	.states = {
		ARM_CPUIDLE_WFI_STATE_PWR(600),
#ifdef CONFIG_PM_SLEEP
		{
			.enter            = tegra20_idle_lp2_coupled,
			.exit_latency     = 5000,
			.target_residency = 10000,
			.power_usage      = 0,
<<<<<<< HEAD
			.flags            = CPUIDLE_FLAG_TIME_VALID |
			CPUIDLE_FLAG_COUPLED,
=======
			.flags            = CPUIDLE_FLAG_COUPLED,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			.name             = "powered-down",
			.desc             = "CPU power gated",
		},
#endif
	},
	.state_count = TEGRA20_MAX_STATES,
	.safe_state_index = 0,
};

#ifdef CONFIG_PM_SLEEP
#ifdef CONFIG_SMP
<<<<<<< HEAD
static void __iomem *pmc = IO_ADDRESS(TEGRA_PMC_BASE);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int tegra20_reset_sleeping_cpu_1(void)
{
	int ret = 0;

	tegra_pen_lock();

<<<<<<< HEAD
	if (readl(pmc + PMC_SCRATCH41) == CPU_RESETTABLE)
=======
	if (readb(tegra20_cpu1_resettable_status) == CPU_RESETTABLE)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		tegra20_cpu_shutdown(1);
	else
		ret = -EINVAL;

	tegra_pen_unlock();

	return ret;
}

static void tegra20_wake_cpu1_from_reset(void)
{
	tegra_pen_lock();

	tegra20_cpu_clear_resettable();

	/* enable cpu clock on cpu */
	tegra_enable_cpu_clock(1);

	/* take the CPU out of reset */
	tegra_cpu_out_of_reset(1);

	/* unhalt the cpu */
	flowctrl_write_cpu_halt(1, 0);

	tegra_pen_unlock();
}

static int tegra20_reset_cpu_1(void)
{
	if (!cpu_online(1) || !tegra20_reset_sleeping_cpu_1())
		return 0;

	tegra20_wake_cpu1_from_reset();
	return -EBUSY;
}
#else
static inline void tegra20_wake_cpu1_from_reset(void)
{
}

static inline int tegra20_reset_cpu_1(void)
{
	return 0;
}
#endif

static bool tegra20_cpu_cluster_power_down(struct cpuidle_device *dev,
					   struct cpuidle_driver *drv,
					   int index)
{
	while (tegra20_cpu_is_resettable_soon())
		cpu_relax();

	if (tegra20_reset_cpu_1() || !tegra_cpu_rail_off_ready())
		return false;

<<<<<<< HEAD
	clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_ENTER, &dev->cpu);

	tegra_idle_lp2_last();

	clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_EXIT, &dev->cpu);
=======
	tick_broadcast_enter();

	tegra_idle_lp2_last();

	tick_broadcast_exit();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (cpu_online(1))
		tegra20_wake_cpu1_from_reset();

	return true;
}

#ifdef CONFIG_SMP
static bool tegra20_idle_enter_lp2_cpu_1(struct cpuidle_device *dev,
					 struct cpuidle_driver *drv,
					 int index)
{
<<<<<<< HEAD
	clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_ENTER, &dev->cpu);
=======
	tick_broadcast_enter();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	cpu_suspend(0, tegra20_sleep_cpu_secondary_finish);

	tegra20_cpu_clear_resettable();

<<<<<<< HEAD
	clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_EXIT, &dev->cpu);
=======
	tick_broadcast_exit();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return true;
}
#else
static inline bool tegra20_idle_enter_lp2_cpu_1(struct cpuidle_device *dev,
						struct cpuidle_driver *drv,
						int index)
{
	return true;
}
#endif

static int tegra20_idle_lp2_coupled(struct cpuidle_device *dev,
				    struct cpuidle_driver *drv,
				    int index)
{
<<<<<<< HEAD
	u32 cpu = is_smp() ? cpu_logical_map(dev->cpu) : dev->cpu;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bool entered_lp2 = false;

	if (tegra_pending_sgi())
		ACCESS_ONCE(abort_flag) = true;

	cpuidle_coupled_parallel_barrier(dev, &abort_barrier);

	if (abort_flag) {
		cpuidle_coupled_parallel_barrier(dev, &abort_barrier);
		abort_flag = false;	/* clean flag for next coming */
		return -EINTR;
	}

	local_fiq_disable();

<<<<<<< HEAD
	tegra_set_cpu_in_lp2(cpu);
	cpu_pm_enter();

	if (cpu == 0)
=======
	tegra_set_cpu_in_lp2();
	cpu_pm_enter();

	if (dev->cpu == 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		entered_lp2 = tegra20_cpu_cluster_power_down(dev, drv, index);
	else
		entered_lp2 = tegra20_idle_enter_lp2_cpu_1(dev, drv, index);

	cpu_pm_exit();
<<<<<<< HEAD
	tegra_clear_cpu_in_lp2(cpu);
=======
	tegra_clear_cpu_in_lp2();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	local_fiq_enable();

	smp_rmb();

	return entered_lp2 ? index : 0;
}
#endif

<<<<<<< HEAD
int __init tegra20_cpuidle_init(void)
{
#ifdef CONFIG_PM_SLEEP
	tegra_tear_down_cpu = tegra20_tear_down_cpu;
#endif
=======
/*
 * Tegra20 HW appears to have a bug such that PCIe device interrupts, whether
 * they are legacy IRQs or MSI, are lost when LP2 is enabled. To work around
 * this, simply disable LP2 if the PCI driver and DT node are both enabled.
 */
void tegra20_cpuidle_pcie_irqs_in_use(void)
{
	pr_info_once(
		"Disabling cpuidle LP2 state, since PCIe IRQs are in use\n");
	tegra_idle_driver.states[1].disabled = true;
}

int __init tegra20_cpuidle_init(void)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return cpuidle_register(&tegra_idle_driver, cpu_possible_mask);
}
