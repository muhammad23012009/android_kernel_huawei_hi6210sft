#ifndef _LINUX_CPUPRI_H
#define _LINUX_CPUPRI_H

#include <linux/sched.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO + 2)

#define CPUPRI_INVALID -1
#define CPUPRI_IDLE     0
#define CPUPRI_NORMAL   1
/* values 2-101 are RT priorities 0-99 */

struct cpupri_vec {
	atomic_t	count;
	cpumask_var_t	mask;
};

struct cpupri {
	struct cpupri_vec pri_to_cpu[CPUPRI_NR_PRIORITIES];
<<<<<<< HEAD
	int               cpu_to_pri[NR_CPUS];
=======
	int *cpu_to_pri;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#ifdef CONFIG_SMP
int  cpupri_find(struct cpupri *cp,
		 struct task_struct *p, struct cpumask *lowest_mask);
void cpupri_set(struct cpupri *cp, int cpu, int pri);
int cpupri_init(struct cpupri *cp);
void cpupri_cleanup(struct cpupri *cp);
<<<<<<< HEAD
#else
#define cpupri_set(cp, cpu, pri) do { } while (0)
#define cpupri_init() do { } while (0)
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

#endif /* _LINUX_CPUPRI_H */
