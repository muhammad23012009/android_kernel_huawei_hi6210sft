/*
 * lib/smp_processor_id.c
 *
 * DEBUG_PREEMPT variant of smp_processor_id().
 */
#include <linux/export.h>
#include <linux/kallsyms.h>
#include <linux/sched.h>

<<<<<<< HEAD
notrace unsigned int debug_smp_processor_id(void)
{
	unsigned long preempt_count = preempt_count();
	int this_cpu = raw_smp_processor_id();

	if (likely(preempt_count))
=======
notrace static unsigned int check_preemption_disabled(const char *what1,
							const char *what2)
{
	int this_cpu = raw_smp_processor_id();

	if (likely(preempt_count()))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto out;

	if (irqs_disabled())
		goto out;

	/*
	 * Kernel threads bound to a single CPU can safely use
	 * smp_processor_id():
	 */
	if (cpumask_equal(tsk_cpus_allowed(current), cpumask_of(this_cpu)))
		goto out;

	/*
	 * It is valid to assume CPU-locality during early bootup:
	 */
	if (system_state != SYSTEM_RUNNING)
		goto out;

	/*
	 * Avoid recursion:
	 */
	preempt_disable_notrace();

	if (!printk_ratelimit())
		goto out_enable;

<<<<<<< HEAD
	printk(KERN_ERR "BUG: using smp_processor_id() in preemptible [%08x] "
			"code: %s/%d\n",
			preempt_count() - 1, current->comm, current->pid);
=======
	printk(KERN_ERR "BUG: using %s%s() in preemptible [%08x] code: %s/%d\n",
		what1, what2, preempt_count() - 1, current->comm, current->pid);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	print_symbol("caller is %s\n", (long)__builtin_return_address(0));
	dump_stack();

out_enable:
	preempt_enable_no_resched_notrace();
out:
	return this_cpu;
}

<<<<<<< HEAD
EXPORT_SYMBOL(debug_smp_processor_id);

=======
notrace unsigned int debug_smp_processor_id(void)
{
	return check_preemption_disabled("smp_processor_id", "");
}
EXPORT_SYMBOL(debug_smp_processor_id);

notrace void __this_cpu_preempt_check(const char *op)
{
	check_preemption_disabled("__this_cpu_", op);
}
EXPORT_SYMBOL(__this_cpu_preempt_check);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
