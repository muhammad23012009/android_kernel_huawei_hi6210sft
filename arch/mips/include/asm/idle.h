#ifndef __ASM_IDLE_H
#define __ASM_IDLE_H

<<<<<<< HEAD
=======
#include <linux/cpuidle.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/linkage.h>

extern void (*cpu_wait)(void);
extern void r4k_wait(void);
extern asmlinkage void __r4k_wait(void);
extern void r4k_wait_irqoff(void);
<<<<<<< HEAD
extern void __pastwait(void);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline int using_rollback_handler(void)
{
	return cpu_wait == r4k_wait;
}

<<<<<<< HEAD
static inline int address_is_in_r4k_wait_irqoff(unsigned long addr)
{
	return addr >= (unsigned long)r4k_wait_irqoff &&
	       addr < (unsigned long)__pastwait;
=======
extern int mips_cpuidle_wait_enter(struct cpuidle_device *dev,
				   struct cpuidle_driver *drv, int index);

#define MIPS_CPUIDLE_WAIT_STATE {\
	.enter			= mips_cpuidle_wait_enter,\
	.exit_latency		= 1,\
	.target_residency	= 1,\
	.power_usage		= UINT_MAX,\
	.name			= "wait",\
	.desc			= "MIPS wait",\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif /* __ASM_IDLE_H  */
