/*
<<<<<<< HEAD
 * Copyright (C) 2010 Red Hat, Inc., Peter Zijlstra <pzijlstr@redhat.com>
=======
 * Copyright (C) 2010 Red Hat, Inc., Peter Zijlstra
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Provides a framework for enqueueing and running callbacks from hardirq
 * context. The enqueueing is NMI-safe.
 */

#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/irq_work.h>
#include <linux/percpu.h>
#include <linux/hardirq.h>
#include <linux/irqflags.h>
#include <linux/sched.h>
#include <linux/tick.h>
#include <linux/cpu.h>
#include <linux/notifier.h>
<<<<<<< HEAD
#include <asm/processor.h>


static DEFINE_PER_CPU(struct llist_head, irq_work_list);
static DEFINE_PER_CPU(int, irq_work_raised);
=======
#include <linux/smp.h>
#include <asm/processor.h>


static DEFINE_PER_CPU(struct llist_head, raised_list);
static DEFINE_PER_CPU(struct llist_head, lazy_list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Claim the entry so that no one else will poke at it.
 */
static bool irq_work_claim(struct irq_work *work)
{
	unsigned long flags, oflags, nflags;

	/*
	 * Start with our best wish as a premise but only trust any
	 * flag value after cmpxchg() result.
	 */
	flags = work->flags & ~IRQ_WORK_PENDING;
	for (;;) {
		nflags = flags | IRQ_WORK_FLAGS;
		oflags = cmpxchg(&work->flags, flags, nflags);
		if (oflags == flags)
			break;
		if (oflags & IRQ_WORK_PENDING)
			return false;
		flags = oflags;
		cpu_relax();
	}

	return true;
}

void __weak arch_irq_work_raise(void)
{
	/*
	 * Lame architectures will get the timer tick callback
	 */
}

<<<<<<< HEAD
/*
 * Enqueue the irq_work @entry unless it's already pending
=======
#ifdef CONFIG_SMP
/*
 * Enqueue the irq_work @work on @cpu unless it's already pending
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * somewhere.
 *
 * Can be re-enqueued while the callback is still in progress.
 */
<<<<<<< HEAD
void irq_work_queue(struct irq_work *work)
{
	/* Only queue if not already pending */
	if (!irq_work_claim(work))
		return;
=======
bool irq_work_queue_on(struct irq_work *work, int cpu)
{
	/* All work should have been flushed before going offline */
	WARN_ON_ONCE(cpu_is_offline(cpu));

	/* Arch remote IPI send/receive backend aren't NMI safe */
	WARN_ON_ONCE(in_nmi());

	/* Only queue if not already pending */
	if (!irq_work_claim(work))
		return false;

	if (llist_add(&work->llnode, &per_cpu(raised_list, cpu)))
		arch_send_call_function_single_ipi(cpu);

	return true;
}
EXPORT_SYMBOL_GPL(irq_work_queue_on);
#endif

/* Enqueue the irq work @work on the current CPU */
bool irq_work_queue(struct irq_work *work)
{
	/* Only queue if not already pending */
	if (!irq_work_claim(work))
		return false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Queue the entry and raise the IPI if needed. */
	preempt_disable();

<<<<<<< HEAD
	llist_add(&work->llnode, &__get_cpu_var(irq_work_list));

	/*
	 * If the work is not "lazy" or the tick is stopped, raise the irq
	 * work interrupt (if supported by the arch), otherwise, just wait
	 * for the next tick.
	 */
	if (!(work->flags & IRQ_WORK_LAZY) || tick_nohz_tick_stopped()) {
		if (!this_cpu_cmpxchg(irq_work_raised, 0, 1))
=======
	/* If the work is "lazy", handle it from next tick if any */
	if (work->flags & IRQ_WORK_LAZY) {
		if (llist_add(&work->llnode, this_cpu_ptr(&lazy_list)) &&
		    tick_nohz_tick_stopped())
			arch_irq_work_raise();
	} else {
		if (llist_add(&work->llnode, this_cpu_ptr(&raised_list)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			arch_irq_work_raise();
	}

	preempt_enable();
<<<<<<< HEAD
=======

	return true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(irq_work_queue);

bool irq_work_needs_cpu(void)
{
<<<<<<< HEAD
	struct llist_head *this_list;

	this_list = &__get_cpu_var(irq_work_list);
	if (llist_empty(this_list))
		return false;
=======
	struct llist_head *raised, *lazy;

	raised = this_cpu_ptr(&raised_list);
	lazy = this_cpu_ptr(&lazy_list);

	if (llist_empty(raised) || arch_irq_work_has_interrupt())
		if (llist_empty(lazy))
			return false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* All work should have been flushed before going offline */
	WARN_ON_ONCE(cpu_is_offline(smp_processor_id()));

	return true;
}

<<<<<<< HEAD
static void __irq_work_run(void)
{
	unsigned long flags;
	struct irq_work *work;
	struct llist_head *this_list;
	struct llist_node *llnode;


	/*
	 * Reset the "raised" state right before we check the list because
	 * an NMI may enqueue after we find the list empty from the runner.
	 */
	__this_cpu_write(irq_work_raised, 0);
	barrier();

	this_list = &__get_cpu_var(irq_work_list);
	if (llist_empty(this_list))
		return;

	BUG_ON(!irqs_disabled());

	llnode = llist_del_all(this_list);
=======
static void irq_work_run_list(struct llist_head *list)
{
	unsigned long flags;
	struct irq_work *work;
	struct llist_node *llnode;

	BUG_ON(!irqs_disabled());

	if (llist_empty(list))
		return;

	llnode = llist_del_all(list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	while (llnode != NULL) {
		work = llist_entry(llnode, struct irq_work, llnode);

		llnode = llist_next(llnode);

		/*
		 * Clear the PENDING bit, after this point the @work
		 * can be re-used.
		 * Make it immediately visible so that other CPUs trying
		 * to claim that work don't rely on us to handle their data
		 * while we are in the middle of the func.
		 */
		flags = work->flags & ~IRQ_WORK_PENDING;
		xchg(&work->flags, flags);

		work->func(work);
		/*
		 * Clear the BUSY bit and return to the free state if
		 * no-one else claimed it meanwhile.
		 */
		(void)cmpxchg(&work->flags, flags, flags & ~IRQ_WORK_BUSY);
	}
}

/*
<<<<<<< HEAD
 * Run the irq_work entries on this cpu. Requires to be ran from hardirq
 * context with local IRQs disabled.
 */
void irq_work_run(void)
{
	BUG_ON(!in_irq());
	__irq_work_run();
}
EXPORT_SYMBOL_GPL(irq_work_run);

=======
 * hotplug calls this through:
 *  hotplug_cfd() -> flush_smp_call_function_queue()
 */
void irq_work_run(void)
{
	irq_work_run_list(this_cpu_ptr(&raised_list));
	irq_work_run_list(this_cpu_ptr(&lazy_list));
}
EXPORT_SYMBOL_GPL(irq_work_run);

void irq_work_tick(void)
{
	struct llist_head *raised = this_cpu_ptr(&raised_list);

	if (!llist_empty(raised) && !arch_irq_work_has_interrupt())
		irq_work_run_list(raised);
	irq_work_run_list(this_cpu_ptr(&lazy_list));
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Synchronize against the irq_work @entry, ensures the entry is not
 * currently in use.
 */
void irq_work_sync(struct irq_work *work)
{
	WARN_ON_ONCE(irqs_disabled());

	while (work->flags & IRQ_WORK_BUSY)
		cpu_relax();
}
EXPORT_SYMBOL_GPL(irq_work_sync);
<<<<<<< HEAD

#ifdef CONFIG_HOTPLUG_CPU
static int irq_work_cpu_notify(struct notifier_block *self,
			       unsigned long action, void *hcpu)
{
	long cpu = (long)hcpu;

	switch (action) {
	case CPU_DYING:
		/* Called from stop_machine */
		if (WARN_ON_ONCE(cpu != smp_processor_id()))
			break;
		__irq_work_run();
		break;
	default:
		break;
	}
	return NOTIFY_OK;
}

static struct notifier_block cpu_notify;

static __init int irq_work_init_cpu_notifier(void)
{
	cpu_notify.notifier_call = irq_work_cpu_notify;
	cpu_notify.priority = 0;
	register_cpu_notifier(&cpu_notify);
	return 0;
}
device_initcall(irq_work_init_cpu_notifier);

#endif /* CONFIG_HOTPLUG_CPU */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
