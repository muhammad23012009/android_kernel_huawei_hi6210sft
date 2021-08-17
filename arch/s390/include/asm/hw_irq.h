#ifndef _HW_IRQ_H
#define _HW_IRQ_H

#include <linux/msi.h>
#include <linux/pci.h>

<<<<<<< HEAD
static inline struct msi_desc *irq_get_msi_desc(unsigned int irq)
{
	return __irq_get_msi_desc(irq);
}

/* Must be called with msi map lock held */
static inline int irq_set_msi_desc(unsigned int irq, struct msi_desc *msi)
{
	if (!msi)
		return -EINVAL;

	msi->irq = irq;
	return 0;
}
=======
void __init init_airq_interrupts(void);
void __init init_cio_interrupts(void);
void __init init_ext_interrupts(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
