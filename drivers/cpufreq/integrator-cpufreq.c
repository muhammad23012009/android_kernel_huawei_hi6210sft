/*
 *  Copyright (C) 2001-2002 Deep Blue Solutions Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * CPU support functions
 */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/cpufreq.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/init.h>
#include <linux/io.h>
<<<<<<< HEAD

#include <mach/hardware.h>
#include <mach/platform.h>
#include <asm/mach-types.h>
#include <asm/hardware/icst.h>

static struct cpufreq_driver integrator_driver;

#define CM_ID  	__io_address(INTEGRATOR_HDR_ID)
#define CM_OSC	__io_address(INTEGRATOR_HDR_OSC)
#define CM_STAT __io_address(INTEGRATOR_HDR_STAT)
#define CM_LOCK __io_address(INTEGRATOR_HDR_LOCK)
=======
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>

#include <asm/mach-types.h>
#include <asm/hardware/icst.h>

static void __iomem *cm_base;
/* The cpufreq driver only use the OSC register */
#define INTEGRATOR_HDR_OSC_OFFSET       0x08
#define INTEGRATOR_HDR_LOCK_OFFSET      0x14

static struct cpufreq_driver integrator_driver;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static const struct icst_params lclk_params = {
	.ref		= 24000000,
	.vco_max	= ICST525_VCO_MAX_5V,
	.vco_min	= ICST525_VCO_MIN,
	.vd_min		= 8,
	.vd_max		= 132,
	.rd_min		= 24,
	.rd_max		= 24,
	.s2div		= icst525_s2div,
	.idx2s		= icst525_idx2s,
};

static const struct icst_params cclk_params = {
	.ref		= 24000000,
	.vco_max	= ICST525_VCO_MAX_5V,
	.vco_min	= ICST525_VCO_MIN,
	.vd_min		= 12,
	.vd_max		= 160,
	.rd_min		= 24,
	.rd_max		= 24,
	.s2div		= icst525_s2div,
	.idx2s		= icst525_idx2s,
};

/*
 * Validate the speed policy.
 */
static int integrator_verify_policy(struct cpufreq_policy *policy)
{
	struct icst_vco vco;

<<<<<<< HEAD
	cpufreq_verify_within_limits(policy, 
				     policy->cpuinfo.min_freq, 
				     policy->cpuinfo.max_freq);
=======
	cpufreq_verify_within_cpu_limits(policy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	vco = icst_hz_to_vco(&cclk_params, policy->max * 1000);
	policy->max = icst_hz(&cclk_params, vco) / 1000;

	vco = icst_hz_to_vco(&cclk_params, policy->min * 1000);
	policy->min = icst_hz(&cclk_params, vco) / 1000;

<<<<<<< HEAD
	cpufreq_verify_within_limits(policy, 
				     policy->cpuinfo.min_freq, 
				     policy->cpuinfo.max_freq);

=======
	cpufreq_verify_within_cpu_limits(policy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}


static int integrator_set_target(struct cpufreq_policy *policy,
				 unsigned int target_freq,
				 unsigned int relation)
{
	cpumask_t cpus_allowed;
	int cpu = policy->cpu;
	struct icst_vco vco;
	struct cpufreq_freqs freqs;
	u_int cm_osc;

	/*
	 * Save this threads cpus_allowed mask.
	 */
	cpus_allowed = current->cpus_allowed;

	/*
	 * Bind to the specified CPU.  When this call returns,
	 * we should be running on the right CPU.
	 */
<<<<<<< HEAD
	set_cpus_allowed(current, cpumask_of_cpu(cpu));
	BUG_ON(cpu != smp_processor_id());

	/* get current setting */
	cm_osc = __raw_readl(CM_OSC);

	if (machine_is_integrator()) {
		vco.s = (cm_osc >> 8) & 7;
	} else if (machine_is_cintegrator()) {
		vco.s = 1;
	}
=======
	set_cpus_allowed_ptr(current, cpumask_of(cpu));
	BUG_ON(cpu != smp_processor_id());

	/* get current setting */
	cm_osc = __raw_readl(cm_base + INTEGRATOR_HDR_OSC_OFFSET);

	if (machine_is_integrator())
		vco.s = (cm_osc >> 8) & 7;
	else if (machine_is_cintegrator())
		vco.s = 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	vco.v = cm_osc & 255;
	vco.r = 22;
	freqs.old = icst_hz(&cclk_params, vco) / 1000;

	/* icst_hz_to_vco rounds down -- so we need the next
	 * larger freq in case of CPUFREQ_RELATION_L.
	 */
	if (relation == CPUFREQ_RELATION_L)
		target_freq += 999;
	if (target_freq > policy->max)
		target_freq = policy->max;
	vco = icst_hz_to_vco(&cclk_params, target_freq * 1000);
	freqs.new = icst_hz(&cclk_params, vco) / 1000;

	if (freqs.old == freqs.new) {
<<<<<<< HEAD
		set_cpus_allowed(current, cpus_allowed);
		return 0;
	}

	cpufreq_notify_transition(policy, &freqs, CPUFREQ_PRECHANGE);

	cm_osc = __raw_readl(CM_OSC);
=======
		set_cpus_allowed_ptr(current, &cpus_allowed);
		return 0;
	}

	cpufreq_freq_transition_begin(policy, &freqs);

	cm_osc = __raw_readl(cm_base + INTEGRATOR_HDR_OSC_OFFSET);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (machine_is_integrator()) {
		cm_osc &= 0xfffff800;
		cm_osc |= vco.s << 8;
	} else if (machine_is_cintegrator()) {
		cm_osc &= 0xffffff00;
	}
	cm_osc |= vco.v;

<<<<<<< HEAD
	__raw_writel(0xa05f, CM_LOCK);
	__raw_writel(cm_osc, CM_OSC);
	__raw_writel(0, CM_LOCK);
=======
	__raw_writel(0xa05f, cm_base + INTEGRATOR_HDR_LOCK_OFFSET);
	__raw_writel(cm_osc, cm_base + INTEGRATOR_HDR_OSC_OFFSET);
	__raw_writel(0, cm_base + INTEGRATOR_HDR_LOCK_OFFSET);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Restore the CPUs allowed mask.
	 */
<<<<<<< HEAD
	set_cpus_allowed(current, cpus_allowed);

	cpufreq_notify_transition(policy, &freqs, CPUFREQ_POSTCHANGE);
=======
	set_cpus_allowed_ptr(current, &cpus_allowed);

	cpufreq_freq_transition_end(policy, &freqs, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static unsigned int integrator_get(unsigned int cpu)
{
	cpumask_t cpus_allowed;
	unsigned int current_freq;
	u_int cm_osc;
	struct icst_vco vco;

	cpus_allowed = current->cpus_allowed;

<<<<<<< HEAD
	set_cpus_allowed(current, cpumask_of_cpu(cpu));
	BUG_ON(cpu != smp_processor_id());

	/* detect memory etc. */
	cm_osc = __raw_readl(CM_OSC);

	if (machine_is_integrator()) {
		vco.s = (cm_osc >> 8) & 7;
	} else {
		vco.s = 1;
	}
=======
	set_cpus_allowed_ptr(current, cpumask_of(cpu));
	BUG_ON(cpu != smp_processor_id());

	/* detect memory etc. */
	cm_osc = __raw_readl(cm_base + INTEGRATOR_HDR_OSC_OFFSET);

	if (machine_is_integrator())
		vco.s = (cm_osc >> 8) & 7;
	else
		vco.s = 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	vco.v = cm_osc & 255;
	vco.r = 22;

	current_freq = icst_hz(&cclk_params, vco) / 1000; /* current freq */

<<<<<<< HEAD
	set_cpus_allowed(current, cpus_allowed);
=======
	set_cpus_allowed_ptr(current, &cpus_allowed);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return current_freq;
}

static int integrator_cpufreq_init(struct cpufreq_policy *policy)
{

	/* set default policy and cpuinfo */
<<<<<<< HEAD
	policy->cpuinfo.max_freq = 160000;
	policy->cpuinfo.min_freq = 12000;
	policy->cpuinfo.transition_latency = 1000000; /* 1 ms, assumed */
	policy->cur = policy->min = policy->max = integrator_get(policy->cpu);
=======
	policy->max = policy->cpuinfo.max_freq = 160000;
	policy->min = policy->cpuinfo.min_freq = 12000;
	policy->cpuinfo.transition_latency = 1000000; /* 1 ms, assumed */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static struct cpufreq_driver integrator_driver = {
<<<<<<< HEAD
=======
	.flags		= CPUFREQ_NEED_INITIAL_FREQ_CHECK,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.verify		= integrator_verify_policy,
	.target		= integrator_set_target,
	.get		= integrator_get,
	.init		= integrator_cpufreq_init,
	.name		= "integrator",
};

<<<<<<< HEAD
static int __init integrator_cpu_init(void)
{
	return cpufreq_register_driver(&integrator_driver);
}

static void __exit integrator_cpu_exit(void)
{
	cpufreq_unregister_driver(&integrator_driver);
}

MODULE_AUTHOR ("Russell M. King");
MODULE_DESCRIPTION ("cpufreq driver for ARM Integrator CPUs");
MODULE_LICENSE ("GPL");

module_init(integrator_cpu_init);
module_exit(integrator_cpu_exit);
=======
static int __init integrator_cpufreq_probe(struct platform_device *pdev)
{
	struct resource *res;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	cm_base = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (!cm_base)
		return -ENODEV;

	return cpufreq_register_driver(&integrator_driver);
}

static int __exit integrator_cpufreq_remove(struct platform_device *pdev)
{
	return cpufreq_unregister_driver(&integrator_driver);
}

static const struct of_device_id integrator_cpufreq_match[] = {
	{ .compatible = "arm,core-module-integrator"},
	{ },
};

MODULE_DEVICE_TABLE(of, integrator_cpufreq_match);

static struct platform_driver integrator_cpufreq_driver = {
	.driver = {
		.name = "integrator-cpufreq",
		.of_match_table = integrator_cpufreq_match,
	},
	.remove = __exit_p(integrator_cpufreq_remove),
};

module_platform_driver_probe(integrator_cpufreq_driver,
			     integrator_cpufreq_probe);

MODULE_AUTHOR("Russell M. King");
MODULE_DESCRIPTION("cpufreq driver for ARM Integrator CPUs");
MODULE_LICENSE("GPL");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
