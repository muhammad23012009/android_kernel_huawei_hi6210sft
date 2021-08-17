/*
<<<<<<< HEAD
 * Copyright (c) 2011-2012 Samsung Electronics Co., Ltd.
=======
 * Copyright (c) 2011-2014 Samsung Electronics Co., Ltd.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *		http://www.samsung.com
 *
 * EXYNOS - Power Management support
 *
 * Based on arch/arm/mach-s3c2410/pm.c
 * Copyright (c) 2006 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/init.h>
#include <linux/suspend.h>
<<<<<<< HEAD
#include <linux/syscore_ops.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/clk.h>

#include <asm/cacheflush.h>
#include <asm/hardware/cache-l2x0.h>
#include <asm/smp_scu.h>

#include <plat/cpu.h>
#include <plat/pm.h>
#include <plat/pll.h>
#include <plat/regs-srom.h>

#include <mach/regs-irq.h>
#include <mach/regs-gpio.h>
#include <mach/regs-clock.h>
#include <mach/regs-pmu.h>
#include <mach/pm-core.h>

#include "common.h"

static struct sleep_save exynos4_set_clksrc[] = {
	{ .reg = EXYNOS4_CLKSRC_MASK_TOP		, .val = 0x00000001, },
	{ .reg = EXYNOS4_CLKSRC_MASK_CAM		, .val = 0x11111111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_TV			, .val = 0x00000111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_LCD0		, .val = 0x00001111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_MAUDIO		, .val = 0x00000001, },
	{ .reg = EXYNOS4_CLKSRC_MASK_FSYS		, .val = 0x01011111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_PERIL0		, .val = 0x01111111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_PERIL1		, .val = 0x01110111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_DMC		, .val = 0x00010000, },
};

static struct sleep_save exynos4210_set_clksrc[] = {
	{ .reg = EXYNOS4210_CLKSRC_MASK_LCD1		, .val = 0x00001111, },
};

static struct sleep_save exynos4_epll_save[] = {
	SAVE_ITEM(EXYNOS4_EPLL_CON0),
	SAVE_ITEM(EXYNOS4_EPLL_CON1),
};

static struct sleep_save exynos4_vpll_save[] = {
	SAVE_ITEM(EXYNOS4_VPLL_CON0),
	SAVE_ITEM(EXYNOS4_VPLL_CON1),
};

static struct sleep_save exynos5_sys_save[] = {
	SAVE_ITEM(EXYNOS5_SYS_I2C_CFG),
};

static struct sleep_save exynos_core_save[] = {
	/* SROM side */
	SAVE_ITEM(S5P_SROM_BW),
	SAVE_ITEM(S5P_SROM_BC0),
	SAVE_ITEM(S5P_SROM_BC1),
	SAVE_ITEM(S5P_SROM_BC2),
	SAVE_ITEM(S5P_SROM_BC3),
};

=======
#include <linux/cpu_pm.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/soc/samsung/exynos-regs-pmu.h>
#include <linux/soc/samsung/exynos-pmu.h>

#include <asm/firmware.h>
#include <asm/smp_scu.h>
#include <asm/suspend.h>
#include <asm/cacheflush.h>

#include <mach/map.h>

#include "common.h"

static inline void __iomem *exynos_boot_vector_addr(void)
{
	if (samsung_rev() == EXYNOS4210_REV_1_1)
		return pmu_base_addr + S5P_INFORM7;
	else if (samsung_rev() == EXYNOS4210_REV_1_0)
		return sysram_base_addr + 0x24;
	return pmu_base_addr + S5P_INFORM0;
}

static inline void __iomem *exynos_boot_vector_flag(void)
{
	if (samsung_rev() == EXYNOS4210_REV_1_1)
		return pmu_base_addr + S5P_INFORM6;
	else if (samsung_rev() == EXYNOS4210_REV_1_0)
		return sysram_base_addr + 0x20;
	return pmu_base_addr + S5P_INFORM1;
}

#define S5P_CHECK_AFTR  0xFCBA0D10
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* For Cortex-A9 Diagnostic and Power control register */
static unsigned int save_arm_register[2];

<<<<<<< HEAD
static int exynos_cpu_suspend(unsigned long arg)
{
#ifdef CONFIG_CACHE_L2X0
	outer_flush_all();
#endif

	if (soc_is_exynos5250())
		flush_cache_all();

	/* issue the standby signal into the pm unit. */
	cpu_do_idle();

	pr_info("Failed to suspend the system\n");
	return 1; /* Aborting suspend */
}

static void exynos_pm_prepare(void)
{
	unsigned int tmp;

	s3c_pm_do_save(exynos_core_save, ARRAY_SIZE(exynos_core_save));

	if (!soc_is_exynos5250()) {
		s3c_pm_do_save(exynos4_epll_save, ARRAY_SIZE(exynos4_epll_save));
		s3c_pm_do_save(exynos4_vpll_save, ARRAY_SIZE(exynos4_vpll_save));
	} else {
		s3c_pm_do_save(exynos5_sys_save, ARRAY_SIZE(exynos5_sys_save));
		/* Disable USE_RETENTION of JPEG_MEM_OPTION */
		tmp = __raw_readl(EXYNOS5_JPEG_MEM_OPTION);
		tmp &= ~EXYNOS5_OPTION_USE_RETENTION;
		__raw_writel(tmp, EXYNOS5_JPEG_MEM_OPTION);
	}

	/* Set value of power down register for sleep mode */

	exynos_sys_powerdown_conf(SYS_SLEEP);
	__raw_writel(S5P_CHECK_SLEEP, S5P_INFORM1);

	/* ensure at least INFORM0 has the resume address */

	__raw_writel(virt_to_phys(s3c_cpu_resume), S5P_INFORM0);

	/* Before enter central sequence mode, clock src register have to set */

	if (!soc_is_exynos5250())
		s3c_pm_do_restore_core(exynos4_set_clksrc, ARRAY_SIZE(exynos4_set_clksrc));

	if (soc_is_exynos4210())
		s3c_pm_do_restore_core(exynos4210_set_clksrc, ARRAY_SIZE(exynos4210_set_clksrc));

}

static int exynos_pm_add(struct device *dev, struct subsys_interface *sif)
{
	pm_cpu_prep = exynos_pm_prepare;
	pm_cpu_sleep = exynos_cpu_suspend;
=======
void exynos_cpu_save_register(void)
{
	unsigned long tmp;

	/* Save Power control register */
	asm ("mrc p15, 0, %0, c15, c0, 0"
	     : "=r" (tmp) : : "cc");

	save_arm_register[0] = tmp;

	/* Save Diagnostic register */
	asm ("mrc p15, 0, %0, c15, c0, 1"
	     : "=r" (tmp) : : "cc");

	save_arm_register[1] = tmp;
}

void exynos_cpu_restore_register(void)
{
	unsigned long tmp;

	/* Restore Power control register */
	tmp = save_arm_register[0];

	asm volatile ("mcr p15, 0, %0, c15, c0, 0"
		      : : "r" (tmp)
		      : "cc");

	/* Restore Diagnostic register */
	tmp = save_arm_register[1];

	asm volatile ("mcr p15, 0, %0, c15, c0, 1"
		      : : "r" (tmp)
		      : "cc");
}

void exynos_pm_central_suspend(void)
{
	unsigned long tmp;

	/* Setting Central Sequence Register for power down mode */
	tmp = pmu_raw_readl(S5P_CENTRAL_SEQ_CONFIGURATION);
	tmp &= ~S5P_CENTRAL_LOWPWR_CFG;
	pmu_raw_writel(tmp, S5P_CENTRAL_SEQ_CONFIGURATION);
}

int exynos_pm_central_resume(void)
{
	unsigned long tmp;

	/*
	 * If PMU failed while entering sleep mode, WFI will be
	 * ignored by PMU and then exiting cpu_do_idle().
	 * S5P_CENTRAL_LOWPWR_CFG bit will not be set automatically
	 * in this situation.
	 */
	tmp = pmu_raw_readl(S5P_CENTRAL_SEQ_CONFIGURATION);
	if (!(tmp & S5P_CENTRAL_LOWPWR_CFG)) {
		tmp |= S5P_CENTRAL_LOWPWR_CFG;
		pmu_raw_writel(tmp, S5P_CENTRAL_SEQ_CONFIGURATION);
		/* clear the wakeup state register */
		pmu_raw_writel(0x0, S5P_WAKEUP_STAT);
		/* No need to perform below restore code */
		return -1;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static unsigned long pll_base_rate;

static void exynos4_restore_pll(void)
{
	unsigned long pll_con, locktime, lockcnt;
	unsigned long pll_in_rate;
	unsigned int p_div, epll_wait = 0, vpll_wait = 0;

	if (pll_base_rate == 0)
		return;

	pll_in_rate = pll_base_rate;

	/* EPLL */
	pll_con = exynos4_epll_save[0].val;

	if (pll_con & (1 << 31)) {
		pll_con &= (PLL46XX_PDIV_MASK << PLL46XX_PDIV_SHIFT);
		p_div = (pll_con >> PLL46XX_PDIV_SHIFT);

		pll_in_rate /= 1000000;

		locktime = (3000 / pll_in_rate) * p_div;
		lockcnt = locktime * 10000 / (10000 / pll_in_rate);

		__raw_writel(lockcnt, EXYNOS4_EPLL_LOCK);

		s3c_pm_do_restore_core(exynos4_epll_save,
					ARRAY_SIZE(exynos4_epll_save));
		epll_wait = 1;
	}

	pll_in_rate = pll_base_rate;

	/* VPLL */
	pll_con = exynos4_vpll_save[0].val;

	if (pll_con & (1 << 31)) {
		pll_in_rate /= 1000000;
		/* 750us */
		locktime = 750;
		lockcnt = locktime * 10000 / (10000 / pll_in_rate);

		__raw_writel(lockcnt, EXYNOS4_VPLL_LOCK);

		s3c_pm_do_restore_core(exynos4_vpll_save,
					ARRAY_SIZE(exynos4_vpll_save));
		vpll_wait = 1;
	}

	/* Wait PLL locking */

	do {
		if (epll_wait) {
			pll_con = __raw_readl(EXYNOS4_EPLL_CON0);
			if (pll_con & (1 << EXYNOS4_EPLLCON0_LOCKED_SHIFT))
				epll_wait = 0;
		}

		if (vpll_wait) {
			pll_con = __raw_readl(EXYNOS4_VPLL_CON0);
			if (pll_con & (1 << EXYNOS4_VPLLCON0_LOCKED_SHIFT))
				vpll_wait = 0;
		}
	} while (epll_wait || vpll_wait);
}

static struct subsys_interface exynos_pm_interface = {
	.name		= "exynos_pm",
	.subsys		= &exynos_subsys,
	.add_dev	= exynos_pm_add,
};

static __init int exynos_pm_drvinit(void)
{
	struct clk *pll_base;
	unsigned int tmp;

	s3c_pm_init();

	/* All wakeup disable */

	tmp = __raw_readl(S5P_WAKEUP_MASK);
	tmp |= ((0xFF << 8) | (0x1F << 1));
	__raw_writel(tmp, S5P_WAKEUP_MASK);

	if (!soc_is_exynos5250()) {
		pll_base = clk_get(NULL, "xtal");

		if (!IS_ERR(pll_base)) {
			pll_base_rate = clk_get_rate(pll_base);
			clk_put(pll_base);
		}
	}

	return subsys_interface_register(&exynos_pm_interface);
}
arch_initcall(exynos_pm_drvinit);

static int exynos_pm_suspend(void)
{
	unsigned long tmp;

	/* Setting Central Sequence Register for power down mode */

	tmp = __raw_readl(S5P_CENTRAL_SEQ_CONFIGURATION);
	tmp &= ~S5P_CENTRAL_LOWPWR_CFG;
	__raw_writel(tmp, S5P_CENTRAL_SEQ_CONFIGURATION);

	/* Setting SEQ_OPTION register */

	tmp = (S5P_USE_STANDBY_WFI0 | S5P_USE_STANDBY_WFE0);
	__raw_writel(tmp, S5P_CENTRAL_SEQ_OPTION);

	if (!soc_is_exynos5250()) {
		/* Save Power control register */
		asm ("mrc p15, 0, %0, c15, c0, 0"
		     : "=r" (tmp) : : "cc");
		save_arm_register[0] = tmp;

		/* Save Diagnostic register */
		asm ("mrc p15, 0, %0, c15, c0, 1"
		     : "=r" (tmp) : : "cc");
		save_arm_register[1] = tmp;
	}

	return 0;
}

static void exynos_pm_resume(void)
{
	unsigned long tmp;

	/*
	 * If PMU failed while entering sleep mode, WFI will be
	 * ignored by PMU and then exiting cpu_do_idle().
	 * S5P_CENTRAL_LOWPWR_CFG bit will not be set automatically
	 * in this situation.
	 */
	tmp = __raw_readl(S5P_CENTRAL_SEQ_CONFIGURATION);
	if (!(tmp & S5P_CENTRAL_LOWPWR_CFG)) {
		tmp |= S5P_CENTRAL_LOWPWR_CFG;
		__raw_writel(tmp, S5P_CENTRAL_SEQ_CONFIGURATION);
		/* clear the wakeup state register */
		__raw_writel(0x0, S5P_WAKEUP_STAT);
		/* No need to perform below restore code */
		goto early_wakeup;
	}
	if (!soc_is_exynos5250()) {
		/* Restore Power control register */
		tmp = save_arm_register[0];
		asm volatile ("mcr p15, 0, %0, c15, c0, 0"
			      : : "r" (tmp)
			      : "cc");

		/* Restore Diagnostic register */
		tmp = save_arm_register[1];
		asm volatile ("mcr p15, 0, %0, c15, c0, 1"
			      : : "r" (tmp)
			      : "cc");
	}

	/* For release retention */

	__raw_writel((1 << 28), S5P_PAD_RET_MAUDIO_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_GPIO_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_UART_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_MMCA_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_MMCB_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_EBIA_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_EBIB_OPTION);

	if (soc_is_exynos5250())
		s3c_pm_do_restore(exynos5_sys_save,
			ARRAY_SIZE(exynos5_sys_save));

	s3c_pm_do_restore_core(exynos_core_save, ARRAY_SIZE(exynos_core_save));

	if (!soc_is_exynos5250()) {
		exynos4_restore_pll();

#ifdef CONFIG_SMP
		scu_enable(S5P_VA_SCU);
#endif
	}

early_wakeup:

	/* Clear SLEEP mode set in INFORM1 */
	__raw_writel(0x0, S5P_INFORM1);

	return;
}

static struct syscore_ops exynos_pm_syscore_ops = {
	.suspend	= exynos_pm_suspend,
	.resume		= exynos_pm_resume,
};

static __init int exynos_pm_syscore_init(void)
{
	register_syscore_ops(&exynos_pm_syscore_ops);
	return 0;
}
arch_initcall(exynos_pm_syscore_init);
=======
/* Ext-GIC nIRQ/nFIQ is the only wakeup source in AFTR */
static void exynos_set_wakeupmask(long mask)
{
	pmu_raw_writel(mask, S5P_WAKEUP_MASK);
	if (soc_is_exynos3250())
		pmu_raw_writel(0x0, S5P_WAKEUP_MASK2);
}

static void exynos_cpu_set_boot_vector(long flags)
{
	writel_relaxed(virt_to_phys(exynos_cpu_resume),
		       exynos_boot_vector_addr());
	writel_relaxed(flags, exynos_boot_vector_flag());
}

static int exynos_aftr_finisher(unsigned long flags)
{
	int ret;

	exynos_set_wakeupmask(soc_is_exynos3250() ? 0x40003ffe : 0x0000ff3e);
	/* Set value of power down register for aftr mode */
	exynos_sys_powerdown_conf(SYS_AFTR);

	ret = call_firmware_op(do_idle, FW_DO_IDLE_AFTR);
	if (ret == -ENOSYS) {
		if (read_cpuid_part() == ARM_CPU_PART_CORTEX_A9)
			exynos_cpu_save_register();
		exynos_cpu_set_boot_vector(S5P_CHECK_AFTR);
		cpu_do_idle();
	}

	return 1;
}

void exynos_enter_aftr(void)
{
	unsigned int cpuid = smp_processor_id();

	cpu_pm_enter();

	if (soc_is_exynos3250())
		exynos_set_boot_flag(cpuid, C2_STATE);

	exynos_pm_central_suspend();

	if (of_machine_is_compatible("samsung,exynos4212") ||
	    of_machine_is_compatible("samsung,exynos4412")) {
		/* Setting SEQ_OPTION register */
		pmu_raw_writel(S5P_USE_STANDBY_WFI0 | S5P_USE_STANDBY_WFE0,
			       S5P_CENTRAL_SEQ_OPTION);
	}

	cpu_suspend(0, exynos_aftr_finisher);

	if (read_cpuid_part() == ARM_CPU_PART_CORTEX_A9) {
		scu_enable(S5P_VA_SCU);
		if (call_firmware_op(resume) == -ENOSYS)
			exynos_cpu_restore_register();
	}

	exynos_pm_central_resume();

	if (soc_is_exynos3250())
		exynos_clear_boot_flag(cpuid, C2_STATE);

	cpu_pm_exit();
}

#if defined(CONFIG_SMP) && defined(CONFIG_ARM_EXYNOS_CPUIDLE)
static atomic_t cpu1_wakeup = ATOMIC_INIT(0);

static int exynos_cpu0_enter_aftr(void)
{
	int ret = -1;

	/*
	 * If the other cpu is powered on, we have to power it off, because
	 * the AFTR state won't work otherwise
	 */
	if (cpu_online(1)) {
		/*
		 * We reach a sync point with the coupled idle state, we know
		 * the other cpu will power down itself or will abort the
		 * sequence, let's wait for one of these to happen
		 */
		while (exynos_cpu_power_state(1)) {
			unsigned long boot_addr;

			/*
			 * The other cpu may skip idle and boot back
			 * up again
			 */
			if (atomic_read(&cpu1_wakeup))
				goto abort;

			/*
			 * The other cpu may bounce through idle and
			 * boot back up again, getting stuck in the
			 * boot rom code
			 */
			ret = exynos_get_boot_addr(1, &boot_addr);
			if (ret)
				goto fail;
			ret = -1;
			if (boot_addr == 0)
				goto abort;

			cpu_relax();
		}
	}

	exynos_enter_aftr();
	ret = 0;

abort:
	if (cpu_online(1)) {
		unsigned long boot_addr = virt_to_phys(exynos_cpu_resume);

		/*
		 * Set the boot vector to something non-zero
		 */
		ret = exynos_set_boot_addr(1, boot_addr);
		if (ret)
			goto fail;
		dsb();

		/*
		 * Turn on cpu1 and wait for it to be on
		 */
		exynos_cpu_power_up(1);
		while (exynos_cpu_power_state(1) != S5P_CORE_LOCAL_PWR_EN)
			cpu_relax();

		if (soc_is_exynos3250()) {
			while (!pmu_raw_readl(S5P_PMU_SPARE2) &&
			       !atomic_read(&cpu1_wakeup))
				cpu_relax();

			if (!atomic_read(&cpu1_wakeup))
				exynos_core_restart(1);
		}

		while (!atomic_read(&cpu1_wakeup)) {
			smp_rmb();

			/*
			 * Poke cpu1 out of the boot rom
			 */

			ret = exynos_set_boot_addr(1, boot_addr);
			if (ret)
				goto fail;

			call_firmware_op(cpu_boot, 1);

			if (soc_is_exynos3250())
				dsb_sev();
			else
				arch_send_wakeup_ipi_mask(cpumask_of(1));
		}
	}
fail:
	return ret;
}

static int exynos_wfi_finisher(unsigned long flags)
{
	if (soc_is_exynos3250())
		flush_cache_all();
	cpu_do_idle();

	return -1;
}

static int exynos_cpu1_powerdown(void)
{
	int ret = -1;

	/*
	 * Idle sequence for cpu1
	 */
	if (cpu_pm_enter())
		goto cpu1_aborted;

	/*
	 * Turn off cpu 1
	 */
	exynos_cpu_power_down(1);

	if (soc_is_exynos3250())
		pmu_raw_writel(0, S5P_PMU_SPARE2);

	ret = cpu_suspend(0, exynos_wfi_finisher);

	cpu_pm_exit();

cpu1_aborted:
	dsb();
	/*
	 * Notify cpu 0 that cpu 1 is awake
	 */
	atomic_set(&cpu1_wakeup, 1);

	return ret;
}

static void exynos_pre_enter_aftr(void)
{
	unsigned long boot_addr = virt_to_phys(exynos_cpu_resume);

	(void)exynos_set_boot_addr(1, boot_addr);
}

static void exynos_post_enter_aftr(void)
{
	atomic_set(&cpu1_wakeup, 0);
}

struct cpuidle_exynos_data cpuidle_coupled_exynos_data = {
	.cpu0_enter_aftr		= exynos_cpu0_enter_aftr,
	.cpu1_powerdown		= exynos_cpu1_powerdown,
	.pre_enter_aftr		= exynos_pre_enter_aftr,
	.post_enter_aftr		= exynos_post_enter_aftr,
};
#endif /* CONFIG_SMP && CONFIG_ARM_EXYNOS_CPUIDLE */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
