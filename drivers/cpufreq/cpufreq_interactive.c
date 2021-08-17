/*
 * drivers/cpufreq/cpufreq_interactive.c
 *
<<<<<<< HEAD
 * Copyright (C) 2010 Google, Inc.
=======
 * Copyright (C) 2010-2016 Google, Inc.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Author: Mike Chan (mike@android.com)
<<<<<<< HEAD
 *
 */

#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/cpufreq.h>
=======
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/cpufreq.h>
#include <linux/irq_work.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/rwsem.h>
#include <linux/sched.h>
#include <linux/sched/rt.h>
#include <linux/tick.h>
#include <linux/time.h>
#include <linux/timer.h>
<<<<<<< HEAD
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/input.h>
#include "cpufreq_governor.h"
=======
#include <linux/kthread.h>
#include <linux/slab.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define CREATE_TRACE_POINTS
#include <trace/events/cpufreq_interactive.h>

<<<<<<< HEAD
#ifdef CONFIG_HUAWEI_MSG_POLICY
#include <linux/kernel_stat.h>
#include <huawei_platform/power/msgnotify.h>
#endif

#ifdef CONFIG_HISI_HMPTH_OPEN
#define	DEFAULT_HMP_UP_THRESHOLD	(768)
#define	DEFAULT_HMP_DOWN_THRESHOLD	(448)
#define	HMP_OFF			(0)
#define	HMP_ON			(1)
#define HMP_PRIO		(1)
#define HMP_NAME		"boostpulse"
extern int set_hmp_policy(const char *pname, int prio, int state, int up_thresholds, int down_thresholds);
#endif

#ifdef CONFIG_ARCH_HI3XXX
static int cpufreq_interactive_initialized;
#endif

struct cpufreq_interactive_cpuinfo {
	struct timer_list cpu_timer;
	struct timer_list cpu_slack_timer;
	spinlock_t load_lock; /* protects the next 4 fields */
	u64 time_in_idle;
	u64 time_in_idle_timestamp;
	u64 cputime_speedadj;
	u64 cputime_speedadj_timestamp;
#ifdef CONFIG_HUAWEI_MSG_POLICY
	u64 cputime_msg_timestamp;
#endif
	struct cpufreq_policy *policy;
	struct cpufreq_frequency_table *freq_table;
	spinlock_t target_freq_lock; /*protects target freq */
	unsigned int target_freq;
	unsigned int floor_freq;
	unsigned int max_freq;
	u64 floor_validate_time;
	u64 hispeed_validate_time;
	struct rw_semaphore enable_sem;
	int governor_enabled;
};

static DEFINE_PER_CPU(struct cpufreq_interactive_cpuinfo, cpuinfo);

#ifdef CONFIG_INPUT_PULSE_SUPPORT
struct cpufreq_interactive_inputopen {
	struct input_handle *handle;
	struct work_struct inputopen_work;
};

static struct cpufreq_interactive_inputopen inputopen;
static struct workqueue_struct *down_wq;
#endif

/* realtime thread handles frequency scaling */
static struct task_struct *speedchange_task;
static cpumask_t speedchange_cpumask;
static spinlock_t speedchange_cpumask_lock;
static struct mutex gov_lock;

/* Target load.  Lower values result in higher CPU speeds. */
#define DEFAULT_TARGET_LOAD 95
static unsigned int default_target_loads[] = {DEFAULT_TARGET_LOAD};

#define DEFAULT_TIMER_RATE (20 * USEC_PER_MSEC)
#define DEFAULT_ABOVE_HISPEED_DELAY DEFAULT_TIMER_RATE
static unsigned int default_above_hispeed_delay[] = {
	DEFAULT_ABOVE_HISPEED_DELAY };

struct cpufreq_interactive_tunables {
	int usage_count;
	/* Hi speed to bump to from lo speed when load burst (default max) */
	unsigned int hispeed_freq;
	/* Go to hi speed when CPU load at or above this value. */
#define DEFAULT_GO_HISPEED_LOAD 99
	unsigned long go_hispeed_load;
=======
#define gov_attr_ro(_name)						\
static struct governor_attr _name =					\
__ATTR(_name, 0444, show_##_name, NULL)

#define gov_attr_wo(_name)						\
static struct governor_attr _name =					\
__ATTR(_name, 0200, NULL, store_##_name)

#define gov_attr_rw(_name)						\
static struct governor_attr _name =					\
__ATTR(_name, 0644, show_##_name, store_##_name)

/* Separate instance required for each 'interactive' directory in sysfs */
struct interactive_tunables {
	struct gov_attr_set attr_set;

	/* Hi speed to bump to from lo speed when load burst (default max) */
	unsigned int hispeed_freq;

	/* Go to hi speed when CPU load at or above this value. */
#define DEFAULT_GO_HISPEED_LOAD 99
	unsigned long go_hispeed_load;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Target load. Lower values result in higher CPU speeds. */
	spinlock_t target_loads_lock;
	unsigned int *target_loads;
	int ntarget_loads;
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * The minimum amount of time to spend at a frequency before we can ramp
	 * down.
	 */
#define DEFAULT_MIN_SAMPLE_TIME (80 * USEC_PER_MSEC)
	unsigned long min_sample_time;
<<<<<<< HEAD
	/*
	 * The sample rate of the timer used to increase frequency
	 */
	unsigned long timer_rate;
=======

	/* The sample rate of the timer used to increase frequency */
	unsigned long sampling_rate;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * Wait this long before raising speed above hispeed, by default a
	 * single timer interval.
	 */
	spinlock_t above_hispeed_delay_lock;
	unsigned int *above_hispeed_delay;
	int nabove_hispeed_delay;
<<<<<<< HEAD
	/* Non-zero means indefinite speed boost active */
	int boost_val;
	/* Duration of a boot pulse in usecs */
	int boostpulse_duration_val;
	/* End time of boost pulse in ktime converted to usecs */
	u64 boostpulse_endtime;

#ifdef CONFIG_ARCH_HI3XXX
	/* Minimun boostpulse interval */
#define DEFAULT_MIN_BOOSTPULSE_INTERVAL (500 * USEC_PER_MSEC)
	int boostpulse_min_interval;
#endif
	/*
	 * Max additional time to wait in idle, beyond timer_rate, at speeds
	 * above minimum before wakeup to reduce speed, or -1 if unnecessary.
	 */
#define DEFAULT_TIMER_SLACK (4 * DEFAULT_TIMER_RATE)
	int timer_slack_val;
	bool io_is_busy;

#ifdef CONFIG_HISI_HMPTH_OPEN
	/* Non-zero mean hmp boost active */
	int boost_hmp_val;
	bool hmp_boosted;
	/* boost hmp upthreshold, default 768 */
	int boost_hmp_upthreshold;
	/* boost hmp downthreshold, default 448 */
	int boost_hmp_downthreshold;
#endif
};

/* For cases where we have single governor instance for system */
static struct cpufreq_interactive_tunables *common_tunables;

static struct attribute_group *get_sysfs_attr(void);

static void cpufreq_interactive_timer_resched(
	struct cpufreq_interactive_cpuinfo *pcpu)
{
	struct cpufreq_interactive_tunables *tunables =
		pcpu->policy->governor_data;
	unsigned long expires;
	unsigned long flags;

	spin_lock_irqsave(&pcpu->load_lock, flags);
	pcpu->time_in_idle =
		get_cpu_idle_time(smp_processor_id(),
				  &pcpu->time_in_idle_timestamp,
				  tunables->io_is_busy);
	pcpu->cputime_speedadj = 0;
	pcpu->cputime_speedadj_timestamp = pcpu->time_in_idle_timestamp;
	expires = jiffies + usecs_to_jiffies(tunables->timer_rate);
	mod_timer_pinned(&pcpu->cpu_timer, expires);

	if (tunables->timer_slack_val >= 0 &&
	    pcpu->target_freq > pcpu->policy->min) {
		expires += usecs_to_jiffies(tunables->timer_slack_val);
		mod_timer_pinned(&pcpu->cpu_slack_timer, expires);
	}

	spin_unlock_irqrestore(&pcpu->load_lock, flags);
}

/* The caller shall take enable_sem write semaphore to avoid any timer race.
 * The cpu_timer and cpu_slack_timer must be deactivated when calling this
 * function.
 */
static void cpufreq_interactive_timer_start(
	struct cpufreq_interactive_tunables *tunables, int cpu)
{
	struct cpufreq_interactive_cpuinfo *pcpu = &per_cpu(cpuinfo, cpu);
	unsigned long expires = jiffies +
		usecs_to_jiffies(tunables->timer_rate);
	unsigned long flags;

	spin_lock_irqsave(&pcpu->load_lock, flags);

	pcpu->cpu_timer.expires = expires;
	add_timer_on(&pcpu->cpu_timer, cpu);
	if (tunables->timer_slack_val >= 0 &&
	    pcpu->target_freq > pcpu->policy->min) {
		expires += usecs_to_jiffies(tunables->timer_slack_val);
		pcpu->cpu_slack_timer.expires = expires;
		add_timer_on(&pcpu->cpu_slack_timer, cpu);
	}

	pcpu->time_in_idle =
		get_cpu_idle_time(cpu, &pcpu->time_in_idle_timestamp,
				  tunables->io_is_busy);
	pcpu->cputime_speedadj = 0;
	pcpu->cputime_speedadj_timestamp = pcpu->time_in_idle_timestamp;
#ifdef CONFIG_HUAWEI_MSG_POLICY
	pcpu->cputime_msg_timestamp = kcpustat_cpu(cpu).cpustat[CPUTIME_MESSAGE];
#endif
	spin_unlock_irqrestore(&pcpu->load_lock, flags);
}

static unsigned int freq_to_above_hispeed_delay(
	struct cpufreq_interactive_tunables *tunables,
	unsigned int freq)
{
	int i;
	unsigned int ret;
	unsigned long flags;
=======

	/* Non-zero means indefinite speed boost active */
	int boost;
	/* Duration of a boot pulse in usecs */
	int boostpulse_duration;
	/* End time of boost pulse in ktime converted to usecs */
	u64 boostpulse_endtime;
	bool boosted;

	/*
	 * Max additional time to wait in idle, beyond sampling_rate, at speeds
	 * above minimum before wakeup to reduce speed, or -1 if unnecessary.
	 */
#define DEFAULT_TIMER_SLACK (4 * DEFAULT_SAMPLING_RATE)
	unsigned long timer_slack_delay;
	unsigned long timer_slack;
	bool io_is_busy;
};

/* Separate instance required for each 'struct cpufreq_policy' */
struct interactive_policy {
	struct cpufreq_policy *policy;
	struct interactive_tunables *tunables;
	struct list_head tunables_hook;
};

/* Separate instance required for each CPU */
struct interactive_cpu {
	struct update_util_data update_util;
	struct interactive_policy *ipolicy;

	struct irq_work irq_work;
	u64 last_sample_time;
	unsigned long next_sample_jiffies;
	bool work_in_progress;

	struct rw_semaphore enable_sem;
	struct timer_list slack_timer;

	spinlock_t load_lock; /* protects the next 4 fields */
	u64 time_in_idle;
	u64 time_in_idle_timestamp;
	u64 cputime_speedadj;
	u64 cputime_speedadj_timestamp;

	spinlock_t target_freq_lock; /*protects target freq */
	unsigned int target_freq;

	unsigned int floor_freq;
	u64 pol_floor_val_time; /* policy floor_validate_time */
	u64 loc_floor_val_time; /* per-cpu floor_validate_time */
	u64 pol_hispeed_val_time; /* policy hispeed_validate_time */
	u64 loc_hispeed_val_time; /* per-cpu hispeed_validate_time */
};

static DEFINE_PER_CPU(struct interactive_cpu, interactive_cpu);

/* Realtime thread handles frequency scaling */
static struct task_struct *speedchange_task;
static cpumask_t speedchange_cpumask;
static spinlock_t speedchange_cpumask_lock;

/* Target load. Lower values result in higher CPU speeds. */
#define DEFAULT_TARGET_LOAD 90
static unsigned int default_target_loads[] = {DEFAULT_TARGET_LOAD};

#define DEFAULT_SAMPLING_RATE (20 * USEC_PER_MSEC)
#define DEFAULT_ABOVE_HISPEED_DELAY DEFAULT_SAMPLING_RATE
static unsigned int default_above_hispeed_delay[] = {
	DEFAULT_ABOVE_HISPEED_DELAY
};

/* Iterate over interactive policies for tunables */
#define for_each_ipolicy(__ip)	\
	list_for_each_entry(__ip, &tunables->attr_set.policy_list, tunables_hook)

static struct interactive_tunables *global_tunables;
static DEFINE_MUTEX(global_tunables_lock);

static inline void update_slack_delay(struct interactive_tunables *tunables)
{
	tunables->timer_slack_delay = usecs_to_jiffies(tunables->timer_slack +
						       tunables->sampling_rate);
}

static bool timer_slack_required(struct interactive_cpu *icpu)
{
	struct interactive_policy *ipolicy = icpu->ipolicy;
	struct interactive_tunables *tunables = ipolicy->tunables;

	if (tunables->timer_slack < 0)
		return false;

	if (icpu->target_freq > ipolicy->policy->min)
		return true;

	return false;
}

static void gov_slack_timer_start(struct interactive_cpu *icpu, int cpu)
{
	struct interactive_tunables *tunables = icpu->ipolicy->tunables;

	icpu->slack_timer.expires = jiffies + tunables->timer_slack_delay;
	add_timer_on(&icpu->slack_timer, cpu);
}

static void gov_slack_timer_modify(struct interactive_cpu *icpu)
{
	struct interactive_tunables *tunables = icpu->ipolicy->tunables;

	mod_timer(&icpu->slack_timer, jiffies + tunables->timer_slack_delay);
}

static void slack_timer_resched(struct interactive_cpu *icpu, int cpu,
				bool modify)
{
	struct interactive_tunables *tunables = icpu->ipolicy->tunables;
	unsigned long flags;

	spin_lock_irqsave(&icpu->load_lock, flags);

	icpu->time_in_idle = get_cpu_idle_time(cpu,
					       &icpu->time_in_idle_timestamp,
					       tunables->io_is_busy);
	icpu->cputime_speedadj = 0;
	icpu->cputime_speedadj_timestamp = icpu->time_in_idle_timestamp;

	if (timer_slack_required(icpu)) {
		if (modify)
			gov_slack_timer_modify(icpu);
		else
			gov_slack_timer_start(icpu, cpu);
	}

	spin_unlock_irqrestore(&icpu->load_lock, flags);
}

static unsigned int
freq_to_above_hispeed_delay(struct interactive_tunables *tunables,
			    unsigned int freq)
{
	unsigned long flags;
	unsigned int ret;
	int i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_irqsave(&tunables->above_hispeed_delay_lock, flags);

	for (i = 0; i < tunables->nabove_hispeed_delay - 1 &&
<<<<<<< HEAD
			freq >= tunables->above_hispeed_delay[i+1]; i += 2)
=======
	     freq >= tunables->above_hispeed_delay[i + 1]; i += 2)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		;

	ret = tunables->above_hispeed_delay[i];
	spin_unlock_irqrestore(&tunables->above_hispeed_delay_lock, flags);
<<<<<<< HEAD
	return ret;
}

static unsigned int freq_to_targetload(
	struct cpufreq_interactive_tunables *tunables, unsigned int freq)
{
	int i;
	unsigned int ret;
	unsigned long flags;
=======

	return ret;
}

static unsigned int freq_to_targetload(struct interactive_tunables *tunables,
				       unsigned int freq)
{
	unsigned long flags;
	unsigned int ret;
	int i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_irqsave(&tunables->target_loads_lock, flags);

	for (i = 0; i < tunables->ntarget_loads - 1 &&
<<<<<<< HEAD
		    freq >= tunables->target_loads[i+1]; i += 2)
=======
	     freq >= tunables->target_loads[i + 1]; i += 2)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		;

	ret = tunables->target_loads[i];
	spin_unlock_irqrestore(&tunables->target_loads_lock, flags);
	return ret;
}

/*
 * If increasing frequencies never map to a lower target load then
 * choose_freq() will find the minimum frequency that does not exceed its
 * target load given the current load.
 */
<<<<<<< HEAD
static unsigned int choose_freq(struct cpufreq_interactive_cpuinfo *pcpu,
		unsigned int loadadjfreq)
{
	unsigned int freq = pcpu->policy->cur;
	unsigned int prevfreq, freqmin, freqmax;
	unsigned int tl;
	int index;

	freqmin = 0;
	freqmax = UINT_MAX;

	do {
		prevfreq = freq;
		tl = freq_to_targetload(pcpu->policy->governor_data, freq);
=======
static unsigned int choose_freq(struct interactive_cpu *icpu,
				unsigned int loadadjfreq)
{
	struct cpufreq_policy *policy = icpu->ipolicy->policy;
	struct cpufreq_frequency_table *freq_table = policy->freq_table;
	unsigned int prevfreq, freqmin = 0, freqmax = UINT_MAX, tl;
	unsigned int freq = policy->cur;
	int index;

	do {
		prevfreq = freq;
		tl = freq_to_targetload(icpu->ipolicy->tunables, freq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/*
		 * Find the lowest frequency where the computed load is less
		 * than or equal to the target load.
		 */

<<<<<<< HEAD
		if (cpufreq_frequency_table_target(
			    pcpu->policy, pcpu->freq_table, loadadjfreq / tl,
			    CPUFREQ_RELATION_L, &index))
			break;
		freq = pcpu->freq_table[index].frequency;

		if (freq > prevfreq) {
			/* The previous frequency is too low. */
			freqmin = prevfreq;

			if (freq >= freqmax) {
				/*
				 * Find the highest frequency that is less
				 * than freqmax.
				 */
				if (cpufreq_frequency_table_target(
					    pcpu->policy, pcpu->freq_table,
					    freqmax - 1, CPUFREQ_RELATION_H,
					    &index))
					break;
				freq = pcpu->freq_table[index].frequency;

				if (freq == freqmin) {
					/*
					 * The first frequency below freqmax
					 * has already been found to be too
					 * low.  freqmax is the lowest speed
					 * we found that is fast enough.
					 */
					freq = freqmax;
					break;
				}
=======
		index = cpufreq_frequency_table_target(policy, loadadjfreq / tl,
						       CPUFREQ_RELATION_L);

		freq = freq_table[index].frequency;

		if (freq > prevfreq) {
			/* The previous frequency is too low */
			freqmin = prevfreq;

			if (freq < freqmax)
				continue;

			/* Find highest frequency that is less than freqmax */
			index = cpufreq_frequency_table_target(policy,
					freqmax - 1, CPUFREQ_RELATION_H);

			freq = freq_table[index].frequency;

			if (freq == freqmin) {
				/*
				 * The first frequency below freqmax has already
				 * been found to be too low. freqmax is the
				 * lowest speed we found that is fast enough.
				 */
				freq = freqmax;
				break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			}
		} else if (freq < prevfreq) {
			/* The previous frequency is high enough. */
			freqmax = prevfreq;

<<<<<<< HEAD
			if (freq <= freqmin) {
				/*
				 * Find the lowest frequency that is higher
				 * than freqmin.
				 */
				if (cpufreq_frequency_table_target(
					    pcpu->policy, pcpu->freq_table,
					    freqmin + 1, CPUFREQ_RELATION_L,
					    &index))
					break;
				freq = pcpu->freq_table[index].frequency;

				/*
				 * If freqmax is the first frequency above
				 * freqmin then we have already found that
				 * this speed is fast enough.
				 */
				if (freq == freqmax)
					break;
			}
=======
			if (freq > freqmin)
				continue;

			/* Find lowest frequency that is higher than freqmin */
			index = cpufreq_frequency_table_target(policy,
					freqmin + 1, CPUFREQ_RELATION_L);

			freq = freq_table[index].frequency;

			/*
			 * If freqmax is the first frequency above
			 * freqmin then we have already found that
			 * this speed is fast enough.
			 */
			if (freq == freqmax)
				break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/* If same frequency chosen as previous then done. */
	} while (freq != prevfreq);

	return freq;
}

<<<<<<< HEAD
static u64 update_load(int cpu)
{
	struct cpufreq_interactive_cpuinfo *pcpu = &per_cpu(cpuinfo, cpu);
	struct cpufreq_interactive_tunables *tunables =
		pcpu->policy->governor_data;
	u64 now;
	u64 now_idle;
	u64 delta_idle;
	u64 delta_time;
	u64 active_time;
#ifdef CONFIG_HUAWEI_MSG_POLICY
	u64 now_msg_timestamp;
#endif

	now_idle = get_cpu_idle_time(cpu, &now, tunables->io_is_busy);
	delta_idle = (now_idle - pcpu->time_in_idle);
	delta_time = (now - pcpu->time_in_idle_timestamp);
=======
static u64 update_load(struct interactive_cpu *icpu, int cpu)
{
	struct interactive_tunables *tunables = icpu->ipolicy->tunables;
	u64 now_idle, now, active_time, delta_idle, delta_time;

	now_idle = get_cpu_idle_time(cpu, &now, tunables->io_is_busy);
	delta_idle = (now_idle - icpu->time_in_idle);
	delta_time = (now - icpu->time_in_idle_timestamp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (delta_time <= delta_idle)
		active_time = 0;
	else
		active_time = delta_time - delta_idle;

<<<<<<< HEAD
#ifdef CONFIG_HUAWEI_MSG_POLICY
	now_msg_timestamp = kcpustat_cpu(cpu).cpustat[CPUTIME_MESSAGE];

	if (active_time != 0) {
		active_time = adjust_active_time_by_msg(cpu,active_time,delta_time,
			(now_msg_timestamp - pcpu->cputime_msg_timestamp));

	}
	pcpu->cputime_msg_timestamp = now_msg_timestamp;
#endif
	pcpu->cputime_speedadj += active_time * pcpu->policy->cur;

	pcpu->time_in_idle = now_idle;
	pcpu->time_in_idle_timestamp = now;
	return now;
}

static void cpufreq_interactive_timer(unsigned long data)
{
	u64 now;
	unsigned int delta_time;
	u64 cputime_speedadj;
	int cpu_load;
	struct cpufreq_interactive_cpuinfo *pcpu =
		&per_cpu(cpuinfo, data);
	struct cpufreq_interactive_tunables *tunables =
		pcpu->policy->governor_data;
	unsigned int new_freq;
	unsigned int loadadjfreq;
	unsigned int index;
	unsigned long flags;
	bool boosted;

	if (!down_read_trylock(&pcpu->enable_sem))
		return;
	if (!pcpu->governor_enabled)
		goto exit;

	spin_lock_irqsave(&pcpu->load_lock, flags);
	now = update_load(data);
	delta_time = (unsigned int)(now - pcpu->cputime_speedadj_timestamp);
	cputime_speedadj = pcpu->cputime_speedadj;
	spin_unlock_irqrestore(&pcpu->load_lock, flags);

	if (WARN_ON_ONCE(!delta_time))
		goto rearm;

	spin_lock_irqsave(&pcpu->target_freq_lock, flags);
	do_div(cputime_speedadj, delta_time);
	loadadjfreq = (unsigned int)cputime_speedadj * 100;
	cpu_load = loadadjfreq / pcpu->target_freq;
	boosted = tunables->boost_val || now < tunables->boostpulse_endtime;

#ifdef CONFIG_HISI_HMPTH_OPEN
	if (tunables->hmp_boosted && !boosted) {
		set_hmp_policy(HMP_NAME, HMP_PRIO, HMP_OFF, tunables->boost_hmp_upthreshold,
				tunables->boost_hmp_downthreshold);
		tunables->hmp_boosted = false;
	}
#endif

	if (cpu_load >= tunables->go_hispeed_load || boosted) {
		if (pcpu->target_freq < tunables->hispeed_freq) {
			new_freq = tunables->hispeed_freq;
		} else {
			new_freq = choose_freq(pcpu, loadadjfreq);
=======
	icpu->cputime_speedadj += active_time * icpu->ipolicy->policy->cur;

	icpu->time_in_idle = now_idle;
	icpu->time_in_idle_timestamp = now;

	return now;
}

/* Re-evaluate load to see if a frequency change is required or not */
static void eval_target_freq(struct interactive_cpu *icpu)
{
	struct interactive_tunables *tunables = icpu->ipolicy->tunables;
	struct cpufreq_policy *policy = icpu->ipolicy->policy;
	struct cpufreq_frequency_table *freq_table = policy->freq_table;
	u64 cputime_speedadj, now, max_fvtime;
	unsigned int new_freq, loadadjfreq, index, delta_time;
	unsigned long flags;
	int cpu_load;
	int cpu = smp_processor_id();

	spin_lock_irqsave(&icpu->load_lock, flags);
	now = update_load(icpu, smp_processor_id());
	delta_time = (unsigned int)(now - icpu->cputime_speedadj_timestamp);
	cputime_speedadj = icpu->cputime_speedadj;
	spin_unlock_irqrestore(&icpu->load_lock, flags);

	if (WARN_ON_ONCE(!delta_time))
		return;

	spin_lock_irqsave(&icpu->target_freq_lock, flags);
	do_div(cputime_speedadj, delta_time);
	loadadjfreq = (unsigned int)cputime_speedadj * 100;
	cpu_load = loadadjfreq / policy->cur;
	tunables->boosted = tunables->boost ||
			    now < tunables->boostpulse_endtime;

	if (cpu_load >= tunables->go_hispeed_load || tunables->boosted) {
		if (policy->cur < tunables->hispeed_freq) {
			new_freq = tunables->hispeed_freq;
		} else {
			new_freq = choose_freq(icpu, loadadjfreq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			if (new_freq < tunables->hispeed_freq)
				new_freq = tunables->hispeed_freq;
		}
	} else {
<<<<<<< HEAD
		new_freq = choose_freq(pcpu, loadadjfreq);
		if (new_freq > tunables->hispeed_freq &&
				pcpu->target_freq < tunables->hispeed_freq)
			new_freq = tunables->hispeed_freq;
	}

	if (pcpu->target_freq >= tunables->hispeed_freq &&
	    new_freq > pcpu->target_freq &&
	    now - pcpu->hispeed_validate_time <
	    freq_to_above_hispeed_delay(tunables, pcpu->target_freq)) {
		trace_cpufreq_interactive_notyet(
			data, cpu_load, pcpu->target_freq,
			pcpu->policy->cur, new_freq);
		spin_unlock_irqrestore(&pcpu->target_freq_lock, flags);
		goto rearm;
	}

	pcpu->hispeed_validate_time = now;

	if (cpufreq_frequency_table_target(pcpu->policy, pcpu->freq_table,
					   new_freq, CPUFREQ_RELATION_L,
					   &index)) {
		spin_unlock_irqrestore(&pcpu->target_freq_lock, flags);
		goto rearm;
	}

	new_freq = pcpu->freq_table[index].frequency;
=======
		new_freq = choose_freq(icpu, loadadjfreq);
		if (new_freq > tunables->hispeed_freq &&
		    policy->cur < tunables->hispeed_freq)
			new_freq = tunables->hispeed_freq;
	}

	if (policy->cur >= tunables->hispeed_freq &&
	    new_freq > policy->cur &&
	    now - icpu->pol_hispeed_val_time < freq_to_above_hispeed_delay(tunables, policy->cur)) {
		trace_cpufreq_interactive_notyet(cpu, cpu_load,
				icpu->target_freq, policy->cur, new_freq);
		goto exit;
	}

	icpu->loc_hispeed_val_time = now;

	index = cpufreq_frequency_table_target(policy, new_freq,
					       CPUFREQ_RELATION_L);
	new_freq = freq_table[index].frequency;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Do not scale below floor_freq unless we have been at or above the
	 * floor frequency for the minimum sample time since last validated.
	 */
<<<<<<< HEAD
	if (new_freq < pcpu->floor_freq) {
		if (now - pcpu->floor_validate_time <
				tunables->min_sample_time) {
			trace_cpufreq_interactive_notyet(
				data, cpu_load, pcpu->target_freq,
				pcpu->policy->cur, new_freq);
			spin_unlock_irqrestore(&pcpu->target_freq_lock, flags);
			goto rearm;
=======
	max_fvtime = max(icpu->pol_floor_val_time, icpu->loc_floor_val_time);
	if (new_freq < icpu->floor_freq && icpu->target_freq >= policy->cur) {
		if (now - max_fvtime < tunables->min_sample_time) {
			trace_cpufreq_interactive_notyet(cpu, cpu_load,
				icpu->target_freq, policy->cur, new_freq);
			goto exit;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	/*
	 * Update the timestamp for checking whether speed has been held at
	 * or above the selected frequency for a minimum of min_sample_time,
	 * if not boosted to hispeed_freq.  If boosted to hispeed_freq then we
	 * allow the speed to drop as soon as the boostpulse duration expires
	 * (or the indefinite boost is turned off).
	 */

<<<<<<< HEAD
	if (!boosted || new_freq > tunables->hispeed_freq) {
		pcpu->floor_freq = new_freq;
		pcpu->floor_validate_time = now;
	}

	if (pcpu->target_freq == new_freq) {
		trace_cpufreq_interactive_already(
			data, cpu_load, pcpu->target_freq,
			pcpu->policy->cur, new_freq);
		spin_unlock_irqrestore(&pcpu->target_freq_lock, flags);
		goto rearm_if_notmax;
	}

	trace_cpufreq_interactive_target(data, cpu_load, pcpu->target_freq,
					 pcpu->policy->cur, new_freq);

	pcpu->target_freq = new_freq;
	spin_unlock_irqrestore(&pcpu->target_freq_lock, flags);
	spin_lock_irqsave(&speedchange_cpumask_lock, flags);
	cpumask_set_cpu(data, &speedchange_cpumask);
	spin_unlock_irqrestore(&speedchange_cpumask_lock, flags);
	wake_up_process(speedchange_task);

rearm_if_notmax:
	/*
	 * Already set max speed and don't see a need to change that,
	 * wait until next idle to re-evaluate, don't need timer.
	 */
	if (pcpu->target_freq == pcpu->policy->max)
		goto exit;

rearm:
	if (!timer_pending(&pcpu->cpu_timer))
		cpufreq_interactive_timer_resched(pcpu);

exit:
	up_read(&pcpu->enable_sem);
	return;
}

static void cpufreq_interactive_idle_start(void)
{
	struct cpufreq_interactive_cpuinfo *pcpu =
		&per_cpu(cpuinfo, smp_processor_id());
	int pending;

	if (!down_read_trylock(&pcpu->enable_sem))
		return;
	if (!pcpu->governor_enabled) {
		up_read(&pcpu->enable_sem);
		return;
	}

	pending = timer_pending(&pcpu->cpu_timer);

	if (pcpu->target_freq != pcpu->policy->min) {
		/*
		 * Entering idle while not at lowest speed.  On some
		 * platforms this can hold the other CPU(s) at that speed
		 * even though the CPU is idle. Set a timer to re-evaluate
		 * speed so this idle CPU doesn't hold the other CPUs above
		 * min indefinitely.  This should probably be a quirk of
		 * the CPUFreq driver.
		 */
		if (!pending)
			cpufreq_interactive_timer_resched(pcpu);
	}

	up_read(&pcpu->enable_sem);
}

static void cpufreq_interactive_idle_end(void)
{
	struct cpufreq_interactive_cpuinfo *pcpu =
		&per_cpu(cpuinfo, smp_processor_id());

	if (!down_read_trylock(&pcpu->enable_sem))
		return;
	if (!pcpu->governor_enabled) {
		up_read(&pcpu->enable_sem);
		return;
	}

	/* Arm the timer for 1-2 ticks later if not already. */
	if (!timer_pending(&pcpu->cpu_timer)) {
		cpufreq_interactive_timer_resched(pcpu);
	} else if (time_after_eq(jiffies, pcpu->cpu_timer.expires)) {
		del_timer(&pcpu->cpu_timer);
		del_timer(&pcpu->cpu_slack_timer);
		cpufreq_interactive_timer(smp_processor_id());
	}

	up_read(&pcpu->enable_sem);
=======
	if (!tunables->boosted || new_freq > tunables->hispeed_freq) {
		icpu->floor_freq = new_freq;
		if (icpu->target_freq >= policy->cur || new_freq >= policy->cur)
			icpu->loc_floor_val_time = now;
	}

	if (icpu->target_freq == new_freq &&
	    icpu->target_freq <= policy->cur) {
		trace_cpufreq_interactive_already(cpu, cpu_load,
			icpu->target_freq, policy->cur, new_freq);
		goto exit;
	}

	trace_cpufreq_interactive_target(cpu, cpu_load, icpu->target_freq,
					 policy->cur, new_freq);

	icpu->target_freq = new_freq;
	spin_unlock_irqrestore(&icpu->target_freq_lock, flags);

	spin_lock_irqsave(&speedchange_cpumask_lock, flags);
	cpumask_set_cpu(cpu, &speedchange_cpumask);
	spin_unlock_irqrestore(&speedchange_cpumask_lock, flags);

	wake_up_process(speedchange_task);
	return;

exit:
	spin_unlock_irqrestore(&icpu->target_freq_lock, flags);
}

static void cpufreq_interactive_update(struct interactive_cpu *icpu)
{
	eval_target_freq(icpu);
	slack_timer_resched(icpu, smp_processor_id(), true);
}

static void cpufreq_interactive_idle_end(void)
{
	struct interactive_cpu *icpu = &per_cpu(interactive_cpu,
						smp_processor_id());

	if (!down_read_trylock(&icpu->enable_sem))
		return;

	if (icpu->ipolicy) {
		/*
		 * We haven't sampled load for more than sampling_rate time, do
		 * it right now.
		 */
		if (time_after_eq(jiffies, icpu->next_sample_jiffies))
			cpufreq_interactive_update(icpu);
	}

	up_read(&icpu->enable_sem);
}

static void cpufreq_interactive_get_policy_info(struct cpufreq_policy *policy,
						unsigned int *pmax_freq,
						u64 *phvt, u64 *pfvt)
{
	struct interactive_cpu *icpu;
	u64 hvt = ~0ULL, fvt = 0;
	unsigned int max_freq = 0, i;

	for_each_cpu(i, policy->cpus) {
		icpu = &per_cpu(interactive_cpu, i);

		fvt = max(fvt, icpu->loc_floor_val_time);
		if (icpu->target_freq > max_freq) {
			max_freq = icpu->target_freq;
			hvt = icpu->loc_hispeed_val_time;
		} else if (icpu->target_freq == max_freq) {
			hvt = min(hvt, icpu->loc_hispeed_val_time);
		}
	}

	*pmax_freq = max_freq;
	*phvt = hvt;
	*pfvt = fvt;
}

static void cpufreq_interactive_adjust_cpu(unsigned int cpu,
					   struct cpufreq_policy *policy)
{
	struct interactive_cpu *icpu;
	u64 hvt, fvt;
	unsigned int max_freq;
	int i;

	cpufreq_interactive_get_policy_info(policy, &max_freq, &hvt, &fvt);

	for_each_cpu(i, policy->cpus) {
		icpu = &per_cpu(interactive_cpu, i);
		icpu->pol_floor_val_time = fvt;
	}

	if (max_freq != policy->cur) {
		__cpufreq_driver_target(policy, max_freq, CPUFREQ_RELATION_H);
		for_each_cpu(i, policy->cpus) {
			icpu = &per_cpu(interactive_cpu, i);
			icpu->pol_hispeed_val_time = hvt;
		}
	}

	trace_cpufreq_interactive_setspeed(cpu, max_freq, policy->cur);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int cpufreq_interactive_speedchange_task(void *data)
{
	unsigned int cpu;
	cpumask_t tmp_mask;
	unsigned long flags;
<<<<<<< HEAD
	struct cpufreq_interactive_cpuinfo *pcpu;

	while (1) {
		set_current_state(TASK_INTERRUPTIBLE);
		spin_lock_irqsave(&speedchange_cpumask_lock, flags);

		if (cpumask_empty(&speedchange_cpumask)) {
			spin_unlock_irqrestore(&speedchange_cpumask_lock,
					       flags);
			schedule();

			if (kthread_should_stop())
				break;

			spin_lock_irqsave(&speedchange_cpumask_lock, flags);
		}

		set_current_state(TASK_RUNNING);
		tmp_mask = speedchange_cpumask;
		cpumask_clear(&speedchange_cpumask);
		spin_unlock_irqrestore(&speedchange_cpumask_lock, flags);

		for_each_cpu(cpu, &tmp_mask) {
			unsigned int j;
			unsigned int max_freq = 0;

			pcpu = &per_cpu(cpuinfo, cpu);
			if (!down_read_trylock(&pcpu->enable_sem))
				continue;
			if (!pcpu->governor_enabled) {
				up_read(&pcpu->enable_sem);
				continue;
			}

			for_each_cpu(j, pcpu->policy->cpus) {
				struct cpufreq_interactive_cpuinfo *pjcpu =
					&per_cpu(cpuinfo, j);

				if (pjcpu->target_freq > max_freq)
					max_freq = pjcpu->target_freq;
			}

			if (max_freq != pcpu->policy->cur)
				__cpufreq_driver_target(pcpu->policy,
							max_freq,
							CPUFREQ_RELATION_H);
			trace_cpufreq_interactive_setspeed(cpu,
						     pcpu->target_freq,
						     pcpu->policy->cur);

			up_read(&pcpu->enable_sem);
		}
	}

	return 0;
}

static void cpufreq_interactive_boost(void)
{
	int i;
	int anyboost = 0;
	unsigned long flags[2];
	struct cpufreq_interactive_cpuinfo *pcpu;
	struct cpufreq_interactive_tunables *tunables;

	spin_lock_irqsave(&speedchange_cpumask_lock, flags[0]);

	for_each_online_cpu(i) {
		pcpu = &per_cpu(cpuinfo, i);
		if (!pcpu->governor_enabled)
			continue;

		tunables = pcpu->policy->governor_data;

		spin_lock_irqsave(&pcpu->target_freq_lock, flags[1]);
		if (pcpu->target_freq < tunables->hispeed_freq) {
			pcpu->target_freq = tunables->hispeed_freq;
			cpumask_set_cpu(i, &speedchange_cpumask);
			pcpu->hispeed_validate_time =
				ktime_to_us(ktime_get());
			anyboost = 1;
		}

		/*
		 * Set floor freq and (re)start timer for when last
		 * validated.
		 */

		pcpu->floor_freq = tunables->hispeed_freq;
		pcpu->floor_validate_time = ktime_to_us(ktime_get());
		spin_unlock_irqrestore(&pcpu->target_freq_lock, flags[1]);
	}

	spin_unlock_irqrestore(&speedchange_cpumask_lock, flags[0]);

	if (anyboost)
		wake_up_process(speedchange_task);
}

#ifdef CONFIG_ARCH_HI3XXX
#define MAX_LITTLE_CPU_NR	4

int hisi_little_cluster_boost(void)
{
	int i;
	int anyboost = 0;
	unsigned long flags;
	struct cpufreq_interactive_cpuinfo *pcpu;
	struct cpufreq_interactive_tunables *tunables;

	if (!cpufreq_interactive_initialized)
		return -ENODEV;

	spin_lock_irqsave(&speedchange_cpumask_lock, flags);

	for_each_online_cpu(i) {
		if (i == MAX_LITTLE_CPU_NR)
			break;

		pcpu = &per_cpu(cpuinfo, i);
		if (!pcpu->governor_enabled)
			continue;

		tunables = pcpu->policy->governor_data;

		if (pcpu->target_freq < tunables->hispeed_freq) {
			pcpu->target_freq = tunables->hispeed_freq;
			cpumask_set_cpu(i, &speedchange_cpumask);
			pcpu->hispeed_validate_time =
				ktime_to_us(ktime_get());
			anyboost = 1;
		}

		/*
		 * Set floor freq and (re)start timer for when last
		 * validated.
		 */

		pcpu->floor_freq = tunables->hispeed_freq;
		pcpu->floor_validate_time = ktime_to_us(ktime_get());
	}

	spin_unlock_irqrestore(&speedchange_cpumask_lock, flags);

	if (anyboost)
		wake_up_process(speedchange_task);

	return 0;
}
EXPORT_SYMBOL(hisi_little_cluster_boost);
#endif

static int cpufreq_interactive_notifier(
	struct notifier_block *nb, unsigned long val, void *data)
{
	struct cpufreq_freqs *freq = data;
	struct cpufreq_interactive_cpuinfo *pcpu;
	int cpu;
	unsigned long flags;

	if (val == CPUFREQ_POSTCHANGE) {
		pcpu = &per_cpu(cpuinfo, freq->cpu);
		if (!down_read_trylock(&pcpu->enable_sem))
			return 0;
		if (!pcpu->governor_enabled) {
			up_read(&pcpu->enable_sem);
			return 0;
		}

		for_each_cpu(cpu, pcpu->policy->cpus) {
			struct cpufreq_interactive_cpuinfo *pjcpu =
				&per_cpu(cpuinfo, cpu);
			if (cpu != freq->cpu) {
				if (!down_read_trylock(&pjcpu->enable_sem))
					continue;
				if (!pjcpu->governor_enabled) {
					up_read(&pjcpu->enable_sem);
					continue;
				}
			}
			spin_lock_irqsave(&pjcpu->load_lock, flags);
			update_load(cpu);
			spin_unlock_irqrestore(&pjcpu->load_lock, flags);
			if (cpu != freq->cpu)
				up_read(&pjcpu->enable_sem);
		}

		up_read(&pcpu->enable_sem);
	}
	return 0;
}

static struct notifier_block cpufreq_notifier_block = {
	.notifier_call = cpufreq_interactive_notifier,
};


#ifdef CONFIG_INPUT_PULSE_SUPPORT
/*
 * Pulsed boost on input event raises CPUs to hispeed_freq and lets
 * usual algorithm of min_sample_time  decide when to allow speed
 * to drop.
 */

static void cpufreq_interactive_input_event(struct input_handle *handle,
					    unsigned int type,
					    unsigned int code, int value)
{
	struct cpufreq_policy *policy;
	struct cpufreq_interactive_tunables *tunables;
	unsigned int cpu = smp_processor_id();

	if (type == EV_SYN && code == SYN_REPORT) {
		trace_cpufreq_interactive_boost("input");

		policy = cpufreq_cpu_get(cpu);
		if (policy != NULL) {
			tunables = policy->governor_data;
			tunables->boostpulse_endtime = ktime_to_us(ktime_get()) +
				tunables->boostpulse_duration_val;

			cpufreq_interactive_boost();
			cpufreq_cpu_put(policy);
		}
	}
}

static void cpufreq_interactive_input_open(struct work_struct *w)
{
	struct cpufreq_interactive_inputopen *io =
		container_of(w, struct cpufreq_interactive_inputopen,
			     inputopen_work);
	int error;

	error = input_open_device(io->handle);
	if (error)
		input_unregister_handle(io->handle);
}

static int cpufreq_interactive_input_connect(struct input_handler *handler,
					     struct input_dev *dev,
					     const struct input_device_id *id)
{
	struct input_handle *handle;
	int error;

	pr_info("%s: connect to %s\n", __func__, dev->name);
	handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
	if (!handle)
		return -ENOMEM;

	handle->dev = dev;
	handle->handler = handler;
	handle->name = "cpufreq_interactive";

	error = input_register_handle(handle);
	if (error)
		goto err;

	inputopen.handle = handle;
	queue_work(down_wq, &inputopen.inputopen_work);
	return 0;
err:
	kfree(handle);
	return error;
}

static void cpufreq_interactive_input_disconnect(struct input_handle *handle)
{
	input_close_device(handle);
	input_unregister_handle(handle);
	kfree(handle);
}

static const struct input_device_id cpufreq_interactive_ids[] = {
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT |
			 INPUT_DEVICE_ID_MATCH_ABSBIT,
		.evbit = { BIT_MASK(EV_ABS) },
		.absbit = { [BIT_WORD(ABS_MT_POSITION_X)] =
			    BIT_MASK(ABS_MT_POSITION_X) |
			    BIT_MASK(ABS_MT_POSITION_Y) },
	}, /* multi-touch touchscreen */
	{
		.flags = INPUT_DEVICE_ID_MATCH_KEYBIT |
			 INPUT_DEVICE_ID_MATCH_ABSBIT,
		.keybit = { [BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH) },
		.absbit = { [BIT_WORD(ABS_X)] =
			    BIT_MASK(ABS_X) | BIT_MASK(ABS_Y) },
	}, /* touchpad */
	{ },
};

static struct input_handler cpufreq_interactive_input_handler = {
	.event          = cpufreq_interactive_input_event,
	.connect        = cpufreq_interactive_input_connect,
	.disconnect     = cpufreq_interactive_input_disconnect,
	.name           = "cpufreq_interactive",
	.id_table       = cpufreq_interactive_ids,
};

#endif /*CONFIG_INPUT_PULSE_SUPPORT*/

static unsigned int *get_tokenized_data(const char *buf, int *num_tokens)
{
	const char *cp;
	int i;
	int ntokens = 1;
	unsigned int *tokenized_data;
	int err = -EINVAL;

	cp = buf;
=======

again:
	set_current_state(TASK_INTERRUPTIBLE);
	spin_lock_irqsave(&speedchange_cpumask_lock, flags);

	if (cpumask_empty(&speedchange_cpumask)) {
		spin_unlock_irqrestore(&speedchange_cpumask_lock, flags);
		schedule();

		if (kthread_should_stop())
			return 0;

		spin_lock_irqsave(&speedchange_cpumask_lock, flags);
	}

	set_current_state(TASK_RUNNING);
	tmp_mask = speedchange_cpumask;
	cpumask_clear(&speedchange_cpumask);
	spin_unlock_irqrestore(&speedchange_cpumask_lock, flags);

	for_each_cpu(cpu, &tmp_mask) {
		struct interactive_cpu *icpu = &per_cpu(interactive_cpu, cpu);
		struct cpufreq_policy *policy;

		if (unlikely(!down_read_trylock(&icpu->enable_sem)))
			continue;

		if (likely(icpu->ipolicy)) {
			policy = icpu->ipolicy->policy;
			cpufreq_interactive_adjust_cpu(cpu, policy);
		}

		up_read(&icpu->enable_sem);
	}

	goto again;
}

static void cpufreq_interactive_boost(struct interactive_tunables *tunables)
{
	struct interactive_policy *ipolicy;
	struct cpufreq_policy *policy;
	struct interactive_cpu *icpu;
	unsigned long flags[2];
	bool wakeup = false;
	int i;

	tunables->boosted = true;

	spin_lock_irqsave(&speedchange_cpumask_lock, flags[0]);

	for_each_ipolicy(ipolicy) {
		policy = ipolicy->policy;

		for_each_cpu(i, policy->cpus) {
			icpu = &per_cpu(interactive_cpu, i);

			if (!down_read_trylock(&icpu->enable_sem))
				continue;

			if (!icpu->ipolicy) {
				up_read(&icpu->enable_sem);
				continue;
			}

			spin_lock_irqsave(&icpu->target_freq_lock, flags[1]);
			if (icpu->target_freq < tunables->hispeed_freq) {
				icpu->target_freq = tunables->hispeed_freq;
				cpumask_set_cpu(i, &speedchange_cpumask);
				icpu->pol_hispeed_val_time = ktime_to_us(ktime_get());
				wakeup = true;
			}
			spin_unlock_irqrestore(&icpu->target_freq_lock, flags[1]);

			up_read(&icpu->enable_sem);
		}
	}

	spin_unlock_irqrestore(&speedchange_cpumask_lock, flags[0]);

	if (wakeup)
		wake_up_process(speedchange_task);
}

static int cpufreq_interactive_notifier(struct notifier_block *nb,
					unsigned long val, void *data)
{
	struct cpufreq_freqs *freq = data;
	struct interactive_cpu *icpu = &per_cpu(interactive_cpu, freq->cpu);
	unsigned long flags;

	if (val != CPUFREQ_POSTCHANGE)
		return 0;

	if (!down_read_trylock(&icpu->enable_sem))
		return 0;

	if (!icpu->ipolicy) {
		up_read(&icpu->enable_sem);
		return 0;
	}

	spin_lock_irqsave(&icpu->load_lock, flags);
	update_load(icpu, freq->cpu);
	spin_unlock_irqrestore(&icpu->load_lock, flags);

	up_read(&icpu->enable_sem);

	return 0;
}

static struct notifier_block cpufreq_notifier_block = {
	.notifier_call = cpufreq_interactive_notifier,
};

static unsigned int *get_tokenized_data(const char *buf, int *num_tokens)
{
	const char *cp = buf;
	int ntokens = 1, i = 0;
	unsigned int *tokenized_data;
	int err = -EINVAL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	while ((cp = strpbrk(cp + 1, " :")))
		ntokens++;

	if (!(ntokens & 0x1))
		goto err;

<<<<<<< HEAD
	tokenized_data = kmalloc(ntokens * sizeof(unsigned int), GFP_KERNEL);
=======
	tokenized_data = kcalloc(ntokens, sizeof(*tokenized_data), GFP_KERNEL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!tokenized_data) {
		err = -ENOMEM;
		goto err;
	}

	cp = buf;
<<<<<<< HEAD
	i = 0;
	while (i < ntokens) {
		if (sscanf(cp, "%u", &tokenized_data[i++]) != 1)/*[false alarm]*/
=======
	while (i < ntokens) {
		if (kstrtouint(cp, 0, &tokenized_data[i++]) < 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto err_kfree;

		cp = strpbrk(cp, " :");
		if (!cp)
			break;
		cp++;
	}

	if (i != ntokens)
		goto err_kfree;

	*num_tokens = ntokens;
	return tokenized_data;

err_kfree:
	kfree(tokenized_data);
err:
	return ERR_PTR(err);
}

<<<<<<< HEAD
static ssize_t show_target_loads(
	struct cpufreq_interactive_tunables *tunables,
	char *buf)
{
	int i;
	ssize_t ret = 0;
	unsigned long flags;
=======
/* Interactive governor sysfs interface */
static struct interactive_tunables *to_tunables(struct gov_attr_set *attr_set)
{
	return container_of(attr_set, struct interactive_tunables, attr_set);
}

#define show_one(file_name, type)					\
static ssize_t show_##file_name(struct gov_attr_set *attr_set, char *buf) \
{									\
	struct interactive_tunables *tunables = to_tunables(attr_set);	\
	return sprintf(buf, type "\n", tunables->file_name);		\
}

static ssize_t show_target_loads(struct gov_attr_set *attr_set, char *buf)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long flags;
	ssize_t ret = 0;
	int i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_irqsave(&tunables->target_loads_lock, flags);

	for (i = 0; i < tunables->ntarget_loads; i++)
		ret += sprintf(buf + ret, "%u%s", tunables->target_loads[i],
			       i & 0x1 ? ":" : " ");

	sprintf(buf + ret - 1, "\n");
	spin_unlock_irqrestore(&tunables->target_loads_lock, flags);
<<<<<<< HEAD
	return ret;
}

static ssize_t store_target_loads(
	struct cpufreq_interactive_tunables *tunables,
	const char *buf, size_t count)
{
	int ntokens;
	unsigned int *new_target_loads = NULL;
	unsigned long flags;

	new_target_loads = get_tokenized_data(buf, &ntokens);
	if (IS_ERR(new_target_loads))
		return PTR_RET(new_target_loads);
=======

	return ret;
}

static ssize_t store_target_loads(struct gov_attr_set *attr_set,
				  const char *buf, size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned int *new_target_loads;
	unsigned long flags;
	int ntokens;

	new_target_loads = get_tokenized_data(buf, &ntokens);
	if (IS_ERR(new_target_loads))
		return PTR_ERR(new_target_loads);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_irqsave(&tunables->target_loads_lock, flags);
	if (tunables->target_loads != default_target_loads)
		kfree(tunables->target_loads);
	tunables->target_loads = new_target_loads;
	tunables->ntarget_loads = ntokens;
	spin_unlock_irqrestore(&tunables->target_loads_lock, flags);
<<<<<<< HEAD
	return count;
}

static ssize_t show_above_hispeed_delay(
	struct cpufreq_interactive_tunables *tunables, char *buf)
{
	int i;
	ssize_t ret = 0;
	unsigned long flags;
=======

	return count;
}

static ssize_t show_above_hispeed_delay(struct gov_attr_set *attr_set,
					char *buf)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long flags;
	ssize_t ret = 0;
	int i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_irqsave(&tunables->above_hispeed_delay_lock, flags);

	for (i = 0; i < tunables->nabove_hispeed_delay; i++)
		ret += sprintf(buf + ret, "%u%s",
			       tunables->above_hispeed_delay[i],
			       i & 0x1 ? ":" : " ");

	sprintf(buf + ret - 1, "\n");
	spin_unlock_irqrestore(&tunables->above_hispeed_delay_lock, flags);
<<<<<<< HEAD
	return ret;
}

static ssize_t store_above_hispeed_delay(
	struct cpufreq_interactive_tunables *tunables,
	const char *buf, size_t count)
{
	int ntokens;
	unsigned int *new_above_hispeed_delay = NULL;
	unsigned long flags;

	new_above_hispeed_delay = get_tokenized_data(buf, &ntokens);
	if (IS_ERR(new_above_hispeed_delay))
		return PTR_RET(new_above_hispeed_delay);
=======

	return ret;
}

static ssize_t store_above_hispeed_delay(struct gov_attr_set *attr_set,
					 const char *buf, size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned int *new_above_hispeed_delay = NULL;
	unsigned long flags;
	int ntokens;

	new_above_hispeed_delay = get_tokenized_data(buf, &ntokens);
	if (IS_ERR(new_above_hispeed_delay))
		return PTR_ERR(new_above_hispeed_delay);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_irqsave(&tunables->above_hispeed_delay_lock, flags);
	if (tunables->above_hispeed_delay != default_above_hispeed_delay)
		kfree(tunables->above_hispeed_delay);
	tunables->above_hispeed_delay = new_above_hispeed_delay;
	tunables->nabove_hispeed_delay = ntokens;
	spin_unlock_irqrestore(&tunables->above_hispeed_delay_lock, flags);
<<<<<<< HEAD
	return count;

}

static ssize_t show_hispeed_freq(struct cpufreq_interactive_tunables *tunables,
		char *buf)
{
	return sprintf(buf, "%u\n", tunables->hispeed_freq);
}

static ssize_t store_hispeed_freq(struct cpufreq_interactive_tunables *tunables,
		const char *buf, size_t count)
{
	int ret;
	long unsigned int val;

	ret = strict_strtoul(buf, 0, &val);
	if (ret < 0)
		return ret;
	tunables->hispeed_freq = val;
	return count;
}

static ssize_t show_go_hispeed_load(struct cpufreq_interactive_tunables
		*tunables, char *buf)
{
	return sprintf(buf, "%lu\n", tunables->go_hispeed_load);
}

static ssize_t store_go_hispeed_load(struct cpufreq_interactive_tunables
		*tunables, const char *buf, size_t count)
{
	int ret;
	unsigned long val;

	ret = strict_strtoul(buf, 0, &val);
	if (ret < 0)
		return ret;
	tunables->go_hispeed_load = val;
	return count;
}

static ssize_t show_min_sample_time(struct cpufreq_interactive_tunables
		*tunables, char *buf)
{
	return sprintf(buf, "%lu\n", tunables->min_sample_time);
}

static ssize_t store_min_sample_time(struct cpufreq_interactive_tunables
		*tunables, const char *buf, size_t count)
{
	int ret;
	unsigned long val;

	ret = strict_strtoul(buf, 0, &val);
	if (ret < 0)
		return ret;
	tunables->min_sample_time = val;
	return count;
}

static ssize_t show_timer_rate(struct cpufreq_interactive_tunables *tunables,
		char *buf)
{
	return sprintf(buf, "%lu\n", tunables->timer_rate);
}

static ssize_t store_timer_rate(struct cpufreq_interactive_tunables *tunables,
		const char *buf, size_t count)
{
	int ret;
	unsigned long val;

	ret = strict_strtoul(buf, 0, &val);
	if (ret < 0)
		return ret;
	tunables->timer_rate = val;
	return count;
}

static ssize_t show_timer_slack(struct cpufreq_interactive_tunables *tunables,
		char *buf)
{
	return sprintf(buf, "%d\n", tunables->timer_slack_val);
}

static ssize_t store_timer_slack(struct cpufreq_interactive_tunables *tunables,
		const char *buf, size_t count)
{
	int ret;
	unsigned long val;
=======

	return count;
}

static ssize_t store_hispeed_freq(struct gov_attr_set *attr_set,
				  const char *buf, size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long int val;
	int ret;

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;

	tunables->hispeed_freq = val;

	return count;
}

static ssize_t store_go_hispeed_load(struct gov_attr_set *attr_set,
				     const char *buf, size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long val;
	int ret;

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;

	tunables->go_hispeed_load = val;

	return count;
}

static ssize_t store_min_sample_time(struct gov_attr_set *attr_set,
				     const char *buf, size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long val;
	int ret;

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;

	tunables->min_sample_time = val;

	return count;
}

static ssize_t show_timer_rate(struct gov_attr_set *attr_set, char *buf)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);

	return sprintf(buf, "%lu\n", tunables->sampling_rate);
}

static ssize_t store_timer_rate(struct gov_attr_set *attr_set, const char *buf,
				size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long val, val_round;
	int ret;

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;

	val_round = jiffies_to_usecs(usecs_to_jiffies(val));
	if (val != val_round)
		pr_warn("timer_rate not aligned to jiffy. Rounded up to %lu\n",
			val_round);

	tunables->sampling_rate = val_round;

	return count;
}

static ssize_t store_timer_slack(struct gov_attr_set *attr_set, const char *buf,
				 size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long val;
	int ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = kstrtol(buf, 10, &val);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	tunables->timer_slack_val = val;
	return count;
}

static ssize_t show_boost(struct cpufreq_interactive_tunables *tunables,
			  char *buf)
{
	return sprintf(buf, "%d\n", tunables->boost_val);
}

static ssize_t store_boost(struct cpufreq_interactive_tunables *tunables,
			   const char *buf, size_t count)
{
	int ret;
	unsigned long val;
=======
	tunables->timer_slack = val;
	update_slack_delay(tunables);

	return count;
}

static ssize_t store_boost(struct gov_attr_set *attr_set, const char *buf,
			   size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long val;
	int ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	tunables->boost_val = val;

	if (tunables->boost_val) {
		trace_cpufreq_interactive_boost("on");
		cpufreq_interactive_boost();
=======
	tunables->boost = val;

	if (tunables->boost) {
		trace_cpufreq_interactive_boost("on");
		if (!tunables->boosted)
			cpufreq_interactive_boost(tunables);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else {
		tunables->boostpulse_endtime = ktime_to_us(ktime_get());
		trace_cpufreq_interactive_unboost("off");
	}

	return count;
}

<<<<<<< HEAD
static ssize_t store_boostpulse(struct cpufreq_interactive_tunables *tunables,
				const char *buf, size_t count)
{
	int ret;
	unsigned long val;
=======
static ssize_t store_boostpulse(struct gov_attr_set *attr_set, const char *buf,
				size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long val;
	int ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;

	tunables->boostpulse_endtime = ktime_to_us(ktime_get()) +
<<<<<<< HEAD
		tunables->boostpulse_duration_val;
	trace_cpufreq_interactive_boost("pulse");
#ifdef CONFIG_HISI_HMPTH_OPEN
		if(tunables->boost_hmp_val && !tunables->hmp_boosted) {
			tunables->hmp_boosted = true;
			set_hmp_policy(HMP_NAME, HMP_PRIO, HMP_ON, tunables->boost_hmp_upthreshold,
					tunables->boost_hmp_downthreshold);
		}
#endif
	cpufreq_interactive_boost();
	return count;
}

static ssize_t show_boostpulse_duration(struct cpufreq_interactive_tunables
		*tunables, char *buf)
{
	return sprintf(buf, "%d\n", tunables->boostpulse_duration_val);
}

static ssize_t store_boostpulse_duration(struct cpufreq_interactive_tunables
		*tunables, const char *buf, size_t count)
{
	int ret;
	unsigned long val;
=======
					tunables->boostpulse_duration;
	trace_cpufreq_interactive_boost("pulse");
	if (!tunables->boosted)
		cpufreq_interactive_boost(tunables);

	return count;
}

static ssize_t store_boostpulse_duration(struct gov_attr_set *attr_set,
					 const char *buf, size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long val;
	int ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	tunables->boostpulse_duration_val = val;
	return count;
}
#ifdef CONFIG_ARCH_HI3XXX
static ssize_t show_boostpulse_min_interval(struct cpufreq_interactive_tunables
		*tunables, char *buf)
{
	return sprintf(buf, "%d\n", tunables->boostpulse_min_interval);
}

static ssize_t store_boostpulse_min_interval(struct cpufreq_interactive_tunables
		*tunables, const char *buf, size_t count)
{
	int ret;
	unsigned long val;
=======
	tunables->boostpulse_duration = val;

	return count;
}

static ssize_t store_io_is_busy(struct gov_attr_set *attr_set, const char *buf,
				size_t count)
{
	struct interactive_tunables *tunables = to_tunables(attr_set);
	unsigned long val;
	int ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	tunables->boostpulse_min_interval = val;
	return count;
}
#endif

static ssize_t show_io_is_busy(struct cpufreq_interactive_tunables *tunables,
		char *buf)
{
	return sprintf(buf, "%u\n", tunables->io_is_busy);
}

static ssize_t store_io_is_busy(struct cpufreq_interactive_tunables *tunables,
		const char *buf, size_t count)
{
	int ret;
	unsigned long val;

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;
	tunables->io_is_busy = val;
	return count;
}

#ifdef CONFIG_HISI_HMPTH_OPEN
static ssize_t show_boost_hmp(struct cpufreq_interactive_tunables *tunables,
		char *buf)
{
	return sprintf(buf, "%u\n", tunables->boost_hmp_val);
}

static ssize_t store_boost_hmp(struct cpufreq_interactive_tunables *tunables,
		const char *buf, size_t count)
{
	int ret;
	unsigned long val;

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;
	tunables->boost_hmp_val = val;
	return count;
}

static ssize_t show_boost_hmp_upthreshold(struct cpufreq_interactive_tunables
		*tunables, char *buf)
{
	return sprintf(buf, "%u\n", tunables->boost_hmp_upthreshold);
}

static ssize_t store_boost_hmp_upthreshold(struct cpufreq_interactive_tunables
		*tunables, const char *buf, size_t count)
{
	int ret;
	unsigned long val;

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;
	tunables->boost_hmp_upthreshold = val;
	return count;
}

static ssize_t show_boost_hmp_downthreshold(struct cpufreq_interactive_tunables
		*tunables, char *buf)
{
	return sprintf(buf, "%u\n", tunables->boost_hmp_downthreshold);
}

static ssize_t store_boost_hmp_downthreshold(struct cpufreq_interactive_tunables
		*tunables, const char *buf, size_t count)
{
	int ret;
	unsigned long val;

	ret = kstrtoul(buf, 0, &val);
	if (ret < 0)
		return ret;
	tunables->boost_hmp_downthreshold = val;
	return count;
}
#endif

/*
 * Create show/store routines
 * - sys: One governor instance for complete SYSTEM
 * - pol: One governor instance per struct cpufreq_policy
 */
#define show_gov_pol_sys(file_name)					\
static ssize_t show_##file_name##_gov_sys				\
(struct kobject *kobj, struct attribute *attr, char *buf)		\
{									\
	return show_##file_name(common_tunables, buf);			\
}									\
									\
static ssize_t show_##file_name##_gov_pol				\
(struct cpufreq_policy *policy, char *buf)				\
{									\
	return show_##file_name(policy->governor_data, buf);		\
}

#define store_gov_pol_sys(file_name)					\
static ssize_t store_##file_name##_gov_sys				\
(struct kobject *kobj, struct attribute *attr, const char *buf,		\
	size_t count)							\
{									\
	return store_##file_name(common_tunables, buf, count);		\
}									\
									\
static ssize_t store_##file_name##_gov_pol				\
(struct cpufreq_policy *policy, const char *buf, size_t count)		\
{									\
	return store_##file_name(policy->governor_data, buf, count);	\
}

#define show_store_gov_pol_sys(file_name)				\
show_gov_pol_sys(file_name);						\
store_gov_pol_sys(file_name)

show_store_gov_pol_sys(target_loads);
show_store_gov_pol_sys(above_hispeed_delay);
show_store_gov_pol_sys(hispeed_freq);
show_store_gov_pol_sys(go_hispeed_load);
show_store_gov_pol_sys(min_sample_time);
show_store_gov_pol_sys(timer_rate);
show_store_gov_pol_sys(timer_slack);
show_store_gov_pol_sys(boost);
store_gov_pol_sys(boostpulse);
show_store_gov_pol_sys(boostpulse_duration);
#ifdef CONFIG_ARCH_HI3XXX
show_store_gov_pol_sys(boostpulse_min_interval);
#endif
show_store_gov_pol_sys(io_is_busy);
#ifdef CONFIG_HISI_HMPTH_OPEN
show_store_gov_pol_sys(boost_hmp);
show_store_gov_pol_sys(boost_hmp_upthreshold);
show_store_gov_pol_sys(boost_hmp_downthreshold);
#endif


#define gov_sys_attr_rw(_name)						\
static struct global_attr _name##_gov_sys =				\
__ATTR(_name, 0644, show_##_name##_gov_sys, store_##_name##_gov_sys)

#define gov_pol_attr_rw(_name)						\
static struct freq_attr _name##_gov_pol =				\
__ATTR(_name, 0644, show_##_name##_gov_pol, store_##_name##_gov_pol)

#define gov_sys_pol_attr_rw(_name)					\
	gov_sys_attr_rw(_name);						\
	gov_pol_attr_rw(_name)

gov_sys_pol_attr_rw(target_loads);
gov_sys_pol_attr_rw(above_hispeed_delay);
gov_sys_pol_attr_rw(hispeed_freq);
gov_sys_pol_attr_rw(go_hispeed_load);
gov_sys_pol_attr_rw(min_sample_time);
gov_sys_pol_attr_rw(timer_rate);
gov_sys_pol_attr_rw(timer_slack);
gov_sys_pol_attr_rw(boost);
gov_sys_pol_attr_rw(boostpulse_duration);
#ifdef CONFIG_ARCH_HI3XXX
gov_sys_pol_attr_rw(boostpulse_min_interval);
#endif
gov_sys_pol_attr_rw(io_is_busy);
#ifdef CONFIG_HISI_HMPTH_OPEN
gov_sys_pol_attr_rw(boost_hmp);
gov_sys_pol_attr_rw(boost_hmp_upthreshold);
gov_sys_pol_attr_rw(boost_hmp_downthreshold);
#endif


static struct global_attr boostpulse_gov_sys =
	__ATTR(boostpulse, 0200, NULL, store_boostpulse_gov_sys);

static struct freq_attr boostpulse_gov_pol =
	__ATTR(boostpulse, 0200, NULL, store_boostpulse_gov_pol);

/* One Governor instance for entire system */
static struct attribute *interactive_attributes_gov_sys[] = {
	&target_loads_gov_sys.attr,
	&above_hispeed_delay_gov_sys.attr,
	&hispeed_freq_gov_sys.attr,
	&go_hispeed_load_gov_sys.attr,
	&min_sample_time_gov_sys.attr,
	&timer_rate_gov_sys.attr,
	&timer_slack_gov_sys.attr,
	&boost_gov_sys.attr,
	&boostpulse_gov_sys.attr,
	&boostpulse_duration_gov_sys.attr,
#ifdef CONFIG_ARCH_HI3XXX
	&boostpulse_min_interval_gov_sys.attr,
#endif
	&io_is_busy_gov_sys.attr,
#ifdef CONFIG_HISI_HMPTH_OPEN
	&boost_hmp_gov_sys.attr,
	&boost_hmp_upthreshold_gov_sys.attr,
	&boost_hmp_downthreshold_gov_sys.attr,
#endif
	NULL,
};

static struct attribute_group interactive_attr_group_gov_sys = {
	.attrs = interactive_attributes_gov_sys,
	.name = "interactive",
};

/* Per policy governor instance */
static struct attribute *interactive_attributes_gov_pol[] = {
	&target_loads_gov_pol.attr,
	&above_hispeed_delay_gov_pol.attr,
	&hispeed_freq_gov_pol.attr,
	&go_hispeed_load_gov_pol.attr,
	&min_sample_time_gov_pol.attr,
	&timer_rate_gov_pol.attr,
	&timer_slack_gov_pol.attr,
	&boost_gov_pol.attr,
	&boostpulse_gov_pol.attr,
	&boostpulse_duration_gov_pol.attr,
#ifdef CONFIG_ARCH_HI3XXX
	&boostpulse_min_interval_gov_pol.attr,
#endif
	&io_is_busy_gov_pol.attr,
#ifdef CONFIG_HISI_HMPTH_OPEN
	&boost_hmp_gov_pol.attr,
	&boost_hmp_upthreshold_gov_pol.attr,
	&boost_hmp_downthreshold_gov_pol.attr,
#endif
	NULL,
};

static struct attribute_group interactive_attr_group_gov_pol = {
	.attrs = interactive_attributes_gov_pol,
	.name = "interactive",
};

static struct attribute_group *get_sysfs_attr(void)
{
	if (have_governor_per_policy())
		return &interactive_attr_group_gov_pol;
	else
		return &interactive_attr_group_gov_sys;
}

static int cpufreq_interactive_idle_notifier(struct notifier_block *nb,
					     unsigned long val,
					     void *data)
{
	switch (val) {
	case IDLE_START:
		cpufreq_interactive_idle_start();
		break;
	case IDLE_END:
		cpufreq_interactive_idle_end();
		break;
	}

	return 0;
}

static struct notifier_block cpufreq_interactive_idle_nb = {
	.notifier_call = cpufreq_interactive_idle_notifier,
};

static int cpufreq_governor_interactive(struct cpufreq_policy *policy,
		unsigned int event)
{
	int rc;
	unsigned int j;
	struct cpufreq_interactive_cpuinfo *pcpu;
	struct cpufreq_frequency_table *freq_table;
	struct cpufreq_interactive_tunables *tunables;
	unsigned long flags;

	if (have_governor_per_policy())
		tunables = policy->governor_data;
	else
		tunables = common_tunables;

	WARN_ON(!tunables && (event != CPUFREQ_GOV_POLICY_INIT));
	if (!tunables && (event != CPUFREQ_GOV_POLICY_INIT))
		return 0;

	switch (event) {
	case CPUFREQ_GOV_POLICY_INIT:
		if (have_governor_per_policy()) {
			WARN_ON(tunables);
		} else if (tunables) {
			tunables->usage_count++;
			policy->governor_data = tunables;
			return 0;
		}

		tunables = kzalloc(sizeof(*tunables), GFP_KERNEL);
		if (!tunables) {
			pr_err("%s: POLICY_INIT: kzalloc failed\n", __func__);
			return -ENOMEM;
		}

#ifdef CONFIG_INPUT_PULSE_SUPPORT
		if (policy->cpu == 0) {
			rc = input_register_handler(&cpufreq_interactive_input_handler);
			if (rc)
				pr_warn("%s: failed to register input handler\n",
					__func__);
		}
#endif /*#ifdef CONFIG_INPUT_PULSE_SUPPORT*/

		tunables->usage_count = 1;
		tunables->above_hispeed_delay = default_above_hispeed_delay;
		tunables->nabove_hispeed_delay =
			ARRAY_SIZE(default_above_hispeed_delay);
		tunables->go_hispeed_load = DEFAULT_GO_HISPEED_LOAD;
		tunables->target_loads = default_target_loads;
		tunables->ntarget_loads = ARRAY_SIZE(default_target_loads);
		tunables->min_sample_time = DEFAULT_MIN_SAMPLE_TIME;
		tunables->timer_rate = DEFAULT_TIMER_RATE;
		tunables->boostpulse_duration_val = DEFAULT_MIN_SAMPLE_TIME;
		tunables->timer_slack_val = DEFAULT_TIMER_SLACK;
#ifdef CONFIG_ARCH_HI3XXX
		tunables->boostpulse_min_interval = DEFAULT_MIN_BOOSTPULSE_INTERVAL;
#endif

#ifdef CONFIG_HISI_HMPTH_OPEN
		tunables->boost_hmp_upthreshold = DEFAULT_HMP_UP_THRESHOLD;
		tunables->boost_hmp_downthreshold = DEFAULT_HMP_DOWN_THRESHOLD;
#endif

		spin_lock_init(&tunables->target_loads_lock);
		spin_lock_init(&tunables->above_hispeed_delay_lock);

		policy->governor_data = tunables;
		if (!have_governor_per_policy())
			common_tunables = tunables;

		rc = sysfs_create_group(get_governor_parent_kobj(policy),
				get_sysfs_attr());
		if (rc) {
			kfree(tunables);
			policy->governor_data = NULL;
			if (!have_governor_per_policy())
				common_tunables = NULL;
			return rc;
		}

		if (!policy->governor->initialized) {
			idle_notifier_register(&cpufreq_interactive_idle_nb);
			cpufreq_register_notifier(&cpufreq_notifier_block,
					CPUFREQ_TRANSITION_NOTIFIER);
		}

#ifdef CONFIG_ARCH_HI3XXX
		if (policy->cpu == 0)
			cpufreq_interactive_initialized = 1;
#endif

		break;

	case CPUFREQ_GOV_POLICY_EXIT:
#ifdef CONFIG_ARCH_HI3XXX
		if (policy->cpu == 0)
			cpufreq_interactive_initialized = 0;
#endif
		if (!--tunables->usage_count) {
			if (policy->governor->initialized == 1) {
				cpufreq_unregister_notifier(&cpufreq_notifier_block,
						CPUFREQ_TRANSITION_NOTIFIER);
				idle_notifier_unregister(&cpufreq_interactive_idle_nb);
			}

#ifdef CONFIG_INPUT_PULSE_SUPPORT
			if (policy->cpu == 0)
				input_unregister_handler(&cpufreq_interactive_input_handler);
#endif /*#ifdef CONFIG_INPUT_PULSE_SUPPORT*/

			sysfs_remove_group(get_governor_parent_kobj(policy),
					get_sysfs_attr());
			kfree(tunables);
			common_tunables = NULL;
		}

		policy->governor_data = NULL;
		break;

	case CPUFREQ_GOV_START:
		mutex_lock(&gov_lock);

		freq_table = cpufreq_frequency_get_table(policy->cpu);
		if (!tunables->hispeed_freq)
#ifdef CONFIG_ARCH_HI3XXX
#define HI3XXX_DEFAULT_HISPEED_FREQ	(1209600)
			tunables->hispeed_freq = HI3XXX_DEFAULT_HISPEED_FREQ;
#else
			tunables->hispeed_freq = policy->max;
#endif
		for_each_cpu(j, policy->cpus) {
			pcpu = &per_cpu(cpuinfo, j);
			pcpu->policy = policy;
			pcpu->target_freq = policy->cur;
			pcpu->freq_table = freq_table;
			pcpu->floor_freq = pcpu->target_freq;
			pcpu->floor_validate_time =
				ktime_to_us(ktime_get());
			pcpu->hispeed_validate_time =
				pcpu->floor_validate_time;
			pcpu->max_freq = policy->max;
			down_write(&pcpu->enable_sem);
			del_timer_sync(&pcpu->cpu_timer);
			del_timer_sync(&pcpu->cpu_slack_timer);
			cpufreq_interactive_timer_start(tunables, j);
			pcpu->governor_enabled = 1;
			up_write(&pcpu->enable_sem);
		}

		mutex_unlock(&gov_lock);
		break;

	case CPUFREQ_GOV_STOP:
		mutex_lock(&gov_lock);
		for_each_cpu(j, policy->cpus) {
			pcpu = &per_cpu(cpuinfo, j);
			down_write(&pcpu->enable_sem);
			pcpu->governor_enabled = 0;
			del_timer_sync(&pcpu->cpu_timer);
			del_timer_sync(&pcpu->cpu_slack_timer);
			up_write(&pcpu->enable_sem);
		}

		mutex_unlock(&gov_lock);
		break;

	case CPUFREQ_GOV_LIMITS:
		if (policy->max < policy->cur)
			__cpufreq_driver_target(policy,
					policy->max, CPUFREQ_RELATION_H);
		else if (policy->min > policy->cur)
			__cpufreq_driver_target(policy,
					policy->min, CPUFREQ_RELATION_L);

		mutex_lock(&gov_lock);

		for_each_cpu(j, policy->cpus) {
			pcpu = &per_cpu(cpuinfo, j);

			down_read(&pcpu->enable_sem);
			if (pcpu->governor_enabled == 0) {
				up_read(&pcpu->enable_sem);
				continue;
			}

			spin_lock_irqsave(&pcpu->target_freq_lock, flags);
			if (policy->max < pcpu->target_freq)
				pcpu->target_freq = policy->max;
			else if (policy->min > pcpu->target_freq)
				pcpu->target_freq = policy->min;

			spin_unlock_irqrestore(&pcpu->target_freq_lock, flags);
			up_read(&pcpu->enable_sem);

			/* Reschedule timer only if policy->max is raised.
			 * Delete the timers, else the timer callback may
			 * return without re-arm the timer when failed
			 * acquire the semaphore. This race may cause timer
			 * stopped unexpectedly.
			 */

			if (policy->max > pcpu->max_freq) {
				down_write(&pcpu->enable_sem);
				del_timer_sync(&pcpu->cpu_timer);
				del_timer_sync(&pcpu->cpu_slack_timer);
				cpufreq_interactive_timer_start(tunables, j);
				up_write(&pcpu->enable_sem);
			}

			pcpu->max_freq = policy->max;
		}
		mutex_unlock(&gov_lock);
		break;
	}
	return 0;
}

#ifndef CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE
static
#endif
struct cpufreq_governor cpufreq_gov_interactive = {
	.name = "interactive",
	.governor = cpufreq_governor_interactive,
	.max_transition_latency = 10000000,
	.owner = THIS_MODULE,
=======
	tunables->io_is_busy = val;

	return count;
}

show_one(hispeed_freq, "%u");
show_one(go_hispeed_load, "%lu");
show_one(min_sample_time, "%lu");
show_one(timer_slack, "%lu");
show_one(boost, "%u");
show_one(boostpulse_duration, "%u");
show_one(io_is_busy, "%u");

gov_attr_rw(target_loads);
gov_attr_rw(above_hispeed_delay);
gov_attr_rw(hispeed_freq);
gov_attr_rw(go_hispeed_load);
gov_attr_rw(min_sample_time);
gov_attr_rw(timer_rate);
gov_attr_rw(timer_slack);
gov_attr_rw(boost);
gov_attr_wo(boostpulse);
gov_attr_rw(boostpulse_duration);
gov_attr_rw(io_is_busy);

static struct attribute *interactive_attributes[] = {
	&target_loads.attr,
	&above_hispeed_delay.attr,
	&hispeed_freq.attr,
	&go_hispeed_load.attr,
	&min_sample_time.attr,
	&timer_rate.attr,
	&timer_slack.attr,
	&boost.attr,
	&boostpulse.attr,
	&boostpulse_duration.attr,
	&io_is_busy.attr,
	NULL
};

static struct kobj_type interactive_tunables_ktype = {
	.default_attrs = interactive_attributes,
	.sysfs_ops = &governor_sysfs_ops,
};

static int cpufreq_interactive_idle_notifier(struct notifier_block *nb,
					     unsigned long val, void *data)
{
	if (val == IDLE_END)
		cpufreq_interactive_idle_end();

	return 0;
}

static struct notifier_block cpufreq_interactive_idle_nb = {
	.notifier_call = cpufreq_interactive_idle_notifier,
};

/* Interactive Governor callbacks */
struct interactive_governor {
	struct cpufreq_governor gov;
	unsigned int usage_count;
};

static struct interactive_governor interactive_gov;

#define CPU_FREQ_GOV_INTERACTIVE	(&interactive_gov.gov)

static void irq_work(struct irq_work *irq_work)
{
	struct interactive_cpu *icpu = container_of(irq_work, struct
						    interactive_cpu, irq_work);

	cpufreq_interactive_update(icpu);
	icpu->work_in_progress = false;
}

static void update_util_handler(struct update_util_data *data, u64 time,
				unsigned int flags)
{
	struct interactive_cpu *icpu = container_of(data,
					struct interactive_cpu, update_util);
	struct interactive_policy *ipolicy = icpu->ipolicy;
	struct interactive_tunables *tunables = ipolicy->tunables;
	u64 delta_ns;

	/*
	 * The irq-work may not be allowed to be queued up right now.
	 * Possible reasons:
	 * - Work has already been queued up or is in progress.
	 * - It is too early (too little time from the previous sample).
	 */
	if (icpu->work_in_progress)
		return;

	delta_ns = time - icpu->last_sample_time;
	if ((s64)delta_ns < tunables->sampling_rate * NSEC_PER_USEC)
		return;

	icpu->last_sample_time = time;
	icpu->next_sample_jiffies = usecs_to_jiffies(tunables->sampling_rate) +
				    jiffies;

	icpu->work_in_progress = true;
	irq_work_queue(&icpu->irq_work);
}

static void gov_set_update_util(struct interactive_policy *ipolicy)
{
	struct cpufreq_policy *policy = ipolicy->policy;
	struct interactive_cpu *icpu;
	int cpu;

	for_each_cpu(cpu, policy->cpus) {
		icpu = &per_cpu(interactive_cpu, cpu);

		icpu->last_sample_time = 0;
		icpu->next_sample_jiffies = 0;
		cpufreq_add_update_util_hook(cpu, &icpu->update_util,
					     update_util_handler);
	}
}

static inline void gov_clear_update_util(struct cpufreq_policy *policy)
{
	int i;

	for_each_cpu(i, policy->cpus)
		cpufreq_remove_update_util_hook(i);

	synchronize_sched();
}

static void icpu_cancel_work(struct interactive_cpu *icpu)
{
	irq_work_sync(&icpu->irq_work);
	icpu->work_in_progress = false;
	del_timer_sync(&icpu->slack_timer);
}

static struct interactive_policy *
interactive_policy_alloc(struct cpufreq_policy *policy)
{
	struct interactive_policy *ipolicy;

	ipolicy = kzalloc(sizeof(*ipolicy), GFP_KERNEL);
	if (!ipolicy)
		return NULL;

	ipolicy->policy = policy;

	return ipolicy;
}

static void interactive_policy_free(struct interactive_policy *ipolicy)
{
	kfree(ipolicy);
}

static struct interactive_tunables *
interactive_tunables_alloc(struct interactive_policy *ipolicy)
{
	struct interactive_tunables *tunables;

	tunables = kzalloc(sizeof(*tunables), GFP_KERNEL);
	if (!tunables)
		return NULL;

	gov_attr_set_init(&tunables->attr_set, &ipolicy->tunables_hook);
	if (!have_governor_per_policy())
		global_tunables = tunables;

	ipolicy->tunables = tunables;

	return tunables;
}

static void interactive_tunables_free(struct interactive_tunables *tunables)
{
	if (!have_governor_per_policy())
		global_tunables = NULL;

	kfree(tunables);
}

int cpufreq_interactive_init(struct cpufreq_policy *policy)
{
	struct interactive_policy *ipolicy;
	struct interactive_tunables *tunables;
	int ret;

	/* State should be equivalent to EXIT */
	if (policy->governor_data)
		return -EBUSY;

	ipolicy = interactive_policy_alloc(policy);
	if (!ipolicy)
		return -ENOMEM;

	mutex_lock(&global_tunables_lock);

	if (global_tunables) {
		if (WARN_ON(have_governor_per_policy())) {
			ret = -EINVAL;
			goto free_int_policy;
		}

		policy->governor_data = ipolicy;
		ipolicy->tunables = global_tunables;

		gov_attr_set_get(&global_tunables->attr_set,
				 &ipolicy->tunables_hook);
		goto out;
	}

	tunables = interactive_tunables_alloc(ipolicy);
	if (!tunables) {
		ret = -ENOMEM;
		goto free_int_policy;
	}

	tunables->hispeed_freq = policy->max;
	tunables->above_hispeed_delay = default_above_hispeed_delay;
	tunables->nabove_hispeed_delay =
		ARRAY_SIZE(default_above_hispeed_delay);
	tunables->go_hispeed_load = DEFAULT_GO_HISPEED_LOAD;
	tunables->target_loads = default_target_loads;
	tunables->ntarget_loads = ARRAY_SIZE(default_target_loads);
	tunables->min_sample_time = DEFAULT_MIN_SAMPLE_TIME;
	tunables->boostpulse_duration = DEFAULT_MIN_SAMPLE_TIME;
	tunables->sampling_rate = DEFAULT_SAMPLING_RATE;
	tunables->timer_slack = DEFAULT_TIMER_SLACK;
	update_slack_delay(tunables);

	spin_lock_init(&tunables->target_loads_lock);
	spin_lock_init(&tunables->above_hispeed_delay_lock);

	policy->governor_data = ipolicy;

	ret = kobject_init_and_add(&tunables->attr_set.kobj,
				   &interactive_tunables_ktype,
				   get_governor_parent_kobj(policy), "%s",
				   interactive_gov.gov.name);
	if (ret)
		goto fail;

	/* One time initialization for governor */
	if (!interactive_gov.usage_count++) {
		idle_notifier_register(&cpufreq_interactive_idle_nb);
		cpufreq_register_notifier(&cpufreq_notifier_block,
					  CPUFREQ_TRANSITION_NOTIFIER);
	}

 out:
	mutex_unlock(&global_tunables_lock);
	return 0;

 fail:
	policy->governor_data = NULL;
	interactive_tunables_free(tunables);

 free_int_policy:
	mutex_unlock(&global_tunables_lock);

	interactive_policy_free(ipolicy);
	pr_err("governor initialization failed (%d)\n", ret);

	return ret;
}

void cpufreq_interactive_exit(struct cpufreq_policy *policy)
{
	struct interactive_policy *ipolicy = policy->governor_data;
	struct interactive_tunables *tunables = ipolicy->tunables;
	unsigned int count;

	mutex_lock(&global_tunables_lock);

	/* Last policy using the governor ? */
	if (!--interactive_gov.usage_count) {
		cpufreq_unregister_notifier(&cpufreq_notifier_block,
					    CPUFREQ_TRANSITION_NOTIFIER);
		idle_notifier_unregister(&cpufreq_interactive_idle_nb);
	}

	count = gov_attr_set_put(&tunables->attr_set, &ipolicy->tunables_hook);
	policy->governor_data = NULL;
	if (!count)
		interactive_tunables_free(tunables);

	mutex_unlock(&global_tunables_lock);

	interactive_policy_free(ipolicy);
}

int cpufreq_interactive_start(struct cpufreq_policy *policy)
{
	struct interactive_policy *ipolicy = policy->governor_data;
	struct interactive_cpu *icpu;
	unsigned int cpu;

	for_each_cpu(cpu, policy->cpus) {
		icpu = &per_cpu(interactive_cpu, cpu);

		icpu->target_freq = policy->cur;
		icpu->floor_freq = icpu->target_freq;
		icpu->pol_floor_val_time = ktime_to_us(ktime_get());
		icpu->loc_floor_val_time = icpu->pol_floor_val_time;
		icpu->pol_hispeed_val_time = icpu->pol_floor_val_time;
		icpu->loc_hispeed_val_time = icpu->pol_floor_val_time;

		down_write(&icpu->enable_sem);
		icpu->ipolicy = ipolicy;
		up_write(&icpu->enable_sem);

		slack_timer_resched(icpu, cpu, false);
	}

	gov_set_update_util(ipolicy);
	return 0;
}

void cpufreq_interactive_stop(struct cpufreq_policy *policy)
{
	struct interactive_policy *ipolicy = policy->governor_data;
	struct interactive_cpu *icpu;
	unsigned int cpu;

	gov_clear_update_util(ipolicy->policy);

	for_each_cpu(cpu, policy->cpus) {
		icpu = &per_cpu(interactive_cpu, cpu);

		icpu_cancel_work(icpu);

		down_write(&icpu->enable_sem);
		icpu->ipolicy = NULL;
		up_write(&icpu->enable_sem);
	}
}

void cpufreq_interactive_limits(struct cpufreq_policy *policy)
{
	struct interactive_cpu *icpu;
	unsigned int cpu;
	unsigned long flags;

	cpufreq_policy_apply_limits(policy);

	for_each_cpu(cpu, policy->cpus) {
		icpu = &per_cpu(interactive_cpu, cpu);

		spin_lock_irqsave(&icpu->target_freq_lock, flags);

		if (policy->max < icpu->target_freq)
			icpu->target_freq = policy->max;
		else if (policy->min > icpu->target_freq)
			icpu->target_freq = policy->min;

		spin_unlock_irqrestore(&icpu->target_freq_lock, flags);
	}
}

static struct interactive_governor interactive_gov = {
	.gov = {
		.name			= "interactive",
		.max_transition_latency	= TRANSITION_LATENCY_LIMIT,
		.owner			= THIS_MODULE,
		.init			= cpufreq_interactive_init,
		.exit			= cpufreq_interactive_exit,
		.start			= cpufreq_interactive_start,
		.stop			= cpufreq_interactive_stop,
		.limits			= cpufreq_interactive_limits,
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static void cpufreq_interactive_nop_timer(unsigned long data)
{
<<<<<<< HEAD
}

static int __init cpufreq_interactive_init(void)
{
	unsigned int i;
	struct cpufreq_interactive_cpuinfo *pcpu;
	struct sched_param param = { .sched_priority = MAX_RT_PRIO-1 };

	/* Initalize per-cpu timers */
	for_each_possible_cpu(i) {
		pcpu = &per_cpu(cpuinfo, i);
		init_timer_deferrable(&pcpu->cpu_timer);
		pcpu->cpu_timer.function = cpufreq_interactive_timer;
		pcpu->cpu_timer.data = i;
		init_timer(&pcpu->cpu_slack_timer);
		pcpu->cpu_slack_timer.function = cpufreq_interactive_nop_timer;
		spin_lock_init(&pcpu->load_lock);
		spin_lock_init(&pcpu->target_freq_lock);
		init_rwsem(&pcpu->enable_sem);
	}

	spin_lock_init(&speedchange_cpumask_lock);
	mutex_init(&gov_lock);
	speedchange_task =
		kthread_create(cpufreq_interactive_speedchange_task, NULL,
			       "cfinteractive");
=======
	/*
	 * The purpose of slack-timer is to wake up the CPU from IDLE, in order
	 * to decrease its frequency if it is not set to minimum already.
	 *
	 * This is important for platforms where CPU with higher frequencies
	 * consume higher power even at IDLE.
	 */
}

static int __init cpufreq_interactive_gov_init(void)
{
	struct sched_param param = { .sched_priority = MAX_RT_PRIO - 1 };
	struct interactive_cpu *icpu;
	unsigned int cpu;

	for_each_possible_cpu(cpu) {
		icpu = &per_cpu(interactive_cpu, cpu);

		init_irq_work(&icpu->irq_work, irq_work);
		spin_lock_init(&icpu->load_lock);
		spin_lock_init(&icpu->target_freq_lock);
		init_rwsem(&icpu->enable_sem);

		/* Initialize per-cpu slack-timer */
		init_timer_pinned(&icpu->slack_timer);
		icpu->slack_timer.function = cpufreq_interactive_nop_timer;
	}

	spin_lock_init(&speedchange_cpumask_lock);
	speedchange_task = kthread_create(cpufreq_interactive_speedchange_task,
					  NULL, "cfinteractive");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (IS_ERR(speedchange_task))
		return PTR_ERR(speedchange_task);

	sched_setscheduler_nocheck(speedchange_task, SCHED_FIFO, &param);
	get_task_struct(speedchange_task);

<<<<<<< HEAD
	/* NB: wake up so the thread does not look hung to the freezer */
	wake_up_process(speedchange_task);

#ifdef CONFIG_INPUT_PULSE_SUPPORT
	/* No rescuer thread, bind to CPU queuing the work for possibly
	   warm cache (probably doesn't matter much). */
	down_wq = alloc_workqueue("knteractive_down", 0, 1);

	if (!down_wq)
		goto err_freeuptask;

	INIT_WORK(&inputopen.inputopen_work, cpufreq_interactive_input_open);
#endif /*#ifdef CONFIG_INPUT_PULSE_SUPPORT*/

	return cpufreq_register_governor(&cpufreq_gov_interactive);

#ifdef CONFIG_INPUT_PULSE_SUPPORT
err_freeuptask:
	put_task_struct(speedchange_task);
	return -ENOMEM;
#endif /*#ifdef CONFIG_INPUT_PULSE_SUPPORT*/
}

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE
fs_initcall(cpufreq_interactive_init);
#else
module_init(cpufreq_interactive_init);
#endif

static void __exit cpufreq_interactive_exit(void)
{
	cpufreq_unregister_governor(&cpufreq_gov_interactive);
	kthread_stop(speedchange_task);
	put_task_struct(speedchange_task);
}

module_exit(cpufreq_interactive_exit);

MODULE_AUTHOR("Mike Chan <mike@android.com>");
MODULE_DESCRIPTION("'cpufreq_interactive' - A cpufreq governor for "
	"Latency sensitive workloads");
=======
	/* wake up so the thread does not look hung to the freezer */
	wake_up_process(speedchange_task);

	return cpufreq_register_governor(CPU_FREQ_GOV_INTERACTIVE);
}

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE
struct cpufreq_governor *cpufreq_default_governor(void)
{
	return CPU_FREQ_GOV_INTERACTIVE;
}

fs_initcall(cpufreq_interactive_gov_init);
#else
module_init(cpufreq_interactive_gov_init);
#endif

static void __exit cpufreq_interactive_gov_exit(void)
{
	cpufreq_unregister_governor(CPU_FREQ_GOV_INTERACTIVE);
	kthread_stop(speedchange_task);
	put_task_struct(speedchange_task);
}
module_exit(cpufreq_interactive_gov_exit);

MODULE_AUTHOR("Mike Chan <mike@android.com>");
MODULE_DESCRIPTION("'cpufreq_interactive' - A dynamic cpufreq governor for Latency sensitive workloads");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_LICENSE("GPL");
