#ifndef __ASM_TOPOLOGY_H
#define __ASM_TOPOLOGY_H

<<<<<<< HEAD
#ifdef CONFIG_SMP

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/cpumask.h>

struct cpu_topology {
	int thread_id;
	int core_id;
	int cluster_id;
	cpumask_t thread_sibling;
	cpumask_t core_sibling;
};

extern struct cpu_topology cpu_topology[NR_CPUS];

#define topology_physical_package_id(cpu)	(cpu_topology[cpu].cluster_id)
#define topology_core_id(cpu)		(cpu_topology[cpu].core_id)
#define topology_core_cpumask(cpu)	(&cpu_topology[cpu].core_sibling)
<<<<<<< HEAD
#define topology_thread_cpumask(cpu)	(&cpu_topology[cpu].thread_sibling)

#define mc_capable()	(cpu_topology[0].cluster_id != -1)
#define smt_capable()	(cpu_topology[0].thread_id != -1)
=======
#define topology_sibling_cpumask(cpu)	(&cpu_topology[cpu].thread_sibling)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void init_cpu_topology(void);
void store_cpu_topology(unsigned int cpuid);
const struct cpumask *cpu_coregroup_mask(int cpu);

<<<<<<< HEAD
#ifdef CONFIG_DISABLE_CPU_SCHED_DOMAIN_BALANCE
/* Common values for CPUs */
#ifndef SD_CPU_INIT
#define SD_CPU_INIT (struct sched_domain) {				\
	.min_interval		= 1,					\
	.max_interval		= 4,					\
	.busy_factor		= 64,					\
	.imbalance_pct		= 125,					\
	.cache_nice_tries	= 1,					\
	.busy_idx		= 2,					\
	.idle_idx		= 1,					\
	.newidle_idx		= 0,					\
	.wake_idx		= 0,					\
	.forkexec_idx		= 0,					\
									\
	.flags			= 0*SD_LOAD_BALANCE			\
				| 1*SD_BALANCE_NEWIDLE			\
				| 1*SD_BALANCE_EXEC			\
				| 1*SD_BALANCE_FORK			\
				| 0*SD_BALANCE_WAKE			\
				| 1*SD_WAKE_AFFINE			\
				| 0*SD_SHARE_CPUPOWER			\
				| 0*SD_SHARE_PKG_RESOURCES		\
				| 0*SD_SERIALIZE			\
				,					\
	.last_balance		 = jiffies,				\
	.balance_interval	= 1,					\
}
#endif
#endif /* CONFIG_DISABLE_CPU_SCHED_DOMAIN_BALANCE */

#else

static inline void init_cpu_topology(void) { }
static inline void store_cpu_topology(unsigned int cpuid) { }

#endif
=======
#ifdef CONFIG_NUMA

struct pci_bus;
int pcibus_to_node(struct pci_bus *bus);
#define cpumask_of_pcibus(bus)	(pcibus_to_node(bus) == -1 ?		\
				 cpu_all_mask :				\
				 cpumask_of_node(pcibus_to_node(bus)))

#endif /* CONFIG_NUMA */
struct sched_domain;
#ifdef CONFIG_CPU_FREQ
#define arch_scale_freq_capacity cpufreq_scale_freq_capacity
extern unsigned long cpufreq_scale_freq_capacity(struct sched_domain *sd, int cpu);
extern unsigned long cpufreq_scale_max_freq_capacity(int cpu);
#endif
#define arch_scale_cpu_capacity scale_cpu_capacity
extern unsigned long scale_cpu_capacity(struct sched_domain *sd, int cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm-generic/topology.h>

#endif /* _ASM_ARM_TOPOLOGY_H */
