#ifndef POWERPC_85XX_SMP_H_
#define POWERPC_85XX_SMP_H_ 1

#include <linux/init.h>

#ifdef CONFIG_SMP
void __init mpc85xx_smp_init(void);
<<<<<<< HEAD
=======
int __init mpc85xx_setup_pmc(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
static inline void mpc85xx_smp_init(void)
{
	/* Nothing to do */
}
#endif

#endif /* not POWERPC_85XX_SMP_H_ */
