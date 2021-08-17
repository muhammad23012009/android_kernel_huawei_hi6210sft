#ifndef _ASM_POWERPC_HARDIRQ_H
#define _ASM_POWERPC_HARDIRQ_H

#include <linux/threads.h>
#include <linux/irq.h>

typedef struct {
	unsigned int __softirq_pending;
<<<<<<< HEAD
	unsigned int timer_irqs;
	unsigned int pmu_irqs;
	unsigned int mce_exceptions;
	unsigned int spurious_irqs;
=======
	unsigned int timer_irqs_event;
	unsigned int timer_irqs_others;
	unsigned int pmu_irqs;
	unsigned int mce_exceptions;
	unsigned int spurious_irqs;
	unsigned int hmi_exceptions;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_PPC_DOORBELL
	unsigned int doorbell_irqs;
#endif
} ____cacheline_aligned irq_cpustat_t;

DECLARE_PER_CPU_SHARED_ALIGNED(irq_cpustat_t, irq_stat);

#define __ARCH_IRQ_STAT

<<<<<<< HEAD
#define local_softirq_pending()	__get_cpu_var(irq_stat).__softirq_pending
=======
#define local_softirq_pending()	__this_cpu_read(irq_stat.__softirq_pending)

#define __ARCH_SET_SOFTIRQ_PENDING

#define set_softirq_pending(x) __this_cpu_write(irq_stat.__softirq_pending, (x))
#define or_softirq_pending(x) __this_cpu_or(irq_stat.__softirq_pending, (x))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline void ack_bad_irq(unsigned int irq)
{
	printk(KERN_CRIT "unexpected IRQ trap at vector %02x\n", irq);
}

extern u64 arch_irq_stat_cpu(unsigned int cpu);
#define arch_irq_stat_cpu	arch_irq_stat_cpu

#endif /* _ASM_POWERPC_HARDIRQ_H */
