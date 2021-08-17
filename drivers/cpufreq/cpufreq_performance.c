/*
 *  linux/drivers/cpufreq/cpufreq_performance.c
 *
 *  Copyright (C) 2002 - 2003 Dominik Brodowski <linux@brodo.de>
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

#if defined(CONFIG_ARCH_HI3XXX) && defined(CONFIG_HI3XXX_CMDLINE_PARSE)
extern int get_lowbatteryflag(void);
extern void set_lowBatteryflag(int flag);
#ifdef CONFIG_ARCH_HI3630
static int big_is_booting;
#endif
#endif

static int cpufreq_governor_performance(struct cpufreq_policy *policy,
					unsigned int event)
{
#if defined(CONFIG_ARCH_HI3XXX) && defined(CONFIG_HI3XXX_CMDLINE_PARSE)
	unsigned int utarget = policy->max;
#endif

	switch (event) {
	case CPUFREQ_GOV_START:
	case CPUFREQ_GOV_LIMITS:
		pr_debug("setting to %u kHz because of event %u\n",
						policy->max, event);

#if defined(CONFIG_ARCH_HI3XXX) && defined(CONFIG_HI3XXX_CMDLINE_PARSE)
#ifdef CONFIG_ARCH_HI3630
		if (policy->cpu == 4) {
			if (big_is_booting) {
#define BIG_BOOTING_PERFORMANCE_OPERATING_POINT 1708800
				utarget = BIG_BOOTING_PERFORMANCE_OPERATING_POINT;
			}
		}
#endif

		if ((get_lowbatteryflag() == 1) && (policy->cpu == 4))
			utarget = policy->min;

		pr_info("%s utarget=%d\n", __func__, utarget);

		__cpufreq_driver_target(policy, utarget,
			CPUFREQ_RELATION_H);
#else
		__cpufreq_driver_target(policy, policy->max,
						CPUFREQ_RELATION_H);
#endif
		break;

#if defined(CONFIG_ARCH_HI3XXX) && defined(CONFIG_HI3XXX_CMDLINE_PARSE)
	case CPUFREQ_GOV_POLICY_EXIT:
		set_lowBatteryflag(0);

#ifdef CONFIG_ARCH_HI3630
		if (policy->cpu == 4)
			big_is_booting = 0;
#endif

		break;
#endif

	default:
		break;
	}
	return 0;
=======
#include <linux/cpufreq.h>
#include <linux/init.h>
#include <linux/module.h>

static void cpufreq_gov_performance_limits(struct cpufreq_policy *policy)
{
	pr_debug("setting to %u kHz\n", policy->max);
	__cpufreq_driver_target(policy, policy->max, CPUFREQ_RELATION_H);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifdef CONFIG_CPU_FREQ_GOV_PERFORMANCE_MODULE
static
#endif
struct cpufreq_governor cpufreq_gov_performance = {
	.name		= "performance",
<<<<<<< HEAD
	.governor	= cpufreq_governor_performance,
	.owner		= THIS_MODULE,
};


static int __init cpufreq_gov_performance_init(void)
{
#if defined(CONFIG_ARCH_HI3630) && defined(CONFIG_HI3XXX_CMDLINE_PARSE)
	big_is_booting = 1;
#endif

	return cpufreq_register_governor(&cpufreq_gov_performance);
}


=======
	.owner		= THIS_MODULE,
	.limits		= cpufreq_gov_performance_limits,
};

static int __init cpufreq_gov_performance_init(void)
{
	return cpufreq_register_governor(&cpufreq_gov_performance);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void __exit cpufreq_gov_performance_exit(void)
{
	cpufreq_unregister_governor(&cpufreq_gov_performance);
}

<<<<<<< HEAD
=======
#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE
struct cpufreq_governor *cpufreq_default_governor(void)
{
	return &cpufreq_gov_performance;
}
#endif
#ifndef CONFIG_CPU_FREQ_GOV_PERFORMANCE_MODULE
struct cpufreq_governor *cpufreq_fallback_governor(void)
{
	return &cpufreq_gov_performance;
}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

MODULE_AUTHOR("Dominik Brodowski <linux@brodo.de>");
MODULE_DESCRIPTION("CPUfreq policy governor 'performance'");
MODULE_LICENSE("GPL");

fs_initcall(cpufreq_gov_performance_init);
module_exit(cpufreq_gov_performance_exit);
