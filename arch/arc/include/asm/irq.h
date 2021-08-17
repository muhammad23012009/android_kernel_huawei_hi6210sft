/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARC_IRQ_H
#define __ASM_ARC_IRQ_H

#define NR_CPU_IRQS	32  /* number of interrupt lines of ARC770 CPU */
#define NR_IRQS		128 /* allow some CPU external IRQ handling */

/* Platform Independent IRQs */
<<<<<<< HEAD
#define TIMER0_IRQ      3
#define TIMER1_IRQ      4

#include <asm-generic/irq.h>

extern void __init arc_init_IRQ(void);
extern int __init get_hw_config_num_irq(void);

void __cpuinit arc_local_timer_setup(unsigned int cpu);
=======
#ifdef CONFIG_ISA_ARCV2
#define IPI_IRQ		19
#define SOFTIRQ_IRQ	21
#endif

#include <linux/interrupt.h>
#include <asm-generic/irq.h>

extern void arc_init_IRQ(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
