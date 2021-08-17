/*
 *  linux/drivers/cpufreq/cpufreq.c
 *
 *  Copyright (C) 2001 Russell King
 *            (C) 2002 - 2003 Dominik Brodowski <linux@brodo.de>
<<<<<<< HEAD
=======
 *            (C) 2013 Viresh Kumar <viresh.kumar@linaro.org>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 *  Oct 2005 - Ashok Raj <ashok.raj@intel.com>
 *	Added handling for CPU hotplug
 *  Feb 2006 - Jacob Shin <jacob.shin@amd.com>
 *	Fix handling for CPU hotplug -- affected CPUs
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
<<<<<<< HEAD
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

<<<<<<< HEAD
#include <asm/cputime.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/tick.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/cpu.h>
#include <linux/completion.h>
#include <linux/mutex.h>
#include <linux/syscore_ops.h>
#include <linux/suspend.h>
#include <linux/tick.h>

#include <trace/events/power.h>

#ifdef CONFIG_HUAWEI_MSG_POLICY
#include <huawei_platform/power/msgnotify.h>
#endif
=======
#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/suspend.h>
#include <linux/syscore_ops.h>
#include <linux/tick.h>
#ifdef CONFIG_SMP
#include <linux/sched.h>
#endif
#include <trace/events/power.h>

static LIST_HEAD(cpufreq_policy_list);

static inline bool policy_is_inactive(struct cpufreq_policy *policy)
{
	return cpumask_empty(policy->cpus);
}

/* Macros to iterate over CPU policies */
#define for_each_suitable_policy(__policy, __active)			 \
	list_for_each_entry(__policy, &cpufreq_policy_list, policy_list) \
		if ((__active) == !policy_is_inactive(__policy))

#define for_each_active_policy(__policy)		\
	for_each_suitable_policy(__policy, true)
#define for_each_inactive_policy(__policy)		\
	for_each_suitable_policy(__policy, false)

#define for_each_policy(__policy)			\
	list_for_each_entry(__policy, &cpufreq_policy_list, policy_list)

/* Iterate over governors */
static LIST_HEAD(cpufreq_governor_list);
#define for_each_governor(__governor)				\
	list_for_each_entry(__governor, &cpufreq_governor_list, governor_list)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * The "cpufreq driver" - the arch- or hardware-dependent low
 * level driver of CPUFreq support, and its spinlock. This lock
 * also protects the cpufreq_cpu_data array.
 */
static struct cpufreq_driver *cpufreq_driver;
static DEFINE_PER_CPU(struct cpufreq_policy *, cpufreq_cpu_data);
<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU
/* This one keeps track of the previously set governor of a removed CPU */
static DEFINE_PER_CPU(char[CPUFREQ_NAME_LEN], cpufreq_cpu_governor);
#endif
static DEFINE_RWLOCK(cpufreq_driver_lock);
DEFINE_MUTEX(cpufreq_governor_lock);
=======
static DEFINE_RWLOCK(cpufreq_driver_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Flag to suspend/resume CPUFreq governors */
static bool cpufreq_suspended;

static inline bool has_target(void)
{
<<<<<<< HEAD
	return cpufreq_driver->target;
}

/*
 * cpu_policy_rwsem is a per CPU reader-writer semaphore designed to cure
 * all cpufreq/hotplug/workqueue/etc related lock issues.
 *
 * The rules for this semaphore:
 * - Any routine that wants to read from the policy structure will
 *   do a down_read on this semaphore.
 * - Any routine that will write to the policy structure and/or may take away
 *   the policy altogether (eg. CPU hotplug), will hold this lock in write
 *   mode before doing so.
 *
 * Additional rules:
 * - Governor routines that can be called in cpufreq hotplug path should not
 *   take this sem as top level hotplug notifier handler takes this.
 * - Lock should not be held across
 *     __cpufreq_governor(data, CPUFREQ_GOV_STOP);
 */
static DEFINE_PER_CPU(int, cpufreq_policy_cpu);
static DEFINE_PER_CPU(struct rw_semaphore, cpu_policy_rwsem);

#define lock_policy_rwsem(mode, cpu)					\
static int lock_policy_rwsem_##mode(int cpu)				\
{									\
	int policy_cpu = per_cpu(cpufreq_policy_cpu, cpu);		\
	BUG_ON(policy_cpu == -1);					\
	down_##mode(&per_cpu(cpu_policy_rwsem, policy_cpu));		\
									\
	return 0;							\
}

lock_policy_rwsem(read, cpu);
lock_policy_rwsem(write, cpu);

#define unlock_policy_rwsem(mode, cpu)					\
static void unlock_policy_rwsem_##mode(int cpu)				\
{									\
	int policy_cpu = per_cpu(cpufreq_policy_cpu, cpu);		\
	BUG_ON(policy_cpu == -1);					\
	up_##mode(&per_cpu(cpu_policy_rwsem, policy_cpu));		\
}

unlock_policy_rwsem(read, cpu);
unlock_policy_rwsem(write, cpu);

/* internal prototypes */
static int __cpufreq_governor(struct cpufreq_policy *policy,
		unsigned int event);
static unsigned int __cpufreq_get(unsigned int cpu);
static void handle_update(struct work_struct *work);
=======
	return cpufreq_driver->target_index || cpufreq_driver->target;
}

/* internal prototypes */
static unsigned int __cpufreq_get(struct cpufreq_policy *policy);
static int cpufreq_init_governor(struct cpufreq_policy *policy);
static void cpufreq_exit_governor(struct cpufreq_policy *policy);
static int cpufreq_start_governor(struct cpufreq_policy *policy);
static void cpufreq_stop_governor(struct cpufreq_policy *policy);
static void cpufreq_governor_limits(struct cpufreq_policy *policy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * Two notifier lists: the "policy" list is involved in the
 * validation process for a new CPU frequency policy; the
 * "transition" list for kernel code that needs to handle
 * changes to devices when the CPU clock speed changes.
 * The mutex locks both lists.
 */
static BLOCKING_NOTIFIER_HEAD(cpufreq_policy_notifier_list);
static struct srcu_notifier_head cpufreq_transition_notifier_list;

static bool init_cpufreq_transition_notifier_list_called;
static int __init init_cpufreq_transition_notifier_list(void)
{
	srcu_init_notifier_head(&cpufreq_transition_notifier_list);
	init_cpufreq_transition_notifier_list_called = true;
	return 0;
}
pure_initcall(init_cpufreq_transition_notifier_list);

static int off __read_mostly;
static int cpufreq_disabled(void)
{
	return off;
}
void disable_cpufreq(void)
{
	off = 1;
}
<<<<<<< HEAD
static LIST_HEAD(cpufreq_governor_list);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static DEFINE_MUTEX(cpufreq_governor_mutex);

bool have_governor_per_policy(void)
{
<<<<<<< HEAD
	return cpufreq_driver->have_governor_per_policy;
}
EXPORT_SYMBOL_GPL(have_governor_per_policy);

=======
	return !!(cpufreq_driver->flags & CPUFREQ_HAVE_GOVERNOR_PER_POLICY);
}
EXPORT_SYMBOL_GPL(have_governor_per_policy);

bool cpufreq_driver_is_slow(void)
{
	return !(cpufreq_driver->flags & CPUFREQ_DRIVER_FAST);
}
EXPORT_SYMBOL_GPL(cpufreq_driver_is_slow);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct kobject *get_governor_parent_kobj(struct cpufreq_policy *policy)
{
	if (have_governor_per_policy())
		return &policy->kobj;
	else
		return cpufreq_global_kobject;
}
EXPORT_SYMBOL_GPL(get_governor_parent_kobj);

static inline u64 get_cpu_idle_time_jiffy(unsigned int cpu, u64 *wall)
{
	u64 idle_time;
	u64 cur_wall_time;
	u64 busy_time;

<<<<<<< HEAD
	cur_wall_time = jiffies64_to_cputime64(get_jiffies_64());
=======
	cur_wall_time = jiffies64_to_nsecs(get_jiffies_64());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	busy_time = kcpustat_cpu(cpu).cpustat[CPUTIME_USER];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SYSTEM];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_IRQ];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SOFTIRQ];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_STEAL];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_NICE];

	idle_time = cur_wall_time - busy_time;
	if (wall)
<<<<<<< HEAD
		*wall = cputime_to_usecs(cur_wall_time);

	return cputime_to_usecs(idle_time);
=======
		*wall = div_u64(cur_wall_time, NSEC_PER_USEC);

	return div_u64(idle_time, NSEC_PER_USEC);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

u64 get_cpu_idle_time(unsigned int cpu, u64 *wall, int io_busy)
{
	u64 idle_time = get_cpu_idle_time_us(cpu, io_busy ? wall : NULL);

	if (idle_time == -1ULL)
		return get_cpu_idle_time_jiffy(cpu, wall);
	else if (!io_busy)
		idle_time += get_cpu_iowait_time_us(cpu, wall);

	return idle_time;
}
EXPORT_SYMBOL_GPL(get_cpu_idle_time);

<<<<<<< HEAD
static struct cpufreq_policy *__cpufreq_cpu_get(unsigned int cpu, bool sysfs)
{
	struct cpufreq_policy *data;
	unsigned long flags;

	if (cpu >= nr_cpu_ids)
		goto err_out;

	/* get the cpufreq driver */
	read_lock_irqsave(&cpufreq_driver_lock, flags);

	if (!cpufreq_driver)
		goto err_out_unlock;

	if (!try_module_get(cpufreq_driver->owner))
		goto err_out_unlock;


	/* get the CPU */
	data = per_cpu(cpufreq_cpu_data, cpu);

	if (!data)
		goto err_out_put_module;

	if (!sysfs && !kobject_get(&data->kobj))
		goto err_out_put_module;

	read_unlock_irqrestore(&cpufreq_driver_lock, flags);
	return data;

err_out_put_module:
	module_put(cpufreq_driver->owner);
err_out_unlock:
	read_unlock_irqrestore(&cpufreq_driver_lock, flags);
err_out:
	return NULL;
}

struct cpufreq_policy *cpufreq_cpu_get(unsigned int cpu)
{
	if (cpufreq_disabled())
		return NULL;

	return __cpufreq_cpu_get(cpu, false);
}
EXPORT_SYMBOL_GPL(cpufreq_cpu_get);

static struct cpufreq_policy *cpufreq_cpu_get_sysfs(unsigned int cpu)
{
	return __cpufreq_cpu_get(cpu, true);
}

static void __cpufreq_cpu_put(struct cpufreq_policy *data, bool sysfs)
{
	if (!sysfs)
		kobject_put(&data->kobj);
	module_put(cpufreq_driver->owner);
}

void cpufreq_cpu_put(struct cpufreq_policy *data)
{
	if (cpufreq_disabled())
		return;

	__cpufreq_cpu_put(data, false);
}
EXPORT_SYMBOL_GPL(cpufreq_cpu_put);

static void cpufreq_cpu_put_sysfs(struct cpufreq_policy *data)
{
	__cpufreq_cpu_put(data, true);
}
=======
/*
 * This is a generic cpufreq init() routine which can be used by cpufreq
 * drivers of SMP systems. It will do following:
 * - validate & show freq table passed
 * - set policies transition latency
 * - policy->cpus with all possible CPUs
 */
int cpufreq_generic_init(struct cpufreq_policy *policy,
		struct cpufreq_frequency_table *table,
		unsigned int transition_latency)
{
	int ret;

	ret = cpufreq_table_validate_and_show(policy, table);
	if (ret) {
		pr_err("%s: invalid frequency table: %d\n", __func__, ret);
		return ret;
	}

	policy->cpuinfo.transition_latency = transition_latency;

	/*
	 * The driver only supports the SMP configuration where all processors
	 * share the clock and voltage and clock.
	 */
	cpumask_setall(policy->cpus);

	return 0;
}
EXPORT_SYMBOL_GPL(cpufreq_generic_init);

struct cpufreq_policy *cpufreq_cpu_get_raw(unsigned int cpu)
{
	struct cpufreq_policy *policy = per_cpu(cpufreq_cpu_data, cpu);

	return policy && cpumask_test_cpu(cpu, policy->cpus) ? policy : NULL;
}
EXPORT_SYMBOL_GPL(cpufreq_cpu_get_raw);

unsigned int cpufreq_generic_get(unsigned int cpu)
{
	struct cpufreq_policy *policy = cpufreq_cpu_get_raw(cpu);

	if (!policy || IS_ERR(policy->clk)) {
		pr_err("%s: No %s associated to cpu: %d\n",
		       __func__, policy ? "clk" : "policy", cpu);
		return 0;
	}

	return clk_get_rate(policy->clk) / 1000;
}
EXPORT_SYMBOL_GPL(cpufreq_generic_get);

/**
 * cpufreq_cpu_get: returns policy for a cpu and marks it busy.
 *
 * @cpu: cpu to find policy for.
 *
 * This returns policy for 'cpu', returns NULL if it doesn't exist.
 * It also increments the kobject reference count to mark it busy and so would
 * require a corresponding call to cpufreq_cpu_put() to decrement it back.
 * If corresponding call cpufreq_cpu_put() isn't made, the policy wouldn't be
 * freed as that depends on the kobj count.
 *
 * Return: A valid policy on success, otherwise NULL on failure.
 */
struct cpufreq_policy *cpufreq_cpu_get(unsigned int cpu)
{
	struct cpufreq_policy *policy = NULL;
	unsigned long flags;

	if (WARN_ON(cpu >= nr_cpu_ids))
		return NULL;

	/* get the cpufreq driver */
	read_lock_irqsave(&cpufreq_driver_lock, flags);

	if (cpufreq_driver) {
		/* get the CPU */
		policy = cpufreq_cpu_get_raw(cpu);
		if (policy)
			kobject_get(&policy->kobj);
	}

	read_unlock_irqrestore(&cpufreq_driver_lock, flags);

	return policy;
}
EXPORT_SYMBOL_GPL(cpufreq_cpu_get);

/**
 * cpufreq_cpu_put: Decrements the usage count of a policy
 *
 * @policy: policy earlier returned by cpufreq_cpu_get().
 *
 * This decrements the kobject reference count incremented earlier by calling
 * cpufreq_cpu_get().
 */
void cpufreq_cpu_put(struct cpufreq_policy *policy)
{
	kobject_put(&policy->kobj);
}
EXPORT_SYMBOL_GPL(cpufreq_cpu_put);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*********************************************************************
 *            EXTERNALLY AFFECTING FREQUENCY CHANGES                 *
 *********************************************************************/

/**
 * adjust_jiffies - adjust the system "loops_per_jiffy"
 *
 * This function alters the system "loops_per_jiffy" for the clock
 * speed change. Note that loops_per_jiffy cannot be updated on SMP
 * systems as each CPU might be scaled differently. So, use the arch
 * per-CPU loops_per_jiffy value wherever possible.
 */
<<<<<<< HEAD
#ifndef CONFIG_SMP
static unsigned long l_p_j_ref;
static unsigned int  l_p_j_ref_freq;

static void adjust_jiffies(unsigned long val, struct cpufreq_freqs *ci)
{
=======
static void adjust_jiffies(unsigned long val, struct cpufreq_freqs *ci)
{
#ifndef CONFIG_SMP
	static unsigned long l_p_j_ref;
	static unsigned int l_p_j_ref_freq;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ci->flags & CPUFREQ_CONST_LOOPS)
		return;

	if (!l_p_j_ref_freq) {
		l_p_j_ref = loops_per_jiffy;
		l_p_j_ref_freq = ci->old;
<<<<<<< HEAD
		pr_debug("saving %lu as reference value for loops_per_jiffy; "
			"freq is %u kHz\n", l_p_j_ref, l_p_j_ref_freq);
	}
	if ((val == CPUFREQ_POSTCHANGE  && ci->old != ci->new) ||
	    (val == CPUFREQ_RESUMECHANGE || val == CPUFREQ_SUSPENDCHANGE)) {
		loops_per_jiffy = cpufreq_scale(l_p_j_ref, l_p_j_ref_freq,
								ci->new);
		pr_debug("scaling loops_per_jiffy to %lu "
			"for frequency %u kHz\n", loops_per_jiffy, ci->new);
	}
}
#else
static inline void adjust_jiffies(unsigned long val, struct cpufreq_freqs *ci)
{
	return;
}
#endif


void __cpufreq_notify_transition(struct cpufreq_policy *policy,
=======
		pr_debug("saving %lu as reference value for loops_per_jiffy; freq is %u kHz\n",
			 l_p_j_ref, l_p_j_ref_freq);
	}
	if (val == CPUFREQ_POSTCHANGE && ci->old != ci->new) {
		loops_per_jiffy = cpufreq_scale(l_p_j_ref, l_p_j_ref_freq,
								ci->new);
		pr_debug("scaling loops_per_jiffy to %lu for frequency %u kHz\n",
			 loops_per_jiffy, ci->new);
	}
#endif
}

/*********************************************************************
 *               FREQUENCY INVARIANT CPU CAPACITY                    *
 *********************************************************************/

static DEFINE_PER_CPU(unsigned long, freq_scale) = SCHED_CAPACITY_SCALE;
static DEFINE_PER_CPU(unsigned long, max_freq_scale) = SCHED_CAPACITY_SCALE;

static void
scale_freq_capacity(struct cpufreq_policy *policy, struct cpufreq_freqs *freqs)
{
	unsigned long cur = freqs ? freqs->new : policy->cur;
	unsigned long scale = (cur << SCHED_CAPACITY_SHIFT) / policy->max;
	struct cpufreq_cpuinfo *cpuinfo = &policy->cpuinfo;
	int cpu;

	pr_debug("cpus %*pbl cur/cur max freq %lu/%u kHz freq scale %lu\n",
		 cpumask_pr_args(policy->cpus), cur, policy->max, scale);

	for_each_cpu(cpu, policy->cpus)
		per_cpu(freq_scale, cpu) = scale;

	if (freqs)
		return;

	scale = (policy->max << SCHED_CAPACITY_SHIFT) / cpuinfo->max_freq;

	pr_debug("cpus %*pbl cur max/max freq %u/%u kHz max freq scale %lu\n",
		 cpumask_pr_args(policy->cpus), policy->max, cpuinfo->max_freq,
		 scale);

	for_each_cpu(cpu, policy->cpus)
		per_cpu(max_freq_scale, cpu) = scale;
}

unsigned long cpufreq_scale_freq_capacity(struct sched_domain *sd, int cpu)
{
	return per_cpu(freq_scale, cpu);
}

unsigned long cpufreq_scale_max_freq_capacity(int cpu)
{
	return per_cpu(max_freq_scale, cpu);
}

static void __cpufreq_notify_transition(struct cpufreq_policy *policy,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		struct cpufreq_freqs *freqs, unsigned int state)
{
	BUG_ON(irqs_disabled());

	if (cpufreq_disabled())
		return;

	freqs->flags = cpufreq_driver->flags;
	pr_debug("notification %u of frequency transition to %u kHz\n",
<<<<<<< HEAD
		state, freqs->new);
=======
		 state, freqs->new);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	switch (state) {

	case CPUFREQ_PRECHANGE:
		/* detect if the driver reported a value as "old frequency"
		 * which is not equal to what the cpufreq core thinks is
		 * "old frequency".
		 */
		if (!(cpufreq_driver->flags & CPUFREQ_CONST_LOOPS)) {
			if ((policy) && (policy->cpu == freqs->cpu) &&
			    (policy->cur) && (policy->cur != freqs->old)) {
<<<<<<< HEAD
				pr_debug("Warning: CPU frequency is"
					" %u, cpufreq assumed %u kHz.\n",
					freqs->old, policy->cur);
=======
				pr_debug("Warning: CPU frequency is %u, cpufreq assumed %u kHz\n",
					 freqs->old, policy->cur);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				freqs->old = policy->cur;
			}
		}
		srcu_notifier_call_chain(&cpufreq_transition_notifier_list,
				CPUFREQ_PRECHANGE, freqs);
		adjust_jiffies(CPUFREQ_PRECHANGE, freqs);
		break;

	case CPUFREQ_POSTCHANGE:
		adjust_jiffies(CPUFREQ_POSTCHANGE, freqs);
<<<<<<< HEAD
		pr_debug("FREQ: %lu - CPU: %lu", (unsigned long)freqs->new,
			(unsigned long)freqs->cpu);
		trace_cpu_frequency(freqs->new, freqs->cpu);
=======
		pr_debug("FREQ: %lu - CPU: %lu\n",
			 (unsigned long)freqs->new, (unsigned long)freqs->cpu);
		trace_cpu_frequency(freqs->new, freqs->cpu);
		cpufreq_stats_record_transition(policy, freqs->new);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		srcu_notifier_call_chain(&cpufreq_transition_notifier_list,
				CPUFREQ_POSTCHANGE, freqs);
		if (likely(policy) && likely(policy->cpu == freqs->cpu))
			policy->cur = freqs->new;
		break;
	}
}
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * cpufreq_notify_transition - call notifier chain and adjust_jiffies
 * on frequency transition.
 *
 * This function calls the transition notifiers and the "adjust_jiffies"
 * function. It is called twice on all CPU frequency changes that have
 * external effects.
 */
<<<<<<< HEAD
void cpufreq_notify_transition(struct cpufreq_policy *policy,
		struct cpufreq_freqs *freqs, unsigned int state)
{
	if (state == CPUFREQ_POSTCHANGE) {
		trace_cpufreq(policy->cpus, freqs->old,  freqs->new);
	}
	for_each_cpu(freqs->cpu, policy->cpus)
		__cpufreq_notify_transition(policy, freqs, state);
}
EXPORT_SYMBOL_GPL(cpufreq_notify_transition);


=======
static void cpufreq_notify_transition(struct cpufreq_policy *policy,
		struct cpufreq_freqs *freqs, unsigned int state)
{
	for_each_cpu(freqs->cpu, policy->cpus)
		__cpufreq_notify_transition(policy, freqs, state);
}

/* Do post notifications when there are chances that transition has failed */
static void cpufreq_notify_post_transition(struct cpufreq_policy *policy,
		struct cpufreq_freqs *freqs, int transition_failed)
{
	cpufreq_notify_transition(policy, freqs, CPUFREQ_POSTCHANGE);
	if (!transition_failed)
		return;

	swap(freqs->old, freqs->new);
	cpufreq_notify_transition(policy, freqs, CPUFREQ_PRECHANGE);
	cpufreq_notify_transition(policy, freqs, CPUFREQ_POSTCHANGE);
}

void cpufreq_freq_transition_begin(struct cpufreq_policy *policy,
		struct cpufreq_freqs *freqs)
{
#ifdef CONFIG_SMP
	int cpu;
#endif

	/*
	 * Catch double invocations of _begin() which lead to self-deadlock.
	 * ASYNC_NOTIFICATION drivers are left out because the cpufreq core
	 * doesn't invoke _begin() on their behalf, and hence the chances of
	 * double invocations are very low. Moreover, there are scenarios
	 * where these checks can emit false-positive warnings in these
	 * drivers; so we avoid that by skipping them altogether.
	 */
	WARN_ON(!(cpufreq_driver->flags & CPUFREQ_ASYNC_NOTIFICATION)
				&& current == policy->transition_task);

wait:
	wait_event(policy->transition_wait, !policy->transition_ongoing);

	spin_lock(&policy->transition_lock);

	if (unlikely(policy->transition_ongoing)) {
		spin_unlock(&policy->transition_lock);
		goto wait;
	}

	policy->transition_ongoing = true;
	policy->transition_task = current;

	spin_unlock(&policy->transition_lock);

	scale_freq_capacity(policy, freqs);
#ifdef CONFIG_SMP
	for_each_cpu(cpu, policy->cpus)
		trace_cpu_capacity(capacity_curr_of(cpu), cpu);
#endif

	cpufreq_notify_transition(policy, freqs, CPUFREQ_PRECHANGE);
}
EXPORT_SYMBOL_GPL(cpufreq_freq_transition_begin);

void cpufreq_freq_transition_end(struct cpufreq_policy *policy,
		struct cpufreq_freqs *freqs, int transition_failed)
{
	if (unlikely(WARN_ON(!policy->transition_ongoing)))
		return;

	cpufreq_notify_post_transition(policy, freqs, transition_failed);

	policy->transition_ongoing = false;
	policy->transition_task = NULL;

	wake_up(&policy->transition_wait);
}
EXPORT_SYMBOL_GPL(cpufreq_freq_transition_end);

/*
 * Fast frequency switching status count.  Positive means "enabled", negative
 * means "disabled" and 0 means "not decided yet".
 */
static int cpufreq_fast_switch_count;
static DEFINE_MUTEX(cpufreq_fast_switch_lock);

static void cpufreq_list_transition_notifiers(void)
{
	struct notifier_block *nb;

	pr_info("Registered transition notifiers:\n");

	mutex_lock(&cpufreq_transition_notifier_list.mutex);

	for (nb = cpufreq_transition_notifier_list.head; nb; nb = nb->next)
		pr_info("%pF\n", nb->notifier_call);

	mutex_unlock(&cpufreq_transition_notifier_list.mutex);
}

/**
 * cpufreq_enable_fast_switch - Enable fast frequency switching for policy.
 * @policy: cpufreq policy to enable fast frequency switching for.
 *
 * Try to enable fast frequency switching for @policy.
 *
 * The attempt will fail if there is at least one transition notifier registered
 * at this point, as fast frequency switching is quite fundamentally at odds
 * with transition notifiers.  Thus if successful, it will make registration of
 * transition notifiers fail going forward.
 */
void cpufreq_enable_fast_switch(struct cpufreq_policy *policy)
{
	lockdep_assert_held(&policy->rwsem);

	if (!policy->fast_switch_possible)
		return;

	mutex_lock(&cpufreq_fast_switch_lock);
	if (cpufreq_fast_switch_count >= 0) {
		cpufreq_fast_switch_count++;
		policy->fast_switch_enabled = true;
	} else {
		pr_warn("CPU%u: Fast frequency switching not enabled\n",
			policy->cpu);
		cpufreq_list_transition_notifiers();
	}
	mutex_unlock(&cpufreq_fast_switch_lock);
}
EXPORT_SYMBOL_GPL(cpufreq_enable_fast_switch);

/**
 * cpufreq_disable_fast_switch - Disable fast frequency switching for policy.
 * @policy: cpufreq policy to disable fast frequency switching for.
 */
void cpufreq_disable_fast_switch(struct cpufreq_policy *policy)
{
	mutex_lock(&cpufreq_fast_switch_lock);
	if (policy->fast_switch_enabled) {
		policy->fast_switch_enabled = false;
		if (!WARN_ON(cpufreq_fast_switch_count <= 0))
			cpufreq_fast_switch_count--;
	}
	mutex_unlock(&cpufreq_fast_switch_lock);
}
EXPORT_SYMBOL_GPL(cpufreq_disable_fast_switch);

/**
 * cpufreq_driver_resolve_freq - Map a target frequency to a driver-supported
 * one.
 * @target_freq: target frequency to resolve.
 *
 * The target to driver frequency mapping is cached in the policy.
 *
 * Return: Lowest driver-supported frequency greater than or equal to the
 * given target_freq, subject to policy (min/max) and driver limitations.
 */
unsigned int cpufreq_driver_resolve_freq(struct cpufreq_policy *policy,
					 unsigned int target_freq)
{
	target_freq = clamp_val(target_freq, policy->min, policy->max);
	policy->cached_target_freq = target_freq;

	if (cpufreq_driver->target_index) {
		int idx;

		idx = cpufreq_frequency_table_target(policy, target_freq,
						     CPUFREQ_RELATION_L);
		policy->cached_resolved_idx = idx;
		return policy->freq_table[idx].frequency;
	}

	if (cpufreq_driver->resolve_freq)
		return cpufreq_driver->resolve_freq(policy, target_freq);

	return target_freq;
}
EXPORT_SYMBOL_GPL(cpufreq_driver_resolve_freq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*********************************************************************
 *                          SYSFS INTERFACE                          *
 *********************************************************************/
<<<<<<< HEAD

static struct cpufreq_governor *__find_governor(const char *str_governor)
{
	struct cpufreq_governor *t;

	list_for_each_entry(t, &cpufreq_governor_list, governor_list)
		if (!strnicmp(str_governor, t->name, CPUFREQ_NAME_LEN))
=======
static ssize_t show_boost(struct kobject *kobj,
			  struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", cpufreq_driver->boost_enabled);
}

static ssize_t store_boost(struct kobject *kobj, struct kobj_attribute *attr,
			   const char *buf, size_t count)
{
	int ret, enable;

	ret = sscanf(buf, "%d", &enable);
	if (ret != 1 || enable < 0 || enable > 1)
		return -EINVAL;

	if (cpufreq_boost_trigger_state(enable)) {
		pr_err("%s: Cannot %s BOOST!\n",
		       __func__, enable ? "enable" : "disable");
		return -EINVAL;
	}

	pr_debug("%s: cpufreq BOOST %s\n",
		 __func__, enable ? "enabled" : "disabled");

	return count;
}
define_one_global_rw(boost);

static struct cpufreq_governor *find_governor(const char *str_governor)
{
	struct cpufreq_governor *t;

	for_each_governor(t)
		if (!strncasecmp(str_governor, t->name, CPUFREQ_NAME_LEN))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return t;

	return NULL;
}

/**
 * cpufreq_parse_governor - parse a governor string
 */
static int cpufreq_parse_governor(char *str_governor, unsigned int *policy,
				struct cpufreq_governor **governor)
{
	int err = -EINVAL;

<<<<<<< HEAD
	if (!cpufreq_driver)
		goto out;

	if (cpufreq_driver->setpolicy) {
		if (!strnicmp(str_governor, "performance", CPUFREQ_NAME_LEN)) {
			*policy = CPUFREQ_POLICY_PERFORMANCE;
			err = 0;
		} else if (!strnicmp(str_governor, "powersave",
=======
	if (cpufreq_driver->setpolicy) {
		if (!strncasecmp(str_governor, "performance", CPUFREQ_NAME_LEN)) {
			*policy = CPUFREQ_POLICY_PERFORMANCE;
			err = 0;
		} else if (!strncasecmp(str_governor, "powersave",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						CPUFREQ_NAME_LEN)) {
			*policy = CPUFREQ_POLICY_POWERSAVE;
			err = 0;
		}
<<<<<<< HEAD
	} else if (cpufreq_driver->target) {
=======
	} else {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		struct cpufreq_governor *t;

		mutex_lock(&cpufreq_governor_mutex);

<<<<<<< HEAD
		t = __find_governor(str_governor);
=======
		t = find_governor(str_governor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (t == NULL) {
			int ret;

			mutex_unlock(&cpufreq_governor_mutex);
			ret = request_module("cpufreq_%s", str_governor);
			mutex_lock(&cpufreq_governor_mutex);

			if (ret == 0)
<<<<<<< HEAD
				t = __find_governor(str_governor);
=======
				t = find_governor(str_governor);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		if (t != NULL) {
			*governor = t;
			err = 0;
		}

		mutex_unlock(&cpufreq_governor_mutex);
	}
<<<<<<< HEAD
out:
	return err;
}


=======
	return err;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * cpufreq_per_cpu_attr_read() / show_##file_name() -
 * print out cpufreq information
 *
 * Write out information from cpufreq_driver->policy[cpu]; object must be
 * "unsigned int".
 */

#define show_one(file_name, object)			\
static ssize_t show_##file_name				\
(struct cpufreq_policy *policy, char *buf)		\
{							\
	return sprintf(buf, "%u\n", policy->object);	\
}

show_one(cpuinfo_min_freq, cpuinfo.min_freq);
show_one(cpuinfo_max_freq, cpuinfo.max_freq);
show_one(cpuinfo_transition_latency, cpuinfo.transition_latency);
show_one(scaling_min_freq, min);
show_one(scaling_max_freq, max);
<<<<<<< HEAD
show_one(scaling_cur_freq, cur);

static int __cpufreq_set_policy(struct cpufreq_policy *data,
				struct cpufreq_policy *policy);
=======

static ssize_t show_scaling_cur_freq(struct cpufreq_policy *policy, char *buf)
{
	ssize_t ret;

	if (cpufreq_driver && cpufreq_driver->setpolicy && cpufreq_driver->get)
		ret = sprintf(buf, "%u\n", cpufreq_driver->get(policy->cpu));
	else
		ret = sprintf(buf, "%u\n", policy->cur);
	return ret;
}

static int cpufreq_set_policy(struct cpufreq_policy *policy,
				struct cpufreq_policy *new_policy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * cpufreq_per_cpu_attr_write() / store_##file_name() - sysfs write access
 */
#define store_one(file_name, object)			\
static ssize_t store_##file_name					\
(struct cpufreq_policy *policy, const char *buf, size_t count)		\
{									\
<<<<<<< HEAD
	unsigned int ret;						\
	struct cpufreq_policy new_policy;				\
									\
	ret = cpufreq_get_policy(&new_policy, policy->cpu);		\
	if (ret)							\
		return -EINVAL;						\
=======
	int ret, temp;							\
	struct cpufreq_policy new_policy;				\
									\
	memcpy(&new_policy, policy, sizeof(*policy));			\
	new_policy.min = policy->user_policy.min;			\
	new_policy.max = policy->user_policy.max;			\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
									\
	ret = sscanf(buf, "%u", &new_policy.object);			\
	if (ret != 1)							\
		return -EINVAL;						\
									\
<<<<<<< HEAD
	ret = __cpufreq_set_policy(policy, &new_policy);		\
	policy->user_policy.object = policy->object;			\
=======
	temp = new_policy.object;					\
	ret = cpufreq_set_policy(policy, &new_policy);		\
	if (!ret)							\
		policy->user_policy.object = temp;			\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
									\
	return ret ? ret : count;					\
}

store_one(scaling_min_freq, min);
store_one(scaling_max_freq, max);

/**
 * show_cpuinfo_cur_freq - current CPU frequency as detected by hardware
 */
static ssize_t show_cpuinfo_cur_freq(struct cpufreq_policy *policy,
					char *buf)
{
<<<<<<< HEAD
	unsigned int cur_freq = __cpufreq_get(policy->cpu);
=======
	unsigned int cur_freq = __cpufreq_get(policy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (cur_freq)
		return sprintf(buf, "%u\n", cur_freq);

	return sprintf(buf, "<unknown>\n");
}

<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * show_scaling_governor - show the current policy for the specified CPU
 */
static ssize_t show_scaling_governor(struct cpufreq_policy *policy, char *buf)
{
	if (policy->policy == CPUFREQ_POLICY_POWERSAVE)
		return sprintf(buf, "powersave\n");
	else if (policy->policy == CPUFREQ_POLICY_PERFORMANCE)
		return sprintf(buf, "performance\n");
	else if (policy->governor)
		return scnprintf(buf, CPUFREQ_NAME_PLEN, "%s\n",
				policy->governor->name);
	return -EINVAL;
}

<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * store_scaling_governor - store policy for the specified CPU
 */
static ssize_t store_scaling_governor(struct cpufreq_policy *policy,
					const char *buf, size_t count)
{
<<<<<<< HEAD
	unsigned int ret;
	char	str_governor[16];
	struct cpufreq_policy new_policy;

	ret = cpufreq_get_policy(&new_policy, policy->cpu);
	if (ret)
		return ret;
=======
	int ret;
	char	str_governor[16];
	struct cpufreq_policy new_policy;

	memcpy(&new_policy, policy, sizeof(*policy));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = sscanf(buf, "%15s", str_governor);
	if (ret != 1)
		return -EINVAL;

	if (cpufreq_parse_governor(str_governor, &new_policy.policy,
						&new_policy.governor))
		return -EINVAL;

<<<<<<< HEAD
	/* Do not use cpufreq_set_policy here or the user_policy.max
	   will be wrongly overridden */
	ret = __cpufreq_set_policy(policy, &new_policy);

	policy->user_policy.policy = policy->policy;
	policy->user_policy.governor = policy->governor;

	if (ret)
		return ret;
	else
		return count;
=======
	ret = cpufreq_set_policy(policy, &new_policy);
	return ret ? ret : count;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * show_scaling_driver - show the cpufreq driver currently loaded
 */
static ssize_t show_scaling_driver(struct cpufreq_policy *policy, char *buf)
{
	return scnprintf(buf, CPUFREQ_NAME_PLEN, "%s\n", cpufreq_driver->name);
}

/**
 * show_scaling_available_governors - show the available CPUfreq governors
 */
static ssize_t show_scaling_available_governors(struct cpufreq_policy *policy,
						char *buf)
{
	ssize_t i = 0;
	struct cpufreq_governor *t;

<<<<<<< HEAD
	if (!cpufreq_driver->target) {
=======
	if (!has_target()) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		i += sprintf(buf, "performance powersave");
		goto out;
	}

<<<<<<< HEAD
	list_for_each_entry(t, &cpufreq_governor_list, governor_list) {
=======
	for_each_governor(t) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (i >= (ssize_t) ((PAGE_SIZE / sizeof(char))
		    - (CPUFREQ_NAME_LEN + 2)))
			goto out;
		i += scnprintf(&buf[i], CPUFREQ_NAME_PLEN, "%s ", t->name);
	}
out:
	i += sprintf(&buf[i], "\n");
	return i;
}

<<<<<<< HEAD
static ssize_t show_cpus(const struct cpumask *mask, char *buf)
=======
ssize_t cpufreq_show_cpus(const struct cpumask *mask, char *buf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	ssize_t i = 0;
	unsigned int cpu;

	for_each_cpu(cpu, mask) {
		if (i)
			i += scnprintf(&buf[i], (PAGE_SIZE - i - 2), " ");
		i += scnprintf(&buf[i], (PAGE_SIZE - i - 2), "%u", cpu);
		if (i >= (PAGE_SIZE - 5))
			break;
	}
	i += sprintf(&buf[i], "\n");
	return i;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(cpufreq_show_cpus);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * show_related_cpus - show the CPUs affected by each transition even if
 * hw coordination is in use
 */
static ssize_t show_related_cpus(struct cpufreq_policy *policy, char *buf)
{
<<<<<<< HEAD
	return show_cpus(policy->related_cpus, buf);
=======
	return cpufreq_show_cpus(policy->related_cpus, buf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * show_affected_cpus - show the CPUs affected by each transition
 */
static ssize_t show_affected_cpus(struct cpufreq_policy *policy, char *buf)
{
<<<<<<< HEAD
	return show_cpus(policy->cpus, buf);
=======
	return cpufreq_show_cpus(policy->cpus, buf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static ssize_t store_scaling_setspeed(struct cpufreq_policy *policy,
					const char *buf, size_t count)
{
	unsigned int freq = 0;
	unsigned int ret;

	if (!policy->governor || !policy->governor->store_setspeed)
		return -EINVAL;

	ret = sscanf(buf, "%u", &freq);
	if (ret != 1)
		return -EINVAL;

	policy->governor->store_setspeed(policy, freq);

	return count;
}

static ssize_t show_scaling_setspeed(struct cpufreq_policy *policy, char *buf)
{
	if (!policy->governor || !policy->governor->show_setspeed)
		return sprintf(buf, "<unsupported>\n");

	return policy->governor->show_setspeed(policy, buf);
}

<<<<<<< HEAD
#ifdef CONFIG_HUAWEI_MSG_POLICY
static ssize_t store_msg_policy(struct cpufreq_policy *policy,
					const char *buf, size_t count)
{
	unsigned int value = 0;
	unsigned int ret;

	ret = sscanf(buf, "%u", &value);
	if (ret != 1)
		return -EINVAL;

	set_msg_threshold(value);

	return count;
}

static ssize_t show_msg_policy(struct cpufreq_policy *policy, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "threshold:%u,max_msg_percent:%u\n", 
		get_msg_threshold(), get_max_msg_percent());
}
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * show_bios_limit - show the current cpufreq HW/BIOS limitation
 */
static ssize_t show_bios_limit(struct cpufreq_policy *policy, char *buf)
{
	unsigned int limit;
	int ret;
	if (cpufreq_driver->bios_limit) {
		ret = cpufreq_driver->bios_limit(policy->cpu, &limit);
		if (!ret)
			return sprintf(buf, "%u\n", limit);
	}
	return sprintf(buf, "%u\n", policy->cpuinfo.max_freq);
}

cpufreq_freq_attr_ro_perm(cpuinfo_cur_freq, 0400);
cpufreq_freq_attr_ro(cpuinfo_min_freq);
cpufreq_freq_attr_ro(cpuinfo_max_freq);
cpufreq_freq_attr_ro(cpuinfo_transition_latency);
cpufreq_freq_attr_ro(scaling_available_governors);
cpufreq_freq_attr_ro(scaling_driver);
cpufreq_freq_attr_ro(scaling_cur_freq);
cpufreq_freq_attr_ro(bios_limit);
cpufreq_freq_attr_ro(related_cpus);
cpufreq_freq_attr_ro(affected_cpus);
cpufreq_freq_attr_rw(scaling_min_freq);
cpufreq_freq_attr_rw(scaling_max_freq);
cpufreq_freq_attr_rw(scaling_governor);
cpufreq_freq_attr_rw(scaling_setspeed);
<<<<<<< HEAD
#ifdef CONFIG_HUAWEI_MSG_POLICY
cpufreq_freq_attr_rw(msg_policy);
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static struct attribute *default_attrs[] = {
	&cpuinfo_min_freq.attr,
	&cpuinfo_max_freq.attr,
	&cpuinfo_transition_latency.attr,
	&scaling_min_freq.attr,
	&scaling_max_freq.attr,
	&affected_cpus.attr,
	&related_cpus.attr,
	&scaling_governor.attr,
	&scaling_driver.attr,
	&scaling_available_governors.attr,
	&scaling_setspeed.attr,
<<<<<<< HEAD
#ifdef CONFIG_HUAWEI_MSG_POLICY
	&msg_policy.attr,
#endif
	NULL
};

struct kobject *cpufreq_global_kobject;
EXPORT_SYMBOL(cpufreq_global_kobject);

=======
	NULL
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define to_policy(k) container_of(k, struct cpufreq_policy, kobj)
#define to_attr(a) container_of(a, struct freq_attr, attr)

static ssize_t show(struct kobject *kobj, struct attribute *attr, char *buf)
{
	struct cpufreq_policy *policy = to_policy(kobj);
	struct freq_attr *fattr = to_attr(attr);
<<<<<<< HEAD
	ssize_t ret = -EINVAL;
	policy = cpufreq_cpu_get_sysfs(policy->cpu);
	if (!policy)
		goto no_policy;

	if (lock_policy_rwsem_read(policy->cpu) < 0)
		goto fail;

	if (fattr->show)
		ret = fattr->show(policy, buf);
	else
		ret = -EIO;

	unlock_policy_rwsem_read(policy->cpu);
fail:
	cpufreq_cpu_put_sysfs(policy);
no_policy:
=======
	ssize_t ret;

	if (!fattr->show)
		return -EIO;

	down_read(&policy->rwsem);
	ret = fattr->show(policy, buf);
	up_read(&policy->rwsem);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static ssize_t store(struct kobject *kobj, struct attribute *attr,
		     const char *buf, size_t count)
{
	struct cpufreq_policy *policy = to_policy(kobj);
	struct freq_attr *fattr = to_attr(attr);
	ssize_t ret = -EINVAL;
<<<<<<< HEAD
	policy = cpufreq_cpu_get_sysfs(policy->cpu);
	if (!policy)
		goto no_policy;

	if (lock_policy_rwsem_write(policy->cpu) < 0)
		goto fail;

	if (fattr->store)
		ret = fattr->store(policy, buf, count);
	else
		ret = -EIO;

	unlock_policy_rwsem_write(policy->cpu);
fail:
	cpufreq_cpu_put_sysfs(policy);
no_policy:
=======

	if (!fattr->store)
		return -EIO;

	get_online_cpus();

	if (cpu_online(policy->cpu)) {
		down_write(&policy->rwsem);
		ret = fattr->store(policy, buf, count);
		up_write(&policy->rwsem);
	}

	put_online_cpus();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static void cpufreq_sysfs_release(struct kobject *kobj)
{
	struct cpufreq_policy *policy = to_policy(kobj);
	pr_debug("last reference is dropped\n");
	complete(&policy->kobj_unregister);
}

static const struct sysfs_ops sysfs_ops = {
	.show	= show,
	.store	= store,
};

static struct kobj_type ktype_cpufreq = {
	.sysfs_ops	= &sysfs_ops,
	.default_attrs	= default_attrs,
	.release	= cpufreq_sysfs_release,
};

<<<<<<< HEAD
/* symlink affected CPUs */
static int cpufreq_add_dev_symlink(unsigned int cpu,
				   struct cpufreq_policy *policy)
{
	unsigned int j;
	int ret = 0;

	for_each_cpu(j, policy->cpus) {
		struct cpufreq_policy *managed_policy;
		struct device *cpu_dev;

		if (j == cpu)
			continue;

		pr_debug("CPU %u already managed, adding link\n", j);
		managed_policy = cpufreq_cpu_get(cpu);
		cpu_dev = get_cpu_device(j);
		ret = sysfs_create_link(&cpu_dev->kobj, &policy->kobj,
					"cpufreq");
		if (ret) {
			cpufreq_cpu_put(managed_policy);
			return ret;
		}
	}
	return ret;
}

static int cpufreq_add_dev_interface(unsigned int cpu,
				     struct cpufreq_policy *policy,
				     struct device *dev)
{
	struct cpufreq_policy new_policy;
	struct freq_attr **drv_attr;
	unsigned long flags;
	int ret = 0;
	unsigned int j;

	/* prepare interface data */
	ret = kobject_init_and_add(&policy->kobj, &ktype_cpufreq,
				   &dev->kobj, "cpufreq");
	if (ret)
		return ret;

	/* set up files for this cpu device */
	drv_attr = cpufreq_driver->attr;
	while ((drv_attr) && (*drv_attr)) {
		ret = sysfs_create_file(&policy->kobj, &((*drv_attr)->attr));
		if (ret)
			goto err_out_kobj_put;
=======
static void add_cpu_dev_symlink(struct cpufreq_policy *policy, unsigned int cpu)
{
	struct device *dev = get_cpu_device(cpu);

	if (!dev)
		return;

	if (cpumask_test_and_set_cpu(cpu, policy->real_cpus))
		return;

	dev_dbg(dev, "%s: Adding symlink\n", __func__);
	if (sysfs_create_link(&dev->kobj, &policy->kobj, "cpufreq"))
		dev_err(dev, "cpufreq symlink creation failed\n");
}

static void remove_cpu_dev_symlink(struct cpufreq_policy *policy,
				   struct device *dev)
{
	dev_dbg(dev, "%s: Removing symlink\n", __func__);
	sysfs_remove_link(&dev->kobj, "cpufreq");
}

static int cpufreq_add_dev_interface(struct cpufreq_policy *policy)
{
	struct freq_attr **drv_attr;
	int ret = 0;

	/* set up files for this cpu device */
	drv_attr = cpufreq_driver->attr;
	while (drv_attr && *drv_attr) {
		ret = sysfs_create_file(&policy->kobj, &((*drv_attr)->attr));
		if (ret)
			return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		drv_attr++;
	}
	if (cpufreq_driver->get) {
		ret = sysfs_create_file(&policy->kobj, &cpuinfo_cur_freq.attr);
		if (ret)
<<<<<<< HEAD
			goto err_out_kobj_put;
	}
	if (cpufreq_driver->target) {
		ret = sysfs_create_file(&policy->kobj, &scaling_cur_freq.attr);
		if (ret)
			goto err_out_kobj_put;
	}
	if (cpufreq_driver->bios_limit) {
		ret = sysfs_create_file(&policy->kobj, &bios_limit.attr);
		if (ret)
			goto err_out_kobj_put;
	}

	write_lock_irqsave(&cpufreq_driver_lock, flags);
	for_each_cpu(j, policy->cpus) {
		per_cpu(cpufreq_cpu_data, j) = policy;
		per_cpu(cpufreq_policy_cpu, j) = policy->cpu;
	}
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	ret = cpufreq_add_dev_symlink(cpu, policy);
	if (ret)
		goto err_out_kobj_put;

	memcpy(&new_policy, policy, sizeof(struct cpufreq_policy));
	/* assure that the starting sequence is run in __cpufreq_set_policy */
	policy->governor = NULL;

	/* set default policy */
	ret = __cpufreq_set_policy(policy, &new_policy);
	policy->user_policy.policy = policy->policy;
	policy->user_policy.governor = policy->governor;

	if (ret) {
		pr_debug("setting policy failed\n");
		if (cpufreq_driver->exit)
			cpufreq_driver->exit(policy);
	}
	return ret;

err_out_kobj_put:
	kobject_put(&policy->kobj);
	wait_for_completion(&policy->kobj_unregister);
	return ret;
}

#ifdef CONFIG_HOTPLUG_CPU
static int cpufreq_add_policy_cpu(unsigned int cpu, unsigned int sibling,
				  struct device *dev)
{
	struct cpufreq_policy *policy;
	int ret = 0, has_target = !!cpufreq_driver->target;
	unsigned long flags;

	policy = cpufreq_cpu_get(sibling);
	if(NULL == policy){
		pr_err("[%s]: policy cpufreq_cpu_get return null.\n", __func__);
		return -1;
	}
	WARN_ON(!policy);

	if (has_target)
		__cpufreq_governor(policy, CPUFREQ_GOV_STOP);

	lock_policy_rwsem_write(sibling);

	write_lock_irqsave(&cpufreq_driver_lock, flags);

	cpumask_set_cpu(cpu, policy->cpus);
	per_cpu(cpufreq_policy_cpu, cpu) = policy->cpu;
	per_cpu(cpufreq_cpu_data, cpu) = policy;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	unlock_policy_rwsem_write(sibling);

	if (has_target) {
		__cpufreq_governor(policy, CPUFREQ_GOV_START);
		__cpufreq_governor(policy, CPUFREQ_GOV_LIMITS);
	}

	ret = sysfs_create_link(&dev->kobj, &policy->kobj, "cpufreq");
	if (ret) {
		cpufreq_cpu_put(policy);
		return ret;
	}

	return 0;
}
#endif

#ifdef CONFIG_HISI_RDR
typedef void (*rdr_funcptr_3)(u32, u32, u32);
static rdr_funcptr_3 g_rdr_cpu_on_off_hook;

void rdr_cpu_on_off_hook_add(rdr_funcptr_3 p_hook_func)
{
	g_rdr_cpu_on_off_hook = p_hook_func;
}

void rdr_cpu_on_off_hook_delete(void)
{
	g_rdr_cpu_on_off_hook = NULL;
}
#else
typedef void (*rdr_funcptr_3)(u32, u32, u32);
void rdr_cpu_on_off_hook_add(rdr_funcptr_3 p_hook_func)
{
}

void rdr_cpu_on_off_hook_delete(void)
{
}
#endif

/**
 * cpufreq_add_dev - add a CPU device
 *
 * Adds the cpufreq interface for a CPU device.
 *
 * The Oracle says: try running cpufreq registration/unregistration concurrently
 * with with cpu hotplugging and all hell will break loose. Tried to clean this
 * mess up, but more thorough testing is needed. - Mathieu
 */
static int cpufreq_add_dev(struct device *dev, struct subsys_interface *sif)
{
	unsigned int j, cpu = dev->id;
	int ret = -ENOMEM;
	struct cpufreq_policy *policy;
	unsigned long flags;
#ifdef CONFIG_HOTPLUG_CPU
	struct cpufreq_governor *gov;
	int sibling;
#endif

	if (cpu_is_offline(cpu))
		return 0;

	pr_debug("adding CPU %u\n", cpu);

#ifdef CONFIG_HISI_RDR
	if (g_rdr_cpu_on_off_hook != NULL)
		g_rdr_cpu_on_off_hook(cpu, 0xff, 1);
#endif

#ifdef CONFIG_SMP
	/* check whether a different CPU already registered this
	 * CPU because it is in the same boat. */
	policy = cpufreq_cpu_get(cpu);
	if (unlikely(policy)) {
		cpufreq_cpu_put(policy);
		return 0;
	}

#ifdef CONFIG_HOTPLUG_CPU
	/* Check if this cpu was hot-unplugged earlier and has siblings */
	read_lock_irqsave(&cpufreq_driver_lock, flags);
	for_each_online_cpu(sibling) {
		struct cpufreq_policy *cp = per_cpu(cpufreq_cpu_data, sibling);
		if (cp && cpumask_test_cpu(cpu, cp->related_cpus)) {
			read_unlock_irqrestore(&cpufreq_driver_lock, flags);
			return cpufreq_add_policy_cpu(cpu, sibling, dev);
		}
	}
	read_unlock_irqrestore(&cpufreq_driver_lock, flags);
#endif
#endif

	if (!try_module_get(cpufreq_driver->owner)) {
		ret = -EINVAL;
		goto module_out;
	}

	policy = kzalloc(sizeof(struct cpufreq_policy), GFP_KERNEL);
	if (!policy)
		goto nomem_out;

	if (!alloc_cpumask_var(&policy->cpus, GFP_KERNEL))
		goto err_free_policy;

	if (!zalloc_cpumask_var(&policy->related_cpus, GFP_KERNEL))
		goto err_free_cpumask;

	policy->cpu = cpu;
	policy->governor = CPUFREQ_DEFAULT_GOVERNOR;
	cpumask_copy(policy->cpus, cpumask_of(cpu));

	/* Initially set CPU itself as the policy_cpu */
	per_cpu(cpufreq_policy_cpu, cpu) = cpu;

	init_completion(&policy->kobj_unregister);
	INIT_WORK(&policy->update, handle_update);

=======
			return ret;
	}

	ret = sysfs_create_file(&policy->kobj, &scaling_cur_freq.attr);
	if (ret)
		return ret;

	if (cpufreq_driver->bios_limit) {
		ret = sysfs_create_file(&policy->kobj, &bios_limit.attr);
		if (ret)
			return ret;
	}

	return 0;
}

__weak struct cpufreq_governor *cpufreq_default_governor(void)
{
	return NULL;
}

static int cpufreq_init_policy(struct cpufreq_policy *policy)
{
	struct cpufreq_governor *gov = NULL;
	struct cpufreq_policy new_policy;

	memcpy(&new_policy, policy, sizeof(*policy));

	/* Update governor of new_policy to the governor used before hotplug */
	gov = find_governor(policy->last_governor);
	if (gov) {
		pr_debug("Restoring governor %s for cpu %d\n",
				policy->governor->name, policy->cpu);
	} else {
		gov = cpufreq_default_governor();
		if (!gov)
			return -ENODATA;
	}

	new_policy.governor = gov;

	/* Use the default policy if there is no last_policy. */
	if (cpufreq_driver->setpolicy) {
		if (policy->last_policy)
			new_policy.policy = policy->last_policy;
		else
			cpufreq_parse_governor(gov->name, &new_policy.policy,
					       NULL);
	}
	/* set default policy */
	return cpufreq_set_policy(policy, &new_policy);
}

static int cpufreq_add_policy_cpu(struct cpufreq_policy *policy, unsigned int cpu)
{
	int ret = 0;

	/* Has this CPU been taken care of already? */
	if (cpumask_test_cpu(cpu, policy->cpus))
		return 0;

	down_write(&policy->rwsem);
	if (has_target())
		cpufreq_stop_governor(policy);

	cpumask_set_cpu(cpu, policy->cpus);

	if (has_target()) {
		ret = cpufreq_start_governor(policy);
		if (ret)
			pr_err("%s: Failed to start governor\n", __func__);
	}
	up_write(&policy->rwsem);
	return ret;
}

static void handle_update(struct work_struct *work)
{
	struct cpufreq_policy *policy =
		container_of(work, struct cpufreq_policy, update);
	unsigned int cpu = policy->cpu;
	pr_debug("handle_update for cpu %u called\n", cpu);
	cpufreq_update_policy(cpu);
}

static struct cpufreq_policy *cpufreq_policy_alloc(unsigned int cpu)
{
	struct cpufreq_policy *policy;
	int ret;

	policy = kzalloc(sizeof(*policy), GFP_KERNEL);
	if (!policy)
		return NULL;

	if (!alloc_cpumask_var(&policy->cpus, GFP_KERNEL))
		goto err_free_policy;

	if (!zalloc_cpumask_var(&policy->related_cpus, GFP_KERNEL))
		goto err_free_cpumask;

	if (!zalloc_cpumask_var(&policy->real_cpus, GFP_KERNEL))
		goto err_free_rcpumask;

	ret = kobject_init_and_add(&policy->kobj, &ktype_cpufreq,
				   cpufreq_global_kobject, "policy%u", cpu);
	if (ret) {
		pr_err("%s: failed to init policy->kobj: %d\n", __func__, ret);
		kobject_put(&policy->kobj);
		goto err_free_real_cpus;
	}

	INIT_LIST_HEAD(&policy->policy_list);
	init_rwsem(&policy->rwsem);
	spin_lock_init(&policy->transition_lock);
	init_waitqueue_head(&policy->transition_wait);
	init_completion(&policy->kobj_unregister);
	INIT_WORK(&policy->update, handle_update);

	policy->cpu = cpu;
	return policy;

err_free_real_cpus:
	free_cpumask_var(policy->real_cpus);
err_free_rcpumask:
	free_cpumask_var(policy->related_cpus);
err_free_cpumask:
	free_cpumask_var(policy->cpus);
err_free_policy:
	kfree(policy);

	return NULL;
}

static void cpufreq_policy_put_kobj(struct cpufreq_policy *policy, bool notify)
{
	struct kobject *kobj;
	struct completion *cmp;

	if (notify)
		blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
					     CPUFREQ_REMOVE_POLICY, policy);

	down_write(&policy->rwsem);
	cpufreq_stats_free_table(policy);
	kobj = &policy->kobj;
	cmp = &policy->kobj_unregister;
	up_write(&policy->rwsem);
	kobject_put(kobj);

	/*
	 * We need to make sure that the underlying kobj is
	 * actually not referenced anymore by anybody before we
	 * proceed with unloading.
	 */
	pr_debug("waiting for dropping of refcount\n");
	wait_for_completion(cmp);
	pr_debug("wait complete\n");
}

static void cpufreq_policy_free(struct cpufreq_policy *policy, bool notify)
{
	unsigned long flags;
	int cpu;

	/* Remove policy from list */
	write_lock_irqsave(&cpufreq_driver_lock, flags);
	list_del(&policy->policy_list);

	for_each_cpu(cpu, policy->related_cpus)
		per_cpu(cpufreq_cpu_data, cpu) = NULL;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	cpufreq_policy_put_kobj(policy, notify);
	free_cpumask_var(policy->real_cpus);
	free_cpumask_var(policy->related_cpus);
	free_cpumask_var(policy->cpus);
	kfree(policy);
}

static int cpufreq_online(unsigned int cpu)
{
	struct cpufreq_policy *policy;
	bool new_policy;
	unsigned long flags;
	unsigned int j;
	int ret;

	pr_debug("%s: bringing CPU%u online\n", __func__, cpu);

	/* Check if this CPU already has a policy to manage it */
	policy = per_cpu(cpufreq_cpu_data, cpu);
	if (policy) {
		WARN_ON(!cpumask_test_cpu(cpu, policy->related_cpus));
		if (!policy_is_inactive(policy))
			return cpufreq_add_policy_cpu(policy, cpu);

		/* This is the only online CPU for the policy.  Start over. */
		new_policy = false;
		down_write(&policy->rwsem);
		policy->cpu = cpu;
		policy->governor = NULL;
		up_write(&policy->rwsem);
	} else {
		new_policy = true;
		policy = cpufreq_policy_alloc(cpu);
		if (!policy)
			return -ENOMEM;
	}

	cpumask_copy(policy->cpus, cpumask_of(cpu));

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* call driver. From then on the cpufreq must be able
	 * to accept all calls to ->verify and ->setpolicy for this CPU
	 */
	ret = cpufreq_driver->init(policy);
	if (ret) {
		pr_debug("initialization failed\n");
<<<<<<< HEAD
		goto err_set_policy_cpu;
	}

	/* related cpus should atleast have policy->cpus */
	cpumask_or(policy->related_cpus, policy->related_cpus, policy->cpus);
=======
		goto out_free_policy;
	}

	down_write(&policy->rwsem);

	if (new_policy) {
		/* related_cpus should at least include policy->cpus. */
		cpumask_copy(policy->related_cpus, policy->cpus);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * affected cpus must always be the one, which are online. We aren't
	 * managing offline cpus here.
	 */
	cpumask_and(policy->cpus, policy->cpus, cpu_online_mask);

<<<<<<< HEAD
	policy->user_policy.min = policy->min;
	policy->user_policy.max = policy->max;
=======
	if (new_policy) {
		policy->user_policy.min = policy->min;
		policy->user_policy.max = policy->max;

		for_each_cpu(j, policy->related_cpus) {
			per_cpu(cpufreq_cpu_data, j) = policy;
			add_cpu_dev_symlink(policy, j);
		}
	} else {
		policy->min = policy->user_policy.min;
		policy->max = policy->user_policy.max;
	}

	if (cpufreq_driver->get && !cpufreq_driver->setpolicy) {
		policy->cur = cpufreq_driver->get(policy->cpu);
		if (!policy->cur) {
			pr_err("%s: ->get() failed\n", __func__);
			goto out_exit_policy;
		}
	}

	/*
	 * Sometimes boot loaders set CPU frequency to a value outside of
	 * frequency table present with cpufreq core. In such cases CPU might be
	 * unstable if it has to run on that frequency for long duration of time
	 * and so its better to set it to a frequency which is specified in
	 * freq-table. This also makes cpufreq stats inconsistent as
	 * cpufreq-stats would fail to register because current frequency of CPU
	 * isn't found in freq-table.
	 *
	 * Because we don't want this change to effect boot process badly, we go
	 * for the next freq which is >= policy->cur ('cur' must be set by now,
	 * otherwise we will end up setting freq to lowest of the table as 'cur'
	 * is initialized to zero).
	 *
	 * We are passing target-freq as "policy->cur - 1" otherwise
	 * __cpufreq_driver_target() would simply fail, as policy->cur will be
	 * equal to target-freq.
	 */
	if ((cpufreq_driver->flags & CPUFREQ_NEED_INITIAL_FREQ_CHECK)
	    && has_target()) {
		/* Are we running at unknown frequency ? */
		ret = cpufreq_frequency_table_get_index(policy, policy->cur);
		if (ret == -EINVAL) {
			/* Warn user and fix it */
			pr_warn("%s: CPU%d: Running at unlisted freq: %u KHz\n",
				__func__, policy->cpu, policy->cur);
			ret = __cpufreq_driver_target(policy, policy->cur - 1,
				CPUFREQ_RELATION_L);

			/*
			 * Reaching here after boot in a few seconds may not
			 * mean that system will remain stable at "unknown"
			 * frequency for longer duration. Hence, a BUG_ON().
			 */
			BUG_ON(ret);
			pr_warn("%s: CPU%d: Unlisted initial frequency changed to: %u KHz\n",
				__func__, policy->cpu, policy->cur);
		}
	}

	if (new_policy) {
		ret = cpufreq_add_dev_interface(policy);
		if (ret)
			goto out_exit_policy;

		cpufreq_stats_create_table(policy);
		blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
				CPUFREQ_CREATE_POLICY, policy);

		write_lock_irqsave(&cpufreq_driver_lock, flags);
		list_add(&policy->policy_list, &cpufreq_policy_list);
		write_unlock_irqrestore(&cpufreq_driver_lock, flags);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
				     CPUFREQ_START, policy);

<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU
	gov = __find_governor(per_cpu(cpufreq_cpu_governor, cpu));
	if (gov) {
		policy->governor = gov;
		pr_debug("Restoring governor %s for cpu %d\n",
		       policy->governor->name, cpu);
	}
#endif

	ret = cpufreq_add_dev_interface(cpu, policy, dev);
	if (ret)
		goto err_out_unregister;

	kobject_uevent(&policy->kobj, KOBJ_ADD);
	module_put(cpufreq_driver->owner);
=======
	ret = cpufreq_init_policy(policy);
	if (ret) {
		pr_err("%s: Failed to initialize policy for cpu: %d (%d)\n",
		       __func__, cpu, ret);
		/* cpufreq_policy_free() will notify based on this */
		new_policy = false;
		goto out_exit_policy;
	}

	up_write(&policy->rwsem);

	kobject_uevent(&policy->kobj, KOBJ_ADD);

	/* Callback for handling stuff after policy is ready */
	if (cpufreq_driver->ready)
		cpufreq_driver->ready(policy);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pr_debug("initialization complete\n");

	return 0;

<<<<<<< HEAD
err_out_unregister:
	write_lock_irqsave(&cpufreq_driver_lock, flags);
	for_each_cpu(j, policy->cpus)
		per_cpu(cpufreq_cpu_data, j) = NULL;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	kobject_put(&policy->kobj);
	wait_for_completion(&policy->kobj_unregister);

err_set_policy_cpu:
	per_cpu(cpufreq_policy_cpu, cpu) = -1;
	free_cpumask_var(policy->related_cpus);
err_free_cpumask:
	free_cpumask_var(policy->cpus);
err_free_policy:
	kfree(policy);
nomem_out:
	module_put(cpufreq_driver->owner);
module_out:
	return ret;
}

static void update_policy_cpu(struct cpufreq_policy *policy, unsigned int cpu)
{
	int j;

	policy->last_cpu = policy->cpu;
	policy->cpu = cpu;

	for_each_cpu(j, policy->cpus)
		per_cpu(cpufreq_policy_cpu, j) = cpu;

#ifdef CONFIG_CPU_FREQ_TABLE
	cpufreq_frequency_table_update_policy_cpu(policy);
#endif
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
			CPUFREQ_UPDATE_POLICY_CPU, policy);
}

/**
 * __cpufreq_remove_dev - remove a CPU device
 *
 * Removes the cpufreq interface for a CPU device.
 * Caller should already have policy_rwsem in write mode for this CPU.
 * This routine frees the rwsem before returning.
 */
static int __cpufreq_remove_dev(struct device *dev, struct subsys_interface *sif)
{
	unsigned int cpu = dev->id, ret, cpus;
	unsigned long flags;
	struct cpufreq_policy *data;
	struct kobject *kobj;
	struct completion *cmp;
	struct device *cpu_dev;

	pr_debug("%s: unregistering CPU %u\n", __func__, cpu);

	write_lock_irqsave(&cpufreq_driver_lock, flags);

	data = per_cpu(cpufreq_cpu_data, cpu);
	per_cpu(cpufreq_cpu_data, cpu) = NULL;

	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	if (!data) {
		pr_debug("%s: No cpu_data found\n", __func__);
		return -EINVAL;
	}

	if (cpufreq_driver->target)
		__cpufreq_governor(data, CPUFREQ_GOV_STOP);

#ifdef CONFIG_HOTPLUG_CPU
	if (!cpufreq_driver->setpolicy)
		strncpy(per_cpu(cpufreq_cpu_governor, cpu),
			data->governor->name, CPUFREQ_NAME_LEN);
#endif

	WARN_ON(lock_policy_rwsem_write(cpu));
	cpus = cpumask_weight(data->cpus);

	if (cpus > 1)
		cpumask_clear_cpu(cpu, data->cpus);
	unlock_policy_rwsem_write(cpu);

	if (cpu != data->cpu) {
		sysfs_remove_link(&dev->kobj, "cpufreq");
	} else if (cpus > 1) {
		/* first sibling now owns the new sysfs dir */
		cpu_dev = get_cpu_device(cpumask_first(data->cpus));
		sysfs_remove_link(&cpu_dev->kobj, "cpufreq");
		ret = kobject_move(&data->kobj, &cpu_dev->kobj);
		if (ret) {
			pr_err("%s: Failed to move kobj: %d", __func__, ret);

			WARN_ON(lock_policy_rwsem_write(cpu));
			cpumask_set_cpu(cpu, data->cpus);

			write_lock_irqsave(&cpufreq_driver_lock, flags);
			per_cpu(cpufreq_cpu_data, cpu) = data;
			write_unlock_irqrestore(&cpufreq_driver_lock, flags);

			unlock_policy_rwsem_write(cpu);

			ret = sysfs_create_link(&cpu_dev->kobj, &data->kobj,
					"cpufreq");
			return -EINVAL;
		}

		WARN_ON(lock_policy_rwsem_write(cpu));
		update_policy_cpu(data, cpu_dev->id);
		unlock_policy_rwsem_write(cpu);
		pr_debug("%s: policy Kobject moved to cpu: %d from: %d\n",
				__func__, cpu_dev->id, cpu);
	}

	/* If cpu is last user of policy, free policy */
	if (cpus == 1) {
		if (cpufreq_driver->target)
			__cpufreq_governor(data, CPUFREQ_GOV_POLICY_EXIT);

		lock_policy_rwsem_read(cpu);
		kobj = &data->kobj;
		cmp = &data->kobj_unregister;
		unlock_policy_rwsem_read(cpu);
		kobject_put(kobj);

		/* we need to make sure that the underlying kobj is actually
		 * not referenced anymore by anybody before we proceed with
		 * unloading.
		 */
		pr_debug("waiting for dropping of refcount\n");
		wait_for_completion(cmp);
		pr_debug("wait complete\n");

		if (cpufreq_driver->exit)
			cpufreq_driver->exit(data);

		free_cpumask_var(data->related_cpus);
		free_cpumask_var(data->cpus);
		kfree(data);
	} else {
		pr_debug("%s: removing link, cpu: %d\n", __func__, cpu);
		cpufreq_cpu_put(data);
		if (cpufreq_driver->target) {
			__cpufreq_governor(data, CPUFREQ_GOV_START);
			__cpufreq_governor(data, CPUFREQ_GOV_LIMITS);
		}
	}

	per_cpu(cpufreq_policy_cpu, cpu) = -1;
#ifdef CONFIG_HISI_RDR
	if (g_rdr_cpu_on_off_hook != NULL)
		g_rdr_cpu_on_off_hook(cpu, 0xff, 0);
#endif
	return 0;
}


static int cpufreq_remove_dev(struct device *dev, struct subsys_interface *sif)
{
	unsigned int cpu = dev->id;
	int retval;

	if (cpu_is_offline(cpu))
		return 0;

	retval = __cpufreq_remove_dev(dev, sif);
	return retval;
}


static void handle_update(struct work_struct *work)
{
	struct cpufreq_policy *policy =
		container_of(work, struct cpufreq_policy, update);
	unsigned int cpu = policy->cpu;
	pr_debug("handle_update for cpu %u called\n", cpu);
	cpufreq_update_policy(cpu);
}

/**
 *	cpufreq_out_of_sync - If actual and saved CPU frequency differs, we're in deep trouble.
 *	@cpu: cpu number
 *	@old_freq: CPU frequency the kernel thinks the CPU runs at
=======
out_exit_policy:
	for_each_cpu(j, policy->real_cpus)
		remove_cpu_dev_symlink(policy, get_cpu_device(j));

	up_write(&policy->rwsem);

	if (cpufreq_driver->exit)
		cpufreq_driver->exit(policy);

out_free_policy:
	cpufreq_policy_free(policy, !new_policy);
	return ret;
}

/**
 * cpufreq_add_dev - the cpufreq interface for a CPU device.
 * @dev: CPU device.
 * @sif: Subsystem interface structure pointer (not used)
 */
static int cpufreq_add_dev(struct device *dev, struct subsys_interface *sif)
{
	struct cpufreq_policy *policy;
	unsigned cpu = dev->id;
	int ret;

	dev_dbg(dev, "%s: adding CPU%u\n", __func__, cpu);

	if (cpu_online(cpu)) {
		ret = cpufreq_online(cpu);
		if (ret)
			return ret;
	}

	/* Create sysfs link on CPU registration */
	policy = per_cpu(cpufreq_cpu_data, cpu);
	if (policy)
		add_cpu_dev_symlink(policy, cpu);

	return 0;
}

static int cpufreq_offline(unsigned int cpu)
{
	struct cpufreq_policy *policy;
	int ret;

	pr_debug("%s: unregistering CPU %u\n", __func__, cpu);

	policy = cpufreq_cpu_get_raw(cpu);
	if (!policy) {
		pr_debug("%s: No cpu_data found\n", __func__);
		return 0;
	}

	down_write(&policy->rwsem);
	if (has_target())
		cpufreq_stop_governor(policy);

	cpumask_clear_cpu(cpu, policy->cpus);

	if (policy_is_inactive(policy)) {
		if (has_target())
			strncpy(policy->last_governor, policy->governor->name,
				CPUFREQ_NAME_LEN);
		else
			policy->last_policy = policy->policy;
	} else if (cpu == policy->cpu) {
		/* Nominate new CPU */
		policy->cpu = cpumask_any(policy->cpus);
	}

	/* Start governor again for active policy */
	if (!policy_is_inactive(policy)) {
		if (has_target()) {
			ret = cpufreq_start_governor(policy);
			if (ret)
				pr_err("%s: Failed to start governor\n", __func__);
		}

		goto unlock;
	}

	if (cpufreq_driver->stop_cpu)
		cpufreq_driver->stop_cpu(policy);

	if (has_target())
		cpufreq_exit_governor(policy);

	/*
	 * Perform the ->exit() even during light-weight tear-down,
	 * since this is a core component, and is essential for the
	 * subsequent light-weight ->init() to succeed.
	 */
	if (cpufreq_driver->exit) {
		cpufreq_driver->exit(policy);
		policy->freq_table = NULL;
	}

unlock:
	up_write(&policy->rwsem);
	return 0;
}

/**
 * cpufreq_remove_dev - remove a CPU device
 *
 * Removes the cpufreq interface for a CPU device.
 */
static void cpufreq_remove_dev(struct device *dev, struct subsys_interface *sif)
{
	unsigned int cpu = dev->id;
	struct cpufreq_policy *policy = per_cpu(cpufreq_cpu_data, cpu);

	if (!policy)
		return;

	if (cpu_online(cpu))
		cpufreq_offline(cpu);

	cpumask_clear_cpu(cpu, policy->real_cpus);
	remove_cpu_dev_symlink(policy, dev);

	if (cpumask_empty(policy->real_cpus))
		cpufreq_policy_free(policy, true);
}

/**
 *	cpufreq_out_of_sync - If actual and saved CPU frequency differs, we're
 *	in deep trouble.
 *	@policy: policy managing CPUs
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *	@new_freq: CPU frequency the CPU actually runs at
 *
 *	We adjust to current frequency first, and need to clean up later.
 *	So either call to cpufreq_update_policy() or schedule handle_update()).
 */
<<<<<<< HEAD
static void cpufreq_out_of_sync(unsigned int cpu, unsigned int old_freq,
				unsigned int new_freq)
{
	struct cpufreq_policy *policy;
	struct cpufreq_freqs freqs;
	unsigned long flags;


	pr_debug("Warning: CPU frequency out of sync: cpufreq and timing "
	       "core thinks of %u, is %u kHz.\n", old_freq, new_freq);

	freqs.old = old_freq;
	freqs.new = new_freq;

	read_lock_irqsave(&cpufreq_driver_lock, flags);
	policy = per_cpu(cpufreq_cpu_data, cpu);
	read_unlock_irqrestore(&cpufreq_driver_lock, flags);

	cpufreq_notify_transition(policy, &freqs, CPUFREQ_PRECHANGE);
	cpufreq_notify_transition(policy, &freqs, CPUFREQ_POSTCHANGE);
}


=======
static void cpufreq_out_of_sync(struct cpufreq_policy *policy,
				unsigned int new_freq)
{
	struct cpufreq_freqs freqs;

	pr_debug("Warning: CPU frequency out of sync: cpufreq and timing core thinks of %u, is %u kHz\n",
		 policy->cur, new_freq);

	freqs.old = policy->cur;
	freqs.new = new_freq;

	cpufreq_freq_transition_begin(policy, &freqs);
	cpufreq_freq_transition_end(policy, &freqs, 0);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * cpufreq_quick_get - get the CPU frequency (in kHz) from policy->cur
 * @cpu: CPU number
 *
 * This is the last known freq, without actually getting it from the driver.
 * Return value will be same as what is shown in scaling_cur_freq in sysfs.
 */
unsigned int cpufreq_quick_get(unsigned int cpu)
{
	struct cpufreq_policy *policy;
	unsigned int ret_freq = 0;
<<<<<<< HEAD

	if (cpufreq_driver && cpufreq_driver->setpolicy && cpufreq_driver->get)
		return cpufreq_driver->get(cpu);
=======
	unsigned long flags;

	read_lock_irqsave(&cpufreq_driver_lock, flags);

	if (cpufreq_driver && cpufreq_driver->setpolicy && cpufreq_driver->get) {
		ret_freq = cpufreq_driver->get(cpu);
		read_unlock_irqrestore(&cpufreq_driver_lock, flags);
		return ret_freq;
	}

	read_unlock_irqrestore(&cpufreq_driver_lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	policy = cpufreq_cpu_get(cpu);
	if (policy) {
		ret_freq = policy->cur;
		cpufreq_cpu_put(policy);
	}

	return ret_freq;
}
EXPORT_SYMBOL(cpufreq_quick_get);

/**
 * cpufreq_quick_get_max - get the max reported CPU frequency for this CPU
 * @cpu: CPU number
 *
 * Just return the max possible frequency for a given CPU.
 */
unsigned int cpufreq_quick_get_max(unsigned int cpu)
{
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);
	unsigned int ret_freq = 0;

	if (policy) {
		ret_freq = policy->max;
		cpufreq_cpu_put(policy);
	}

	return ret_freq;
}
EXPORT_SYMBOL(cpufreq_quick_get_max);

<<<<<<< HEAD

static unsigned int __cpufreq_get(unsigned int cpu)
{
	struct cpufreq_policy *policy = per_cpu(cpufreq_cpu_data, cpu);
	unsigned int ret_freq = 0;

	if (!cpufreq_driver->get)
		return ret_freq;

	ret_freq = cpufreq_driver->get(cpu);
=======
static unsigned int __cpufreq_get(struct cpufreq_policy *policy)
{
	unsigned int ret_freq = 0;

	if (unlikely(policy_is_inactive(policy)) || !cpufreq_driver->get)
		return ret_freq;

	ret_freq = cpufreq_driver->get(policy->cpu);

	/*
	 * If fast frequency switching is used with the given policy, the check
	 * against policy->cur is pointless, so skip it in that case too.
	 */
	if (policy->fast_switch_enabled)
		return ret_freq;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (ret_freq && policy->cur &&
		!(cpufreq_driver->flags & CPUFREQ_CONST_LOOPS)) {
		/* verify no discrepancy between actual and
					saved value exists */
		if (unlikely(ret_freq != policy->cur)) {
<<<<<<< HEAD
			cpufreq_out_of_sync(cpu, policy->cur, ret_freq);
=======
			cpufreq_out_of_sync(policy, ret_freq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			schedule_work(&policy->update);
		}
	}

	return ret_freq;
}

/**
 * cpufreq_get - get the current CPU frequency (in kHz)
 * @cpu: CPU number
 *
 * Get the CPU current (static) CPU frequency
 */
unsigned int cpufreq_get(unsigned int cpu)
{
<<<<<<< HEAD
	unsigned int ret_freq = 0;
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);

	if (!policy)
		goto out;

	if (unlikely(lock_policy_rwsem_read(cpu)))
		goto out_policy;

	ret_freq = __cpufreq_get(cpu);

	unlock_policy_rwsem_read(cpu);

out_policy:
	cpufreq_cpu_put(policy);
out:
	return ret_freq;
}
EXPORT_SYMBOL(cpufreq_get);
/*DTS2014091201709 songliangliang 20140902 begin */
unsigned int cpufreq_get_fb(unsigned int cpu)
{
	return cpufreq_get(cpu);

}
/*DTS2014091201709 songliangliang 20140902 end */
=======
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);
	unsigned int ret_freq = 0;

	if (policy) {
		down_read(&policy->rwsem);
		ret_freq = __cpufreq_get(policy);
		up_read(&policy->rwsem);

		cpufreq_cpu_put(policy);
	}

	return ret_freq;
}
EXPORT_SYMBOL(cpufreq_get);

static unsigned int cpufreq_update_current_freq(struct cpufreq_policy *policy)
{
	unsigned int new_freq;

	new_freq = cpufreq_driver->get(policy->cpu);
	if (!new_freq)
		return 0;

	if (!policy->cur) {
		pr_debug("cpufreq: Driver did not initialize current freq\n");
		policy->cur = new_freq;
	} else if (policy->cur != new_freq && has_target()) {
		cpufreq_out_of_sync(policy, new_freq);
	}

	return new_freq;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct subsys_interface cpufreq_interface = {
	.name		= "cpufreq",
	.subsys		= &cpu_subsys,
	.add_dev	= cpufreq_add_dev,
	.remove_dev	= cpufreq_remove_dev,
};

<<<<<<< HEAD
=======
/*
 * In case platform wants some specific frequency to be configured
 * during suspend..
 */
int cpufreq_generic_suspend(struct cpufreq_policy *policy)
{
	int ret;

	if (!policy->suspend_freq) {
		pr_debug("%s: suspend_freq not defined\n", __func__);
		return 0;
	}

	pr_debug("%s: Setting suspend-freq: %u\n", __func__,
			policy->suspend_freq);

	ret = __cpufreq_driver_target(policy, policy->suspend_freq,
			CPUFREQ_RELATION_H);
	if (ret)
		pr_err("%s: unable to set suspend-freq: %u. err: %d\n",
				__func__, policy->suspend_freq, ret);

	return ret;
}
EXPORT_SYMBOL(cpufreq_generic_suspend);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * cpufreq_suspend() - Suspend CPUFreq governors
 *
 * Called during system wide Suspend/Hibernate cycles for suspending governors
 * as some platforms can't change frequency after this point in suspend cycle.
 * Because some of the devices (like: i2c, regulators, etc) they use for
 * changing frequency are suspended quickly after this point.
 */
void cpufreq_suspend(void)
{
	struct cpufreq_policy *policy;
<<<<<<< HEAD
	int cpu;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!cpufreq_driver)
		return;

<<<<<<< HEAD
	if (!has_target())
		return;

	pr_debug("%s: Suspending Governors\n", __func__);

	for_each_possible_cpu(cpu) {
		if (!cpu_online(cpu))
			continue;

		policy = cpufreq_cpu_get(cpu);
		/* fix linaro cpufreq bugs */
#ifdef CONFIG_HISI_CPUFREQ
		if(!policy)
			continue;
#endif
		if (__cpufreq_governor(policy, CPUFREQ_GOV_STOP))
			pr_debug("%s: Failed to stop governor for policy: %p\n",
				__func__, policy);
		else if (cpufreq_driver->suspend
		    && cpufreq_driver->suspend(policy))
			pr_err("%s: Failed to suspend driver: %p\n", __func__,
				policy);

#ifdef CONFIG_HISI_CPUFREQ
		cpufreq_cpu_put(policy);
#endif
	}

=======
	if (!has_target() && !cpufreq_driver->suspend)
		goto suspend;

	pr_debug("%s: Suspending Governors\n", __func__);

	for_each_active_policy(policy) {
		if (has_target()) {
			down_write(&policy->rwsem);
			cpufreq_stop_governor(policy);
			up_write(&policy->rwsem);
		}

		if (cpufreq_driver->suspend && cpufreq_driver->suspend(policy))
			pr_err("%s: Failed to suspend driver: %p\n", __func__,
				policy);
	}

suspend:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cpufreq_suspended = true;
}

/**
 * cpufreq_resume() - Resume CPUFreq governors
 *
 * Called during system wide Suspend/Hibernate cycle for resuming governors that
 * are suspended with cpufreq_suspend().
 */
void cpufreq_resume(void)
{
	struct cpufreq_policy *policy;
<<<<<<< HEAD
	int cpu;
=======
	int ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!cpufreq_driver)
		return;

<<<<<<< HEAD
	if (!has_target())
		return;

	pr_debug("%s: Resuming Governors\n", __func__);

	cpufreq_suspended = false;

	for_each_possible_cpu(cpu) {
		if (!cpu_online(cpu))
			continue;

		policy = cpufreq_cpu_get(cpu);
#ifdef CONFIG_HISI_CPUFREQ
		if(!policy)
			continue;

		if (cpufreq_driver->resume
		    && cpufreq_driver->resume(policy))
			pr_err("%s: Failed to resume driver: %p\n", __func__,
				policy);
		else if (__cpufreq_governor(policy, CPUFREQ_GOV_START)
		    || __cpufreq_governor(policy, CPUFREQ_GOV_LIMITS))
			pr_debug("%s: Failed to start governor for policy: %p\n",
				__func__, policy);
#else
		if (__cpufreq_governor(policy, CPUFREQ_GOV_START)
		    || __cpufreq_governor(policy, CPUFREQ_GOV_LIMITS))
			pr_debug("%s: Failed to start governor for policy: %p\n",
				__func__, policy);
		else if (cpufreq_driver->resume
		    && cpufreq_driver->resume(policy))
			pr_err("%s: Failed to resume driver: %p\n", __func__,
				policy);
#endif

		/*
		 * schedule call cpufreq_update_policy() for boot CPU, i.e. last
		 * policy in list. It will verify that the current freq is in
		 * sync with what we believe it to be.
		 */
		if (cpu == 0)
			schedule_work(&policy->update);

#ifdef CONFIG_HISI_CPUFREQ
		cpufreq_cpu_put(policy);
#endif
=======
	if (unlikely(!cpufreq_suspended))
		return;

	cpufreq_suspended = false;

	if (!has_target() && !cpufreq_driver->resume)
		return;

	pr_debug("%s: Resuming Governors\n", __func__);

	for_each_active_policy(policy) {
		if (cpufreq_driver->resume && cpufreq_driver->resume(policy)) {
			pr_err("%s: Failed to resume driver: %p\n", __func__,
				policy);
		} else if (has_target()) {
			down_write(&policy->rwsem);
			ret = cpufreq_start_governor(policy);
			up_write(&policy->rwsem);

			if (ret)
				pr_err("%s: Failed to start governor for policy: %p\n",
				       __func__, policy);
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

/**
 *	cpufreq_get_current_driver - return current driver's name
 *
 *	Return the name string of the currently loaded cpufreq driver
 *	or NULL, if none.
 */
const char *cpufreq_get_current_driver(void)
{
	if (cpufreq_driver)
		return cpufreq_driver->name;

	return NULL;
}
EXPORT_SYMBOL_GPL(cpufreq_get_current_driver);

<<<<<<< HEAD
=======
/**
 *	cpufreq_get_driver_data - return current driver data
 *
 *	Return the private data of the currently loaded cpufreq
 *	driver, or NULL if no cpufreq driver is loaded.
 */
void *cpufreq_get_driver_data(void)
{
	if (cpufreq_driver)
		return cpufreq_driver->driver_data;

	return NULL;
}
EXPORT_SYMBOL_GPL(cpufreq_get_driver_data);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*********************************************************************
 *                     NOTIFIER LISTS INTERFACE                      *
 *********************************************************************/

/**
 *	cpufreq_register_notifier - register a driver with cpufreq
 *	@nb: notifier function to register
 *      @list: CPUFREQ_TRANSITION_NOTIFIER or CPUFREQ_POLICY_NOTIFIER
 *
 *	Add a driver to one of two lists: either a list of drivers that
 *      are notified about clock rate changes (once before and once after
 *      the transition), or a list of drivers that are notified about
 *      changes in cpufreq policy.
 *
 *	This function may sleep, and has the same return conditions as
 *	blocking_notifier_chain_register.
 */
int cpufreq_register_notifier(struct notifier_block *nb, unsigned int list)
{
	int ret;

	if (cpufreq_disabled())
		return -EINVAL;

	WARN_ON(!init_cpufreq_transition_notifier_list_called);

	switch (list) {
	case CPUFREQ_TRANSITION_NOTIFIER:
<<<<<<< HEAD
		ret = srcu_notifier_chain_register(
				&cpufreq_transition_notifier_list, nb);
=======
		mutex_lock(&cpufreq_fast_switch_lock);

		if (cpufreq_fast_switch_count > 0) {
			mutex_unlock(&cpufreq_fast_switch_lock);
			return -EBUSY;
		}
		ret = srcu_notifier_chain_register(
				&cpufreq_transition_notifier_list, nb);
		if (!ret)
			cpufreq_fast_switch_count--;

		mutex_unlock(&cpufreq_fast_switch_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	case CPUFREQ_POLICY_NOTIFIER:
		ret = blocking_notifier_chain_register(
				&cpufreq_policy_notifier_list, nb);
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}
EXPORT_SYMBOL(cpufreq_register_notifier);

<<<<<<< HEAD

/**
 *	cpufreq_unregister_notifier - unregister a driver with cpufreq
 *	@nb: notifier block to be unregistered
 *      @list: CPUFREQ_TRANSITION_NOTIFIER or CPUFREQ_POLICY_NOTIFIER
=======
/**
 *	cpufreq_unregister_notifier - unregister a driver with cpufreq
 *	@nb: notifier block to be unregistered
 *	@list: CPUFREQ_TRANSITION_NOTIFIER or CPUFREQ_POLICY_NOTIFIER
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 *	Remove a driver from the CPU frequency notifier list.
 *
 *	This function may sleep, and has the same return conditions as
 *	blocking_notifier_chain_unregister.
 */
int cpufreq_unregister_notifier(struct notifier_block *nb, unsigned int list)
{
	int ret;

<<<<<<< HEAD
	if (cpufreq_disabled())
		return -EINVAL;

	switch (list) {
	case CPUFREQ_TRANSITION_NOTIFIER:
		ret = srcu_notifier_chain_unregister(
				&cpufreq_transition_notifier_list, nb);
		break;
	case CPUFREQ_POLICY_NOTIFIER:
		ret = blocking_notifier_chain_unregister(
				&cpufreq_policy_notifier_list, nb);
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}
EXPORT_SYMBOL(cpufreq_unregister_notifier);


/*********************************************************************
 *                              GOVERNORS                            *
 *********************************************************************/

=======
	if (cpufreq_disabled())
		return -EINVAL;

	switch (list) {
	case CPUFREQ_TRANSITION_NOTIFIER:
		mutex_lock(&cpufreq_fast_switch_lock);

		ret = srcu_notifier_chain_unregister(
				&cpufreq_transition_notifier_list, nb);
		if (!ret && !WARN_ON(cpufreq_fast_switch_count >= 0))
			cpufreq_fast_switch_count++;

		mutex_unlock(&cpufreq_fast_switch_lock);
		break;
	case CPUFREQ_POLICY_NOTIFIER:
		ret = blocking_notifier_chain_unregister(
				&cpufreq_policy_notifier_list, nb);
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}
EXPORT_SYMBOL(cpufreq_unregister_notifier);


/*********************************************************************
 *                              GOVERNORS                            *
 *********************************************************************/

/**
 * cpufreq_driver_fast_switch - Carry out a fast CPU frequency switch.
 * @policy: cpufreq policy to switch the frequency for.
 * @target_freq: New frequency to set (may be approximate).
 *
 * Carry out a fast frequency switch without sleeping.
 *
 * The driver's ->fast_switch() callback invoked by this function must be
 * suitable for being called from within RCU-sched read-side critical sections
 * and it is expected to select the minimum available frequency greater than or
 * equal to @target_freq (CPUFREQ_RELATION_L).
 *
 * This function must not be called if policy->fast_switch_enabled is unset.
 *
 * Governors calling this function must guarantee that it will never be invoked
 * twice in parallel for the same policy and that it will never be called in
 * parallel with either ->target() or ->target_index() for the same policy.
 *
 * If CPUFREQ_ENTRY_INVALID is returned by the driver's ->fast_switch()
 * callback to indicate an error condition, the hardware configuration must be
 * preserved.
 */
unsigned int cpufreq_driver_fast_switch(struct cpufreq_policy *policy,
					unsigned int target_freq)
{
	target_freq = clamp_val(target_freq, policy->min, policy->max);

	return cpufreq_driver->fast_switch(policy, target_freq);
}
EXPORT_SYMBOL_GPL(cpufreq_driver_fast_switch);

/* Must set freqs->new to intermediate frequency */
static int __target_intermediate(struct cpufreq_policy *policy,
				 struct cpufreq_freqs *freqs, int index)
{
	int ret;

	freqs->new = cpufreq_driver->get_intermediate(policy, index);

	/* We don't need to switch to intermediate freq */
	if (!freqs->new)
		return 0;

	pr_debug("%s: cpu: %d, switching to intermediate freq: oldfreq: %u, intermediate freq: %u\n",
		 __func__, policy->cpu, freqs->old, freqs->new);

	cpufreq_freq_transition_begin(policy, freqs);
	ret = cpufreq_driver->target_intermediate(policy, index);
	cpufreq_freq_transition_end(policy, freqs, ret);

	if (ret)
		pr_err("%s: Failed to change to intermediate frequency: %d\n",
		       __func__, ret);

	return ret;
}

static int __target_index(struct cpufreq_policy *policy, int index)
{
	struct cpufreq_freqs freqs = {.old = policy->cur, .flags = 0};
	unsigned int intermediate_freq = 0;
	unsigned int newfreq = policy->freq_table[index].frequency;
	int retval = -EINVAL;
	bool notify;

	if (newfreq == policy->cur)
		return 0;

	notify = !(cpufreq_driver->flags & CPUFREQ_ASYNC_NOTIFICATION);
	if (notify) {
		/* Handle switching to intermediate frequency */
		if (cpufreq_driver->get_intermediate) {
			retval = __target_intermediate(policy, &freqs, index);
			if (retval)
				return retval;

			intermediate_freq = freqs.new;
			/* Set old freq to intermediate */
			if (intermediate_freq)
				freqs.old = freqs.new;
		}

		freqs.new = newfreq;
		pr_debug("%s: cpu: %d, oldfreq: %u, new freq: %u\n",
			 __func__, policy->cpu, freqs.old, freqs.new);

		cpufreq_freq_transition_begin(policy, &freqs);
	}

	retval = cpufreq_driver->target_index(policy, index);
	if (retval)
		pr_err("%s: Failed to change cpu frequency: %d\n", __func__,
		       retval);

	if (notify) {
		cpufreq_freq_transition_end(policy, &freqs, retval);

		/*
		 * Failed after setting to intermediate freq? Driver should have
		 * reverted back to initial frequency and so should we. Check
		 * here for intermediate_freq instead of get_intermediate, in
		 * case we haven't switched to intermediate freq at all.
		 */
		if (unlikely(retval && intermediate_freq)) {
			freqs.old = intermediate_freq;
			freqs.new = policy->restore_freq;
			cpufreq_freq_transition_begin(policy, &freqs);
			cpufreq_freq_transition_end(policy, &freqs, 0);
		}
	}

	return retval;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int __cpufreq_driver_target(struct cpufreq_policy *policy,
			    unsigned int target_freq,
			    unsigned int relation)
{
<<<<<<< HEAD
	int retval = -EINVAL;
	unsigned int old_target_freq = target_freq;
=======
	unsigned int old_target_freq = target_freq;
	int index;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (cpufreq_disabled())
		return -ENODEV;

	/* Make sure that target_freq is within supported range */
<<<<<<< HEAD
	if (target_freq > policy->max)
		target_freq = policy->max;
	if (target_freq < policy->min)
		target_freq = policy->min;

	pr_debug("target for CPU %u: %u kHz, relation %u, requested %u kHz\n",
			policy->cpu, target_freq, relation, old_target_freq);

	if (target_freq == policy->cur)
		return 0;

	if (cpufreq_driver->target)
		retval = cpufreq_driver->target(policy, target_freq, relation);

	return retval;
=======
	target_freq = clamp_val(target_freq, policy->min, policy->max);

	pr_debug("target for CPU %u: %u kHz, relation %u, requested %u kHz\n",
		 policy->cpu, target_freq, relation, old_target_freq);

	/*
	 * This might look like a redundant call as we are checking it again
	 * after finding index. But it is left intentionally for cases where
	 * exactly same freq is called again and so we can save on few function
	 * calls.
	 */
	if (target_freq == policy->cur)
		return 0;

	/* Save last value to restore later on errors */
	policy->restore_freq = policy->cur;

	if (cpufreq_driver->target)
		return cpufreq_driver->target(policy, target_freq, relation);

	if (!cpufreq_driver->target_index)
		return -EINVAL;

	index = cpufreq_frequency_table_target(policy, target_freq, relation);

	return __target_index(policy, index);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(__cpufreq_driver_target);

int cpufreq_driver_target(struct cpufreq_policy *policy,
			  unsigned int target_freq,
			  unsigned int relation)
{
	int ret = -EINVAL;

<<<<<<< HEAD
	policy = cpufreq_cpu_get(policy->cpu);
	if (!policy)
		goto no_policy;

	if (unlikely(lock_policy_rwsem_write(policy->cpu)))
		goto fail;

	ret = __cpufreq_driver_target(policy, target_freq, relation);

	unlock_policy_rwsem_write(policy->cpu);

fail:
	cpufreq_cpu_put(policy);
no_policy:
=======
	down_write(&policy->rwsem);

	ret = __cpufreq_driver_target(policy, target_freq, relation);

	up_write(&policy->rwsem);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}
EXPORT_SYMBOL_GPL(cpufreq_driver_target);

<<<<<<< HEAD
int __cpufreq_driver_getavg(struct cpufreq_policy *policy, unsigned int cpu)
{
	int ret = 0;

	if (cpufreq_disabled())
		return ret;

	if (!cpufreq_driver->getavg)
		return 0;

	policy = cpufreq_cpu_get(policy->cpu);
	if (!policy)
		return -EINVAL;

	ret = cpufreq_driver->getavg(policy, cpu);

	cpufreq_cpu_put(policy);
	return ret;
}
EXPORT_SYMBOL_GPL(__cpufreq_driver_getavg);

/*
 * when "event" is CPUFREQ_GOV_LIMITS
 */

static int __cpufreq_governor(struct cpufreq_policy *policy,
					unsigned int event)
{
	int ret;

	/* Only must be defined when default governor is known to have latency
	   restrictions, like e.g. conservative or ondemand.
	   That this is the case is already ensured in Kconfig
	*/
#ifdef CONFIG_CPU_FREQ_GOV_PERFORMANCE
	struct cpufreq_governor *gov = &cpufreq_gov_performance;
#else
	struct cpufreq_governor *gov = NULL;
#endif

#ifndef CONFIG_HISI_CPUFREQ
	/* Don't start any governor operations if we are entering suspend */
	if (cpufreq_suspended)
		return 0;
#endif
=======
__weak struct cpufreq_governor *cpufreq_fallback_governor(void)
{
	return NULL;
}

static int cpufreq_init_governor(struct cpufreq_policy *policy)
{
	int ret;

	/* Don't start any governor operations if we are entering suspend */
	if (cpufreq_suspended)
		return 0;
	/*
	 * Governor might not be initiated here if ACPI _PPC changed
	 * notification happened, so check it.
	 */
	if (!policy->governor)
		return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (policy->governor->max_transition_latency &&
	    policy->cpuinfo.transition_latency >
	    policy->governor->max_transition_latency) {
<<<<<<< HEAD
		if (!gov)
			return -EINVAL;
		else {
			printk(KERN_WARNING "%s governor failed, too long"
			       " transition latency of HW, fallback"
			       " to %s governor\n",
			       policy->governor->name,
			       gov->name);
			policy->governor = gov;
=======
		struct cpufreq_governor *gov = cpufreq_fallback_governor();

		if (gov) {
			pr_warn("%s governor failed, too long transition latency of HW, fallback to %s governor\n",
				policy->governor->name, gov->name);
			policy->governor = gov;
		} else {
			return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	if (!try_module_get(policy->governor->owner))
		return -EINVAL;

<<<<<<< HEAD
	pr_debug("__cpufreq_governor for CPU %u, event %u\n",
						policy->cpu, event);

	mutex_lock(&cpufreq_governor_lock);
	if ((!policy->governor_enabled && (event == CPUFREQ_GOV_STOP)) ||
	    (policy->governor_enabled && (event == CPUFREQ_GOV_START))) {
		mutex_unlock(&cpufreq_governor_lock);
		return -EBUSY;
	}

	if (event == CPUFREQ_GOV_STOP)
		policy->governor_enabled = false;
	else if (event == CPUFREQ_GOV_START)
		policy->governor_enabled = true;

	mutex_unlock(&cpufreq_governor_lock);

	ret = policy->governor->governor(policy, event);

	if (!ret) {
		if (event == CPUFREQ_GOV_POLICY_INIT)
			policy->governor->initialized++;
		else if (event == CPUFREQ_GOV_POLICY_EXIT)
			policy->governor->initialized--;
	} else {
		/* Restore original values */
		mutex_lock(&cpufreq_governor_lock);
		if (event == CPUFREQ_GOV_STOP)
			policy->governor_enabled = true;
		else if (event == CPUFREQ_GOV_START)
			policy->governor_enabled = false;
		mutex_unlock(&cpufreq_governor_lock);
	}

	/* we keep one module reference alive for
			each CPU governed by this CPU */
	if ((event != CPUFREQ_GOV_START) || ret)
		module_put(policy->governor->owner);
	if ((event == CPUFREQ_GOV_STOP) && !ret)
		module_put(policy->governor->owner);

	return ret;
}

=======
	pr_debug("%s: for CPU %u\n", __func__, policy->cpu);

	if (policy->governor->init) {
		ret = policy->governor->init(policy);
		if (ret) {
			module_put(policy->governor->owner);
			return ret;
		}
	}

	return 0;
}

static void cpufreq_exit_governor(struct cpufreq_policy *policy)
{
	if (cpufreq_suspended || !policy->governor)
		return;

	pr_debug("%s: for CPU %u\n", __func__, policy->cpu);

	if (policy->governor->exit)
		policy->governor->exit(policy);

	module_put(policy->governor->owner);
}

static int cpufreq_start_governor(struct cpufreq_policy *policy)
{
	int ret;

	if (cpufreq_suspended)
		return 0;

	if (!policy->governor)
		return -EINVAL;

	pr_debug("%s: for CPU %u\n", __func__, policy->cpu);

	if (cpufreq_driver->get && !cpufreq_driver->setpolicy)
		cpufreq_update_current_freq(policy);

	if (policy->governor->start) {
		ret = policy->governor->start(policy);
		if (ret)
			return ret;
	}

	if (policy->governor->limits)
		policy->governor->limits(policy);

	return 0;
}

static void cpufreq_stop_governor(struct cpufreq_policy *policy)
{
	if (cpufreq_suspended || !policy->governor)
		return;

	pr_debug("%s: for CPU %u\n", __func__, policy->cpu);

	if (policy->governor->stop)
		policy->governor->stop(policy);
}

static void cpufreq_governor_limits(struct cpufreq_policy *policy)
{
	if (cpufreq_suspended || !policy->governor)
		return;

	pr_debug("%s: for CPU %u\n", __func__, policy->cpu);

	if (policy->governor->limits)
		policy->governor->limits(policy);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int cpufreq_register_governor(struct cpufreq_governor *governor)
{
	int err;

	if (!governor)
		return -EINVAL;

	if (cpufreq_disabled())
		return -ENODEV;

	mutex_lock(&cpufreq_governor_mutex);

<<<<<<< HEAD
	governor->initialized = 0;
	err = -EBUSY;
	if (__find_governor(governor->name) == NULL) {
=======
	err = -EBUSY;
	if (!find_governor(governor->name)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		err = 0;
		list_add(&governor->governor_list, &cpufreq_governor_list);
	}

	mutex_unlock(&cpufreq_governor_mutex);
	return err;
}
EXPORT_SYMBOL_GPL(cpufreq_register_governor);

<<<<<<< HEAD

void cpufreq_unregister_governor(struct cpufreq_governor *governor)
{
#ifdef CONFIG_HOTPLUG_CPU
	int cpu;
#endif
=======
void cpufreq_unregister_governor(struct cpufreq_governor *governor)
{
	struct cpufreq_policy *policy;
	unsigned long flags;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!governor)
		return;

	if (cpufreq_disabled())
		return;

<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU
	for_each_present_cpu(cpu) {
		if (cpu_online(cpu))
			continue;
		if (!strcmp(per_cpu(cpufreq_cpu_governor, cpu), governor->name))
			strcpy(per_cpu(cpufreq_cpu_governor, cpu), "\0");
	}
#endif
=======
	/* clear last_governor for all inactive policies */
	read_lock_irqsave(&cpufreq_driver_lock, flags);
	for_each_inactive_policy(policy) {
		if (!strcmp(policy->last_governor, governor->name)) {
			policy->governor = NULL;
			strcpy(policy->last_governor, "\0");
		}
	}
	read_unlock_irqrestore(&cpufreq_driver_lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	mutex_lock(&cpufreq_governor_mutex);
	list_del(&governor->governor_list);
	mutex_unlock(&cpufreq_governor_mutex);
	return;
}
EXPORT_SYMBOL_GPL(cpufreq_unregister_governor);


<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*********************************************************************
 *                          POLICY INTERFACE                         *
 *********************************************************************/

/**
 * cpufreq_get_policy - get the current cpufreq_policy
 * @policy: struct cpufreq_policy into which the current cpufreq_policy
 *	is written
 *
 * Reads the current cpufreq policy.
 */
int cpufreq_get_policy(struct cpufreq_policy *policy, unsigned int cpu)
{
	struct cpufreq_policy *cpu_policy;
	if (!policy)
		return -EINVAL;

	cpu_policy = cpufreq_cpu_get(cpu);
	if (!cpu_policy)
		return -EINVAL;

<<<<<<< HEAD
	memcpy(policy, cpu_policy, sizeof(struct cpufreq_policy));
=======
	memcpy(policy, cpu_policy, sizeof(*policy));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	cpufreq_cpu_put(cpu_policy);
	return 0;
}
EXPORT_SYMBOL(cpufreq_get_policy);

<<<<<<< HEAD

/*
 * data   : current policy.
 * policy : policy to be set.
 */
static int __cpufreq_set_policy(struct cpufreq_policy *data,
				struct cpufreq_policy *policy)
{
	int ret = 0, failed = 1;

	pr_debug("setting new policy for CPU %u: %u - %u kHz\n", policy->cpu,
		policy->min, policy->max);

	memcpy(&policy->cpuinfo, &data->cpuinfo,
				sizeof(struct cpufreq_cpuinfo));

	if (policy->min > data->max || policy->max < data->min) {
		ret = -EINVAL;
		goto error_out;
	}

	/* verify the cpu speed can be set within this limit */
	ret = cpufreq_driver->verify(policy);
	if (ret)
		goto error_out;

	/* adjust if necessary - all reasons */
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
			CPUFREQ_ADJUST, policy);

	/* adjust if necessary - hardware incompatibility*/
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
			CPUFREQ_INCOMPATIBLE, policy);

	/* verify the cpu speed can be set within this limit,
	   which might be different to the first one */
	ret = cpufreq_driver->verify(policy);
	if (ret)
		goto error_out;

	/* notification of the new policy */
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
			CPUFREQ_NOTIFY, policy);

	data->min = policy->min;
	data->max = policy->max;

	pr_debug("new min and max freqs are %u - %u kHz\n",
					data->min, data->max);

	if (cpufreq_driver->setpolicy) {
		data->policy = policy->policy;
		pr_debug("setting range\n");
		ret = cpufreq_driver->setpolicy(policy);
	} else {
		if (policy->governor != data->governor) {
			/* save old, working values */
			struct cpufreq_governor *old_gov = data->governor;

			pr_debug("governor switch\n");

			/* end old governor */
			if (data->governor) {
				__cpufreq_governor(data, CPUFREQ_GOV_STOP);
				unlock_policy_rwsem_write(policy->cpu);
				__cpufreq_governor(data,
						CPUFREQ_GOV_POLICY_EXIT);
				lock_policy_rwsem_write(policy->cpu);
			}

			/* start new governor */
			data->governor = policy->governor;
			if (!__cpufreq_governor(data, CPUFREQ_GOV_POLICY_INIT)) {
				if (!__cpufreq_governor(data, CPUFREQ_GOV_START)) {
					failed = 0;
				} else {
					unlock_policy_rwsem_write(policy->cpu);
					__cpufreq_governor(data,
							CPUFREQ_GOV_POLICY_EXIT);
					lock_policy_rwsem_write(policy->cpu);
				}
			}

			if (failed) {
				/* new governor failed, so re-start old one */
				pr_debug("starting governor %s failed\n",
							data->governor->name);
				if (old_gov) {
					data->governor = old_gov;
					__cpufreq_governor(data,
							CPUFREQ_GOV_POLICY_INIT);
					__cpufreq_governor(data,
							   CPUFREQ_GOV_START);
				}
				ret = -EINVAL;
				goto error_out;
			}
			/* might be a policy change, too, so fall through */
		}
		pr_debug("governor: change or update limits\n");
		__cpufreq_governor(data, CPUFREQ_GOV_LIMITS);
	}

error_out:
=======
/*
 * policy : current policy.
 * new_policy: policy to be set.
 */
static int cpufreq_set_policy(struct cpufreq_policy *policy,
				struct cpufreq_policy *new_policy)
{
	struct cpufreq_governor *old_gov;
	int ret;

	pr_debug("setting new policy for CPU %u: %u - %u kHz\n",
		 new_policy->cpu, new_policy->min, new_policy->max);

	memcpy(&new_policy->cpuinfo, &policy->cpuinfo, sizeof(policy->cpuinfo));

	/*
	* This check works well when we store new min/max freq attributes,
	* because new_policy is a copy of policy with one field updated.
	*/
	if (new_policy->min > new_policy->max)
		return -EINVAL;

	/* verify the cpu speed can be set within this limit */
	ret = cpufreq_driver->verify(new_policy);
	if (ret)
		return ret;

	/* adjust if necessary - all reasons */
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
			CPUFREQ_ADJUST, new_policy);

	/*
	 * verify the cpu speed can be set within this limit, which might be
	 * different to the first one
	 */
	ret = cpufreq_driver->verify(new_policy);
	if (ret)
		return ret;

	/* notification of the new policy */
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
			CPUFREQ_NOTIFY, new_policy);

	scale_freq_capacity(new_policy, NULL);

	policy->min = new_policy->min;
	policy->max = new_policy->max;
	trace_cpu_frequency_limits(policy->max, policy->min, policy->cpu);

	policy->cached_target_freq = UINT_MAX;

	pr_debug("new min and max freqs are %u - %u kHz\n",
		 policy->min, policy->max);

	if (cpufreq_driver->setpolicy) {
		policy->policy = new_policy->policy;
		pr_debug("setting range\n");
		return cpufreq_driver->setpolicy(new_policy);
	}

	if (new_policy->governor == policy->governor) {
		pr_debug("cpufreq: governor limits update\n");
		cpufreq_governor_limits(policy);
		return 0;
	}

	pr_debug("governor switch\n");

	/* save old, working values */
	old_gov = policy->governor;
	/* end old governor */
	if (old_gov) {
		cpufreq_stop_governor(policy);
		cpufreq_exit_governor(policy);
	}

	/* start new governor */
	policy->governor = new_policy->governor;
	ret = cpufreq_init_governor(policy);
	if (!ret) {
		ret = cpufreq_start_governor(policy);
		if (!ret) {
			pr_debug("cpufreq: governor change\n");
			return 0;
		}
		cpufreq_exit_governor(policy);
	}

	/* new governor failed, so re-start old one */
	pr_debug("starting governor %s failed\n", policy->governor->name);
	if (old_gov) {
		policy->governor = old_gov;
		if (cpufreq_init_governor(policy))
			policy->governor = NULL;
		else
			cpufreq_start_governor(policy);
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

/**
 *	cpufreq_update_policy - re-evaluate an existing cpufreq policy
 *	@cpu: CPU which shall be re-evaluated
 *
 *	Useful for policy notifiers which have different necessities
 *	at different times.
 */
int cpufreq_update_policy(unsigned int cpu)
{
<<<<<<< HEAD
	struct cpufreq_policy *data = cpufreq_cpu_get(cpu);
	struct cpufreq_policy policy;
	int ret;

	if (!data) {
		ret = -ENODEV;
		goto no_policy;
	}

	if (unlikely(lock_policy_rwsem_write(cpu))) {
		ret = -EINVAL;
		goto fail;
	}

	pr_debug("updating policy for CPU %u\n", cpu);
	memcpy(&policy, data, sizeof(struct cpufreq_policy));
	policy.min = data->user_policy.min;
	policy.max = data->user_policy.max;
	policy.policy = data->user_policy.policy;
	policy.governor = data->user_policy.governor;

	/* BIOS might change freq behind our back
	  -> ask driver for current freq and notify governors about a change */
	if (cpufreq_driver->get) {
		policy.cur = cpufreq_driver->get(cpu);
		if (!data->cur) {
			pr_debug("Driver did not initialize current freq");
			data->cur = policy.cur;
		} else {
			if (data->cur != policy.cur && cpufreq_driver->target)
				cpufreq_out_of_sync(cpu, data->cur,
								policy.cur);
		}
	}

	ret = __cpufreq_set_policy(data, &policy);

	unlock_policy_rwsem_write(cpu);

fail:
	cpufreq_cpu_put(data);
no_policy:
=======
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);
	struct cpufreq_policy new_policy;
	int ret;

	if (!policy)
		return -ENODEV;

	down_write(&policy->rwsem);

	pr_debug("updating policy for CPU %u\n", cpu);
	memcpy(&new_policy, policy, sizeof(*policy));
	new_policy.min = policy->user_policy.min;
	new_policy.max = policy->user_policy.max;

	/*
	 * BIOS might change freq behind our back
	 * -> ask driver for current freq and notify governors about a change
	 */
	if (cpufreq_driver->get && !cpufreq_driver->setpolicy) {
		if (cpufreq_suspended) {
			ret = -EAGAIN;
			goto unlock;
		}
		new_policy.cur = cpufreq_update_current_freq(policy);
		if (WARN_ON(!new_policy.cur)) {
			ret = -EIO;
			goto unlock;
		}
	}

	ret = cpufreq_set_policy(policy, &new_policy);

unlock:
	up_write(&policy->rwsem);

	cpufreq_cpu_put(policy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}
EXPORT_SYMBOL(cpufreq_update_policy);

<<<<<<< HEAD
static int __cpuinit cpufreq_cpu_callback(struct notifier_block *nfb,
					unsigned long action, void *hcpu)
{
	unsigned int cpu = (unsigned long)hcpu;
	struct device *dev;

	dev = get_cpu_device(cpu);
	if (dev) {
		switch (action) {
		case CPU_ONLINE:
		case CPU_ONLINE_FROZEN:
			cpufreq_add_dev(dev, NULL);
			//kobject_uevent(&dev->kobj, KOBJ_ADD);
			cpufreq_update_policy(cpu);
			break;
		case CPU_DOWN_PREPARE:
		case CPU_DOWN_PREPARE_FROZEN:
			__cpufreq_remove_dev(dev, NULL);
			break;
		case CPU_DOWN_FAILED:
		case CPU_DOWN_FAILED_FROZEN:
			cpufreq_add_dev(dev, NULL);
			break;
		}
	}
	return NOTIFY_OK;
}

static struct notifier_block __refdata cpufreq_cpu_notifier = {
    .notifier_call = cpufreq_cpu_callback,
};
=======
/*********************************************************************
 *               BOOST						     *
 *********************************************************************/
static int cpufreq_boost_set_sw(int state)
{
	struct cpufreq_policy *policy;
	int ret = -EINVAL;

	for_each_active_policy(policy) {
		if (!policy->freq_table)
			continue;

		ret = cpufreq_frequency_table_cpuinfo(policy,
						      policy->freq_table);
		if (ret) {
			pr_err("%s: Policy frequency update failed\n",
			       __func__);
			break;
		}

		down_write(&policy->rwsem);
		policy->user_policy.max = policy->max;
		cpufreq_governor_limits(policy);
		up_write(&policy->rwsem);
	}

	return ret;
}

int cpufreq_boost_trigger_state(int state)
{
	unsigned long flags;
	int ret = 0;

	if (cpufreq_driver->boost_enabled == state)
		return 0;

	write_lock_irqsave(&cpufreq_driver_lock, flags);
	cpufreq_driver->boost_enabled = state;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	ret = cpufreq_driver->set_boost(state);
	if (ret) {
		write_lock_irqsave(&cpufreq_driver_lock, flags);
		cpufreq_driver->boost_enabled = !state;
		write_unlock_irqrestore(&cpufreq_driver_lock, flags);

		pr_err("%s: Cannot %s BOOST\n",
		       __func__, state ? "enable" : "disable");
	}

	return ret;
}

static bool cpufreq_boost_supported(void)
{
	return likely(cpufreq_driver) && cpufreq_driver->set_boost;
}

static int create_boost_sysfs_file(void)
{
	int ret;

	ret = sysfs_create_file(cpufreq_global_kobject, &boost.attr);
	if (ret)
		pr_err("%s: cannot register global BOOST sysfs file\n",
		       __func__);

	return ret;
}

static void remove_boost_sysfs_file(void)
{
	if (cpufreq_boost_supported())
		sysfs_remove_file(cpufreq_global_kobject, &boost.attr);
}

int cpufreq_enable_boost_support(void)
{
	if (!cpufreq_driver)
		return -EINVAL;

	if (cpufreq_boost_supported())
		return 0;

	cpufreq_driver->set_boost = cpufreq_boost_set_sw;

	/* This will get removed on driver unregister */
	return create_boost_sysfs_file();
}
EXPORT_SYMBOL_GPL(cpufreq_enable_boost_support);

int cpufreq_boost_enabled(void)
{
	return cpufreq_driver->boost_enabled;
}
EXPORT_SYMBOL_GPL(cpufreq_boost_enabled);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*********************************************************************
 *               REGISTER / UNREGISTER CPUFREQ DRIVER                *
 *********************************************************************/
<<<<<<< HEAD
=======
static enum cpuhp_state hp_online;

static int cpuhp_cpufreq_online(unsigned int cpu)
{
	cpufreq_online(cpu);

	return 0;
}

static int cpuhp_cpufreq_offline(unsigned int cpu)
{
	cpufreq_offline(cpu);

	return 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * cpufreq_register_driver - register a CPU Frequency driver
 * @driver_data: A struct cpufreq_driver containing the values#
 * submitted by the CPU Frequency driver.
 *
<<<<<<< HEAD
 *   Registers a CPU Frequency driver to this core code. This code
 * returns zero on success, -EBUSY when another driver got here first
=======
 * Registers a CPU Frequency driver to this core code. This code
 * returns zero on success, -EEXIST when another driver got here first
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * (and isn't unregistered in the meantime).
 *
 */
int cpufreq_register_driver(struct cpufreq_driver *driver_data)
{
	unsigned long flags;
	int ret;

	if (cpufreq_disabled())
		return -ENODEV;

<<<<<<< HEAD
	if (!driver_data || !driver_data->verify || !driver_data->init ||
	    ((!driver_data->setpolicy) && (!driver_data->target)))
=======
	/*
	 * The cpufreq core depends heavily on the availability of device
	 * structure, make sure they are available before proceeding further.
	 */
	if (!get_cpu_device(0))
		return -EPROBE_DEFER;

	if (!driver_data || !driver_data->verify || !driver_data->init ||
	    !(driver_data->setpolicy || driver_data->target_index ||
		    driver_data->target) ||
	     (driver_data->setpolicy && (driver_data->target_index ||
		    driver_data->target)) ||
	     (!!driver_data->get_intermediate != !!driver_data->target_intermediate))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;

	pr_debug("trying to register driver %s\n", driver_data->name);

<<<<<<< HEAD
	if (driver_data->setpolicy)
		driver_data->flags |= CPUFREQ_CONST_LOOPS;
=======
	/* Protect against concurrent CPU online/offline. */
	get_online_cpus();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	write_lock_irqsave(&cpufreq_driver_lock, flags);
	if (cpufreq_driver) {
		write_unlock_irqrestore(&cpufreq_driver_lock, flags);
<<<<<<< HEAD
		return -EBUSY;
=======
		ret = -EEXIST;
		goto out;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	cpufreq_driver = driver_data;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

<<<<<<< HEAD
	ret = subsys_interface_register(&cpufreq_interface);
	if (ret)
		goto err_null_driver;

	if (!(cpufreq_driver->flags & CPUFREQ_STICKY)) {
		int i;
		ret = -ENODEV;

		/* check for at least one working CPU */
		for (i = 0; i < nr_cpu_ids; i++)
			if (cpu_possible(i) && per_cpu(cpufreq_cpu_data, i)) {
				ret = 0;
				break;
			}

		/* if all ->init() calls failed, unregister */
		if (ret) {
			pr_debug("no CPU initialized for driver %s\n",
							driver_data->name);
			goto err_if_unreg;
		}
	}

	register_hotcpu_notifier(&cpufreq_cpu_notifier);
	pr_debug("driver %s up and running\n", driver_data->name);

	return 0;
err_if_unreg:
	subsys_interface_unregister(&cpufreq_interface);
=======
	if (driver_data->setpolicy)
		driver_data->flags |= CPUFREQ_CONST_LOOPS;

	if (cpufreq_boost_supported()) {
		ret = create_boost_sysfs_file();
		if (ret)
			goto err_null_driver;
	}

	ret = subsys_interface_register(&cpufreq_interface);
	if (ret)
		goto err_boost_unreg;

	if (!(cpufreq_driver->flags & CPUFREQ_STICKY) &&
	    list_empty(&cpufreq_policy_list)) {
		/* if all ->init() calls failed, unregister */
		ret = -ENODEV;
		pr_debug("%s: No CPU initialized for driver %s\n", __func__,
			 driver_data->name);
		goto err_if_unreg;
	}

	ret = cpuhp_setup_state_nocalls(CPUHP_AP_ONLINE_DYN, "cpufreq:online",
					cpuhp_cpufreq_online,
					cpuhp_cpufreq_offline);
	if (ret < 0)
		goto err_if_unreg;
	hp_online = ret;
	ret = 0;

	pr_debug("driver %s up and running\n", driver_data->name);
	goto out;

err_if_unreg:
	subsys_interface_unregister(&cpufreq_interface);
err_boost_unreg:
	remove_boost_sysfs_file();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
err_null_driver:
	write_lock_irqsave(&cpufreq_driver_lock, flags);
	cpufreq_driver = NULL;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);
<<<<<<< HEAD
=======
out:
	put_online_cpus();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}
EXPORT_SYMBOL_GPL(cpufreq_register_driver);

<<<<<<< HEAD

/**
 * cpufreq_unregister_driver - unregister the current CPUFreq driver
 *
 *    Unregister the current CPUFreq driver. Only call this if you have
=======
/**
 * cpufreq_unregister_driver - unregister the current CPUFreq driver
 *
 * Unregister the current CPUFreq driver. Only call this if you have
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * the right to do so, i.e. if you have succeeded in initialising before!
 * Returns zero if successful, and -EINVAL if the cpufreq_driver is
 * currently not initialised.
 */
int cpufreq_unregister_driver(struct cpufreq_driver *driver)
{
	unsigned long flags;

	if (!cpufreq_driver || (driver != cpufreq_driver))
		return -EINVAL;

	pr_debug("unregistering driver %s\n", driver->name);

<<<<<<< HEAD
	subsys_interface_unregister(&cpufreq_interface);
	unregister_hotcpu_notifier(&cpufreq_cpu_notifier);

	write_lock_irqsave(&cpufreq_driver_lock, flags);
	cpufreq_driver = NULL;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);
=======
	/* Protect against concurrent cpu hotplug */
	get_online_cpus();
	subsys_interface_unregister(&cpufreq_interface);
	remove_boost_sysfs_file();
	cpuhp_remove_state_nocalls(hp_online);

	write_lock_irqsave(&cpufreq_driver_lock, flags);

	cpufreq_driver = NULL;

	write_unlock_irqrestore(&cpufreq_driver_lock, flags);
	put_online_cpus();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}
EXPORT_SYMBOL_GPL(cpufreq_unregister_driver);

<<<<<<< HEAD
static int __init cpufreq_core_init(void)
{
	int cpu;

	if (cpufreq_disabled())
		return -ENODEV;

	for_each_possible_cpu(cpu) {
		per_cpu(cpufreq_policy_cpu, cpu) = -1;
		init_rwsem(&per_cpu(cpu_policy_rwsem, cpu));
	}

=======
struct kobject *cpufreq_global_kobject;
EXPORT_SYMBOL(cpufreq_global_kobject);

static int __init cpufreq_core_init(void)
{
	if (cpufreq_disabled())
		return -ENODEV;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cpufreq_global_kobject = kobject_create_and_add("cpufreq", &cpu_subsys.dev_root->kobj);
	BUG_ON(!cpufreq_global_kobject);

	return 0;
}
core_initcall(cpufreq_core_init);
