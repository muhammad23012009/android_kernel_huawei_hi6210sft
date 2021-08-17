/*
 * arch/arm/mach-mvebu/include/mach/coherency.h
 *
 *
 * Coherency fabric (Aurora) support for Armada 370 and XP platforms.
 *
 * Copyright (C) 2012 Marvell
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __MACH_370_XP_COHERENCY_H
#define __MACH_370_XP_COHERENCY_H

<<<<<<< HEAD
#ifdef CONFIG_SMP
int coherency_get_cpu_count(void);
#endif

int set_cpu_coherent(int cpu_id, int smp_group_id);
int coherency_init(void);
=======
extern void __iomem *coherency_base;	/* for coherency_ll.S */
extern unsigned long coherency_phys_base;
int set_cpu_coherent(void);

int coherency_init(void);
int coherency_available(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif	/* __MACH_370_XP_COHERENCY_H */
