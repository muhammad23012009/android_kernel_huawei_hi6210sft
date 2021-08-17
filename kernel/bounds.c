/*
 * Generate definitions needed by the preprocessor.
 * This code generates raw asm output which is post-processed
 * to extract and format the required data.
 */

#define __GENERATING_BOUNDS_H
/* Include headers that define the enum constants of interest */
#include <linux/page-flags.h>
#include <linux/mmzone.h>
#include <linux/kbuild.h>
<<<<<<< HEAD
#include <linux/page_cgroup.h>

void foo(void)
=======
#include <linux/log2.h>
#include <linux/spinlock_types.h>

int main(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	/* The enum constants to put into include/generated/bounds.h */
	DEFINE(NR_PAGEFLAGS, __NR_PAGEFLAGS);
	DEFINE(MAX_NR_ZONES, __MAX_NR_ZONES);
<<<<<<< HEAD
	DEFINE(NR_PCG_FLAGS, __NR_PCG_FLAGS);
	/* End of constants */
=======
#ifdef CONFIG_SMP
	DEFINE(NR_CPUS_BITS, ilog2(CONFIG_NR_CPUS));
#endif
	DEFINE(SPINLOCK_SIZE, sizeof(spinlock_t));
	/* End of constants */

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
