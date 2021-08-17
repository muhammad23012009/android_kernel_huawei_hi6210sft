#include "sched.h"
<<<<<<< HEAD
=======
#include "walt.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */

#ifdef CONFIG_SMP
static int
<<<<<<< HEAD
select_task_rq_stop(struct task_struct *p, int sd_flag, int flags)
=======
select_task_rq_stop(struct task_struct *p, int cpu, int sd_flag, int flags)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return task_cpu(p); /* stop tasks as never migrate */
}
#endif /* CONFIG_SMP */

static void
check_preempt_curr_stop(struct rq *rq, struct task_struct *p, int flags)
{
	/* we're never preempted */
}

<<<<<<< HEAD
static struct task_struct *pick_next_task_stop(struct rq *rq)
{
	struct task_struct *stop = rq->stop;

	if (stop && stop->on_rq) {
		stop->se.exec_start = rq->clock_task;
		return stop;
	}

	return NULL;
=======
static struct task_struct *
pick_next_task_stop(struct rq *rq, struct task_struct *prev, struct pin_cookie cookie)
{
	struct task_struct *stop = rq->stop;

	if (!stop || !task_on_rq_queued(stop))
		return NULL;

	put_prev_task(rq, prev);

	stop->se.exec_start = rq_clock_task(rq);

	return stop;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void
enqueue_task_stop(struct rq *rq, struct task_struct *p, int flags)
{
<<<<<<< HEAD
	inc_nr_running(rq);
=======
	add_nr_running(rq, 1);
	walt_inc_cumulative_runnable_avg(rq, p);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void
dequeue_task_stop(struct rq *rq, struct task_struct *p, int flags)
{
<<<<<<< HEAD
	dec_nr_running(rq);
=======
	sub_nr_running(rq, 1);
	walt_dec_cumulative_runnable_avg(rq, p);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void yield_task_stop(struct rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

static void put_prev_task_stop(struct rq *rq, struct task_struct *prev)
{
	struct task_struct *curr = rq->curr;
	u64 delta_exec;

<<<<<<< HEAD
	delta_exec = rq->clock_task - curr->se.exec_start;
=======
	delta_exec = rq_clock_task(rq) - curr->se.exec_start;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (unlikely((s64)delta_exec < 0))
		delta_exec = 0;

	schedstat_set(curr->se.statistics.exec_max,
			max(curr->se.statistics.exec_max, delta_exec));

	curr->se.sum_exec_runtime += delta_exec;
	account_group_exec_runtime(curr, delta_exec);

<<<<<<< HEAD
	curr->se.exec_start = rq->clock_task;
=======
	curr->se.exec_start = rq_clock_task(rq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cpuacct_charge(curr, delta_exec);
}

static void task_tick_stop(struct rq *rq, struct task_struct *curr, int queued)
{
}

static void set_curr_task_stop(struct rq *rq)
{
	struct task_struct *stop = rq->stop;

<<<<<<< HEAD
	stop->se.exec_start = rq->clock_task;
=======
	stop->se.exec_start = rq_clock_task(rq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void switched_to_stop(struct rq *rq, struct task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

static void
prio_changed_stop(struct rq *rq, struct task_struct *p, int oldprio)
{
	BUG(); /* how!?, what priority? */
}

static unsigned int
get_rr_interval_stop(struct rq *rq, struct task_struct *task)
{
	return 0;
}

<<<<<<< HEAD
=======
static void update_curr_stop(struct rq *rq)
{
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Simple, special scheduling class for the per-CPU stop tasks:
 */
const struct sched_class stop_sched_class = {
<<<<<<< HEAD
	.next			= &rt_sched_class,
=======
	.next			= &dl_sched_class,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	.enqueue_task		= enqueue_task_stop,
	.dequeue_task		= dequeue_task_stop,
	.yield_task		= yield_task_stop,

	.check_preempt_curr	= check_preempt_curr_stop,

	.pick_next_task		= pick_next_task_stop,
	.put_prev_task		= put_prev_task_stop,

#ifdef CONFIG_SMP
	.select_task_rq		= select_task_rq_stop,
<<<<<<< HEAD
=======
	.set_cpus_allowed	= set_cpus_allowed_common,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

	.set_curr_task          = set_curr_task_stop,
	.task_tick		= task_tick_stop,

	.get_rr_interval	= get_rr_interval_stop,

	.prio_changed		= prio_changed_stop,
	.switched_to		= switched_to_stop,
<<<<<<< HEAD
=======
	.update_curr		= update_curr_stop,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
