/*
 * linux/arch/arm/mach-vexpress/mcpm_platsmp.c
 *
 * Created by:  Nicolas Pitre, November 2012
 * Copyright:   (C) 2012-2013  Linaro Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Code to handle secondary CPU bringup and hotplug for the cluster power API.
 */

#include <linux/init.h>
#include <linux/smp.h>
#include <linux/spinlock.h>
<<<<<<< HEAD
#include <linux/io.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm/mcpm.h>
#include <asm/smp.h>
#include <asm/smp_plat.h>

<<<<<<< HEAD
static void __init simple_smp_init_cpus(void)
{
}

static int __cpuinit mcpm_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned int mpidr, pcpu, pcluster, ret;
	extern void secondary_startup(void);

	mpidr = cpu_logical_map(cpu);
	pcpu = MPIDR_AFFINITY_LEVEL(mpidr, 0);
	pcluster = MPIDR_AFFINITY_LEVEL(mpidr, 1);
=======
static void cpu_to_pcpu(unsigned int cpu,
			unsigned int *pcpu, unsigned int *pcluster)
{
	unsigned int mpidr;

	mpidr = cpu_logical_map(cpu);
	*pcpu = MPIDR_AFFINITY_LEVEL(mpidr, 0);
	*pcluster = MPIDR_AFFINITY_LEVEL(mpidr, 1);
}

static int mcpm_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned int pcpu, pcluster, ret;
	extern void secondary_startup(void);

	cpu_to_pcpu(cpu, &pcpu, &pcluster);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pr_debug("%s: logical CPU %d is physical CPU %d cluster %d\n",
		 __func__, cpu, pcpu, pcluster);

	mcpm_set_entry_vector(pcpu, pcluster, NULL);
<<<<<<< HEAD
#if 0	
#if !defined (CONFIG_HISILICON_PLATFORM_PM_SLEEP)
    /*由于hisi_pm_init函数未实现，mcpm的上电接口会失败，故先打桩*/
    ret = 0;
#else
	ret = mcpm_cpu_power_up(pcpu, pcluster);
	if (ret)
		return ret;
#endif
#endif

	ret = mcpm_cpu_power_up(pcpu, pcluster);
	if (ret)
		return ret;

=======
	ret = mcpm_cpu_power_up(pcpu, pcluster);
	if (ret)
		return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mcpm_set_entry_vector(pcpu, pcluster, secondary_startup);
	arch_send_wakeup_ipi_mask(cpumask_of(cpu));
	dsb_sev();
	return 0;
}

<<<<<<< HEAD
static void __cpuinit mcpm_secondary_init(unsigned int cpu)
=======
static void mcpm_secondary_init(unsigned int cpu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	mcpm_cpu_powered_up();
}

#ifdef CONFIG_HOTPLUG_CPU

<<<<<<< HEAD
static int mcpm_cpu_disable(unsigned int cpu)
{
	/*
	 * We assume all CPUs may be shut down.
	 * This would be the hook to use for eventual Secure
	 * OS migration requests as described in the PSCI spec.
	 */
	return 0;
=======
static int mcpm_cpu_kill(unsigned int cpu)
{
	unsigned int pcpu, pcluster;

	cpu_to_pcpu(cpu, &pcpu, &pcluster);

	return !mcpm_wait_for_cpu_powerdown(pcpu, pcluster);
}

static bool mcpm_cpu_can_disable(unsigned int cpu)
{
	/* We assume all CPUs may be shut down. */
	return true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void mcpm_cpu_die(unsigned int cpu)
{
	unsigned int mpidr, pcpu, pcluster;
	mpidr = read_cpuid_mpidr();
	pcpu = MPIDR_AFFINITY_LEVEL(mpidr, 0);
	pcluster = MPIDR_AFFINITY_LEVEL(mpidr, 1);
	mcpm_set_entry_vector(pcpu, pcluster, NULL);
	mcpm_cpu_power_down();
}

#endif
<<<<<<< HEAD
#if 0
#if defined (CHIP_BB_HI6210)
/*需要将从核跳转的入口地址(函数secondary_startup的地址)写到SRAM的标志单元中*/
extern void secondary_startup(void);
#include <linux/hisi/hi6xxx-iomap.h>                   /* For IO_ADDRESS access */

static void mcpm_smp_prepare_cpus(unsigned int cpu)
{
    unsigned long addr;
    static int flag = 0;

    if (0 != flag)
        return;

    flag = 1;

    addr = (unsigned long)ioremap(MEMORY_AXI_SECOND_CPU_BOOT_ADDR, 0x10);

    printk("mcpm_smp_prepare_cpus: addr 0x%lx at 0x%x\n", addr, virt_to_phys(secondary_startup));

    /*
     * Write the address of secondary startup into the system-wide flags
     * register. The BootMonitor waits for this register to become
     * non-zero.
     */
    *(volatile int *)(addr) = virt_to_phys(secondary_startup);

    iounmap((void __iomem*)addr);

    return;
}
#endif
#endif 

static struct smp_operations __initdata mcpm_smp_ops = {
	.smp_init_cpus		= simple_smp_init_cpus,
	.smp_boot_secondary	= mcpm_boot_secondary,
	.smp_secondary_init	= mcpm_secondary_init,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_disable		= mcpm_cpu_disable,
	.cpu_die		= mcpm_cpu_die,
#endif
#if 0
#if defined (CHIP_BB_HI6210)
    .smp_prepare_cpus = mcpm_smp_prepare_cpus
#endif
#endif
=======

static const struct smp_operations mcpm_smp_ops __initconst = {
	.smp_boot_secondary	= mcpm_boot_secondary,
	.smp_secondary_init	= mcpm_secondary_init,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_kill		= mcpm_cpu_kill,
	.cpu_can_disable	= mcpm_cpu_can_disable,
	.cpu_die		= mcpm_cpu_die,
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

void __init mcpm_smp_set_ops(void)
{
	smp_set_ops(&mcpm_smp_ops);
}
