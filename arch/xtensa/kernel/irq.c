/*
 * linux/arch/xtensa/kernel/irq.c
 *
 * Xtensa built-in interrupt controller and some generic functions copied
 * from i386.
 *
<<<<<<< HEAD
 * Copyright (C) 2002 - 2006 Tensilica, Inc.
=======
 * Copyright (C) 2002 - 2013 Tensilica, Inc.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Copyright (C) 1992, 1998 Linus Torvalds, Ingo Molnar
 *
 *
 * Chris Zankel <chris@zankel.net>
 * Kevin Chea
 *
 */

#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel_stat.h>
<<<<<<< HEAD
#include <linux/irqdomain.h>
#include <linux/of.h>

#include <asm/uaccess.h>
#include <asm/platform.h>

static unsigned int cached_irq_mask;

atomic_t irq_err_count;

static struct irq_domain *root_domain;

/*
 * do_IRQ handles all normal device IRQ's (the special
 * SMP cross-CPU interrupts have their own specific
 * handlers).
 */

asmlinkage void do_IRQ(int hwirq, struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);
	int irq = irq_find_mapping(root_domain, hwirq);

	if (hwirq >= NR_IRQS) {
		printk(KERN_EMERG "%s: cannot handle IRQ %d\n",
				__func__, hwirq);
	}

	irq_enter();
=======
#include <linux/irqchip.h>
#include <linux/irqchip/xtensa-mx.h>
#include <linux/irqchip/xtensa-pic.h>
#include <linux/irqdomain.h>
#include <linux/of.h>

#include <asm/mxregs.h>
#include <asm/uaccess.h>
#include <asm/platform.h>

DECLARE_PER_CPU(unsigned long, nmi_count);

asmlinkage void do_IRQ(int hwirq, struct pt_regs *regs)
{
	int irq = irq_find_mapping(NULL, hwirq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_DEBUG_STACKOVERFLOW
	/* Debugging check for stack overflow: is there less than 1KB free? */
	{
		unsigned long sp;

		__asm__ __volatile__ ("mov %0, a1\n" : "=a" (sp));
		sp &= THREAD_SIZE - 1;

		if (unlikely(sp < (sizeof(thread_info) + 1024)))
			printk("Stack overflow in do_IRQ: %ld\n",
			       sp - sizeof(struct thread_info));
	}
#endif
	generic_handle_irq(irq);
<<<<<<< HEAD

	irq_exit();
	set_irq_regs(old_regs);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

int arch_show_interrupts(struct seq_file *p, int prec)
{
<<<<<<< HEAD
	seq_printf(p, "%*s: ", prec, "ERR");
	seq_printf(p, "%10u\n", atomic_read(&irq_err_count));
	return 0;
}

static void xtensa_irq_mask(struct irq_data *d)
{
	cached_irq_mask &= ~(1 << d->hwirq);
	set_sr (cached_irq_mask, intenable);
}

static void xtensa_irq_unmask(struct irq_data *d)
{
	cached_irq_mask |= 1 << d->hwirq;
	set_sr (cached_irq_mask, intenable);
}

static void xtensa_irq_enable(struct irq_data *d)
{
	variant_irq_enable(d->hwirq);
	xtensa_irq_unmask(d);
}

static void xtensa_irq_disable(struct irq_data *d)
{
	xtensa_irq_mask(d);
	variant_irq_disable(d->hwirq);
}

static void xtensa_irq_ack(struct irq_data *d)
{
	set_sr(1 << d->hwirq, intclear);
}

static int xtensa_irq_retrigger(struct irq_data *d)
{
	set_sr(1 << d->hwirq, intset);
	return 1;
}

static struct irq_chip xtensa_irq_chip = {
	.name		= "xtensa",
	.irq_enable	= xtensa_irq_enable,
	.irq_disable	= xtensa_irq_disable,
	.irq_mask	= xtensa_irq_mask,
	.irq_unmask	= xtensa_irq_unmask,
	.irq_ack	= xtensa_irq_ack,
	.irq_retrigger	= xtensa_irq_retrigger,
};

static int xtensa_irq_map(struct irq_domain *d, unsigned int irq,
		irq_hw_number_t hw)
{
	u32 mask = 1 << hw;

	if (mask & XCHAL_INTTYPE_MASK_SOFTWARE) {
		irq_set_chip_and_handler_name(irq, &xtensa_irq_chip,
				handle_simple_irq, "level");
		irq_set_status_flags(irq, IRQ_LEVEL);
	} else if (mask & XCHAL_INTTYPE_MASK_EXTERN_EDGE) {
		irq_set_chip_and_handler_name(irq, &xtensa_irq_chip,
				handle_edge_irq, "edge");
		irq_clear_status_flags(irq, IRQ_LEVEL);
	} else if (mask & XCHAL_INTTYPE_MASK_EXTERN_LEVEL) {
		irq_set_chip_and_handler_name(irq, &xtensa_irq_chip,
				handle_level_irq, "level");
		irq_set_status_flags(irq, IRQ_LEVEL);
	} else if (mask & XCHAL_INTTYPE_MASK_TIMER) {
		irq_set_chip_and_handler_name(irq, &xtensa_irq_chip,
				handle_edge_irq, "edge");
		irq_clear_status_flags(irq, IRQ_LEVEL);
	} else {/* XCHAL_INTTYPE_MASK_WRITE_ERROR */
		/* XCHAL_INTTYPE_MASK_NMI */

		irq_set_chip_and_handler_name(irq, &xtensa_irq_chip,
=======
	unsigned cpu __maybe_unused;
#ifdef CONFIG_SMP
	show_ipi_list(p, prec);
#endif
#if XTENSA_FAKE_NMI
	seq_printf(p, "%*s:", prec, "NMI");
	for_each_online_cpu(cpu)
		seq_printf(p, " %10lu", per_cpu(nmi_count, cpu));
	seq_puts(p, "   Non-maskable interrupts\n");
#endif
	return 0;
}

int xtensa_irq_domain_xlate(const u32 *intspec, unsigned int intsize,
		unsigned long int_irq, unsigned long ext_irq,
		unsigned long *out_hwirq, unsigned int *out_type)
{
	if (WARN_ON(intsize < 1 || intsize > 2))
		return -EINVAL;
	if (intsize == 2 && intspec[1] == 1) {
		int_irq = xtensa_map_ext_irq(ext_irq);
		if (int_irq < XCHAL_NUM_INTERRUPTS)
			*out_hwirq = int_irq;
		else
			return -EINVAL;
	} else {
		*out_hwirq = int_irq;
	}
	*out_type = IRQ_TYPE_NONE;
	return 0;
}

int xtensa_irq_map(struct irq_domain *d, unsigned int irq,
		irq_hw_number_t hw)
{
	struct irq_chip *irq_chip = d->host_data;
	u32 mask = 1 << hw;

	if (mask & XCHAL_INTTYPE_MASK_SOFTWARE) {
		irq_set_chip_and_handler_name(irq, irq_chip,
				handle_simple_irq, "level");
		irq_set_status_flags(irq, IRQ_LEVEL);
	} else if (mask & XCHAL_INTTYPE_MASK_EXTERN_EDGE) {
		irq_set_chip_and_handler_name(irq, irq_chip,
				handle_edge_irq, "edge");
		irq_clear_status_flags(irq, IRQ_LEVEL);
	} else if (mask & XCHAL_INTTYPE_MASK_EXTERN_LEVEL) {
		irq_set_chip_and_handler_name(irq, irq_chip,
				handle_level_irq, "level");
		irq_set_status_flags(irq, IRQ_LEVEL);
	} else if (mask & XCHAL_INTTYPE_MASK_TIMER) {
		irq_set_chip_and_handler_name(irq, irq_chip,
				handle_percpu_irq, "timer");
		irq_clear_status_flags(irq, IRQ_LEVEL);
#ifdef XCHAL_INTTYPE_MASK_PROFILING
	} else if (mask & XCHAL_INTTYPE_MASK_PROFILING) {
		irq_set_chip_and_handler_name(irq, irq_chip,
				handle_percpu_irq, "profiling");
		irq_set_status_flags(irq, IRQ_LEVEL);
#endif
	} else {/* XCHAL_INTTYPE_MASK_WRITE_ERROR */
		/* XCHAL_INTTYPE_MASK_NMI */
		irq_set_chip_and_handler_name(irq, irq_chip,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				handle_level_irq, "level");
		irq_set_status_flags(irq, IRQ_LEVEL);
	}
	return 0;
}

<<<<<<< HEAD
static unsigned map_ext_irq(unsigned ext_irq)
=======
unsigned xtensa_map_ext_irq(unsigned ext_irq)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned mask = XCHAL_INTTYPE_MASK_EXTERN_EDGE |
		XCHAL_INTTYPE_MASK_EXTERN_LEVEL;
	unsigned i;

	for (i = 0; mask; ++i, mask >>= 1) {
		if ((mask & 1) && ext_irq-- == 0)
			return i;
	}
	return XCHAL_NUM_INTERRUPTS;
}

<<<<<<< HEAD
/*
 * Device Tree IRQ specifier translation function which works with one or
 * two cell bindings. First cell value maps directly to the hwirq number.
 * Second cell if present specifies whether hwirq number is external (1) or
 * internal (0).
 */
int xtensa_irq_domain_xlate(struct irq_domain *d, struct device_node *ctrlr,
		const u32 *intspec, unsigned int intsize,
		unsigned long *out_hwirq, unsigned int *out_type)
{
	if (WARN_ON(intsize < 1 || intsize > 2))
		return -EINVAL;
	if (intsize == 2 && intspec[1] == 1) {
		unsigned int_irq = map_ext_irq(intspec[0]);
		if (int_irq < XCHAL_NUM_INTERRUPTS)
			*out_hwirq = int_irq;
		else
			return -EINVAL;
	} else {
		*out_hwirq = intspec[0];
	}
	*out_type = IRQ_TYPE_NONE;
	return 0;
}

static const struct irq_domain_ops xtensa_irq_domain_ops = {
	.xlate = xtensa_irq_domain_xlate,
	.map = xtensa_irq_map,
};

void __init init_IRQ(void)
{
	struct device_node *intc = NULL;

	cached_irq_mask = 0;
	set_sr(~0, intclear);

#ifdef CONFIG_OF
	/* The interrupt controller device node is mandatory */
	intc = of_find_compatible_node(NULL, NULL, "xtensa,pic");
	BUG_ON(!intc);

	root_domain = irq_domain_add_linear(intc, NR_IRQS,
			&xtensa_irq_domain_ops, NULL);
#else
	root_domain = irq_domain_add_legacy(intc, NR_IRQS, 0, 0,
			&xtensa_irq_domain_ops, NULL);
#endif
	irq_set_default_host(root_domain);

	variant_init_irq();
}
=======
unsigned xtensa_get_ext_irq_no(unsigned irq)
{
	unsigned mask = (XCHAL_INTTYPE_MASK_EXTERN_EDGE |
		XCHAL_INTTYPE_MASK_EXTERN_LEVEL) &
		((1u << irq) - 1);
	return hweight32(mask);
}

void __init init_IRQ(void)
{
#ifdef CONFIG_OF
	irqchip_init();
#else
#ifdef CONFIG_HAVE_SMP
	xtensa_mx_init_legacy(NULL);
#else
	xtensa_pic_init_legacy(NULL);
#endif
#endif

#ifdef CONFIG_SMP
	ipi_init();
#endif
	variant_init_irq();
}

#ifdef CONFIG_HOTPLUG_CPU
/*
 * The CPU has been marked offline.  Migrate IRQs off this CPU.  If
 * the affinity settings do not allow other CPUs, force them onto any
 * available CPU.
 */
void migrate_irqs(void)
{
	unsigned int i, cpu = smp_processor_id();

	for_each_active_irq(i) {
		struct irq_data *data = irq_get_irq_data(i);
		struct cpumask *mask;
		unsigned int newcpu;

		if (irqd_is_per_cpu(data))
			continue;

		mask = irq_data_get_affinity_mask(data);
		if (!cpumask_test_cpu(cpu, mask))
			continue;

		newcpu = cpumask_any_and(mask, cpu_online_mask);

		if (newcpu >= nr_cpu_ids) {
			pr_info_ratelimited("IRQ%u no longer affine to CPU%u\n",
					    i, cpu);

			cpumask_setall(mask);
		}
		irq_set_affinity(i, mask);
	}
}
#endif /* CONFIG_HOTPLUG_CPU */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
