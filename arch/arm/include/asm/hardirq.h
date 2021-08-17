#ifndef __ASM_HARDIRQ_H
#define __ASM_HARDIRQ_H

#include <linux/cache.h>
#include <linux/threads.h>
#include <asm/irq.h>

<<<<<<< HEAD
#ifdef CONFIG_SECURE_EXTENSION
#define NR_IPI  9
#else
#define NR_IPI	8
#endif
=======
/* number of IPIS _not_ including IPI_CPU_BACKTRACE */
#define NR_IPI	7
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

typedef struct {
	unsigned int __softirq_pending;
#ifdef CONFIG_SMP
	unsigned int ipi_irqs[NR_IPI];
#endif
} ____cacheline_aligned irq_cpustat_t;

#include <linux/irq_cpustat.h>	/* Standard mappings for irq_cpustat_t above */

#define __inc_irq_stat(cpu, member)	__IRQ_STAT(cpu, member)++
#define __get_irq_stat(cpu, member)	__IRQ_STAT(cpu, member)

#ifdef CONFIG_SMP
u64 smp_irq_stat_cpu(unsigned int cpu);
#else
#define smp_irq_stat_cpu(cpu)	0
#endif

#define arch_irq_stat_cpu	smp_irq_stat_cpu

#define __ARCH_IRQ_EXIT_IRQS_DISABLED	1

#endif /* __ASM_HARDIRQ_H */
