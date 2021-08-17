/*
 * CPU hotplug support for CSR Marco dual-core SMP SoCs
 *
 * Copyright (c) 2012 Cambridge Silicon Radio Limited, a CSR plc group company.
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/smp.h>

#include <asm/smp_plat.h>

static inline void platform_do_lowpower(unsigned int cpu)
{
	/* we put the platform to just WFI */
	for (;;) {
		__asm__ __volatile__("dsb\n\t" "wfi\n\t"
			: : : "memory");
		if (pen_release == cpu_logical_map(cpu)) {
			/*
			 * OK, proper wakeup, we're done
			 */
			break;
		}
	}
}

/*
 * platform-specific code to shutdown a CPU
 *
 * Called with IRQs disabled
 */
<<<<<<< HEAD
void __ref sirfsoc_cpu_die(unsigned int cpu)
=======
void sirfsoc_cpu_die(unsigned int cpu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	platform_do_lowpower(cpu);
}
