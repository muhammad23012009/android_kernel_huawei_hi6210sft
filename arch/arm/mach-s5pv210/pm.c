/* linux/arch/arm/mach-s5pv210/pm.c
 *
<<<<<<< HEAD
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
=======
 * Copyright (c) 2010-2014 Samsung Electronics Co., Ltd.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *		http://www.samsung.com
 *
 * S5PV210 - Power Management support
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
#include <linux/syscore_ops.h>
#include <linux/io.h>

<<<<<<< HEAD
#include <plat/cpu.h>
#include <plat/pm.h>
#include <plat/regs-timer.h>

#include <mach/regs-irq.h>
#include <mach/regs-clock.h>

static struct sleep_save s5pv210_core_save[] = {
	/* Clock source */
	SAVE_ITEM(S5P_CLK_SRC0),
	SAVE_ITEM(S5P_CLK_SRC1),
	SAVE_ITEM(S5P_CLK_SRC2),
	SAVE_ITEM(S5P_CLK_SRC3),
	SAVE_ITEM(S5P_CLK_SRC4),
	SAVE_ITEM(S5P_CLK_SRC5),
	SAVE_ITEM(S5P_CLK_SRC6),

	/* Clock source Mask */
	SAVE_ITEM(S5P_CLK_SRC_MASK0),
	SAVE_ITEM(S5P_CLK_SRC_MASK1),

	/* Clock Divider */
	SAVE_ITEM(S5P_CLK_DIV0),
	SAVE_ITEM(S5P_CLK_DIV1),
	SAVE_ITEM(S5P_CLK_DIV2),
	SAVE_ITEM(S5P_CLK_DIV3),
	SAVE_ITEM(S5P_CLK_DIV4),
	SAVE_ITEM(S5P_CLK_DIV5),
	SAVE_ITEM(S5P_CLK_DIV6),
	SAVE_ITEM(S5P_CLK_DIV7),

	/* Clock Main Gate */
	SAVE_ITEM(S5P_CLKGATE_MAIN0),
	SAVE_ITEM(S5P_CLKGATE_MAIN1),
	SAVE_ITEM(S5P_CLKGATE_MAIN2),

	/* Clock source Peri Gate */
	SAVE_ITEM(S5P_CLKGATE_PERI0),
	SAVE_ITEM(S5P_CLKGATE_PERI1),

	/* Clock source SCLK Gate */
	SAVE_ITEM(S5P_CLKGATE_SCLK0),
	SAVE_ITEM(S5P_CLKGATE_SCLK1),

	/* Clock IP Clock gate */
	SAVE_ITEM(S5P_CLKGATE_IP0),
	SAVE_ITEM(S5P_CLKGATE_IP1),
	SAVE_ITEM(S5P_CLKGATE_IP2),
	SAVE_ITEM(S5P_CLKGATE_IP3),
	SAVE_ITEM(S5P_CLKGATE_IP4),

	/* Clock Blcok and Bus gate */
	SAVE_ITEM(S5P_CLKGATE_BLOCK),
	SAVE_ITEM(S5P_CLKGATE_BUS0),

	/* Clock ETC */
	SAVE_ITEM(S5P_CLK_OUT),
	SAVE_ITEM(S5P_MDNIE_SEL),

	/* PWM Register */
	SAVE_ITEM(S3C2410_TCFG0),
	SAVE_ITEM(S3C2410_TCFG1),
	SAVE_ITEM(S3C64XX_TINT_CSTAT),
	SAVE_ITEM(S3C2410_TCON),
	SAVE_ITEM(S3C2410_TCNTB(0)),
	SAVE_ITEM(S3C2410_TCMPB(0)),
	SAVE_ITEM(S3C2410_TCNTO(0)),
};

=======
#include <asm/cacheflush.h>
#include <asm/suspend.h>

#include <plat/pm-common.h>

#include "common.h"
#include "regs-clock.h"

static struct sleep_save s5pv210_core_save[] = {
	/* Clock ETC */
	SAVE_ITEM(S5P_MDNIE_SEL),
};

/*
 * VIC wake-up support (TODO)
 */
static u32 s5pv210_irqwake_intmask = 0xffffffff;

/*
 * Suspend helpers.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int s5pv210_cpu_suspend(unsigned long arg)
{
	unsigned long tmp;

	/* issue the standby signal into the pm unit. Note, we
	 * issue a write-buffer drain just in case */

	tmp = 0;

	asm("b 1f\n\t"
	    ".align 5\n\t"
	    "1:\n\t"
	    "mcr p15, 0, %0, c7, c10, 5\n\t"
	    "mcr p15, 0, %0, c7, c10, 4\n\t"
	    "wfi" : : "r" (tmp));

	pr_info("Failed to suspend the system\n");
	return 1; /* Aborting suspend */
}

static void s5pv210_pm_prepare(void)
{
	unsigned int tmp;

<<<<<<< HEAD
	/* ensure at least INFORM0 has the resume address */
	__raw_writel(virt_to_phys(s3c_cpu_resume), S5P_INFORM0);
=======
	/* Set wake-up mask registers */
	__raw_writel(exynos_get_eint_wake_mask(), S5P_EINT_WAKEUP_MASK);
	__raw_writel(s5pv210_irqwake_intmask, S5P_WAKEUP_MASK);

	/* ensure at least INFORM0 has the resume address */
	__raw_writel(virt_to_phys(s5pv210_cpu_resume), S5P_INFORM0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	tmp = __raw_readl(S5P_SLEEP_CFG);
	tmp &= ~(S5P_SLEEP_CFG_OSC_EN | S5P_SLEEP_CFG_USBOSC_EN);
	__raw_writel(tmp, S5P_SLEEP_CFG);

	/* WFI for SLEEP mode configuration by SYSCON */
	tmp = __raw_readl(S5P_PWR_CFG);
	tmp &= S5P_CFG_WFI_CLEAN;
	tmp |= S5P_CFG_WFI_SLEEP;
	__raw_writel(tmp, S5P_PWR_CFG);

	/* SYSCON interrupt handling disable */
	tmp = __raw_readl(S5P_OTHERS);
	tmp |= S5P_OTHER_SYSC_INTOFF;
	__raw_writel(tmp, S5P_OTHERS);

	s3c_pm_do_save(s5pv210_core_save, ARRAY_SIZE(s5pv210_core_save));
}

<<<<<<< HEAD
static int s5pv210_pm_add(struct device *dev, struct subsys_interface *sif)
{
	pm_cpu_prep = s5pv210_pm_prepare;
	pm_cpu_sleep = s5pv210_cpu_suspend;
=======
/*
 * Suspend operations.
 */
static int s5pv210_suspend_enter(suspend_state_t state)
{
	int ret;

	s3c_pm_debug_init();

	S3C_PMDBG("%s: suspending the system...\n", __func__);

	S3C_PMDBG("%s: wakeup masks: %08x,%08x\n", __func__,
			s5pv210_irqwake_intmask, exynos_get_eint_wake_mask());

	if (s5pv210_irqwake_intmask == -1U
	    && exynos_get_eint_wake_mask() == -1U) {
		pr_err("%s: No wake-up sources!\n", __func__);
		pr_err("%s: Aborting sleep\n", __func__);
		return -EINVAL;
	}

	s3c_pm_save_uarts();
	s5pv210_pm_prepare();
	flush_cache_all();
	s3c_pm_check_store();

	ret = cpu_suspend(0, s5pv210_cpu_suspend);
	if (ret)
		return ret;

	s3c_pm_restore_uarts();

	S3C_PMDBG("%s: wakeup stat: %08x\n", __func__,
			__raw_readl(S5P_WAKEUP_STAT));

	s3c_pm_check_restore();

	S3C_PMDBG("%s: resuming the system...\n", __func__);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static struct subsys_interface s5pv210_pm_interface = {
	.name		= "s5pv210_pm",
	.subsys		= &s5pv210_subsys,
	.add_dev	= s5pv210_pm_add,
};

static __init int s5pv210_pm_drvinit(void)
{
	return subsys_interface_register(&s5pv210_pm_interface);
}
arch_initcall(s5pv210_pm_drvinit);

=======
static int s5pv210_suspend_prepare(void)
{
	s3c_pm_check_prepare();

	return 0;
}

static void s5pv210_suspend_finish(void)
{
	s3c_pm_check_cleanup();
}

static const struct platform_suspend_ops s5pv210_suspend_ops = {
	.enter		= s5pv210_suspend_enter,
	.prepare	= s5pv210_suspend_prepare,
	.finish		= s5pv210_suspend_finish,
	.valid		= suspend_valid_only_mem,
};

/*
 * Syscore operations used to delay restore of certain registers.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void s5pv210_pm_resume(void)
{
	u32 tmp;

	tmp = __raw_readl(S5P_OTHERS);
	tmp |= (S5P_OTHERS_RET_IO | S5P_OTHERS_RET_CF |\
		S5P_OTHERS_RET_MMC | S5P_OTHERS_RET_UART);
	__raw_writel(tmp , S5P_OTHERS);

	s3c_pm_do_restore_core(s5pv210_core_save, ARRAY_SIZE(s5pv210_core_save));
}

static struct syscore_ops s5pv210_pm_syscore_ops = {
	.resume		= s5pv210_pm_resume,
};

<<<<<<< HEAD
static __init int s5pv210_pm_syscore_init(void)
{
	register_syscore_ops(&s5pv210_pm_syscore_ops);
	return 0;
}
arch_initcall(s5pv210_pm_syscore_init);
=======
/*
 * Initialization entry point.
 */
void __init s5pv210_pm_init(void)
{
	register_syscore_ops(&s5pv210_pm_syscore_ops);
	suspend_set_ops(&s5pv210_suspend_ops);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
