#ifndef _CPUMAP_H
#define _CPUMAP_H

#ifdef CONFIG_SMP
<<<<<<< HEAD
extern void cpu_map_rebuild(void);
extern int  map_to_cpu(unsigned int index);
=======
void cpu_map_rebuild(void);
int map_to_cpu(unsigned int index);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define cpu_map_init() cpu_map_rebuild()
#else
#define cpu_map_init() do {} while (0)
static inline int map_to_cpu(unsigned int index)
{
	return raw_smp_processor_id();
}
#endif

#endif
