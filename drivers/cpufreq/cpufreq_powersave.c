/*
<<<<<<< HEAD
 *  linux/drivers/cpufreq/cpufreq_powersave.c
 *
 *  Copyright (C) 2002 - 2003 Dominik Brodowski <linux@brodo.de>
=======
 * linux/drivers/cpufreq/cpufreq_powersave.c
 *
 * Copyright (C) 2002 - 2003 Dominik Brodowski <linux@brodo.de>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cpufreq.h>
#include <linux/init.h>

static int cpufreq_governor_powersave(struct cpufreq_policy *policy,
					unsigned int event)
{
	switch (event) {
	case CPUFREQ_GOV_START:
	case CPUFREQ_GOV_LIMITS:
		pr_debug("setting to %u kHz because of event %u\n",
							policy->min, event);
		__cpufreq_driver_target(policy, policy->min,
						CPUFREQ_RELATION_L);
		break;
	default:
		break;
	}
	return 0;
=======
#include <linux/cpufreq.h>
#include <linux/init.h>
#include <linux/module.h>

static void cpufreq_gov_powersave_limits(struct cpufreq_policy *policy)
{
	pr_debug("setting to %u kHz\n", policy->min);
	__cpufreq_driver_target(policy, policy->min, CPUFREQ_RELATION_L);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifndef CONFIG_CPU_FREQ_DEFAULT_GOV_POWERSAVE
static
#endif
struct cpufreq_governor cpufreq_gov_powersave = {
	.name		= "powersave",
<<<<<<< HEAD
	.governor	= cpufreq_governor_powersave,
=======
	.limits		= cpufreq_gov_powersave_limits,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.owner		= THIS_MODULE,
};

static int __init cpufreq_gov_powersave_init(void)
{
	return cpufreq_register_governor(&cpufreq_gov_powersave);
}

<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void __exit cpufreq_gov_powersave_exit(void)
{
	cpufreq_unregister_governor(&cpufreq_gov_powersave);
}

<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_AUTHOR("Dominik Brodowski <linux@brodo.de>");
MODULE_DESCRIPTION("CPUfreq policy governor 'powersave'");
MODULE_LICENSE("GPL");

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_POWERSAVE
<<<<<<< HEAD
=======
struct cpufreq_governor *cpufreq_default_governor(void)
{
	return &cpufreq_gov_powersave;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
fs_initcall(cpufreq_gov_powersave_init);
#else
module_init(cpufreq_gov_powersave_init);
#endif
module_exit(cpufreq_gov_powersave_exit);
