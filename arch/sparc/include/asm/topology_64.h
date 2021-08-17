#ifndef _ASM_SPARC64_TOPOLOGY_H
#define _ASM_SPARC64_TOPOLOGY_H

#ifdef CONFIG_NUMA

#include <asm/mmzone.h>

static inline int cpu_to_node(int cpu)
{
	return numa_cpu_lookup_table[cpu];
}

#define parent_node(node)	(node)

#define cpumask_of_node(node) ((node) == -1 ?				\
			       cpu_all_mask :				\
			       &numa_cpumask_lookup_table[node])

struct pci_bus;
#ifdef CONFIG_PCI
<<<<<<< HEAD
extern int pcibus_to_node(struct pci_bus *pbus);
=======
int pcibus_to_node(struct pci_bus *pbus);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
static inline int pcibus_to_node(struct pci_bus *pbus)
{
	return -1;
}
#endif

#define cpumask_of_pcibus(bus)	\
	(pcibus_to_node(bus) == -1 ? \
	 cpu_all_mask : \
	 cpumask_of_node(pcibus_to_node(bus)))

<<<<<<< HEAD
=======
int __node_distance(int, int);
#define node_distance(a, b) __node_distance(a, b)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else /* CONFIG_NUMA */

#include <asm-generic/topology.h>

#endif /* !(CONFIG_NUMA) */

#ifdef CONFIG_SMP
#define topology_physical_package_id(cpu)	(cpu_data(cpu).proc_id)
#define topology_core_id(cpu)			(cpu_data(cpu).core_id)
<<<<<<< HEAD
#define topology_core_cpumask(cpu)		(&cpu_core_map[cpu])
#define topology_thread_cpumask(cpu)		(&per_cpu(cpu_sibling_map, cpu))
#define mc_capable()				(sparc64_multi_core)
#define smt_capable()				(sparc64_multi_core)
#endif /* CONFIG_SMP */

extern cpumask_t cpu_core_map[NR_CPUS];
static inline const struct cpumask *cpu_coregroup_mask(int cpu)
{
        return &cpu_core_map[cpu];
=======
#define topology_core_cpumask(cpu)		(&cpu_core_sib_map[cpu])
#define topology_core_cache_cpumask(cpu)	(&cpu_core_sib_cache_map[cpu])
#define topology_sibling_cpumask(cpu)		(&per_cpu(cpu_sibling_map, cpu))
#endif /* CONFIG_SMP */

extern cpumask_t cpu_core_map[NR_CPUS];
extern cpumask_t cpu_core_sib_map[NR_CPUS];
extern cpumask_t cpu_core_sib_cache_map[NR_CPUS];

/**
 * Return cores that shares the last level cache.
 */
static inline const struct cpumask *cpu_coregroup_mask(int cpu)
{
	return &cpu_core_sib_cache_map[cpu];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif /* _ASM_SPARC64_TOPOLOGY_H */
