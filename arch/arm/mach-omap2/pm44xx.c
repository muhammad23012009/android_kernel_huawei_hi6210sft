/*
<<<<<<< HEAD
 * OMAP4 Power Management Routines
 *
 * Copyright (C) 2010-2011 Texas Instruments, Inc.
=======
 * OMAP4+ Power Management Routines
 *
 * Copyright (C) 2010-2013 Texas Instruments, Inc.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Rajendra Nayak <rnayak@ti.com>
 * Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/pm.h>
#include <linux/suspend.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <asm/system_misc.h>

#include "soc.h"
#include "common.h"
#include "clockdomain.h"
#include "powerdomain.h"
#include "pm.h"

<<<<<<< HEAD
struct power_state {
	struct powerdomain *pwrdm;
	u32 next_state;
=======
u16 pm44xx_errata;

struct power_state {
	struct powerdomain *pwrdm;
	u32 next_state;
	u32 next_logic_state;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_SUSPEND
	u32 saved_state;
	u32 saved_logic_state;
#endif
	struct list_head node;
};

<<<<<<< HEAD
=======
/**
 * struct static_dep_map - Static dependency map
 * @from:	from clockdomain
 * @to:		to clockdomain
  */
struct static_dep_map {
	const char *from;
	const char *to;
};

static u32 cpu_suspend_state = PWRDM_POWER_OFF;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static LIST_HEAD(pwrst_list);

#ifdef CONFIG_SUSPEND
static int omap4_pm_suspend(void)
{
	struct power_state *pwrst;
	int state, ret = 0;
	u32 cpu_id = smp_processor_id();

	/* Save current powerdomain state */
	list_for_each_entry(pwrst, &pwrst_list, node) {
		pwrst->saved_state = pwrdm_read_next_pwrst(pwrst->pwrdm);
		pwrst->saved_logic_state = pwrdm_read_logic_retst(pwrst->pwrdm);
	}

	/* Set targeted power domain states by suspend */
	list_for_each_entry(pwrst, &pwrst_list, node) {
		omap_set_pwrdm_state(pwrst->pwrdm, pwrst->next_state);
<<<<<<< HEAD
		pwrdm_set_logic_retst(pwrst->pwrdm, PWRDM_POWER_OFF);
=======
		pwrdm_set_logic_retst(pwrst->pwrdm, pwrst->next_logic_state);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/*
	 * For MPUSS to hit power domain retention(CSWR or OSWR),
	 * CPU0 and CPU1 power domains need to be in OFF or DORMANT state,
	 * since CPU power domain CSWR is not supported by hardware
	 * Only master CPU follows suspend path. All other CPUs follow
	 * CPU hotplug path in system wide suspend. On OMAP4, CPU power
	 * domain CSWR is not supported by hardware.
	 * More details can be found in OMAP4430 TRM section 4.3.4.2.
	 */
<<<<<<< HEAD
	omap4_enter_lowpower(cpu_id, PWRDM_POWER_OFF);
=======
	omap4_enter_lowpower(cpu_id, cpu_suspend_state);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Restore next powerdomain state */
	list_for_each_entry(pwrst, &pwrst_list, node) {
		state = pwrdm_read_prev_pwrst(pwrst->pwrdm);
		if (state > pwrst->next_state) {
			pr_info("Powerdomain (%s) didn't enter target state %d\n",
				pwrst->pwrdm->name, pwrst->next_state);
			ret = -1;
		}
		omap_set_pwrdm_state(pwrst->pwrdm, pwrst->saved_state);
		pwrdm_set_logic_retst(pwrst->pwrdm, pwrst->saved_logic_state);
	}
	if (ret) {
		pr_crit("Could not enter target state in pm_suspend\n");
		/*
		 * OMAP4 chip PM currently works only with certain (newer)
		 * versions of bootloaders. This is due to missing code in the
		 * kernel to properly reset and initialize some devices.
		 * Warn the user about the bootloader version being one of the
		 * possible causes.
		 * http://www.spinics.net/lists/arm-kernel/msg218641.html
		 */
		pr_warn("A possible cause could be an old bootloader - try u-boot >= v2012.07\n");
	} else {
		pr_info("Successfully put all powerdomains to target state\n");
	}

	return 0;
}
<<<<<<< HEAD
=======
#else
#define omap4_pm_suspend NULL
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* CONFIG_SUSPEND */

static int __init pwrdms_setup(struct powerdomain *pwrdm, void *unused)
{
	struct power_state *pwrst;

	if (!pwrdm->pwrsts)
		return 0;

	/*
	 * Skip CPU0 and CPU1 power domains. CPU1 is programmed
	 * through hotplug path and CPU0 explicitly programmed
	 * further down in the code path
	 */
<<<<<<< HEAD
	if (!strncmp(pwrdm->name, "cpu", 3))
		return 0;
=======
	if (!strncmp(pwrdm->name, "cpu", 3)) {
		if (IS_PM44XX_ERRATUM(PM_OMAP4_CPU_OSWR_DISABLE))
			cpu_suspend_state = PWRDM_POWER_RET;
		return 0;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	pwrst = kmalloc(sizeof(struct power_state), GFP_ATOMIC);
	if (!pwrst)
		return -ENOMEM;

	pwrst->pwrdm = pwrdm;
<<<<<<< HEAD
	pwrst->next_state = PWRDM_POWER_RET;
=======
	pwrst->next_state = pwrdm_get_valid_lp_state(pwrdm, false,
						     PWRDM_POWER_RET);
	pwrst->next_logic_state = pwrdm_get_valid_lp_state(pwrdm, true,
							   PWRDM_POWER_OFF);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	list_add(&pwrst->node, &pwrst_list);

	return omap_set_pwrdm_state(pwrst->pwrdm, pwrst->next_state);
}

/**
 * omap_default_idle - OMAP4 default ilde routine.'
 *
 * Implements OMAP4 memory, IO ordering requirements which can't be addressed
 * with default cpu_do_idle() hook. Used by all CPUs with !CONFIG_CPU_IDLE and
 * by secondary CPU with CONFIG_CPU_IDLE.
 */
static void omap_default_idle(void)
{
	omap_do_wfi();
}

<<<<<<< HEAD
/**
 * omap4_pm_init - Init routine for OMAP4 PM
 *
 * Initializes all powerdomain and clockdomain target states
 * and all PRCM settings.
 */
int __init omap4_pm_init(void)
{
	int ret;
	struct clockdomain *emif_clkdm, *mpuss_clkdm, *l3_1_clkdm;
	struct clockdomain *ducati_clkdm, *l3_2_clkdm;
=======
/*
 * The dynamic dependency between MPUSS -> MEMIF and
 * MPUSS -> L4_PER/L3_* and DUCATI -> L3_* doesn't work as
 * expected. The hardware recommendation is to enable static
 * dependencies for these to avoid system lock ups or random crashes.
 * The L4 wakeup depedency is added to workaround the OCP sync hardware
 * BUG with 32K synctimer which lead to incorrect timer value read
 * from the 32K counter. The BUG applies for GPTIMER1 and WDT2 which
 * are part of L4 wakeup clockdomain.
 */
static const struct static_dep_map omap4_static_dep_map[] = {
	{.from = "mpuss_clkdm", .to = "l3_emif_clkdm"},
	{.from = "mpuss_clkdm", .to = "l3_1_clkdm"},
	{.from = "mpuss_clkdm", .to = "l3_2_clkdm"},
	{.from = "ducati_clkdm", .to = "l3_1_clkdm"},
	{.from = "ducati_clkdm", .to = "l3_2_clkdm"},
	{.from  = NULL} /* TERMINATION */
};

static const struct static_dep_map omap5_dra7_static_dep_map[] = {
	{.from = "mpu_clkdm", .to = "emif_clkdm"},
	{.from  = NULL} /* TERMINATION */
};

/**
 * omap4plus_init_static_deps() - Initialize a static dependency map
 * @map:	Mapping of clock domains
 */
static inline int omap4plus_init_static_deps(const struct static_dep_map *map)
{
	int ret;
	struct clockdomain *from, *to;

	if (!map)
		return 0;

	while (map->from) {
		from = clkdm_lookup(map->from);
		to = clkdm_lookup(map->to);
		if (!from || !to) {
			pr_err("Failed lookup %s or %s for wakeup dependency\n",
			       map->from, map->to);
			return -EINVAL;
		}
		ret = clkdm_add_wkdep(from, to);
		if (ret) {
			pr_err("Failed to add %s -> %s wakeup dependency(%d)\n",
			       map->from, map->to, ret);
			return ret;
		}

		map++;
	}

	return 0;
}

/**
 * omap4_pm_init_early - Does early initialization necessary for OMAP4+ devices
 *
 * Initializes basic stuff for power management functionality.
 */
int __init omap4_pm_init_early(void)
{
	if (cpu_is_omap446x())
		pm44xx_errata |= PM_OMAP4_ROM_SMP_BOOT_ERRATUM_GICD;

	if (soc_is_omap54xx() || soc_is_dra7xx())
		pm44xx_errata |= PM_OMAP4_CPU_OSWR_DISABLE;

	return 0;
}

/**
 * omap4_pm_init - Init routine for OMAP4+ devices
 *
 * Initializes all powerdomain and clockdomain target states
 * and all PRCM settings.
 * Return: Returns the error code returned by called functions.
 */
int __init omap4_pm_init(void)
{
	int ret = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (omap_rev() == OMAP4430_REV_ES1_0) {
		WARN(1, "Power Management not supported on OMAP4430 ES1.0\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	pr_err("Power Management for TI OMAP4.\n");
=======
	pr_info("Power Management for TI OMAP4+ devices.\n");

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * OMAP4 chip PM currently works only with certain (newer)
	 * versions of bootloaders. This is due to missing code in the
	 * kernel to properly reset and initialize some devices.
	 * http://www.spinics.net/lists/arm-kernel/msg218641.html
	 */
<<<<<<< HEAD
	pr_warn("OMAP4 PM: u-boot >= v2012.07 is required for full PM support\n");

	ret = pwrdm_for_each(pwrdms_setup, NULL);
	if (ret) {
		pr_err("Failed to setup powerdomains\n");
		goto err2;
	}

	/*
	 * The dynamic dependency between MPUSS -> MEMIF and
	 * MPUSS -> L4_PER/L3_* and DUCATI -> L3_* doesn't work as
	 * expected. The hardware recommendation is to enable static
	 * dependencies for these to avoid system lock ups or random crashes.
	 */
	mpuss_clkdm = clkdm_lookup("mpuss_clkdm");
	emif_clkdm = clkdm_lookup("l3_emif_clkdm");
	l3_1_clkdm = clkdm_lookup("l3_1_clkdm");
	l3_2_clkdm = clkdm_lookup("l3_2_clkdm");
	ducati_clkdm = clkdm_lookup("ducati_clkdm");
	if ((!mpuss_clkdm) || (!emif_clkdm) || (!l3_1_clkdm) ||
		(!l3_2_clkdm) || (!ducati_clkdm))
		goto err2;

	ret = clkdm_add_wkdep(mpuss_clkdm, emif_clkdm);
	ret |= clkdm_add_wkdep(mpuss_clkdm, l3_1_clkdm);
	ret |= clkdm_add_wkdep(mpuss_clkdm, l3_2_clkdm);
	ret |= clkdm_add_wkdep(ducati_clkdm, l3_1_clkdm);
	ret |= clkdm_add_wkdep(ducati_clkdm, l3_2_clkdm);
	if (ret) {
		pr_err("Failed to add MPUSS -> L3/EMIF/L4PER, DUCATI -> L3 wakeup dependency\n");
=======
	if (cpu_is_omap44xx())
		pr_warn("OMAP4 PM: u-boot >= v2012.07 is required for full PM support\n");

	ret = pwrdm_for_each(pwrdms_setup, NULL);
	if (ret) {
		pr_err("Failed to setup powerdomains.\n");
		goto err2;
	}

	if (cpu_is_omap44xx())
		ret = omap4plus_init_static_deps(omap4_static_dep_map);
	else if (soc_is_omap54xx() || soc_is_dra7xx())
		ret = omap4plus_init_static_deps(omap5_dra7_static_dep_map);

	if (ret) {
		pr_err("Failed to initialise static dependencies.\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto err2;
	}

	ret = omap4_mpuss_init();
	if (ret) {
		pr_err("Failed to initialise OMAP4 MPUSS\n");
		goto err2;
	}

	(void) clkdm_for_each(omap_pm_clkdms_setup, NULL);

<<<<<<< HEAD
#ifdef CONFIG_SUSPEND
	omap_pm_suspend = omap4_pm_suspend;
#endif
=======
	omap_common_suspend_init(omap4_pm_suspend);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Overwrite the default cpu_do_idle() */
	arm_pm_idle = omap_default_idle;

<<<<<<< HEAD
	omap4_idle_init();
=======
	if (cpu_is_omap44xx())
		omap4_idle_init();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

err2:
	return ret;
}
