/*
 * OMAP4 SMP cpu-hotplug support
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 * Author:
 *      Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * Platform file needed for the OMAP4 SMP. This file is based on arm
 * realview smp platform.
 * Copyright (c) 2002 ARM Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/smp.h>
#include <linux/io.h>

#include "omap-wakeupgen.h"
#include "common.h"
#include "powerdomain.h"

/*
 * platform-specific code to shutdown a CPU
 * Called with IRQs disabled
 */
<<<<<<< HEAD
void __ref omap4_cpu_die(unsigned int cpu)
=======
void omap4_cpu_die(unsigned int cpu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned int boot_cpu = 0;
	void __iomem *base = omap_get_wakeupgen_base();

	/*
	 * we're ready for shutdown now, so do it
	 */
	if (omap_secure_apis_support()) {
		if (omap_modify_auxcoreboot0(0x0, 0x200) != 0x0)
			pr_err("Secure clear status failed\n");
	} else {
<<<<<<< HEAD
		__raw_writel(0, base + OMAP_AUX_CORE_BOOT_0);
=======
		writel_relaxed(0, base + OMAP_AUX_CORE_BOOT_0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}


	for (;;) {
		/*
		 * Enter into low power state
		 */
		omap4_hotplug_cpu(cpu, PWRDM_POWER_OFF);

		if (omap_secure_apis_support())
			boot_cpu = omap_read_auxcoreboot0();
		else
			boot_cpu =
<<<<<<< HEAD
				__raw_readl(base + OMAP_AUX_CORE_BOOT_0) >> 5;
=======
				readl_relaxed(base + OMAP_AUX_CORE_BOOT_0) >> 5;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (boot_cpu == smp_processor_id()) {
			/*
			 * OK, proper wakeup, we're done
			 */
			break;
		}
		pr_debug("CPU%u: spurious wakeup call\n", cpu);
	}
}
<<<<<<< HEAD
=======

/* Needed by kexec and platform_can_cpu_hotplug() */
int omap4_cpu_kill(unsigned int cpu)
{
	return 1;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
