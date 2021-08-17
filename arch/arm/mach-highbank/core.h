#ifndef __HIGHBANK_CORE_H
#define __HIGHBANK_CORE_H

<<<<<<< HEAD
extern void highbank_set_cpu_jump(int cpu, void *jump_addr);
extern void highbank_restart(char, const char *);
=======
#include <linux/reboot.h>

extern void highbank_restart(enum reboot_mode, const char *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern void __iomem *scu_base_addr;

#ifdef CONFIG_PM_SLEEP
extern void highbank_pm_init(void);
#else
static inline void highbank_pm_init(void) {}
#endif

extern void highbank_smc1(int fn, int arg);
<<<<<<< HEAD
extern void highbank_cpu_die(unsigned int cpu);

extern struct smp_operations highbank_smp_ops;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
