#ifndef _LINUX_IRQNR_H
#define _LINUX_IRQNR_H

#include <uapi/linux/irqnr.h>


<<<<<<< HEAD
#ifndef CONFIG_GENERIC_HARDIRQS
#include <asm/irq.h>

/*
 * Wrappers for non-genirq architectures:
 */
#define nr_irqs			NR_IRQS
#define irq_to_desc(irq)	(&irq_desc[irq])

# define for_each_irq_desc(irq, desc)		\
	for (irq = 0; irq < nr_irqs; irq++)

# define for_each_irq_desc_reverse(irq, desc)                          \
	for (irq = nr_irqs - 1; irq >= 0; irq--)

#else /* CONFIG_GENERIC_HARDIRQS */

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int nr_irqs;
extern struct irq_desc *irq_to_desc(unsigned int irq);
unsigned int irq_get_next_irq(unsigned int offset);

# define for_each_irq_desc(irq, desc)					\
	for (irq = 0, desc = irq_to_desc(irq); irq < nr_irqs;		\
	     irq++, desc = irq_to_desc(irq))				\
		if (!desc)						\
			;						\
		else


# define for_each_irq_desc_reverse(irq, desc)				\
	for (irq = nr_irqs - 1, desc = irq_to_desc(irq); irq >= 0;	\
	     irq--, desc = irq_to_desc(irq))				\
		if (!desc)						\
			;						\
		else

<<<<<<< HEAD
#ifdef CONFIG_SMP
#define irq_node(irq)	(irq_get_irq_data(irq)->node)
#else
#define irq_node(irq)	0
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
# define for_each_active_irq(irq)			\
	for (irq = irq_get_next_irq(0); irq < nr_irqs;	\
	     irq = irq_get_next_irq(irq + 1))

<<<<<<< HEAD
#endif /* CONFIG_GENERIC_HARDIRQS */

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define for_each_irq_nr(irq)                   \
       for (irq = 0; irq < nr_irqs; irq++)

#endif
