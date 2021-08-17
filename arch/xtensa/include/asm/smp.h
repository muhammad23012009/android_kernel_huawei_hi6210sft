/*
<<<<<<< HEAD
 * include/asm-xtensa/smp.h
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
<<<<<<< HEAD
 * Copyright (C) 2001 - 2005 Tensilica Inc.
=======
 * Copyright (C) 2001 - 2013 Tensilica Inc.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#ifndef _XTENSA_SMP_H
#define _XTENSA_SMP_H

<<<<<<< HEAD
extern struct xtensa_cpuinfo boot_cpu_data;

#define cpu_data (&boot_cpu_data)
#define current_cpu_data boot_cpu_data

struct xtensa_cpuinfo {
	unsigned long	*pgd_cache;
	unsigned long	*pte_cache;
	unsigned long	pgtable_cache_sz;
};

#define cpu_logical_map(cpu)	(cpu)
=======
#ifdef CONFIG_SMP

#define raw_smp_processor_id()	(current_thread_info()->cpu)
#define cpu_logical_map(cpu)	(cpu)

struct start_info {
	unsigned long stack;
};
extern struct start_info start_info;

struct cpumask;
void arch_send_call_function_ipi_mask(const struct cpumask *mask);
void arch_send_call_function_single_ipi(int cpu);

void smp_init_cpus(void);
void secondary_init_irq(void);
void ipi_init(void);
struct seq_file;
void show_ipi_list(struct seq_file *p, int prec);

#ifdef CONFIG_HOTPLUG_CPU

void __cpu_die(unsigned int cpu);
int __cpu_disable(void);
void cpu_die(void);
void cpu_restart(void);

#endif /* CONFIG_HOTPLUG_CPU */

#endif /* CONFIG_SMP */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif	/* _XTENSA_SMP_H */
