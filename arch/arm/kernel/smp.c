/*
 *  linux/arch/arm/kernel/smp.c
 *
 *  Copyright (C) 2002 ARM Limited, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/cache.h>
#include <linux/profile.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/cpu.h>
#include <linux/seq_file.h>
#include <linux/irq.h>
<<<<<<< HEAD
=======
#include <linux/nmi.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/percpu.h>
#include <linux/clockchips.h>
#include <linux/completion.h>
#include <linux/cpufreq.h>
<<<<<<< HEAD

#include <linux/atomic.h>
=======
#include <linux/irq_work.h>
#include <linux/slab.h>

#include <linux/atomic.h>
#include <asm/bugs.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/smp.h>
#include <asm/cacheflush.h>
#include <asm/cpu.h>
#include <asm/cputype.h>
#include <asm/exception.h>
#include <asm/idmap.h>
#include <asm/topology.h>
#include <asm/mmu_context.h>
#include <asm/pgtable.h>
#include <asm/pgalloc.h>
<<<<<<< HEAD
=======
#include <asm/procinfo.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/processor.h>
#include <asm/sections.h>
#include <asm/tlbflush.h>
#include <asm/ptrace.h>
<<<<<<< HEAD
#include <asm/localtimer.h>
#include <asm/smp_plat.h>
#include <asm/virt.h>
#include <asm/mach/arch.h>

/*HI3630: flush all cpu all cache++*/
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
/*HI3630: flush all cpu all cache--*/

#define CREATE_TRACE_POINTS
#include <trace/events/arm-ipi.h>

/*HI3630: flush all cpu all cache++*/
/*#define HI3630_FC_DEBUG					(1)*/
#define HI3630_FC_SKIPOFFCORE				(1)
#define HI3630_FC_LOUIS						(1)
/*HI3630: flush all cpu all cache--*/
=======
#include <asm/smp_plat.h>
#include <asm/virt.h>
#include <asm/mach/arch.h>
#include <asm/mpu.h>

#define CREATE_TRACE_POINTS
#include <trace/events/ipi.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * as from 2.5, kernels no longer have an init_tasks structure
 * so we need some other way of telling a new secondary core
 * where to place its SVC stack
 */
struct secondary_data secondary_data;

<<<<<<< HEAD
#ifdef CONFIG_SECURE_EXTENSION
static ipi_secure_notify_handler *secure_notify_handler = NULL;
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * control for which core is the next to come out of the secondary
 * boot "holding pen"
 */
volatile int pen_release = -1;

enum ipi_msg_type {
	IPI_WAKEUP,
	IPI_TIMER,
	IPI_RESCHEDULE,
	IPI_CALL_FUNC,
<<<<<<< HEAD
	IPI_CALL_FUNC_SINGLE,
	IPI_CPU_STOP,
	IPI_COMPLETION,
	IPI_CPU_BACKTRACE,
#ifdef CONFIG_SECURE_EXTENSION
        IPI_NOTIFY_FUNC,
#endif
=======
	IPI_CPU_STOP,
	IPI_IRQ_WORK,
	IPI_COMPLETION,
	/*
	 * CPU_BACKTRACE is special and not included in NR_IPI
	 * or tracable with trace_ipi_*
	 */
	IPI_CPU_BACKTRACE,
	/*
	 * SGI8-15 can be reserved by secure firmware, and thus may
	 * not be usable by the kernel. Please keep the above limited
	 * to at most 8 entries.
	 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static DECLARE_COMPLETION(cpu_running);

<<<<<<< HEAD
static struct smp_operations smp_ops;

void __init smp_set_ops(struct smp_operations *ops)
=======
static struct smp_operations smp_ops __ro_after_init;

void __init smp_set_ops(const struct smp_operations *ops)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (ops)
		smp_ops = *ops;
};

<<<<<<< HEAD
int __cpuinit __cpu_up(unsigned int cpu, struct task_struct *idle)
{
	int ret;

=======
static unsigned long get_arch_pgd(pgd_t *pgd)
{
#ifdef CONFIG_ARM_LPAE
	return __phys_to_pfn(virt_to_phys(pgd));
#else
	return virt_to_phys(pgd);
#endif
}

#if defined(CONFIG_BIG_LITTLE) && defined(CONFIG_HARDEN_BRANCH_PREDICTOR)
static int secondary_biglittle_prepare(unsigned int cpu)
{
	if (!cpu_vtable[cpu])
		cpu_vtable[cpu] = kzalloc(sizeof(*cpu_vtable[cpu]), GFP_KERNEL);

	return cpu_vtable[cpu] ? 0 : -ENOMEM;
}

static void secondary_biglittle_init(void)
{
	init_proc_vtable(lookup_processor(read_cpuid_id())->proc);
}
#else
static int secondary_biglittle_prepare(unsigned int cpu)
{
	return 0;
}

static void secondary_biglittle_init(void)
{
}
#endif

int __cpu_up(unsigned int cpu, struct task_struct *idle)
{
	int ret;

	if (!smp_ops.smp_boot_secondary)
		return -ENOSYS;

	ret = secondary_biglittle_prepare(cpu);
	if (ret)
		return ret;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * We need to tell the secondary core where to find
	 * its stack and the page tables.
	 */
	secondary_data.stack = task_stack_page(idle) + THREAD_START_SP;
<<<<<<< HEAD
	secondary_data.pgdir = virt_to_idmap(idmap_pgd);
	secondary_data.swapper_pg_dir = virt_to_idmap(swapper_pg_dir);
	__cpuc_flush_dcache_area(&secondary_data, sizeof(secondary_data));
	outer_clean_range(__pa(&secondary_data), __pa(&secondary_data + 1));
=======
#ifdef CONFIG_ARM_MPU
	secondary_data.mpu_rgn_szr = mpu_rgn_info.rgns[MPU_RAM_REGION].drsr;
#endif

#ifdef CONFIG_MMU
	secondary_data.pgdir = virt_to_phys(idmap_pgd);
	secondary_data.swapper_pg_dir = get_arch_pgd(swapper_pg_dir);
#endif
	sync_cache_w(&secondary_data);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Now bring the CPU into our world.
	 */
<<<<<<< HEAD
	ret = boot_secondary(cpu, idle);
=======
	ret = smp_ops.smp_boot_secondary(cpu, idle);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret == 0) {
		/*
		 * CPU was successfully started, wait for it
		 * to come online or time out.
		 */
		wait_for_completion_timeout(&cpu_running,
						 msecs_to_jiffies(1000));

		if (!cpu_online(cpu)) {
			pr_crit("CPU%u: failed to come online\n", cpu);
			ret = -EIO;
		}
	} else {
		pr_err("CPU%u: failed to boot: %d\n", cpu, ret);
	}

<<<<<<< HEAD
	secondary_data.stack = NULL;
	secondary_data.pgdir = 0;

=======

	memset(&secondary_data, 0, sizeof(secondary_data));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

/* platform specific SMP operations */
void __init smp_init_cpus(void)
{
	if (smp_ops.smp_init_cpus)
		smp_ops.smp_init_cpus();
}

<<<<<<< HEAD
int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	if (smp_ops.smp_boot_secondary)
		return smp_ops.smp_boot_secondary(cpu, idle);
	return -ENOSYS;
}

#ifdef CONFIG_HOTPLUG_CPU
static void percpu_timer_stop(void);

=======
int platform_can_secondary_boot(void)
{
	return !!smp_ops.smp_boot_secondary;
}

int platform_can_cpu_hotplug(void)
{
#ifdef CONFIG_HOTPLUG_CPU
	if (smp_ops.cpu_kill)
		return 1;
#endif

	return 0;
}

#ifdef CONFIG_HOTPLUG_CPU
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int platform_cpu_kill(unsigned int cpu)
{
	if (smp_ops.cpu_kill)
		return smp_ops.cpu_kill(cpu);
	return 1;
}

static int platform_cpu_disable(unsigned int cpu)
{
	if (smp_ops.cpu_disable)
		return smp_ops.cpu_disable(cpu);

<<<<<<< HEAD
=======
	return 0;
}

int platform_can_hotplug_cpu(unsigned int cpu)
{
	/* cpu_die must be specified to support hotplug */
	if (!smp_ops.cpu_die)
		return 0;

	if (smp_ops.cpu_can_disable)
		return smp_ops.cpu_can_disable(cpu);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * By default, allow disabling all CPUs except the first one,
	 * since this is special on a lot of platforms, e.g. because
	 * of clock tick interrupts.
	 */
<<<<<<< HEAD
	return cpu == 0 ? -EPERM : 0;
}
/*
 * __cpu_disable runs on the processor to be shutdown.
 */
int __cpuinit __cpu_disable(void)
=======
	return cpu != 0;
}

/*
 * __cpu_disable runs on the processor to be shutdown.
 */
int __cpu_disable(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned int cpu = smp_processor_id();
	int ret;

	ret = platform_cpu_disable(cpu);
	if (ret)
		return ret;

	/*
	 * Take this CPU offline.  Once we clear this, we can't return,
	 * and we must not schedule until we're ready to give up the cpu.
	 */
	set_cpu_online(cpu, false);

	/*
	 * OK - migrate IRQs away from this CPU
	 */
<<<<<<< HEAD
	migrate_irqs();

	/*
	 * Stop the local timer for this CPU.
	 */
	percpu_timer_stop();
=======
	irq_migrate_all_off_this_cpu();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Flush user cache and TLB mappings, and then remove this CPU
	 * from the vm mask set of all processes.
	 *
	 * Caches are flushed to the Level of Unification Inner Shareable
	 * to write-back dirty lines to unified caches shared by all CPUs.
	 */
	flush_cache_louis();
	local_flush_tlb_all();

	clear_tasks_mm_cpumask(cpu);

	return 0;
}

static DECLARE_COMPLETION(cpu_died);

/*
 * called on the thread which is asking for a CPU to be shutdown -
 * waits until shutdown has completed, or it is timed out.
 */
<<<<<<< HEAD
void __cpuinit __cpu_die(unsigned int cpu)
=======
void __cpu_die(unsigned int cpu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (!wait_for_completion_timeout(&cpu_died, msecs_to_jiffies(5000))) {
		pr_err("CPU%u: cpu didn't die\n", cpu);
		return;
	}
<<<<<<< HEAD
	printk(KERN_NOTICE "CPU%u: shutdown\n", cpu);
=======
	pr_notice("CPU%u: shutdown\n", cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * platform_cpu_kill() is generally expected to do the powering off
	 * and/or cutting of clocks to the dying CPU.  Optionally, this may
	 * be done by the CPU which is dying in preference to supporting
	 * this call, but that means there is _no_ synchronisation between
	 * the requesting CPU and the dying CPU actually losing power.
	 */
	if (!platform_cpu_kill(cpu))
<<<<<<< HEAD
		printk("CPU%u: unable to kill\n", cpu);
=======
		pr_err("CPU%u: unable to kill\n", cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Called from the idle thread for the CPU which has been shutdown.
 *
 * Note that we disable IRQs here, but do not re-enable them
 * before returning to the caller. This is also the behaviour
 * of the other hotplug-cpu capable cores, so presumably coming
 * out of idle fixes this.
 */
<<<<<<< HEAD
void __ref cpu_die(void)
=======
void arch_cpu_idle_dead(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned int cpu = smp_processor_id();

	idle_task_exit();

	local_irq_disable();

	/*
	 * Flush the data out of the L1 cache for this CPU.  This must be
	 * before the completion to ensure that data is safely written out
	 * before platform_cpu_kill() gets called - which may disable
	 * *this* CPU and power down its cache.
	 */
	flush_cache_louis();

	/*
	 * Tell __cpu_die() that this CPU is now safe to dispose of.  Once
	 * this returns, power and/or clocks can be removed at any point
	 * from this CPU and its cache by platform_cpu_kill().
	 */
	complete(&cpu_died);

	/*
	 * Ensure that the cache lines associated with that completion are
	 * written out.  This covers the case where _this_ CPU is doing the
	 * powering down, to ensure that the completion is visible to the
	 * CPU waiting for this one.
	 */
	flush_cache_louis();

	/*
	 * The actual CPU shutdown procedure is at least platform (if not
	 * CPU) specific.  This may remove power, or it may simply spin.
	 *
	 * Platforms are generally expected *NOT* to return from this call,
	 * although there are some which do because they have no way to
	 * power down the CPU.  These platforms are the _only_ reason we
	 * have a return path which uses the fragment of assembly below.
	 *
	 * The return path should not be used for platforms which can
	 * power off the CPU.
	 */
	if (smp_ops.cpu_die)
		smp_ops.cpu_die(cpu);

<<<<<<< HEAD
=======
	pr_warn("CPU%u: smp_ops.cpu_die() returned, trying to resuscitate\n",
		cpu);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * Do not return to the idle loop - jump back to the secondary
	 * cpu initialisation.  There's some initialisation which needs
	 * to be repeated to undo the effects of taking the CPU offline.
	 */
	__asm__("mov	sp, %0\n"
	"	mov	fp, #0\n"
	"	b	secondary_start_kernel"
		:
		: "r" (task_stack_page(current) + THREAD_SIZE - 8));
}
#endif /* CONFIG_HOTPLUG_CPU */

/*
 * Called by both boot and secondaries to move global data into
 * per-processor storage.
 */
<<<<<<< HEAD
static void __cpuinit smp_store_cpu_info(unsigned int cpuid)
=======
static void smp_store_cpu_info(unsigned int cpuid)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct cpuinfo_arm *cpu_info = &per_cpu(cpu_data, cpuid);

	cpu_info->loops_per_jiffy = loops_per_jiffy;
	cpu_info->cpuid = read_cpuid_id();

	store_cpu_topology(cpuid);
}

<<<<<<< HEAD
static void percpu_timer_setup(void);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * This is the secondary CPU boot entry.  We're using this CPUs
 * idle thread stack, but a set of temporary page tables.
 */
<<<<<<< HEAD
asmlinkage void __cpuinit secondary_start_kernel(void)
=======
asmlinkage void secondary_start_kernel(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct mm_struct *mm = &init_mm;
	unsigned int cpu;

<<<<<<< HEAD
=======
	secondary_biglittle_init();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * The identity mapping is uncached (strongly ordered), so
	 * switch away from it before attempting any exclusive accesses.
	 */
	cpu_switch_mm(mm->pgd, mm);
	local_flush_bp_all();
	enter_lazy_tlb(mm, current);
	local_flush_tlb_all();

	/*
	 * All kernel threads share the same mm context; grab a
	 * reference and switch to it.
	 */
	cpu = smp_processor_id();
	atomic_inc(&mm->mm_count);
	current->active_mm = mm;
	cpumask_set_cpu(cpu, mm_cpumask(mm));

	cpu_init();

<<<<<<< HEAD
	printk("CPU%u: Booted secondary processor\n", cpu);
=======
	pr_debug("CPU%u: Booted secondary processor\n", cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	preempt_disable();
	trace_hardirqs_off();

	/*
	 * Give the platform a chance to do its own initialisation.
	 */
	if (smp_ops.smp_secondary_init)
		smp_ops.smp_secondary_init(cpu);

	notify_cpu_starting(cpu);

	calibrate_delay();

	smp_store_cpu_info(cpu);

	/*
	 * OK, now it's safe to let the boot CPU continue.  Wait for
	 * the CPU migration code to notice that the CPU is online
	 * before we continue - which happens after __cpu_up returns.
	 */
	set_cpu_online(cpu, true);
<<<<<<< HEAD
	complete(&cpu_running);

	/*
	 * Setup the percpu timer for this CPU.
	 */
	percpu_timer_setup();

	local_irq_enable();
	local_fiq_enable();
=======

	check_other_bugs();

	complete(&cpu_running);

	local_irq_enable();
	local_fiq_enable();
	local_abt_enable();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * OK, it's off to the idle thread for us
	 */
<<<<<<< HEAD
	cpu_startup_entry(CPUHP_ONLINE);
=======
	cpu_startup_entry(CPUHP_AP_ONLINE_IDLE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void __init smp_cpus_done(unsigned int max_cpus)
{
	int cpu;
	unsigned long bogosum = 0;

	for_each_online_cpu(cpu)
		bogosum += per_cpu(cpu_data, cpu).loops_per_jiffy;

	printk(KERN_INFO "SMP: Total of %d processors activated "
	       "(%lu.%02lu BogoMIPS).\n",
	       num_online_cpus(),
	       bogosum / (500000/HZ),
	       (bogosum / (5000/HZ)) % 100);

	hyp_mode_check();
}

void __init smp_prepare_boot_cpu(void)
{
	set_my_cpu_offset(per_cpu_offset(smp_processor_id()));
}

void __init smp_prepare_cpus(unsigned int max_cpus)
{
	unsigned int ncores = num_possible_cpus();

	init_cpu_topology();

	smp_store_cpu_info(smp_processor_id());

	/*
	 * are we trying to boot more cores than exist?
	 */
	if (max_cpus > ncores)
		max_cpus = ncores;
	if (ncores > 1 && max_cpus) {
		/*
<<<<<<< HEAD
		 * Enable the local timer or broadcast device for the
		 * boot CPU, but only if we have more than one CPU.
		 */
		percpu_timer_setup();

		/*
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		 * Initialise the present map, which describes the set of CPUs
		 * actually populated at the present time. A platform should
		 * re-initialize the map in the platforms smp_prepare_cpus()
		 * if present != possible (e.g. physical hotplug).
		 */
		init_cpu_present(cpu_possible_mask);

		/*
		 * Initialise the SCU if there are more than one CPU
		 * and let them know where to start.
		 */
		if (smp_ops.smp_prepare_cpus)
			smp_ops.smp_prepare_cpus(max_cpus);
	}
}

<<<<<<< HEAD
static void (*smp_cross_call)(const struct cpumask *, unsigned int);

void __init set_smp_cross_call(void (*fn)(const struct cpumask *, unsigned int))
{
	if (!smp_cross_call)
		smp_cross_call = fn;
}

void arch_send_call_function_ipi_mask(const struct cpumask *mask)
{
	smp_cross_call(mask, IPI_CALL_FUNC);
}

void arch_send_wakeup_ipi_mask(const struct cpumask *mask)
{
	smp_cross_call(mask, IPI_WAKEUP);
}

void arch_send_call_function_single_ipi(int cpu)
{
	smp_cross_call(cpumask_of(cpu), IPI_CALL_FUNC_SINGLE);
}

static const char *ipi_types[NR_IPI] = {
=======
static void (*__smp_cross_call)(const struct cpumask *, unsigned int);

void __init set_smp_cross_call(void (*fn)(const struct cpumask *, unsigned int))
{
	if (!__smp_cross_call)
		__smp_cross_call = fn;
}

static const char *ipi_types[NR_IPI] __tracepoint_string = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define S(x,s)	[x] = s
	S(IPI_WAKEUP, "CPU wakeup interrupts"),
	S(IPI_TIMER, "Timer broadcast interrupts"),
	S(IPI_RESCHEDULE, "Rescheduling interrupts"),
	S(IPI_CALL_FUNC, "Function call interrupts"),
<<<<<<< HEAD
	S(IPI_CALL_FUNC_SINGLE, "Single function call interrupts"),
	S(IPI_CPU_STOP, "CPU stop interrupts"),
	S(IPI_COMPLETION, "completion interrupts"),
	S(IPI_CPU_BACKTRACE, "CPU backtrace"),
#ifdef CONFIG_SECURE_EXTENSION
    S(IPI_NOTIFY_FUNC, "Secure kernel notification"),
#endif
};

=======
	S(IPI_CPU_STOP, "CPU stop interrupts"),
	S(IPI_IRQ_WORK, "IRQ work interrupts"),
	S(IPI_COMPLETION, "completion interrupts"),
};

static void smp_cross_call(const struct cpumask *target, unsigned int ipinr)
{
	trace_ipi_raise_rcuidle(target, ipi_types[ipinr]);
	__smp_cross_call(target, ipinr);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void show_ipi_list(struct seq_file *p, int prec)
{
	unsigned int cpu, i;

	for (i = 0; i < NR_IPI; i++) {
		seq_printf(p, "%*s%u: ", prec - 1, "IPI", i);

		for_each_online_cpu(cpu)
			seq_printf(p, "%10u ",
				   __get_irq_stat(cpu, ipi_irqs[i]));

		seq_printf(p, " %s\n", ipi_types[i]);
	}
}

u64 smp_irq_stat_cpu(unsigned int cpu)
{
	u64 sum = 0;
	int i;

	for (i = 0; i < NR_IPI; i++)
		sum += __get_irq_stat(cpu, ipi_irqs[i]);

	return sum;
}

<<<<<<< HEAD
/*
 * Timer (local or broadcast) support
 */
static DEFINE_PER_CPU(struct clock_event_device, percpu_clockevent);

#ifdef CONFIG_GENERIC_CLOCKEVENTS_BROADCAST
void tick_broadcast(const struct cpumask *mask)
{
	smp_cross_call(mask, IPI_TIMER);
}
#endif

static void broadcast_timer_set_mode(enum clock_event_mode mode,
	struct clock_event_device *evt)
{
}

static void __cpuinit broadcast_timer_setup(struct clock_event_device *evt)
{
	evt->name	= "dummy_timer";
	evt->features	= CLOCK_EVT_FEAT_ONESHOT |
			  CLOCK_EVT_FEAT_PERIODIC |
			  CLOCK_EVT_FEAT_DUMMY;
	evt->rating	= 100;
	evt->mult	= 1;
	evt->set_mode	= broadcast_timer_set_mode;

	clockevents_register_device(evt);
}

static struct local_timer_ops *lt_ops;

#ifdef CONFIG_LOCAL_TIMERS
int local_timer_register(struct local_timer_ops *ops)
{
	if (!is_smp() || !setup_max_cpus)
		return -ENXIO;

	if (lt_ops)
		return -EBUSY;

	lt_ops = ops;
	return 0;
}
#endif

static void __cpuinit percpu_timer_setup(void)
{
	unsigned int cpu = smp_processor_id();
	struct clock_event_device *evt = &per_cpu(percpu_clockevent, cpu);

	evt->cpumask = cpumask_of(cpu);

	if (!lt_ops || lt_ops->setup(evt))
		broadcast_timer_setup(evt);
}

#ifdef CONFIG_HOTPLUG_CPU
/*
 * The generic clock events code purposely does not stop the local timer
 * on CPU_DEAD/CPU_DEAD_FROZEN hotplug events, so we have to do it
 * manually here.
 */
static void percpu_timer_stop(void)
{
	unsigned int cpu = smp_processor_id();
	struct clock_event_device *evt = &per_cpu(percpu_clockevent, cpu);

	if (lt_ops)
		lt_ops->stop(evt);
=======
void arch_send_call_function_ipi_mask(const struct cpumask *mask)
{
	smp_cross_call(mask, IPI_CALL_FUNC);
}

void arch_send_wakeup_ipi_mask(const struct cpumask *mask)
{
	smp_cross_call(mask, IPI_WAKEUP);
}

void arch_send_call_function_single_ipi(int cpu)
{
	smp_cross_call(cpumask_of(cpu), IPI_CALL_FUNC);
}

#ifdef CONFIG_IRQ_WORK
void arch_irq_work_raise(void)
{
	if (arch_irq_work_has_interrupt())
		smp_cross_call(cpumask_of(smp_processor_id()), IPI_IRQ_WORK);
}
#endif

#ifdef CONFIG_GENERIC_CLOCKEVENTS_BROADCAST
void tick_broadcast(const struct cpumask *mask)
{
	smp_cross_call(mask, IPI_TIMER);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#endif

static DEFINE_RAW_SPINLOCK(stop_lock);

/*
 * ipi_cpu_stop - handle IPI from smp_send_stop()
 */
static void ipi_cpu_stop(unsigned int cpu)
{
	if (system_state == SYSTEM_BOOTING ||
	    system_state == SYSTEM_RUNNING) {
		raw_spin_lock(&stop_lock);
<<<<<<< HEAD
		printk(KERN_CRIT "CPU%u: stopping\n", cpu);
=======
		pr_crit("CPU%u: stopping\n", cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dump_stack();
		raw_spin_unlock(&stop_lock);
	}

	set_cpu_online(cpu, false);

	local_fiq_disable();
	local_irq_disable();

<<<<<<< HEAD
	while (1)
		cpu_relax();
=======
	while (1) {
		cpu_relax();
		wfe();
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static DEFINE_PER_CPU(struct completion *, cpu_completion);

int register_ipi_completion(struct completion *completion, int cpu)
{
	per_cpu(cpu_completion, cpu) = completion;
	return IPI_COMPLETION;
}

static void ipi_complete(unsigned int cpu)
{
	complete(per_cpu(cpu_completion, cpu));
}

<<<<<<< HEAD
static cpumask_t backtrace_mask;
static DEFINE_RAW_SPINLOCK(backtrace_lock);

/* "in progress" flag of arch_trigger_all_cpu_backtrace */
static unsigned long backtrace_flag;

void smp_send_all_cpu_backtrace(void)
{
	unsigned int this_cpu = smp_processor_id();
	int i;

	if (test_and_set_bit(0, &backtrace_flag))
		/*
		 * If there is already a trigger_all_cpu_backtrace() in progress
		 * (backtrace_flag == 1), don't output double cpu dump infos.
		 */
		return;

	cpumask_copy(&backtrace_mask, cpu_online_mask);
	cpu_clear(this_cpu, backtrace_mask);

	pr_info("Backtrace for cpu %d (current):\n", this_cpu);
	dump_stack();

	pr_info("\nsending IPI to all other CPUs:\n");
	smp_cross_call(&backtrace_mask, IPI_CPU_BACKTRACE);

	/* Wait for up to 10 seconds for all other CPUs to do the backtrace */
	for (i = 0; i < 10 * 1000; i++) {
		if (cpumask_empty(&backtrace_mask))
			break;
		mdelay(1);
	}

	clear_bit(0, &backtrace_flag);
	smp_mb__after_clear_bit();
}

/*
 * ipi_cpu_backtrace - handle IPI from smp_send_all_cpu_backtrace()
 */
static void ipi_cpu_backtrace(unsigned int cpu, struct pt_regs *regs)
{
	if (cpu_isset(cpu, backtrace_mask)) {
		raw_spin_lock(&backtrace_lock);
		pr_warning("IPI backtrace for cpu %d\n", cpu);
		show_regs(regs);
		raw_spin_unlock(&backtrace_lock);
		cpu_clear(cpu, backtrace_mask);
	}
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Main handler for inter-processor interrupts
 */
asmlinkage void __exception_irq_entry do_IPI(int ipinr, struct pt_regs *regs)
{
	handle_IPI(ipinr, regs);
}

void handle_IPI(int ipinr, struct pt_regs *regs)
{
	unsigned int cpu = smp_processor_id();
	struct pt_regs *old_regs = set_irq_regs(regs);

<<<<<<< HEAD
	if (ipinr < NR_IPI)
		__inc_irq_stat(cpu, ipi_irqs[ipinr]);

	trace_arm_ipi_entry(ipinr);
=======
	if ((unsigned)ipinr < NR_IPI) {
		trace_ipi_entry_rcuidle(ipi_types[ipinr]);
		__inc_irq_stat(cpu, ipi_irqs[ipinr]);
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	switch (ipinr) {
	case IPI_WAKEUP:
		break;

#ifdef CONFIG_GENERIC_CLOCKEVENTS_BROADCAST
	case IPI_TIMER:
		irq_enter();
		tick_receive_broadcast();
		irq_exit();
		break;
#endif

	case IPI_RESCHEDULE:
		scheduler_ipi();
		break;

	case IPI_CALL_FUNC:
		irq_enter();
		generic_smp_call_function_interrupt();
		irq_exit();
		break;

<<<<<<< HEAD
	case IPI_CALL_FUNC_SINGLE:
		irq_enter();
		generic_smp_call_function_single_interrupt();
		irq_exit();
		break;

	case IPI_CPU_STOP:
		irq_enter();
		ipi_cpu_stop(cpu);
		irq_exit();
		break;
=======
	case IPI_CPU_STOP:
		irq_enter();
		ipi_cpu_stop(cpu);
		irq_exit();
		break;

#ifdef CONFIG_IRQ_WORK
	case IPI_IRQ_WORK:
		irq_enter();
		irq_work_run();
		irq_exit();
		break;
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	case IPI_COMPLETION:
		irq_enter();
		ipi_complete(cpu);
		irq_exit();
		break;

	case IPI_CPU_BACKTRACE:
<<<<<<< HEAD
		ipi_cpu_backtrace(cpu, regs);
		break;
#ifdef CONFIG_SECURE_EXTENSION
           case IPI_NOTIFY_FUNC:
                printk("come into ipi notify handler\n");
                if(secure_notify_handler)
                secure_notify_handler(regs);
                break;
#endif

	default:
		printk(KERN_CRIT "CPU%u: Unknown IPI message 0x%x\n",
		       cpu, ipinr);
		break;
	}
	trace_arm_ipi_exit(ipinr);
=======
		printk_nmi_enter();
		irq_enter();
		nmi_cpu_backtrace(regs);
		irq_exit();
		printk_nmi_exit();
		break;

	default:
		pr_crit("CPU%u: Unknown IPI message 0x%x\n",
		        cpu, ipinr);
		break;
	}

	if ((unsigned)ipinr < NR_IPI)
		trace_ipi_exit_rcuidle(ipi_types[ipinr]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	set_irq_regs(old_regs);
}

void smp_send_reschedule(int cpu)
{
	smp_cross_call(cpumask_of(cpu), IPI_RESCHEDULE);
}

void smp_send_stop(void)
{
	unsigned long timeout;
	struct cpumask mask;

	cpumask_copy(&mask, cpu_online_mask);
	cpumask_clear_cpu(smp_processor_id(), &mask);
	if (!cpumask_empty(&mask))
		smp_cross_call(&mask, IPI_CPU_STOP);

	/* Wait up to one second for other CPUs to stop */
	timeout = USEC_PER_SEC;
	while (num_online_cpus() > 1 && timeout--)
		udelay(1);

	if (num_online_cpus() > 1)
<<<<<<< HEAD
		pr_warning("SMP: failed to stop secondary CPUs\n");
=======
		pr_warn("SMP: failed to stop secondary CPUs\n");
}

/* In case panic() and panic() called at the same time on CPU1 and CPU2,
 * and CPU 1 calls panic_smp_self_stop() before crash_smp_send_stop()
 * CPU1 can't receive the ipi irqs from CPU2, CPU1 will be always online,
 * kdump fails. So split out the panic_smp_self_stop() and add
 * set_cpu_online(smp_processor_id(), false).
 */
void panic_smp_self_stop(void)
{
	pr_debug("CPU %u will stop doing anything useful since another CPU has paniced\n",
	         smp_processor_id());
	set_cpu_online(smp_processor_id(), false);
	while (1)
		cpu_relax();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * not supported here
 */
int setup_profiling_timer(unsigned int multiplier)
{
	return -EINVAL;
}

#ifdef CONFIG_CPU_FREQ

static DEFINE_PER_CPU(unsigned long, l_p_j_ref);
static DEFINE_PER_CPU(unsigned long, l_p_j_ref_freq);
static unsigned long global_l_p_j_ref;
static unsigned long global_l_p_j_ref_freq;

static int cpufreq_callback(struct notifier_block *nb,
					unsigned long val, void *data)
{
	struct cpufreq_freqs *freq = data;
	int cpu = freq->cpu;

	if (freq->flags & CPUFREQ_CONST_LOOPS)
		return NOTIFY_OK;

	if (!per_cpu(l_p_j_ref, cpu)) {
		per_cpu(l_p_j_ref, cpu) =
			per_cpu(cpu_data, cpu).loops_per_jiffy;
		per_cpu(l_p_j_ref_freq, cpu) = freq->old;
		if (!global_l_p_j_ref) {
			global_l_p_j_ref = loops_per_jiffy;
			global_l_p_j_ref_freq = freq->old;
		}
	}

	if ((val == CPUFREQ_PRECHANGE  && freq->old < freq->new) ||
<<<<<<< HEAD
	    (val == CPUFREQ_POSTCHANGE && freq->old > freq->new) ||
	    (val == CPUFREQ_RESUMECHANGE || val == CPUFREQ_SUSPENDCHANGE)) {
=======
	    (val == CPUFREQ_POSTCHANGE && freq->old > freq->new)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		loops_per_jiffy = cpufreq_scale(global_l_p_j_ref,
						global_l_p_j_ref_freq,
						freq->new);
		per_cpu(cpu_data, cpu).loops_per_jiffy =
			cpufreq_scale(per_cpu(l_p_j_ref, cpu),
					per_cpu(l_p_j_ref_freq, cpu),
					freq->new);
	}
	return NOTIFY_OK;
}

static struct notifier_block cpufreq_notifier = {
	.notifier_call  = cpufreq_callback,
};

<<<<<<< HEAD
/*HI3630: flush all cpu all cache++*/
#ifdef HI3630_FC_SKIPOFFCORE
static int hi3630_fc_init(void);
#endif
/*HI3630: flush all cpu all cache--*/

static int __init register_cpufreq_notifier(void)
{
/*HI3630: flush all cpu all cache++*/
#ifdef HI3630_FC_SKIPOFFCORE
	hi3630_fc_init();
#endif
/*HI3630: flush all cpu all cache--*/
=======
static int __init register_cpufreq_notifier(void)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return cpufreq_register_notifier(&cpufreq_notifier,
						CPUFREQ_TRANSITION_NOTIFIER);
}
core_initcall(register_cpufreq_notifier);
<<<<<<< HEAD
#endif

#ifdef CONFIG_SECURE_EXTENSION
int register_secure_notify_handler(ipi_secure_notify_handler handler)
{
        secure_notify_handler = handler;
            return 0;
}

int unregister_secure_notify_handler(void)
{
        secure_notify_handler = NULL;
            return 0;
}


EXPORT_SYMBOL(register_secure_notify_handler);
EXPORT_SYMBOL(unregister_secure_notify_handler);

#endif


#if defined(CONFIG_ARCH_HI3630)
/*HI3630: flush all cpu all cache++*/
#ifdef HI3630_FC_DEBUG
#define HI3630_FC_PRINTK(format, args...)		do { printk(KERN_ERR format, args); } while (0)
/*#define HI3630_FC_PRINTK(x...)				printk(x)*/
#else
#define HI3630_FC_PRINTK(format, args...)		do { ; } while (0)
#endif

#ifdef HI3630_FC_SKIPOFFCORE
#define HI3630_FC_REG_SCBD4_OFFSET		(0x324)
#define HI3630_FC_GET_OFFSTATUS(x)		(((x) >> 16) & 0xFF)
static void __iomem *Hi3630_FC_SC_BaseAddr = NULL;

static int hi3630_fc_init(void)
{
	struct device_node *np = NULL;

	HI3630_FC_PRINTK("function=%s, line=%d, hi3630_fc_init++\n", __FUNCTION__, __LINE__);
	np = of_find_compatible_node(NULL, NULL, "hisilicon,sysctrl");
	if (np == NULL) {
		pr_err("%s: ERROR - hisilicon,sysctrl No compatible node found!\n", __FUNCTION__);
		return -ENODEV;
	}
	Hi3630_FC_SC_BaseAddr = of_iomap(np, 0);
	if (Hi3630_FC_SC_BaseAddr == NULL) {
		pr_err("%s: ERROR - Failed to get sysctrl base address!\n", __FUNCTION__);
		return -ENXIO;
	}
	HI3630_FC_PRINTK("function=%s, line=%d, hi3630_fc_init-- Hi3630_FC_SC_BaseAddr = 0x%x\n", __FUNCTION__, __LINE__, (unsigned int)Hi3630_FC_SC_BaseAddr);
	return 0;
}

static void hi3630_fc_get_cpumask_running(struct cpumask *dstp)
{
	unsigned int cpuoff_value;
	unsigned int cpuoff_status;
	unsigned int core_idx;

	cpumask_copy(dstp, cpu_online_mask);
	if (Hi3630_FC_SC_BaseAddr == NULL) {
		pr_err("%s: ERROR -Hi3630_FC_SC_BaseAddr is NULL!\n", __FUNCTION__);
		return;
	}
	/*cpuoff_value: bit0~3 A15, bit4~7 A7*/
	cpuoff_value = HI3630_FC_GET_OFFSTATUS(readl(Hi3630_FC_SC_BaseAddr + HI3630_FC_REG_SCBD4_OFFSET));
	/*cpuoff_status: bit0~7, cpu0~7*/
	cpuoff_status = ((cpuoff_value & 0x0F) << 4) + ((cpuoff_value & 0xF0) >> 4);
	HI3630_FC_PRINTK("function=%s, line=%d, cpuoff_value = 0x%x, cpuoff_status = 0x%x\n", __FUNCTION__, __LINE__, cpuoff_value, cpuoff_status);

	for(core_idx = 0; core_idx < 8; core_idx++) {
		/*0: cpu off, 1:cpu non-off*/
		if((cpuoff_status & 0x01) == 0) {
			HI3630_FC_PRINTK("function=%s, line=%d, cpumask_clear_cpu core_idx = 0x%x\n", __FUNCTION__, __LINE__, core_idx);
			cpumask_clear_cpu(core_idx, dstp);
		}
		cpuoff_status = cpuoff_status >> 1;
	};
	return;
}
#endif

#ifdef HI3630_FC_LOUIS
#define HI3630_FC_CLUSTER_CORENUM		(4)

 static void hi3630_fc_onecpu_cache_louis(void *info)
{
#ifdef HI3630_FC_DEBUG
	preempt_disable();
	HI3630_FC_PRINTK("function=%s, line=%d, onecpu = 0x%x\n", __FUNCTION__, __LINE__, smp_processor_id());
#endif

	flush_cache_louis();

#ifdef HI3630_FC_DEBUG
	preempt_enable();
#endif
}
#endif

static void hi3630_fc_onecpu_cache_all(void *info)
{
#ifdef HI3630_FC_DEBUG
	preempt_disable();
	HI3630_FC_PRINTK("function=%s, line=%d, onecpu = 0x%x\n", __FUNCTION__, __LINE__, smp_processor_id());
#endif

        flush_cache_all();

#ifdef HI3630_FC_DEBUG
	preempt_enable();
#endif
}

void hi3630_fc_allcpu_allcache(void)
{
	struct cpumask cm_fc_general;
#ifdef HI3630_FC_LOUIS
	unsigned int this_cpu;
	struct cpumask cm_fc_special;
	unsigned int core_min_idx, core_max_idx;
#endif

	preempt_disable();
	//cpu_hotplug_driver_lock();
	/*1. get and check running cpumask*/
	HI3630_FC_PRINTK("function=%s, line=%d, smp_processor_id() = 0x%x, num_online_cpus() = 0x%x, nr_cpu_ids = 0x%x\n", __FUNCTION__, __LINE__, smp_processor_id(), num_online_cpus(), nr_cpu_ids);

#ifdef HI3630_FC_SKIPOFFCORE
	hi3630_fc_get_cpumask_running(&cm_fc_general);
#else
	cpumask_copy(&cm_fc_general, cpu_online_mask);
#endif

#ifdef HI3630_FC_DEBUG
	if (cpumask_test_cpu(smp_processor_id(), &cm_fc_general) == 0) {
		pr_err("function=%s, ERROR - this_cpu = 0x%x is not running cpu!\n", __FUNCTION__, smp_processor_id());
		cpumask_copy(&cm_fc_general, cpu_online_mask);
		WARN_ON(1);
	}
#endif

#ifdef HI3630_FC_LOUIS
	this_cpu = smp_processor_id();
	/*2. get general cpumask and special cpumask*/
	cpumask_clear(&cm_fc_special);
	if (this_cpu < HI3630_FC_CLUSTER_CORENUM) {
		core_min_idx = HI3630_FC_CLUSTER_CORENUM;
		core_max_idx = nr_cpu_ids;
	} else {
		core_min_idx = 0;
		core_max_idx = HI3630_FC_CLUSTER_CORENUM;
	}
	for (; core_min_idx < core_max_idx; core_min_idx++) {
		HI3630_FC_PRINTK("function=%s, line=%d,  core_min_idx = 0x%x\n", __FUNCTION__, __LINE__, core_min_idx);
		if (cpumask_test_cpu(core_min_idx, &cm_fc_general)) {
			cpumask_set_cpu(core_min_idx, &cm_fc_special);
			cpumask_clear_cpu(core_min_idx, &cm_fc_general);
			HI3630_FC_PRINTK("function=%s, line=%d,  set special cpumask and clear general cpumask: core_min_idx = 0x%x\n", __FUNCTION__, __LINE__, core_min_idx);
			break;
		}
	}
	cpumask_set_cpu(this_cpu, &cm_fc_special);
	cpumask_clear_cpu(this_cpu, &cm_fc_general);
	HI3630_FC_PRINTK("function=%s, line=%d,  set special cpumask and clear general cpumask: this_cpu = 0x%x\n", __FUNCTION__, __LINE__, this_cpu);

	/*flush cache*/
	on_each_cpu_mask(&cm_fc_general, hi3630_fc_onecpu_cache_louis, NULL, 1);
	on_each_cpu_mask(&cm_fc_special, hi3630_fc_onecpu_cache_all, NULL, 1);
#else
	/*flush cache*/
	on_each_cpu_mask(&cm_fc_general, hi3630_fc_onecpu_cache_all, NULL, 1);
#endif
	HI3630_FC_PRINTK("function=%s, line=%d, on_each_cpu_mask is finished\n", __FUNCTION__, __LINE__);
	//cpu_hotplug_driver_unlock();
	preempt_enable();
}
/*HI3630: flush all cpu all cache--*/
#endif
=======

#endif

static void raise_nmi(cpumask_t *mask)
{
	__smp_cross_call(mask, IPI_CPU_BACKTRACE);
}

void arch_trigger_cpumask_backtrace(const cpumask_t *mask, bool exclude_self)
{
	nmi_trigger_cpumask_backtrace(mask, exclude_self, raise_nmi);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
