/*
 *  kernel/sched/core.c
 *
 *  Kernel scheduler and related syscalls
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *
 *  1996-12-23  Modified by Dave Grothe to fix bugs in semaphores and
 *		make semaphores SMP safe
 *  1998-11-19	Implemented schedule_timeout() and related stuff
 *		by Andrea Arcangeli
 *  2002-01-04	New ultra-scalable O(1) scheduler by Ingo Molnar:
 *		hybrid priority-list and round-robin design with
 *		an array-switch method of distributing timeslices
 *		and per-CPU runqueues.  Cleanups and useful suggestions
 *		by Davide Libenzi, preemptible kernel bits by Robert Love.
 *  2003-09-03	Interactivity tuning by Con Kolivas.
 *  2004-04-02	Scheduler domains code by Nick Piggin
 *  2007-04-15  Work begun on replacing all interactivity tuning with a
 *              fair scheduling design by Con Kolivas.
 *  2007-05-05  Load balancing (smp-nice) and other improvements
 *              by Peter Williams
 *  2007-05-06  Interactivity improvements to CFS by Mike Galbraith
 *  2007-07-01  Group scheduling enhancements by Srivatsa Vaddagiri
 *  2007-11-29  RT balancing improvements by Steven Rostedt, Gregory Haskins,
 *              Thomas Gleixner, Mike Kravetz
 */

<<<<<<< HEAD
=======
#include <linux/kasan.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/nmi.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/highmem.h>
<<<<<<< HEAD
#include <asm/mmu_context.h>
=======
#include <linux/mmu_context.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/interrupt.h>
#include <linux/capability.h>
#include <linux/completion.h>
#include <linux/kernel_stat.h>
#include <linux/debug_locks.h>
#include <linux/perf_event.h>
#include <linux/security.h>
#include <linux/notifier.h>
#include <linux/profile.h>
#include <linux/freezer.h>
#include <linux/vmalloc.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/pid_namespace.h>
#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/timer.h>
#include <linux/rcupdate.h>
#include <linux/cpu.h>
#include <linux/cpuset.h>
#include <linux/percpu.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sysctl.h>
#include <linux/syscalls.h>
#include <linux/times.h>
#include <linux/tsacct_kern.h>
#include <linux/kprobes.h>
#include <linux/delayacct.h>
#include <linux/unistd.h>
#include <linux/pagemap.h>
#include <linux/hrtimer.h>
#include <linux/tick.h>
<<<<<<< HEAD
#include <linux/debugfs.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/ctype.h>
#include <linux/ftrace.h>
#include <linux/slab.h>
#include <linux/init_task.h>
<<<<<<< HEAD
#include <linux/binfmts.h>
#include <linux/context_tracking.h>
=======
#include <linux/context_tracking.h>
#include <linux/compiler.h>
#include <linux/frame.h>
#include <linux/prefetch.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/switch_to.h>
#include <asm/tlb.h>
#include <asm/irq_regs.h>
#include <asm/mutex.h>
#ifdef CONFIG_PARAVIRT
#include <asm/paravirt.h>
#endif
<<<<<<< HEAD
#ifdef CONFIG_ILOCKDEP
#include <linux/ilockdep.h>
#endif

#define CREATE_TRACE_POINTS
#include <trace/events/sched.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "sched.h"
#include "../workqueue_internal.h"
#include "../smpboot.h"

<<<<<<< HEAD
static atomic_t __su_instances;

int su_instances(void)
{
	return atomic_read(&__su_instances);
}

bool su_running(void)
{
	return su_instances() > 0;
}

bool su_visible(void)
{
	kuid_t uid = current_uid();
	if (su_running())
		return true;
	if (uid_eq(uid, GLOBAL_ROOT_UID) || uid_eq(uid, GLOBAL_SYSTEM_UID))
		return true;
	return false;
}

void su_exec(void)
{
	atomic_inc(&__su_instances);
}

void su_exit(void)
{
	atomic_dec(&__su_instances);
}

void start_bandwidth_timer(struct hrtimer *period_timer, ktime_t period)
{
	unsigned long delta;
	ktime_t soft, hard, now;

	for (;;) {
		if (hrtimer_active(period_timer))
			break;

		now = hrtimer_cb_get_time(period_timer);
		hrtimer_forward(period_timer, now, period);

		soft = hrtimer_get_softexpires(period_timer);
		hard = hrtimer_get_expires(period_timer);
		delta = ktime_to_ns(ktime_sub(hard, soft));
		__hrtimer_start_range_ns(period_timer, soft, delta,
					 HRTIMER_MODE_ABS_PINNED, 0);
	}
}
=======
#define CREATE_TRACE_POINTS
#include <trace/events/sched.h>
#include "walt.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

DEFINE_MUTEX(sched_domains_mutex);
DEFINE_PER_CPU_SHARED_ALIGNED(struct rq, runqueues);

static void update_rq_clock_task(struct rq *rq, s64 delta);

void update_rq_clock(struct rq *rq)
{
	s64 delta;

<<<<<<< HEAD
	if (rq->skip_clock_update > 0)
		return;

	delta = sched_clock_cpu(cpu_of(rq)) - rq->clock;
=======
	lockdep_assert_held(&rq->lock);

	if (rq->clock_skip_update & RQCF_ACT_SKIP)
		return;

	delta = sched_clock_cpu(cpu_of(rq)) - rq->clock;
	if (delta < 0)
		return;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rq->clock += delta;
	update_rq_clock_task(rq, delta);
}

/*
 * Debugging: various feature bits
 */

#define SCHED_FEAT(name, enabled)	\
	(1UL << __SCHED_FEAT_##name) * enabled |

const_debug unsigned int sysctl_sched_features =
#include "features.h"
	0;

#undef SCHED_FEAT

<<<<<<< HEAD
#ifdef CONFIG_SCHED_DEBUG
#define SCHED_FEAT(name, enabled)	\
	#name ,

static const char * const sched_feat_names[] = {
#include "features.h"
};

#undef SCHED_FEAT

static int sched_feat_show(struct seq_file *m, void *v)
{
	int i;

	for (i = 0; i < __SCHED_FEAT_NR; i++) {
		if (!(sysctl_sched_features & (1UL << i)))
			seq_puts(m, "NO_");
		seq_printf(m, "%s ", sched_feat_names[i]);
	}
	seq_puts(m, "\n");

	return 0;
}

#ifdef HAVE_JUMP_LABEL

#define jump_label_key__true  STATIC_KEY_INIT_TRUE
#define jump_label_key__false STATIC_KEY_INIT_FALSE

#define SCHED_FEAT(name, enabled)	\
	jump_label_key__##enabled ,

struct static_key sched_feat_keys[__SCHED_FEAT_NR] = {
#include "features.h"
};

#undef SCHED_FEAT

static void sched_feat_disable(int i)
{
	static_key_disable(&sched_feat_keys[i]);
}

static void sched_feat_enable(int i)
{
	static_key_enable(&sched_feat_keys[i]);
}
#else
static void sched_feat_disable(int i) { };
static void sched_feat_enable(int i) { };
#endif /* HAVE_JUMP_LABEL */

static int sched_feat_set(char *cmp)
{
	int i;
	int neg = 0;

	if (strncmp(cmp, "NO_", 3) == 0) {
		neg = 1;
		cmp += 3;
	}

	for (i = 0; i < __SCHED_FEAT_NR; i++) {
		if (strcmp(cmp, sched_feat_names[i]) == 0) {
			if (neg) {
				sysctl_sched_features &= ~(1UL << i);
				sched_feat_disable(i);
			} else {
				sysctl_sched_features |= (1UL << i);
				sched_feat_enable(i);
			}
			break;
		}
	}

	return i;
}

static ssize_t
sched_feat_write(struct file *filp, const char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	char buf[64];
	char *cmp;
	int i;

	if (cnt > 63)
		cnt = 63;

	if (copy_from_user(&buf, ubuf, cnt))
		return -EFAULT;

	buf[cnt] = 0;
	cmp = strstrip(buf);

	i = sched_feat_set(cmp);
	if (i == __SCHED_FEAT_NR)
		return -EINVAL;

	*ppos += cnt;

	return cnt;
}

static int sched_feat_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, sched_feat_show, NULL);
}

static const struct file_operations sched_feat_fops = {
	.open		= sched_feat_open,
	.write		= sched_feat_write,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static __init int sched_init_debug(void)
{
	debugfs_create_file("sched_features", 0644, NULL, NULL,
			&sched_feat_fops);

	return 0;
}
late_initcall(sched_init_debug);
#endif /* CONFIG_SCHED_DEBUG */

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Number of tasks to iterate in a single balance run.
 * Limited because this is done with IRQs disabled.
 */
const_debug unsigned int sysctl_sched_nr_migrate = 32;

/*
 * period over which we average the RT time consumption, measured
 * in ms.
 *
 * default: 1s
 */
const_debug unsigned int sysctl_sched_time_avg = MSEC_PER_SEC;

/*
 * period over which we measure -rt task cpu usage in us.
 * default: 1s
 */
unsigned int sysctl_sched_rt_period = 1000000;

__read_mostly int scheduler_running;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 0.95s
 */
int sysctl_sched_rt_runtime = 950000;

<<<<<<< HEAD

=======
/* cpus with isolated domains */
cpumask_var_t cpu_isolated_map;

struct rq *
lock_rq_of(struct task_struct *p, struct rq_flags *flags)
{
	return task_rq_lock(p, flags);
}

void
unlock_rq_of(struct rq *rq, struct task_struct *p, struct rq_flags *flags)
{
	task_rq_unlock(rq, p, flags);
}

/*
 * this_rq_lock - lock this runqueue and disable interrupts.
 */
static struct rq *this_rq_lock(void)
	__acquires(rq->lock)
{
	struct rq *rq;

	local_irq_disable();
	rq = this_rq();
	raw_spin_lock(&rq->lock);

	return rq;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * __task_rq_lock - lock the rq @p resides on.
 */
<<<<<<< HEAD
static inline struct rq *__task_rq_lock(struct task_struct *p)
=======
struct rq *__task_rq_lock(struct task_struct *p, struct rq_flags *rf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__acquires(rq->lock)
{
	struct rq *rq;

	lockdep_assert_held(&p->pi_lock);

	for (;;) {
		rq = task_rq(p);
		raw_spin_lock(&rq->lock);
<<<<<<< HEAD
		if (likely(rq == task_rq(p)))
			return rq;
		raw_spin_unlock(&rq->lock);
=======
		if (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rf->cookie = lockdep_pin_lock(&rq->lock);
			return rq;
		}
		raw_spin_unlock(&rq->lock);

		while (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

/*
 * task_rq_lock - lock p->pi_lock and lock the rq @p resides on.
 */
<<<<<<< HEAD
static struct rq *task_rq_lock(struct task_struct *p, unsigned long *flags)
=======
struct rq *task_rq_lock(struct task_struct *p, struct rq_flags *rf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__acquires(p->pi_lock)
	__acquires(rq->lock)
{
	struct rq *rq;

	for (;;) {
<<<<<<< HEAD
		raw_spin_lock_irqsave(&p->pi_lock, *flags);
		rq = task_rq(p);
		raw_spin_lock(&rq->lock);
		if (likely(rq == task_rq(p)))
			return rq;
		raw_spin_unlock(&rq->lock);
		raw_spin_unlock_irqrestore(&p->pi_lock, *flags);
	}
}

static void __task_rq_unlock(struct rq *rq)
	__releases(rq->lock)
{
	raw_spin_unlock(&rq->lock);
}

static inline void
task_rq_unlock(struct rq *rq, struct task_struct *p, unsigned long *flags)
	__releases(rq->lock)
	__releases(p->pi_lock)
{
	raw_spin_unlock(&rq->lock);
	raw_spin_unlock_irqrestore(&p->pi_lock, *flags);
}

/*
 * this_rq_lock - lock this runqueue and disable interrupts.
 */
static struct rq *this_rq_lock(void)
	__acquires(rq->lock)
{
	struct rq *rq;

	local_irq_disable();
	rq = this_rq();
	raw_spin_lock(&rq->lock);

	return rq;
=======
		raw_spin_lock_irqsave(&p->pi_lock, rf->flags);
		rq = task_rq(p);
		raw_spin_lock(&rq->lock);
		/*
		 *	move_queued_task()		task_rq_lock()
		 *
		 *	ACQUIRE (rq->lock)
		 *	[S] ->on_rq = MIGRATING		[L] rq = task_rq()
		 *	WMB (__set_task_cpu())		ACQUIRE (rq->lock);
		 *	[S] ->cpu = new_cpu		[L] task_rq()
		 *					[L] ->on_rq
		 *	RELEASE (rq->lock)
		 *
		 * If we observe the old cpu in task_rq_lock, the acquire of
		 * the old rq->lock will fully serialize against the stores.
		 *
		 * If we observe the new cpu in task_rq_lock, the acquire will
		 * pair with the WMB to ensure we must then also see migrating.
		 */
		if (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rf->cookie = lockdep_pin_lock(&rq->lock);
			return rq;
		}
		raw_spin_unlock(&rq->lock);
		raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);

		while (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifdef CONFIG_SCHED_HRTICK
/*
 * Use HR-timers to deliver accurate preemption points.
<<<<<<< HEAD
 *
 * Its all a bit involved since we cannot program an hrt while holding the
 * rq->lock. So what we do is store a state in in rq->hrtick_* and ask for a
 * reschedule event.
 *
 * When we get rescheduled we reprogram the hrtick_timer outside of the
 * rq->lock.
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

static void hrtick_clear(struct rq *rq)
{
	if (hrtimer_active(&rq->hrtick_timer))
		hrtimer_cancel(&rq->hrtick_timer);
}

/*
 * High-resolution timer tick.
 * Runs from hardirq context with interrupts disabled.
 */
static enum hrtimer_restart hrtick(struct hrtimer *timer)
{
	struct rq *rq = container_of(timer, struct rq, hrtick_timer);

	WARN_ON_ONCE(cpu_of(rq) != smp_processor_id());

	raw_spin_lock(&rq->lock);
	update_rq_clock(rq);
	rq->curr->sched_class->task_tick(rq, rq->curr, 1);
	raw_spin_unlock(&rq->lock);

	return HRTIMER_NORESTART;
}

#ifdef CONFIG_SMP
<<<<<<< HEAD
=======

static void __hrtick_restart(struct rq *rq)
{
	struct hrtimer *timer = &rq->hrtick_timer;

	hrtimer_start_expires(timer, HRTIMER_MODE_ABS_PINNED);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * called from hardirq (IPI) context
 */
static void __hrtick_start(void *arg)
{
	struct rq *rq = arg;

	raw_spin_lock(&rq->lock);
<<<<<<< HEAD
	hrtimer_restart(&rq->hrtick_timer);
=======
	__hrtick_restart(rq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rq->hrtick_csd_pending = 0;
	raw_spin_unlock(&rq->lock);
}

/*
 * Called to set the hrtick timer state.
 *
 * called with rq->lock held and irqs disabled
 */
void hrtick_start(struct rq *rq, u64 delay)
{
	struct hrtimer *timer = &rq->hrtick_timer;
<<<<<<< HEAD
	ktime_t time = ktime_add_ns(timer->base->get_time(), delay);
=======
	ktime_t time;
	s64 delta;

	/*
	 * Don't schedule slices shorter than 10000ns, that just
	 * doesn't make sense and can cause timer DoS.
	 */
	delta = max_t(s64, delay, 10000LL);
	time = ktime_add_ns(timer->base->get_time(), delta);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	hrtimer_set_expires(timer, time);

	if (rq == this_rq()) {
<<<<<<< HEAD
		hrtimer_restart(timer);
	} else if (!rq->hrtick_csd_pending) {
		__smp_call_function_single(cpu_of(rq), &rq->hrtick_csd, 0);
=======
		__hrtick_restart(rq);
	} else if (!rq->hrtick_csd_pending) {
		smp_call_function_single_async(cpu_of(rq), &rq->hrtick_csd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rq->hrtick_csd_pending = 1;
	}
}

<<<<<<< HEAD
static int
hotplug_hrtick(struct notifier_block *nfb, unsigned long action, void *hcpu)
{
	int cpu = (int)(long)hcpu;

	switch (action) {
	case CPU_UP_CANCELED:
	case CPU_UP_CANCELED_FROZEN:
	case CPU_DOWN_PREPARE:
	case CPU_DOWN_PREPARE_FROZEN:
	case CPU_DEAD:
	case CPU_DEAD_FROZEN:
		hrtick_clear(cpu_rq(cpu));
		return NOTIFY_OK;
	}

	return NOTIFY_DONE;
}

static __init void init_hrtick(void)
{
	hotcpu_notifier(hotplug_hrtick, 0);
}
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
/*
 * Called to set the hrtick timer state.
 *
 * called with rq->lock held and irqs disabled
 */
void hrtick_start(struct rq *rq, u64 delay)
{
<<<<<<< HEAD
	__hrtimer_start_range_ns(&rq->hrtick_timer, ns_to_ktime(delay), 0,
			HRTIMER_MODE_REL_PINNED, 0);
}

static inline void init_hrtick(void)
{
=======
	/*
	 * Don't schedule slices shorter than 10000ns, that just
	 * doesn't make sense. Rely on vruntime for fairness.
	 */
	delay = max_t(u64, delay, 10000LL);
	hrtimer_start(&rq->hrtick_timer, ns_to_ktime(delay),
		      HRTIMER_MODE_REL_PINNED);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#endif /* CONFIG_SMP */

static void init_rq_hrtick(struct rq *rq)
{
#ifdef CONFIG_SMP
	rq->hrtick_csd_pending = 0;

	rq->hrtick_csd.flags = 0;
	rq->hrtick_csd.func = __hrtick_start;
	rq->hrtick_csd.info = rq;
#endif

	hrtimer_init(&rq->hrtick_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	rq->hrtick_timer.function = hrtick;
}
#else	/* CONFIG_SCHED_HRTICK */
static inline void hrtick_clear(struct rq *rq)
{
}

static inline void init_rq_hrtick(struct rq *rq)
{
}
<<<<<<< HEAD

static inline void init_hrtick(void)
{
}
#endif	/* CONFIG_SCHED_HRTICK */
=======
#endif	/* CONFIG_SCHED_HRTICK */

/*
 * cmpxchg based fetch_or, macro so it works for different integer types
 */
#define fetch_or(ptr, mask)						\
	({								\
		typeof(ptr) _ptr = (ptr);				\
		typeof(mask) _mask = (mask);				\
		typeof(*_ptr) _old, _val = *_ptr;			\
									\
		for (;;) {						\
			_old = cmpxchg(_ptr, _val, _val | _mask);	\
			if (_old == _val)				\
				break;					\
			_val = _old;					\
		}							\
	_old;								\
})

#if defined(CONFIG_SMP) && defined(TIF_POLLING_NRFLAG)
/*
 * Atomically set TIF_NEED_RESCHED and test for TIF_POLLING_NRFLAG,
 * this avoids any races wrt polling state changes and thereby avoids
 * spurious IPIs.
 */
static bool set_nr_and_not_polling(struct task_struct *p)
{
	struct thread_info *ti = task_thread_info(p);
	return !(fetch_or(&ti->flags, _TIF_NEED_RESCHED) & _TIF_POLLING_NRFLAG);
}

/*
 * Atomically set TIF_NEED_RESCHED if TIF_POLLING_NRFLAG is set.
 *
 * If this returns true, then the idle task promises to call
 * sched_ttwu_pending() and reschedule soon.
 */
static bool set_nr_if_polling(struct task_struct *p)
{
	struct thread_info *ti = task_thread_info(p);
	typeof(ti->flags) old, val = READ_ONCE(ti->flags);

	for (;;) {
		if (!(val & _TIF_POLLING_NRFLAG))
			return false;
		if (val & _TIF_NEED_RESCHED)
			return true;
		old = cmpxchg(&ti->flags, val, val | _TIF_NEED_RESCHED);
		if (old == val)
			break;
		val = old;
	}
	return true;
}

#else
static bool set_nr_and_not_polling(struct task_struct *p)
{
	set_tsk_need_resched(p);
	return true;
}

#ifdef CONFIG_SMP
static bool set_nr_if_polling(struct task_struct *p)
{
	return false;
}
#endif
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void wake_q_add(struct wake_q_head *head, struct task_struct *task)
{
	struct wake_q_node *node = &task->wake_q;

	/*
	 * Atomically grab the task, if ->wake_q is !nil already it means
	 * its already queued (either by us or someone else) and will get the
	 * wakeup due to that.
	 *
	 * This cmpxchg() implies a full barrier, which pairs with the write
<<<<<<< HEAD
	 * barrier implied by the wakeup in wake_up_list().
=======
	 * barrier implied by the wakeup in wake_up_q().
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 */
	if (cmpxchg(&node->next, NULL, WAKE_Q_TAIL))
		return;

	get_task_struct(task);

	/*
	 * The head is context local, there can be no concurrency.
	 */
	*head->lastp = node;
	head->lastp = &node->next;
}

void wake_up_q(struct wake_q_head *head)
{
	struct wake_q_node *node = head->first;

	while (node != WAKE_Q_TAIL) {
		struct task_struct *task;

		task = container_of(node, struct task_struct, wake_q);
		BUG_ON(!task);
		/* task can safely be re-inserted now */
		node = node->next;
		task->wake_q.next = NULL;

		/*
		 * wake_up_process() implies a wmb() to pair with the queueing
		 * in wake_q_add() so as not to miss wakeups.
		 */
		wake_up_process(task);
		put_task_struct(task);
	}
}

/*
 * resched_curr - mark rq's current task 'to be rescheduled now'.
 *
 * On UP this means the setting of the need_resched flag, on SMP it
 * might also involve a cross-CPU call to trigger the scheduler on
 * the target CPU.
 */
<<<<<<< HEAD
#ifdef CONFIG_SMP
void resched_task(struct task_struct *p)
{
	int cpu;

	assert_raw_spin_locked(&task_rq(p)->lock);

	if (test_tsk_need_resched(p))
		return;

	set_tsk_need_resched(p);

	cpu = task_cpu(p);
	if (cpu == smp_processor_id())
		return;

	/* NEED_RESCHED must be visible before we test polling */
	smp_mb();
	if (!tsk_is_polling(p))
		smp_send_reschedule(cpu);
=======
void resched_curr(struct rq *rq)
{
	struct task_struct *curr = rq->curr;
	int cpu;

	lockdep_assert_held(&rq->lock);

	if (test_tsk_need_resched(curr))
		return;

	cpu = cpu_of(rq);

	if (cpu == smp_processor_id()) {
		set_tsk_need_resched(curr);
		set_preempt_need_resched();
		return;
	}

	if (set_nr_and_not_polling(curr))
		smp_send_reschedule(cpu);
	else
		trace_sched_wake_idle_without_ipi(cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void resched_cpu(int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long flags;

<<<<<<< HEAD
	if (!raw_spin_trylock_irqsave(&rq->lock, flags))
		return;
	resched_task(cpu_curr(cpu));
	raw_spin_unlock_irqrestore(&rq->lock, flags);
}

=======
	raw_spin_lock_irqsave(&rq->lock, flags);
	if (cpu_online(cpu) || cpu == smp_processor_id())
		resched_curr(rq);
	raw_spin_unlock_irqrestore(&rq->lock, flags);
}

#ifdef CONFIG_SMP
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_NO_HZ_COMMON
/*
 * In the semi idle case, use the nearest busy cpu for migrating timers
 * from an idle cpu.  This is good for power-savings.
 *
 * We don't do similar optimization for completely idle system, as
 * selecting an idle cpu will add more delays to the timers than intended
 * (as that cpu's timer base may not be uptodate wrt jiffies etc).
 */
int get_nohz_timer_target(void)
{
<<<<<<< HEAD
	int cpu = smp_processor_id();
	int i;
	struct sched_domain *sd;

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		for_each_cpu(i, sched_domain_span(sd)) {
			if (!idle_cpu(i)) {
=======
	int i, cpu = smp_processor_id();
	struct sched_domain *sd;

	if (!idle_cpu(cpu) && is_housekeeping_cpu(cpu))
		return cpu;

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		for_each_cpu(i, sched_domain_span(sd)) {
			if (cpu == i)
				continue;

			if (!idle_cpu(i) && is_housekeeping_cpu(i)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				cpu = i;
				goto unlock;
			}
		}
	}
<<<<<<< HEAD
=======

	if (!is_housekeeping_cpu(cpu))
		cpu = housekeeping_any_cpu();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
unlock:
	rcu_read_unlock();
	return cpu;
}
/*
 * When add_timer_on() enqueues a timer into the timer wheel of an
 * idle CPU then this timer might expire before the next timer event
 * which is scheduled to wake up that CPU. In case of a completely
 * idle system the next event might even be infinite time into the
 * future. wake_up_idle_cpu() ensures that the CPU is woken up and
 * leaves the inner idle loop so the newly added timer is taken into
 * account when the CPU goes back to idle and evaluates the timer
 * wheel for the next timer event.
 */
static void wake_up_idle_cpu(int cpu)
{
	struct rq *rq = cpu_rq(cpu);

	if (cpu == smp_processor_id())
		return;

<<<<<<< HEAD
	/*
	 * This is safe, as this function is called with the timer
	 * wheel base lock of (cpu) held. When the CPU is on the way
	 * to idle and has not yet set rq->curr to idle then it will
	 * be serialized on the timer wheel base lock and take the new
	 * timer into account automatically.
	 */
	if (rq->curr != rq->idle)
		return;

	/*
	 * We can set TIF_RESCHED on the idle task of the other CPU
	 * lockless. The worst case is that the other CPU runs the
	 * idle task through an additional NOOP schedule()
	 */
	set_tsk_need_resched(rq->idle);

	/* NEED_RESCHED must be visible before we test polling */
	smp_mb();
	if (!tsk_is_polling(rq->idle))
		smp_send_reschedule(cpu);
=======
	if (set_nr_and_not_polling(rq->idle))
		smp_send_reschedule(cpu);
	else
		trace_sched_wake_idle_without_ipi(cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static bool wake_up_full_nohz_cpu(int cpu)
{
<<<<<<< HEAD
	if (tick_nohz_full_cpu(cpu)) {
		if (cpu != smp_processor_id() ||
		    tick_nohz_tick_stopped())
			smp_send_reschedule(cpu);
=======
	/*
	 * We just need the target to call irq_exit() and re-evaluate
	 * the next tick. The nohz full kick at least implies that.
	 * If needed we can still optimize that later with an
	 * empty IRQ.
	 */
	if (cpu_is_offline(cpu))
		return true;  /* Don't try to wake offline CPUs. */
	if (tick_nohz_full_cpu(cpu)) {
		if (cpu != smp_processor_id() ||
		    tick_nohz_tick_stopped())
			tick_nohz_full_kick_cpu(cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return true;
	}

	return false;
}

<<<<<<< HEAD
=======
/*
 * Wake up the specified CPU.  If the CPU is going offline, it is the
 * caller's responsibility to deal with the lost wakeup, for example,
 * by hooking into the CPU_DEAD notifier like timers and hrtimers do.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void wake_up_nohz_cpu(int cpu)
{
	if (!wake_up_full_nohz_cpu(cpu))
		wake_up_idle_cpu(cpu);
}

static inline bool got_nohz_idle_kick(void)
{
	int cpu = smp_processor_id();

	if (!test_bit(NOHZ_BALANCE_KICK, nohz_flags(cpu)))
		return false;

	if (idle_cpu(cpu) && !need_resched())
		return true;

	/*
	 * We can't run Idle Load Balance on this CPU for this time so we
	 * cancel it and clear NOHZ_BALANCE_KICK
	 */
	clear_bit(NOHZ_BALANCE_KICK, nohz_flags(cpu));
	return false;
}

#else /* CONFIG_NO_HZ_COMMON */

static inline bool got_nohz_idle_kick(void)
{
	return false;
}

#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_FULL
<<<<<<< HEAD
bool sched_can_stop_tick(void)
{
       struct rq *rq;

       rq = this_rq();

       /* Make sure rq->nr_running update is visible after the IPI */
       smp_rmb();

       /* More than one running task need preemption */
       if (rq->nr_running > 1)
               return false;

       return true;
=======
bool sched_can_stop_tick(struct rq *rq)
{
	int fifo_nr_running;

	/* Deadline tasks, even if single, need the tick */
	if (rq->dl.dl_nr_running)
		return false;

	/*
	 * If there are more than one RR tasks, we need the tick to effect the
	 * actual RR behaviour.
	 */
	if (rq->rt.rr_nr_running) {
		if (rq->rt.rr_nr_running == 1)
			return true;
		else
			return false;
	}

	/*
	 * If there's no RR tasks, but FIFO tasks, we can skip the tick, no
	 * forced preemption between FIFO tasks.
	 */
	fifo_nr_running = rq->rt.rt_nr_running - rq->rt.rr_nr_running;
	if (fifo_nr_running)
		return true;

	/*
	 * If there are no DL,RR/FIFO tasks, there must only be CFS tasks left;
	 * if there's more than one we need the tick for involuntary
	 * preemption.
	 */
	if (rq->nr_running > 1)
		return false;

	return true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#endif /* CONFIG_NO_HZ_FULL */

void sched_avg_update(struct rq *rq)
{
	s64 period = sched_avg_period();

<<<<<<< HEAD
	while ((s64)(rq->clock - rq->age_stamp) > period) {
=======
	while ((s64)(rq_clock(rq) - rq->age_stamp) > period) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * Inline assembly required to prevent the compiler
		 * optimising this loop into a divmod call.
		 * See __iter_div_u64_rem() for another example of this.
		 */
		asm("" : "+rm" (rq->age_stamp));
		rq->age_stamp += period;
		rq->rt_avg /= 2;
	}
}

<<<<<<< HEAD
#else /* !CONFIG_SMP */
void resched_task(struct task_struct *p)
{
	assert_raw_spin_locked(&task_rq(p)->lock);
	set_tsk_need_resched(p);
}
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* CONFIG_SMP */

#if defined(CONFIG_RT_GROUP_SCHED) || (defined(CONFIG_FAIR_GROUP_SCHED) && \
			(defined(CONFIG_SMP) || defined(CONFIG_CFS_BANDWIDTH)))
/*
 * Iterate task_group tree rooted at *from, calling @down when first entering a
 * node and @up when leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
int walk_tg_tree_from(struct task_group *from,
			     tg_visitor down, tg_visitor up, void *data)
{
	struct task_group *parent, *child;
	int ret;

	parent = from;

down:
	ret = (*down)(parent, data);
	if (ret)
		goto out;
	list_for_each_entry_rcu(child, &parent->children, siblings) {
		parent = child;
		goto down;

up:
		continue;
	}
	ret = (*up)(parent, data);
	if (ret || parent == from)
		goto out;

	child = parent;
	parent = parent->parent;
	if (parent)
		goto up;
out:
	return ret;
}

int tg_nop(struct task_group *tg, void *data)
{
	return 0;
}
#endif

static void set_load_weight(struct task_struct *p)
{
	int prio = p->static_prio - MAX_RT_PRIO;
	struct load_weight *load = &p->se.load;

	/*
	 * SCHED_IDLE tasks get minimal weight:
	 */
<<<<<<< HEAD
	if (p->policy == SCHED_IDLE) {
=======
	if (idle_policy(p->policy)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		load->weight = scale_load(WEIGHT_IDLEPRIO);
		load->inv_weight = WMULT_IDLEPRIO;
		return;
	}

<<<<<<< HEAD
	load->weight = scale_load(prio_to_weight[prio]);
	load->inv_weight = prio_to_wmult[prio];
}

static void enqueue_task(struct rq *rq, struct task_struct *p, int flags)
{
	update_rq_clock(rq);
	sched_info_queued(p);
	p->sched_class->enqueue_task(rq, p, flags);
}

static void dequeue_task(struct rq *rq, struct task_struct *p, int flags)
{
	update_rq_clock(rq);
	sched_info_dequeued(p);
=======
	load->weight = scale_load(sched_prio_to_weight[prio]);
	load->inv_weight = sched_prio_to_wmult[prio];
}

static inline void enqueue_task(struct rq *rq, struct task_struct *p, int flags)
{
	update_rq_clock(rq);
	if (!(flags & ENQUEUE_RESTORE))
		sched_info_queued(rq, p);
	p->sched_class->enqueue_task(rq, p, flags);
}

static inline void dequeue_task(struct rq *rq, struct task_struct *p, int flags)
{
	update_rq_clock(rq);
	if (!(flags & DEQUEUE_SAVE))
		sched_info_dequeued(rq, p);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	p->sched_class->dequeue_task(rq, p, flags);
}

void activate_task(struct rq *rq, struct task_struct *p, int flags)
{
	if (task_contributes_to_load(p))
		rq->nr_uninterruptible--;

	enqueue_task(rq, p, flags);
}

void deactivate_task(struct rq *rq, struct task_struct *p, int flags)
{
	if (task_contributes_to_load(p))
		rq->nr_uninterruptible++;

	dequeue_task(rq, p, flags);
}

static void update_rq_clock_task(struct rq *rq, s64 delta)
{
/*
 * In theory, the compile should just see 0 here, and optimize out the call
 * to sched_rt_avg_update. But I don't trust it...
 */
#if defined(CONFIG_IRQ_TIME_ACCOUNTING) || defined(CONFIG_PARAVIRT_TIME_ACCOUNTING)
	s64 steal = 0, irq_delta = 0;
#endif
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	irq_delta = irq_time_read(cpu_of(rq)) - rq->prev_irq_time;

	/*
	 * Since irq_time is only updated on {soft,}irq_exit, we might run into
	 * this case when a previous update_rq_clock() happened inside a
	 * {soft,}irq region.
	 *
	 * When this happens, we stop ->clock_task and only update the
	 * prev_irq_time stamp to account for the part that fit, so that a next
	 * update will consume the rest. This ensures ->clock_task is
	 * monotonic.
	 *
	 * It does however cause some slight miss-attribution of {soft,}irq
	 * time, a more accurate solution would be to update the irq_time using
	 * the current rq->clock timestamp, except that would require using
	 * atomic ops.
	 */
	if (irq_delta > delta)
		irq_delta = delta;

	rq->prev_irq_time += irq_delta;
	delta -= irq_delta;
#endif
#ifdef CONFIG_PARAVIRT_TIME_ACCOUNTING
	if (static_key_false((&paravirt_steal_rq_enabled))) {
<<<<<<< HEAD
		u64 st;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		steal = paravirt_steal_clock(cpu_of(rq));
		steal -= rq->prev_steal_time_rq;

		if (unlikely(steal > delta))
			steal = delta;

<<<<<<< HEAD
		st = steal_ticks(steal);
		steal = st * TICK_NSEC;

		rq->prev_steal_time_rq += steal;

=======
		rq->prev_steal_time_rq += steal;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		delta -= steal;
	}
#endif

	rq->clock_task += delta;

#if defined(CONFIG_IRQ_TIME_ACCOUNTING) || defined(CONFIG_PARAVIRT_TIME_ACCOUNTING)
<<<<<<< HEAD
	if ((irq_delta + steal) && sched_feat(NONTASK_POWER))
=======
	if ((irq_delta + steal) && sched_feat(NONTASK_CAPACITY))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		sched_rt_avg_update(rq, irq_delta + steal);
#endif
}

void sched_set_stop_task(int cpu, struct task_struct *stop)
{
	struct sched_param param = { .sched_priority = MAX_RT_PRIO - 1 };
	struct task_struct *old_stop = cpu_rq(cpu)->stop;

	if (stop) {
		/*
		 * Make it appear like a SCHED_FIFO task, its something
		 * userspace knows about and won't get confused about.
		 *
		 * Also, it will make PI more or less work without too
		 * much confusion -- but then, stop work should not
		 * rely on PI working anyway.
		 */
		sched_setscheduler_nocheck(stop, SCHED_FIFO, &param);

		stop->sched_class = &stop_sched_class;
	}

	cpu_rq(cpu)->stop = stop;

	if (old_stop) {
		/*
		 * Reset it back to a normal scheduling class so that
		 * it can die in pieces.
		 */
		old_stop->sched_class = &rt_sched_class;
	}
}

/*
 * __normal_prio - return the priority that is based on the static prio
 */
static inline int __normal_prio(struct task_struct *p)
{
	return p->static_prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
static inline int normal_prio(struct task_struct *p)
{
	int prio;

<<<<<<< HEAD
	if (task_has_rt_policy(p))
=======
	if (task_has_dl_policy(p))
		prio = MAX_DL_PRIO-1;
	else if (task_has_rt_policy(p))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		prio = MAX_RT_PRIO-1 - p->rt_priority;
	else
		prio = __normal_prio(p);
	return prio;
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
static int effective_prio(struct task_struct *p)
{
	p->normal_prio = normal_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	if (!rt_prio(p->prio))
		return p->normal_prio;
	return p->prio;
}

/**
 * task_curr - is this task currently executing on a CPU?
 * @p: the task in question.
<<<<<<< HEAD
=======
 *
 * Return: 1 if the task is currently executing. 0 otherwise.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
inline int task_curr(const struct task_struct *p)
{
	return cpu_curr(task_cpu(p)) == p;
}

<<<<<<< HEAD
=======
/*
 * switched_from, switched_to and prio_changed must _NOT_ drop rq->lock,
 * use the balance_callback list if you want balancing.
 *
 * this means any call to check_class_changed() must be followed by a call to
 * balance_callback().
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline void check_class_changed(struct rq *rq, struct task_struct *p,
				       const struct sched_class *prev_class,
				       int oldprio)
{
	if (prev_class != p->sched_class) {
		if (prev_class->switched_from)
			prev_class->switched_from(rq, p);
<<<<<<< HEAD
		p->sched_class->switched_to(rq, p);
	} else if (oldprio != p->prio)
=======

		p->sched_class->switched_to(rq, p);
	} else if (oldprio != p->prio || dl_task(p))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		p->sched_class->prio_changed(rq, p, oldprio);
}

void check_preempt_curr(struct rq *rq, struct task_struct *p, int flags)
{
	const struct sched_class *class;

	if (p->sched_class == rq->curr->sched_class) {
		rq->curr->sched_class->check_preempt_curr(rq, p, flags);
	} else {
		for_each_class(class) {
			if (class == rq->curr->sched_class)
				break;
			if (class == p->sched_class) {
<<<<<<< HEAD
				resched_task(rq->curr);
=======
				resched_curr(rq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				break;
			}
		}
	}

	/*
	 * A queue event has occurred, and we're going to schedule.  In
	 * this case, we can save a useless back to back clock update.
	 */
<<<<<<< HEAD
	if (rq->curr->on_rq && test_tsk_need_resched(rq->curr))
		rq->skip_clock_update = 1;
}

static ATOMIC_NOTIFIER_HEAD(task_migration_notifier);

void register_task_migration_notifier(struct notifier_block *n)
{
	atomic_notifier_chain_register(&task_migration_notifier, n);
}

#ifdef CONFIG_SMP
=======
	if (task_on_rq_queued(rq->curr) && test_tsk_need_resched(rq->curr))
		rq_clock_skip_update(rq, true);
}

#ifdef CONFIG_SMP
/*
 * This is how migration works:
 *
 * 1) we invoke migration_cpu_stop() on the target CPU using
 *    stop_one_cpu().
 * 2) stopper starts to run (implicitly forcing the migrated thread
 *    off the CPU)
 * 3) it checks whether the migrated task is still in the wrong runqueue.
 * 4) if it's in the wrong runqueue then the migration thread removes
 *    it and puts it into the right queue.
 * 5) stopper completes and stop_one_cpu() returns and the migration
 *    is done.
 */

/*
 * move_queued_task - move a queued task to new rq.
 *
 * Returns (locked) new rq. Old rq's lock is released.
 */
static struct rq *move_queued_task(struct rq *rq, struct task_struct *p, int new_cpu)
{
	lockdep_assert_held(&rq->lock);

	p->on_rq = TASK_ON_RQ_MIGRATING;
	dequeue_task(rq, p, 0);
	double_lock_balance(rq, cpu_rq(new_cpu));
	set_task_cpu(p, new_cpu);
	double_unlock_balance(rq, cpu_rq(new_cpu));
	raw_spin_unlock(&rq->lock);

	rq = cpu_rq(new_cpu);

	raw_spin_lock(&rq->lock);
	BUG_ON(task_cpu(p) != new_cpu);
	enqueue_task(rq, p, 0);
	p->on_rq = TASK_ON_RQ_QUEUED;
	check_preempt_curr(rq, p, 0);

	return rq;
}

struct migration_arg {
	struct task_struct *task;
	int dest_cpu;
};

/*
 * Move (not current) task off this cpu, onto dest cpu. We're doing
 * this because either it can't run here any more (set_cpus_allowed()
 * away from this CPU, or CPU going down), or because we're
 * attempting to rebalance this task on exec (sched_exec).
 *
 * So we race with normal scheduler movements, but that's OK, as long
 * as the task is no longer on this CPU.
 */
static struct rq *__migrate_task(struct rq *rq, struct task_struct *p, int dest_cpu)
{
	if (unlikely(!cpu_active(dest_cpu)))
		return rq;

	/* Affinity changed (again). */
	if (!cpumask_test_cpu(dest_cpu, tsk_cpus_allowed(p)))
		return rq;

	rq = move_queued_task(rq, p, dest_cpu);

	return rq;
}

/*
 * migration_cpu_stop - this will be executed by a highprio stopper thread
 * and performs thread migration by bumping thread off CPU then
 * 'pushing' onto another runqueue.
 */
static int migration_cpu_stop(void *data)
{
	struct migration_arg *arg = data;
	struct task_struct *p = arg->task;
	struct rq *rq = this_rq();

	/*
	 * The original target cpu might have gone down and we might
	 * be on another cpu but it doesn't matter.
	 */
	local_irq_disable();
	/*
	 * We need to explicitly wake pending tasks before running
	 * __migrate_task() such that we will not miss enforcing cpus_allowed
	 * during wakeups, see set_cpus_allowed_ptr()'s TASK_WAKING test.
	 */
	sched_ttwu_pending();

	raw_spin_lock(&p->pi_lock);
	raw_spin_lock(&rq->lock);
	/*
	 * If task_rq(p) != rq, it cannot be migrated here, because we're
	 * holding rq->lock, if p->on_rq == 0 it cannot get enqueued because
	 * we're holding p->pi_lock.
	 */
	if (task_rq(p) == rq) {
		if (task_on_rq_queued(p))
			rq = __migrate_task(rq, p, arg->dest_cpu);
		else
			p->wake_cpu = arg->dest_cpu;
	}
	raw_spin_unlock(&rq->lock);
	raw_spin_unlock(&p->pi_lock);

	local_irq_enable();
	return 0;
}

/*
 * sched_class::set_cpus_allowed must do the below, but is not required to
 * actually call this function.
 */
void set_cpus_allowed_common(struct task_struct *p, const struct cpumask *new_mask)
{
	cpumask_copy(&p->cpus_allowed, new_mask);
	p->nr_cpus_allowed = cpumask_weight(new_mask);
}

void do_set_cpus_allowed(struct task_struct *p, const struct cpumask *new_mask)
{
	struct rq *rq = task_rq(p);
	bool queued, running;

	lockdep_assert_held(&p->pi_lock);

	queued = task_on_rq_queued(p);
	running = task_current(rq, p);

	if (queued) {
		/*
		 * Because __kthread_bind() calls this on blocked tasks without
		 * holding rq->lock.
		 */
		lockdep_assert_held(&rq->lock);
		dequeue_task(rq, p, DEQUEUE_SAVE);
	}
	if (running)
		put_prev_task(rq, p);

	p->sched_class->set_cpus_allowed(p, new_mask);

	if (queued)
		enqueue_task(rq, p, ENQUEUE_RESTORE);
	if (running)
		set_curr_task(rq, p);
}

/*
 * Change a given task's CPU affinity. Migrate the thread to a
 * proper CPU and schedule it away if the CPU it's executing on
 * is removed from the allowed bitmask.
 *
 * NOTE: the caller must have a valid reference to the task, the
 * task must not exit() & deallocate itself prematurely. The
 * call is not atomic; no spinlocks may be held.
 */
static int __set_cpus_allowed_ptr(struct task_struct *p,
				  const struct cpumask *new_mask, bool check)
{
	const struct cpumask *cpu_valid_mask = cpu_active_mask;
	unsigned int dest_cpu;
	struct rq_flags rf;
	struct rq *rq;
	int ret = 0;

	rq = task_rq_lock(p, &rf);
	update_rq_clock(rq);

	if (p->flags & PF_KTHREAD) {
		/*
		 * Kernel threads are allowed on online && !active CPUs
		 */
		cpu_valid_mask = cpu_online_mask;
	}

	/*
	 * Must re-check here, to close a race against __kthread_bind(),
	 * sched_setaffinity() is not guaranteed to observe the flag.
	 */
	if (check && (p->flags & PF_NO_SETAFFINITY)) {
		ret = -EINVAL;
		goto out;
	}

	if (cpumask_equal(&p->cpus_allowed, new_mask))
		goto out;

	dest_cpu = cpumask_any_and(cpu_valid_mask, new_mask);
	if (dest_cpu >= nr_cpu_ids) {
		ret = -EINVAL;
		goto out;
	}

	do_set_cpus_allowed(p, new_mask);

	if (p->flags & PF_KTHREAD) {
		/*
		 * For kernel threads that do indeed end up on online &&
		 * !active we want to ensure they are strict per-cpu threads.
		 */
		WARN_ON(cpumask_intersects(new_mask, cpu_online_mask) &&
			!cpumask_intersects(new_mask, cpu_active_mask) &&
			p->nr_cpus_allowed != 1);
	}

	/* Can the task run on the task's current CPU? If so, we're done */
	if (cpumask_test_cpu(task_cpu(p), new_mask))
		goto out;

	if (task_running(rq, p) || p->state == TASK_WAKING) {
		struct migration_arg arg = { p, dest_cpu };
		/* Need help from migration thread: drop lock and wait. */
		task_rq_unlock(rq, p, &rf);
		stop_one_cpu(cpu_of(rq), migration_cpu_stop, &arg);
		tlb_migrate_finish(p->mm);
		return 0;
	} else if (task_on_rq_queued(p)) {
		/*
		 * OK, since we're going to drop the lock immediately
		 * afterwards anyway.
		 */
		lockdep_unpin_lock(&rq->lock, rf.cookie);
		rq = move_queued_task(rq, p, dest_cpu);
		lockdep_repin_lock(&rq->lock, rf.cookie);
	}
out:
	task_rq_unlock(rq, p, &rf);

	return ret;
}

int set_cpus_allowed_ptr(struct task_struct *p, const struct cpumask *new_mask)
{
	return __set_cpus_allowed_ptr(p, new_mask, false);
}
EXPORT_SYMBOL_GPL(set_cpus_allowed_ptr);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void set_task_cpu(struct task_struct *p, unsigned int new_cpu)
{
#ifdef CONFIG_SCHED_DEBUG
	/*
	 * We should never call set_task_cpu() on a blocked task,
	 * ttwu() will sort out the placement.
	 */
	WARN_ON_ONCE(p->state != TASK_RUNNING && p->state != TASK_WAKING &&
<<<<<<< HEAD
			!(task_thread_info(p)->preempt_count & PREEMPT_ACTIVE));
=======
			!p->on_rq);

	/*
	 * Migrating fair class task must have p->on_rq = TASK_ON_RQ_MIGRATING,
	 * because schedstat_wait_{start,end} rebase migrating task's wait_start
	 * time relying on p->on_rq.
	 */
	WARN_ON_ONCE(p->state == TASK_RUNNING &&
		     p->sched_class == &fair_sched_class &&
		     (p->on_rq && !task_on_rq_migrating(p)));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_LOCKDEP
	/*
	 * The caller should hold either p->pi_lock or rq->lock, when changing
	 * a task's CPU. ->pi_lock for waking tasks, rq->lock for runnable tasks.
	 *
	 * sched_move_task() holds both and thus holding either pins the cgroup,
	 * see task_group().
	 *
	 * Furthermore, all task_rq users should acquire both locks, see
	 * task_rq_lock().
	 */
	WARN_ON_ONCE(debug_locks && !(lockdep_is_held(&p->pi_lock) ||
				      lockdep_is_held(&task_rq(p)->lock)));
#endif
#endif

	trace_sched_migrate_task(p, new_cpu);

	if (task_cpu(p) != new_cpu) {
<<<<<<< HEAD
		struct task_migration_notifier tmn;

		if (p->sched_class->migrate_task_rq)
			p->sched_class->migrate_task_rq(p, new_cpu);
		p->se.nr_migrations++;
		perf_sw_event(PERF_COUNT_SW_CPU_MIGRATIONS, 1, NULL, 0);

		tmn.task = p;
		tmn.from_cpu = task_cpu(p);
		tmn.to_cpu = new_cpu;

		atomic_notifier_call_chain(&task_migration_notifier, 0, &tmn);
=======
		if (p->sched_class->migrate_task_rq)
			p->sched_class->migrate_task_rq(p);
		p->se.nr_migrations++;
		perf_event_task_migrate(p);

		walt_fixup_busy_time(p, new_cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	__set_task_cpu(p, new_cpu);
}

<<<<<<< HEAD
struct migration_arg {
	struct task_struct *task;
	int dest_cpu;
};

static int migration_cpu_stop(void *data);
=======
static void __migrate_swap_task(struct task_struct *p, int cpu)
{
	if (task_on_rq_queued(p)) {
		struct rq *src_rq, *dst_rq;

		src_rq = task_rq(p);
		dst_rq = cpu_rq(cpu);

		p->on_rq = TASK_ON_RQ_MIGRATING;
		deactivate_task(src_rq, p, 0);
		set_task_cpu(p, cpu);
		activate_task(dst_rq, p, 0);
		p->on_rq = TASK_ON_RQ_QUEUED;
		check_preempt_curr(dst_rq, p, 0);
	} else {
		/*
		 * Task isn't running anymore; make it appear like we migrated
		 * it before it went to sleep. This means on wakeup we make the
		 * previous cpu our target instead of where it really is.
		 */
		p->wake_cpu = cpu;
	}
}

struct migration_swap_arg {
	struct task_struct *src_task, *dst_task;
	int src_cpu, dst_cpu;
};

static int migrate_swap_stop(void *data)
{
	struct migration_swap_arg *arg = data;
	struct rq *src_rq, *dst_rq;
	int ret = -EAGAIN;

	if (!cpu_active(arg->src_cpu) || !cpu_active(arg->dst_cpu))
		return -EAGAIN;

	src_rq = cpu_rq(arg->src_cpu);
	dst_rq = cpu_rq(arg->dst_cpu);

	double_raw_lock(&arg->src_task->pi_lock,
			&arg->dst_task->pi_lock);
	double_rq_lock(src_rq, dst_rq);

	if (task_cpu(arg->dst_task) != arg->dst_cpu)
		goto unlock;

	if (task_cpu(arg->src_task) != arg->src_cpu)
		goto unlock;

	if (!cpumask_test_cpu(arg->dst_cpu, tsk_cpus_allowed(arg->src_task)))
		goto unlock;

	if (!cpumask_test_cpu(arg->src_cpu, tsk_cpus_allowed(arg->dst_task)))
		goto unlock;

	__migrate_swap_task(arg->src_task, arg->dst_cpu);
	__migrate_swap_task(arg->dst_task, arg->src_cpu);

	ret = 0;

unlock:
	double_rq_unlock(src_rq, dst_rq);
	raw_spin_unlock(&arg->dst_task->pi_lock);
	raw_spin_unlock(&arg->src_task->pi_lock);

	return ret;
}

/*
 * Cross migrate two tasks
 */
int migrate_swap(struct task_struct *cur, struct task_struct *p)
{
	struct migration_swap_arg arg;
	int ret = -EINVAL;

	arg = (struct migration_swap_arg){
		.src_task = cur,
		.src_cpu = task_cpu(cur),
		.dst_task = p,
		.dst_cpu = task_cpu(p),
	};

	if (arg.src_cpu == arg.dst_cpu)
		goto out;

	/*
	 * These three tests are all lockless; this is OK since all of them
	 * will be re-checked with proper locks held further down the line.
	 */
	if (!cpu_active(arg.src_cpu) || !cpu_active(arg.dst_cpu))
		goto out;

	if (!cpumask_test_cpu(arg.dst_cpu, tsk_cpus_allowed(arg.src_task)))
		goto out;

	if (!cpumask_test_cpu(arg.src_cpu, tsk_cpus_allowed(arg.dst_task)))
		goto out;

	trace_sched_swap_numa(cur, arg.src_cpu, p, arg.dst_cpu);
	ret = stop_two_cpus(arg.dst_cpu, arg.src_cpu, migrate_swap_stop, &arg);

out:
	return ret;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * wait_task_inactive - wait for a thread to unschedule.
 *
 * If @match_state is nonzero, it's the @p->state value just checked and
 * not expected to change.  If it changes, i.e. @p might have woken up,
 * then return zero.  When we succeed in waiting for @p to be off its CPU,
 * we return a positive number (its total switch count).  If a second call
 * a short while later returns the same number, the caller can be sure that
 * @p has remained unscheduled the whole time.
 *
 * The caller must ensure that the task *will* unschedule sometime soon,
 * else this function might spin for a *long* time. This function can't
 * be called with interrupts off, or it may introduce deadlock with
 * smp_call_function() if an IPI is sent by the same process we are
 * waiting to become inactive.
 */
unsigned long wait_task_inactive(struct task_struct *p, long match_state)
{
<<<<<<< HEAD
	unsigned long flags;
	int running, on_rq;
=======
	int running, queued;
	struct rq_flags rf;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long ncsw;
	struct rq *rq;

	for (;;) {
		/*
		 * We do the initial early heuristics without holding
		 * any task-queue locks at all. We'll only try to get
		 * the runqueue lock when things look like they will
		 * work out!
		 */
		rq = task_rq(p);

		/*
		 * If the task is actively running on another CPU
		 * still, just relax and busy-wait without holding
		 * any locks.
		 *
		 * NOTE! Since we don't hold any locks, it's not
		 * even sure that "rq" stays as the right runqueue!
		 * But we don't care, since "task_running()" will
		 * return false if the runqueue has changed and p
		 * is actually now running somewhere else!
		 */
		while (task_running(rq, p)) {
			if (match_state && unlikely(p->state != match_state))
				return 0;
			cpu_relax();
		}

		/*
		 * Ok, time to look more closely! We need the rq
		 * lock now, to be *sure*. If we're wrong, we'll
		 * just go back and repeat.
		 */
<<<<<<< HEAD
		rq = task_rq_lock(p, &flags);
		trace_sched_wait_task(p);
		running = task_running(rq, p);
		on_rq = p->on_rq;
		ncsw = 0;
		if (!match_state || p->state == match_state)
			ncsw = p->nvcsw | LONG_MIN; /* sets MSB */
		task_rq_unlock(rq, p, &flags);
=======
		rq = task_rq_lock(p, &rf);
		trace_sched_wait_task(p);
		running = task_running(rq, p);
		queued = task_on_rq_queued(p);
		ncsw = 0;
		if (!match_state || p->state == match_state)
			ncsw = p->nvcsw | LONG_MIN; /* sets MSB */
		task_rq_unlock(rq, p, &rf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/*
		 * If it changed from the expected state, bail out now.
		 */
		if (unlikely(!ncsw))
			break;

		/*
		 * Was it really running after all now that we
		 * checked with the proper locks actually held?
		 *
		 * Oops. Go back and try again..
		 */
		if (unlikely(running)) {
			cpu_relax();
			continue;
		}

		/*
		 * It's not enough that it's not actively running,
		 * it must be off the runqueue _entirely_, and not
		 * preempted!
		 *
		 * So if it was still runnable (but just not actively
		 * running right now), it's preempted, and we should
		 * yield - it could be a while.
		 */
<<<<<<< HEAD
		if (unlikely(on_rq)) {
=======
		if (unlikely(queued)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ktime_t to = ktime_set(0, NSEC_PER_SEC/HZ);

			set_current_state(TASK_UNINTERRUPTIBLE);
			schedule_hrtimeout(&to, HRTIMER_MODE_REL);
			continue;
		}

		/*
		 * Ahh, all good. It wasn't running, and it wasn't
		 * runnable, which means that it will never become
		 * running in the future either. We're all done!
		 */
		break;
	}

	return ncsw;
}

/***
 * kick_process - kick a running thread to enter/exit the kernel
 * @p: the to-be-kicked thread
 *
 * Cause a process which is running on another CPU to enter
 * kernel-mode, without any delay. (to get signals handled.)
 *
 * NOTE: this function doesn't have to take the runqueue lock,
 * because all it wants to ensure is that the remote task enters
 * the kernel. If the IPI races and the task has been migrated
 * to another CPU then no harm is done and the purpose has been
 * achieved as well.
 */
void kick_process(struct task_struct *p)
{
	int cpu;

	preempt_disable();
	cpu = task_cpu(p);
	if ((cpu != smp_processor_id()) && task_curr(p))
		smp_send_reschedule(cpu);
	preempt_enable();
}
EXPORT_SYMBOL_GPL(kick_process);
<<<<<<< HEAD
#endif /* CONFIG_SMP */

#ifdef CONFIG_SMP
/*
 * ->cpus_allowed is protected by both rq->lock and p->pi_lock
=======

/*
 * ->cpus_allowed is protected by both rq->lock and p->pi_lock
 *
 * A few notes on cpu_active vs cpu_online:
 *
 *  - cpu_active must be a subset of cpu_online
 *
 *  - on cpu-up we allow per-cpu kthreads on the online && !active cpu,
 *    see __set_cpus_allowed_ptr(). At this point the newly online
 *    cpu isn't yet part of the sched domains, and balancing will not
 *    see it.
 *
 *  - on cpu-down we clear cpu_active() to mask the sched domains and
 *    avoid the load balancer to place new tasks on the to be removed
 *    cpu. Existing tasks will remain running there and will be taken
 *    off.
 *
 * This means that fallback selection must not select !active CPUs.
 * And can assume that any active CPU must be online. Conversely
 * select_task_rq() below may allow selection of !active CPUs in order
 * to satisfy the above rules.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static int select_fallback_rq(int cpu, struct task_struct *p)
{
	int nid = cpu_to_node(cpu);
	const struct cpumask *nodemask = NULL;
	enum { cpuset, possible, fail } state = cpuset;
	int dest_cpu;

	/*
	 * If the node that the cpu is on has been offlined, cpu_to_node()
	 * will return -1. There is no cpu on the node, and we should
	 * select the cpu on the other node.
	 */
	if (nid != -1) {
		nodemask = cpumask_of_node(nid);

		/* Look for allowed, online CPU in same node. */
		for_each_cpu(dest_cpu, nodemask) {
<<<<<<< HEAD
			if (!cpu_online(dest_cpu))
				continue;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (!cpu_active(dest_cpu))
				continue;
			if (cpumask_test_cpu(dest_cpu, tsk_cpus_allowed(p)))
				return dest_cpu;
		}
	}

	for (;;) {
		/* Any allowed, online CPU? */
		for_each_cpu(dest_cpu, tsk_cpus_allowed(p)) {
<<<<<<< HEAD
			if (!cpu_online(dest_cpu))
				continue;
			if (!cpu_active(dest_cpu))
=======
			if (!(p->flags & PF_KTHREAD) && !cpu_active(dest_cpu))
				continue;
			if (!cpu_online(dest_cpu))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				continue;
			goto out;
		}

<<<<<<< HEAD
		switch (state) {
		case cpuset:
			/* No more Mr. Nice Guy. */
			cpuset_cpus_allowed_fallback(p);
			state = possible;
			break;

=======
		/* No more Mr. Nice Guy. */
		switch (state) {
		case cpuset:
			if (IS_ENABLED(CONFIG_CPUSETS)) {
				cpuset_cpus_allowed_fallback(p);
				state = possible;
				break;
			}
			/* fall-through */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		case possible:
			do_set_cpus_allowed(p, cpu_possible_mask);
			state = fail;
			break;

		case fail:
			BUG();
			break;
		}
	}

out:
	if (state != cpuset) {
		/*
		 * Don't tell them about moving exiting tasks or
		 * kernel threads (both mm NULL), since they never
		 * leave kernel.
		 */
		if (p->mm && printk_ratelimit()) {
			printk_deferred("process %d (%s) no longer affine to cpu%d\n",
					task_pid_nr(p), p->comm, cpu);
		}
	}

	return dest_cpu;
}

/*
 * The caller (fork, wakeup) owns p->pi_lock, ->cpus_allowed is stable.
 */
static inline
<<<<<<< HEAD
int select_task_rq(struct task_struct *p, int sd_flags, int wake_flags)
{
	int cpu = p->sched_class->select_task_rq(p, sd_flags, wake_flags);
=======
int select_task_rq(struct task_struct *p, int cpu, int sd_flags, int wake_flags)
{
	lockdep_assert_held(&p->pi_lock);

	if (tsk_nr_cpus_allowed(p) > 1)
		cpu = p->sched_class->select_task_rq(p, cpu, sd_flags, wake_flags);
	else
		cpu = cpumask_any(tsk_cpus_allowed(p));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * In order not to call set_task_cpu() on a blocking task we need
	 * to rely on ttwu() to place the task on a valid ->cpus_allowed
	 * cpu.
	 *
	 * Since this is common to all placement strategies, this lives here.
	 *
	 * [ this allows ->select_task() to simply return task_cpu(p) and
	 *   not worry about this generic constraint ]
	 */
	if (unlikely(!cpumask_test_cpu(cpu, tsk_cpus_allowed(p)) ||
		     !cpu_online(cpu)))
		cpu = select_fallback_rq(task_cpu(p), p);

	return cpu;
}

static void update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;
	*avg += diff >> 3;
}
<<<<<<< HEAD
#endif
=======

#else

static inline int __set_cpus_allowed_ptr(struct task_struct *p,
					 const struct cpumask *new_mask, bool check)
{
	return set_cpus_allowed_ptr(p, new_mask);
}

#endif /* CONFIG_SMP */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void
ttwu_stat(struct task_struct *p, int cpu, int wake_flags)
{
<<<<<<< HEAD
#ifdef CONFIG_SCHEDSTATS
	struct rq *rq = this_rq();

#ifdef CONFIG_SMP
	int this_cpu = smp_processor_id();

	if (cpu == this_cpu) {
		schedstat_inc(rq, ttwu_local);
		schedstat_inc(p, se.statistics.nr_wakeups_local);
	} else {
		struct sched_domain *sd;

		schedstat_inc(p, se.statistics.nr_wakeups_remote);
		rcu_read_lock();
		for_each_domain(this_cpu, sd) {
			if (cpumask_test_cpu(cpu, sched_domain_span(sd))) {
				schedstat_inc(sd, ttwu_wake_remote);
=======
	struct rq *rq;

	if (!schedstat_enabled())
		return;

	rq = this_rq();

#ifdef CONFIG_SMP
	if (cpu == rq->cpu) {
		schedstat_inc(rq->ttwu_local);
		schedstat_inc(p->se.statistics.nr_wakeups_local);
	} else {
		struct sched_domain *sd;

		schedstat_inc(p->se.statistics.nr_wakeups_remote);
		rcu_read_lock();
		for_each_domain(rq->cpu, sd) {
			if (cpumask_test_cpu(cpu, sched_domain_span(sd))) {
				schedstat_inc(sd->ttwu_wake_remote);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				break;
			}
		}
		rcu_read_unlock();
	}

	if (wake_flags & WF_MIGRATED)
<<<<<<< HEAD
		schedstat_inc(p, se.statistics.nr_wakeups_migrate);

#endif /* CONFIG_SMP */

	schedstat_inc(rq, ttwu_count);
	schedstat_inc(p, se.statistics.nr_wakeups);

	if (wake_flags & WF_SYNC)
		schedstat_inc(p, se.statistics.nr_wakeups_sync);

#endif /* CONFIG_SCHEDSTATS */
}

static void ttwu_activate(struct rq *rq, struct task_struct *p, int en_flags)
{
	activate_task(rq, p, en_flags);
	p->on_rq = 1;
=======
		schedstat_inc(p->se.statistics.nr_wakeups_migrate);
#endif /* CONFIG_SMP */

	schedstat_inc(rq->ttwu_count);
	schedstat_inc(p->se.statistics.nr_wakeups);

	if (wake_flags & WF_SYNC)
		schedstat_inc(p->se.statistics.nr_wakeups_sync);
}

static inline void ttwu_activate(struct rq *rq, struct task_struct *p, int en_flags)
{
	activate_task(rq, p, en_flags);
	p->on_rq = TASK_ON_RQ_QUEUED;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* if a worker is waking up, notify workqueue */
	if (p->flags & PF_WQ_WORKER)
		wq_worker_waking_up(p, cpu_of(rq));
}

/*
 * Mark the task runnable and perform wakeup-preemption.
 */
<<<<<<< HEAD
static void
ttwu_do_wakeup(struct rq *rq, struct task_struct *p, int wake_flags)
{
	check_preempt_curr(rq, p, wake_flags);
	trace_sched_wakeup(p, true);

	p->state = TASK_RUNNING;
#ifdef CONFIG_SMP
	if (p->sched_class->task_woken)
		p->sched_class->task_woken(rq, p);

	if (rq->idle_stamp) {
		u64 delta = rq->clock - rq->idle_stamp;
		u64 max = 2*sysctl_sched_migration_cost;

		if (delta > max)
			rq->avg_idle = max;
		else
			update_avg(&rq->avg_idle, delta);
=======
static void ttwu_do_wakeup(struct rq *rq, struct task_struct *p, int wake_flags,
			   struct pin_cookie cookie)
{
	check_preempt_curr(rq, p, wake_flags);
	p->state = TASK_RUNNING;
	trace_sched_wakeup(p);

#ifdef CONFIG_SMP
	if (p->sched_class->task_woken) {
		/*
		 * Our task @p is fully woken up and running; so its safe to
		 * drop the rq->lock, hereafter rq is only used for statistics.
		 */
		lockdep_unpin_lock(&rq->lock, cookie);
		p->sched_class->task_woken(rq, p);
		lockdep_repin_lock(&rq->lock, cookie);
	}

	if (rq->idle_stamp) {
		u64 delta = rq_clock(rq) - rq->idle_stamp;
		u64 max = 2*rq->max_idle_balance_cost;

		update_avg(&rq->avg_idle, delta);

		if (rq->avg_idle > max)
			rq->avg_idle = max;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rq->idle_stamp = 0;
	}
#endif
}

static void
<<<<<<< HEAD
ttwu_do_activate(struct rq *rq, struct task_struct *p, int wake_flags)
{
#ifdef CONFIG_SMP
	if (p->sched_contributes_to_load)
		rq->nr_uninterruptible--;
#endif

	ttwu_activate(rq, p, ENQUEUE_WAKEUP | ENQUEUE_WAKING);
	ttwu_do_wakeup(rq, p, wake_flags);
=======
ttwu_do_activate(struct rq *rq, struct task_struct *p, int wake_flags,
		 struct pin_cookie cookie)
{
	int en_flags = ENQUEUE_WAKEUP;

	lockdep_assert_held(&rq->lock);

#ifdef CONFIG_SMP
	if (p->sched_contributes_to_load)
		rq->nr_uninterruptible--;

	if (wake_flags & WF_MIGRATED)
		en_flags |= ENQUEUE_MIGRATED;
#endif

	ttwu_activate(rq, p, en_flags);
	ttwu_do_wakeup(rq, p, wake_flags, cookie);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Called in case the task @p isn't fully descheduled from its runqueue,
 * in this case we must do a remote wakeup. Its a 'light' wakeup though,
 * since all we need to do is flip p->state to TASK_RUNNING, since
 * the task is still ->on_rq.
 */
static int ttwu_remote(struct task_struct *p, int wake_flags)
{
<<<<<<< HEAD
	struct rq *rq;
	int ret = 0;

	rq = __task_rq_lock(p);
	if (p->on_rq) {
		ttwu_do_wakeup(rq, p, wake_flags);
		ret = 1;
	}
	__task_rq_unlock(rq);
=======
	struct rq_flags rf;
	struct rq *rq;
	int ret = 0;

	rq = __task_rq_lock(p, &rf);
	if (task_on_rq_queued(p)) {
		/* check_preempt_curr() may use rq clock */
		update_rq_clock(rq);
		ttwu_do_wakeup(rq, p, wake_flags, rf.cookie);
		ret = 1;
	}
	__task_rq_unlock(rq, &rf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ret;
}

#ifdef CONFIG_SMP
<<<<<<< HEAD
static void sched_ttwu_pending(void)
{
	struct rq *rq = this_rq();
	struct llist_node *llist = llist_del_all(&rq->wake_list);
	struct task_struct *p;

	raw_spin_lock(&rq->lock);

	while (llist) {
		p = llist_entry(llist, struct task_struct, wake_entry);
		llist = llist_next(llist);
		ttwu_do_activate(rq, p, 0);
	}

	raw_spin_unlock(&rq->lock);
=======
void sched_ttwu_pending(void)
{
	struct rq *rq = this_rq();
	struct llist_node *llist = llist_del_all(&rq->wake_list);
	struct pin_cookie cookie;
	struct task_struct *p;
	unsigned long flags;

	if (!llist)
		return;

	raw_spin_lock_irqsave(&rq->lock, flags);
	cookie = lockdep_pin_lock(&rq->lock);

	while (llist) {
		int wake_flags = 0;

		p = llist_entry(llist, struct task_struct, wake_entry);
		llist = llist_next(llist);

		if (p->sched_remote_wakeup)
			wake_flags = WF_MIGRATED;

		ttwu_do_activate(rq, p, wake_flags, cookie);
	}

	lockdep_unpin_lock(&rq->lock, cookie);
	raw_spin_unlock_irqrestore(&rq->lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void scheduler_ipi(void)
{
<<<<<<< HEAD
	if (llist_empty(&this_rq()->wake_list)
			&& !tick_nohz_full_cpu(smp_processor_id())
			&& !got_nohz_idle_kick()
#ifdef CONFIG_SCHED_HMP
			&& !this_rq()->wake_for_idle_pull
#endif
			)
=======
	/*
	 * Fold TIF_NEED_RESCHED into the preempt_count; anybody setting
	 * TIF_NEED_RESCHED remotely (for the first time) will also send
	 * this IPI.
	 */
	preempt_fold_need_resched();

	if (llist_empty(&this_rq()->wake_list) && !got_nohz_idle_kick())
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;

	/*
	 * Not all reschedule IPI handlers call irq_enter/irq_exit, since
	 * traditionally all their work was done from the interrupt return
	 * path. Now that we actually do some work, we need to make sure
	 * we do call them.
	 *
	 * Some archs already do call them, luckily irq_enter/exit nest
	 * properly.
	 *
	 * Arguably we should visit all archs and update all handlers,
	 * however a fair share of IPIs are still resched only so this would
	 * somewhat pessimize the simple resched case.
	 */
	irq_enter();
<<<<<<< HEAD
	tick_nohz_full_check();
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sched_ttwu_pending();

	/*
	 * Check if someone kicked us for doing the nohz idle load balance.
	 */
	if (unlikely(got_nohz_idle_kick())) {
		this_rq()->idle_balance = 1;
		raise_softirq_irqoff(SCHED_SOFTIRQ);
	}
<<<<<<< HEAD
#ifdef CONFIG_SCHED_HMP
	else if (unlikely(this_rq()->wake_for_idle_pull))
		raise_softirq_irqoff(SCHED_SOFTIRQ);
#endif

	irq_exit();
}

static void ttwu_queue_remote(struct task_struct *p, int cpu)
{
	if (llist_add(&p->wake_entry, &cpu_rq(cpu)->wake_list))
		smp_send_reschedule(cpu);
=======
	irq_exit();
}

static void ttwu_queue_remote(struct task_struct *p, int cpu, int wake_flags)
{
	struct rq *rq = cpu_rq(cpu);

	p->sched_remote_wakeup = !!(wake_flags & WF_MIGRATED);

	if (llist_add(&p->wake_entry, &cpu_rq(cpu)->wake_list)) {
		if (!set_nr_if_polling(rq->idle))
			smp_send_reschedule(cpu);
		else
			trace_sched_wake_idle_without_ipi(cpu);
	}
}

void wake_up_if_idle(int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long flags;

	rcu_read_lock();

	if (!is_idle_task(rcu_dereference(rq->curr)))
		goto out;

	if (set_nr_if_polling(rq->idle)) {
		trace_sched_wake_idle_without_ipi(cpu);
	} else {
		raw_spin_lock_irqsave(&rq->lock, flags);
		if (is_idle_task(rq->curr))
			smp_send_reschedule(cpu);
		/* Else cpu is not in idle, do nothing here */
		raw_spin_unlock_irqrestore(&rq->lock, flags);
	}

out:
	rcu_read_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

bool cpus_share_cache(int this_cpu, int that_cpu)
{
	return per_cpu(sd_llc_id, this_cpu) == per_cpu(sd_llc_id, that_cpu);
}
#endif /* CONFIG_SMP */

<<<<<<< HEAD
static void ttwu_queue(struct task_struct *p, int cpu)
{
	struct rq *rq = cpu_rq(cpu);
=======
static void ttwu_queue(struct task_struct *p, int cpu, int wake_flags)
{
	struct rq *rq = cpu_rq(cpu);
	struct pin_cookie cookie;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#if defined(CONFIG_SMP)
	if (sched_feat(TTWU_QUEUE) && !cpus_share_cache(smp_processor_id(), cpu)) {
		sched_clock_cpu(cpu); /* sync clocks x-cpu */
<<<<<<< HEAD
		ttwu_queue_remote(p, cpu);
=======
		ttwu_queue_remote(p, cpu, wake_flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;
	}
#endif

	raw_spin_lock(&rq->lock);
<<<<<<< HEAD
	ttwu_do_activate(rq, p, 0);
	raw_spin_unlock(&rq->lock);
}

=======
	cookie = lockdep_pin_lock(&rq->lock);
	ttwu_do_activate(rq, p, wake_flags, cookie);
	lockdep_unpin_lock(&rq->lock, cookie);
	raw_spin_unlock(&rq->lock);
}

/*
 * Notes on Program-Order guarantees on SMP systems.
 *
 *  MIGRATION
 *
 * The basic program-order guarantee on SMP systems is that when a task [t]
 * migrates, all its activity on its old cpu [c0] happens-before any subsequent
 * execution on its new cpu [c1].
 *
 * For migration (of runnable tasks) this is provided by the following means:
 *
 *  A) UNLOCK of the rq(c0)->lock scheduling out task t
 *  B) migration for t is required to synchronize *both* rq(c0)->lock and
 *     rq(c1)->lock (if not at the same time, then in that order).
 *  C) LOCK of the rq(c1)->lock scheduling in task
 *
 * Transitivity guarantees that B happens after A and C after B.
 * Note: we only require RCpc transitivity.
 * Note: the cpu doing B need not be c0 or c1
 *
 * Example:
 *
 *   CPU0            CPU1            CPU2
 *
 *   LOCK rq(0)->lock
 *   sched-out X
 *   sched-in Y
 *   UNLOCK rq(0)->lock
 *
 *                                   LOCK rq(0)->lock // orders against CPU0
 *                                   dequeue X
 *                                   UNLOCK rq(0)->lock
 *
 *                                   LOCK rq(1)->lock
 *                                   enqueue X
 *                                   UNLOCK rq(1)->lock
 *
 *                   LOCK rq(1)->lock // orders against CPU2
 *                   sched-out Z
 *                   sched-in X
 *                   UNLOCK rq(1)->lock
 *
 *
 *  BLOCKING -- aka. SLEEP + WAKEUP
 *
 * For blocking we (obviously) need to provide the same guarantee as for
 * migration. However the means are completely different as there is no lock
 * chain to provide order. Instead we do:
 *
 *   1) smp_store_release(X->on_cpu, 0)
 *   2) smp_cond_load_acquire(!X->on_cpu)
 *
 * Example:
 *
 *   CPU0 (schedule)  CPU1 (try_to_wake_up) CPU2 (schedule)
 *
 *   LOCK rq(0)->lock LOCK X->pi_lock
 *   dequeue X
 *   sched-out X
 *   smp_store_release(X->on_cpu, 0);
 *
 *                    smp_cond_load_acquire(&X->on_cpu, !VAL);
 *                    X->state = WAKING
 *                    set_task_cpu(X,2)
 *
 *                    LOCK rq(2)->lock
 *                    enqueue X
 *                    X->state = RUNNING
 *                    UNLOCK rq(2)->lock
 *
 *                                          LOCK rq(2)->lock // orders against CPU1
 *                                          sched-out Z
 *                                          sched-in X
 *                                          UNLOCK rq(2)->lock
 *
 *                    UNLOCK X->pi_lock
 *   UNLOCK rq(0)->lock
 *
 *
 * However; for wakeups there is a second guarantee we must provide, namely we
 * must observe the state that lead to our wakeup. That is, not only must our
 * task observe its own prior state, it must also observe the stores prior to
 * its wakeup.
 *
 * This means that any means of doing remote wakeups must order the CPU doing
 * the wakeup against the CPU the task is going to end up running on. This,
 * however, is already required for the regular Program-Order guarantee above,
 * since the waking CPU is the one issueing the ACQUIRE (smp_cond_load_acquire).
 *
 */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * try_to_wake_up - wake up a thread
 * @p: the thread to be awakened
 * @state: the mask of task states that can be woken
 * @wake_flags: wake modifier flags (WF_*)
 *
 * Put it on the run-queue if it's not already there. The "current"
 * thread is always on the run-queue (except when the actual
 * re-schedule is in progress), and as such you're allowed to do
 * the simpler "current->state = TASK_RUNNING" to mark yourself
 * runnable without the overhead of this.
 *
<<<<<<< HEAD
 * Returns %true if @p was woken up, %false if it was already running
 * or @state didn't match @p's state.
 */
int
=======
 * Return: %true if @p was woken up, %false if it was already running.
 * or @state didn't match @p's state.
 */
static int
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
try_to_wake_up(struct task_struct *p, unsigned int state, int wake_flags)
{
	unsigned long flags;
	int cpu, success = 0;
<<<<<<< HEAD
=======
#ifdef CONFIG_SMP
	struct rq *rq;
	u64 wallclock;
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * If we are going to wake up a thread waiting for CONDITION we
	 * need to ensure that CONDITION=1 done by the caller can not be
	 * reordered with p->state check below. This pairs with mb() in
	 * set_current_state() the waiting thread does.
	 */
	smp_mb__before_spinlock();
	raw_spin_lock_irqsave(&p->pi_lock, flags);
	if (!(p->state & state))
		goto out;

<<<<<<< HEAD
=======
	trace_sched_waking(p);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	success = 1; /* we're going to change ->state */
	cpu = task_cpu(p);

	/*
	 * Ensure we load p->on_rq _after_ p->state, otherwise it would
	 * be possible to, falsely, observe p->on_rq == 0 and get stuck
	 * in smp_cond_load_acquire() below.
	 *
	 * sched_ttwu_pending()                 try_to_wake_up()
	 *   [S] p->on_rq = 1;                  [L] P->state
	 *       UNLOCK rq->lock  -----.
	 *                              \
	 *				 +---   RMB
	 * schedule()                   /
	 *       LOCK rq->lock    -----'
	 *       UNLOCK rq->lock
	 *
	 * [task p]
	 *   [S] p->state = UNINTERRUPTIBLE     [L] p->on_rq
	 *
	 * Pairs with the UNLOCK+LOCK on rq->lock from the
	 * last wakeup of our task and the schedule that got our task
	 * current.
	 */
	smp_rmb();
	if (p->on_rq && ttwu_remote(p, wake_flags))
		goto stat;

#ifdef CONFIG_SMP
	/*
	 * Ensure we load p->on_cpu _after_ p->on_rq, otherwise it would be
	 * possible to, falsely, observe p->on_cpu == 0.
	 *
	 * One must be running (->on_cpu == 1) in order to remove oneself
	 * from the runqueue.
	 *
	 *  [S] ->on_cpu = 1;	[L] ->on_rq
	 *      UNLOCK rq->lock
	 *			RMB
	 *      LOCK   rq->lock
	 *  [S] ->on_rq = 0;    [L] ->on_cpu
	 *
	 * Pairs with the full barrier implied in the UNLOCK+LOCK on rq->lock
	 * from the consecutive calls to schedule(); the first switching to our
	 * task, the second putting it to sleep.
	 */
	smp_rmb();

	/*
	 * If the owning (remote) cpu is still in the middle of schedule() with
	 * this task as prev, wait until its done referencing the task.
<<<<<<< HEAD
	 */
	while (p->on_cpu)
		cpu_relax();
	/*
	 * Pairs with the smp_wmb() in finish_lock_switch().
	 */
	smp_rmb();
=======
	 *
	 * Pairs with the smp_store_release() in finish_lock_switch().
	 *
	 * This ensures that tasks getting woken will be fully ordered against
	 * their previous state and preserve Program Order.
	 */
	smp_cond_load_acquire(&p->on_cpu, !VAL);

	rq = cpu_rq(task_cpu(p));

	raw_spin_lock(&rq->lock);
	wallclock = walt_ktime_clock();
	walt_update_task_ravg(rq->curr, rq, TASK_UPDATE, wallclock, 0);
	walt_update_task_ravg(p, rq, TASK_WAKE, wallclock, 0);
	raw_spin_unlock(&rq->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	p->sched_contributes_to_load = !!task_contributes_to_load(p);
	p->state = TASK_WAKING;

<<<<<<< HEAD
	if (p->sched_class->task_waking)
		p->sched_class->task_waking(p);

	cpu = select_task_rq(p, SD_BALANCE_WAKE, wake_flags);
=======
	cpu = select_task_rq(p, p->wake_cpu, SD_BALANCE_WAKE, wake_flags);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (task_cpu(p) != cpu) {
		wake_flags |= WF_MIGRATED;
		set_task_cpu(p, cpu);
	}
<<<<<<< HEAD
#endif /* CONFIG_SMP */

	ttwu_queue(p, cpu);
=======

#endif /* CONFIG_SMP */

	ttwu_queue(p, cpu, wake_flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
stat:
	ttwu_stat(p, cpu, wake_flags);
out:
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	return success;
}
<<<<<<< HEAD
EXPORT_SYMBOL(try_to_wake_up);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * try_to_wake_up_local - try to wake up a local task with rq lock held
 * @p: the thread to be awakened
<<<<<<< HEAD
=======
 * @cookie: context's cookie for pinning
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Put @p on the run-queue if it's not already there. The caller must
 * ensure that this_rq() is locked, @p is bound to this_rq() and not
 * the current task.
 */
<<<<<<< HEAD
static void try_to_wake_up_local(struct task_struct *p)
=======
static void try_to_wake_up_local(struct task_struct *p, struct pin_cookie cookie)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct rq *rq = task_rq(p);

	if (WARN_ON_ONCE(rq != this_rq()) ||
	    WARN_ON_ONCE(p == current))
		return;

	lockdep_assert_held(&rq->lock);

	if (!raw_spin_trylock(&p->pi_lock)) {
<<<<<<< HEAD
		raw_spin_unlock(&rq->lock);
		raw_spin_lock(&p->pi_lock);
		raw_spin_lock(&rq->lock);
=======
		/*
		 * This is OK, because current is on_cpu, which avoids it being
		 * picked for load-balance and preemption/IRQs are still
		 * disabled avoiding further scheduler activity on it and we've
		 * not yet picked a replacement task.
		 */
		lockdep_unpin_lock(&rq->lock, cookie);
		raw_spin_unlock(&rq->lock);
		raw_spin_lock(&p->pi_lock);
		raw_spin_lock(&rq->lock);
		lockdep_repin_lock(&rq->lock, cookie);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (!(p->state & TASK_NORMAL))
		goto out;

<<<<<<< HEAD
	if (!p->on_rq)
		ttwu_activate(rq, p, ENQUEUE_WAKEUP);

	ttwu_do_wakeup(rq, p, 0);
=======
	trace_sched_waking(p);

	if (!task_on_rq_queued(p)) {
		u64 wallclock = walt_ktime_clock();

		walt_update_task_ravg(rq->curr, rq, TASK_UPDATE, wallclock, 0);
		walt_update_task_ravg(p, rq, TASK_WAKE, wallclock, 0);
		ttwu_activate(rq, p, ENQUEUE_WAKEUP);
	}

	ttwu_do_wakeup(rq, p, 0, cookie);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ttwu_stat(p, smp_processor_id(), 0);
out:
	raw_spin_unlock(&p->pi_lock);
}

/**
 * wake_up_process - Wake up a specific process
 * @p: The process to be woken up.
 *
 * Attempt to wake up the nominated process and move it to the set of runnable
<<<<<<< HEAD
 * processes.  Returns 1 if the process was woken up, 0 if it was already
 * running.
=======
 * processes.
 *
 * Return: 1 if the process was woken up, 0 if it was already running.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * It may be assumed that this function implies a write memory barrier before
 * changing the task state if and only if any tasks are woken up.
 */
int wake_up_process(struct task_struct *p)
{
	return try_to_wake_up(p, TASK_NORMAL, 0);
}
EXPORT_SYMBOL(wake_up_process);

int wake_up_state(struct task_struct *p, unsigned int state)
{
	return try_to_wake_up(p, state, 0);
}

<<<<<<< HEAD
#if defined(CONFIG_ARCH_HI3XXX)||defined(CONFIG_ARCH_HI6XXX)
extern unsigned int task_fork_on_bigcluster;
#endif
=======
/*
 * This function clears the sched_dl_entity static params.
 */
void __dl_clear_params(struct task_struct *p)
{
	struct sched_dl_entity *dl_se = &p->dl;

	dl_se->dl_runtime = 0;
	dl_se->dl_deadline = 0;
	dl_se->dl_period = 0;
	dl_se->flags = 0;
	dl_se->dl_bw = 0;
	dl_se->dl_density = 0;

	dl_se->dl_throttled = 0;
	dl_se->dl_yielded = 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Perform scheduler related setup for a newly forked process p.
 * p is forked by current.
 *
 * __sched_fork() is basic setup used by init_idle() too:
 */
<<<<<<< HEAD
static void __sched_fork(struct task_struct *p)
=======
static void __sched_fork(unsigned long clone_flags, struct task_struct *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	p->on_rq			= 0;

	p->se.on_rq			= 0;
	p->se.exec_start		= 0;
	p->se.sum_exec_runtime		= 0;
	p->se.prev_sum_exec_runtime	= 0;
	p->se.nr_migrations		= 0;
	p->se.vruntime			= 0;
	INIT_LIST_HEAD(&p->se.group_node);
<<<<<<< HEAD

/*
 * Load-tracking only depends on SMP, FAIR_GROUP_SCHED dependency below may be
 * removed when useful for applications beyond shares distribution (e.g.
 * load-balance).
 */
#if defined(CONFIG_SMP) && defined(CONFIG_FAIR_GROUP_SCHED)
	p->se.avg.runnable_avg_period = 0;
	p->se.avg.runnable_avg_sum = 0;
#ifdef CONFIG_SCHED_HMP
	/* keep LOAD_AVG_MAX in sync with fair.c if load avg series is changed */
#define LOAD_AVG_MAX 47742	
#define LOAD_AVG_MIN 0

	p->se.avg.hmp_last_up_migration = 0;
	p->se.avg.hmp_last_down_migration = 0;
#if defined(CONFIG_ARCH_HI3XXX)||defined(CONFIG_ARCH_HI6XXX)
	if(!task_fork_on_bigcluster){
            p->se.avg.load_avg_ratio = 0;
            p->se.avg.load_avg_contrib =
                    (0 * scale_load_down(p->se.load.weight));
            p->se.avg.runnable_avg_period = LOAD_AVG_MIN;
            p->se.avg.runnable_avg_sum = LOAD_AVG_MIN;
            p->se.avg.usage_avg_sum = LOAD_AVG_MIN;
    }else if(hmp_task_should_forkboost(p)){
		    p->se.avg.load_avg_ratio = 1023;
		    p->se.avg.load_avg_contrib =
				    (1023 * scale_load_down(p->se.load.weight));
		    p->se.avg.runnable_avg_period = LOAD_AVG_MAX;
		    p->se.avg.runnable_avg_sum = LOAD_AVG_MAX;
		    p->se.avg.usage_avg_sum = LOAD_AVG_MAX;
	}
#else
	if(hmp_task_should_forkboost(p)){
	    p->se.avg.load_avg_ratio = 1023;
	    p->se.avg.load_avg_contrib =
			    (1023 * scale_load_down(p->se.load.weight));
	    p->se.avg.runnable_avg_period = LOAD_AVG_MAX;
	    p->se.avg.runnable_avg_sum = LOAD_AVG_MAX;
	    p->se.avg.usage_avg_sum = LOAD_AVG_MAX;
	}
#endif    
#endif
#endif
#ifdef CONFIG_SCHEDSTATS
	memset(&p->se.statistics, 0, sizeof(p->se.statistics));
#endif

	INIT_LIST_HEAD(&p->rt.run_list);
=======
	walt_init_new_task_load(p);

#ifdef CONFIG_FAIR_GROUP_SCHED
	p->se.cfs_rq			= NULL;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* Even if schedstat is disabled, there should not be garbage */
	memset(&p->se.statistics, 0, sizeof(p->se.statistics));
#endif

	RB_CLEAR_NODE(&p->dl.rb_node);
	init_dl_task_timer(&p->dl);
	__dl_clear_params(p);

	INIT_LIST_HEAD(&p->rt.run_list);
	p->rt.timeout		= 0;
	p->rt.time_slice	= sched_rr_timeslice;
	p->rt.on_rq		= 0;
	p->rt.on_list		= 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_PREEMPT_NOTIFIERS
	INIT_HLIST_HEAD(&p->preempt_notifiers);
#endif

#ifdef CONFIG_NUMA_BALANCING
	if (p->mm && atomic_read(&p->mm->mm_users) == 1) {
<<<<<<< HEAD
		p->mm->numa_next_scan = jiffies;
		p->mm->numa_next_reset = jiffies;
		p->mm->numa_scan_seq = 0;
	}

	p->node_stamp = 0ULL;
	p->numa_scan_seq = p->mm ? p->mm->numa_scan_seq : 0;
	p->numa_migrate_seq = p->mm ? p->mm->numa_scan_seq - 1 : 0;
	p->numa_scan_period = sysctl_numa_balancing_scan_delay;
	p->numa_work.next = &p->numa_work;
#endif /* CONFIG_NUMA_BALANCING */
}

#ifdef CONFIG_NUMA_BALANCING
#ifdef CONFIG_SCHED_DEBUG
void set_numabalancing_state(bool enabled)
{
	if (enabled)
		sched_feat_set("NUMA");
	else
		sched_feat_set("NO_NUMA");
}
#else
__read_mostly bool numabalancing_enabled;

void set_numabalancing_state(bool enabled)
{
	numabalancing_enabled = enabled;
}
#endif /* CONFIG_SCHED_DEBUG */
#endif /* CONFIG_NUMA_BALANCING */
=======
		p->mm->numa_next_scan = jiffies + msecs_to_jiffies(sysctl_numa_balancing_scan_delay);
		p->mm->numa_scan_seq = 0;
	}

	if (clone_flags & CLONE_VM)
		p->numa_preferred_nid = current->numa_preferred_nid;
	else
		p->numa_preferred_nid = -1;

	p->node_stamp = 0ULL;
	p->numa_scan_seq = p->mm ? p->mm->numa_scan_seq : 0;
	p->numa_scan_period = sysctl_numa_balancing_scan_delay;
	p->numa_work.next = &p->numa_work;
	p->numa_faults = NULL;
	p->last_task_numa_placement = 0;
	p->last_sum_exec_runtime = 0;

	p->numa_group = NULL;
#endif /* CONFIG_NUMA_BALANCING */
}

DEFINE_STATIC_KEY_FALSE(sched_numa_balancing);

#ifdef CONFIG_NUMA_BALANCING

void set_numabalancing_state(bool enabled)
{
	if (enabled)
		static_branch_enable(&sched_numa_balancing);
	else
		static_branch_disable(&sched_numa_balancing);
}

#ifdef CONFIG_PROC_SYSCTL
int sysctl_numa_balancing(struct ctl_table *table, int write,
			 void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table t;
	int err;
	int state = static_branch_likely(&sched_numa_balancing);

	if (write && !capable(CAP_SYS_ADMIN))
		return -EPERM;

	t = *table;
	t.data = &state;
	err = proc_dointvec_minmax(&t, write, buffer, lenp, ppos);
	if (err < 0)
		return err;
	if (write)
		set_numabalancing_state(state);
	return err;
}
#endif
#endif

#ifdef CONFIG_SCHEDSTATS

DEFINE_STATIC_KEY_FALSE(sched_schedstats);
static bool __initdata __sched_schedstats = false;

static void set_schedstats(bool enabled)
{
	if (enabled)
		static_branch_enable(&sched_schedstats);
	else
		static_branch_disable(&sched_schedstats);
}

void force_schedstat_enabled(void)
{
	if (!schedstat_enabled()) {
		pr_info("kernel profiling enabled schedstats, disable via kernel.sched_schedstats.\n");
		static_branch_enable(&sched_schedstats);
	}
}

static int __init setup_schedstats(char *str)
{
	int ret = 0;
	if (!str)
		goto out;

	/*
	 * This code is called before jump labels have been set up, so we can't
	 * change the static branch directly just yet.  Instead set a temporary
	 * variable so init_schedstats() can do it later.
	 */
	if (!strcmp(str, "enable")) {
		__sched_schedstats = true;
		ret = 1;
	} else if (!strcmp(str, "disable")) {
		__sched_schedstats = false;
		ret = 1;
	}
out:
	if (!ret)
		pr_warn("Unable to parse schedstats=\n");

	return ret;
}
__setup("schedstats=", setup_schedstats);

static void __init init_schedstats(void)
{
	set_schedstats(__sched_schedstats);
}

#ifdef CONFIG_PROC_SYSCTL
int sysctl_schedstats(struct ctl_table *table, int write,
			 void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table t;
	int err;
	int state = static_branch_likely(&sched_schedstats);

	if (write && !capable(CAP_SYS_ADMIN))
		return -EPERM;

	t = *table;
	t.data = &state;
	err = proc_dointvec_minmax(&t, write, buffer, lenp, ppos);
	if (err < 0)
		return err;
	if (write)
		set_schedstats(state);
	return err;
}
#endif /* CONFIG_PROC_SYSCTL */
#else  /* !CONFIG_SCHEDSTATS */
static inline void init_schedstats(void) {}
#endif /* CONFIG_SCHEDSTATS */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * fork()/clone()-time setup:
 */
<<<<<<< HEAD
void sched_fork(struct task_struct *p)
=======
int sched_fork(unsigned long clone_flags, struct task_struct *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned long flags;
	int cpu = get_cpu();

<<<<<<< HEAD
	__sched_fork(p);
	/*
	 * We mark the process as running here. This guarantees that
	 * nobody will actually run it, and a signal or other external
	 * event cannot wake it up and insert it on the runqueue either.
	 */
	p->state = TASK_RUNNING;
=======
	__sched_fork(clone_flags, p);
	/*
	 * We mark the process as NEW here. This guarantees that
	 * nobody will actually run it, and a signal or other external
	 * event cannot wake it up and insert it on the runqueue either.
	 */
	p->state = TASK_NEW;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Make sure we do not leak PI boosting priority to the child.
	 */
	p->prio = current->normal_prio;

	/*
	 * Revert to default priority/policy on fork if requested.
	 */
	if (unlikely(p->sched_reset_on_fork)) {
<<<<<<< HEAD
		if (task_has_rt_policy(p)) {
=======
		if (task_has_dl_policy(p) || task_has_rt_policy(p)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			p->policy = SCHED_NORMAL;
			p->static_prio = NICE_TO_PRIO(0);
			p->rt_priority = 0;
		} else if (PRIO_TO_NICE(p->static_prio) < 0)
			p->static_prio = NICE_TO_PRIO(0);

		p->prio = p->normal_prio = __normal_prio(p);
		set_load_weight(p);

		/*
		 * We don't need the reset flag anymore after the fork. It has
		 * fulfilled its duty:
		 */
		p->sched_reset_on_fork = 0;
	}

<<<<<<< HEAD
	if (!rt_prio(p->prio))
		p->sched_class = &fair_sched_class;

	if (p->sched_class->task_fork)
		p->sched_class->task_fork(p);
=======
	if (dl_prio(p->prio)) {
		put_cpu();
		return -EAGAIN;
	} else if (rt_prio(p->prio)) {
		p->sched_class = &rt_sched_class;
	} else {
		p->sched_class = &fair_sched_class;
	}

	init_entity_runnable_average(&p->se);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * The child is not yet in the pid-hash so no cgroup attach races,
	 * and the cgroup is pinned to this child due to cgroup_fork()
	 * is ran before sched_fork().
	 *
	 * Silence PROVE_RCU.
	 */
	raw_spin_lock_irqsave(&p->pi_lock, flags);
<<<<<<< HEAD
	set_task_cpu(p, cpu);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

#if defined(CONFIG_SCHEDSTATS) || defined(CONFIG_TASK_DELAY_ACCT)
=======
	/*
	 * We're setting the cpu for the first time, we don't migrate,
	 * so use __set_task_cpu().
	 */
	__set_task_cpu(p, cpu);
	if (p->sched_class->task_fork)
		p->sched_class->task_fork(p);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

#ifdef CONFIG_SCHED_INFO
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (likely(sched_info_on()))
		memset(&p->sched_info, 0, sizeof(p->sched_info));
#endif
#if defined(CONFIG_SMP)
	p->on_cpu = 0;
#endif
<<<<<<< HEAD
#ifdef CONFIG_PREEMPT_COUNT
	/* Want to start with kernel preemption disabled. */
	task_thread_info(p)->preempt_count = 1;
#endif
#ifdef CONFIG_SMP
	plist_node_init(&p->pushable_tasks, MAX_PRIO);
#endif

	put_cpu();
}

/*
 * wake_up_new_task - wake up a newly created task for the first time.
 *
 * This function will do some initial scheduler statistics housekeeping
 * that must be done for every newly created context, then puts the task
 * on the runqueue and wakes it.
 */
void wake_up_new_task(struct task_struct *p)
{
	unsigned long flags;
	struct rq *rq;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
#ifdef CONFIG_SMP
	/*
	 * Fork balancing, do it here and not earlier because:
	 *  - cpus_allowed can change in the fork path
	 *  - any previously selected cpu might disappear through hotplug
	 */
	set_task_cpu(p, select_task_rq(p, SD_BALANCE_FORK, 0));
#endif

	rq = __task_rq_lock(p);
	activate_task(rq, p, 0);
	p->on_rq = 1;
	trace_sched_wakeup_new(p, true);
	check_preempt_curr(rq, p, WF_FORK);
#ifdef CONFIG_SMP
	if (p->sched_class->task_woken)
		p->sched_class->task_woken(rq, p);
#endif
	task_rq_unlock(rq, p, &flags);
}

#ifdef CONFIG_PREEMPT_NOTIFIERS

/**
 * preempt_notifier_register - tell me when current is being preempted & rescheduled
 * @notifier: notifier struct to register
 */
void preempt_notifier_register(struct preempt_notifier *notifier)
{
	hlist_add_head(&notifier->link, &current->preempt_notifiers);
}
EXPORT_SYMBOL_GPL(preempt_notifier_register);

/**
 * preempt_notifier_unregister - no longer interested in preemption notifications
 * @notifier: notifier struct to unregister
 *
 * This is safe to call from within a preemption notifier.
=======
	init_task_preempt_count(p);
#ifdef CONFIG_SMP
	plist_node_init(&p->pushable_tasks, MAX_PRIO);
	RB_CLEAR_NODE(&p->pushable_dl_tasks);
#endif

	put_cpu();
	return 0;
}

unsigned long to_ratio(u64 period, u64 runtime)
{
	if (runtime == RUNTIME_INF)
		return 1ULL << 20;

	/*
	 * Doing this here saves a lot of checks in all
	 * the calling paths, and returning zero seems
	 * safe for them anyway.
	 */
	if (period == 0)
		return 0;

	return div64_u64(runtime << 20, period);
}

#ifdef CONFIG_SMP
inline struct dl_bw *dl_bw_of(int i)
{
	RCU_LOCKDEP_WARN(!rcu_read_lock_sched_held(),
			 "sched RCU must be held");
	return &cpu_rq(i)->rd->dl_bw;
}

static inline int dl_bw_cpus(int i)
{
	struct root_domain *rd = cpu_rq(i)->rd;
	int cpus = 0;

	RCU_LOCKDEP_WARN(!rcu_read_lock_sched_held(),
			 "sched RCU must be held");
	for_each_cpu_and(i, rd->span, cpu_active_mask)
		cpus++;

	return cpus;
}
#else
inline struct dl_bw *dl_bw_of(int i)
{
	return &cpu_rq(i)->dl.dl_bw;
}

static inline int dl_bw_cpus(int i)
{
	return 1;
}
#endif

/*
 * We must be sure that accepting a new task (or allowing changing the
 * parameters of an existing one) is consistent with the bandwidth
 * constraints. If yes, this function also accordingly updates the currently
 * allocated bandwidth to reflect the new situation.
 *
 * This function is called while holding p's rq->lock.
 *
 * XXX we should delay bw change until the task's 0-lag point, see
 * __setparam_dl().
 */
static int dl_overflow(struct task_struct *p, int policy,
		       const struct sched_attr *attr)
{

	struct dl_bw *dl_b = dl_bw_of(task_cpu(p));
	u64 period = attr->sched_period ?: attr->sched_deadline;
	u64 runtime = attr->sched_runtime;
	u64 new_bw = dl_policy(policy) ? to_ratio(period, runtime) : 0;
	int cpus, err = -1;

	/* !deadline task may carry old deadline bandwidth */
	if (new_bw == p->dl.dl_bw && task_has_dl_policy(p))
		return 0;

	/*
	 * Either if a task, enters, leave, or stays -deadline but changes
	 * its parameters, we may need to update accordingly the total
	 * allocated bandwidth of the container.
	 */
	raw_spin_lock(&dl_b->lock);
	cpus = dl_bw_cpus(task_cpu(p));
	if (dl_policy(policy) && !task_has_dl_policy(p) &&
	    !__dl_overflow(dl_b, cpus, 0, new_bw)) {
		__dl_add(dl_b, new_bw);
		err = 0;
	} else if (dl_policy(policy) && task_has_dl_policy(p) &&
		   !__dl_overflow(dl_b, cpus, p->dl.dl_bw, new_bw)) {
		__dl_clear(dl_b, p->dl.dl_bw);
		__dl_add(dl_b, new_bw);
		err = 0;
	} else if (!dl_policy(policy) && task_has_dl_policy(p)) {
		__dl_clear(dl_b, p->dl.dl_bw);
		err = 0;
	}
	raw_spin_unlock(&dl_b->lock);

	return err;
}

extern void init_dl_bw(struct dl_bw *dl_b);

/*
 * wake_up_new_task - wake up a newly created task for the first time.
 *
 * This function will do some initial scheduler statistics housekeeping
 * that must be done for every newly created context, then puts the task
 * on the runqueue and wakes it.
 */
void wake_up_new_task(struct task_struct *p)
{
	struct rq_flags rf;
	struct rq *rq;

	raw_spin_lock_irqsave(&p->pi_lock, rf.flags);

	walt_init_new_task_load(p);

	p->state = TASK_RUNNING;
#ifdef CONFIG_SMP
	/*
	 * Fork balancing, do it here and not earlier because:
	 *  - cpus_allowed can change in the fork path
	 *  - any previously selected cpu might disappear through hotplug
	 *
	 * Use __set_task_cpu() to avoid calling sched_class::migrate_task_rq,
	 * as we're not fully set-up yet.
	 */
	__set_task_cpu(p, select_task_rq(p, task_cpu(p), SD_BALANCE_FORK, 0));
#endif
	rq = __task_rq_lock(p, &rf);
	post_init_entity_util_avg(&p->se);

	walt_mark_task_starting(p);

	activate_task(rq, p, ENQUEUE_WAKEUP_NEW);
	p->on_rq = TASK_ON_RQ_QUEUED;
	trace_sched_wakeup_new(p);
	check_preempt_curr(rq, p, WF_FORK);
#ifdef CONFIG_SMP
	if (p->sched_class->task_woken) {
		/*
		 * Nothing relies on rq->lock after this, so its fine to
		 * drop it.
		 */
		lockdep_unpin_lock(&rq->lock, rf.cookie);
		p->sched_class->task_woken(rq, p);
		lockdep_repin_lock(&rq->lock, rf.cookie);
	}
#endif
	task_rq_unlock(rq, p, &rf);
}

#ifdef CONFIG_PREEMPT_NOTIFIERS

static struct static_key preempt_notifier_key = STATIC_KEY_INIT_FALSE;

void preempt_notifier_inc(void)
{
	static_key_slow_inc(&preempt_notifier_key);
}
EXPORT_SYMBOL_GPL(preempt_notifier_inc);

void preempt_notifier_dec(void)
{
	static_key_slow_dec(&preempt_notifier_key);
}
EXPORT_SYMBOL_GPL(preempt_notifier_dec);

/**
 * preempt_notifier_register - tell me when current is being preempted & rescheduled
 * @notifier: notifier struct to register
 */
void preempt_notifier_register(struct preempt_notifier *notifier)
{
	if (!static_key_false(&preempt_notifier_key))
		WARN(1, "registering preempt_notifier while notifiers disabled\n");

	hlist_add_head(&notifier->link, &current->preempt_notifiers);
}
EXPORT_SYMBOL_GPL(preempt_notifier_register);

/**
 * preempt_notifier_unregister - no longer interested in preemption notifications
 * @notifier: notifier struct to unregister
 *
 * This is *not* safe to call from within a preemption notifier.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
void preempt_notifier_unregister(struct preempt_notifier *notifier)
{
	hlist_del(&notifier->link);
}
EXPORT_SYMBOL_GPL(preempt_notifier_unregister);

<<<<<<< HEAD
static void fire_sched_in_preempt_notifiers(struct task_struct *curr)
=======
static void __fire_sched_in_preempt_notifiers(struct task_struct *curr)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct preempt_notifier *notifier;

	hlist_for_each_entry(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_in(notifier, raw_smp_processor_id());
}

<<<<<<< HEAD
static void
fire_sched_out_preempt_notifiers(struct task_struct *curr,
				 struct task_struct *next)
=======
static __always_inline void fire_sched_in_preempt_notifiers(struct task_struct *curr)
{
	if (static_key_false(&preempt_notifier_key))
		__fire_sched_in_preempt_notifiers(curr);
}

static void
__fire_sched_out_preempt_notifiers(struct task_struct *curr,
				   struct task_struct *next)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct preempt_notifier *notifier;

	hlist_for_each_entry(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_out(notifier, next);
}

<<<<<<< HEAD
#else /* !CONFIG_PREEMPT_NOTIFIERS */

static void fire_sched_in_preempt_notifiers(struct task_struct *curr)
{
}

static void
=======
static __always_inline void
fire_sched_out_preempt_notifiers(struct task_struct *curr,
				 struct task_struct *next)
{
	if (static_key_false(&preempt_notifier_key))
		__fire_sched_out_preempt_notifiers(curr, next);
}

#else /* !CONFIG_PREEMPT_NOTIFIERS */

static inline void fire_sched_in_preempt_notifiers(struct task_struct *curr)
{
}

static inline void
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
fire_sched_out_preempt_notifiers(struct task_struct *curr,
				 struct task_struct *next)
{
}

#endif /* CONFIG_PREEMPT_NOTIFIERS */

/**
 * prepare_task_switch - prepare to switch tasks
 * @rq: the runqueue preparing to switch
 * @prev: the current task that is being switched out
 * @next: the task we are going to switch to.
 *
 * This is called with the rq lock held and interrupts off. It must
 * be paired with a subsequent finish_task_switch after the context
 * switch.
 *
 * prepare_task_switch sets up locking and calls architecture specific
 * hooks.
 */
static inline void
prepare_task_switch(struct rq *rq, struct task_struct *prev,
		    struct task_struct *next)
{
<<<<<<< HEAD
	trace_sched_switch(prev, next);
	sched_info_switch(prev, next);
=======
	sched_info_switch(rq, prev, next);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	perf_event_task_sched_out(prev, next);
	fire_sched_out_preempt_notifiers(prev, next);
	prepare_lock_switch(rq, next);
	prepare_arch_switch(next);
}

/**
 * finish_task_switch - clean up after a task-switch
<<<<<<< HEAD
 * @rq: runqueue associated with task-switch
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @prev: the thread we just switched away from.
 *
 * finish_task_switch must be called after the context switch, paired
 * with a prepare_task_switch call before the context switch.
 * finish_task_switch will reconcile locking set up by prepare_task_switch,
 * and do any other architecture-specific cleanup actions.
 *
 * Note that we may have delayed dropping an mm in context_switch(). If
 * so, we finish that here outside of the runqueue lock. (Doing it
 * with the lock held can cause deadlocks; see schedule() for
 * details.)
<<<<<<< HEAD
 */
static void finish_task_switch(struct rq *rq, struct task_struct *prev)
	__releases(rq->lock)
{
	struct mm_struct *mm = rq->prev_mm;
	long prev_state;

=======
 *
 * The context switch have flipped the stack from under us and restored the
 * local variables which were saved when this task called schedule() in the
 * past. prev == current is still correct but we need to recalculate this_rq
 * because prev may have moved to another CPU.
 */
static struct rq *finish_task_switch(struct task_struct *prev)
	__releases(rq->lock)
{
	struct rq *rq = this_rq();
	struct mm_struct *mm = rq->prev_mm;
	long prev_state;

	/*
	 * The previous task will have left us with a preempt_count of 2
	 * because it left us after:
	 *
	 *	schedule()
	 *	  preempt_disable();			// 1
	 *	  __schedule()
	 *	    raw_spin_lock_irq(&rq->lock)	// 2
	 *
	 * Also, see FORK_PREEMPT_COUNT.
	 */
	if (WARN_ONCE(preempt_count() != 2*PREEMPT_DISABLE_OFFSET,
		      "corrupted preempt_count: %s/%d/0x%x\n",
		      current->comm, current->pid, preempt_count()))
		preempt_count_set(FORK_PREEMPT_COUNT);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rq->prev_mm = NULL;

	/*
	 * A task struct has one reference for the use as "current".
	 * If a task dies, then it sets TASK_DEAD in tsk->state and calls
	 * schedule one last time. The schedule call will never return, and
	 * the scheduled task must drop that reference.
<<<<<<< HEAD
	 * The test for TASK_DEAD must occur while the runqueue locks are
	 * still held, otherwise prev could be scheduled on another cpu, die
	 * there before we look at prev->state, and then the reference would
	 * be dropped twice.
	 *		Manfred Spraul <manfred@colorfullife.com>
	 */
	prev_state = prev->state;
	vtime_task_switch(prev);
	finish_arch_switch(prev);
=======
	 *
	 * We must observe prev->state before clearing prev->on_cpu (in
	 * finish_lock_switch), otherwise a concurrent wakeup can get prev
	 * running on another CPU and we could rave with its RUNNING -> DEAD
	 * transition, resulting in a double drop.
	 */
	prev_state = prev->state;
	vtime_task_switch(prev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	perf_event_task_sched_in(prev, current);
	finish_lock_switch(rq, prev);
	finish_arch_post_lock_switch();

	fire_sched_in_preempt_notifiers(current);
	if (mm)
		mmdrop(mm);
	if (unlikely(prev_state == TASK_DEAD)) {
<<<<<<< HEAD
=======
		if (prev->sched_class->task_dead)
			prev->sched_class->task_dead(prev);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * Remove function-return probe instances associated with this
		 * task and put them back on the free list.
		 */
		kprobe_flush_task(prev);
<<<<<<< HEAD
		put_task_struct(prev);
	}

	tick_nohz_task_switch(current);
=======

		/* Task is done with its stack. */
		put_task_stack(prev);

		put_task_struct(prev);
	}

	tick_nohz_task_switch();
	return rq;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifdef CONFIG_SMP

<<<<<<< HEAD
/* assumes rq->lock is held */
static inline void pre_schedule(struct rq *rq, struct task_struct *prev)
{
	if (prev->sched_class->pre_schedule)
		prev->sched_class->pre_schedule(rq, prev);
}

/* rq->lock is NOT held, but preemption is disabled */
static inline void post_schedule(struct rq *rq)
{
	if (rq->post_schedule) {
		unsigned long flags;

		raw_spin_lock_irqsave(&rq->lock, flags);
		if (rq->curr->sched_class->post_schedule)
			rq->curr->sched_class->post_schedule(rq);
		raw_spin_unlock_irqrestore(&rq->lock, flags);

		rq->post_schedule = 0;
	}
}

#else

static inline void pre_schedule(struct rq *rq, struct task_struct *p)
{
}

static inline void post_schedule(struct rq *rq)
=======
/* rq->lock is NOT held, but preemption is disabled */
static void __balance_callback(struct rq *rq)
{
	struct callback_head *head, *next;
	void (*func)(struct rq *rq);
	unsigned long flags;

	raw_spin_lock_irqsave(&rq->lock, flags);
	head = rq->balance_callback;
	rq->balance_callback = NULL;
	while (head) {
		func = (void (*)(struct rq *))head->func;
		next = head->next;
		head->next = NULL;
		head = next;

		func(rq);
	}
	raw_spin_unlock_irqrestore(&rq->lock, flags);
}

static inline void balance_callback(struct rq *rq)
{
	if (unlikely(rq->balance_callback))
		__balance_callback(rq);
}

#else

static inline void balance_callback(struct rq *rq)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
}

#endif

/**
 * schedule_tail - first thing a freshly forked thread must call.
 * @prev: the thread we just switched away from.
 */
<<<<<<< HEAD
asmlinkage void schedule_tail(struct task_struct *prev)
	__releases(rq->lock)
{
	struct rq *rq = this_rq();

	finish_task_switch(rq, prev);

	/*
	 * FIXME: do we need to worry about rq being invalidated by the
	 * task_switch?
	 */
	post_schedule(rq);

#ifdef __ARCH_WANT_UNLOCKED_CTXSW
	/* In this case, finish_task_switch does not reenable preemption */
	preempt_enable();
#endif
=======
asmlinkage __visible void schedule_tail(struct task_struct *prev)
	__releases(rq->lock)
{
	struct rq *rq;

	/*
	 * New tasks start with FORK_PREEMPT_COUNT, see there and
	 * finish_task_switch() for details.
	 *
	 * finish_task_switch() will drop rq->lock() and lower preempt_count
	 * and the preempt_enable() will end up enabling preemption (on
	 * PREEMPT_COUNT kernels).
	 */

	rq = finish_task_switch(prev);
	balance_callback(rq);
	preempt_enable();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (current->set_child_tid)
		put_user(task_pid_vnr(current), current->set_child_tid);
}

<<<<<<< HEAD
#include <linux/huawei/rdr_private.h>
#ifdef CONFIG_HISI_RDR
#define MAX_TASK_SWITCH_RTN 3 /* max task switch callout routines */
static rdr_funcptr_2 task_switch_ls[MAX_TASK_SWITCH_RTN] = { NULL };

/**
 * param1:hook:routine to be added to table
 * param2:table:table which to add
 * param3:max_entries:max entries in table
*/
int hook_add_to_tail(void *hook, void *table[], int max_entries)
{
	int ix;

	if (table == NULL)
		return -1;

	preempt_disable(); /* disable task switching */

	/* find a slot after last hook in table */
	for (ix = 0; ix < max_entries; ++ix) {
		if (table[ix] == NULL) {
			table[ix] = hook;
			preempt_enable(); /* re-enable task switching */
			return 0;
		}
	}

	/* no free slot found */
	preempt_enable(); /* re-enable task switching */

	return -1;
}

int task_switch_hook_add(rdr_funcptr_2 switch_hook)
{
	/* translate hookLib errno's to taskLib errno's */
	return hook_add_to_tail(switch_hook, (void **)task_switch_ls,
			MAX_TASK_SWITCH_RTN);
}
EXPORT_SYMBOL(task_switch_hook_add);

int task_switch_hook_del(rdr_funcptr_2 switch_hook)
{
	int ix;

	preempt_disable();/* disable task switching */

	/* find a slot after last hook in table */
	for (ix = 0; ix < MAX_TASK_SWITCH_RTN; ++ix) {
		if (task_switch_ls[ix] == switch_hook) {
			task_switch_ls[ix] = NULL;
			preempt_enable(); /* re-enable task switching */
			return 0;
		}
	}

	/* no free slot found */
	preempt_enable(); /* re-enable task switching */

	return -1;
}
EXPORT_SYMBOL(task_switch_hook_del);
#else
#define MAX_TASK_SWITCH_RTN 3 /* max task switch callout routines */
static rdr_funcptr_2 task_switch_ls[MAX_TASK_SWITCH_RTN] = { NULL };

/**
 * param1:hook:routine to be added to table
 * param2:table:table which to add
 * param3:max_entries:max entries in table
*/
int hook_add_to_tail(void *hook, void *table[], int max_entries)
{
       int ix;

       if (table == NULL)
               return -1;

       preempt_disable(); /* disable task switching */

       /* find a slot after last hook in table */
       for (ix = 0; ix < max_entries; ++ix) {
               if (table[ix] == NULL) {
                       table[ix] = hook;
                       preempt_enable(); /* re-enable task switching */
                       return 0;
               }
       }

       /* no free slot found */
       preempt_enable(); /* re-enable task switching */

       return -1;
}

int task_switch_hook_add(rdr_funcptr_2 switch_hook)
{
       /* translate hookLib errno's to taskLib errno's */
       return hook_add_to_tail(switch_hook, (void **)task_switch_ls,
                       MAX_TASK_SWITCH_RTN);
}
EXPORT_SYMBOL(task_switch_hook_add);

int task_switch_hook_del(rdr_funcptr_2 switch_hook)
{
       int ix;

       preempt_disable();/* disable task switching */

       /* find a slot after last hook in table */
       for (ix = 0; ix < MAX_TASK_SWITCH_RTN; ++ix) {
               if (task_switch_ls[ix] == switch_hook) {
                       task_switch_ls[ix] = NULL;
                       preempt_enable(); /* re-enable task switching */
                       return 0;
               }
       }

       /* no free slot found */
       preempt_enable(); /* re-enable task switching */

       return -1;
}
EXPORT_SYMBOL(task_switch_hook_del);

#if 0
int hook_add_to_tail(void *hook, void *table[], int max_entries)
{
	return -1;
}

int task_switch_hook_add(rdr_funcptr_2 switch_hook)
{
	return -1;
}

int task_switch_hook_del(rdr_funcptr_2 switch_hook)
{
	return -1;
}
#endif
#endif
/*
 * context_switch - switch to the new MM and the new
 * thread's register state.
 */
static inline void
context_switch(struct rq *rq, struct task_struct *prev,
	       struct task_struct *next)
=======
/*
 * context_switch - switch to the new MM and the new thread's register state.
 */
static __always_inline struct rq *
context_switch(struct rq *rq, struct task_struct *prev,
	       struct task_struct *next, struct pin_cookie cookie)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct mm_struct *mm, *oldmm;

	prepare_task_switch(rq, prev, next);

	mm = next->mm;
	oldmm = prev->active_mm;
	/*
	 * For paravirt, this is coupled with an exit in switch_to to
	 * combine the page table reload and the switch backend into
	 * one hypercall.
	 */
	arch_start_context_switch(prev);

	if (!mm) {
		next->active_mm = oldmm;
		atomic_inc(&oldmm->mm_count);
		enter_lazy_tlb(oldmm, next);
	} else
<<<<<<< HEAD
		switch_mm(oldmm, mm, next);
=======
		switch_mm_irqs_off(oldmm, mm, next);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!prev->mm) {
		prev->active_mm = NULL;
		rq->prev_mm = oldmm;
	}
	/*
	 * Since the runqueue lock will be released by the next
	 * task (which is an invalid locking op but in the case
	 * of the scheduler it's an obvious special-case), so we
	 * do an early lockdep release here:
	 */
<<<<<<< HEAD
#ifndef __ARCH_WANT_UNLOCKED_CTXSW
	spin_release(&rq->lock.dep_map, 1, _THIS_IP_);
#endif

	context_tracking_task_switch(prev, next);

#ifdef CONFIG_HISI_RDR
#ifdef CONFIG_HISI_RDR_SWITCH
	/* do switch hooks */
	{
		int ix;
		for (ix = 0; (ix < MAX_TASK_SWITCH_RTN)
				&& (task_switch_ls[ix]); ++ix)
			(*task_switch_ls[ix])((int)prev, (int)next);
	}
#endif
#else
//#ifndef CONFIG_ARCH_HI6XXX
    /*y00145322*/
       /* do switch hooks */
       {
               int ix;
               for (ix = 0; (ix < MAX_TASK_SWITCH_RTN)
                               && (task_switch_ls[ix]); ++ix)
                       (*task_switch_ls[ix])((void *)prev, (void *)next);
       }
//#endif
#endif
	/* Here we just switch the register state and the stack. */
	switch_to(prev, next, prev);

	barrier();
	/*
	 * this_rq must be evaluated again because prev may have moved
	 * CPUs since it called schedule(), thus the 'rq' on its stack
	 * frame will be invalid.
	 */
	finish_task_switch(this_rq(), prev);
=======
	lockdep_unpin_lock(&rq->lock, cookie);
	spin_release(&rq->lock.dep_map, 1, _THIS_IP_);

	/* Here we just switch the register state and the stack. */
	switch_to(prev, next, prev);
	barrier();

	return finish_task_switch(prev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * nr_running and nr_context_switches:
 *
 * externally visible scheduler statistics: current number of runnable
 * threads, total number of context switches performed since bootup.
 */
unsigned long nr_running(void)
{
	unsigned long i, sum = 0;

	for_each_online_cpu(i)
		sum += cpu_rq(i)->nr_running;

	return sum;
}

<<<<<<< HEAD
=======
/*
 * Check if only the current task is running on the cpu.
 *
 * Caution: this function does not check that the caller has disabled
 * preemption, thus the result might have a time-of-check-to-time-of-use
 * race.  The caller is responsible to use it correctly, for example:
 *
 * - from a non-preemptable section (of course)
 *
 * - from a thread that is bound to a single CPU
 *
 * - in a loop with very short iterations (e.g. a polling loop)
 */
bool single_task_running(void)
{
	return raw_rq()->nr_running == 1;
}
EXPORT_SYMBOL(single_task_running);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
unsigned long long nr_context_switches(void)
{
	int i;
	unsigned long long sum = 0;

	for_each_possible_cpu(i)
		sum += cpu_rq(i)->nr_switches;

	return sum;
}

unsigned long nr_iowait(void)
{
	unsigned long i, sum = 0;

	for_each_possible_cpu(i)
		sum += atomic_read(&cpu_rq(i)->nr_iowait);

	return sum;
}

unsigned long nr_iowait_cpu(int cpu)
{
	struct rq *this = cpu_rq(cpu);
	return atomic_read(&this->nr_iowait);
}

<<<<<<< HEAD
unsigned long this_cpu_load(void)
{
	struct rq *this = this_rq();
	return this->cpu_load[0];
}


/*
 * Global load-average calculations
 *
 * We take a distributed and async approach to calculating the global load-avg
 * in order to minimize overhead.
 *
 * The global load average is an exponentially decaying average of nr_running +
 * nr_uninterruptible.
 *
 * Once every LOAD_FREQ:
 *
 *   nr_active = 0;
 *   for_each_possible_cpu(cpu)
 *   	nr_active += cpu_of(cpu)->nr_running + cpu_of(cpu)->nr_uninterruptible;
 *
 *   avenrun[n] = avenrun[0] * exp_n + nr_active * (1 - exp_n)
 *
 * Due to a number of reasons the above turns in the mess below:
 *
 *  - for_each_possible_cpu() is prohibitively expensive on machines with
 *    serious number of cpus, therefore we need to take a distributed approach
 *    to calculating nr_active.
 *
 *        \Sum_i x_i(t) = \Sum_i x_i(t) - x_i(t_0) | x_i(t_0) := 0
 *                      = \Sum_i { \Sum_j=1 x_i(t_j) - x_i(t_j-1) }
 *
 *    So assuming nr_active := 0 when we start out -- true per definition, we
 *    can simply take per-cpu deltas and fold those into a global accumulate
 *    to obtain the same result. See calc_load_fold_active().
 *
 *    Furthermore, in order to avoid synchronizing all per-cpu delta folding
 *    across the machine, we assume 10 ticks is sufficient time for every
 *    cpu to have completed this task.
 *
 *    This places an upper-bound on the IRQ-off latency of the machine. Then
 *    again, being late doesn't loose the delta, just wrecks the sample.
 *
 *  - cpu_rq()->nr_uninterruptible isn't accurately tracked per-cpu because
 *    this would add another cross-cpu cacheline miss and atomic operation
 *    to the wakeup path. Instead we increment on whatever cpu the task ran
 *    when it went into uninterruptible state and decrement on whatever cpu
 *    did the wakeup. This means that only the sum of nr_uninterruptible over
 *    all cpus yields the correct result.
 *
 *  This covers the NO_HZ=n code, for extra head-aches, see the comment below.
 */

/* Variables and functions for calc_load */
static atomic_long_t calc_load_tasks;
static unsigned long calc_load_update;
unsigned long avenrun[3];
EXPORT_SYMBOL(avenrun); /* should be removed */

/**
 * get_avenrun - get the load average array
 * @loads:	pointer to dest load array
 * @offset:	offset to add
 * @shift:	shift count to shift the result left
 *
 * These values are estimates at best, so no need for locking.
 */
void get_avenrun(unsigned long *loads, unsigned long offset, int shift)
{
	loads[0] = (avenrun[0] + offset) << shift;
	loads[1] = (avenrun[1] + offset) << shift;
	loads[2] = (avenrun[2] + offset) << shift;
}

static long calc_load_fold_active(struct rq *this_rq)
{
	long nr_active, delta = 0;

	nr_active = this_rq->nr_running;
	nr_active += (long) this_rq->nr_uninterruptible;

	if (nr_active != this_rq->calc_load_active) {
		delta = nr_active - this_rq->calc_load_active;
		this_rq->calc_load_active = nr_active;
	}

	return delta;
}

/*
 * a1 = a0 * e + a * (1 - e)
 */
static unsigned long
calc_load(unsigned long load, unsigned long exp, unsigned long active)
{
	load *= exp;
	load += active * (FIXED_1 - exp);
	load += 1UL << (FSHIFT - 1);
	return load >> FSHIFT;
}

#ifdef CONFIG_NO_HZ_COMMON
/*
 * Handle NO_HZ for the global load-average.
 *
 * Since the above described distributed algorithm to compute the global
 * load-average relies on per-cpu sampling from the tick, it is affected by
 * NO_HZ.
 *
 * The basic idea is to fold the nr_active delta into a global idle-delta upon
 * entering NO_HZ state such that we can include this as an 'extra' cpu delta
 * when we read the global state.
 *
 * Obviously reality has to ruin such a delightfully simple scheme:
 *
 *  - When we go NO_HZ idle during the window, we can negate our sample
 *    contribution, causing under-accounting.
 *
 *    We avoid this by keeping two idle-delta counters and flipping them
 *    when the window starts, thus separating old and new NO_HZ load.
 *
 *    The only trick is the slight shift in index flip for read vs write.
 *
 *        0s            5s            10s           15s
 *          +10           +10           +10           +10
 *        |-|-----------|-|-----------|-|-----------|-|
 *    r:0 0 1           1 0           0 1           1 0
 *    w:0 1 1           0 0           1 1           0 0
 *
 *    This ensures we'll fold the old idle contribution in this window while
 *    accumlating the new one.
 *
 *  - When we wake up from NO_HZ idle during the window, we push up our
 *    contribution, since we effectively move our sample point to a known
 *    busy state.
 *
 *    This is solved by pushing the window forward, and thus skipping the
 *    sample, for this cpu (effectively using the idle-delta for this cpu which
 *    was in effect at the time the window opened). This also solves the issue
 *    of having to deal with a cpu having been in NOHZ idle for multiple
 *    LOAD_FREQ intervals.
 *
 * When making the ILB scale, we should try to pull this in as well.
 */
static atomic_long_t calc_load_idle[2];
static int calc_load_idx;

static inline int calc_load_write_idx(void)
{
	int idx = calc_load_idx;

	/*
	 * See calc_global_nohz(), if we observe the new index, we also
	 * need to observe the new update time.
	 */
	smp_rmb();

	/*
	 * If the folding window started, make sure we start writing in the
	 * next idle-delta.
	 */
	if (!time_before(jiffies, calc_load_update))
		idx++;

	return idx & 1;
}

static inline int calc_load_read_idx(void)
{
	return calc_load_idx & 1;
}

void calc_load_enter_idle(void)
{
	struct rq *this_rq = this_rq();
	long delta;

	/*
	 * We're going into NOHZ mode, if there's any pending delta, fold it
	 * into the pending idle delta.
	 */
	delta = calc_load_fold_active(this_rq);
	if (delta) {
		int idx = calc_load_write_idx();
		atomic_long_add(delta, &calc_load_idle[idx]);
	}
}

void calc_load_exit_idle(void)
{
	struct rq *this_rq = this_rq();

	/*
	 * If we're still before the sample window, we're done.
	 */
	if (time_before(jiffies, this_rq->calc_load_update))
		return;

	/*
	 * We woke inside or after the sample window, this means we're already
	 * accounted through the nohz accounting, so skip the entire deal and
	 * sync up for the next window.
	 */
	this_rq->calc_load_update = calc_load_update;
	if (time_before(jiffies, this_rq->calc_load_update + 10))
		this_rq->calc_load_update += LOAD_FREQ;
}

static long calc_load_fold_idle(void)
{
	int idx = calc_load_read_idx();
	long delta = 0;

	if (atomic_long_read(&calc_load_idle[idx]))
		delta = atomic_long_xchg(&calc_load_idle[idx], 0);

	return delta;
}

/**
 * fixed_power_int - compute: x^n, in O(log n) time
 *
 * @x:         base of the power
 * @frac_bits: fractional bits of @x
 * @n:         power to raise @x to.
 *
 * By exploiting the relation between the definition of the natural power
 * function: x^n := x*x*...*x (x multiplied by itself for n times), and
 * the binary encoding of numbers used by computers: n := \Sum n_i * 2^i,
 * (where: n_i \elem {0, 1}, the binary vector representing n),
 * we find: x^n := x^(\Sum n_i * 2^i) := \Prod x^(n_i * 2^i), which is
 * of course trivially computable in O(log_2 n), the length of our binary
 * vector.
 */
static unsigned long
fixed_power_int(unsigned long x, unsigned int frac_bits, unsigned int n)
{
	unsigned long result = 1UL << frac_bits;

	if (n) for (;;) {
		if (n & 1) {
			result *= x;
			result += 1UL << (frac_bits - 1);
			result >>= frac_bits;
		}
		n >>= 1;
		if (!n)
			break;
		x *= x;
		x += 1UL << (frac_bits - 1);
		x >>= frac_bits;
	}

	return result;
}

/*
 * a1 = a0 * e + a * (1 - e)
 *
 * a2 = a1 * e + a * (1 - e)
 *    = (a0 * e + a * (1 - e)) * e + a * (1 - e)
 *    = a0 * e^2 + a * (1 - e) * (1 + e)
 *
 * a3 = a2 * e + a * (1 - e)
 *    = (a0 * e^2 + a * (1 - e) * (1 + e)) * e + a * (1 - e)
 *    = a0 * e^3 + a * (1 - e) * (1 + e + e^2)
 *
 *  ...
 *
 * an = a0 * e^n + a * (1 - e) * (1 + e + ... + e^n-1) [1]
 *    = a0 * e^n + a * (1 - e) * (1 - e^n)/(1 - e)
 *    = a0 * e^n + a * (1 - e^n)
 *
 * [1] application of the geometric series:
 *
 *              n         1 - x^(n+1)
 *     S_n := \Sum x^i = -------------
 *             i=0          1 - x
 */
static unsigned long
calc_load_n(unsigned long load, unsigned long exp,
	    unsigned long active, unsigned int n)
{

	return calc_load(load, fixed_power_int(exp, FSHIFT, n), active);
}

/*
 * NO_HZ can leave us missing all per-cpu ticks calling
 * calc_load_account_active(), but since an idle CPU folds its delta into
 * calc_load_tasks_idle per calc_load_account_idle(), all we need to do is fold
 * in the pending idle delta if our idle period crossed a load cycle boundary.
 *
 * Once we've updated the global active value, we need to apply the exponential
 * weights adjusted to the number of cycles missed.
 */
static void calc_global_nohz(void)
{
	long delta, active, n;

	if (!time_before(jiffies, calc_load_update + 10)) {
		/*
		 * Catch-up, fold however many we are behind still
		 */
		delta = jiffies - calc_load_update - 10;
		n = 1 + (delta / LOAD_FREQ);

		active = atomic_long_read(&calc_load_tasks);
		active = active > 0 ? active * FIXED_1 : 0;

		avenrun[0] = calc_load_n(avenrun[0], EXP_1, active, n);
		avenrun[1] = calc_load_n(avenrun[1], EXP_5, active, n);
		avenrun[2] = calc_load_n(avenrun[2], EXP_15, active, n);

		calc_load_update += n * LOAD_FREQ;
	}

	/*
	 * Flip the idle index...
	 *
	 * Make sure we first write the new time then flip the index, so that
	 * calc_load_write_idx() will see the new time when it reads the new
	 * index, this avoids a double flip messing things up.
	 */
	smp_wmb();
	calc_load_idx++;
}
#else /* !CONFIG_NO_HZ_COMMON */

static inline long calc_load_fold_idle(void) { return 0; }
static inline void calc_global_nohz(void) { }

#endif /* CONFIG_NO_HZ_COMMON */

/*
 * calc_load - update the avenrun load estimates 10 ticks after the
 * CPUs have updated calc_load_tasks.
 */
void calc_global_load(unsigned long ticks)
{
	long active, delta;

	if (time_before(jiffies, calc_load_update + 10))
		return;

	/*
	 * Fold the 'old' idle-delta to include all NO_HZ cpus.
	 */
	delta = calc_load_fold_idle();
	if (delta)
		atomic_long_add(delta, &calc_load_tasks);

	active = atomic_long_read(&calc_load_tasks);
	active = active > 0 ? active * FIXED_1 : 0;

	avenrun[0] = calc_load(avenrun[0], EXP_1, active);
	avenrun[1] = calc_load(avenrun[1], EXP_5, active);
	avenrun[2] = calc_load(avenrun[2], EXP_15, active);

	calc_load_update += LOAD_FREQ;

	/*
	 * In case we idled for multiple LOAD_FREQ intervals, catch up in bulk.
	 */
	calc_global_nohz();
}

/*
 * Called from update_cpu_load() to periodically update this CPU's
 * active count.
 */
static void calc_load_account_active(struct rq *this_rq)
{
	long delta;

	if (time_before(jiffies, this_rq->calc_load_update))
		return;

	delta  = calc_load_fold_active(this_rq);
	if (delta)
		atomic_long_add(delta, &calc_load_tasks);

	this_rq->calc_load_update += LOAD_FREQ;
}

/*
 * End of global load-average stuff
 */

/*
 * The exact cpuload at various idx values, calculated at every tick would be
 * load = (2^idx - 1) / 2^idx * load + 1 / 2^idx * cur_load
 *
 * If a cpu misses updates for n-1 ticks (as it was idle) and update gets called
 * on nth tick when cpu may be busy, then we have:
 * load = ((2^idx - 1) / 2^idx)^(n-1) * load
 * load = (2^idx - 1) / 2^idx) * load + 1 / 2^idx * cur_load
 *
 * decay_load_missed() below does efficient calculation of
 * load = ((2^idx - 1) / 2^idx)^(n-1) * load
 * avoiding 0..n-1 loop doing load = ((2^idx - 1) / 2^idx) * load
 *
 * The calculation is approximated on a 128 point scale.
 * degrade_zero_ticks is the number of ticks after which load at any
 * particular idx is approximated to be zero.
 * degrade_factor is a precomputed table, a row for each load idx.
 * Each column corresponds to degradation factor for a power of two ticks,
 * based on 128 point scale.
 * Example:
 * row 2, col 3 (=12) says that the degradation at load idx 2 after
 * 8 ticks is 12/128 (which is an approximation of exact factor 3^8/4^8).
 *
 * With this power of 2 load factors, we can degrade the load n times
 * by looking at 1 bits in n and doing as many mult/shift instead of
 * n mult/shifts needed by the exact degradation.
 */
#define DEGRADE_SHIFT		7
static const unsigned char
		degrade_zero_ticks[CPU_LOAD_IDX_MAX] = {0, 8, 32, 64, 128};
static const unsigned char
		degrade_factor[CPU_LOAD_IDX_MAX][DEGRADE_SHIFT + 1] = {
					{0, 0, 0, 0, 0, 0, 0, 0},
					{64, 32, 8, 0, 0, 0, 0, 0},
					{96, 72, 40, 12, 1, 0, 0},
					{112, 98, 75, 43, 15, 1, 0},
					{120, 112, 98, 76, 45, 16, 2} };

/*
 * Update cpu_load for any missed ticks, due to tickless idle. The backlog
 * would be when CPU is idle and so we just decay the old load without
 * adding any new load.
 */
static unsigned long
decay_load_missed(unsigned long load, unsigned long missed_updates, int idx)
{
	int j = 0;

	if (!missed_updates)
		return load;

	if (missed_updates >= degrade_zero_ticks[idx])
		return 0;

	if (idx == 1)
		return load >> missed_updates;

	while (missed_updates) {
		if (missed_updates % 2)
			load = (load * degrade_factor[idx][j]) >> DEGRADE_SHIFT;

		missed_updates >>= 1;
		j++;
	}
	return load;
}

/*
 * Update rq->cpu_load[] statistics. This function is usually called every
 * scheduler tick (TICK_NSEC). With tickless idle this will not be called
 * every tick. We fix it up based on jiffies.
 */
static void __update_cpu_load(struct rq *this_rq, unsigned long this_load,
			      unsigned long pending_updates)
{
	int i, scale;

	this_rq->nr_load_updates++;

	/* Update our load: */
	this_rq->cpu_load[0] = this_load; /* Fasttrack for idx 0 */
	for (i = 1, scale = 2; i < CPU_LOAD_IDX_MAX; i++, scale += scale) {
		unsigned long old_load, new_load;

		/* scale is effectively 1 << i now, and >> i divides by scale */

		old_load = this_rq->cpu_load[i];
		old_load = decay_load_missed(old_load, pending_updates - 1, i);
		new_load = this_load;
		/*
		 * Round up the averaging division if load is increasing. This
		 * prevents us from getting stuck on 9 if the load is 10, for
		 * example.
		 */
		if (new_load > old_load)
			new_load += scale - 1;

		this_rq->cpu_load[i] = (old_load * (scale - 1) + new_load) >> i;
	}

	sched_avg_update(this_rq);
}

#ifdef CONFIG_SMP
unsigned long get_rq_runnable_load(struct rq *rq)
{
       return rq->cfs.runnable_load_avg;
}
#else
unsigned long get_rq_runnable_load(struct rq *rq)
{
       return rq->load.weight;
}
#endif

#ifdef CONFIG_NO_HZ_COMMON
/*
 * There is no sane way to deal with nohz on smp when using jiffies because the
 * cpu doing the jiffies update might drift wrt the cpu doing the jiffy reading
 * causing off-by-one errors in observed deltas; {0,2} instead of {1,1}.
 *
 * Therefore we cannot use the delta approach from the regular tick since that
 * would seriously skew the load calculation. However we'll make do for those
 * updates happening while idle (nohz_idle_balance) or coming out of idle
 * (tick_nohz_idle_exit).
 *
 * This means we might still be one tick off for nohz periods.
 */

/*
 * Called from nohz_idle_balance() to update the load ratings before doing the
 * idle balance.
 */
void update_idle_cpu_load(struct rq *this_rq)
{
	unsigned long curr_jiffies = ACCESS_ONCE(jiffies);
	unsigned long load = get_rq_runnable_load(this_rq);
	unsigned long pending_updates;

	/*
	 * bail if there's load or we're actually up-to-date.
	 */
	if (load || curr_jiffies == this_rq->last_load_update_tick)
		return;

	pending_updates = curr_jiffies - this_rq->last_load_update_tick;
	this_rq->last_load_update_tick = curr_jiffies;

	__update_cpu_load(this_rq, load, pending_updates);
}

/*
 * Called from tick_nohz_idle_exit() -- try and fix up the ticks we missed.
 */
void update_cpu_load_nohz(void)
{
	struct rq *this_rq = this_rq();
	unsigned long curr_jiffies = ACCESS_ONCE(jiffies);
	unsigned long pending_updates;

	if (curr_jiffies == this_rq->last_load_update_tick)
		return;

	raw_spin_lock(&this_rq->lock);
	pending_updates = curr_jiffies - this_rq->last_load_update_tick;
	if (pending_updates) {
		this_rq->last_load_update_tick = curr_jiffies;
		/*
		 * We were idle, this means load 0, the current load might be
		 * !0 due to remote wakeups and the sort.
		 */
		__update_cpu_load(this_rq, 0, pending_updates);
	}
	raw_spin_unlock(&this_rq->lock);
}
#endif /* CONFIG_NO_HZ_COMMON */

/*
 * Called from scheduler_tick()
 */
static void update_cpu_load_active(struct rq *this_rq)
{
	unsigned long load = get_rq_runnable_load(this_rq);
	/*
	 * See the mess around update_idle_cpu_load() / update_cpu_load_nohz().
	 */
	this_rq->last_load_update_tick = jiffies;
	__update_cpu_load(this_rq, load, 1);

	calc_load_account_active(this_rq);
}

#ifdef CONFIG_SMP

/*
 * sched_exec - execve() is a valuable balancing opportunity, because at
 * this point the task has the smallest effective memory and cache footprint.
 */
void sched_exec(void)
{
	struct task_struct *p = current;
	unsigned long flags;
	int dest_cpu;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	dest_cpu = p->sched_class->select_task_rq(p, SD_BALANCE_EXEC, 0);
	if (dest_cpu == smp_processor_id())
		goto unlock;

	if (likely(cpu_active(dest_cpu))) {
		struct migration_arg arg = { p, dest_cpu };

		raw_spin_unlock_irqrestore(&p->pi_lock, flags);
		stop_one_cpu(task_cpu(p), migration_cpu_stop, &arg);
		return;
	}
unlock:
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);
}

#endif

DEFINE_PER_CPU(struct kernel_stat, kstat);
DEFINE_PER_CPU(struct kernel_cpustat, kernel_cpustat);

EXPORT_PER_CPU_SYMBOL(kstat);
EXPORT_PER_CPU_SYMBOL(kernel_cpustat);

/*
 * Return any ns on the sched_clock that have not yet been accounted in
 * @p in case that task is currently running.
 *
 * Called with task_rq_lock() held on @rq.
 */
static u64 do_task_delta_exec(struct task_struct *p, struct rq *rq)
{
	u64 ns = 0;

	if (task_current(rq, p)) {
		update_rq_clock(rq);
		ns = rq->clock_task - p->se.exec_start;
		if ((s64)ns < 0)
			ns = 0;
	}

	return ns;
}

unsigned long long task_delta_exec(struct task_struct *p)
{
	unsigned long flags;
	struct rq *rq;
	u64 ns = 0;

	rq = task_rq_lock(p, &flags);
	ns = do_task_delta_exec(p, rq);
	task_rq_unlock(rq, p, &flags);

	return ns;
}

/*
 * Return accounted runtime for the task.
 * In case the task is currently running, return the runtime plus current's
 * pending runtime that have not been accounted yet.
 */
unsigned long long task_sched_runtime(struct task_struct *p)
{
	unsigned long flags;
	struct rq *rq;
	u64 ns = 0;

	rq = task_rq_lock(p, &flags);
	ns = p->se.sum_exec_runtime + do_task_delta_exec(p, rq);
	task_rq_unlock(rq, p, &flags);

	return ns;
}
=======
#ifdef CONFIG_CPU_QUIET
u64 nr_running_integral(unsigned int cpu)
{
	unsigned int seqcnt;
	u64 integral;
	struct rq *q;

	if (cpu >= nr_cpu_ids)
		return 0;

	q = cpu_rq(cpu);

	/*
	 * Update average to avoid reading stalled value if there were
	 * no run-queue changes for a long time. On the other hand if
	 * the changes are happening right now, just read current value
	 * directly.
	 */

	seqcnt = read_seqcount_begin(&q->ave_seqcnt);
	integral = do_nr_running_integral(q);
	if (read_seqcount_retry(&q->ave_seqcnt, seqcnt)) {
		read_seqcount_begin(&q->ave_seqcnt);
		integral = q->nr_running_integral;
	}

	return integral;
}
#endif

void get_iowait_load(unsigned long *nr_waiters, unsigned long *load)
{
	struct rq *rq = this_rq();
	*nr_waiters = atomic_read(&rq->nr_iowait);
	*load = rq->load.weight;
}

#ifdef CONFIG_SMP

/*
 * sched_exec - execve() is a valuable balancing opportunity, because at
 * this point the task has the smallest effective memory and cache footprint.
 */
void sched_exec(void)
{
	struct task_struct *p = current;
	unsigned long flags;
	int dest_cpu;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	dest_cpu = p->sched_class->select_task_rq(p, task_cpu(p), SD_BALANCE_EXEC, 0);
	if (dest_cpu == smp_processor_id())
		goto unlock;

	if (likely(cpu_active(dest_cpu))) {
		struct migration_arg arg = { p, dest_cpu };

		raw_spin_unlock_irqrestore(&p->pi_lock, flags);
		stop_one_cpu(task_cpu(p), migration_cpu_stop, &arg);
		return;
	}
unlock:
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);
}

#endif

DEFINE_PER_CPU(struct kernel_stat, kstat);
DEFINE_PER_CPU(struct kernel_cpustat, kernel_cpustat);

EXPORT_PER_CPU_SYMBOL(kstat);
EXPORT_PER_CPU_SYMBOL(kernel_cpustat);

/*
 * The function fair_sched_class.update_curr accesses the struct curr
 * and its field curr->exec_start; when called from task_sched_runtime(),
 * we observe a high rate of cache misses in practice.
 * Prefetching this data results in improved performance.
 */
static inline void prefetch_curr_exec_start(struct task_struct *p)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	struct sched_entity *curr = (&p->se)->cfs_rq->curr;
#else
	struct sched_entity *curr = (&task_rq(p)->cfs)->curr;
#endif
	prefetch(curr);
	prefetch(&curr->exec_start);
}

/*
 * Return accounted runtime for the task.
 * In case the task is currently running, return the runtime plus current's
 * pending runtime that have not been accounted yet.
 */
unsigned long long task_sched_runtime(struct task_struct *p)
{
	struct rq_flags rf;
	struct rq *rq;
	u64 ns;

#if defined(CONFIG_64BIT) && defined(CONFIG_SMP)
	/*
	 * 64-bit doesn't need locks to atomically read a 64bit value.
	 * So we have a optimization chance when the task's delta_exec is 0.
	 * Reading ->on_cpu is racy, but this is ok.
	 *
	 * If we race with it leaving cpu, we'll take a lock. So we're correct.
	 * If we race with it entering cpu, unaccounted time is 0. This is
	 * indistinguishable from the read occurring a few cycles earlier.
	 * If we see ->on_cpu without ->on_rq, the task is leaving, and has
	 * been accounted, so we're correct here as well.
	 */
	if (!p->on_cpu || !task_on_rq_queued(p))
		return p->se.sum_exec_runtime;
#endif

	rq = task_rq_lock(p, &rf);
	/*
	 * Must be ->curr _and_ ->on_rq.  If dequeued, we would
	 * project cycles that may never be accounted to this
	 * thread, breaking clock_gettime().
	 */
	if (task_current(rq, p) && task_on_rq_queued(p)) {
		prefetch_curr_exec_start(p);
		update_rq_clock(rq);
		p->sched_class->update_curr(rq);
	}
	ns = p->se.sum_exec_runtime;
	task_rq_unlock(rq, p, &rf);

	return ns;
}

#ifdef CONFIG_CPU_FREQ_GOV_SCHED

static inline
unsigned long add_capacity_margin(unsigned long cpu_capacity)
{
	cpu_capacity  = cpu_capacity * capacity_margin;
	cpu_capacity /= SCHED_CAPACITY_SCALE;
	return cpu_capacity;
}

static inline
unsigned long sum_capacity_reqs(unsigned long cfs_cap,
				struct sched_capacity_reqs *scr)
{
	unsigned long total = add_capacity_margin(cfs_cap + scr->rt);
	return total += scr->dl;
}

unsigned long boosted_cpu_util(int cpu);
static void sched_freq_tick_pelt(int cpu)
{
	unsigned long cpu_utilization = boosted_cpu_util(cpu);
	unsigned long capacity_curr = capacity_curr_of(cpu);
	struct sched_capacity_reqs *scr;

	scr = &per_cpu(cpu_sched_capacity_reqs, cpu);
	if (sum_capacity_reqs(cpu_utilization, scr) < capacity_curr)
		return;

	/*
	 * To make free room for a task that is building up its "real"
	 * utilization and to harm its performance the least, request
	 * a jump to a higher OPP as soon as the margin of free capacity
	 * is impacted (specified by capacity_margin).
	 */
	set_cfs_cpu_capacity(cpu, true, cpu_utilization);
}

#ifdef CONFIG_SCHED_WALT
static void sched_freq_tick_walt(int cpu)
{
	unsigned long cpu_utilization = cpu_util(cpu);
	unsigned long capacity_curr = capacity_curr_of(cpu);

	if (walt_disabled || !sysctl_sched_use_walt_cpu_util)
		return sched_freq_tick_pelt(cpu);

	/*
	 * Add a margin to the WALT utilization.
	 * NOTE: WALT tracks a single CPU signal for all the scheduling
	 * classes, thus this margin is going to be added to the DL class as
	 * well, which is something we do not do in sched_freq_tick_pelt case.
	 */
	cpu_utilization = add_capacity_margin(cpu_utilization);
	if (cpu_utilization <= capacity_curr)
		return;

	/*
	 * It is likely that the load is growing so we
	 * keep the added margin in our request as an
	 * extra boost.
	 */
	set_cfs_cpu_capacity(cpu, true, cpu_utilization);

}
#define _sched_freq_tick(cpu) sched_freq_tick_walt(cpu)
#else
#define _sched_freq_tick(cpu) sched_freq_tick_pelt(cpu)
#endif /* CONFIG_SCHED_WALT */

static void sched_freq_tick(int cpu)
{
	unsigned long capacity_orig, capacity_curr;

	if (!sched_freq())
		return;

	capacity_orig = capacity_orig_of(cpu);
	capacity_curr = capacity_curr_of(cpu);
	if (capacity_curr == capacity_orig)
		return;

	_sched_freq_tick(cpu);
}
#else
static inline void sched_freq_tick(int cpu) { }
#endif /* CONFIG_CPU_FREQ_GOV_SCHED */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * This function gets called by the timer code, with HZ frequency.
 * We call it with interrupts disabled.
 */
void scheduler_tick(void)
{
	int cpu = smp_processor_id();
	struct rq *rq = cpu_rq(cpu);
	struct task_struct *curr = rq->curr;

	sched_clock_tick();

	raw_spin_lock(&rq->lock);
<<<<<<< HEAD
	update_rq_clock(rq);
	update_cpu_load_active(rq);
	curr->sched_class->task_tick(rq, curr, 0);
=======
	walt_set_window_start(rq);
	update_rq_clock(rq);
	curr->sched_class->task_tick(rq, curr, 0);
	cpu_load_update_active(rq);
	walt_update_task_ravg(rq->curr, rq, TASK_UPDATE,
			walt_ktime_clock(), 0);
	calc_global_load_tick(rq);
	sched_freq_tick(cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	raw_spin_unlock(&rq->lock);

	perf_event_task_tick();

#ifdef CONFIG_SMP
	rq->idle_balance = idle_cpu(cpu);
<<<<<<< HEAD
	trigger_load_balance(rq, cpu);
=======
	trigger_load_balance(rq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
	rq_last_tick_reset(rq);
}

#ifdef CONFIG_NO_HZ_FULL
/**
 * scheduler_tick_max_deferment
 *
 * Keep at least one tick per second when a single
 * active task is running because the scheduler doesn't
 * yet completely support full dynticks environment.
 *
 * This makes sure that uptime, CFS vruntime, load
 * balancing, etc... continue to move forward, even
 * with a very low granularity.
<<<<<<< HEAD
=======
 *
 * Return: Maximum deferment in nanoseconds.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
u64 scheduler_tick_max_deferment(void)
{
	struct rq *rq = this_rq();
<<<<<<< HEAD
	unsigned long next, now = ACCESS_ONCE(jiffies);
=======
	unsigned long next, now = READ_ONCE(jiffies);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	next = rq->last_sched_tick + HZ;

	if (time_before_eq(next, now))
		return 0;

<<<<<<< HEAD
	return jiffies_to_usecs(next - now) * NSEC_PER_USEC;
}
#endif

notrace unsigned long get_parent_ip(unsigned long addr)
{
	if (in_lock_functions(addr)) {
		addr = CALLER_ADDR2;
		if (in_lock_functions(addr))
			addr = CALLER_ADDR3;
	}
	return addr;
}

#if defined(CONFIG_PREEMPT) && (defined(CONFIG_DEBUG_PREEMPT) || \
				defined(CONFIG_PREEMPT_TRACER))

void __kprobes add_preempt_count(int val)
=======
	return jiffies_to_nsecs(next - now);
}
#endif

#if defined(CONFIG_PREEMPT) && (defined(CONFIG_DEBUG_PREEMPT) || \
				defined(CONFIG_PREEMPT_TRACER))
/*
 * If the value passed in is equal to the current preempt count
 * then we just disabled preemption. Start timing the latency.
 */
static inline void preempt_latency_start(int val)
{
	if (preempt_count() == val) {
		unsigned long ip = get_lock_parent_ip();
#ifdef CONFIG_DEBUG_PREEMPT
		current->preempt_disable_ip = ip;
#endif
		trace_preempt_off(CALLER_ADDR0, ip);
	}
}

void preempt_count_add(int val)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
#ifdef CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 */
	if (DEBUG_LOCKS_WARN_ON((preempt_count() < 0)))
		return;
#endif
<<<<<<< HEAD
	preempt_count() += val;
=======
	__preempt_count_add(val);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_DEBUG_PREEMPT
	/*
	 * Spinlock count overflowing soon?
	 */
	DEBUG_LOCKS_WARN_ON((preempt_count() & PREEMPT_MASK) >=
				PREEMPT_MASK - 10);
#endif
<<<<<<< HEAD
	if (preempt_count() == val)
		trace_preempt_off(CALLER_ADDR0, get_parent_ip(CALLER_ADDR1));
}
EXPORT_SYMBOL(add_preempt_count);

void __kprobes sub_preempt_count(int val)
=======
	preempt_latency_start(val);
}
EXPORT_SYMBOL(preempt_count_add);
NOKPROBE_SYMBOL(preempt_count_add);

/*
 * If the value passed in equals to the current preempt count
 * then we just enabled preemption. Stop timing the latency.
 */
static inline void preempt_latency_stop(int val)
{
	if (preempt_count() == val)
		trace_preempt_on(CALLER_ADDR0, get_lock_parent_ip());
}

void preempt_count_sub(int val)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
#ifdef CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 */
	if (DEBUG_LOCKS_WARN_ON(val > preempt_count()))
		return;
	/*
	 * Is the spinlock portion underflowing?
	 */
	if (DEBUG_LOCKS_WARN_ON((val < PREEMPT_MASK) &&
			!(preempt_count() & PREEMPT_MASK)))
		return;
#endif

<<<<<<< HEAD
	if (preempt_count() == val)
		trace_preempt_on(CALLER_ADDR0, get_parent_ip(CALLER_ADDR1));
	preempt_count() -= val;
}
EXPORT_SYMBOL(sub_preempt_count);

=======
	preempt_latency_stop(val);
	__preempt_count_sub(val);
}
EXPORT_SYMBOL(preempt_count_sub);
NOKPROBE_SYMBOL(preempt_count_sub);

#else
static inline void preempt_latency_start(int val) { }
static inline void preempt_latency_stop(int val) { }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

/*
 * Print scheduling while atomic bug:
 */
static noinline void __schedule_bug(struct task_struct *prev)
{
<<<<<<< HEAD
=======
	/* Save this before calling printk(), since that will clobber it */
	unsigned long preempt_disable_ip = get_preempt_disable_ip(current);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (oops_in_progress)
		return;

	printk(KERN_ERR "BUG: scheduling while atomic: %s/%d/0x%08x\n",
		prev->comm, prev->pid, preempt_count());

	debug_show_held_locks(prev);
	print_modules();
	if (irqs_disabled())
		print_irqtrace_events(prev);
<<<<<<< HEAD
=======
	if (IS_ENABLED(CONFIG_DEBUG_PREEMPT)
	    && in_atomic_preempt_off()) {
		pr_err("Preemption disabled at:");
		print_ip_sym(preempt_disable_ip);
		pr_cont("\n");
	}
	if (panic_on_warn)
		panic("scheduling while atomic\n");

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dump_stack();
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}

/*
 * Various schedule()-time debugging checks and statistics:
 */
static inline void schedule_debug(struct task_struct *prev)
{
<<<<<<< HEAD
	/*
	 * Test if we are atomic. Since do_exit() needs to call into
	 * schedule() atomically, we ignore that path for now.
	 * Otherwise, whine if we are scheduling when we should not be.
	 */
	if (unlikely(in_atomic_preempt_off() && !prev->exit_state))
		__schedule_bug(prev);
=======
#ifdef CONFIG_SCHED_STACK_END_CHECK
	if (task_stack_end_corrupted(prev))
		panic("corrupted stack end detected inside scheduler\n");
#endif

	if (unlikely(in_atomic_preempt_off())) {
		__schedule_bug(prev);
		preempt_count_set(PREEMPT_DISABLED);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_sleep_check();

	profile_hit(SCHED_PROFILING, __builtin_return_address(0));

<<<<<<< HEAD
	schedstat_inc(this_rq(), sched_count);
}

static void put_prev_task(struct rq *rq, struct task_struct *prev)
{
	if (prev->on_rq || rq->skip_clock_update < 0)
		update_rq_clock(rq);
	prev->sched_class->put_prev_task(rq, prev);
=======
	schedstat_inc(this_rq()->sched_count);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Pick up the highest-prio task:
 */
static inline struct task_struct *
<<<<<<< HEAD
pick_next_task(struct rq *rq)
{
	const struct sched_class *class;
=======
pick_next_task(struct rq *rq, struct task_struct *prev, struct pin_cookie cookie)
{
	const struct sched_class *class = &fair_sched_class;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct task_struct *p;

	/*
	 * Optimization: we know that if all tasks are in
	 * the fair class we can call that function directly:
	 */
<<<<<<< HEAD
	if (likely(rq->nr_running == rq->cfs.h_nr_running)) {
		p = fair_sched_class.pick_next_task(rq);
		if (likely(p))
			return p;
	}

	for_each_class(class) {
		p = class->pick_next_task(rq);
		if (p)
			return p;
=======
	if (likely(prev->sched_class == class &&
		   rq->nr_running == rq->cfs.h_nr_running)) {
		p = fair_sched_class.pick_next_task(rq, prev, cookie);
		if (unlikely(p == RETRY_TASK))
			goto again;

		/* assumes fair_sched_class->next == idle_sched_class */
		if (unlikely(!p))
			p = idle_sched_class.pick_next_task(rq, prev, cookie);

		return p;
	}

again:
	for_each_class(class) {
		p = class->pick_next_task(rq, prev, cookie);
		if (p) {
			if (unlikely(p == RETRY_TASK))
				goto again;
			return p;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	BUG(); /* the idle class will always have a runnable task */
}

/*
 * __schedule() is the main scheduler function.
 *
 * The main means of driving the scheduler and thus entering this function are:
 *
 *   1. Explicit blocking: mutex, semaphore, waitqueue, etc.
 *
 *   2. TIF_NEED_RESCHED flag is checked on interrupt and userspace return
 *      paths. For example, see arch/x86/entry_64.S.
 *
 *      To drive preemption between tasks, the scheduler sets the flag in timer
 *      interrupt handler scheduler_tick().
 *
 *   3. Wakeups don't really cause entry into schedule(). They add a
 *      task to the run-queue and that's it.
 *
 *      Now, if the new task added to the run-queue preempts the current
 *      task, then the wakeup sets TIF_NEED_RESCHED and schedule() gets
 *      called on the nearest possible occasion:
 *
 *       - If the kernel is preemptible (CONFIG_PREEMPT=y):
 *
 *         - in syscall or exception context, at the next outmost
 *           preempt_enable(). (this might be as soon as the wake_up()'s
 *           spin_unlock()!)
 *
 *         - in IRQ context, return from interrupt-handler to
 *           preemptible context
 *
 *       - If the kernel is not preemptible (CONFIG_PREEMPT is not set)
 *         then at the next:
 *
 *          - cond_resched() call
 *          - explicit schedule() call
 *          - return from syscall or exception to user-space
 *          - return from interrupt-handler to user-space
<<<<<<< HEAD
 */
static void __sched __schedule(void)
{
	struct task_struct *prev, *next;
	unsigned long *switch_count;
	struct rq *rq;
	int cpu;

need_resched:
	preempt_disable();
	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	rcu_note_context_switch(cpu);
=======
 *
 * WARNING: must be called with preemption disabled!
 */
static void __sched notrace __schedule(bool preempt)
{
	struct task_struct *prev, *next;
	unsigned long *switch_count;
	struct pin_cookie cookie;
	struct rq *rq;
	int cpu;
	u64 wallclock;

	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	prev = rq->curr;

	schedule_debug(prev);

	if (sched_feat(HRTICK))
		hrtick_clear(rq);

<<<<<<< HEAD
=======
	local_irq_disable();
	rcu_note_context_switch();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * Make sure that signal_pending_state()->signal_pending() below
	 * can't be reordered with __set_current_state(TASK_INTERRUPTIBLE)
	 * done by the caller to avoid the race with signal_wake_up().
	 */
	smp_mb__before_spinlock();
<<<<<<< HEAD
	raw_spin_lock_irq(&rq->lock);

	switch_count = &prev->nivcsw;
	if (prev->state && !(preempt_count() & PREEMPT_ACTIVE)) {
=======
	raw_spin_lock(&rq->lock);
	cookie = lockdep_pin_lock(&rq->lock);

	rq->clock_skip_update <<= 1; /* promote REQ to ACT */

	switch_count = &prev->nivcsw;
	if (!preempt && prev->state) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (unlikely(signal_pending_state(prev->state, prev))) {
			prev->state = TASK_RUNNING;
		} else {
			deactivate_task(rq, prev, DEQUEUE_SLEEP);
			prev->on_rq = 0;

			/*
			 * If a worker went to sleep, notify and ask workqueue
			 * whether it wants to wake up a task to maintain
			 * concurrency.
			 */
			if (prev->flags & PF_WQ_WORKER) {
				struct task_struct *to_wakeup;

<<<<<<< HEAD
				to_wakeup = wq_worker_sleeping(prev, cpu);
				if (to_wakeup)
					try_to_wake_up_local(to_wakeup);
=======
				to_wakeup = wq_worker_sleeping(prev);
				if (to_wakeup)
					try_to_wake_up_local(to_wakeup, cookie);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			}
		}
		switch_count = &prev->nvcsw;
	}

<<<<<<< HEAD
	pre_schedule(rq, prev);

	if (unlikely(!rq->nr_running))
		idle_balance(cpu, rq);

	put_prev_task(rq, prev);
	next = pick_next_task(rq);
	clear_tsk_need_resched(prev);
	rq->skip_clock_update = 0;
=======
	if (task_on_rq_queued(prev))
		update_rq_clock(rq);

	next = pick_next_task(rq, prev, cookie);
	wallclock = walt_ktime_clock();
	walt_update_task_ravg(prev, rq, PUT_PREV_TASK, wallclock, 0);
	walt_update_task_ravg(next, rq, PICK_NEXT_TASK, wallclock, 0);
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();
	rq->clock_skip_update = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (likely(prev != next)) {
		rq->nr_switches++;
		rq->curr = next;
		++*switch_count;

<<<<<<< HEAD
		context_switch(rq, prev, next); /* unlocks the rq */
		/*
		 * The context switch have flipped the stack from under us
		 * and restored the local variables which were saved when
		 * this task called schedule() in the past. prev == current
		 * is still correct, but it can be moved to another cpu/rq.
		 */
		cpu = smp_processor_id();
		rq = cpu_rq(cpu);
	} else
		raw_spin_unlock_irq(&rq->lock);

	post_schedule(rq);

	sched_preempt_enable_no_resched();
	if (need_resched())
		goto need_resched;
=======
		trace_sched_switch(preempt, prev, next);
		rq = context_switch(rq, prev, next, cookie); /* unlocks the rq */
	} else {
		lockdep_unpin_lock(&rq->lock, cookie);
		raw_spin_unlock_irq(&rq->lock);
	}

	balance_callback(rq);
}

void __noreturn do_task_dead(void)
{
	/*
	 * The setting of TASK_RUNNING by try_to_wake_up() may be delayed
	 * when the following two conditions become true.
	 *   - There is race condition of mmap_sem (It is acquired by
	 *     exit_mm()), and
	 *   - SMI occurs before setting TASK_RUNINNG.
	 *     (or hypervisor of virtual machine switches to other guest)
	 *  As a result, we may become TASK_RUNNING after becoming TASK_DEAD
	 *
	 * To avoid it, we have to wait for releasing tsk->pi_lock which
	 * is held by try_to_wake_up()
	 */
	smp_mb();
	raw_spin_unlock_wait(&current->pi_lock);

	/* causes final put_task_struct in finish_task_switch(). */
	__set_current_state(TASK_DEAD);
	current->flags |= PF_NOFREEZE;	/* tell freezer to ignore us */
	__schedule(false);
	BUG();
	/* Avoid "noreturn function does return".  */
	for (;;)
		cpu_relax();	/* For when BUG is null */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void sched_submit_work(struct task_struct *tsk)
{
	if (!tsk->state || tsk_is_pi_blocked(tsk))
		return;
	/*
	 * If we are going to sleep and we have plugged IO queued,
	 * make sure to submit it to avoid deadlocks.
	 */
	if (blk_needs_flush_plug(tsk))
<<<<<<< HEAD
		blk_schedule_flush_plug(tsk);
}

asmlinkage void __sched schedule(void)
{
	struct task_struct *tsk = current;

	sched_submit_work(tsk);
	__schedule();
}
EXPORT_SYMBOL(schedule);

#ifdef CONFIG_CONTEXT_TRACKING
asmlinkage void __sched schedule_user(void)
{
	/*
	 * If we come here after a random call to set_need_resched(),
	 * or we have been woken up remotely but the IPI has not yet arrived,
	 * we haven't yet exited the RCU idle mode. Do it here manually until
	 * we find a better solution.
	 */
	user_exit();
	schedule();
	user_enter();
}
#endif

/**
 * schedule_preempt_disabled - called with preemption disabled
 *
 * Returns with preemption disabled. Note: preempt_count must be 1
 */
void __sched schedule_preempt_disabled(void)
{
	sched_preempt_enable_no_resched();
	schedule();
	preempt_disable();
}

#ifdef CONFIG_PREEMPT
/*
 * this is the entry point to schedule() from in-kernel preemption
 * off of preempt_enable. Kernel preemptions off return from interrupt
 * occur there and call schedule directly.
 */
asmlinkage void __sched notrace preempt_schedule(void)
{
	struct thread_info *ti = current_thread_info();

	/*
	 * If there is a non-zero preempt_count or interrupts are disabled,
	 * we do not want to preempt the current task. Just return..
	 */
	if (likely(ti->preempt_count || irqs_disabled()))
		return;

	do {
		add_preempt_count_notrace(PREEMPT_ACTIVE);
		__schedule();
		sub_preempt_count_notrace(PREEMPT_ACTIVE);

		/*
		 * Check again in case we missed a preemption opportunity
		 * between schedule and now.
		 */
		barrier();
	} while (need_resched());
}
EXPORT_SYMBOL(preempt_schedule);

/*
 * this is the entry point to schedule() from kernel preemption
 * off of irq context.
 * Note, that this is called and return with irqs disabled. This will
 * protect us against recursive calling from irq.
 */
asmlinkage void __sched preempt_schedule_irq(void)
{
	struct thread_info *ti = current_thread_info();
	enum ctx_state prev_state;

	/* Catch callers which need to be fixed */
	BUG_ON(ti->preempt_count || !irqs_disabled());

	prev_state = exception_enter();

	do {
		add_preempt_count(PREEMPT_ACTIVE);
		local_irq_enable();
		__schedule();
		local_irq_disable();
		sub_preempt_count(PREEMPT_ACTIVE);

		/*
		 * Check again in case we missed a preemption opportunity
		 * between schedule and now.
		 */
		barrier();
	} while (need_resched());

	exception_exit(prev_state);
}

#endif /* CONFIG_PREEMPT */

int default_wake_function(wait_queue_t *curr, unsigned mode, int wake_flags,
			  void *key)
{
	return try_to_wake_up(curr->private, mode, wake_flags);
}
EXPORT_SYMBOL(default_wake_function);

/*
 * The core wakeup function. Non-exclusive wakeups (nr_exclusive == 0) just
 * wake everything up. If it's an exclusive wakeup (nr_exclusive == small +ve
 * number) then we wake all the non-exclusive tasks and one exclusive task.
 *
 * There are circumstances in which we can try to wake a task which has already
 * started to run but is not in state TASK_RUNNING. try_to_wake_up() returns
 * zero in this (rare) case, and we handle it by continuing to scan the queue.
 */
static void __wake_up_common(wait_queue_head_t *q, unsigned int mode,
			int nr_exclusive, int wake_flags, void *key)
{
	wait_queue_t *curr, *next;

	list_for_each_entry_safe(curr, next, &q->task_list, task_list) {
		unsigned flags = curr->flags;

		if (curr->func(curr, mode, wake_flags, key) &&
				(flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			break;
	}
}

/**
 * __wake_up - wake up threads blocked on a waitqueue.
 * @q: the waitqueue
 * @mode: which threads
 * @nr_exclusive: how many wake-one or wake-many threads to wake up
 * @key: is directly passed to the wakeup function
 *
 * It may be assumed that this function implies a write memory barrier before
 * changing the task state if and only if any tasks are woken up.
 */
void __wake_up(wait_queue_head_t *q, unsigned int mode,
			int nr_exclusive, void *key)
{
	unsigned long flags;

	spin_lock_irqsave(&q->lock, flags);
	__wake_up_common(q, mode, nr_exclusive, 0, key);
	spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_SYMBOL(__wake_up);

/*
 * Same as __wake_up but called with the spinlock in wait_queue_head_t held.
 */
void __wake_up_locked(wait_queue_head_t *q, unsigned int mode, int nr)
{
	__wake_up_common(q, mode, nr, 0, NULL);
}
EXPORT_SYMBOL_GPL(__wake_up_locked);

void __wake_up_locked_key(wait_queue_head_t *q, unsigned int mode, void *key)
{
	__wake_up_common(q, mode, 1, 0, key);
}
EXPORT_SYMBOL_GPL(__wake_up_locked_key);

/**
 * __wake_up_sync_key - wake up threads blocked on a waitqueue.
 * @q: the waitqueue
 * @mode: which threads
 * @nr_exclusive: how many wake-one or wake-many threads to wake up
 * @key: opaque value to be passed to wakeup targets
 *
 * The sync wakeup differs that the waker knows that it will schedule
 * away soon, so while the target thread will be woken up, it will not
 * be migrated to another CPU - ie. the two threads are 'synchronized'
 * with each other. This can prevent needless bouncing between CPUs.
 *
 * On UP it can prevent extra preemption.
 *
 * It may be assumed that this function implies a write memory barrier before
 * changing the task state if and only if any tasks are woken up.
 */
void __wake_up_sync_key(wait_queue_head_t *q, unsigned int mode,
			int nr_exclusive, void *key)
{
	unsigned long flags;
	int wake_flags = WF_SYNC;

	if (unlikely(!q))
		return;

	if (unlikely(!nr_exclusive))
		wake_flags = 0;

	spin_lock_irqsave(&q->lock, flags);
	__wake_up_common(q, mode, nr_exclusive, wake_flags, key);
	spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_SYMBOL_GPL(__wake_up_sync_key);

/*
 * __wake_up_sync - see __wake_up_sync_key()
 */
void __wake_up_sync(wait_queue_head_t *q, unsigned int mode, int nr_exclusive)
{
	__wake_up_sync_key(q, mode, nr_exclusive, NULL);
}
EXPORT_SYMBOL_GPL(__wake_up_sync);	/* For internal use only */

/**
 * complete: - signals a single thread waiting on this completion
 * @x:  holds the state of this particular completion
 *
 * This will wake up a single thread waiting on this completion. Threads will be
 * awakened in the same order in which they were queued.
 *
 * See also complete_all(), wait_for_completion() and related routines.
 *
 * It may be assumed that this function implies a write memory barrier before
 * changing the task state if and only if any tasks are woken up.
 */
void complete(struct completion *x)
{
	unsigned long flags;

	spin_lock_irqsave(&x->wait.lock, flags);
	x->done++;
	__wake_up_common(&x->wait, TASK_NORMAL, 1, 0, NULL);
	spin_unlock_irqrestore(&x->wait.lock, flags);
}
EXPORT_SYMBOL(complete);

/**
 * complete_all: - signals all threads waiting on this completion
 * @x:  holds the state of this particular completion
 *
 * This will wake up all threads waiting on this particular completion event.
 *
 * It may be assumed that this function implies a write memory barrier before
 * changing the task state if and only if any tasks are woken up.
 */
void complete_all(struct completion *x)
{
	unsigned long flags;

	spin_lock_irqsave(&x->wait.lock, flags);
	x->done += UINT_MAX/2;
	__wake_up_common(&x->wait, TASK_NORMAL, 0, 0, NULL);
	spin_unlock_irqrestore(&x->wait.lock, flags);
}
EXPORT_SYMBOL(complete_all);

static inline long __sched
do_wait_for_common(struct completion *x,
		   long (*action)(long), long timeout, int state)
{
	if (!x->done) {
		DECLARE_WAITQUEUE(wait, current);

		__add_wait_queue_tail_exclusive(&x->wait, &wait);
		do {
			if (signal_pending_state(state, current)) {
				timeout = -ERESTARTSYS;
				break;
			}
			__set_current_state(state);
			spin_unlock_irq(&x->wait.lock);
			timeout = action(timeout);
			spin_lock_irq(&x->wait.lock);
		} while (!x->done && timeout);
		__remove_wait_queue(&x->wait, &wait);
		if (!x->done)
			return timeout;
	}
	x->done--;
	return timeout ?: 1;
}

static inline long __sched
__wait_for_common(struct completion *x,
		  long (*action)(long), long timeout, int state)
{
	might_sleep();

	spin_lock_irq(&x->wait.lock);
	timeout = do_wait_for_common(x, action, timeout, state);
	spin_unlock_irq(&x->wait.lock);
	return timeout;
}

static long __sched
wait_for_common(struct completion *x, long timeout, int state)
{
	return __wait_for_common(x, schedule_timeout, timeout, state);
}

static long __sched
wait_for_common_io(struct completion *x, long timeout, int state)
{
	return __wait_for_common(x, io_schedule_timeout, timeout, state);
}

/**
 * wait_for_completion: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout.
 *
 * See also similar routines (i.e. wait_for_completion_timeout()) with timeout
 * and interrupt capability. Also see complete().
 */
void __sched wait_for_completion(struct completion *x)
{
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_SYMBOL(wait_for_completion);

/**
 * wait_for_completion_timeout: - waits for completion of a task (w/timeout)
 * @x:  holds the state of this particular completion
 * @timeout:  timeout value in jiffies
 *
 * This waits for either a completion of a specific task to be signaled or for a
 * specified timeout to expire. The timeout is in jiffies. It is not
 * interruptible.
 *
 * The return value is 0 if timed out, and positive (at least 1, or number of
 * jiffies left till timeout) if completed.
 */
unsigned long __sched
wait_for_completion_timeout(struct completion *x, unsigned long timeout)
{
	return wait_for_common(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_SYMBOL(wait_for_completion_timeout);

/**
 * wait_for_completion_io: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout. The caller is accounted as waiting
 * for IO.
 */
void __sched wait_for_completion_io(struct completion *x)
{
	wait_for_common_io(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_SYMBOL(wait_for_completion_io);

/**
 * wait_for_completion_io_timeout: - waits for completion of a task (w/timeout)
 * @x:  holds the state of this particular completion
 * @timeout:  timeout value in jiffies
 *
 * This waits for either a completion of a specific task to be signaled or for a
 * specified timeout to expire. The timeout is in jiffies. It is not
 * interruptible. The caller is accounted as waiting for IO.
 *
 * The return value is 0 if timed out, and positive (at least 1, or number of
 * jiffies left till timeout) if completed.
 */
unsigned long __sched
wait_for_completion_io_timeout(struct completion *x, unsigned long timeout)
{
	return wait_for_common_io(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_SYMBOL(wait_for_completion_io_timeout);

/**
 * wait_for_completion_interruptible: - waits for completion of a task (w/intr)
 * @x:  holds the state of this particular completion
 *
 * This waits for completion of a specific task to be signaled. It is
 * interruptible.
 *
 * The return value is -ERESTARTSYS if interrupted, 0 if completed.
 */
int __sched wait_for_completion_interruptible(struct completion *x)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_INTERRUPTIBLE);
	if (t == -ERESTARTSYS)
		return t;
	return 0;
}
EXPORT_SYMBOL(wait_for_completion_interruptible);

/**
 * wait_for_completion_interruptible_timeout: - waits for completion (w/(to,intr))
 * @x:  holds the state of this particular completion
 * @timeout:  timeout value in jiffies
 *
 * This waits for either a completion of a specific task to be signaled or for a
 * specified timeout to expire. It is interruptible. The timeout is in jiffies.
 *
 * The return value is -ERESTARTSYS if interrupted, 0 if timed out,
 * positive (at least 1, or number of jiffies left till timeout) if completed.
 */
long __sched
wait_for_completion_interruptible_timeout(struct completion *x,
					  unsigned long timeout)
{
	return wait_for_common(x, timeout, TASK_INTERRUPTIBLE);
}
EXPORT_SYMBOL(wait_for_completion_interruptible_timeout);

/**
 * wait_for_completion_killable: - waits for completion of a task (killable)
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It can be
 * interrupted by a kill signal.
 *
 * The return value is -ERESTARTSYS if interrupted, 0 if completed.
 */
int __sched wait_for_completion_killable(struct completion *x)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_KILLABLE);
	if (t == -ERESTARTSYS)
		return t;
	return 0;
}
EXPORT_SYMBOL(wait_for_completion_killable);

/**
 * wait_for_completion_killable_timeout: - waits for completion of a task (w/(to,killable))
 * @x:  holds the state of this particular completion
 * @timeout:  timeout value in jiffies
 *
 * This waits for either a completion of a specific task to be
 * signaled or for a specified timeout to expire. It can be
 * interrupted by a kill signal. The timeout is in jiffies.
 *
 * The return value is -ERESTARTSYS if interrupted, 0 if timed out,
 * positive (at least 1, or number of jiffies left till timeout) if completed.
 */
long __sched
wait_for_completion_killable_timeout(struct completion *x,
				     unsigned long timeout)
{
	return wait_for_common(x, timeout, TASK_KILLABLE);
}
EXPORT_SYMBOL(wait_for_completion_killable_timeout);

/**
 *	try_wait_for_completion - try to decrement a completion without blocking
 *	@x:	completion structure
 *
 *	Returns: 0 if a decrement cannot be done without blocking
 *		 1 if a decrement succeeded.
 *
 *	If a completion is being used as a counting completion,
 *	attempt to decrement the counter without blocking. This
 *	enables us to avoid waiting if the resource the completion
 *	is protecting is not available.
 */
bool try_wait_for_completion(struct completion *x)
{
	unsigned long flags;
	int ret = 1;

	spin_lock_irqsave(&x->wait.lock, flags);
	if (!x->done)
		ret = 0;
	else
		x->done--;
	spin_unlock_irqrestore(&x->wait.lock, flags);
	return ret;
}
EXPORT_SYMBOL(try_wait_for_completion);

/**
 *	completion_done - Test to see if a completion has any waiters
 *	@x:	completion structure
 *
 *	Returns: 0 if there are waiters (wait_for_completion() in progress)
 *		 1 if there are no waiters.
 *
 */
bool completion_done(struct completion *x)
{
	unsigned long flags;
	int ret = 1;

	spin_lock_irqsave(&x->wait.lock, flags);
	if (!x->done)
		ret = 0;
	spin_unlock_irqrestore(&x->wait.lock, flags);
	return ret;
}
EXPORT_SYMBOL(completion_done);

static long __sched
sleep_on_common(wait_queue_head_t *q, int state, long timeout)
{
	unsigned long flags;
	wait_queue_t wait;

	init_waitqueue_entry(&wait, current);

	__set_current_state(state);

	spin_lock_irqsave(&q->lock, flags);
	__add_wait_queue(q, &wait);
	spin_unlock(&q->lock);
	timeout = schedule_timeout(timeout);
	spin_lock_irq(&q->lock);
	__remove_wait_queue(q, &wait);
	spin_unlock_irqrestore(&q->lock, flags);

	return timeout;
}

void __sched interruptible_sleep_on(wait_queue_head_t *q)
{
	sleep_on_common(q, TASK_INTERRUPTIBLE, MAX_SCHEDULE_TIMEOUT);
}
EXPORT_SYMBOL(interruptible_sleep_on);

long __sched
interruptible_sleep_on_timeout(wait_queue_head_t *q, long timeout)
{
	return sleep_on_common(q, TASK_INTERRUPTIBLE, timeout);
}
EXPORT_SYMBOL(interruptible_sleep_on_timeout);

void __sched sleep_on(wait_queue_head_t *q)
{
	sleep_on_common(q, TASK_UNINTERRUPTIBLE, MAX_SCHEDULE_TIMEOUT);
}
EXPORT_SYMBOL(sleep_on);

long __sched sleep_on_timeout(wait_queue_head_t *q, long timeout)
{
	return sleep_on_common(q, TASK_UNINTERRUPTIBLE, timeout);
}
EXPORT_SYMBOL(sleep_on_timeout);
=======
		blk_schedule_flush_plug(tsk);
}

asmlinkage __visible void __sched schedule(void)
{
	struct task_struct *tsk = current;

	sched_submit_work(tsk);
	do {
		preempt_disable();
		__schedule(false);
		sched_preempt_enable_no_resched();
	} while (need_resched());
}
EXPORT_SYMBOL(schedule);

#ifdef CONFIG_CONTEXT_TRACKING
asmlinkage __visible void __sched schedule_user(void)
{
	/*
	 * If we come here after a random call to set_need_resched(),
	 * or we have been woken up remotely but the IPI has not yet arrived,
	 * we haven't yet exited the RCU idle mode. Do it here manually until
	 * we find a better solution.
	 *
	 * NB: There are buggy callers of this function.  Ideally we
	 * should warn if prev_state != CONTEXT_USER, but that will trigger
	 * too frequently to make sense yet.
	 */
	enum ctx_state prev_state = exception_enter();
	schedule();
	exception_exit(prev_state);
}
#endif

/**
 * schedule_preempt_disabled - called with preemption disabled
 *
 * Returns with preemption disabled. Note: preempt_count must be 1
 */
void __sched schedule_preempt_disabled(void)
{
	sched_preempt_enable_no_resched();
	schedule();
	preempt_disable();
}

static void __sched notrace preempt_schedule_common(void)
{
	do {
		/*
		 * Because the function tracer can trace preempt_count_sub()
		 * and it also uses preempt_enable/disable_notrace(), if
		 * NEED_RESCHED is set, the preempt_enable_notrace() called
		 * by the function tracer will call this function again and
		 * cause infinite recursion.
		 *
		 * Preemption must be disabled here before the function
		 * tracer can trace. Break up preempt_disable() into two
		 * calls. One to disable preemption without fear of being
		 * traced. The other to still record the preemption latency,
		 * which can also be traced by the function tracer.
		 */
		preempt_disable_notrace();
		preempt_latency_start(1);
		__schedule(true);
		preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();

		/*
		 * Check again in case we missed a preemption opportunity
		 * between schedule and now.
		 */
	} while (need_resched());
}

#ifdef CONFIG_PREEMPT
/*
 * this is the entry point to schedule() from in-kernel preemption
 * off of preempt_enable. Kernel preemptions off return from interrupt
 * occur there and call schedule directly.
 */
asmlinkage __visible void __sched notrace preempt_schedule(void)
{
	/*
	 * If there is a non-zero preempt_count or interrupts are disabled,
	 * we do not want to preempt the current task. Just return..
	 */
	if (likely(!preemptible()))
		return;

	preempt_schedule_common();
}
NOKPROBE_SYMBOL(preempt_schedule);
EXPORT_SYMBOL(preempt_schedule);

/**
 * preempt_schedule_notrace - preempt_schedule called by tracing
 *
 * The tracing infrastructure uses preempt_enable_notrace to prevent
 * recursion and tracing preempt enabling caused by the tracing
 * infrastructure itself. But as tracing can happen in areas coming
 * from userspace or just about to enter userspace, a preempt enable
 * can occur before user_exit() is called. This will cause the scheduler
 * to be called when the system is still in usermode.
 *
 * To prevent this, the preempt_enable_notrace will use this function
 * instead of preempt_schedule() to exit user context if needed before
 * calling the scheduler.
 */
asmlinkage __visible void __sched notrace preempt_schedule_notrace(void)
{
	enum ctx_state prev_ctx;

	if (likely(!preemptible()))
		return;

	do {
		/*
		 * Because the function tracer can trace preempt_count_sub()
		 * and it also uses preempt_enable/disable_notrace(), if
		 * NEED_RESCHED is set, the preempt_enable_notrace() called
		 * by the function tracer will call this function again and
		 * cause infinite recursion.
		 *
		 * Preemption must be disabled here before the function
		 * tracer can trace. Break up preempt_disable() into two
		 * calls. One to disable preemption without fear of being
		 * traced. The other to still record the preemption latency,
		 * which can also be traced by the function tracer.
		 */
		preempt_disable_notrace();
		preempt_latency_start(1);
		/*
		 * Needs preempt disabled in case user_exit() is traced
		 * and the tracer calls preempt_enable_notrace() causing
		 * an infinite recursion.
		 */
		prev_ctx = exception_enter();
		__schedule(true);
		exception_exit(prev_ctx);

		preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();
	} while (need_resched());
}
EXPORT_SYMBOL_GPL(preempt_schedule_notrace);

#endif /* CONFIG_PREEMPT */

/*
 * this is the entry point to schedule() from kernel preemption
 * off of irq context.
 * Note, that this is called and return with irqs disabled. This will
 * protect us against recursive calling from irq.
 */
asmlinkage __visible void __sched preempt_schedule_irq(void)
{
	enum ctx_state prev_state;

	/* Catch callers which need to be fixed */
	BUG_ON(preempt_count() || !irqs_disabled());

	prev_state = exception_enter();

	do {
		preempt_disable();
		local_irq_enable();
		__schedule(true);
		local_irq_disable();
		sched_preempt_enable_no_resched();
	} while (need_resched());

	exception_exit(prev_state);
}

int default_wake_function(wait_queue_t *curr, unsigned mode, int wake_flags,
			  void *key)
{
	return try_to_wake_up(curr->private, mode, wake_flags);
}
EXPORT_SYMBOL(default_wake_function);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_RT_MUTEXES

/*
 * rt_mutex_setprio - set the current priority of a task
 * @p: task
 * @prio: prio value (kernel-internal form)
 *
 * This function changes the 'effective' priority of a task. It does
 * not touch ->normal_prio like __setscheduler().
 *
<<<<<<< HEAD
 * Used by the rt_mutex code to implement priority inheritance logic.
 */
void rt_mutex_setprio(struct task_struct *p, int prio)
{
	int oldprio, on_rq, running;
	struct rq *rq;
	const struct sched_class *prev_class;

	BUG_ON(prio < 0 || prio > MAX_PRIO);

	rq = __task_rq_lock(p);
=======
 * Used by the rt_mutex code to implement priority inheritance
 * logic. Call site only calls if the priority of the task changed.
 */
void rt_mutex_setprio(struct task_struct *p, int prio)
{
	int oldprio, queued, running, queue_flag = DEQUEUE_SAVE | DEQUEUE_MOVE;
	const struct sched_class *prev_class;
	struct rq_flags rf;
	struct rq *rq;

	BUG_ON(prio > MAX_PRIO);

	rq = __task_rq_lock(p, &rf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Idle task boosting is a nono in general. There is one
	 * exception, when PREEMPT_RT and NOHZ is active:
	 *
	 * The idle task calls get_next_timer_interrupt() and holds
	 * the timer wheel base->lock on the CPU and another CPU wants
	 * to access the timer (probably to cancel it). We can safely
	 * ignore the boosting request, as the idle CPU runs this code
	 * with interrupts disabled and will complete the lock
	 * protected section without being interrupted. So there is no
	 * real need to boost.
	 */
	if (unlikely(p == rq->idle)) {
		WARN_ON(p != rq->curr);
		WARN_ON(p->pi_blocked_on);
		goto out_unlock;
	}

	trace_sched_pi_setprio(p, prio);
	oldprio = p->prio;
<<<<<<< HEAD
	prev_class = p->sched_class;
	on_rq = p->on_rq;
	running = task_current(rq, p);
	if (on_rq)
		dequeue_task(rq, p, 0);
	if (running)
		p->sched_class->put_prev_task(rq, p);

	if (rt_prio(prio))
		p->sched_class = &rt_sched_class;
	else
		p->sched_class = &fair_sched_class;

	p->prio = prio;

	if (running)
		p->sched_class->set_curr_task(rq);
	if (on_rq)
		enqueue_task(rq, p, oldprio < prio ? ENQUEUE_HEAD : 0);

	check_class_changed(rq, p, prev_class, oldprio);
out_unlock:
	__task_rq_unlock(rq);
}
#endif
void set_user_nice(struct task_struct *p, long nice)
{
	int old_prio, delta, on_rq;
	unsigned long flags;
	struct rq *rq;

	if (TASK_NICE(p) == nice || nice < -20 || nice > 19)
=======

	if (oldprio == prio)
		queue_flag &= ~DEQUEUE_MOVE;

	prev_class = p->sched_class;
	queued = task_on_rq_queued(p);
	running = task_current(rq, p);
	if (queued)
		dequeue_task(rq, p, queue_flag);
	if (running)
		put_prev_task(rq, p);

	/*
	 * Boosting condition are:
	 * 1. -rt task is running and holds mutex A
	 *      --> -dl task blocks on mutex A
	 *
	 * 2. -dl task is running and holds mutex A
	 *      --> -dl task blocks on mutex A and could preempt the
	 *          running task
	 */
	if (dl_prio(prio)) {
		struct task_struct *pi_task = rt_mutex_get_top_task(p);
		if (!dl_prio(p->normal_prio) ||
		    (pi_task && dl_prio(pi_task->prio) &&
		     dl_entity_preempt(&pi_task->dl, &p->dl))) {
			p->dl.dl_boosted = 1;
			queue_flag |= ENQUEUE_REPLENISH;
		} else
			p->dl.dl_boosted = 0;
		p->sched_class = &dl_sched_class;
	} else if (rt_prio(prio)) {
		if (dl_prio(oldprio))
			p->dl.dl_boosted = 0;
		if (oldprio < prio)
			queue_flag |= ENQUEUE_HEAD;
		p->sched_class = &rt_sched_class;
	} else {
		if (dl_prio(oldprio))
			p->dl.dl_boosted = 0;
		if (rt_prio(oldprio))
			p->rt.timeout = 0;
		p->sched_class = &fair_sched_class;
	}

	p->prio = prio;

	if (queued)
		enqueue_task(rq, p, queue_flag);
	if (running)
		set_curr_task(rq, p);

	check_class_changed(rq, p, prev_class, oldprio);
out_unlock:
	preempt_disable(); /* avoid rq from going away on us */
	__task_rq_unlock(rq, &rf);

	balance_callback(rq);
	preempt_enable();
}
#endif

void set_user_nice(struct task_struct *p, long nice)
{
	bool queued, running;
	int old_prio, delta;
	struct rq_flags rf;
	struct rq *rq;

	if (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;
	/*
	 * We have to be careful, if called from sys_setpriority(),
	 * the task might be in the middle of scheduling on another CPU.
	 */
<<<<<<< HEAD
	rq = task_rq_lock(p, &flags);
=======
	rq = task_rq_lock(p, &rf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * The RT priorities are set via sched_setscheduler(), but we still
	 * allow the 'normal' nice value to be set - but as expected
	 * it wont have any effect on scheduling until the task is
<<<<<<< HEAD
	 * SCHED_FIFO/SCHED_RR:
	 */
	if (task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		goto out_unlock;
	}
	on_rq = p->on_rq;
	if (on_rq)
		dequeue_task(rq, p, 0);
=======
	 * SCHED_DEADLINE, SCHED_FIFO or SCHED_RR:
	 */
	if (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		goto out_unlock;
	}
	queued = task_on_rq_queued(p);
	running = task_current(rq, p);
	if (queued)
		dequeue_task(rq, p, DEQUEUE_SAVE);
	if (running)
		put_prev_task(rq, p);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	p->static_prio = NICE_TO_PRIO(nice);
	set_load_weight(p);
	old_prio = p->prio;
	p->prio = effective_prio(p);
	delta = p->prio - old_prio;

<<<<<<< HEAD
	if (on_rq) {
		enqueue_task(rq, p, 0);
=======
	if (queued) {
		enqueue_task(rq, p, ENQUEUE_RESTORE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * If the task increased its priority or is running and
		 * lowered its priority, then reschedule its CPU:
		 */
		if (delta < 0 || (delta > 0 && task_running(rq, p)))
<<<<<<< HEAD
			resched_task(rq->curr);
	}
out_unlock:
	task_rq_unlock(rq, p, &flags);
=======
			resched_curr(rq);
	}
	if (running)
		set_curr_task(rq, p);
out_unlock:
	task_rq_unlock(rq, p, &rf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL(set_user_nice);

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
int can_nice(const struct task_struct *p, const int nice)
{
	/* convert nice value [19,-20] to rlimit style value [1,40] */
<<<<<<< HEAD
	int nice_rlim = 20 - nice;
=======
	int nice_rlim = nice_to_rlimit(nice);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return (nice_rlim <= task_rlimit(p, RLIMIT_NICE) ||
		capable(CAP_SYS_NICE));
}

#ifdef __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, int, increment)
{
	long nice, retval;

	/*
	 * Setpriority might change our priority at the same moment.
	 * We don't have to worry. Conceptually one call occurs first
	 * and we have a single winner.
	 */
<<<<<<< HEAD
	if (increment < -40)
		increment = -40;
	if (increment > 40)
		increment = 40;

	nice = TASK_NICE(current) + increment;
	if (nice < -20)
		nice = -20;
	if (nice > 19)
		nice = 19;

=======
	increment = clamp(increment, -NICE_WIDTH, NICE_WIDTH);
	nice = task_nice(current) + increment;

	nice = clamp_val(nice, MIN_NICE, MAX_NICE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (increment < 0 && !can_nice(current, nice))
		return -EPERM;

	retval = security_task_setnice(current, nice);
	if (retval)
		return retval;

	set_user_nice(current, nice);
	return 0;
}

#endif

/**
 * task_prio - return the priority value of a given task.
 * @p: the task in question.
 *
<<<<<<< HEAD
 * This is the priority value as seen by users in /proc.
=======
 * Return: The priority value as seen by users in /proc.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * RT tasks are offset by -200. Normal tasks are centered
 * around 0, value goes from -16 to +15.
 */
int task_prio(const struct task_struct *p)
{
	return p->prio - MAX_RT_PRIO;
}

/**
<<<<<<< HEAD
 * task_nice - return the nice value of a given task.
 * @p: the task in question.
 */
int task_nice(const struct task_struct *p)
{
	return TASK_NICE(p);
}
EXPORT_SYMBOL(task_nice);

/**
 * idle_cpu - is a given cpu idle currently?
 * @cpu: the processor in question.
=======
 * idle_cpu - is a given cpu idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
int idle_cpu(int cpu)
{
	struct rq *rq = cpu_rq(cpu);

	if (rq->curr != rq->idle)
		return 0;

	if (rq->nr_running)
		return 0;

#ifdef CONFIG_SMP
	if (!llist_empty(&rq->wake_list))
		return 0;
#endif

	return 1;
}

/**
 * idle_task - return the idle task for a given cpu.
 * @cpu: the processor in question.
<<<<<<< HEAD
=======
 *
 * Return: The idle task for the cpu @cpu.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
struct task_struct *idle_task(int cpu)
{
	return cpu_rq(cpu)->idle;
}

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
<<<<<<< HEAD
=======
 *
 * The task of @pid, if found. %NULL otherwise.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static struct task_struct *find_process_by_pid(pid_t pid)
{
	return pid ? find_task_by_vpid(pid) : current;
}

<<<<<<< HEAD
extern struct cpumask hmp_slow_cpu_mask;

/* Actually do priority change: must hold pi & rq lock. */
static void __setscheduler(struct rq *rq, struct task_struct *p,
			   const struct sched_attr *attr)
{
	int policy = attr->sched_policy;

	p->policy = policy;

	if (rt_policy(policy))
		p->rt_priority = attr->sched_priority;
	else
		p->static_prio = NICE_TO_PRIO(attr->sched_nice);

	p->normal_prio = normal_prio(p);

	p->prio = rt_mutex_getprio(p);

	if (rt_prio(p->prio)) {
		p->sched_class = &rt_sched_class;
#ifdef CONFIG_SCHED_HMP
		if (!cpumask_empty(&hmp_slow_cpu_mask))
			if (cpumask_equal(&p->cpus_allowed, cpu_all_mask)) {
				p->nr_cpus_allowed =
					cpumask_weight(&hmp_slow_cpu_mask);
				do_set_cpus_allowed(p, &hmp_slow_cpu_mask);
			}
#endif
	}
	else
		p->sched_class = &fair_sched_class;

	set_load_weight(p);
=======
/*
 * This function initializes the sched_dl_entity of a newly becoming
 * SCHED_DEADLINE task.
 *
 * Only the static values are considered here, the actual runtime and the
 * absolute deadline will be properly calculated when the task is enqueued
 * for the first time with its new policy.
 */
static void
__setparam_dl(struct task_struct *p, const struct sched_attr *attr)
{
	struct sched_dl_entity *dl_se = &p->dl;

	dl_se->dl_runtime = attr->sched_runtime;
	dl_se->dl_deadline = attr->sched_deadline;
	dl_se->dl_period = attr->sched_period ?: dl_se->dl_deadline;
	dl_se->flags = attr->sched_flags;
	dl_se->dl_bw = to_ratio(dl_se->dl_period, dl_se->dl_runtime);
	dl_se->dl_density = to_ratio(dl_se->dl_deadline, dl_se->dl_runtime);

	/*
	 * Changing the parameters of a task is 'tricky' and we're not doing
	 * the correct thing -- also see task_dead_dl() and switched_from_dl().
	 *
	 * What we SHOULD do is delay the bandwidth release until the 0-lag
	 * point. This would include retaining the task_struct until that time
	 * and change dl_overflow() to not immediately decrement the current
	 * amount.
	 *
	 * Instead we retain the current runtime/deadline and let the new
	 * parameters take effect after the current reservation period lapses.
	 * This is safe (albeit pessimistic) because the 0-lag point is always
	 * before the current scheduling deadline.
	 *
	 * We can still have temporary overloads because we do not delay the
	 * change in bandwidth until that time; so admission control is
	 * not on the safe side. It does however guarantee tasks will never
	 * consume more than promised.
	 */
}

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#define SETPARAM_POLICY	-1

static void __setscheduler_params(struct task_struct *p,
		const struct sched_attr *attr)
{
	int policy = attr->sched_policy;

	if (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	if (dl_policy(policy))
		__setparam_dl(p, attr);
	else if (fair_policy(policy))
		p->static_prio = NICE_TO_PRIO(attr->sched_nice);

	/*
	 * __sched_setscheduler() ensures attr->sched_priority == 0 when
	 * !rt_policy. Always setting this ensures that things like
	 * getparam()/getattr() don't report silly values for !rt tasks.
	 */
	p->rt_priority = attr->sched_priority;
	p->normal_prio = normal_prio(p);
	set_load_weight(p);
}

/* Actually do priority change: must hold pi & rq lock. */
static void __setscheduler(struct rq *rq, struct task_struct *p,
			   const struct sched_attr *attr, bool keep_boost)
{
	__setscheduler_params(p, attr);

	/*
	 * Keep a potential priority boosting if called from
	 * sched_setscheduler().
	 */
	if (keep_boost)
		p->prio = rt_mutex_get_effective_prio(p, normal_prio(p));
	else
		p->prio = normal_prio(p);

	if (dl_prio(p->prio))
		p->sched_class = &dl_sched_class;
	else if (rt_prio(p->prio))
		p->sched_class = &rt_sched_class;
	else
		p->sched_class = &fair_sched_class;
}

static void
__getparam_dl(struct task_struct *p, struct sched_attr *attr)
{
	struct sched_dl_entity *dl_se = &p->dl;

	attr->sched_priority = p->rt_priority;
	attr->sched_runtime = dl_se->dl_runtime;
	attr->sched_deadline = dl_se->dl_deadline;
	attr->sched_period = dl_se->dl_period;
	attr->sched_flags = dl_se->flags;
}

/*
 * This function validates the new parameters of a -deadline task.
 * We ask for the deadline not being zero, and greater or equal
 * than the runtime, as well as the period of being zero or
 * greater than deadline. Furthermore, we have to be sure that
 * user parameters are above the internal resolution of 1us (we
 * check sched_runtime only since it is always the smaller one) and
 * below 2^63 ns (we have to check both sched_deadline and
 * sched_period, as the latter can be zero).
 */
static bool
__checkparam_dl(const struct sched_attr *attr)
{
	/* deadline != 0 */
	if (attr->sched_deadline == 0)
		return false;

	/*
	 * Since we truncate DL_SCALE bits, make sure we're at least
	 * that big.
	 */
	if (attr->sched_runtime < (1ULL << DL_SCALE))
		return false;

	/*
	 * Since we use the MSB for wrap-around and sign issues, make
	 * sure it's not set (mind that period can be equal to zero).
	 */
	if (attr->sched_deadline & (1ULL << 63) ||
	    attr->sched_period & (1ULL << 63))
		return false;

	/* runtime <= deadline <= period (if period != 0) */
	if ((attr->sched_period != 0 &&
	     attr->sched_period < attr->sched_deadline) ||
	    attr->sched_deadline < attr->sched_runtime)
		return false;

	return true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * check the target process has a UID that matches the current process's
 */
static bool check_same_owner(struct task_struct *p)
{
	const struct cred *cred = current_cred(), *pcred;
	bool match;

	rcu_read_lock();
	pcred = __task_cred(p);
	match = (uid_eq(cred->euid, pcred->euid) ||
		 uid_eq(cred->euid, pcred->uid));
	rcu_read_unlock();
	return match;
}

<<<<<<< HEAD
static int __sched_setscheduler(struct task_struct *p,
				const struct sched_attr *attr,
				bool user)
{
	int retval, oldprio, oldpolicy = -1, on_rq, running;
	int policy = attr->sched_policy;
	unsigned long flags;
	const struct sched_class *prev_class;
	struct rq *rq;
	int reset_on_fork;

	/* may grab non-irq protected spin_locks */
	BUG_ON(in_interrupt());
=======
static bool dl_param_changed(struct task_struct *p,
		const struct sched_attr *attr)
{
	struct sched_dl_entity *dl_se = &p->dl;

	if (dl_se->dl_runtime != attr->sched_runtime ||
		dl_se->dl_deadline != attr->sched_deadline ||
		dl_se->dl_period != attr->sched_period ||
		dl_se->flags != attr->sched_flags)
		return true;

	return false;
}

static int __sched_setscheduler(struct task_struct *p,
				const struct sched_attr *attr,
				bool user, bool pi)
{
	int newprio = dl_policy(attr->sched_policy) ? MAX_DL_PRIO - 1 :
		      MAX_RT_PRIO - 1 - attr->sched_priority;
	int retval, oldprio, oldpolicy = -1, queued, running;
	int new_effective_prio, policy = attr->sched_policy;
	const struct sched_class *prev_class;
	struct rq_flags rf;
	int reset_on_fork;
	int queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE;
	struct rq *rq;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && in_interrupt());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
recheck:
	/* double check policy once rq lock held */
	if (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} else {
<<<<<<< HEAD
		reset_on_fork = !!(policy & SCHED_RESET_ON_FORK);
		policy &= ~SCHED_RESET_ON_FORK;

		if (policy != SCHED_FIFO && policy != SCHED_RR &&
				policy != SCHED_NORMAL && policy != SCHED_BATCH &&
				policy != SCHED_IDLE)
			return -EINVAL;
	}

=======
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		if (!valid_policy(policy))
			return -EINVAL;
	}

	if (attr->sched_flags & ~(SCHED_FLAG_RESET_ON_FORK))
		return -EINVAL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_USER_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
<<<<<<< HEAD
	if (attr->sched_priority < 0 ||
	    (p->mm && attr->sched_priority > MAX_USER_RT_PRIO-1) ||
	    (!p->mm && attr->sched_priority > MAX_RT_PRIO-1))
		return -EINVAL;
	if (rt_policy(policy) != (attr->sched_priority != 0))
=======
	if ((p->mm && attr->sched_priority > MAX_USER_RT_PRIO-1) ||
	    (!p->mm && attr->sched_priority > MAX_RT_PRIO-1))
		return -EINVAL;
	if ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_priority != 0)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;

	/*
	 * Allow unprivileged RT tasks to decrease priority:
	 */
	if (user && !capable(CAP_SYS_NICE)) {
		if (fair_policy(policy)) {
<<<<<<< HEAD
			if (!can_nice(p, attr->sched_nice))
=======
			if (attr->sched_nice < task_nice(p) &&
			    !can_nice(p, attr->sched_nice))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				return -EPERM;
		}

		if (rt_policy(policy)) {
			unsigned long rlim_rtprio =
					task_rlimit(p, RLIMIT_RTPRIO);

			/* can't set/change the rt policy */
			if (policy != p->policy && !rlim_rtprio)
				return -EPERM;

			/* can't increase priority */
			if (attr->sched_priority > p->rt_priority &&
			    attr->sched_priority > rlim_rtprio)
				return -EPERM;
		}

<<<<<<< HEAD
=======
		 /*
		  * Can't set/change SCHED_DEADLINE policy at all for now
		  * (safest behavior); in the future we would like to allow
		  * unprivileged DL tasks to increase their relative deadline
		  * or reduce their runtime (both ways reducing utilization)
		  */
		if (dl_policy(policy))
			return -EPERM;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * Treat SCHED_IDLE as nice 20. Only allow a switch to
		 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
		 */
<<<<<<< HEAD
		if (p->policy == SCHED_IDLE && policy != SCHED_IDLE) {
			if (!can_nice(p, TASK_NICE(p)))
=======
		if (idle_policy(p->policy) && !idle_policy(policy)) {
			if (!can_nice(p, task_nice(p)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				return -EPERM;
		}

		/* can't change other user's priorities */
		if (!check_same_owner(p))
			return -EPERM;

		/* Normal users shall not reset the sched_reset_on_fork flag */
		if (p->sched_reset_on_fork && !reset_on_fork)
			return -EPERM;
	}

	if (user) {
		retval = security_task_setscheduler(p);
		if (retval)
			return retval;
	}

	/*
	 * make sure no PI-waiters arrive (or leave) while we are
	 * changing the priority of the task:
	 *
	 * To be able to change p->policy safely, the appropriate
	 * runqueue lock must be held.
	 */
<<<<<<< HEAD
	rq = task_rq_lock(p, &flags);
=======
	rq = task_rq_lock(p, &rf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Changing the policy of the stop threads its a very bad idea
	 */
	if (p == rq->stop) {
<<<<<<< HEAD
		task_rq_unlock(rq, p, &flags);
=======
		task_rq_unlock(rq, p, &rf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;
	}

	/*
<<<<<<< HEAD
	 * If not changing anything there's no need to proceed further:
	 */
	if (unlikely(policy == p->policy)) {
		if (fair_policy(policy) && attr->sched_nice != TASK_NICE(p))
			goto change;
		if (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			goto change;

		task_rq_unlock(rq, p, &flags);
=======
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	if (unlikely(policy == p->policy)) {
		if (fair_policy(policy) && attr->sched_nice != task_nice(p))
			goto change;
		if (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			goto change;
		if (dl_policy(policy) && dl_param_changed(p, attr))
			goto change;

		p->sched_reset_on_fork = reset_on_fork;
		task_rq_unlock(rq, p, &rf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 0;
	}
change:

<<<<<<< HEAD
#ifdef CONFIG_RT_GROUP_SCHED
	if (user) {
=======
	if (user) {
#ifdef CONFIG_RT_GROUP_SCHED
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * Do not allow realtime tasks into groups that have no runtime
		 * assigned.
		 */
		if (rt_bandwidth_enabled() && rt_policy(policy) &&
				task_group(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_group_is_autogroup(task_group(p))) {
<<<<<<< HEAD
			task_rq_unlock(rq, p, &flags);
			return -EPERM;
		}
	}
#endif
=======
			task_rq_unlock(rq, p, &rf);
			return -EPERM;
		}
#endif
#ifdef CONFIG_SMP
		if (dl_bandwidth_enabled() && dl_policy(policy)) {
			cpumask_t *span = rq->rd->span;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			if (!cpumask_subset(span, &p->cpus_allowed) ||
			    rq->rd->dl_bw.bw == 0) {
				task_rq_unlock(rq, p, &rf);
				return -EPERM;
			}
		}
#endif
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* recheck policy now with rq lock held */
	if (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
<<<<<<< HEAD
		task_rq_unlock(rq, p, &flags);
		goto recheck;
	}
	on_rq = p->on_rq;
	running = task_current(rq, p);
	if (on_rq)
		dequeue_task(rq, p, 0);
	if (running)
		p->sched_class->put_prev_task(rq, p);

	p->sched_reset_on_fork = reset_on_fork;

	oldprio = p->prio;
	prev_class = p->sched_class;
	__setscheduler(rq, p, attr);

	if (running)
		p->sched_class->set_curr_task(rq);
	if (on_rq)
		enqueue_task(rq, p, 0);

	check_class_changed(rq, p, prev_class, oldprio);
	task_rq_unlock(rq, p, &flags);

	rt_mutex_adjust_pi(p);
=======
		task_rq_unlock(rq, p, &rf);
		goto recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	if ((dl_policy(policy) || dl_task(p)) && dl_overflow(p, policy, attr)) {
		task_rq_unlock(rq, p, &rf);
		return -EBUSY;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	if (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		new_effective_prio = rt_mutex_get_effective_prio(p, newprio);
		if (new_effective_prio == oldprio)
			queue_flags &= ~DEQUEUE_MOVE;
	}

	queued = task_on_rq_queued(p);
	running = task_current(rq, p);
	if (queued)
		dequeue_task(rq, p, queue_flags);
	if (running)
		put_prev_task(rq, p);

	prev_class = p->sched_class;
	__setscheduler(rq, p, attr, pi);

	if (queued) {
		/*
		 * We enqueue to tail when the priority of a task is
		 * increased (user space view).
		 */
		if (oldprio < p->prio)
			queue_flags |= ENQUEUE_HEAD;

		enqueue_task(rq, p, queue_flags);
	}
	if (running)
		set_curr_task(rq, p);

	check_class_changed(rq, p, prev_class, oldprio);
	preempt_disable(); /* avoid rq from going away on us */
	task_rq_unlock(rq, p, &rf);

	if (pi)
		rt_mutex_adjust_pi(p);

	/*
	 * Run balance callbacks after we've adjusted the PI chain.
	 */
	balance_callback(rq);
	preempt_enable();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
=======
static int _sched_setscheduler(struct task_struct *p, int policy,
			       const struct sched_param *param, bool check)
{
	struct sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority,
		.sched_nice	= PRIO_TO_NICE(p->static_prio),
	};

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	if ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	return __sched_setscheduler(p, &attr, check, true);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * sched_setscheduler - change the scheduling policy and/or RT priority of a thread.
 * @p: the task in question.
 * @policy: new policy.
 * @param: structure containing the new RT priority.
 *
<<<<<<< HEAD
=======
 * Return: 0 on success. An error code otherwise.
 *
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * NOTE that the task may be already dead.
 */
int sched_setscheduler(struct task_struct *p, int policy,
		       const struct sched_param *param)
{
<<<<<<< HEAD
	struct sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority
	};
	return __sched_setscheduler(p, &attr, true);
=======
	return _sched_setscheduler(p, policy, param, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(sched_setscheduler);

int sched_setattr(struct task_struct *p, const struct sched_attr *attr)
{
<<<<<<< HEAD
	return __sched_setscheduler(p, attr, true);
=======
	return __sched_setscheduler(p, attr, true, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(sched_setattr);

/**
 * sched_setscheduler_nocheck - change the scheduling policy and/or RT priority of a thread from kernelspace.
 * @p: the task in question.
 * @policy: new policy.
 * @param: structure containing the new RT priority.
 *
 * Just like sched_setscheduler, only don't bother checking if the
 * current context has permission.  For example, this is needed in
 * stop_machine(): we create temporary high priority worker threads,
 * but our caller might not have that capability.
<<<<<<< HEAD
=======
 *
 * Return: 0 on success. An error code otherwise.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
int sched_setscheduler_nocheck(struct task_struct *p, int policy,
			       const struct sched_param *param)
{
<<<<<<< HEAD
	struct sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority
	};
	return __sched_setscheduler(p, &attr, false);
}
EXPORT_SYMBOL(sched_setscheduler_nocheck);
=======
	return _sched_setscheduler(p, policy, param, false);
}
EXPORT_SYMBOL_GPL(sched_setscheduler_nocheck);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int
do_sched_setscheduler(pid_t pid, int policy, struct sched_param __user *param)
{
	struct sched_param lparam;
	struct task_struct *p;
	int retval;

	if (!param || pid < 0)
		return -EINVAL;
	if (copy_from_user(&lparam, param, sizeof(struct sched_param)))
		return -EFAULT;

	rcu_read_lock();
	retval = -ESRCH;
	p = find_process_by_pid(pid);
	if (p != NULL)
		retval = sched_setscheduler(p, policy, &lparam);
	rcu_read_unlock();

	return retval;
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
static int sched_copy_attr(struct sched_attr __user *uattr,
			   struct sched_attr *attr)
{
	u32 size;
	int ret;

	if (!access_ok(VERIFY_WRITE, uattr, SCHED_ATTR_SIZE_VER0))
		return -EFAULT;

	/*
	 * zero the full structure, so that a short copy will be nice.
	 */
	memset(attr, 0, sizeof(*attr));

	ret = get_user(size, &uattr->size);
	if (ret)
		return ret;

	if (size > PAGE_SIZE)	/* silly large */
		goto err_size;

	if (!size)		/* abi compat */
		size = SCHED_ATTR_SIZE_VER0;

	if (size < SCHED_ATTR_SIZE_VER0)
		goto err_size;

	/*
	 * If we're handed a bigger struct than we know of,
	 * ensure all the unknown bits are 0 - i.e. new
	 * user-space does not rely on any kernel feature
	 * extensions we dont know about yet.
	 */
	if (size > sizeof(*attr)) {
		unsigned char __user *addr;
		unsigned char __user *end;
		unsigned char val;

		addr = (void __user *)uattr + sizeof(*attr);
		end  = (void __user *)uattr + size;

		for (; addr < end; addr++) {
			ret = get_user(val, addr);
			if (ret)
				return ret;
			if (val)
				goto err_size;
		}
		size = sizeof(*attr);
	}

	ret = copy_from_user(attr, uattr, size);
	if (ret)
		return -EFAULT;

	/*
	 * XXX: do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
<<<<<<< HEAD
	attr->sched_nice = clamp(attr->sched_nice, -20, 19);

out:
	return ret;

err_size:
	put_user(sizeof(*attr), &uattr->size);
	ret = -E2BIG;
	goto out;
=======
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	return 0;

err_size:
	put_user(sizeof(*attr), &uattr->size);
	return -E2BIG;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * sys_sched_setscheduler - set/change the scheduler policy and RT priority
 * @pid: the pid in question.
 * @policy: new policy.
 * @param: structure containing the new RT priority.
<<<<<<< HEAD
=======
 *
 * Return: 0 on success. An error code otherwise.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, int, policy,
		struct sched_param __user *, param)
{
	/* negative values for policy are not valid */
	if (policy < 0)
		return -EINVAL;

	return do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
<<<<<<< HEAD
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, struct sched_param __user *, param)
{
	return do_sched_setscheduler(pid, -1, param);
=======
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, struct sched_param __user *, param)
{
	return do_sched_setscheduler(pid, SETPARAM_POLICY, param);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
<<<<<<< HEAD
 * @attr: structure containing the extended parameters.
 */
SYSCALL_DEFINE2(sched_setattr, pid_t, pid, struct sched_attr __user *, uattr)
=======
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, struct sched_attr __user *, uattr,
			       unsigned int, flags)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct sched_attr attr;
	struct task_struct *p;
	int retval;

<<<<<<< HEAD
	if (!uattr || pid < 0)
		return -EINVAL;

	if (sched_copy_attr(uattr, &attr))
		return -EFAULT;
=======
	if (!uattr || pid < 0 || flags)
		return -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	if (retval)
		return retval;

	if ((int)attr.sched_policy < 0)
		return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rcu_read_lock();
	retval = -ESRCH;
	p = find_process_by_pid(pid);
	if (p != NULL)
		retval = sched_setattr(p, &attr);
	rcu_read_unlock();

	return retval;
}

/**
 * sys_sched_getscheduler - get the policy (scheduling class) of a thread
 * @pid: the pid in question.
<<<<<<< HEAD
=======
 *
 * Return: On success, the policy of the thread. Otherwise, a negative error
 * code.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
SYSCALL_DEFINE1(sched_getscheduler, pid_t, pid)
{
	struct task_struct *p;
	int retval;

	if (pid < 0)
		return -EINVAL;

	retval = -ESRCH;
	rcu_read_lock();
	p = find_process_by_pid(pid);
	if (p) {
		retval = security_task_getscheduler(p);
		if (!retval)
			retval = p->policy
				| (p->sched_reset_on_fork ? SCHED_RESET_ON_FORK : 0);
	}
	rcu_read_unlock();
	return retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
<<<<<<< HEAD
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, struct sched_param __user *, param)
{
	struct sched_param lp;
=======
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, struct sched_param __user *, param)
{
	struct sched_param lp = { .sched_priority = 0 };
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct task_struct *p;
	int retval;

	if (!param || pid < 0)
		return -EINVAL;

	rcu_read_lock();
	p = find_process_by_pid(pid);
	retval = -ESRCH;
	if (!p)
		goto out_unlock;

	retval = security_task_getscheduler(p);
	if (retval)
		goto out_unlock;

<<<<<<< HEAD
	lp.sched_priority = p->rt_priority;
=======
	if (task_has_rt_policy(p))
		lp.sched_priority = p->rt_priority;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_read_unlock();

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	retval = copy_to_user(param, &lp, sizeof(*param)) ? -EFAULT : 0;

	return retval;

out_unlock:
	rcu_read_unlock();
	return retval;
}

static int sched_read_attr(struct sched_attr __user *uattr,
			   struct sched_attr *attr,
			   unsigned int usize)
{
	int ret;

	if (!access_ok(VERIFY_WRITE, uattr, usize))
		return -EFAULT;

	/*
	 * If we're handed a smaller struct than we know of,
	 * ensure all the unknown bits are 0 - i.e. old
	 * user-space does not get uncomplete information.
	 */
	if (usize < sizeof(*attr)) {
		unsigned char *addr;
		unsigned char *end;

		addr = (void *)attr + usize;
		end  = (void *)attr + sizeof(*attr);

		for (; addr < end; addr++) {
			if (*addr)
<<<<<<< HEAD
				goto err_size;
=======
				return -EFBIG;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		attr->size = usize;
	}

	ret = copy_to_user(uattr, attr, attr->size);
	if (ret)
		return -EFAULT;

<<<<<<< HEAD
out:
	return ret;

err_size:
	ret = -E2BIG;
	goto out;
}

/**
 * sys_sched_getattr - same as above, but with extended "sched_param"
 * @pid: the pid in question.
 * @attr: structure containing the extended parameters.
 * @size: sizeof(attr) for fwd/bwd comp.
 */
SYSCALL_DEFINE3(sched_getattr, pid_t, pid, struct sched_attr __user *, uattr,
		unsigned int, size)
=======
	return 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @size: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, struct sched_attr __user *, uattr,
		unsigned int, size, unsigned int, flags)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct sched_attr attr = {
		.size = sizeof(struct sched_attr),
	};
	struct task_struct *p;
	int retval;

	if (!uattr || pid < 0 || size > PAGE_SIZE ||
<<<<<<< HEAD
	    size < SCHED_ATTR_SIZE_VER0)
=======
	    size < SCHED_ATTR_SIZE_VER0 || flags)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;

	rcu_read_lock();
	p = find_process_by_pid(pid);
	retval = -ESRCH;
	if (!p)
		goto out_unlock;

	retval = security_task_getscheduler(p);
	if (retval)
		goto out_unlock;

	attr.sched_policy = p->policy;
<<<<<<< HEAD
	if (task_has_rt_policy(p))
		attr.sched_priority = p->rt_priority;
	else
		attr.sched_nice = TASK_NICE(p);
=======
	if (p->sched_reset_on_fork)
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
	if (task_has_dl_policy(p))
		__getparam_dl(p, &attr);
	else if (task_has_rt_policy(p))
		attr.sched_priority = p->rt_priority;
	else
		attr.sched_nice = task_nice(p);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rcu_read_unlock();

	retval = sched_read_attr(uattr, &attr, size);
	return retval;

out_unlock:
	rcu_read_unlock();
	return retval;
}

long sched_setaffinity(pid_t pid, const struct cpumask *in_mask)
{
	cpumask_var_t cpus_allowed, new_mask;
	struct task_struct *p;
	int retval;

<<<<<<< HEAD
	get_online_cpus();
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_read_lock();

	p = find_process_by_pid(pid);
	if (!p) {
		rcu_read_unlock();
<<<<<<< HEAD
		put_online_cpus();
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -ESRCH;
	}

	/* Prevent p going away */
	get_task_struct(p);
	rcu_read_unlock();

	if (p->flags & PF_NO_SETAFFINITY) {
		retval = -EINVAL;
		goto out_put_task;
	}
	if (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL)) {
		retval = -ENOMEM;
		goto out_put_task;
	}
	if (!alloc_cpumask_var(&new_mask, GFP_KERNEL)) {
		retval = -ENOMEM;
		goto out_free_cpus_allowed;
	}
	retval = -EPERM;
	if (!check_same_owner(p)) {
		rcu_read_lock();
		if (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE)) {
			rcu_read_unlock();
<<<<<<< HEAD
			goto out_unlock;
=======
			goto out_free_new_mask;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		rcu_read_unlock();
	}

	retval = security_task_setscheduler(p);
	if (retval)
<<<<<<< HEAD
		goto out_unlock;

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_mask, in_mask, cpus_allowed);
again:
	retval = set_cpus_allowed_ptr(p, new_mask);
=======
		goto out_free_new_mask;


	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_mask, in_mask, cpus_allowed);

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
#ifdef CONFIG_SMP
	if (task_has_dl_policy(p) && dl_bandwidth_enabled()) {
		rcu_read_lock();
		if (!cpumask_subset(task_rq(p)->rd->span, new_mask)) {
			retval = -EBUSY;
			rcu_read_unlock();
			goto out_free_new_mask;
		}
		rcu_read_unlock();
	}
#endif
again:
	retval = __set_cpus_allowed_ptr(p, new_mask, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!retval) {
		cpuset_cpus_allowed(p, cpus_allowed);
		if (!cpumask_subset(new_mask, cpus_allowed)) {
			/*
			 * We must have raced with a concurrent cpuset
			 * update. Just reset the cpus_allowed to the
			 * cpuset's cpus_allowed
			 */
			cpumask_copy(new_mask, cpus_allowed);
			goto again;
		}
	}
<<<<<<< HEAD
out_unlock:
=======
out_free_new_mask:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	free_cpumask_var(new_mask);
out_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
out_put_task:
	put_task_struct(p);
<<<<<<< HEAD
	put_online_cpus();
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return retval;
}

static int get_user_cpu_mask(unsigned long __user *user_mask_ptr, unsigned len,
			     struct cpumask *new_mask)
{
	if (len < cpumask_size())
		cpumask_clear(new_mask);
	else if (len > cpumask_size())
		len = cpumask_size();

	return copy_from_user(new_mask, user_mask_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the cpu affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new cpu mask
<<<<<<< HEAD
=======
 *
 * Return: 0 on success. An error code otherwise.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, unsigned int, len,
		unsigned long __user *, user_mask_ptr)
{
	cpumask_var_t new_mask;
	int retval;

	if (!alloc_cpumask_var(&new_mask, GFP_KERNEL))
		return -ENOMEM;

	retval = get_user_cpu_mask(user_mask_ptr, len, new_mask);
	if (retval == 0)
		retval = sched_setaffinity(pid, new_mask);
	free_cpumask_var(new_mask);
	return retval;
}

long sched_getaffinity(pid_t pid, struct cpumask *mask)
{
	struct task_struct *p;
	unsigned long flags;
	int retval;

<<<<<<< HEAD
	get_online_cpus();
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_read_lock();

	retval = -ESRCH;
	p = find_process_by_pid(pid);
	if (!p)
		goto out_unlock;

	retval = security_task_getscheduler(p);
	if (retval)
		goto out_unlock;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
<<<<<<< HEAD
	cpumask_and(mask, &p->cpus_allowed, cpu_online_mask);
=======
	cpumask_and(mask, &p->cpus_allowed, cpu_active_mask);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

out_unlock:
	rcu_read_unlock();
<<<<<<< HEAD
	put_online_cpus();
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return retval;
}

/**
 * sys_sched_getaffinity - get the cpu affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to hold the current cpu mask
<<<<<<< HEAD
=======
 *
 * Return: size of CPU mask copied to user_mask_ptr on success. An
 * error code otherwise.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, unsigned int, len,
		unsigned long __user *, user_mask_ptr)
{
	int ret;
	cpumask_var_t mask;

	if ((len * BITS_PER_BYTE) < nr_cpu_ids)
		return -EINVAL;
	if (len & (sizeof(unsigned long)-1))
		return -EINVAL;

	if (!alloc_cpumask_var(&mask, GFP_KERNEL))
		return -ENOMEM;

	ret = sched_getaffinity(pid, mask);
	if (ret == 0) {
		size_t retlen = min_t(size_t, len, cpumask_size());

		if (copy_to_user(user_mask_ptr, mask, retlen))
			ret = -EFAULT;
		else
			ret = retlen;
	}
	free_cpumask_var(mask);

	return ret;
}

/**
 * sys_sched_yield - yield the current processor to other threads.
 *
 * This function yields the current CPU to other tasks. If there are no
 * other threads running on this CPU then this function will return.
<<<<<<< HEAD
=======
 *
 * Return: 0.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
SYSCALL_DEFINE0(sched_yield)
{
	struct rq *rq = this_rq_lock();

<<<<<<< HEAD
	schedstat_inc(rq, yld_count);
=======
	schedstat_inc(rq->yld_count);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	current->sched_class->yield_task(rq);

	/*
	 * Since we are going to call schedule() anyway, there's
	 * no need to preempt or enable interrupts:
	 */
	__release(rq->lock);
	spin_release(&rq->lock.dep_map, 1, _THIS_IP_);
	do_raw_spin_unlock(&rq->lock);
	sched_preempt_enable_no_resched();

	schedule();

	return 0;
}

<<<<<<< HEAD
static inline int should_resched(void)
{
	return need_resched() && !(preempt_count() & PREEMPT_ACTIVE);
}

static void __cond_resched(void)
{
	add_preempt_count(PREEMPT_ACTIVE);
	__schedule();
	sub_preempt_count(PREEMPT_ACTIVE);
}

int __sched _cond_resched(void)
{
	if (should_resched()) {
		__cond_resched();
=======
#ifndef CONFIG_PREEMPT
int __sched _cond_resched(void)
{
	if (should_resched(0)) {
		preempt_schedule_common();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 1;
	}
	return 0;
}
EXPORT_SYMBOL(_cond_resched);
<<<<<<< HEAD
=======
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * __cond_resched_lock() - if a reschedule is pending, drop the given lock,
 * call schedule, and on return reacquire the lock.
 *
 * This works OK both with and without CONFIG_PREEMPT. We do strange low-level
 * operations here to prevent schedule() from being called twice (once via
 * spin_unlock(), once by hand).
 */
int __cond_resched_lock(spinlock_t *lock)
{
<<<<<<< HEAD
	int resched = should_resched();
=======
	int resched = should_resched(PREEMPT_LOCK_OFFSET);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret = 0;

	lockdep_assert_held(lock);

	if (spin_needbreak(lock) || resched) {
		spin_unlock(lock);
		if (resched)
<<<<<<< HEAD
			__cond_resched();
=======
			preempt_schedule_common();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		else
			cpu_relax();
		ret = 1;
		spin_lock(lock);
	}
	return ret;
}
EXPORT_SYMBOL(__cond_resched_lock);

int __sched __cond_resched_softirq(void)
{
	BUG_ON(!in_softirq());

<<<<<<< HEAD
	if (should_resched()) {
		local_bh_enable();
		__cond_resched();
=======
	if (should_resched(SOFTIRQ_DISABLE_OFFSET)) {
		local_bh_enable();
		preempt_schedule_common();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		local_bh_disable();
		return 1;
	}
	return 0;
}
EXPORT_SYMBOL(__cond_resched_softirq);

/**
 * yield - yield the current processor to other threads.
 *
 * Do not ever use this function, there's a 99% chance you're doing it wrong.
 *
 * The scheduler is at all times free to pick the calling task as the most
 * eligible task to run, if removing the yield() call from your code breaks
 * it, its already broken.
 *
 * Typical broken usage is:
 *
 * while (!event)
 * 	yield();
 *
 * where one assumes that yield() will let 'the other' process run that will
 * make event true. If the current task is a SCHED_FIFO task that will never
 * happen. Never use yield() as a progress guarantee!!
 *
 * If you want to use yield() to wait for something, use wait_event().
 * If you want to use yield() to be 'nice' for others, use cond_resched().
 * If you still want to use yield(), do not!
 */
void __sched yield(void)
{
	set_current_state(TASK_RUNNING);
	sys_sched_yield();
}
EXPORT_SYMBOL(yield);

/**
 * yield_to - yield the current processor to another thread in
 * your thread group, or accelerate that thread toward the
 * processor it's on.
 * @p: target task
 * @preempt: whether task preemption is allowed or not
 *
 * It's the caller's job to ensure that the target task struct
 * can't go away on us before we can do any checks.
 *
<<<<<<< HEAD
 * Returns:
=======
 * Return:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *	true (>0) if we indeed boosted the target task.
 *	false (0) if we failed to boost the target.
 *	-ESRCH if there's no task to yield to.
 */
<<<<<<< HEAD
bool __sched yield_to(struct task_struct *p, bool preempt)
=======
int __sched yield_to(struct task_struct *p, bool preempt)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct task_struct *curr = current;
	struct rq *rq, *p_rq;
	unsigned long flags;
	int yielded = 0;

	local_irq_save(flags);
	rq = this_rq();

again:
	p_rq = task_rq(p);
	/*
	 * If we're the only runnable task on the rq and target rq also
	 * has only one task, there's absolutely no point in yielding.
	 */
	if (rq->nr_running == 1 && p_rq->nr_running == 1) {
		yielded = -ESRCH;
		goto out_irq;
	}

	double_rq_lock(rq, p_rq);
<<<<<<< HEAD
	while (task_rq(p) != p_rq) {
=======
	if (task_rq(p) != p_rq) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		double_rq_unlock(rq, p_rq);
		goto again;
	}

	if (!curr->sched_class->yield_to_task)
		goto out_unlock;

	if (curr->sched_class != p->sched_class)
		goto out_unlock;

	if (task_running(p_rq, p) || p->state)
		goto out_unlock;

	yielded = curr->sched_class->yield_to_task(rq, p, preempt);
	if (yielded) {
<<<<<<< HEAD
		schedstat_inc(rq, yld_count);
=======
		schedstat_inc(rq->yld_count);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * Make p's CPU reschedule; pick_next_entity takes care of
		 * fairness.
		 */
		if (preempt && rq != p_rq)
<<<<<<< HEAD
			resched_task(p_rq->curr);
=======
			resched_curr(p_rq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

out_unlock:
	double_rq_unlock(rq, p_rq);
out_irq:
	local_irq_restore(flags);

	if (yielded > 0)
		schedule();

	return yielded;
}
EXPORT_SYMBOL_GPL(yield_to);

/*
 * This task is about to go to sleep on IO. Increment rq->nr_iowait so
 * that process accounting knows that this is a task in IO wait state.
 */
long __sched io_schedule_timeout(long timeout)
{
	int old_iowait = current->in_iowait;
	struct rq *rq;
	long ret;

	current->in_iowait = 1;
	blk_schedule_flush_plug(current);

	delayacct_blkio_start();
	rq = raw_rq();
	atomic_inc(&rq->nr_iowait);
	ret = schedule_timeout(timeout);
	current->in_iowait = old_iowait;
	atomic_dec(&rq->nr_iowait);
	delayacct_blkio_end();

	return ret;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(io_schedule_timeout);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
<<<<<<< HEAD
 * this syscall returns the maximum rt_priority that can be used
 * by a given scheduling class.
=======
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
SYSCALL_DEFINE1(sched_get_priority_max, int, policy)
{
	int ret = -EINVAL;

	switch (policy) {
	case SCHED_FIFO:
	case SCHED_RR:
		ret = MAX_USER_RT_PRIO-1;
		break;
<<<<<<< HEAD
=======
	case SCHED_DEADLINE:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case SCHED_NORMAL:
	case SCHED_BATCH:
	case SCHED_IDLE:
		ret = 0;
		break;
	}
	return ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
<<<<<<< HEAD
 * this syscall returns the minimum rt_priority that can be used
 * by a given scheduling class.
=======
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
SYSCALL_DEFINE1(sched_get_priority_min, int, policy)
{
	int ret = -EINVAL;

	switch (policy) {
	case SCHED_FIFO:
	case SCHED_RR:
		ret = 1;
		break;
<<<<<<< HEAD
=======
	case SCHED_DEADLINE:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case SCHED_NORMAL:
	case SCHED_BATCH:
	case SCHED_IDLE:
		ret = 0;
	}
	return ret;
}

/**
 * sys_sched_rr_get_interval - return the default timeslice of a process.
 * @pid: pid of the process.
 * @interval: userspace pointer to the timeslice value.
 *
 * this syscall writes the default timeslice value of a given process
 * into the user-space timespec buffer. A value of '0' means infinity.
<<<<<<< HEAD
=======
 *
 * Return: On success, 0 and the timeslice is in @interval. Otherwise,
 * an error code.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
SYSCALL_DEFINE2(sched_rr_get_interval, pid_t, pid,
		struct timespec __user *, interval)
{
	struct task_struct *p;
	unsigned int time_slice;
<<<<<<< HEAD
	unsigned long flags;
	struct rq *rq;
	int retval;
	struct timespec t;
=======
	struct rq_flags rf;
	struct timespec t;
	struct rq *rq;
	int retval;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (pid < 0)
		return -EINVAL;

	retval = -ESRCH;
	rcu_read_lock();
	p = find_process_by_pid(pid);
	if (!p)
		goto out_unlock;

	retval = security_task_getscheduler(p);
	if (retval)
		goto out_unlock;

<<<<<<< HEAD
	rq = task_rq_lock(p, &flags);
	time_slice = p->sched_class->get_rr_interval(rq, p);
	task_rq_unlock(rq, p, &flags);
=======
	rq = task_rq_lock(p, &rf);
	time_slice = 0;
	if (p->sched_class->get_rr_interval)
		time_slice = p->sched_class->get_rr_interval(rq, p);
	task_rq_unlock(rq, p, &rf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rcu_read_unlock();
	jiffies_to_timespec(time_slice, &t);
	retval = copy_to_user(interval, &t, sizeof(t)) ? -EFAULT : 0;
	return retval;

out_unlock:
	rcu_read_unlock();
	return retval;
}

static const char stat_nam[] = TASK_STATE_TO_CHAR_STR;

void sched_show_task(struct task_struct *p)
{
	unsigned long free = 0;
	int ppid;
<<<<<<< HEAD
	unsigned state;

	state = p->state ? __ffs(p->state) + 1 : 0;
	printk(KERN_INFO "%-15.15s %c", p->comm,
		state < sizeof(stat_nam) - 1 ? stat_nam[state] : '?');
#if BITS_PER_LONG == 32
	if (state == TASK_RUNNING)
		printk(KERN_CONT " running  ");
	else
		printk(KERN_CONT " %08lx ", thread_saved_pc(p));
#else
	if (state == TASK_RUNNING)
		printk(KERN_CONT "  running task    ");
	else
		printk(KERN_CONT " %016lx ", thread_saved_pc(p));
#endif
#ifdef CONFIG_DEBUG_STACK_USAGE
	free = stack_not_used(p);
#endif
	rcu_read_lock();
	ppid = task_pid_nr(rcu_dereference(p->real_parent));
=======
	unsigned long state = p->state;

	if (!try_get_task_stack(p))
		return;
	if (state)
		state = __ffs(state) + 1;
	printk(KERN_INFO "%-15.15s %c", p->comm,
		state < sizeof(stat_nam) - 1 ? stat_nam[state] : '?');
	if (state == TASK_RUNNING)
		printk(KERN_CONT "  running task    ");
#ifdef CONFIG_DEBUG_STACK_USAGE
	free = stack_not_used(p);
#endif
	ppid = 0;
	rcu_read_lock();
	if (pid_alive(p))
		ppid = task_pid_nr(rcu_dereference(p->real_parent));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_read_unlock();
	printk(KERN_CONT "%5lu %5d %6d 0x%08lx\n", free,
		task_pid_nr(p), ppid,
		(unsigned long)task_thread_info(p)->flags);

	print_worker_info(KERN_INFO, p);
<<<<<<< HEAD
#ifdef CONFIG_ILOCKDEP
	show_ilockdep_info(p);
#endif
	show_stack(p, NULL);
=======
	show_stack(p, NULL);
	put_task_stack(p);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void show_state_filter(unsigned long state_filter)
{
	struct task_struct *g, *p;

#if BITS_PER_LONG == 32
	printk(KERN_INFO
		"  task                PC stack   pid father\n");
#else
	printk(KERN_INFO
		"  task                        PC stack   pid father\n");
#endif
	rcu_read_lock();
<<<<<<< HEAD
	do_each_thread(g, p) {
		/*
		 * reset the NMI-timeout, listing all files on a slow
		 * console might take a lot of time:
		 */
		touch_nmi_watchdog();
		if (!state_filter || (p->state & state_filter))
			sched_show_task(p);
	} while_each_thread(g, p);

	touch_all_softlockup_watchdogs();
=======
	for_each_process_thread(g, p) {
		/*
		 * reset the NMI-timeout, listing all files on a slow
		 * console might take a lot of time:
		 * Also, reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		if (!state_filter || (p->state & state_filter))
			sched_show_task(p);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_SCHED_DEBUG
	if (!state_filter)
		sysrq_sched_debug_show();
#endif
	rcu_read_unlock();
	/*
	 * Only show locks if all tasks are dumped:
	 */
	if (!state_filter)
		debug_show_all_locks();
}

<<<<<<< HEAD
void __cpuinit init_idle_bootup_task(struct task_struct *idle)
=======
void init_idle_bootup_task(struct task_struct *idle)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	idle->sched_class = &idle_sched_class;
}

/**
 * init_idle - set up an idle thread for a given CPU
 * @idle: task in question
 * @cpu: cpu the idle task belongs to
 *
 * NOTE: this function does not set the idle thread's NEED_RESCHED
 * flag, to make booting more robust.
 */
<<<<<<< HEAD
void __cpuinit init_idle(struct task_struct *idle, int cpu)
=======
void init_idle(struct task_struct *idle, int cpu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long flags;

<<<<<<< HEAD
	raw_spin_lock_irqsave(&rq->lock, flags);

	__sched_fork(idle);
	idle->state = TASK_RUNNING;
	idle->se.exec_start = sched_clock();

	do_set_cpus_allowed(idle, cpumask_of(cpu));
=======
	raw_spin_lock_irqsave(&idle->pi_lock, flags);
	raw_spin_lock(&rq->lock);

	__sched_fork(0, idle);
	idle->state = TASK_RUNNING;
	idle->se.exec_start = sched_clock();

	kasan_unpoison_task_stack(idle);

#ifdef CONFIG_SMP
	/*
	 * Its possible that init_idle() gets called multiple times on a task,
	 * in that case do_set_cpus_allowed() will not do the right thing.
	 *
	 * And since this is boot we can forgo the serialization.
	 */
	set_cpus_allowed_common(idle, cpumask_of(cpu));
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * We're having a chicken and egg problem, even though we are
	 * holding rq->lock, the cpu isn't yet set to this cpu so the
	 * lockdep check in task_group() will fail.
	 *
	 * Similar case to sched_fork(). / Alternatively we could
	 * use task_rq_lock() here and obtain the other rq->lock.
	 *
	 * Silence PROVE_RCU
	 */
	rcu_read_lock();
	__set_task_cpu(idle, cpu);
	rcu_read_unlock();

	rq->curr = rq->idle = idle;
<<<<<<< HEAD
#if defined(CONFIG_SMP)
	idle->on_cpu = 1;
#endif
#ifdef CONFIG_ILOCKDEP
	ilockdep_init(&idle->ilockdep_lock);
#endif
	raw_spin_unlock_irqrestore(&rq->lock, flags);

	/* Set the preempt count _outside_ the spinlocks! */
	task_thread_info(idle)->preempt_count = 0;
=======
	idle->on_rq = TASK_ON_RQ_QUEUED;
#ifdef CONFIG_SMP
	idle->on_cpu = 1;
#endif
	raw_spin_unlock(&rq->lock);
	raw_spin_unlock_irqrestore(&idle->pi_lock, flags);

	/* Set the preempt count _outside_ the spinlocks! */
	init_idle_preempt_count(idle, cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * The idle tasks have their own, simple scheduling class:
	 */
	idle->sched_class = &idle_sched_class;
	ftrace_graph_init_idle_task(idle, cpu);
	vtime_init_idle(idle, cpu);
<<<<<<< HEAD
#if defined(CONFIG_SMP)
=======
#ifdef CONFIG_SMP
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sprintf(idle->comm, "%s/%d", INIT_TASK_COMM, cpu);
#endif
}

<<<<<<< HEAD
#ifdef CONFIG_SMP
void do_set_cpus_allowed(struct task_struct *p, const struct cpumask *new_mask)
{
	if (p->sched_class && p->sched_class->set_cpus_allowed)
		p->sched_class->set_cpus_allowed(p, new_mask);

	cpumask_copy(&p->cpus_allowed, new_mask);
	p->nr_cpus_allowed = cpumask_weight(new_mask);
}

/*
 * This is how migration works:
 *
 * 1) we invoke migration_cpu_stop() on the target CPU using
 *    stop_one_cpu().
 * 2) stopper starts to run (implicitly forcing the migrated thread
 *    off the CPU)
 * 3) it checks whether the migrated task is still in the wrong runqueue.
 * 4) if it's in the wrong runqueue then the migration thread removes
 *    it and puts it into the right queue.
 * 5) stopper completes and stop_one_cpu() returns and the migration
 *    is done.
 */

/*
 * Change a given task's CPU affinity. Migrate the thread to a
 * proper CPU and schedule it away if the CPU it's executing on
 * is removed from the allowed bitmask.
 *
 * NOTE: the caller must have a valid reference to the task, the
 * task must not exit() & deallocate itself prematurely. The
 * call is not atomic; no spinlocks may be held.
 */
int set_cpus_allowed_ptr(struct task_struct *p, const struct cpumask *new_mask)
{
	unsigned long flags;
	struct rq *rq;
	unsigned int dest_cpu;
	int ret = 0;

	rq = task_rq_lock(p, &flags);

	if (cpumask_equal(&p->cpus_allowed, new_mask))
		goto out;

	if (!cpumask_intersects(new_mask, cpu_active_mask)) {
=======
int cpuset_cpumask_can_shrink(const struct cpumask *cur,
			      const struct cpumask *trial)
{
	int ret = 1, trial_cpus;
	struct dl_bw *cur_dl_b;
	unsigned long flags;

	if (!cpumask_weight(cur))
		return ret;

	rcu_read_lock_sched();
	cur_dl_b = dl_bw_of(cpumask_any(cur));
	trial_cpus = cpumask_weight(trial);

	raw_spin_lock_irqsave(&cur_dl_b->lock, flags);
	if (cur_dl_b->bw != -1 &&
	    cur_dl_b->bw * trial_cpus < cur_dl_b->total_bw)
		ret = 0;
	raw_spin_unlock_irqrestore(&cur_dl_b->lock, flags);
	rcu_read_unlock_sched();

	return ret;
}

int task_can_attach(struct task_struct *p,
		    const struct cpumask *cs_cpus_allowed)
{
	int ret = 0;

	/*
	 * Kthreads which disallow setaffinity shouldn't be moved
	 * to a new cpuset; we don't want to change their cpu
	 * affinity and isolating such threads by their set of
	 * allowed nodes is unnecessary.  Thus, cpusets are not
	 * applicable for such threads.  This prevents checking for
	 * success of set_cpus_allowed_ptr() on all attached tasks
	 * before cpus_allowed may be changed.
	 */
	if (p->flags & PF_NO_SETAFFINITY) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	do_set_cpus_allowed(p, new_mask);

	/* Can the task run on the task's current CPU? If so, we're done */
	if (cpumask_test_cpu(task_cpu(p), new_mask))
		goto out;

	dest_cpu = cpumask_any_and(cpu_active_mask, new_mask);
	if (p->on_rq) {
		struct migration_arg arg = { p, dest_cpu };
		/* Need help from migration thread: drop lock and wait. */
		task_rq_unlock(rq, p, &flags);
		stop_one_cpu(cpu_of(rq), migration_cpu_stop, &arg);
		tlb_migrate_finish(p->mm);
		return 0;
	}
out:
	task_rq_unlock(rq, p, &flags);

	return ret;
}
EXPORT_SYMBOL_GPL(set_cpus_allowed_ptr);

/*
 * Move (not current) task off this cpu, onto dest cpu. We're doing
 * this because either it can't run here any more (set_cpus_allowed()
 * away from this CPU, or CPU going down), or because we're
 * attempting to rebalance this task on exec (sched_exec).
 *
 * So we race with normal scheduler movements, but that's OK, as long
 * as the task is no longer on this CPU.
 *
 * Returns non-zero if task was successfully migrated.
 */
static int __migrate_task(struct task_struct *p, int src_cpu, int dest_cpu)
{
	struct rq *rq_dest, *rq_src;
	int ret = 0;

	if (unlikely(!cpu_active(dest_cpu)))
		return ret;

	rq_src = cpu_rq(src_cpu);
	rq_dest = cpu_rq(dest_cpu);

	raw_spin_lock(&p->pi_lock);
	double_rq_lock(rq_src, rq_dest);
	/* Already moved. */
	if (task_cpu(p) != src_cpu)
		goto done;
	/* Affinity changed (again). */
	if (!cpumask_test_cpu(dest_cpu, tsk_cpus_allowed(p)))
		goto fail;

	/*
	 * If we're not on a rq, the next wake-up will ensure we're
	 * placed properly.
	 */
	if (p->on_rq) {
		dequeue_task(rq_src, p, 0);
		set_task_cpu(p, dest_cpu);
		enqueue_task(rq_dest, p, 0);
		check_preempt_curr(rq_dest, p, 0);
	}
done:
	ret = 1;
fail:
	double_rq_unlock(rq_src, rq_dest);
	raw_spin_unlock(&p->pi_lock);
	return ret;
}

/*
 * migration_cpu_stop - this will be executed by a highprio stopper thread
 * and performs thread migration by bumping thread off CPU then
 * 'pushing' onto another runqueue.
 */
static int migration_cpu_stop(void *data)
{
	struct migration_arg *arg = data;

	/*
	 * The original target cpu might have gone down and we might
	 * be on another cpu but it doesn't matter.
	 */
	local_irq_disable();
	__migrate_task(arg->task, raw_smp_processor_id(), arg->dest_cpu);
	local_irq_enable();
	return 0;
}

#ifdef CONFIG_HOTPLUG_CPU

=======
#ifdef CONFIG_SMP
	if (dl_task(p) && !cpumask_intersects(task_rq(p)->rd->span,
					      cs_cpus_allowed)) {
		unsigned int dest_cpu = cpumask_any_and(cpu_active_mask,
							cs_cpus_allowed);
		struct dl_bw *dl_b;
		bool overflow;
		int cpus;
		unsigned long flags;

		rcu_read_lock_sched();
		dl_b = dl_bw_of(dest_cpu);
		raw_spin_lock_irqsave(&dl_b->lock, flags);
		cpus = dl_bw_cpus(dest_cpu);
		overflow = __dl_overflow(dl_b, cpus, 0, p->dl.dl_bw);
		if (overflow)
			ret = -EBUSY;
		else {
			/*
			 * We reserve space for this task in the destination
			 * root_domain, as we can't fail after this point.
			 * We will free resources in the source root_domain
			 * later on (see set_cpus_allowed_dl()).
			 */
			__dl_add(dl_b, p->dl.dl_bw);
		}
		raw_spin_unlock_irqrestore(&dl_b->lock, flags);
		rcu_read_unlock_sched();

	}
#endif
out:
	return ret;
}

#ifdef CONFIG_SMP

static bool sched_smp_initialized __read_mostly;

#ifdef CONFIG_NUMA_BALANCING
/* Migrate current task p to target_cpu */
int migrate_task_to(struct task_struct *p, int target_cpu)
{
	struct migration_arg arg = { p, target_cpu };
	int curr_cpu = task_cpu(p);

	if (curr_cpu == target_cpu)
		return 0;

	if (!cpumask_test_cpu(target_cpu, tsk_cpus_allowed(p)))
		return -EINVAL;

	/* TODO: This is not properly updating schedstats */

	trace_sched_move_numa(p, curr_cpu, target_cpu);
	return stop_one_cpu(curr_cpu, migration_cpu_stop, &arg);
}

/*
 * Requeue a task on a given node and accurately track the number of NUMA
 * tasks on the runqueues
 */
void sched_setnuma(struct task_struct *p, int nid)
{
	bool queued, running;
	struct rq_flags rf;
	struct rq *rq;

	rq = task_rq_lock(p, &rf);
	queued = task_on_rq_queued(p);
	running = task_current(rq, p);

	if (queued)
		dequeue_task(rq, p, DEQUEUE_SAVE);
	if (running)
		put_prev_task(rq, p);

	p->numa_preferred_nid = nid;

	if (queued)
		enqueue_task(rq, p, ENQUEUE_RESTORE);
	if (running)
		set_curr_task(rq, p);
	task_rq_unlock(rq, p, &rf);
}
#endif /* CONFIG_NUMA_BALANCING */

#ifdef CONFIG_HOTPLUG_CPU
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Ensures that the idle task is using init_mm right before its cpu goes
 * offline.
 */
void idle_task_exit(void)
{
	struct mm_struct *mm = current->active_mm;

	BUG_ON(cpu_online(smp_processor_id()));

<<<<<<< HEAD
	if (mm != &init_mm)
		switch_mm(mm, &init_mm, current);
=======
	if (mm != &init_mm) {
		switch_mm(mm, &init_mm, current);
		finish_arch_post_lock_switch();
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mmdrop(mm);
}

/*
 * Since this CPU is going 'away' for a while, fold any nr_active delta
 * we might have. Assumes we're called after migrate_tasks() so that the
<<<<<<< HEAD
 * nr_active count is stable.
=======
 * nr_active count is stable. We need to take the teardown thread which
 * is calling this into account, so we hand in adjust = 1 to the load
 * calculation.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Also see the comment "Global load-average calculations".
 */
static void calc_load_migrate(struct rq *rq)
{
<<<<<<< HEAD
	long delta = calc_load_fold_active(rq);
=======
	long delta = calc_load_fold_active(rq, 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (delta)
		atomic_long_add(delta, &calc_load_tasks);
}

<<<<<<< HEAD
/*
 * Migrate all tasks from the rq, sleeping tasks will be migrated by
 * try_to_wake_up()->select_task_rq().
 *
 * Called with rq->lock held even though we'er in stop_machine() and
 * there's no concurrency possible, we hold the required locks anyway
 * because of lock validation efforts.
 */
static void migrate_tasks(unsigned int dead_cpu)
{
	struct rq *rq = cpu_rq(dead_cpu);
	struct task_struct *next, *stop = rq->stop;
	int dest_cpu;

	/*
	 * Fudge the rq selection such that the below task selection loop
	 * doesn't get stuck on the currently eligible stop task.
	 *
	 * We're currently inside stop_machine() and the rq is either stuck
	 * in the stop_machine_cpu_stop() loop, or we're executing this code,
	 * either way we should never end up calling schedule() until we're
	 * done here.
	 */
	rq->stop = NULL;

	for ( ; ; ) {
		/*
		 * There's this thread running, bail when that's the only
		 * remaining thread.
		 */
		if (rq->nr_running == 1)
			break;

		next = pick_next_task(rq);
		BUG_ON(!next);
		next->sched_class->put_prev_task(rq, next);

		/* Find suitable destination for @next, with force if needed. */
		dest_cpu = select_fallback_rq(dead_cpu, next);
		raw_spin_unlock(&rq->lock);

		__migrate_task(next, dead_cpu, dest_cpu);

		raw_spin_lock(&rq->lock);
	}

	rq->stop = stop;
}

#endif /* CONFIG_HOTPLUG_CPU */

#if defined(CONFIG_SCHED_DEBUG) && defined(CONFIG_SYSCTL)

static struct ctl_table sd_ctl_dir[] = {
	{
		.procname	= "sched_domain",
		.mode		= 0555,
	},
	{}
};

static struct ctl_table sd_ctl_root[] = {
	{
		.procname	= "kernel",
		.mode		= 0555,
		.child		= sd_ctl_dir,
	},
	{}
};

static struct ctl_table *sd_alloc_ctl_entry(int n)
{
	struct ctl_table *entry =
		kcalloc(n, sizeof(struct ctl_table), GFP_KERNEL);

	return entry;
}

static void sd_free_ctl_entry(struct ctl_table **tablep)
{
	struct ctl_table *entry;

	/*
	 * In the intermediate directories, both the child directory and
	 * procname are dynamically allocated and could fail but the mode
	 * will always be set. In the lowest directory the names are
	 * static strings and all have proc handlers.
	 */
	for (entry = *tablep; entry->mode; entry++) {
		if (entry->child)
			sd_free_ctl_entry(&entry->child);
		if (entry->proc_handler == NULL)
			kfree(entry->procname);
	}

	kfree(*tablep);
	*tablep = NULL;
}

static int min_load_idx = 0;
static int max_load_idx = CPU_LOAD_IDX_MAX-1;

static void
set_table_entry(struct ctl_table *entry,
		const char *procname, void *data, int maxlen,
		umode_t mode, proc_handler *proc_handler,
		bool load_idx)
{
	entry->procname = procname;
	entry->data = data;
	entry->maxlen = maxlen;
	entry->mode = mode;
	entry->proc_handler = proc_handler;

	if (load_idx) {
		entry->extra1 = &min_load_idx;
		entry->extra2 = &max_load_idx;
	}
}

static struct ctl_table *
sd_alloc_ctl_domain_table(struct sched_domain *sd)
{
	struct ctl_table *table = sd_alloc_ctl_entry(13);

	if (table == NULL)
		return NULL;

	set_table_entry(&table[0], "min_interval", &sd->min_interval,
		sizeof(long), 0644, proc_doulongvec_minmax, false);
	set_table_entry(&table[1], "max_interval", &sd->max_interval,
		sizeof(long), 0644, proc_doulongvec_minmax, false);
	set_table_entry(&table[2], "busy_idx", &sd->busy_idx,
		sizeof(int), 0644, proc_dointvec_minmax, true);
	set_table_entry(&table[3], "idle_idx", &sd->idle_idx,
		sizeof(int), 0644, proc_dointvec_minmax, true);
	set_table_entry(&table[4], "newidle_idx", &sd->newidle_idx,
		sizeof(int), 0644, proc_dointvec_minmax, true);
	set_table_entry(&table[5], "wake_idx", &sd->wake_idx,
		sizeof(int), 0644, proc_dointvec_minmax, true);
	set_table_entry(&table[6], "forkexec_idx", &sd->forkexec_idx,
		sizeof(int), 0644, proc_dointvec_minmax, true);
	set_table_entry(&table[7], "busy_factor", &sd->busy_factor,
		sizeof(int), 0644, proc_dointvec_minmax, false);
	set_table_entry(&table[8], "imbalance_pct", &sd->imbalance_pct,
		sizeof(int), 0644, proc_dointvec_minmax, false);
	set_table_entry(&table[9], "cache_nice_tries",
		&sd->cache_nice_tries,
		sizeof(int), 0644, proc_dointvec_minmax, false);
	set_table_entry(&table[10], "flags", &sd->flags,
		sizeof(int), 0644, proc_dointvec_minmax, false);
	set_table_entry(&table[11], "name", sd->name,
		CORENAME_MAX_SIZE, 0444, proc_dostring, false);
	/* &table[12] is terminator */

	return table;
}

static ctl_table *sd_alloc_ctl_cpu_table(int cpu)
{
	struct ctl_table *entry, *table;
	struct sched_domain *sd;
	int domain_num = 0, i;
	char buf[32];

	for_each_domain(cpu, sd)
		domain_num++;
	entry = table = sd_alloc_ctl_entry(domain_num + 1);
	if (table == NULL)
		return NULL;

	i = 0;
	for_each_domain(cpu, sd) {
		snprintf(buf, 32, "domain%d", i);
		entry->procname = kstrdup(buf, GFP_KERNEL);
		entry->mode = 0555;
		entry->child = sd_alloc_ctl_domain_table(sd);
		entry++;
		i++;
	}
	return table;
}

static struct ctl_table_header *sd_sysctl_header;
static void register_sched_domain_sysctl(void)
{
	int i, cpu_num = num_possible_cpus();
	struct ctl_table *entry = sd_alloc_ctl_entry(cpu_num + 1);
	char buf[32];

	WARN_ON(sd_ctl_dir[0].child);
	sd_ctl_dir[0].child = entry;

	if (entry == NULL)
		return;

	for_each_possible_cpu(i) {
		snprintf(buf, 32, "cpu%d", i);
		entry->procname = kstrdup(buf, GFP_KERNEL);
		entry->mode = 0555;
		entry->child = sd_alloc_ctl_cpu_table(i);
		entry++;
	}

	WARN_ON(sd_sysctl_header);
	sd_sysctl_header = register_sysctl_table(sd_ctl_root);
}

/* may be called multiple times per register */
static void unregister_sched_domain_sysctl(void)
{
	if (sd_sysctl_header)
		unregister_sysctl_table(sd_sysctl_header);
	sd_sysctl_header = NULL;
	if (sd_ctl_dir[0].child)
		sd_free_ctl_entry(&sd_ctl_dir[0].child);
}
#else
static void register_sched_domain_sysctl(void)
{
}
static void unregister_sched_domain_sysctl(void)
{
}
#endif
=======
static void put_prev_task_fake(struct rq *rq, struct task_struct *prev)
{
}

static const struct sched_class fake_sched_class = {
	.put_prev_task = put_prev_task_fake,
};

static struct task_struct fake_task = {
	/*
	 * Avoid pull_{rt,dl}_task()
	 */
	.prio = MAX_PRIO + 1,
	.sched_class = &fake_sched_class,
};

/*
 * Migrate all tasks from the rq, sleeping tasks will be migrated by
 * try_to_wake_up()->select_task_rq().
 *
 * Called with rq->lock held even though we'er in stop_machine() and
 * there's no concurrency possible, we hold the required locks anyway
 * because of lock validation efforts.
 */
static void migrate_tasks(struct rq *dead_rq)
{
	struct rq *rq = dead_rq;
	struct task_struct *next, *stop = rq->stop;
	struct pin_cookie cookie;
	int dest_cpu;

	/*
	 * Fudge the rq selection such that the below task selection loop
	 * doesn't get stuck on the currently eligible stop task.
	 *
	 * We're currently inside stop_machine() and the rq is either stuck
	 * in the stop_machine_cpu_stop() loop, or we're executing this code,
	 * either way we should never end up calling schedule() until we're
	 * done here.
	 */
	rq->stop = NULL;

	/*
	 * put_prev_task() and pick_next_task() sched
	 * class method both need to have an up-to-date
	 * value of rq->clock[_task]
	 */
	update_rq_clock(rq);

	for (;;) {
		/*
		 * There's this thread running, bail when that's the only
		 * remaining thread.
		 */
		if (rq->nr_running == 1)
			break;

		/*
		 * pick_next_task assumes pinned rq->lock.
		 */
		cookie = lockdep_pin_lock(&rq->lock);
		next = pick_next_task(rq, &fake_task, cookie);
		BUG_ON(!next);
		next->sched_class->put_prev_task(rq, next);

		/*
		 * Rules for changing task_struct::cpus_allowed are holding
		 * both pi_lock and rq->lock, such that holding either
		 * stabilizes the mask.
		 *
		 * Drop rq->lock is not quite as disastrous as it usually is
		 * because !cpu_active at this point, which means load-balance
		 * will not interfere. Also, stop-machine.
		 */
		lockdep_unpin_lock(&rq->lock, cookie);
		raw_spin_unlock(&rq->lock);
		raw_spin_lock(&next->pi_lock);
		raw_spin_lock(&rq->lock);

		/*
		 * Since we're inside stop-machine, _nothing_ should have
		 * changed the task, WARN if weird stuff happened, because in
		 * that case the above rq->lock drop is a fail too.
		 */
		if (WARN_ON(task_rq(next) != rq || !task_on_rq_queued(next))) {
			raw_spin_unlock(&next->pi_lock);
			continue;
		}

		/* Find suitable destination for @next, with force if needed. */
		dest_cpu = select_fallback_rq(dead_rq->cpu, next);

		rq = __migrate_task(rq, next, dest_cpu);
		if (rq != dead_rq) {
			raw_spin_unlock(&rq->lock);
			rq = dead_rq;
			raw_spin_lock(&rq->lock);
		}
		raw_spin_unlock(&next->pi_lock);
	}

	rq->stop = stop;
}
#endif /* CONFIG_HOTPLUG_CPU */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void set_rq_online(struct rq *rq)
{
	if (!rq->online) {
		const struct sched_class *class;

		cpumask_set_cpu(rq->cpu, rq->rd->online);
		rq->online = 1;

		for_each_class(class) {
			if (class->rq_online)
				class->rq_online(rq);
		}
	}
}

static void set_rq_offline(struct rq *rq)
{
	if (rq->online) {
		const struct sched_class *class;

		for_each_class(class) {
			if (class->rq_offline)
				class->rq_offline(rq);
		}

		cpumask_clear_cpu(rq->cpu, rq->rd->online);
		rq->online = 0;
	}
}

<<<<<<< HEAD
/*
 * migration_call - callback that gets triggered when a CPU is added.
 * Here we can start up the necessary migration thread for the new CPU.
 */
static int __cpuinit
migration_call(struct notifier_block *nfb, unsigned long action, void *hcpu)
{
	int cpu = (long)hcpu;
	unsigned long flags;
	struct rq *rq = cpu_rq(cpu);

	switch (action & ~CPU_TASKS_FROZEN) {

	case CPU_UP_PREPARE:
		rq->calc_load_update = calc_load_update;
		account_reset_rq(rq);
		break;

	case CPU_ONLINE:
		/* Update our root-domain */
		raw_spin_lock_irqsave(&rq->lock, flags);
		if (rq->rd) {
			BUG_ON(!cpumask_test_cpu(cpu, rq->rd->span));

			set_rq_online(rq);
		}
		raw_spin_unlock_irqrestore(&rq->lock, flags);
		break;

#ifdef CONFIG_HOTPLUG_CPU
	case CPU_DYING:
		sched_ttwu_pending();
		/* Update our root-domain */
		raw_spin_lock_irqsave(&rq->lock, flags);
		if (rq->rd) {
			BUG_ON(!cpumask_test_cpu(cpu, rq->rd->span));
			set_rq_offline(rq);
		}
		migrate_tasks(cpu);
		BUG_ON(rq->nr_running != 1); /* the migration thread */
		raw_spin_unlock_irqrestore(&rq->lock, flags);
		break;

	case CPU_DEAD:
		calc_load_migrate(rq);
		break;
#endif
	}

	update_max_interval();

	return NOTIFY_OK;
}

/*
 * Register at high priority so that task migration (migrate_all_tasks)
 * happens before everything else.  This has to be lower priority than
 * the notifier in the perf_event subsystem, though.
 */
static struct notifier_block __cpuinitdata migration_notifier = {
	.notifier_call = migration_call,
	.priority = CPU_PRI_MIGRATION,
};

static int __cpuinit sched_cpu_active(struct notifier_block *nfb,
				      unsigned long action, void *hcpu)
{
	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_DOWN_FAILED:
		set_cpu_active((long)hcpu, true);
		return NOTIFY_OK;
	default:
		return NOTIFY_DONE;
	}
}

static int __cpuinit sched_cpu_inactive(struct notifier_block *nfb,
					unsigned long action, void *hcpu)
{
	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_DOWN_PREPARE:
		set_cpu_active((long)hcpu, false);
		return NOTIFY_OK;
	default:
		return NOTIFY_DONE;
	}
}

static int __init migration_init(void)
{
	void *cpu = (void *)(long)smp_processor_id();
	int err;

	/* Initialize migration for the boot CPU */
	err = migration_call(&migration_notifier, CPU_UP_PREPARE, cpu);
	BUG_ON(err == NOTIFY_BAD);
	migration_call(&migration_notifier, CPU_ONLINE, cpu);
	register_cpu_notifier(&migration_notifier);

	/* Register cpu active notifiers */
	cpu_notifier(sched_cpu_active, CPU_PRI_SCHED_ACTIVE);
	cpu_notifier(sched_cpu_inactive, CPU_PRI_SCHED_INACTIVE);

	return 0;
}
early_initcall(migration_init);
#endif

#ifdef CONFIG_SMP
=======
static void set_cpu_rq_start_time(unsigned int cpu)
{
	struct rq *rq = cpu_rq(cpu);

	rq->age_stamp = sched_clock_cpu(cpu);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static cpumask_var_t sched_domains_tmpmask; /* sched_domains_mutex */

#ifdef CONFIG_SCHED_DEBUG

static __read_mostly int sched_debug_enabled;

static int __init sched_debug_setup(char *str)
{
	sched_debug_enabled = 1;

	return 0;
}
early_param("sched_debug", sched_debug_setup);

static inline bool sched_debug(void)
{
	return sched_debug_enabled;
}

static int sched_domain_debug_one(struct sched_domain *sd, int cpu, int level,
				  struct cpumask *groupmask)
{
	struct sched_group *group = sd->groups;
<<<<<<< HEAD
	char str[256];

	cpulist_scnprintf(str, sizeof(str), sched_domain_span(sd));
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain %d: ", level, "", level);

	if (!(sd->flags & SD_LOAD_BALANCE)) {
		printk("does not load-balance\n");
<<<<<<< HEAD
		if (sd->parent)
			printk(KERN_ERR "ERROR: !SD_LOAD_BALANCE domain"
					" has parent");
		return -1;
	}

	printk(KERN_CONT "span %s level %s\n", str, sd->name);
=======
		return -1;
	}

	printk(KERN_CONT "span %*pbl level %s\n",
	       cpumask_pr_args(sched_domain_span(sd)), sd->name);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!cpumask_test_cpu(cpu, sched_domain_span(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain "
				"CPU%d\n", cpu);
	}
	if (!cpumask_test_cpu(cpu, sched_group_cpus(group))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain"
				" CPU%d\n", cpu);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	do {
		if (!group) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			break;
		}

<<<<<<< HEAD
		/*
		 * Even though we initialize ->power to something semi-sane,
		 * we leave power_orig unset. This allows us to detect if
		 * domain iteration is still funny without causing /0 traps.
		 */
		if (!group->sgp->power_orig) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: domain->cpu_power not "
					"set\n");
			break;
		}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!cpumask_weight(sched_group_cpus(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			break;
		}

		if (!(sd->flags & SD_OVERLAP) &&
		    cpumask_intersects(groupmask, sched_group_cpus(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			break;
		}

		cpumask_or(groupmask, groupmask, sched_group_cpus(group));

<<<<<<< HEAD
		cpulist_scnprintf(str, sizeof(str), sched_group_cpus(group));

		printk(KERN_CONT " %s", str);
		if (group->sgp->power != SCHED_POWER_SCALE) {
			printk(KERN_CONT " (cpu_power = %d)",
				group->sgp->power);
=======
		printk(KERN_CONT " %*pbl",
		       cpumask_pr_args(sched_group_cpus(group)));
		if (group->sgc->capacity != SCHED_CAPACITY_SCALE) {
			printk(KERN_CONT " (cpu_capacity = %lu)",
				group->sgc->capacity);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		group = group->next;
	} while (group != sd->groups);
	printk(KERN_CONT "\n");

	if (!cpumask_equal(sched_domain_span(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	if (sd->parent &&
	    !cpumask_subset(groupmask, sched_domain_span(sd->parent)))
		printk(KERN_ERR "ERROR: parent span is not a superset "
			"of domain->span\n");
	return 0;
}

static void sched_domain_debug(struct sched_domain *sd, int cpu)
{
	int level = 0;

	if (!sched_debug_enabled)
		return;

	if (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		return;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain:\n", cpu);

	for (;;) {
		if (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			break;
		level++;
		sd = sd->parent;
		if (!sd)
			break;
	}
}
#else /* !CONFIG_SCHED_DEBUG */
<<<<<<< HEAD
=======

# define sched_debug_enabled 0
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
# define sched_domain_debug(sd, cpu) do { } while (0)
static inline bool sched_debug(void)
{
	return false;
}
#endif /* CONFIG_SCHED_DEBUG */

static int sd_degenerate(struct sched_domain *sd)
{
<<<<<<< HEAD
	if (cpumask_weight(sched_domain_span(sd)) == 1)
		return 1;
=======
	if (cpumask_weight(sched_domain_span(sd)) == 1) {
		if (sd->groups->sge)
			sd->flags &= ~SD_LOAD_BALANCE;
		else
			return 1;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Following flags need at least 2 groups */
	if (sd->flags & (SD_LOAD_BALANCE |
			 SD_BALANCE_NEWIDLE |
			 SD_BALANCE_FORK |
			 SD_BALANCE_EXEC |
<<<<<<< HEAD
			 SD_SHARE_CPUPOWER |
			 SD_SHARE_PKG_RESOURCES)) {
=======
			 SD_SHARE_CPUCAPACITY |
			 SD_ASYM_CPUCAPACITY |
			 SD_SHARE_PKG_RESOURCES |
			 SD_SHARE_POWERDOMAIN |
			 SD_SHARE_CAP_STATES)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (sd->groups != sd->groups->next)
			return 0;
	}

	/* Following flags don't use groups */
	if (sd->flags & (SD_WAKE_AFFINE))
		return 0;

	return 1;
}

static int
sd_parent_degenerate(struct sched_domain *sd, struct sched_domain *parent)
{
	unsigned long cflags = sd->flags, pflags = parent->flags;

	if (sd_degenerate(parent))
		return 1;

	if (!cpumask_equal(sched_domain_span(sd), sched_domain_span(parent)))
		return 0;

	/* Flags needing groups don't count if only 1 group in parent */
	if (parent->groups == parent->groups->next) {
		pflags &= ~(SD_LOAD_BALANCE |
				SD_BALANCE_NEWIDLE |
				SD_BALANCE_FORK |
				SD_BALANCE_EXEC |
<<<<<<< HEAD
				SD_SHARE_CPUPOWER |
				SD_SHARE_PKG_RESOURCES);
=======
				SD_ASYM_CPUCAPACITY |
				SD_SHARE_CPUCAPACITY |
				SD_SHARE_PKG_RESOURCES |
				SD_PREFER_SIBLING |
				SD_SHARE_POWERDOMAIN |
				SD_SHARE_CAP_STATES);
		if (parent->groups->sge) {
			parent->flags &= ~SD_LOAD_BALANCE;
			return 0;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (nr_node_ids == 1)
			pflags &= ~SD_SERIALIZE;
	}
	if (~cflags & pflags)
		return 0;

	return 1;
}

static void free_rootdomain(struct rcu_head *rcu)
{
	struct root_domain *rd = container_of(rcu, struct root_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
<<<<<<< HEAD
=======
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_mask);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	free_cpumask_var(rd->rto_mask);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->span);
	kfree(rd);
}

static void rq_attach_root(struct rq *rq, struct root_domain *rd)
{
	struct root_domain *old_rd = NULL;
	unsigned long flags;

	raw_spin_lock_irqsave(&rq->lock, flags);

	if (rq->rd) {
		old_rd = rq->rd;

		if (cpumask_test_cpu(rq->cpu, old_rd->online))
			set_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->span);

		/*
<<<<<<< HEAD
		 * If we dont want to free the old_rt yet then
=======
		 * If we dont want to free the old_rd yet then
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		if (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = NULL;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_set_cpu(rq->cpu, rd->span);
	if (cpumask_test_cpu(rq->cpu, cpu_active_mask))
		set_rq_online(rq);

	raw_spin_unlock_irqrestore(&rq->lock, flags);

	if (old_rd)
		call_rcu_sched(&old_rd->rcu, free_rootdomain);
}

<<<<<<< HEAD
=======
void sched_get_rd(struct root_domain *rd)
{
	atomic_inc(&rd->refcount);
}

void sched_put_rd(struct root_domain *rd)
{
	if (!atomic_dec_and_test(&rd->refcount))
		return;

	call_rcu_sched(&rd->rcu, free_rootdomain);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int init_rootdomain(struct root_domain *rd)
{
	memset(rd, 0, sizeof(*rd));

<<<<<<< HEAD
	if (!alloc_cpumask_var(&rd->span, GFP_KERNEL))
		goto out;
	if (!alloc_cpumask_var(&rd->online, GFP_KERNEL))
		goto free_span;
	if (!alloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		goto free_online;

	if (cpupri_init(&rd->cpupri) != 0)
		goto free_rto_mask;
=======
	if (!zalloc_cpumask_var(&rd->span, GFP_KERNEL))
		goto out;
	if (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		goto free_span;
	if (!zalloc_cpumask_var(&rd->dlo_mask, GFP_KERNEL))
		goto free_online;
	if (!zalloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		goto free_dlo_mask;

#ifdef HAVE_RT_PUSH_IPI
	rd->rto_cpu = -1;
	raw_spin_lock_init(&rd->rto_lock);
	init_irq_work(&rd->rto_push_work, rto_push_irq_work_func);
#endif

	init_dl_bw(&rd->dl_bw);
	if (cpudl_init(&rd->cpudl) != 0)
		goto free_dlo_mask;

	if (cpupri_init(&rd->cpupri) != 0)
		goto free_rto_mask;

	init_max_cpu_capacity(&rd->max_cpu_capacity);

	rd->max_cap_orig_cpu = rd->min_cap_orig_cpu = -1;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;

free_rto_mask:
	free_cpumask_var(rd->rto_mask);
<<<<<<< HEAD
=======
free_dlo_mask:
	free_cpumask_var(rd->dlo_mask);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
free_online:
	free_cpumask_var(rd->online);
free_span:
	free_cpumask_var(rd->span);
out:
	return -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all cpus as
 * members (mimicking the global state we have today).
 */
struct root_domain def_root_domain;

static void init_defrootdomain(void)
{
	init_rootdomain(&def_root_domain);

	atomic_set(&def_root_domain.refcount, 1);
}

static struct root_domain *alloc_rootdomain(void)
{
	struct root_domain *rd;

	rd = kmalloc(sizeof(*rd), GFP_KERNEL);
	if (!rd)
		return NULL;

	if (init_rootdomain(rd) != 0) {
		kfree(rd);
		return NULL;
	}

	return rd;
}

<<<<<<< HEAD
static void free_sched_groups(struct sched_group *sg, int free_sgp)
=======
static void free_sched_groups(struct sched_group *sg, int free_sgc)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct sched_group *tmp, *first;

	if (!sg)
		return;

	first = sg;
	do {
		tmp = sg->next;

<<<<<<< HEAD
		if (free_sgp && atomic_dec_and_test(&sg->sgp->ref))
			kfree(sg->sgp);
=======
		if (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		kfree(sg);
		sg = tmp;
	} while (sg != first);
}

<<<<<<< HEAD
static void free_sched_domain(struct rcu_head *rcu)
{
	struct sched_domain *sd = container_of(rcu, struct sched_domain, rcu);

=======
static void destroy_sched_domain(struct sched_domain *sd)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * If its an overlapping domain it has private groups, iterate and
	 * nuke them all.
	 */
	if (sd->flags & SD_OVERLAP) {
		free_sched_groups(sd->groups, 1);
	} else if (atomic_dec_and_test(&sd->groups->ref)) {
<<<<<<< HEAD
		kfree(sd->groups->sgp);
		kfree(sd->groups);
	}
	kfree(sd);
}

static void destroy_sched_domain(struct sched_domain *sd, int cpu)
{
	call_rcu(&sd->rcu, free_sched_domain);
}

static void destroy_sched_domains(struct sched_domain *sd, int cpu)
{
	for (; sd; sd = sd->parent)
		destroy_sched_domain(sd, cpu);
=======
		kfree(sd->groups->sgc);
		kfree(sd->groups);
	}
	if (sd->shared && atomic_dec_and_test(&sd->shared->ref))
		kfree(sd->shared);
	kfree(sd);
}

static void destroy_sched_domains_rcu(struct rcu_head *rcu)
{
	struct sched_domain *sd = container_of(rcu, struct sched_domain, rcu);

	while (sd) {
		struct sched_domain *parent = sd->parent;
		destroy_sched_domain(sd);
		sd = parent;
	}
}

static void destroy_sched_domains(struct sched_domain *sd)
{
	if (sd)
		call_rcu(&sd->rcu, destroy_sched_domains_rcu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Keep a special pointer to the highest sched_domain that has
 * SD_SHARE_PKG_RESOURCE set (Last Level Cache Domain) for this
 * allows us to avoid some pointer chasing select_idle_sibling().
 *
 * Also keep a unique ID per domain (we use the first cpu number in
 * the cpumask of the domain), this allows us to quickly tell if
 * two cpus are in the same cache domain, see cpus_share_cache().
 */
DEFINE_PER_CPU(struct sched_domain *, sd_llc);
<<<<<<< HEAD
DEFINE_PER_CPU(int, sd_llc_id);

static void update_top_cache_domain(int cpu)
{
	struct sched_domain *sd;
	int id = cpu;

	sd = highest_flag_domain(cpu, SD_SHARE_PKG_RESOURCES);
	if (sd)
		id = cpumask_first(sched_domain_span(sd));

	rcu_assign_pointer(per_cpu(sd_llc, cpu), sd);
	per_cpu(sd_llc_id, cpu) = id;
=======
DEFINE_PER_CPU(int, sd_llc_size);
DEFINE_PER_CPU(int, sd_llc_id);
DEFINE_PER_CPU(struct sched_domain_shared *, sd_llc_shared);
DEFINE_PER_CPU(struct sched_domain *, sd_numa);
DEFINE_PER_CPU(struct sched_domain *, sd_asym);
DEFINE_PER_CPU(struct sched_domain *, sd_ea);
DEFINE_PER_CPU(struct sched_domain *, sd_scs);

static void update_top_cache_domain(int cpu)
{
	struct sched_domain_shared *sds = NULL;
	struct sched_domain *sd;
	struct sched_domain *ea_sd = NULL;
	int id = cpu;
	int size = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_PKG_RESOURCES);
	if (sd) {
		id = cpumask_first(sched_domain_span(sd));
		size = cpumask_weight(sched_domain_span(sd));
		sds = sd->shared;
	}

	rcu_assign_pointer(per_cpu(sd_llc, cpu), sd);
	per_cpu(sd_llc_size, cpu) = size;
	per_cpu(sd_llc_id, cpu) = id;
	rcu_assign_pointer(per_cpu(sd_llc_shared, cpu), sds);

	sd = lowest_flag_domain(cpu, SD_NUMA);
	rcu_assign_pointer(per_cpu(sd_numa, cpu), sd);

	sd = highest_flag_domain(cpu, SD_ASYM_PACKING);
	rcu_assign_pointer(per_cpu(sd_asym, cpu), sd);

	for_each_domain(cpu, sd) {
		if (sd->groups->sge)
			ea_sd = sd;
		else
			break;
	}
	rcu_assign_pointer(per_cpu(sd_ea, cpu), ea_sd);

	sd = highest_flag_domain(cpu, SD_SHARE_CAP_STATES);
	rcu_assign_pointer(per_cpu(sd_scs, cpu), sd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
static void
cpu_attach_domain(struct sched_domain *sd, struct root_domain *rd, int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	struct sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	for (tmp = sd; tmp; ) {
		struct sched_domain *parent = tmp->parent;
		if (!parent)
			break;

		if (sd_parent_degenerate(tmp, parent)) {
			tmp->parent = parent->parent;
			if (parent->parent)
				parent->parent->child = tmp;
<<<<<<< HEAD
			destroy_sched_domain(parent, cpu);
=======
			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			if (parent->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(parent);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		} else
			tmp = tmp->parent;
	}

	if (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->parent;
<<<<<<< HEAD
		destroy_sched_domain(tmp, cpu);
=======
		destroy_sched_domain(tmp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (sd)
			sd->child = NULL;
	}

	sched_domain_debug(sd, cpu);

	rq_attach_root(rq, rd);
	tmp = rq->sd;
	rcu_assign_pointer(rq->sd, sd);
<<<<<<< HEAD
	destroy_sched_domains(tmp, cpu);
=======
	destroy_sched_domains(tmp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	update_top_cache_domain(cpu);
}

<<<<<<< HEAD
/* cpus with isolated domains */
static cpumask_var_t cpu_isolated_map;

/* Setup the mask of cpus configured for isolated domains */
static int __init isolated_cpu_setup(char *str)
{
	alloc_bootmem_cpumask_var(&cpu_isolated_map);
	cpulist_parse(str, cpu_isolated_map);
	return 1;
}

__setup("isolcpus=", isolated_cpu_setup);

static const struct cpumask *cpu_cpu_mask(int cpu)
{
	return cpumask_of_node(cpu_to_node(cpu));
}

struct sd_data {
	struct sched_domain **__percpu sd;
	struct sched_group **__percpu sg;
	struct sched_group_power **__percpu sgp;
};

=======
/* Setup the mask of cpus configured for isolated domains */
static int __init isolated_cpu_setup(char *str)
{
	int ret;

	alloc_bootmem_cpumask_var(&cpu_isolated_map);
	ret = cpulist_parse(str, cpu_isolated_map);
	if (ret) {
		pr_err("sched: Error, all isolcpus= values must be between 0 and %d\n", nr_cpu_ids);
		return 0;
	}
	return 1;
}
__setup("isolcpus=", isolated_cpu_setup);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct s_data {
	struct sched_domain ** __percpu sd;
	struct root_domain	*rd;
};

enum s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_storage,
	sa_none,
};

<<<<<<< HEAD
struct sched_domain_topology_level;

typedef struct sched_domain *(*sched_domain_init_f)(struct sched_domain_topology_level *tl, int cpu);
typedef const struct cpumask *(*sched_domain_mask_f)(int cpu);

#define SDTL_OVERLAP	0x01

struct sched_domain_topology_level {
	sched_domain_init_f init;
	sched_domain_mask_f mask;
	int		    flags;
	int		    numa_level;
	struct sd_data      data;
};

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Build an iteration mask that can exclude certain CPUs from the upwards
 * domain traversal.
 *
<<<<<<< HEAD
=======
 * Only CPUs that can arrive at this group should be considered to continue
 * balancing.
 *
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Asymmetric node setups can result in situations where the domain tree is of
 * unequal depth, make sure to skip domains that already cover the entire
 * range.
 *
 * In that case build_sched_domains() will have terminated the iteration early
 * and our sibling sd spans will be empty. Domains should always include the
 * cpu they're built on, so check that.
 *
 */
static void build_group_mask(struct sched_domain *sd, struct sched_group *sg)
{
<<<<<<< HEAD
	const struct cpumask *span = sched_domain_span(sd);
=======
	const struct cpumask *sg_span = sched_group_cpus(sg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct sd_data *sdd = sd->private;
	struct sched_domain *sibling;
	int i;

<<<<<<< HEAD
	for_each_cpu(i, span) {
		sibling = *per_cpu_ptr(sdd->sd, i);
		if (!cpumask_test_cpu(i, sched_domain_span(sibling)))
=======
	for_each_cpu(i, sg_span) {
		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		if (!sibling->child)
			continue;

		/* If we would not end up here, we can't continue from here */
		if (!cpumask_equal(sg_span, sched_domain_span(sibling->child)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;

		cpumask_set_cpu(i, sched_group_mask(sg));
	}
<<<<<<< HEAD
=======

	/* We must not have empty masks here */
	WARN_ON_ONCE(cpumask_empty(sched_group_mask(sg)));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Return the canonical balance cpu for this group, this is the first cpu
 * of this group that's also in the iteration mask.
 */
int group_balance_cpu(struct sched_group *sg)
{
	return cpumask_first_and(sched_group_cpus(sg), sched_group_mask(sg));
}

static int
build_overlap_sched_groups(struct sched_domain *sd, int cpu)
{
	struct sched_group *first = NULL, *last = NULL, *groups = NULL, *sg;
	const struct cpumask *span = sched_domain_span(sd);
	struct cpumask *covered = sched_domains_tmpmask;
	struct sd_data *sdd = sd->private;
<<<<<<< HEAD
	struct sched_domain *child;
=======
	struct sched_domain *sibling;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int i;

	cpumask_clear(covered);

<<<<<<< HEAD
	for_each_cpu(i, span) {
=======
	for_each_cpu_wrap(i, span, cpu) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		struct cpumask *sg_span;

		if (cpumask_test_cpu(i, covered))
			continue;

<<<<<<< HEAD
		child = *per_cpu_ptr(sdd->sd, i);

		/* See the comment near build_group_mask(). */
		if (!cpumask_test_cpu(i, sched_domain_span(child)))
=======
		sibling = *per_cpu_ptr(sdd->sd, i);

		/* See the comment near build_group_mask(). */
		if (!cpumask_test_cpu(i, sched_domain_span(sibling)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;

		sg = kzalloc_node(sizeof(struct sched_group) + cpumask_size(),
				GFP_KERNEL, cpu_to_node(cpu));

		if (!sg)
			goto fail;

		sg_span = sched_group_cpus(sg);
<<<<<<< HEAD
		if (child->child) {
			child = child->child;
			cpumask_copy(sg_span, sched_domain_span(child));
		} else
=======
		if (sibling->child)
			cpumask_copy(sg_span, sched_domain_span(sibling->child));
		else
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			cpumask_set_cpu(i, sg_span);

		cpumask_or(covered, covered, sg_span);

<<<<<<< HEAD
		sg->sgp = *per_cpu_ptr(sdd->sgp, i);
		if (atomic_inc_return(&sg->sgp->ref) == 1)
			build_group_mask(sd, sg);

		/*
		 * Initialize sgp->power such that even if we mess up the
		 * domains and no possible iteration will get us here, we won't
		 * die on a /0 trap.
		 */
		sg->sgp->power = SCHED_POWER_SCALE * cpumask_weight(sg_span);
=======
		sg->sgc = *per_cpu_ptr(sdd->sgc, i);
		if (atomic_inc_return(&sg->sgc->ref) == 1)
			build_group_mask(sd, sg);

		/*
		 * Initialize sgc->capacity such that even if we mess up the
		 * domains and no possible iteration will get us here, we won't
		 * die on a /0 trap.
		 */
		sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sg_span);
		sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;
		sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/*
		 * Make sure the first group of this domain contains the
		 * canonical balance cpu. Otherwise the sched_domain iteration
		 * breaks. See update_sg_lb_stats().
		 */
		if ((!groups && cpumask_test_cpu(cpu, sg_span)) ||
		    group_balance_cpu(sg) == cpu)
			groups = sg;

		if (!first)
			first = sg;
		if (last)
			last->next = sg;
		last = sg;
		last->next = first;
	}
	sd->groups = groups;

	return 0;

fail:
	free_sched_groups(first, 0);

	return -ENOMEM;
}

static int get_group(int cpu, struct sd_data *sdd, struct sched_group **sg)
{
	struct sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	struct sched_domain *child = sd->child;

	if (child)
		cpu = cpumask_first(sched_domain_span(child));

	if (sg) {
		*sg = *per_cpu_ptr(sdd->sg, cpu);
<<<<<<< HEAD
		(*sg)->sgp = *per_cpu_ptr(sdd->sgp, cpu);
		atomic_set(&(*sg)->sgp->ref, 1); /* for claim_allocations */
=======
		(*sg)->sgc = *per_cpu_ptr(sdd->sgc, cpu);
		atomic_set(&(*sg)->sgc->ref, 1); /* for claim_allocations */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return cpu;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, and will set each group's ->cpumask correctly,
<<<<<<< HEAD
 * and ->cpu_power to 0.
=======
 * and ->cpu_capacity to 0.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Assumes the sched_domain tree is fully constructed
 */
static int
build_sched_groups(struct sched_domain *sd, int cpu)
{
	struct sched_group *first = NULL, *last = NULL;
	struct sd_data *sdd = sd->private;
	const struct cpumask *span = sched_domain_span(sd);
	struct cpumask *covered;
	int i;

	get_group(cpu, sdd, &sd->groups);
	atomic_inc(&sd->groups->ref);

<<<<<<< HEAD
	if (cpu != cpumask_first(sched_domain_span(sd)))
=======
	if (cpu != cpumask_first(span))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 0;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu(i, span) {
		struct sched_group *sg;
<<<<<<< HEAD
		int group = get_group(i, sdd, &sg);
		int j;
=======
		int group, j;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (cpumask_test_cpu(i, covered))
			continue;

<<<<<<< HEAD
		cpumask_clear(sched_group_cpus(sg));
		sg->sgp->power = 0;
=======
		group = get_group(i, sdd, &sg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		cpumask_setall(sched_group_mask(sg));

		for_each_cpu(j, span) {
			if (get_group(j, sdd, NULL) != group)
				continue;

			cpumask_set_cpu(j, covered);
			cpumask_set_cpu(j, sched_group_cpus(sg));
		}

		if (!first)
			first = sg;
		if (last)
			last->next = sg;
		last = sg;
	}
	last->next = first;

	return 0;
}

/*
<<<<<<< HEAD
 * Initialize sched groups cpu_power.
 *
 * cpu_power indicates the capacity of sched group, which is used while
 * distributing the load between different sched groups in a sched domain.
 * Typically cpu_power for all the groups in a sched domain will be same unless
 * there are asymmetries in the topology. If there are asymmetries, group
 * having more cpu_power will pickup more load compared to the group having
 * less cpu_power.
 */
static void init_sched_groups_power(int cpu, struct sched_domain *sd)
{
	struct sched_group *sg = sd->groups;

	WARN_ON(!sd || !sg);
=======
 * Initialize sched groups cpu_capacity.
 *
 * cpu_capacity indicates the capacity of sched group, which is used while
 * distributing the load between different sched groups in a sched domain.
 * Typically cpu_capacity for all the groups in a sched domain will be same
 * unless there are asymmetries in the topology. If there are asymmetries,
 * group having more cpu_capacity will pickup more load compared to the
 * group having less cpu_capacity.
 */
static void init_sched_groups_capacity(int cpu, struct sched_domain *sd)
{
	struct sched_group *sg = sd->groups;

	WARN_ON(!sg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	do {
		sg->group_weight = cpumask_weight(sched_group_cpus(sg));
		sg = sg->next;
	} while (sg != sd->groups);

	if (cpu != group_balance_cpu(sg))
		return;

<<<<<<< HEAD
	update_group_power(sd, cpu);
	atomic_set(&sg->sgp->nr_busy_cpus, sg->group_weight);
}

int __weak arch_sd_sibling_asym_packing(void)
{
       return 0*SD_ASYM_PACKING;
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

#ifdef CONFIG_SCHED_DEBUG
# define SD_INIT_NAME(sd, type)		sd->name = #type
#else
# define SD_INIT_NAME(sd, type)		do { } while (0)
#endif

#define SD_INIT_FUNC(type)						\
static noinline struct sched_domain *					\
sd_init_##type(struct sched_domain_topology_level *tl, int cpu) 	\
{									\
	struct sched_domain *sd = *per_cpu_ptr(tl->data.sd, cpu);	\
	*sd = SD_##type##_INIT;						\
	SD_INIT_NAME(sd, type);						\
	sd->private = &tl->data;					\
	return sd;							\
}

SD_INIT_FUNC(CPU)
#ifdef CONFIG_SCHED_SMT
 SD_INIT_FUNC(SIBLING)
#endif
#ifdef CONFIG_SCHED_MC
 SD_INIT_FUNC(MC)
#endif
#ifdef CONFIG_SCHED_BOOK
 SD_INIT_FUNC(BOOK)
#endif
=======
	update_group_capacity(sd, cpu);
}

/*
 * Check that the per-cpu provided sd energy data is consistent for all cpus
 * within the mask.
 */
static inline void check_sched_energy_data(int cpu, sched_domain_energy_f fn,
					   const struct cpumask *cpumask)
{
	const struct sched_group_energy * const sge = fn(cpu);
	struct cpumask mask;
	int i;

	if (cpumask_weight(cpumask) <= 1)
		return;

	cpumask_xor(&mask, cpumask, get_cpu_mask(cpu));

	for_each_cpu(i, &mask) {
		const struct sched_group_energy * const e = fn(i);
		int y;

		BUG_ON(e->nr_idle_states != sge->nr_idle_states);

		for (y = 0; y < (e->nr_idle_states); y++) {
			BUG_ON(e->idle_states[y].power !=
					sge->idle_states[y].power);
		}

		BUG_ON(e->nr_cap_states != sge->nr_cap_states);

		for (y = 0; y < (e->nr_cap_states); y++) {
			BUG_ON(e->cap_states[y].cap != sge->cap_states[y].cap);
			BUG_ON(e->cap_states[y].power !=
					sge->cap_states[y].power);
		}
	}
}

static void init_sched_energy(int cpu, struct sched_domain *sd,
			      sched_domain_energy_f fn)
{
	if (!(fn && fn(cpu)))
		return;

	if (cpu != group_balance_cpu(sd->groups))
		return;

	if (sd->child && !sd->child->groups->sge) {
		pr_err("BUG: EAS setup broken for CPU%d\n", cpu);
#ifdef CONFIG_SCHED_DEBUG
		pr_err("     energy data on %s but not on %s domain\n",
			sd->name, sd->child->name);
#endif
		return;
	}

	check_sched_energy_data(cpu, fn, sched_group_cpus(sd->groups));

	sd->groups->sge = fn(cpu);
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int default_relax_domain_level = -1;
int sched_domain_level_max;

static int __init setup_relax_domain_level(char *str)
{
	if (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	return 1;
}
__setup("relax_domain_level=", setup_relax_domain_level);

static void set_domain_attribute(struct sched_domain *sd,
				 struct sched_domain_attr *attr)
{
	int request;

	if (!attr || attr->relax_domain_level < 0) {
		if (default_relax_domain_level < 0)
			return;
		else
			request = default_relax_domain_level;
	} else
		request = attr->relax_domain_level;
	if (request < sd->level) {
		/* turn off idle balance on this domain */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	} else {
		/* turn on idle balance on this domain */
		sd->flags |= (SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

static void __sdt_free(const struct cpumask *cpu_map);
static int __sdt_alloc(const struct cpumask *cpu_map);

static void __free_domain_allocs(struct s_data *d, enum s_alloc what,
				 const struct cpumask *cpu_map)
{
	switch (what) {
	case sa_rootdomain:
		if (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu); /* fall through */
	case sa_sd:
		free_percpu(d->sd); /* fall through */
	case sa_sd_storage:
		__sdt_free(cpu_map); /* fall through */
	case sa_none:
		break;
	}
}

static enum s_alloc __visit_domain_allocation_hell(struct s_data *d,
						   const struct cpumask *cpu_map)
{
	memset(d, 0, sizeof(*d));

	if (__sdt_alloc(cpu_map))
		return sa_sd_storage;
	d->sd = alloc_percpu(struct sched_domain *);
	if (!d->sd)
		return sa_sd_storage;
	d->rd = alloc_rootdomain();
	if (!d->rd)
		return sa_sd;
	return sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
static void claim_allocations(int cpu, struct sched_domain *sd)
{
	struct sd_data *sdd = sd->private;

	WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
	*per_cpu_ptr(sdd->sd, cpu) = NULL;

<<<<<<< HEAD
	if (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
		*per_cpu_ptr(sdd->sg, cpu) = NULL;

	if (atomic_read(&(*per_cpu_ptr(sdd->sgp, cpu))->ref))
		*per_cpu_ptr(sdd->sgp, cpu) = NULL;
}

#ifdef CONFIG_SCHED_SMT
static const struct cpumask *cpu_smt_mask(int cpu)
{
	return topology_thread_cpumask(cpu);
}
#endif

/*
 * Topology list, bottom-up.
 */
static struct sched_domain_topology_level default_topology[] = {
#ifdef CONFIG_SCHED_SMT
	{ sd_init_SIBLING, cpu_smt_mask, },
#endif
#ifdef CONFIG_SCHED_MC
	{ sd_init_MC, cpu_coregroup_mask, },
#endif
#ifdef CONFIG_SCHED_BOOK
	{ sd_init_BOOK, cpu_book_mask, },
#endif
	{ sd_init_CPU, cpu_cpu_mask, },
	{ NULL, },
};

static struct sched_domain_topology_level *sched_domain_topology = default_topology;

#ifdef CONFIG_NUMA

static int sched_domains_numa_levels;
static int *sched_domains_numa_distance;
static struct cpumask ***sched_domains_numa_masks;
static int sched_domains_curr_level;

static inline int sd_local_flags(int level)
{
	if (sched_domains_numa_distance[level] > RECLAIM_DISTANCE)
		return 0;

	return SD_BALANCE_EXEC | SD_BALANCE_FORK | SD_WAKE_AFFINE;
}

static struct sched_domain *
sd_numa_init(struct sched_domain_topology_level *tl, int cpu)
{
	struct sched_domain *sd = *per_cpu_ptr(tl->data.sd, cpu);
	int level = tl->numa_level;
	int sd_weight = cpumask_weight(
			sched_domains_numa_masks[level][cpu_to_node(cpu)]);
=======
	if (atomic_read(&(*per_cpu_ptr(sdd->sds, cpu))->ref))
		*per_cpu_ptr(sdd->sds, cpu) = NULL;

	if (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
		*per_cpu_ptr(sdd->sg, cpu) = NULL;

	if (atomic_read(&(*per_cpu_ptr(sdd->sgc, cpu))->ref))
		*per_cpu_ptr(sdd->sgc, cpu) = NULL;
}

#ifdef CONFIG_NUMA
static int sched_domains_numa_levels;
enum numa_topology_type sched_numa_topology_type;
static int *sched_domains_numa_distance;
int sched_max_numa_distance;
static struct cpumask ***sched_domains_numa_masks;
static int sched_domains_curr_level;
#endif

/*
 * SD_flags allowed in topology descriptions.
 *
 * These flags are purely descriptive of the topology and do not prescribe
 * behaviour. Behaviour is artificial and mapped in the below sd_init()
 * function:
 *
 *   SD_SHARE_CPUCAPACITY   - describes SMT topologies
 *   SD_SHARE_PKG_RESOURCES - describes shared caches
 *   SD_NUMA                - describes NUMA topologies
 *   SD_SHARE_POWERDOMAIN   - describes shared power domain
 *   SD_ASYM_CPUCAPACITY    - describes mixed capacity topologies
 *   SD_SHARE_CAP_STATES    - describes shared capacity states
 *
 * Odd one out, which beside describing the topology has a quirk also
 * prescribes the desired behaviour that goes along with it:
 *
 *   SD_ASYM_PACKING        - describes SMT quirks
 */
#define TOPOLOGY_SD_FLAGS		\
	(SD_SHARE_CPUCAPACITY |		\
	 SD_SHARE_PKG_RESOURCES |	\
	 SD_NUMA |			\
	 SD_ASYM_PACKING |		\
	 SD_ASYM_CPUCAPACITY |		\
	 SD_SHARE_POWERDOMAIN |		\
	 SD_SHARE_CAP_STATES)

static struct sched_domain *
sd_init(struct sched_domain_topology_level *tl,
	const struct cpumask *cpu_map,
	struct sched_domain *child, int cpu)
{
	struct sd_data *sdd = &tl->data;
	struct sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	int sd_id, sd_weight, sd_flags = 0;

#ifdef CONFIG_NUMA
	/*
	 * Ugly hack to pass state to sd_numa_mask()...
	 */
	sched_domains_curr_level = tl->numa_level;
#endif

	sd_weight = cpumask_weight(tl->mask(cpu));

	if (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	if (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
			"wrong sd_flags in topology description\n"))
		sd_flags &= ~TOPOLOGY_SD_FLAGS;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	*sd = (struct sched_domain){
		.min_interval		= sd_weight,
		.max_interval		= 2*sd_weight,
		.busy_factor		= 32,
		.imbalance_pct		= 125,
<<<<<<< HEAD
		.cache_nice_tries	= 2,
		.busy_idx		= 3,
		.idle_idx		= 2,
=======

		.cache_nice_tries	= 0,
		.busy_idx		= 0,
		.idle_idx		= 0,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.newidle_idx		= 0,
		.wake_idx		= 0,
		.forkexec_idx		= 0,

		.flags			= 1*SD_LOAD_BALANCE
					| 1*SD_BALANCE_NEWIDLE
<<<<<<< HEAD
					| 0*SD_BALANCE_EXEC
					| 0*SD_BALANCE_FORK
					| 0*SD_BALANCE_WAKE
					| 0*SD_WAKE_AFFINE
					| 0*SD_SHARE_CPUPOWER
					| 0*SD_SHARE_PKG_RESOURCES
					| 1*SD_SERIALIZE
					| 0*SD_PREFER_SIBLING
					| sd_local_flags(level)
					,
		.last_balance		= jiffies,
		.balance_interval	= sd_weight,
	};
	SD_INIT_NAME(sd, NUMA);
	sd->private = &tl->data;

	/*
	 * Ugly hack to pass state to sd_numa_mask()...
	 */
	sched_domains_curr_level = tl->numa_level;
=======
					| 1*SD_BALANCE_EXEC
					| 1*SD_BALANCE_FORK
					| 0*SD_BALANCE_WAKE
					| 1*SD_WAKE_AFFINE
					| 0*SD_SHARE_CPUCAPACITY
					| 0*SD_SHARE_PKG_RESOURCES
					| 0*SD_SERIALIZE
					| 0*SD_PREFER_SIBLING
					| 0*SD_NUMA
					| sd_flags
					,

		.last_balance		= jiffies,
		.balance_interval	= sd_weight,
		.smt_gain		= 0,
		.max_newidle_lb_cost	= 0,
		.next_decay_max_lb_cost	= jiffies,
		.child			= child,
#ifdef CONFIG_SCHED_DEBUG
		.name			= tl->name,
#endif
	};

	cpumask_and(sched_domain_span(sd), cpu_map, tl->mask(cpu));
	sd_id = cpumask_first(sched_domain_span(sd));

	/*
	 * Convert topological properties into behaviour.
	 */

	if (sd->flags & SD_ASYM_CPUCAPACITY) {
		struct sched_domain *t = sd;

		for_each_lower_domain(t)
			t->flags |= SD_BALANCE_WAKE;
	}

	if (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->flags |= SD_PREFER_SIBLING;
		sd->imbalance_pct = 110;
		sd->smt_gain = 1178; /* ~15% */

	} else if (sd->flags & SD_SHARE_PKG_RESOURCES) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;
		sd->busy_idx = 2;

#ifdef CONFIG_NUMA
	} else if (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;
		sd->busy_idx = 3;
		sd->idle_idx = 2;

		sd->flags |= SD_SERIALIZE;
		if (sched_domains_numa_distance[tl->numa_level] > RECLAIM_DISTANCE) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#endif
	} else {
		sd->flags |= SD_PREFER_SIBLING;
		sd->cache_nice_tries = 1;
		sd->busy_idx = 2;
		sd->idle_idx = 1;
	}

	/*
	 * For all levels sharing cache; connect a sched_domain_shared
	 * instance.
	 */
	if (sd->flags & SD_SHARE_PKG_RESOURCES) {
		sd->shared = *per_cpu_ptr(sdd->sds, sd_id);
		atomic_inc(&sd->shared->ref);
		atomic_set(&sd->shared->nr_busy_cpus, sd_weight);
	}

	sd->private = sdd;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return sd;
}

<<<<<<< HEAD
=======
/*
 * Topology list, bottom-up.
 */
static struct sched_domain_topology_level default_topology[] = {
#ifdef CONFIG_SCHED_SMT
	{ cpu_smt_mask, cpu_smt_flags, SD_INIT_NAME(SMT) },
#endif
#ifdef CONFIG_SCHED_MC
	{ cpu_coregroup_mask, cpu_core_flags, SD_INIT_NAME(MC) },
#endif
	{ cpu_cpu_mask, SD_INIT_NAME(DIE) },
	{ NULL, },
};

static struct sched_domain_topology_level *sched_domain_topology =
	default_topology;

#define for_each_sd_topology(tl)			\
	for (tl = sched_domain_topology; tl->mask; tl++)

void set_sched_topology(struct sched_domain_topology_level *tl)
{
	if (WARN_ON_ONCE(sched_smp_initialized))
		return;

	sched_domain_topology = tl;
}

#ifdef CONFIG_NUMA

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static const struct cpumask *sd_numa_mask(int cpu)
{
	return sched_domains_numa_masks[sched_domains_curr_level][cpu_to_node(cpu)];
}

static void sched_numa_warn(const char *str)
{
	static int done = false;
	int i,j;

	if (done)
		return;

	done = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	for (i = 0; i < nr_node_ids; i++) {
		printk(KERN_WARNING "  ");
		for (j = 0; j < nr_node_ids; j++)
			printk(KERN_CONT "%02d ", node_distance(i,j));
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

<<<<<<< HEAD
static bool find_numa_distance(int distance)
=======
bool find_numa_distance(int distance)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int i;

	if (distance == node_distance(0, 0))
		return true;

	for (i = 0; i < sched_domains_numa_levels; i++) {
		if (sched_domains_numa_distance[i] == distance)
			return true;
	}

	return false;
}

<<<<<<< HEAD
=======
/*
 * A system can have three types of NUMA topology:
 * NUMA_DIRECT: all nodes are directly connected, or not a NUMA system
 * NUMA_GLUELESS_MESH: some nodes reachable through intermediary nodes
 * NUMA_BACKPLANE: nodes can reach other nodes through a backplane
 *
 * The difference between a glueless mesh topology and a backplane
 * topology lies in whether communication between not directly
 * connected nodes goes through intermediary nodes (where programs
 * could run), or through backplane controllers. This affects
 * placement of programs.
 *
 * The type of topology can be discerned with the following tests:
 * - If the maximum distance between any nodes is 1 hop, the system
 *   is directly connected.
 * - If for two nodes A and B, located N > 1 hops away from each other,
 *   there is an intermediary node C, which is < N hops away from both
 *   nodes A and B, the system is a glueless mesh.
 */
static void init_numa_topology_type(void)
{
	int a, b, c, n;

	n = sched_max_numa_distance;

	if (sched_domains_numa_levels <= 1) {
		sched_numa_topology_type = NUMA_DIRECT;
		return;
	}

	for_each_online_node(a) {
		for_each_online_node(b) {
			/* Find two nodes furthest removed from each other. */
			if (node_distance(a, b) < n)
				continue;

			/* Is there an intermediary node between a and b? */
			for_each_online_node(c) {
				if (node_distance(a, c) < n &&
				    node_distance(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					return;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			return;
		}
	}
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void sched_init_numa(void)
{
	int next_distance, curr_distance = node_distance(0, 0);
	struct sched_domain_topology_level *tl;
	int level = 0;
	int i, j, k;

	sched_domains_numa_distance = kzalloc(sizeof(int) * nr_node_ids, GFP_KERNEL);
	if (!sched_domains_numa_distance)
		return;

	/*
	 * O(nr_nodes^2) deduplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 *
	 * Assumes node_distance(0,j) includes all distances in
	 * node_distance(i,j) in order to avoid cubic time.
	 */
	next_distance = curr_distance;
	for (i = 0; i < nr_node_ids; i++) {
		for (j = 0; j < nr_node_ids; j++) {
			for (k = 0; k < nr_node_ids; k++) {
				int distance = node_distance(i, k);

				if (distance > curr_distance &&
				    (distance < next_distance ||
				     next_distance == curr_distance))
					next_distance = distance;

				/*
				 * While not a strong assumption it would be nice to know
				 * about cases where if node A is connected to B, B is not
				 * equally connected to A.
				 */
				if (sched_debug() && node_distance(k, i) != distance)
					sched_numa_warn("Node-distance not symmetric");

				if (sched_debug() && i && !find_numa_distance(distance))
					sched_numa_warn("Node-0 not representative");
			}
			if (next_distance != curr_distance) {
				sched_domains_numa_distance[level++] = next_distance;
				sched_domains_numa_levels = level;
				curr_distance = next_distance;
			} else break;
		}

		/*
		 * In case of sched_debug() we verify the above assumption.
		 */
		if (!sched_debug())
			break;
	}
<<<<<<< HEAD
=======

	if (!level)
		return;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * 'level' contains the number of unique distances, excluding the
	 * identity distance node_distance(i,i).
	 *
	 * The sched_domains_numa_distance[] array includes the actual distance
	 * numbers.
	 */

	/*
	 * Here, we should temporarily reset sched_domains_numa_levels to 0.
	 * If it fails to allocate memory for array sched_domains_numa_masks[][],
	 * the array will contain less then 'level' members. This could be
	 * dangerous when we use it to iterate array sched_domains_numa_masks[][]
	 * in other functions.
	 *
	 * We reset it to 'level' at the end of this function.
	 */
	sched_domains_numa_levels = 0;

	sched_domains_numa_masks = kzalloc(sizeof(void *) * level, GFP_KERNEL);
	if (!sched_domains_numa_masks)
		return;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * cpus of nodes that are that many hops away from us.
	 */
	for (i = 0; i < level; i++) {
		sched_domains_numa_masks[i] =
			kzalloc(nr_node_ids * sizeof(void *), GFP_KERNEL);
		if (!sched_domains_numa_masks[i])
			return;

		for (j = 0; j < nr_node_ids; j++) {
			struct cpumask *mask = kzalloc(cpumask_size(), GFP_KERNEL);
			if (!mask)
				return;

			sched_domains_numa_masks[i][j] = mask;

<<<<<<< HEAD
			for (k = 0; k < nr_node_ids; k++) {
=======
			for_each_node(k) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				if (node_distance(j, k) > sched_domains_numa_distance[i])
					continue;

				cpumask_or(mask, mask, cpumask_of_node(k));
			}
		}
	}

<<<<<<< HEAD
	tl = kzalloc((ARRAY_SIZE(default_topology) + level) *
=======
	/* Compute default topology size */
	for (i = 0; sched_domain_topology[i].mask; i++);

	tl = kzalloc((i + level + 1) *
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			sizeof(struct sched_domain_topology_level), GFP_KERNEL);
	if (!tl)
		return;

	/*
	 * Copy the default topology bits..
	 */
<<<<<<< HEAD
	for (i = 0; default_topology[i].init; i++)
		tl[i] = default_topology[i];
=======
	for (i = 0; sched_domain_topology[i].mask; i++)
		tl[i] = sched_domain_topology[i];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	for (j = 0; j < level; i++, j++) {
		tl[i] = (struct sched_domain_topology_level){
<<<<<<< HEAD
			.init = sd_numa_init,
			.mask = sd_numa_mask,
			.flags = SDTL_OVERLAP,
			.numa_level = j,
=======
			.mask = sd_numa_mask,
			.sd_flags = cpu_numa_flags,
			.flags = SDTL_OVERLAP,
			.numa_level = j,
			SD_INIT_NAME(NUMA)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		};
	}

	sched_domain_topology = tl;

	sched_domains_numa_levels = level;
<<<<<<< HEAD
}

static void sched_domains_numa_masks_set(int cpu)
{
	int i, j;
	int node = cpu_to_node(cpu);
=======
	sched_max_numa_distance = sched_domains_numa_distance[level - 1];

	init_numa_topology_type();
}

static void sched_domains_numa_masks_set(unsigned int cpu)
{
	int node = cpu_to_node(cpu);
	int i, j;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (i = 0; i < sched_domains_numa_levels; i++) {
		for (j = 0; j < nr_node_ids; j++) {
			if (node_distance(j, node) <= sched_domains_numa_distance[i])
				cpumask_set_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

<<<<<<< HEAD
static void sched_domains_numa_masks_clear(int cpu)
{
	int i, j;
=======
static void sched_domains_numa_masks_clear(unsigned int cpu)
{
	int i, j;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	for (i = 0; i < sched_domains_numa_levels; i++) {
		for (j = 0; j < nr_node_ids; j++)
			cpumask_clear_cpu(cpu, sched_domains_numa_masks[i][j]);
	}
}

<<<<<<< HEAD
/*
 * Update sched_domains_numa_masks[level][node] array when new cpus
 * are onlined.
 */
static int sched_domains_numa_masks_update(struct notifier_block *nfb,
					   unsigned long action,
					   void *hcpu)
{
	int cpu = (long)hcpu;

	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_ONLINE:
		sched_domains_numa_masks_set(cpu);
		break;

	case CPU_DEAD:
		sched_domains_numa_masks_clear(cpu);
		break;

	default:
		return NOTIFY_DONE;
	}

	return NOTIFY_OK;
}
#else
static inline void sched_init_numa(void)
{
}

static int sched_domains_numa_masks_update(struct notifier_block *nfb,
					   unsigned long action,
					   void *hcpu)
{
	return 0;
}
=======
#else
static inline void sched_init_numa(void) { }
static void sched_domains_numa_masks_set(unsigned int cpu) { }
static void sched_domains_numa_masks_clear(unsigned int cpu) { }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* CONFIG_NUMA */

static int __sdt_alloc(const struct cpumask *cpu_map)
{
	struct sched_domain_topology_level *tl;
	int j;

<<<<<<< HEAD
	for (tl = sched_domain_topology; tl->init; tl++) {
=======
	for_each_sd_topology(tl) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		struct sd_data *sdd = &tl->data;

		sdd->sd = alloc_percpu(struct sched_domain *);
		if (!sdd->sd)
			return -ENOMEM;

<<<<<<< HEAD
=======
		sdd->sds = alloc_percpu(struct sched_domain_shared *);
		if (!sdd->sds)
			return -ENOMEM;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		sdd->sg = alloc_percpu(struct sched_group *);
		if (!sdd->sg)
			return -ENOMEM;

<<<<<<< HEAD
		sdd->sgp = alloc_percpu(struct sched_group_power *);
		if (!sdd->sgp)
=======
		sdd->sgc = alloc_percpu(struct sched_group_capacity *);
		if (!sdd->sgc)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -ENOMEM;

		for_each_cpu(j, cpu_map) {
			struct sched_domain *sd;
<<<<<<< HEAD
			struct sched_group *sg;
			struct sched_group_power *sgp;

		       	sd = kzalloc_node(sizeof(struct sched_domain) + cpumask_size(),
=======
			struct sched_domain_shared *sds;
			struct sched_group *sg;
			struct sched_group_capacity *sgc;

			sd = kzalloc_node(sizeof(struct sched_domain) + cpumask_size(),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					GFP_KERNEL, cpu_to_node(j));
			if (!sd)
				return -ENOMEM;

			*per_cpu_ptr(sdd->sd, j) = sd;

<<<<<<< HEAD
=======
			sds = kzalloc_node(sizeof(struct sched_domain_shared),
					GFP_KERNEL, cpu_to_node(j));
			if (!sds)
				return -ENOMEM;

			*per_cpu_ptr(sdd->sds, j) = sds;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			sg = kzalloc_node(sizeof(struct sched_group) + cpumask_size(),
					GFP_KERNEL, cpu_to_node(j));
			if (!sg)
				return -ENOMEM;

			sg->next = sg;

			*per_cpu_ptr(sdd->sg, j) = sg;

<<<<<<< HEAD
			sgp = kzalloc_node(sizeof(struct sched_group_power) + cpumask_size(),
					GFP_KERNEL, cpu_to_node(j));
			if (!sgp)
				return -ENOMEM;

			*per_cpu_ptr(sdd->sgp, j) = sgp;
=======
			sgc = kzalloc_node(sizeof(struct sched_group_capacity) + cpumask_size(),
					GFP_KERNEL, cpu_to_node(j));
			if (!sgc)
				return -ENOMEM;

			*per_cpu_ptr(sdd->sgc, j) = sgc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	return 0;
}

static void __sdt_free(const struct cpumask *cpu_map)
{
	struct sched_domain_topology_level *tl;
	int j;

<<<<<<< HEAD
	for (tl = sched_domain_topology; tl->init; tl++) {
=======
	for_each_sd_topology(tl) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		struct sd_data *sdd = &tl->data;

		for_each_cpu(j, cpu_map) {
			struct sched_domain *sd;

			if (sdd->sd) {
				sd = *per_cpu_ptr(sdd->sd, j);
				if (sd && (sd->flags & SD_OVERLAP))
					free_sched_groups(sd->groups, 0);
				kfree(*per_cpu_ptr(sdd->sd, j));
			}

<<<<<<< HEAD
			if (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			if (sdd->sgp)
				kfree(*per_cpu_ptr(sdd->sgp, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = NULL;
		free_percpu(sdd->sg);
		sdd->sg = NULL;
		free_percpu(sdd->sgp);
		sdd->sgp = NULL;
=======
			if (sdd->sds)
				kfree(*per_cpu_ptr(sdd->sds, j));
			if (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			if (sdd->sgc)
				kfree(*per_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = NULL;
		free_percpu(sdd->sds);
		sdd->sds = NULL;
		free_percpu(sdd->sg);
		sdd->sg = NULL;
		free_percpu(sdd->sgc);
		sdd->sgc = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

struct sched_domain *build_sched_domain(struct sched_domain_topology_level *tl,
<<<<<<< HEAD
		struct s_data *d, const struct cpumask *cpu_map,
		struct sched_domain_attr *attr, struct sched_domain *child,
		int cpu)
{
	struct sched_domain *sd = tl->init(tl, cpu);
	if (!sd)
		return child;

	cpumask_and(sched_domain_span(sd), cpu_map, tl->mask(cpu));
=======
		const struct cpumask *cpu_map, struct sched_domain_attr *attr,
		struct sched_domain *child, int cpu)
{
	struct sched_domain *sd = sd_init(tl, cpu_map, child, cpu);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (child) {
		sd->level = child->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		child->parent = sd;
<<<<<<< HEAD
	}
	sd->child = child;
=======

		if (!cpumask_subset(sched_domain_span(child),
				    sched_domain_span(sd))) {
			pr_err("BUG: arch topology borken\n");
#ifdef CONFIG_SCHED_DEBUG
			pr_err("     the %s domain not a subset of the %s domain\n",
					child->name, sd->name);
#endif
			/* Fixup, ensure @sd has at least @child cpus. */
			cpumask_or(sched_domain_span(sd),
				   sched_domain_span(sd),
				   sched_domain_span(child));
		}

	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	set_domain_attribute(sd, attr);

	return sd;
}

/*
 * Build sched domains for a given set of cpus and attach the sched domains
 * to the individual cpus
 */
static int build_sched_domains(const struct cpumask *cpu_map,
			       struct sched_domain_attr *attr)
{
<<<<<<< HEAD
	enum s_alloc alloc_state = sa_none;
=======
	enum s_alloc alloc_state;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct sched_domain *sd;
	struct s_data d;
	int i, ret = -ENOMEM;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	if (alloc_state != sa_rootdomain)
		goto error;

	/* Set up domains for cpus specified by the cpu_map. */
	for_each_cpu(i, cpu_map) {
		struct sched_domain_topology_level *tl;

		sd = NULL;
<<<<<<< HEAD
		for (tl = sched_domain_topology; tl->init; tl++) {
			sd = build_sched_domain(tl, &d, cpu_map, attr, sd, i);
			if (tl->flags & SDTL_OVERLAP || sched_feat(FORCE_SD_OVERLAP))
				sd->flags |= SD_OVERLAP;
			if (cpumask_equal(cpu_map, sched_domain_span(sd)))
				break;
		}

		while (sd->child)
			sd = sd->child;

		*per_cpu_ptr(d.sd, i) = sd;
=======
		for_each_sd_topology(tl) {
			sd = build_sched_domain(tl, cpu_map, attr, sd, i);
			if (tl == sched_domain_topology)
				*per_cpu_ptr(d.sd, i) = sd;
			if (tl->flags & SDTL_OVERLAP || sched_feat(FORCE_SD_OVERLAP))
				sd->flags |= SD_OVERLAP;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		for (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			sd->span_weight = cpumask_weight(sched_domain_span(sd));
			if (sd->flags & SD_OVERLAP) {
				if (build_overlap_sched_groups(sd, i))
					goto error;
			} else {
				if (build_sched_groups(sd, i))
					goto error;
			}
		}
	}

<<<<<<< HEAD
	/* Calculate CPU power for physical packages and nodes */
	for (i = nr_cpumask_bits-1; i >= 0; i--) {
		if (!cpumask_test_cpu(i, cpu_map))
			continue;

		for (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			claim_allocations(i, sd);
			init_sched_groups_power(i, sd);
=======
	/* Calculate CPU capacity for physical packages and nodes */
	for (i = nr_cpumask_bits-1; i >= 0; i--) {
		struct sched_domain_topology_level *tl = sched_domain_topology;

		if (!cpumask_test_cpu(i, cpu_map))
			continue;

		for (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent, tl++) {
			init_sched_energy(i, sd, tl->energy);
			claim_allocations(i, sd);
			init_sched_groups_capacity(i, sd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	/* Attach the domains */
	rcu_read_lock();
	for_each_cpu(i, cpu_map) {
<<<<<<< HEAD
		sd = *per_cpu_ptr(d.sd, i);
=======
		int max_cpu = READ_ONCE(d.rd->max_cap_orig_cpu);
		int min_cpu = READ_ONCE(d.rd->min_cap_orig_cpu);

		if ((max_cpu < 0) || (cpu_rq(i)->cpu_capacity_orig >
		    cpu_rq(max_cpu)->cpu_capacity_orig))
			WRITE_ONCE(d.rd->max_cap_orig_cpu, i);

		if ((min_cpu < 0) || (cpu_rq(i)->cpu_capacity_orig <
		    cpu_rq(min_cpu)->cpu_capacity_orig))
			WRITE_ONCE(d.rd->min_cap_orig_cpu, i);

		sd = *per_cpu_ptr(d.sd, i);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		cpu_attach_domain(sd, d.rd, i);
	}
	rcu_read_unlock();

	ret = 0;
error:
	__free_domain_allocs(&d, alloc_state, cpu_map);
	return ret;
}

static cpumask_var_t *doms_cur;	/* current sched domains */
static int ndoms_cur;		/* number of sched domains in 'doms_cur' */
static struct sched_domain_attr *dattr_cur;
				/* attribues of custom domains in 'doms_cur' */

/*
 * Special case: If a kmalloc of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
static cpumask_var_t fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * cpu core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
<<<<<<< HEAD
int __attribute__((weak)) arch_update_cpu_topology(void)
=======
int __weak arch_update_cpu_topology(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return 0;
}

cpumask_var_t *alloc_sched_domains(unsigned int ndoms)
{
	int i;
	cpumask_var_t *doms;

	doms = kmalloc(sizeof(*doms) * ndoms, GFP_KERNEL);
	if (!doms)
		return NULL;
	for (i = 0; i < ndoms; i++) {
		if (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			return NULL;
		}
	}
	return doms;
}

void free_sched_domains(cpumask_var_t doms[], unsigned int ndoms)
{
	unsigned int i;
	for (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups. Callers must hold the hotplug lock.
 * For now this just excludes isolated cpus, but could be used to
 * exclude other special cases in the future.
 */
static int init_sched_domains(const struct cpumask *cpu_map)
{
	int err;

	arch_update_cpu_topology();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	if (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_andnot(doms_cur[0], cpu_map, cpu_isolated_map);
	err = build_sched_domains(doms_cur[0], NULL);
	register_sched_domain_sysctl();

	return err;
}

/*
 * Detach sched domains from a group of cpus specified in cpu_map
 * These cpus will now be attached to the NULL domain
 */
static void detach_destroy_domains(const struct cpumask *cpu_map)
{
	int i;

	rcu_read_lock();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(NULL, &def_root_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
static int dattrs_equal(struct sched_domain_attr *cur, int idx_cur,
			struct sched_domain_attr *new, int idx_new)
{
	struct sched_domain_attr tmp;

	/* fast path */
	if (!new && !cur)
		return 1;

	tmp = SD_ATTR_INIT;
	return !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			sizeof(struct sched_domain_attr));
}

/*
 * Partition sched domains as specified by the 'ndoms_new'
 * cpumasks in the array doms_new[] of cpumasks. This compares
 * doms_new[] to the current sched domain partitioning, doms_cur[].
 * It destroys each deleted domain and builds each new domain.
 *
 * 'doms_new' is an array of cpumask_var_t's of length 'ndoms_new'.
 * The masks don't intersect (don't overlap.) We should setup one
 * sched domain for each mask. CPUs not in any of the cpumasks will
 * not be load balanced. If the same cpumask appears both in the
 * current 'doms_cur' domains and in the new 'doms_new', we can leave
 * it as it is.
 *
 * The passed in 'doms_new' should be allocated using
 * alloc_sched_domains.  This routine takes ownership of it and will
 * free_sched_domains it when done with it. If the caller failed the
 * alloc call, then it can pass in doms_new == NULL && ndoms_new == 1,
 * and partition_sched_domains() will fallback to the single partition
 * 'fallback_doms', it also forces the domains to be rebuilt.
 *
 * If doms_new == NULL it will be replaced with cpu_online_mask.
 * ndoms_new == 0 is a special case for destroying existing domains,
 * and it will not create the default domain.
 *
 * Call with hotplug lock held
 */
void partition_sched_domains(int ndoms_new, cpumask_var_t doms_new[],
			     struct sched_domain_attr *dattr_new)
{
	int i, j, n;
	int new_topology;

	mutex_lock(&sched_domains_mutex);

	/* always unregister in case we don't destroy any domains */
	unregister_sched_domain_sysctl();

	/* Let architecture update cpu core mappings. */
	new_topology = arch_update_cpu_topology();

	n = doms_new ? ndoms_new : 0;

	/* Destroy deleted domains */
	for (i = 0; i < ndoms_cur; i++) {
		for (j = 0; j < n && !new_topology; j++) {
			if (cpumask_equal(doms_cur[i], doms_new[j])
			    && dattrs_equal(dattr_cur, i, dattr_new, j))
				goto match1;
		}
		/* no match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

<<<<<<< HEAD
	if (doms_new == NULL) {
		ndoms_cur = 0;
=======
	n = ndoms_cur;
	if (doms_new == NULL) {
		n = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		doms_new = &fallback_doms;
		cpumask_andnot(doms_new[0], cpu_active_mask, cpu_isolated_map);
		WARN_ON_ONCE(dattr_new);
	}

	/* Build new domains */
	for (i = 0; i < ndoms_new; i++) {
<<<<<<< HEAD
		for (j = 0; j < ndoms_cur && !new_topology; j++) {
=======
		for (j = 0; j < n && !new_topology; j++) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (cpumask_equal(doms_new[i], doms_cur[j])
			    && dattrs_equal(dattr_new, i, dattr_cur, j))
				goto match2;
		}
		/* no match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : NULL);
match2:
		;
	}

	/* Remember the new sched domains */
	if (doms_cur != &fallback_doms)
		free_sched_domains(doms_cur, ndoms_cur);
	kfree(dattr_cur);	/* kfree(NULL) is safe */
	doms_cur = doms_new;
	dattr_cur = dattr_new;
	ndoms_cur = ndoms_new;

	register_sched_domain_sysctl();

	mutex_unlock(&sched_domains_mutex);
}

static int num_cpus_frozen;	/* used to mark begin/end of suspend/resume */

/*
 * Update cpusets according to cpu_active mask.  If cpusets are
 * disabled, cpuset_update_active_cpus() becomes a simple wrapper
 * around partition_sched_domains().
 *
 * If we come here as part of a suspend/resume, don't touch cpusets because we
 * want to restore it back to its original state upon resume anyway.
 */
<<<<<<< HEAD
static int cpuset_cpu_active(struct notifier_block *nfb, unsigned long action,
			     void *hcpu)
{
	switch (action) {
	case CPU_ONLINE_FROZEN:
	case CPU_DOWN_FAILED_FROZEN:

=======
static void cpuset_cpu_active(void)
{
	if (cpuhp_tasks_frozen) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * num_cpus_frozen tracks how many CPUs are involved in suspend
		 * resume sequence. As long as this is not the last online
		 * operation in the resume sequence, just build a single sched
		 * domain, ignoring cpusets.
		 */
		partition_sched_domains(1, NULL, NULL);
		if (--num_cpus_frozen)
<<<<<<< HEAD
			break;

=======
			return;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * This is the last CPU online operation. So fall through and
		 * restore the original sched domains by considering the
		 * cpuset configurations.
		 */
		cpuset_force_rebuild();
<<<<<<< HEAD

	case CPU_ONLINE:
	case CPU_DOWN_FAILED:
		cpuset_update_active_cpus(true);
		break;
	default:
		return NOTIFY_DONE;
	}
	return NOTIFY_OK;
}

static int cpuset_cpu_inactive(struct notifier_block *nfb, unsigned long action,
			       void *hcpu)
{
	switch (action) {
	case CPU_DOWN_PREPARE:
		cpuset_update_active_cpus(false);
		break;
	case CPU_DOWN_PREPARE_FROZEN:
		num_cpus_frozen++;
		partition_sched_domains(1, NULL, NULL);
		break;
	default:
		return NOTIFY_DONE;
	}
	return NOTIFY_OK;
}
=======
	}
	cpuset_update_active_cpus(true);
}

static int cpuset_cpu_inactive(unsigned int cpu)
{
	unsigned long flags;
	struct dl_bw *dl_b;
	bool overflow;
	int cpus;

	if (!cpuhp_tasks_frozen) {
		rcu_read_lock_sched();
		dl_b = dl_bw_of(cpu);

		raw_spin_lock_irqsave(&dl_b->lock, flags);
		cpus = dl_bw_cpus(cpu);
		overflow = __dl_overflow(dl_b, cpus, 0, 0);
		raw_spin_unlock_irqrestore(&dl_b->lock, flags);

		rcu_read_unlock_sched();

		if (overflow)
			return -EBUSY;
		cpuset_update_active_cpus(false);
	} else {
		num_cpus_frozen++;
		partition_sched_domains(1, NULL, NULL);
	}
	return 0;
}

#ifdef CONFIG_SCHED_SMT
atomic_t sched_smt_present = ATOMIC_INIT(0);
#endif

int sched_cpu_activate(unsigned int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long flags;

#ifdef CONFIG_SCHED_SMT
	/*
	 * When going up, increment the number of cores with SMT present.
	 */
	if (cpumask_weight(cpu_smt_mask(cpu)) == 2)
		atomic_inc(&sched_smt_present);
#endif
	set_cpu_active(cpu, true);

	if (sched_smp_initialized) {
		sched_domains_numa_masks_set(cpu);
		cpuset_cpu_active();
	}

	/*
	 * Put the rq online, if not already. This happens:
	 *
	 * 1) In the early boot process, because we build the real domains
	 *    after all cpus have been brought up.
	 *
	 * 2) At runtime, if cpuset_cpu_active() fails to rebuild the
	 *    domains.
	 */
	raw_spin_lock_irqsave(&rq->lock, flags);
	if (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->span));
		set_rq_online(rq);
	}
	raw_spin_unlock_irqrestore(&rq->lock, flags);

	update_max_interval();

	return 0;
}

int sched_cpu_deactivate(unsigned int cpu)
{
	int ret;

	set_cpu_active(cpu, false);
	/*
	 * We've cleared cpu_active_mask, wait for all preempt-disabled and RCU
	 * users of this state to go away such that all new such users will
	 * observe it.
	 *
	 * For CONFIG_PREEMPT we have preemptible RCU and its sync_rcu() might
	 * not imply sync_sched(), so wait for both.
	 *
	 * Do sync before park smpboot threads to take care the rcu boost case.
	 */
	if (IS_ENABLED(CONFIG_PREEMPT))
		synchronize_rcu_mult(call_rcu, call_rcu_sched);
	else
		synchronize_rcu();

#ifdef CONFIG_SCHED_SMT
	/*
	 * When going down, decrement the number of cores with SMT present.
	 */
	if (cpumask_weight(cpu_smt_mask(cpu)) == 2)
		atomic_dec(&sched_smt_present);
#endif

	if (!sched_smp_initialized)
		return 0;

	ret = cpuset_cpu_inactive(cpu);
	if (ret) {
		set_cpu_active(cpu, true);
		return ret;
	}
	sched_domains_numa_masks_clear(cpu);
	return 0;
}

static void sched_rq_cpu_starting(unsigned int cpu)
{
	struct rq *rq = cpu_rq(cpu);

	rq->calc_load_update = calc_load_update;
	update_max_interval();
}

int sched_cpu_starting(unsigned int cpu)
{
	set_cpu_rq_start_time(cpu);
	sched_rq_cpu_starting(cpu);
	return 0;
}

#ifdef CONFIG_HOTPLUG_CPU
int sched_cpu_dying(unsigned int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long flags;

	/* Handle pending wakeups and then migrate everything off */
	sched_ttwu_pending();
	raw_spin_lock_irqsave(&rq->lock, flags);

	walt_migrate_sync_cpu(cpu);

	if (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->span));
		set_rq_offline(rq);
	}
	migrate_tasks(rq);
	BUG_ON(rq->nr_running != 1);
	raw_spin_unlock_irqrestore(&rq->lock, flags);
	calc_load_migrate(rq);
	update_max_interval();
	nohz_balance_exit_idle(cpu);
	hrtick_clear(rq);
	return 0;
}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void __init sched_init_smp(void)
{
	cpumask_var_t non_isolated_cpus;

<<<<<<< HEAD
=======
	walt_init_cpu_efficiency();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	alloc_cpumask_var(&non_isolated_cpus, GFP_KERNEL);
	alloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	sched_init_numa();

<<<<<<< HEAD
	get_online_cpus();
=======
	/*
	 * There's no userspace yet to cause hotplug operations; hence all the
	 * cpu masks are stable and all blatant races in the below code cannot
	 * happen.
	 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_lock(&sched_domains_mutex);
	init_sched_domains(cpu_active_mask);
	cpumask_andnot(non_isolated_cpus, cpu_possible_mask, cpu_isolated_map);
	if (cpumask_empty(non_isolated_cpus))
		cpumask_set_cpu(smp_processor_id(), non_isolated_cpus);
	mutex_unlock(&sched_domains_mutex);
<<<<<<< HEAD
	put_online_cpus();

	hotcpu_notifier(sched_domains_numa_masks_update, CPU_PRI_SCHED_ACTIVE);
	hotcpu_notifier(cpuset_cpu_active, CPU_PRI_CPUSET_ACTIVE);
	hotcpu_notifier(cpuset_cpu_inactive, CPU_PRI_CPUSET_INACTIVE);

	/* RT runtime code needs to handle some hotplug events */
	hotcpu_notifier(update_runtime, 0);

	init_hrtick();
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Move init over to a non-isolated CPU */
	if (set_cpus_allowed_ptr(current, non_isolated_cpus) < 0)
		BUG();
	sched_init_granularity();
	free_cpumask_var(non_isolated_cpus);

	init_sched_rt_class();
<<<<<<< HEAD
}
=======
	init_sched_dl_class();
	sched_smp_initialized = true;
}

static int __init migration_init(void)
{
	sched_rq_cpu_starting(smp_processor_id());
	return 0;
}
early_initcall(migration_init);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
void __init sched_init_smp(void)
{
	sched_init_granularity();
}
#endif /* CONFIG_SMP */

<<<<<<< HEAD
const_debug unsigned int sysctl_timer_migration = 1;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int in_sched_functions(unsigned long addr)
{
	return in_lock_functions(addr) ||
		(addr >= (unsigned long)__sched_text_start
		&& addr < (unsigned long)__sched_text_end);
}

#ifdef CONFIG_CGROUP_SCHED
/*
 * Default task group.
 * Every task in system belongs to this group at bootup.
 */
struct task_group root_task_group;
LIST_HEAD(task_groups);
<<<<<<< HEAD
#endif

DECLARE_PER_CPU(cpumask_var_t, load_balance_mask);
=======

/* Cacheline aligned slab cache for task_group */
static struct kmem_cache *task_group_cache __read_mostly;
#endif

DECLARE_PER_CPU(cpumask_var_t, load_balance_mask);
DECLARE_PER_CPU(cpumask_var_t, select_idle_mask);

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)
static wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(void *word, int bit)
{
	const int shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	return bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_SYMBOL(bit_waitqueue);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void __init sched_init(void)
{
	int i, j;
	unsigned long alloc_size = 0, ptr;

<<<<<<< HEAD
=======
	for (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_FAIR_GROUP_SCHED
	alloc_size += 2 * nr_cpu_ids * sizeof(void **);
#endif
#ifdef CONFIG_RT_GROUP_SCHED
	alloc_size += 2 * nr_cpu_ids * sizeof(void **);
#endif
<<<<<<< HEAD
#ifdef CONFIG_CPUMASK_OFFSTACK
	alloc_size += num_possible_cpus() * cpumask_size();
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (alloc_size) {
		ptr = (unsigned long)kzalloc(alloc_size, GFP_NOWAIT);

#ifdef CONFIG_FAIR_GROUP_SCHED
		root_task_group.se = (struct sched_entity **)ptr;
		ptr += nr_cpu_ids * sizeof(void **);

		root_task_group.cfs_rq = (struct cfs_rq **)ptr;
		ptr += nr_cpu_ids * sizeof(void **);

#endif /* CONFIG_FAIR_GROUP_SCHED */
#ifdef CONFIG_RT_GROUP_SCHED
		root_task_group.rt_se = (struct sched_rt_entity **)ptr;
		ptr += nr_cpu_ids * sizeof(void **);

		root_task_group.rt_rq = (struct rt_rq **)ptr;
		ptr += nr_cpu_ids * sizeof(void **);

#endif /* CONFIG_RT_GROUP_SCHED */
<<<<<<< HEAD
#ifdef CONFIG_CPUMASK_OFFSTACK
		for_each_possible_cpu(i) {
			per_cpu(load_balance_mask, i) = (void *)ptr;
			ptr += cpumask_size();
		}
#endif /* CONFIG_CPUMASK_OFFSTACK */
	}
=======
	}
#ifdef CONFIG_CPUMASK_OFFSTACK
	for_each_possible_cpu(i) {
		per_cpu(load_balance_mask, i) = (cpumask_var_t)kzalloc_node(
			cpumask_size(), GFP_KERNEL, cpu_to_node(i));
		per_cpu(select_idle_mask, i) = (cpumask_var_t)kzalloc_node(
			cpumask_size(), GFP_KERNEL, cpu_to_node(i));
	}
#endif /* CONFIG_CPUMASK_OFFSTACK */

	init_rt_bandwidth(&def_rt_bandwidth,
			global_rt_period(), global_rt_runtime());
	init_dl_bandwidth(&def_dl_bandwidth,
			global_rt_period(), global_rt_runtime());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_SMP
	init_defrootdomain();
#endif

<<<<<<< HEAD
	init_rt_bandwidth(&def_rt_bandwidth,
			global_rt_period(), global_rt_runtime());

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_RT_GROUP_SCHED
	init_rt_bandwidth(&root_task_group.rt_bandwidth,
			global_rt_period(), global_rt_runtime());
#endif /* CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_CGROUP_SCHED
<<<<<<< HEAD
=======
	task_group_cache = KMEM_CACHE(task_group, 0);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	list_add(&root_task_group.list, &task_groups);
	INIT_LIST_HEAD(&root_task_group.children);
	INIT_LIST_HEAD(&root_task_group.siblings);
	autogroup_init(&init_task);
<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* CONFIG_CGROUP_SCHED */

	for_each_possible_cpu(i) {
		struct rq *rq;

		rq = cpu_rq(i);
		raw_spin_lock_init(&rq->lock);
		rq->nr_running = 0;
		rq->calc_load_active = 0;
		rq->calc_load_update = jiffies + LOAD_FREQ;
		init_cfs_rq(&rq->cfs);
<<<<<<< HEAD
		init_rt_rq(&rq->rt, rq);
#ifdef CONFIG_FAIR_GROUP_SCHED
		root_task_group.shares = ROOT_TASK_GROUP_LOAD;
		INIT_LIST_HEAD(&rq->leaf_cfs_rq_list);
=======
		init_rt_rq(&rq->rt);
		init_dl_rq(&rq->dl);
#ifdef CONFIG_FAIR_GROUP_SCHED
		root_task_group.shares = ROOT_TASK_GROUP_LOAD;
		INIT_LIST_HEAD(&rq->leaf_cfs_rq_list);
		rq->tmp_alone_branch = &rq->leaf_cfs_rq_list;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * How much cpu bandwidth does root_task_group get?
		 *
		 * In case of task-groups formed thr' the cgroup filesystem, it
		 * gets 100% of the cpu resources in the system. This overall
		 * system cpu resource is divided among the tasks of
		 * root_task_group and its child task-groups in a fair manner,
		 * based on each entity's (task or task-group's) weight
		 * (se->load.weight).
		 *
		 * In other words, if root_task_group has 10 tasks of weight
		 * 1024) and two child groups A0 and A1 (of weight 1024 each),
		 * then A0's share of the cpu resource is:
		 *
		 *	A0's bandwidth = 1024 / (10*1024 + 1024 + 1024) = 8.33%
		 *
		 * We achieve this by letting root_task_group's tasks sit
		 * directly in rq->cfs (i.e root_task_group->se[] = NULL).
		 */
		init_cfs_bandwidth(&root_task_group.cfs_bandwidth);
		init_tg_cfs_entry(&root_task_group, &rq->cfs, NULL, i, NULL);
#endif /* CONFIG_FAIR_GROUP_SCHED */

		rq->rt.rt_runtime = def_rt_bandwidth.rt_runtime;
#ifdef CONFIG_RT_GROUP_SCHED
<<<<<<< HEAD
		INIT_LIST_HEAD(&rq->leaf_rt_rq_list);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		init_tg_rt_entry(&root_task_group, &rq->rt, NULL, i, NULL);
#endif

		for (j = 0; j < CPU_LOAD_IDX_MAX; j++)
			rq->cpu_load[j] = 0;

<<<<<<< HEAD
		rq->last_load_update_tick = jiffies;

#ifdef CONFIG_SMP
		rq->sd = NULL;
		rq->rd = NULL;
		rq->cpu_power = SCHED_POWER_SCALE;
		rq->post_schedule = 0;
=======
#ifdef CONFIG_SMP
		rq->sd = NULL;
		rq->rd = NULL;
		rq->cpu_capacity = rq->cpu_capacity_orig = SCHED_CAPACITY_SCALE;
		rq->balance_callback = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rq->active_balance = 0;
		rq->next_balance = jiffies;
		rq->push_cpu = 0;
		rq->cpu = i;
		rq->online = 0;
		rq->idle_stamp = 0;
		rq->avg_idle = 2*sysctl_sched_migration_cost;
<<<<<<< HEAD
=======
		rq->max_idle_balance_cost = sysctl_sched_migration_cost;
#ifdef CONFIG_SCHED_WALT
		rq->cur_irqload = 0;
		rq->avg_irqload = 0;
		rq->irqload_ts = 0;
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		INIT_LIST_HEAD(&rq->cfs_tasks);

		rq_attach_root(rq, &def_root_domain);
#ifdef CONFIG_NO_HZ_COMMON
<<<<<<< HEAD
=======
		rq->last_load_update_tick = jiffies;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rq->nohz_flags = 0;
#endif
#ifdef CONFIG_NO_HZ_FULL
		rq->last_sched_tick = 0;
#endif
<<<<<<< HEAD
#endif
=======
#endif /* CONFIG_SMP */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		init_rq_hrtick(rq);
		atomic_set(&rq->nr_iowait, 0);
	}

	set_load_weight(&init_task);

<<<<<<< HEAD
#ifdef CONFIG_PREEMPT_NOTIFIERS
	INIT_HLIST_HEAD(&init_task.preempt_notifiers);
#endif

#ifdef CONFIG_RT_MUTEXES
	plist_head_init(&init_task.pi_waiters);
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * The boot idle thread does lazy MMU switching as well:
	 */
	atomic_inc(&init_mm.mm_count);
	enter_lazy_tlb(&init_mm, current);

	/*
	 * Make us the idle thread. Technically, schedule() should not be
	 * called from this thread, however somewhere below it might be,
	 * but because we are the idle thread, we just pick up running again
	 * when this runqueue becomes "idle".
	 */
	init_idle(current, smp_processor_id());

	calc_load_update = jiffies + LOAD_FREQ;

<<<<<<< HEAD
	/*
	 * During early bootup we pretend to be a normal task:
	 */
	current->sched_class = &fair_sched_class;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_SMP
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_NOWAIT);
	/* May be allocated at isolcpus cmdline parse time */
	if (cpu_isolated_map == NULL)
		zalloc_cpumask_var(&cpu_isolated_map, GFP_NOWAIT);
	idle_thread_set_boot_cpu();
<<<<<<< HEAD
#endif
	init_sched_fair_class();

=======
	set_cpu_rq_start_time(smp_processor_id());
#endif
	init_sched_fair_class();

	init_schedstats();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	scheduler_running = 1;
}

#ifdef CONFIG_DEBUG_ATOMIC_SLEEP
static inline int preempt_count_equals(int preempt_offset)
{
<<<<<<< HEAD
	int nested = (preempt_count() & ~PREEMPT_ACTIVE) + rcu_preempt_depth();
=======
	int nested = preempt_count() + rcu_preempt_depth();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return (nested == preempt_offset);
}

static int __might_sleep_init_called;
int __init __might_sleep_init(void)
{
	__might_sleep_init_called = 1;
	return 0;
}
early_initcall(__might_sleep_init);

void __might_sleep(const char *file, int line, int preempt_offset)
{
<<<<<<< HEAD
	static unsigned long prev_jiffy;	/* ratelimiting */

	rcu_sleep_check(); /* WARN_ON_ONCE() by default, no rate limit reqd. */
	if ((preempt_count_equals(preempt_offset) && !irqs_disabled()) ||
	    oops_in_progress)
=======
	/*
	 * Blocking primitives will set (and therefore destroy) current->state,
	 * since we will exit with TASK_RUNNING make sure we enter with it,
	 * otherwise we will destroy state.
	 */
	WARN_ONCE(current->state != TASK_RUNNING && current->task_state_change,
			"do not call blocking ops when !TASK_RUNNING; "
			"state=%lx set at [<%p>] %pS\n",
			current->state,
			(void *)current->task_state_change,
			(void *)current->task_state_change);

	___might_sleep(file, line, preempt_offset);
}
EXPORT_SYMBOL(__might_sleep);

void ___might_sleep(const char *file, int line, int preempt_offset)
{
	static unsigned long prev_jiffy;	/* ratelimiting */
	unsigned long preempt_disable_ip;

	rcu_sleep_check(); /* WARN_ON_ONCE() by default, no rate limit reqd. */
	if ((preempt_count_equals(preempt_offset) && !irqs_disabled() &&
	     !is_idle_task(current)) || oops_in_progress)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;
	if (system_state != SYSTEM_RUNNING &&
	    (!__might_sleep_init_called || system_state != SYSTEM_BOOTING))
		return;
	if (time_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		return;
	prev_jiffy = jiffies;

<<<<<<< HEAD
=======
	/* Save this before calling printk(), since that will clobber it */
	preempt_disable_ip = get_preempt_disable_ip(current);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	printk(KERN_ERR
		"BUG: sleeping function called from invalid context at %s:%d\n",
			file, line);
	printk(KERN_ERR
		"in_atomic(): %d, irqs_disabled(): %d, pid: %d, name: %s\n",
			in_atomic(), irqs_disabled(),
			current->pid, current->comm);

<<<<<<< HEAD
	debug_show_held_locks(current);
	if (irqs_disabled())
		print_irqtrace_events(current);
	dump_stack();
}
EXPORT_SYMBOL(__might_sleep);
#endif

#ifdef CONFIG_MAGIC_SYSRQ
static void normalize_task(struct rq *rq, struct task_struct *p)
{
	const struct sched_class *prev_class = p->sched_class;
	struct sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
	};
	int old_prio = p->prio;
	int on_rq;

	on_rq = p->on_rq;
	if (on_rq)
		dequeue_task(rq, p, 0);
	__setscheduler(rq, p, &attr);
	if (on_rq) {
		enqueue_task(rq, p, 0);
		resched_task(rq->curr);
	}

	check_class_changed(rq, p, prev_class, old_prio);
}

void normalize_rt_tasks(void)
{
	struct task_struct *g, *p;
	unsigned long flags;
	struct rq *rq;

	read_lock_irqsave(&tasklist_lock, flags);
	do_each_thread(g, p) {
		/*
		 * Only normalize user tasks:
		 */
		if (!p->mm)
			continue;

		p->se.exec_start		= 0;
#ifdef CONFIG_SCHEDSTATS
		p->se.statistics.wait_start	= 0;
		p->se.statistics.sleep_start	= 0;
		p->se.statistics.block_start	= 0;
#endif

		if (!rt_task(p)) {
=======
	if (task_stack_end_corrupted(current))
		printk(KERN_EMERG "Thread overran stack, or stack corrupted\n");

	debug_show_held_locks(current);
	if (irqs_disabled())
		print_irqtrace_events(current);
	if (IS_ENABLED(CONFIG_DEBUG_PREEMPT)
	    && !preempt_count_equals(preempt_offset)) {
		pr_err("Preemption disabled at:");
		print_ip_sym(preempt_disable_ip);
		pr_cont("\n");
	}
	dump_stack();
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_SYMBOL(___might_sleep);
#endif

#ifdef CONFIG_MAGIC_SYSRQ
void normalize_rt_tasks(void)
{
	struct task_struct *g, *p;
	struct sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
	};

	read_lock(&tasklist_lock);
	for_each_process_thread(g, p) {
		/*
		 * Only normalize user tasks:
		 */
		if (p->flags & PF_KTHREAD)
			continue;

		p->se.exec_start = 0;
		schedstat_set(p->se.statistics.wait_start,  0);
		schedstat_set(p->se.statistics.sleep_start, 0);
		schedstat_set(p->se.statistics.block_start, 0);

		if (!dl_task(p) && !rt_task(p)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/*
			 * Renice negative nice level userspace
			 * tasks back to 0:
			 */
<<<<<<< HEAD
			if (TASK_NICE(p) < 0 && p->mm)
=======
			if (task_nice(p) < 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				set_user_nice(p, 0);
			continue;
		}

<<<<<<< HEAD
		raw_spin_lock(&p->pi_lock);
		rq = __task_rq_lock(p);

		normalize_task(rq, p);

		__task_rq_unlock(rq);
		raw_spin_unlock(&p->pi_lock);
	} while_each_thread(g, p);

	read_unlock_irqrestore(&tasklist_lock, flags);
=======
		__sched_setscheduler(p, &attr, false, false);
	}
	read_unlock(&tasklist_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif /* CONFIG_MAGIC_SYSRQ */

#if defined(CONFIG_IA64) || defined(CONFIG_KGDB_KDB)
/*
 * These functions are only useful for the IA64 MCA handling, or kdb.
 *
 * They can only be called when the whole system has been
 * stopped - every CPU needs to be quiescent, and no scheduling
 * activity can take place. Using them for anything else would
 * be a serious bug, and as a result, they aren't even visible
 * under any other configuration.
 */

/**
 * curr_task - return the current task for a given cpu.
 * @cpu: the processor in question.
 *
 * ONLY VALID WHEN THE WHOLE SYSTEM IS STOPPED!
<<<<<<< HEAD
=======
 *
 * Return: The current task for @cpu.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
struct task_struct *curr_task(int cpu)
{
	return cpu_curr(cpu);
}

#endif /* defined(CONFIG_IA64) || defined(CONFIG_KGDB_KDB) */

#ifdef CONFIG_IA64
/**
 * set_curr_task - set the current task for a given cpu.
 * @cpu: the processor in question.
 * @p: the task pointer to set.
 *
 * Description: This function must only be used when non-maskable interrupts
 * are serviced on a separate stack. It allows the architecture to switch the
 * notion of the current task on a cpu in a non-blocking manner. This function
 * must be called with all CPU's synchronized, and interrupts disabled, the
 * and caller must save the original value of the current task (see
 * curr_task() above) and restore that value before reenabling interrupts and
 * re-starting the system.
 *
 * ONLY VALID WHEN THE WHOLE SYSTEM IS STOPPED!
 */
<<<<<<< HEAD
void set_curr_task(int cpu, struct task_struct *p)
=======
void ia64_set_curr_task(int cpu, struct task_struct *p)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	cpu_curr(cpu) = p;
}

#endif

#ifdef CONFIG_CGROUP_SCHED
/* task_group_lock serializes the addition/removal of task groups */
static DEFINE_SPINLOCK(task_group_lock);

<<<<<<< HEAD
static void free_sched_group(struct task_group *tg)
=======
static void sched_free_group(struct task_group *tg)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	free_fair_sched_group(tg);
	free_rt_sched_group(tg);
	autogroup_free(tg);
<<<<<<< HEAD
	kfree(tg);
=======
	kmem_cache_free(task_group_cache, tg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* allocate runqueue etc for a new task group */
struct task_group *sched_create_group(struct task_group *parent)
{
	struct task_group *tg;

<<<<<<< HEAD
	tg = kzalloc(sizeof(*tg), GFP_KERNEL);
=======
	tg = kmem_cache_alloc(task_group_cache, GFP_KERNEL | __GFP_ZERO);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!tg)
		return ERR_PTR(-ENOMEM);

	if (!alloc_fair_sched_group(tg, parent))
		goto err;

	if (!alloc_rt_sched_group(tg, parent))
		goto err;

	return tg;

err:
<<<<<<< HEAD
	free_sched_group(tg);
=======
	sched_free_group(tg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ERR_PTR(-ENOMEM);
}

void sched_online_group(struct task_group *tg, struct task_group *parent)
{
	unsigned long flags;

	spin_lock_irqsave(&task_group_lock, flags);
	list_add_rcu(&tg->list, &task_groups);

	WARN_ON(!parent); /* root should already exist */

	tg->parent = parent;
	INIT_LIST_HEAD(&tg->children);
	list_add_rcu(&tg->siblings, &parent->children);
	spin_unlock_irqrestore(&task_group_lock, flags);
<<<<<<< HEAD
}

/* rcu callback to free various structures associated with a task group */
static void free_sched_group_rcu(struct rcu_head *rhp)
{
	/* now it should be safe to free those cfs_rqs */
	free_sched_group(container_of(rhp, struct task_group, rcu));
}

/* Destroy runqueue etc associated with a task group */
void sched_destroy_group(struct task_group *tg)
{
	/* wait for possible concurrent references to cfs_rqs complete */
	call_rcu(&tg->rcu, free_sched_group_rcu);
=======

	online_fair_sched_group(tg);
}

/* rcu callback to free various structures associated with a task group */
static void sched_free_group_rcu(struct rcu_head *rhp)
{
	/* now it should be safe to free those cfs_rqs */
	sched_free_group(container_of(rhp, struct task_group, rcu));
}

void sched_destroy_group(struct task_group *tg)
{
	/* wait for possible concurrent references to cfs_rqs complete */
	call_rcu(&tg->rcu, sched_free_group_rcu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void sched_offline_group(struct task_group *tg)
{
	unsigned long flags;
<<<<<<< HEAD
	int i;

	/* end participation in shares distribution */
	for_each_possible_cpu(i)
		unregister_fair_sched_group(tg, i);
=======

	/* end participation in shares distribution */
	unregister_fair_sched_group(tg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_irqsave(&task_group_lock, flags);
	list_del_rcu(&tg->list);
	list_del_rcu(&tg->siblings);
	spin_unlock_irqrestore(&task_group_lock, flags);
}

<<<<<<< HEAD
/* change task's runqueue when it moves between groups.
 *	The caller of this function should have put the task in its new group
 *	by now. This function just updates tsk->se.cfs_rq and tsk->se.parent to
 *	reflect its new group.
 */
void sched_move_task(struct task_struct *tsk)
{
	struct task_group *tg;
	int on_rq, running;
	unsigned long flags;
	struct rq *rq;

	rq = task_rq_lock(tsk, &flags);

	running = task_current(rq, tsk);
	on_rq = tsk->on_rq;

	if (on_rq)
		dequeue_task(rq, tsk, 0);
	if (unlikely(running))
		tsk->sched_class->put_prev_task(rq, tsk);

	tg = container_of(task_subsys_state_check(tsk, cpu_cgroup_subsys_id,
				lockdep_is_held(&tsk->sighand->siglock)),
=======
static void sched_change_group(struct task_struct *tsk, int type)
{
	struct task_group *tg;

	/*
	 * All callers are synchronized by task_rq_lock(); we do not use RCU
	 * which is pointless here. Thus, we pass "true" to task_css_check()
	 * to prevent lockdep warnings.
	 */
	tg = container_of(task_css_check(tsk, cpu_cgrp_id, true),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			  struct task_group, css);
	tg = autogroup_task_group(tsk, tg);
	tsk->sched_task_group = tg;

#ifdef CONFIG_FAIR_GROUP_SCHED
<<<<<<< HEAD
	if (tsk->sched_class->task_move_group)
		tsk->sched_class->task_move_group(tsk, on_rq);
	else
#endif
		set_task_rq(tsk, task_cpu(tsk));

	if (unlikely(running))
		tsk->sched_class->set_curr_task(rq);
	if (on_rq)
		enqueue_task(rq, tsk, 0);

	task_rq_unlock(rq, tsk, &flags);
}
#endif /* CONFIG_CGROUP_SCHED */

#if defined(CONFIG_RT_GROUP_SCHED) || defined(CONFIG_CFS_BANDWIDTH)
static unsigned long to_ratio(u64 period, u64 runtime)
{
	if (runtime == RUNTIME_INF)
		return 1ULL << 20;

	return div64_u64(runtime << 20, period);
}
#endif
=======
	if (tsk->sched_class->task_change_group)
		tsk->sched_class->task_change_group(tsk, type);
	else
#endif
		set_task_rq(tsk, task_cpu(tsk));
}

/*
 * Change task's runqueue when it moves between groups.
 *
 * The caller of this function should have put the task in its new group by
 * now. This function just updates tsk->se.cfs_rq and tsk->se.parent to reflect
 * its new group.
 */
void sched_move_task(struct task_struct *tsk)
{
	int queued, running;
	struct rq_flags rf;
	struct rq *rq;

	rq = task_rq_lock(tsk, &rf);
	update_rq_clock(rq);

	running = task_current(rq, tsk);
	queued = task_on_rq_queued(tsk);

	if (queued)
		dequeue_task(rq, tsk, DEQUEUE_SAVE | DEQUEUE_MOVE);
	if (unlikely(running))
		put_prev_task(rq, tsk);

	sched_change_group(tsk, TASK_MOVE_GROUP);

	if (queued)
		enqueue_task(rq, tsk, ENQUEUE_RESTORE | ENQUEUE_MOVE);
	if (unlikely(running))
		set_curr_task(rq, tsk);

	task_rq_unlock(rq, tsk, &rf);
}
#endif /* CONFIG_CGROUP_SCHED */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_RT_GROUP_SCHED
/*
 * Ensure that the real time constraints are schedulable.
 */
static DEFINE_MUTEX(rt_constraints_mutex);

/* Must be called with tasklist_lock held */
static inline int tg_has_rt_tasks(struct task_group *tg)
{
	struct task_struct *g, *p;

<<<<<<< HEAD
	do_each_thread(g, p) {
		if (rt_task(p) && task_rq(p)->rt.tg == tg)
			return 1;
	} while_each_thread(g, p);
=======
	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	if (task_group_is_autogroup(tg))
		return 0;

	for_each_process_thread(g, p) {
		if (rt_task(p) && task_group(p) == tg)
			return 1;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

struct rt_schedulable_data {
	struct task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

static int tg_rt_schedulable(struct task_group *tg, void *data)
{
	struct rt_schedulable_data *d = data;
	struct task_group *child;
	unsigned long total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	if (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	if (runtime > period && runtime != RUNTIME_INF)
		return -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks.
	 */
	if (rt_bandwidth_enabled() && !runtime && tg_has_rt_tasks(tg))
		return -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	if (total > to_ratio(global_rt_period(), global_rt_runtime()))
		return -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		if (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	if (sum > total)
		return -EINVAL;

	return 0;
}

static int __rt_schedulable(struct task_group *tg, u64 period, u64 runtime)
{
	int ret;

	struct rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_tree(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	return ret;
}

static int tg_set_rt_bandwidth(struct task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	int i, err = 0;

<<<<<<< HEAD
=======
	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	if (tg == &root_task_group && rt_runtime == 0)
		return -EINVAL;

	/* No period doesn't make any sense. */
	if (rt_period == 0)
		return -EINVAL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_lock(&rt_constraints_mutex);
	read_lock(&tasklist_lock);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	if (err)
		goto unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		struct rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	read_unlock(&tasklist_lock);
	mutex_unlock(&rt_constraints_mutex);

	return err;
}

static int sched_group_set_rt_runtime(struct task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	if (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;

	return tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

static long sched_group_rt_runtime(struct task_group *tg)
{
	u64 rt_runtime_us;

	if (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		return -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	return rt_runtime_us;
}

<<<<<<< HEAD
static int sched_group_set_rt_period(struct task_group *tg, long rt_period_us)
{
	u64 rt_runtime, rt_period;

	rt_period = (u64)rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	if (rt_period == 0)
		return -EINVAL;

=======
static int sched_group_set_rt_period(struct task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

static long sched_group_rt_period(struct task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	return rt_period_us;
}
<<<<<<< HEAD

static int sched_rt_global_constraints(void)
{
	u64 runtime, period;
	int ret = 0;

	if (sysctl_sched_rt_period <= 0)
		return -EINVAL;

	runtime = global_rt_runtime();
	period = global_rt_period();

	/*
	 * Sanity check on the sysctl variables.
	 */
	if (runtime > period && runtime != RUNTIME_INF)
		return -EINVAL;

=======
#endif /* CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_RT_GROUP_SCHED
static int sched_rt_global_constraints(void)
{
	int ret = 0;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_lock(&rt_constraints_mutex);
	read_lock(&tasklist_lock);
	ret = __rt_schedulable(NULL, 0, 0);
	read_unlock(&tasklist_lock);
	mutex_unlock(&rt_constraints_mutex);

	return ret;
}

static int sched_rt_can_attach(struct task_group *tg, struct task_struct *tsk)
{
	/* Don't accept realtime tasks when there is no way for them to run */
	if (rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		return 0;

	return 1;
}

#else /* !CONFIG_RT_GROUP_SCHED */
static int sched_rt_global_constraints(void)
{
	unsigned long flags;
	int i;

<<<<<<< HEAD
	if (sysctl_sched_rt_period <= 0)
		return -EINVAL;

	/*
	 * There's always some RT tasks in the root group
	 * -- migration, kstopmachine etc..
	 */
	if (sysctl_sched_rt_runtime == 0)
		return -EBUSY;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	raw_spin_lock_irqsave(&def_rt_bandwidth.rt_runtime_lock, flags);
	for_each_possible_cpu(i) {
		struct rt_rq *rt_rq = &cpu_rq(i)->rt;

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = global_rt_runtime();
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irqrestore(&def_rt_bandwidth.rt_runtime_lock, flags);

	return 0;
}
#endif /* CONFIG_RT_GROUP_SCHED */

<<<<<<< HEAD
int sched_rr_handler(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp,
		loff_t *ppos)
{
	int ret;
	static DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	/* make sure that internally we keep jiffies */
	/* also, writing zero resets timeslice to default */
	if (!ret && write) {
		sched_rr_timeslice = sched_rr_timeslice <= 0 ?
			RR_TIMESLICE : msecs_to_jiffies(sched_rr_timeslice);
	}
	mutex_unlock(&mutex);
	return ret;
}

=======
static int sched_dl_global_validate(void)
{
	u64 runtime = global_rt_runtime();
	u64 period = global_rt_period();
	u64 new_bw = to_ratio(period, runtime);
	struct dl_bw *dl_b;
	int cpu, ret = 0;
	unsigned long flags;

	/*
	 * Here we want to check the bandwidth not being set to some
	 * value smaller than the currently allocated bandwidth in
	 * any of the root_domains.
	 *
	 * FIXME: Cycling on all the CPUs is overdoing, but simpler than
	 * cycling on root_domains... Discussion on different/better
	 * solutions is welcome!
	 */
	for_each_possible_cpu(cpu) {
		rcu_read_lock_sched();
		dl_b = dl_bw_of(cpu);

		raw_spin_lock_irqsave(&dl_b->lock, flags);
		if (new_bw < dl_b->total_bw)
			ret = -EBUSY;
		raw_spin_unlock_irqrestore(&dl_b->lock, flags);

		rcu_read_unlock_sched();

		if (ret)
			break;
	}

	return ret;
}

static void sched_dl_do_global(void)
{
	u64 new_bw = -1;
	struct dl_bw *dl_b;
	int cpu;
	unsigned long flags;

	def_dl_bandwidth.dl_period = global_rt_period();
	def_dl_bandwidth.dl_runtime = global_rt_runtime();

	if (global_rt_runtime() != RUNTIME_INF)
		new_bw = to_ratio(global_rt_period(), global_rt_runtime());

	/*
	 * FIXME: As above...
	 */
	for_each_possible_cpu(cpu) {
		rcu_read_lock_sched();
		dl_b = dl_bw_of(cpu);

		raw_spin_lock_irqsave(&dl_b->lock, flags);
		dl_b->bw = new_bw;
		raw_spin_unlock_irqrestore(&dl_b->lock, flags);

		rcu_read_unlock_sched();
	}
}

static int sched_rt_global_validate(void)
{
	if (sysctl_sched_rt_period <= 0)
		return -EINVAL;

	if ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		(sysctl_sched_rt_runtime > sysctl_sched_rt_period))
		return -EINVAL;

	return 0;
}

static void sched_rt_do_global(void)
{
	def_rt_bandwidth.rt_runtime = global_rt_runtime();
	def_rt_bandwidth.rt_period = ns_to_ktime(global_rt_period());
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int sched_rt_handler(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp,
		loff_t *ppos)
{
<<<<<<< HEAD
	int ret;
	int old_period, old_runtime;
	static DEFINE_MUTEX(mutex);
=======
	int old_period, old_runtime;
	static DEFINE_MUTEX(mutex);
	int ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	mutex_lock(&mutex);
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if (!ret && write) {
<<<<<<< HEAD
		ret = sched_rt_global_constraints();
		if (ret) {
			sysctl_sched_rt_period = old_period;
			sysctl_sched_rt_runtime = old_runtime;
		} else {
			def_rt_bandwidth.rt_runtime = global_rt_runtime();
			def_rt_bandwidth.rt_period =
				ns_to_ktime(global_rt_period());
		}
=======
		ret = sched_rt_global_validate();
		if (ret)
			goto undo;

		ret = sched_dl_global_validate();
		if (ret)
			goto undo;

		ret = sched_rt_global_constraints();
		if (ret)
			goto undo;

		sched_rt_do_global();
		sched_dl_do_global();
	}
	if (0) {
undo:
		sysctl_sched_rt_period = old_period;
		sysctl_sched_rt_runtime = old_runtime;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	mutex_unlock(&mutex);

	return ret;
}

<<<<<<< HEAD
#ifdef CONFIG_CGROUP_SCHED

/* return corresponding task_group object of a cgroup */
static inline struct task_group *cgroup_tg(struct cgroup *cgrp)
{
	return container_of(cgroup_subsys_state(cgrp, cpu_cgroup_subsys_id),
			    struct task_group, css);
}

static struct cgroup_subsys_state *cpu_cgroup_css_alloc(struct cgroup *cgrp)
{
	struct task_group *tg, *parent;

	if (!cgrp->parent) {
=======
int sched_rr_handler(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp,
		loff_t *ppos)
{
	int ret;
	static DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	/* make sure that internally we keep jiffies */
	/* also, writing zero resets timeslice to default */
	if (!ret && write) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);
	}
	mutex_unlock(&mutex);
	return ret;
}

#ifdef CONFIG_CGROUP_SCHED

static inline struct task_group *css_tg(struct cgroup_subsys_state *css)
{
	return css ? container_of(css, struct task_group, css) : NULL;
}

static struct cgroup_subsys_state *
cpu_cgroup_css_alloc(struct cgroup_subsys_state *parent_css)
{
	struct task_group *parent = css_tg(parent_css);
	struct task_group *tg;

	if (!parent) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* This is early initialization for the top cgroup */
		return &root_task_group.css;
	}

<<<<<<< HEAD
	parent = cgroup_tg(cgrp->parent);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	tg = sched_create_group(parent);
	if (IS_ERR(tg))
		return ERR_PTR(-ENOMEM);

	return &tg->css;
}

<<<<<<< HEAD
static int cpu_cgroup_css_online(struct cgroup *cgrp)
{
	struct task_group *tg = cgroup_tg(cgrp);
	struct task_group *parent;

	if (!cgrp->parent)
		return 0;

	parent = cgroup_tg(cgrp->parent);
	sched_online_group(tg, parent);
	return 0;
}

static void cpu_cgroup_css_free(struct cgroup *cgrp)
{
	struct task_group *tg = cgroup_tg(cgrp);

	sched_destroy_group(tg);
}

static void cpu_cgroup_css_offline(struct cgroup *cgrp)
{
	struct task_group *tg = cgroup_tg(cgrp);

	sched_offline_group(tg);
}

static int cpu_cgroup_can_attach(struct cgroup *cgrp,
				 struct cgroup_taskset *tset)
{
	struct task_struct *task;

	cgroup_taskset_for_each(task, cgrp, tset) {
#ifdef CONFIG_RT_GROUP_SCHED
		if (!sched_rt_can_attach(cgroup_tg(cgrp), task))
			return -EINVAL;
#else
		/* We don't support RT-tasks being in separate groups */
		if (task->sched_class != &fair_sched_class)
			return -EINVAL;
#endif
	}
	return 0;
}

static void cpu_cgroup_attach(struct cgroup *cgrp,
			      struct cgroup_taskset *tset)
{
	struct task_struct *task;

	cgroup_taskset_for_each(task, cgrp, tset)
		sched_move_task(task);
}

static void
cpu_cgroup_exit(struct cgroup *cgrp, struct cgroup *old_cgrp,
		struct task_struct *task)
{
	/*
	 * cgroup_exit() is called in the copy_process() failure path.
	 * Ignore this case since the task hasn't ran yet, this avoids
	 * trying to poke a half freed task state from generic code.
	 */
	if (!(task->flags & PF_EXITING))
		return;

	sched_move_task(task);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
static int cpu_shares_write_u64(struct cgroup *cgrp, struct cftype *cftype,
				u64 shareval)
{
	return sched_group_set_shares(cgroup_tg(cgrp), scale_load(shareval));
}

static u64 cpu_shares_read_u64(struct cgroup *cgrp, struct cftype *cft)
{
	struct task_group *tg = cgroup_tg(cgrp);
=======
/* Expose task group only after completing cgroup initialization */
static int cpu_cgroup_css_online(struct cgroup_subsys_state *css)
{
	struct task_group *tg = css_tg(css);
	struct task_group *parent = css_tg(css->parent);

	if (parent)
		sched_online_group(tg, parent);
	return 0;
}

static void cpu_cgroup_css_released(struct cgroup_subsys_state *css)
{
	struct task_group *tg = css_tg(css);

	sched_offline_group(tg);
}

static void cpu_cgroup_css_free(struct cgroup_subsys_state *css)
{
	struct task_group *tg = css_tg(css);

	/*
	 * Relies on the RCU grace period between css_released() and this.
	 */
	sched_free_group(tg);
}

/*
 * This is called before wake_up_new_task(), therefore we really only
 * have to set its group bits, all the other stuff does not apply.
 */
static void cpu_cgroup_fork(struct task_struct *task)
{
	struct rq_flags rf;
	struct rq *rq;

	rq = task_rq_lock(task, &rf);

	sched_change_group(task, TASK_SET_GROUP);

	task_rq_unlock(rq, task, &rf);
}

static int cpu_cgroup_can_attach(struct cgroup_taskset *tset)
{
	struct task_struct *task;
	struct cgroup_subsys_state *css;
	int ret = 0;

	cgroup_taskset_for_each(task, css, tset) {
#ifdef CONFIG_RT_GROUP_SCHED
		if (!sched_rt_can_attach(css_tg(css), task))
			return -EINVAL;
#endif
		/*
		 * Serialize against wake_up_new_task() such that if its
		 * running, we're sure to observe its full state.
		 */
		raw_spin_lock_irq(&task->pi_lock);
		/*
		 * Avoid calling sched_move_task() before wake_up_new_task()
		 * has happened. This would lead to problems with PELT, due to
		 * move wanting to detach+attach while we're not attached yet.
		 */
		if (task->state == TASK_NEW)
			ret = -EINVAL;
		raw_spin_unlock_irq(&task->pi_lock);

		if (ret)
			break;
	}
	return ret;
}

static void cpu_cgroup_attach(struct cgroup_taskset *tset)
{
	struct task_struct *task;
	struct cgroup_subsys_state *css;

	cgroup_taskset_for_each(task, css, tset)
		sched_move_task(task);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
static int cpu_shares_write_u64(struct cgroup_subsys_state *css,
				struct cftype *cftype, u64 shareval)
{
	if (shareval > scale_load_down(ULONG_MAX))
		shareval = MAX_SHARES;
	return sched_group_set_shares(css_tg(css), scale_load(shareval));
}

static u64 cpu_shares_read_u64(struct cgroup_subsys_state *css,
			       struct cftype *cft)
{
	struct task_group *tg = css_tg(css);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return (u64) scale_load_down(tg->shares);
}

#ifdef CONFIG_CFS_BANDWIDTH
static DEFINE_MUTEX(cfs_constraints_mutex);

const u64 max_cfs_quota_period = 1 * NSEC_PER_SEC; /* 1s */
const u64 min_cfs_quota_period = 1 * NSEC_PER_MSEC; /* 1ms */

static int __cfs_schedulable(struct task_group *tg, u64 period, u64 runtime);

static int tg_set_cfs_bandwidth(struct task_group *tg, u64 period, u64 quota)
{
	int i, ret = 0, runtime_enabled, runtime_was_enabled;
	struct cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;

	if (tg == &root_task_group)
		return -EINVAL;

	/*
	 * Ensure we have at some amount of bandwidth every period.  This is
	 * to prevent reaching a state of large arrears when throttled via
	 * entity_tick() resulting in prolonged exit starvation.
	 */
	if (quota < min_cfs_quota_period || period < min_cfs_quota_period)
		return -EINVAL;

	/*
	 * Likewise, bound things on the otherside by preventing insane quota
	 * periods.  This also allows us to normalize in computing quota
	 * feasibility.
	 */
	if (period > max_cfs_quota_period)
		return -EINVAL;

<<<<<<< HEAD
=======
	/*
	 * Prevent race between setting of cfs_rq->runtime_enabled and
	 * unthrottle_offline_cfs_rqs().
	 */
	get_online_cpus();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mutex_lock(&cfs_constraints_mutex);
	ret = __cfs_schedulable(tg, period, quota);
	if (ret)
		goto out_unlock;

	runtime_enabled = quota != RUNTIME_INF;
	runtime_was_enabled = cfs_b->quota != RUNTIME_INF;
	/*
	 * If we need to toggle cfs_bandwidth_used, off->on must occur
	 * before making related changes, and on->off must occur afterwards
	 */
	if (runtime_enabled && !runtime_was_enabled)
		cfs_bandwidth_usage_inc();
	raw_spin_lock_irq(&cfs_b->lock);
	cfs_b->period = ns_to_ktime(period);
	cfs_b->quota = quota;

	__refill_cfs_bandwidth_runtime(cfs_b);
	/* restart the period timer (if active) to handle new period expiry */
<<<<<<< HEAD
	if (runtime_enabled && cfs_b->timer_active) {
		/* force a reprogram */
		cfs_b->timer_active = 0;
		__start_cfs_bandwidth(cfs_b);
	}
	raw_spin_unlock_irq(&cfs_b->lock);

	for_each_possible_cpu(i) {
=======
	if (runtime_enabled)
		start_cfs_bandwidth(cfs_b);
	raw_spin_unlock_irq(&cfs_b->lock);

	for_each_online_cpu(i) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		struct cfs_rq *cfs_rq = tg->cfs_rq[i];
		struct rq *rq = cfs_rq->rq;

		raw_spin_lock_irq(&rq->lock);
		cfs_rq->runtime_enabled = runtime_enabled;
		cfs_rq->runtime_remaining = 0;

		if (cfs_rq->throttled)
			unthrottle_cfs_rq(cfs_rq);
		raw_spin_unlock_irq(&rq->lock);
	}
	if (runtime_was_enabled && !runtime_enabled)
		cfs_bandwidth_usage_dec();
out_unlock:
	mutex_unlock(&cfs_constraints_mutex);
<<<<<<< HEAD
=======
	put_online_cpus();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ret;
}

int tg_set_cfs_quota(struct task_group *tg, long cfs_quota_us)
{
	u64 quota, period;

	period = ktime_to_ns(tg->cfs_bandwidth.period);
	if (cfs_quota_us < 0)
		quota = RUNTIME_INF;
<<<<<<< HEAD
	else
		quota = (u64)cfs_quota_us * NSEC_PER_USEC;
=======
	else if ((u64)cfs_quota_us <= U64_MAX / NSEC_PER_USEC)
		quota = (u64)cfs_quota_us * NSEC_PER_USEC;
	else
		return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return tg_set_cfs_bandwidth(tg, period, quota);
}

long tg_get_cfs_quota(struct task_group *tg)
{
	u64 quota_us;

	if (tg->cfs_bandwidth.quota == RUNTIME_INF)
		return -1;

	quota_us = tg->cfs_bandwidth.quota;
	do_div(quota_us, NSEC_PER_USEC);

	return quota_us;
}

int tg_set_cfs_period(struct task_group *tg, long cfs_period_us)
{
	u64 quota, period;

<<<<<<< HEAD
=======
	if ((u64)cfs_period_us > U64_MAX / NSEC_PER_USEC)
		return -EINVAL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	period = (u64)cfs_period_us * NSEC_PER_USEC;
	quota = tg->cfs_bandwidth.quota;

	return tg_set_cfs_bandwidth(tg, period, quota);
}

long tg_get_cfs_period(struct task_group *tg)
{
	u64 cfs_period_us;

	cfs_period_us = ktime_to_ns(tg->cfs_bandwidth.period);
	do_div(cfs_period_us, NSEC_PER_USEC);

	return cfs_period_us;
}

<<<<<<< HEAD
static s64 cpu_cfs_quota_read_s64(struct cgroup *cgrp, struct cftype *cft)
{
	return tg_get_cfs_quota(cgroup_tg(cgrp));
}

static int cpu_cfs_quota_write_s64(struct cgroup *cgrp, struct cftype *cftype,
				s64 cfs_quota_us)
{
	return tg_set_cfs_quota(cgroup_tg(cgrp), cfs_quota_us);
}

static u64 cpu_cfs_period_read_u64(struct cgroup *cgrp, struct cftype *cft)
{
	return tg_get_cfs_period(cgroup_tg(cgrp));
}

static int cpu_cfs_period_write_u64(struct cgroup *cgrp, struct cftype *cftype,
				u64 cfs_period_us)
{
	return tg_set_cfs_period(cgroup_tg(cgrp), cfs_period_us);
=======
static s64 cpu_cfs_quota_read_s64(struct cgroup_subsys_state *css,
				  struct cftype *cft)
{
	return tg_get_cfs_quota(css_tg(css));
}

static int cpu_cfs_quota_write_s64(struct cgroup_subsys_state *css,
				   struct cftype *cftype, s64 cfs_quota_us)
{
	return tg_set_cfs_quota(css_tg(css), cfs_quota_us);
}

static u64 cpu_cfs_period_read_u64(struct cgroup_subsys_state *css,
				   struct cftype *cft)
{
	return tg_get_cfs_period(css_tg(css));
}

static int cpu_cfs_period_write_u64(struct cgroup_subsys_state *css,
				    struct cftype *cftype, u64 cfs_period_us)
{
	return tg_set_cfs_period(css_tg(css), cfs_period_us);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

struct cfs_schedulable_data {
	struct task_group *tg;
	u64 period, quota;
};

/*
 * normalize group quota/period to be quota/max_period
 * note: units are usecs
 */
static u64 normalize_cfs_quota(struct task_group *tg,
			       struct cfs_schedulable_data *d)
{
	u64 quota, period;

	if (tg == d->tg) {
		period = d->period;
		quota = d->quota;
	} else {
		period = tg_get_cfs_period(tg);
		quota = tg_get_cfs_quota(tg);
	}

	/* note: these should typically be equivalent */
	if (quota == RUNTIME_INF || quota == -1)
		return RUNTIME_INF;

	return to_ratio(period, quota);
}

static int tg_cfs_schedulable_down(struct task_group *tg, void *data)
{
	struct cfs_schedulable_data *d = data;
	struct cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	s64 quota = 0, parent_quota = -1;

	if (!tg->parent) {
		quota = RUNTIME_INF;
	} else {
		struct cfs_bandwidth *parent_b = &tg->parent->cfs_bandwidth;

		quota = normalize_cfs_quota(tg, d);
<<<<<<< HEAD
		parent_quota = parent_b->hierarchal_quota;
=======
		parent_quota = parent_b->hierarchical_quota;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/*
		 * ensure max(child_quota) <= parent_quota, inherit when no
		 * limit is set
		 */
		if (quota == RUNTIME_INF)
			quota = parent_quota;
		else if (parent_quota != RUNTIME_INF && quota > parent_quota)
			return -EINVAL;
	}
<<<<<<< HEAD
	cfs_b->hierarchal_quota = quota;
=======
	cfs_b->hierarchical_quota = quota;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static int __cfs_schedulable(struct task_group *tg, u64 period, u64 quota)
{
	int ret;
	struct cfs_schedulable_data data = {
		.tg = tg,
		.period = period,
		.quota = quota,
	};

	if (quota != RUNTIME_INF) {
		do_div(data.period, NSEC_PER_USEC);
		do_div(data.quota, NSEC_PER_USEC);
	}

	rcu_read_lock();
	ret = walk_tg_tree(tg_cfs_schedulable_down, tg_nop, &data);
	rcu_read_unlock();

	return ret;
}

<<<<<<< HEAD
static int cpu_stats_show(struct cgroup *cgrp, struct cftype *cft,
		struct cgroup_map_cb *cb)
{
	struct task_group *tg = cgroup_tg(cgrp);
	struct cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;

	cb->fill(cb, "nr_periods", cfs_b->nr_periods);
	cb->fill(cb, "nr_throttled", cfs_b->nr_throttled);
	cb->fill(cb, "throttled_time", cfs_b->throttled_time);
=======
static int cpu_stats_show(struct seq_file *sf, void *v)
{
	struct task_group *tg = css_tg(seq_css(sf));
	struct cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;

	seq_printf(sf, "nr_periods %d\n", cfs_b->nr_periods);
	seq_printf(sf, "nr_throttled %d\n", cfs_b->nr_throttled);
	seq_printf(sf, "throttled_time %llu\n", cfs_b->throttled_time);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}
#endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_RT_GROUP_SCHED
<<<<<<< HEAD
static int cpu_rt_runtime_write(struct cgroup *cgrp, struct cftype *cft,
				s64 val)
{
	return sched_group_set_rt_runtime(cgroup_tg(cgrp), val);
}

static s64 cpu_rt_runtime_read(struct cgroup *cgrp, struct cftype *cft)
{
	return sched_group_rt_runtime(cgroup_tg(cgrp));
}

static int cpu_rt_period_write_uint(struct cgroup *cgrp, struct cftype *cftype,
		u64 rt_period_us)
{
	return sched_group_set_rt_period(cgroup_tg(cgrp), rt_period_us);
}

static u64 cpu_rt_period_read_uint(struct cgroup *cgrp, struct cftype *cft)
{
	return sched_group_rt_period(cgroup_tg(cgrp));
=======
static int cpu_rt_runtime_write(struct cgroup_subsys_state *css,
				struct cftype *cft, s64 val)
{
	return sched_group_set_rt_runtime(css_tg(css), val);
}

static s64 cpu_rt_runtime_read(struct cgroup_subsys_state *css,
			       struct cftype *cft)
{
	return sched_group_rt_runtime(css_tg(css));
}

static int cpu_rt_period_write_uint(struct cgroup_subsys_state *css,
				    struct cftype *cftype, u64 rt_period_us)
{
	return sched_group_set_rt_period(css_tg(css), rt_period_us);
}

static u64 cpu_rt_period_read_uint(struct cgroup_subsys_state *css,
				   struct cftype *cft)
{
	return sched_group_rt_period(css_tg(css));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#endif /* CONFIG_RT_GROUP_SCHED */

static struct cftype cpu_files[] = {
#ifdef CONFIG_FAIR_GROUP_SCHED
	{
		.name = "shares",
		.read_u64 = cpu_shares_read_u64,
		.write_u64 = cpu_shares_write_u64,
	},
#endif
#ifdef CONFIG_CFS_BANDWIDTH
	{
		.name = "cfs_quota_us",
		.read_s64 = cpu_cfs_quota_read_s64,
		.write_s64 = cpu_cfs_quota_write_s64,
	},
	{
		.name = "cfs_period_us",
		.read_u64 = cpu_cfs_period_read_u64,
		.write_u64 = cpu_cfs_period_write_u64,
	},
	{
		.name = "stat",
<<<<<<< HEAD
		.read_map = cpu_stats_show,
=======
		.seq_show = cpu_stats_show,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
#endif
#ifdef CONFIG_RT_GROUP_SCHED
	{
		.name = "rt_runtime_us",
		.read_s64 = cpu_rt_runtime_read,
		.write_s64 = cpu_rt_runtime_write,
	},
	{
		.name = "rt_period_us",
		.read_u64 = cpu_rt_period_read_uint,
		.write_u64 = cpu_rt_period_write_uint,
	},
#endif
	{ }	/* terminate */
};

<<<<<<< HEAD
struct cgroup_subsys cpu_cgroup_subsys = {
	.name		= "cpu",
	.css_alloc	= cpu_cgroup_css_alloc,
	.css_free	= cpu_cgroup_css_free,
	.css_online	= cpu_cgroup_css_online,
	.css_offline	= cpu_cgroup_css_offline,
	.can_attach	= cpu_cgroup_can_attach,
	.attach		= cpu_cgroup_attach,
	.allow_attach	= subsys_cgroup_allow_attach,
	.exit		= cpu_cgroup_exit,
	.subsys_id	= cpu_cgroup_subsys_id,
	.base_cftypes	= cpu_files,
	.early_init	= 1,
=======
struct cgroup_subsys cpu_cgrp_subsys = {
	.css_alloc	= cpu_cgroup_css_alloc,
	.css_online	= cpu_cgroup_css_online,
	.css_released	= cpu_cgroup_css_released,
	.css_free	= cpu_cgroup_css_free,
	.fork		= cpu_cgroup_fork,
	.can_attach	= cpu_cgroup_can_attach,
	.attach		= cpu_cgroup_attach,
	.legacy_cftypes	= cpu_files,
	.early_init	= true,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif	/* CONFIG_CGROUP_SCHED */

void dump_cpu_task(int cpu)
{
	pr_info("Task dump for CPU %d:\n", cpu);
	sched_show_task(cpu_curr(cpu));
}
<<<<<<< HEAD
=======

/*
 * Nice levels are multiplicative, with a gentle 10% change for every
 * nice level changed. I.e. when a CPU-bound task goes from nice 0 to
 * nice 1, it will get ~10% less CPU time than another CPU-bound task
 * that remained on nice 0.
 *
 * The "10% effect" is relative and cumulative: from _any_ nice level,
 * if you go up 1 level, it's -10% CPU usage, if you go down 1 level
 * it's +10% CPU usage. (to achieve that we use a multiplier of 1.25.
 * If a task goes up by ~10% and another task goes down by ~10% then
 * the relative distance between them is ~25%.)
 */
const int sched_prio_to_weight[40] = {
 /* -20 */     88761,     71755,     56483,     46273,     36291,
 /* -15 */     29154,     23254,     18705,     14949,     11916,
 /* -10 */      9548,      7620,      6100,      4904,      3906,
 /*  -5 */      3121,      2501,      1991,      1586,      1277,
 /*   0 */      1024,       820,       655,       526,       423,
 /*   5 */       335,       272,       215,       172,       137,
 /*  10 */       110,        87,        70,        56,        45,
 /*  15 */        36,        29,        23,        18,        15,
};

/*
 * Inverse (2^32/x) values of the sched_prio_to_weight[] array, precalculated.
 *
 * In cases where the weight does not change often, we can use the
 * precalculated inverse to speed up arithmetics by turning divisions
 * into multiplications:
 */
const u32 sched_prio_to_wmult[40] = {
 /* -20 */     48388,     59856,     76040,     92818,    118348,
 /* -15 */    147320,    184698,    229616,    287308,    360437,
 /* -10 */    449829,    563644,    704093,    875809,   1099582,
 /*  -5 */   1376151,   1717300,   2157191,   2708050,   3363326,
 /*   0 */   4194304,   5237765,   6557202,   8165337,  10153587,
 /*   5 */  12820798,  15790321,  19976592,  24970740,  31350126,
 /*  10 */  39045157,  49367440,  61356676,  76695844,  95443717,
 /*  15 */ 119304647, 148102320, 186737708, 238609294, 286331153,
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
