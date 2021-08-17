/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * vineetg: Jan 1011
 *  -sched_clock( ) no longer jiffies based. Uses the same clocksource
 *   as gtod
 *
 * Rajeshwarr/Vineetg: Mar 2008
 *  -Implemented CONFIG_GENERIC_TIME (rather deleted arch specific code)
 *   for arch independent gettimeofday()
 *  -Implemented CONFIG_GENERIC_CLOCKEVENTS as base for hrtimers
 *
 * Vineetg: Mar 2008: Forked off from time.c which now is time-jiff.c
 */

/* ARC700 has two 32bit independent prog Timers: TIMER0 and TIMER1
 * Each can programmed to go from @count to @limit and optionally
 * interrupt when that happens.
 * A write to Control Register clears the Interrupt
 *
 * We've designated TIMER0 for events (clockevents)
 * while TIMER1 for free running (clocksource)
 *
 * Newer ARC700 cores have 64bit clk fetching RTSC insn, preferred over TIMER1
<<<<<<< HEAD
 */

#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/timex.h>
#include <linux/profile.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <asm/irq.h>
#include <asm/arcregs.h>
#include <asm/clk.h>
#include <asm/mach_desc.h>

#define ARC_TIMER_MAX	0xFFFFFFFF

/********** Clock Source Device *********/

#ifdef CONFIG_ARC_HAS_RTSC

int __cpuinit arc_counter_setup(void)
{
	/* RTSC insn taps into cpu clk, needs no setup */

	/* For SMP, only allowed if cross-core-sync, hence usable as cs */
	return 1;
}

static cycle_t arc_counter_read(struct clocksource *cs)
=======
 * which however is currently broken
 */

#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/cpu.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <asm/irq.h>
#include <asm/arcregs.h>

#include <asm/mcip.h>

/* Timer related Aux registers */
#define ARC_REG_TIMER0_LIMIT	0x23	/* timer 0 limit */
#define ARC_REG_TIMER0_CTRL	0x22	/* timer 0 control */
#define ARC_REG_TIMER0_CNT	0x21	/* timer 0 count */
#define ARC_REG_TIMER1_LIMIT	0x102	/* timer 1 limit */
#define ARC_REG_TIMER1_CTRL	0x101	/* timer 1 control */
#define ARC_REG_TIMER1_CNT	0x100	/* timer 1 count */

#define TIMER_CTRL_IE	(1 << 0) /* Interrupt when Count reaches limit */
#define TIMER_CTRL_NH	(1 << 1) /* Count only when CPU NOT halted */

#define ARC_TIMER_MAX	0xFFFFFFFF

static unsigned long arc_timer_freq;

static int noinline arc_get_timer_clk(struct device_node *node)
{
	struct clk *clk;
	int ret;

	clk = of_clk_get(node, 0);
	if (IS_ERR(clk)) {
		pr_err("timer missing clk");
		return PTR_ERR(clk);
	}

	ret = clk_prepare_enable(clk);
	if (ret) {
		pr_err("Couldn't enable parent clk\n");
		return ret;
	}

	arc_timer_freq = clk_get_rate(clk);

	return 0;
}

/********** Clock Source Device *********/

#ifdef CONFIG_ARC_HAS_GFRC

static cycle_t arc_read_gfrc(struct clocksource *cs)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned long flags;
	union {
#ifdef CONFIG_CPU_BIG_ENDIAN
<<<<<<< HEAD
		struct { u32 high, low; };
#else
		struct { u32 low, high; };
=======
		struct { u32 h, l; };
#else
		struct { u32 l, h; };
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
		cycle_t  full;
	} stamp;

<<<<<<< HEAD
	flags = arch_local_irq_save();

	__asm__ __volatile(
	"	.extCoreRegister tsch, 58,  r, cannot_shortcut	\n"
	"	rtsc %0, 0	\n"
	"	mov  %1, 0	\n"
	: "=r" (stamp.low), "=r" (stamp.high));

	arch_local_irq_restore(flags);
=======
	local_irq_save(flags);

	__mcip_cmd(CMD_GFRC_READ_LO, 0);
	stamp.l = read_aux_reg(ARC_REG_MCIP_READBACK);

	__mcip_cmd(CMD_GFRC_READ_HI, 0);
	stamp.h = read_aux_reg(ARC_REG_MCIP_READBACK);

	local_irq_restore(flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return stamp.full;
}

<<<<<<< HEAD
static struct clocksource arc_counter = {
	.name   = "ARC RTSC",
	.rating = 300,
	.read   = arc_counter_read,
	.mask   = CLOCKSOURCE_MASK(32),
	.flags  = CLOCK_SOURCE_IS_CONTINUOUS,
};

#else /* !CONFIG_ARC_HAS_RTSC */

static bool is_usable_as_clocksource(void)
{
#ifdef CONFIG_SMP
	return 0;
#else
	return 1;
#endif
}

/*
 * set 32bit TIMER1 to keep counting monotonically and wraparound
 */
int __cpuinit arc_counter_setup(void)
{
	write_aux_reg(ARC_REG_TIMER1_LIMIT, ARC_TIMER_MAX);
	write_aux_reg(ARC_REG_TIMER1_CNT, 0);
	write_aux_reg(ARC_REG_TIMER1_CTRL, TIMER_CTRL_NH);

	return is_usable_as_clocksource();
}

static cycle_t arc_counter_read(struct clocksource *cs)
=======
static struct clocksource arc_counter_gfrc = {
	.name   = "ARConnect GFRC",
	.rating = 400,
	.read   = arc_read_gfrc,
	.mask   = CLOCKSOURCE_MASK(64),
	.flags  = CLOCK_SOURCE_IS_CONTINUOUS,
};

static int __init arc_cs_setup_gfrc(struct device_node *node)
{
	int exists = cpuinfo_arc700[0].extn.gfrc;
	int ret;

	if (WARN(!exists, "Global-64-bit-Ctr clocksource not detected"))
		return -ENXIO;

	ret = arc_get_timer_clk(node);
	if (ret)
		return ret;

	return clocksource_register_hz(&arc_counter_gfrc, arc_timer_freq);
}
CLOCKSOURCE_OF_DECLARE(arc_gfrc, "snps,archs-timer-gfrc", arc_cs_setup_gfrc);

#endif

#ifdef CONFIG_ARC_HAS_RTC

#define AUX_RTC_CTRL	0x103
#define AUX_RTC_LOW	0x104
#define AUX_RTC_HIGH	0x105

static cycle_t arc_read_rtc(struct clocksource *cs)
{
	unsigned long status;
	union {
#ifdef CONFIG_CPU_BIG_ENDIAN
		struct { u32 high, low; };
#else
		struct { u32 low, high; };
#endif
		cycle_t  full;
	} stamp;

	/*
	 * hardware has an internal state machine which tracks readout of
	 * low/high and updates the CTRL.status if
	 *  - interrupt/exception taken between the two reads
	 *  - high increments after low has been read
	 */
	do {
		stamp.low = read_aux_reg(AUX_RTC_LOW);
		stamp.high = read_aux_reg(AUX_RTC_HIGH);
		status = read_aux_reg(AUX_RTC_CTRL);
	} while (!(status & _BITUL(31)));

	return stamp.full;
}

static struct clocksource arc_counter_rtc = {
	.name   = "ARCv2 RTC",
	.rating = 350,
	.read   = arc_read_rtc,
	.mask   = CLOCKSOURCE_MASK(64),
	.flags  = CLOCK_SOURCE_IS_CONTINUOUS,
};

static int __init arc_cs_setup_rtc(struct device_node *node)
{
	int exists = cpuinfo_arc700[smp_processor_id()].extn.rtc;
	int ret;

	if (WARN(!exists, "Local-64-bit-Ctr clocksource not detected"))
		return -ENXIO;

	/* Local to CPU hence not usable in SMP */
	if (WARN(IS_ENABLED(CONFIG_SMP), "Local-64-bit-Ctr not usable in SMP"))
		return -EINVAL;

	ret = arc_get_timer_clk(node);
	if (ret)
		return ret;

	write_aux_reg(AUX_RTC_CTRL, 1);

	return clocksource_register_hz(&arc_counter_rtc, arc_timer_freq);
}
CLOCKSOURCE_OF_DECLARE(arc_rtc, "snps,archs-timer-rtc", arc_cs_setup_rtc);

#endif

/*
 * 32bit TIMER1 to keep counting monotonically and wraparound
 */

static cycle_t arc_read_timer1(struct clocksource *cs)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return (cycle_t) read_aux_reg(ARC_REG_TIMER1_CNT);
}

<<<<<<< HEAD
static struct clocksource arc_counter = {
	.name   = "ARC Timer1",
	.rating = 300,
	.read   = arc_counter_read,
=======
static struct clocksource arc_counter_timer1 = {
	.name   = "ARC Timer1",
	.rating = 300,
	.read   = arc_read_timer1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.mask   = CLOCKSOURCE_MASK(32),
	.flags  = CLOCK_SOURCE_IS_CONTINUOUS,
};

<<<<<<< HEAD
#endif

/********** Clock Event Device *********/

/*
 * Arm the timer to interrupt after @limit cycles
 * The distinction for oneshot/periodic is done in arc_event_timer_ack() below
 */
static void arc_timer_event_setup(unsigned int limit)
{
	write_aux_reg(ARC_REG_TIMER0_LIMIT, limit);
=======
static int __init arc_cs_setup_timer1(struct device_node *node)
{
	int ret;

	/* Local to CPU hence not usable in SMP */
	if (IS_ENABLED(CONFIG_SMP))
		return -EINVAL;

	ret = arc_get_timer_clk(node);
	if (ret)
		return ret;

	write_aux_reg(ARC_REG_TIMER1_LIMIT, ARC_TIMER_MAX);
	write_aux_reg(ARC_REG_TIMER1_CNT, 0);
	write_aux_reg(ARC_REG_TIMER1_CTRL, TIMER_CTRL_NH);

	return clocksource_register_hz(&arc_counter_timer1, arc_timer_freq);
}

/********** Clock Event Device *********/

static int arc_timer_irq;

/*
 * Arm the timer to interrupt after @cycles
 * The distinction for oneshot/periodic is done in arc_event_timer_ack() below
 */
static void arc_timer_event_setup(unsigned int cycles)
{
	write_aux_reg(ARC_REG_TIMER0_LIMIT, cycles);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	write_aux_reg(ARC_REG_TIMER0_CNT, 0);	/* start from 0 */

	write_aux_reg(ARC_REG_TIMER0_CTRL, TIMER_CTRL_IE | TIMER_CTRL_NH);
}

<<<<<<< HEAD
/*
 * Acknowledge the interrupt (oneshot) and optionally re-arm it (periodic)
 * -Any write to CTRL Reg will ack the intr (NH bit: Count when not halted)
 * -Rearming is done by setting the IE bit
 *
 * Small optimisation: Normal code would have been
 *   if (irq_reenable)
 *     CTRL_REG = (IE | NH);
 *   else
 *     CTRL_REG = NH;
 * However since IE is BIT0 we can fold the branch
 */
static void arc_timer_event_ack(unsigned int irq_reenable)
{
	write_aux_reg(ARC_REG_TIMER0_CTRL, irq_reenable | TIMER_CTRL_NH);
}
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int arc_clkevent_set_next_event(unsigned long delta,
				       struct clock_event_device *dev)
{
	arc_timer_event_setup(delta);
	return 0;
}

<<<<<<< HEAD
static void arc_clkevent_set_mode(enum clock_event_mode mode,
				  struct clock_event_device *dev)
{
	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		arc_timer_event_setup(arc_get_core_freq() / HZ);
		break;
	case CLOCK_EVT_MODE_ONESHOT:
		break;
	default:
		break;
	}

	return;
}

static DEFINE_PER_CPU(struct clock_event_device, arc_clockevent_device) = {
	.name		= "ARC Timer0",
	.features	= CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_PERIODIC,
	.mode		= CLOCK_EVT_MODE_UNUSED,
	.rating		= 300,
	.irq		= TIMER0_IRQ,	/* hardwired, no need for resources */
	.set_next_event = arc_clkevent_set_next_event,
	.set_mode	= arc_clkevent_set_mode,
=======
static int arc_clkevent_set_periodic(struct clock_event_device *dev)
{
	/*
	 * At X Hz, 1 sec = 1000ms -> X cycles;
	 *		      10ms -> X / 100 cycles
	 */
	arc_timer_event_setup(arc_timer_freq / HZ);
	return 0;
}

static DEFINE_PER_CPU(struct clock_event_device, arc_clockevent_device) = {
	.name			= "ARC Timer0",
	.features		= CLOCK_EVT_FEAT_ONESHOT |
				  CLOCK_EVT_FEAT_PERIODIC,
	.rating			= 300,
	.set_next_event		= arc_clkevent_set_next_event,
	.set_state_periodic	= arc_clkevent_set_periodic,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static irqreturn_t timer_irq_handler(int irq, void *dev_id)
{
<<<<<<< HEAD
	struct clock_event_device *clk = &__get_cpu_var(arc_clockevent_device);

	arc_timer_event_ack(clk->mode == CLOCK_EVT_MODE_PERIODIC);
	clk->event_handler(clk);
	return IRQ_HANDLED;
}

static struct irqaction arc_timer_irq = {
	.name    = "Timer0 (clock-evt-dev)",
	.flags   = IRQF_TIMER | IRQF_PERCPU,
	.handler = timer_irq_handler,
};

/*
 * Setup the local event timer for @cpu
 * N.B. weak so that some exotic ARC SoCs can completely override it
 */
void __attribute__((weak)) __cpuinit arc_local_timer_setup(unsigned int cpu)
{
	struct clock_event_device *clk = &per_cpu(arc_clockevent_device, cpu);

	clockevents_calc_mult_shift(clk, arc_get_core_freq(), 5);

	clk->max_delta_ns = clockevent_delta2ns(ARC_TIMER_MAX, clk);
	clk->cpumask = cpumask_of(cpu);

	clockevents_register_device(clk);

	/*
	 * setup the per-cpu timer IRQ handler - for all cpus
	 * For non boot CPU explicitly unmask at intc
	 * setup_irq() -> .. -> irq_startup() already does this on boot-cpu
	 */
	if (!cpu)
		setup_irq(TIMER0_IRQ, &arc_timer_irq);
	else
		arch_unmask_irq(TIMER0_IRQ);
}

/*
 * Called from start_kernel() - boot CPU only
 *
 * -Sets up h/w timers as applicable on boot cpu
 * -Also sets up any global state needed for timer subsystem:
 *    - for "counting" timer, registers a clocksource, usable across CPUs
 *      (provided that underlying counter h/w is synchronized across cores)
 *    - for "event" timer, sets up TIMER0 IRQ (as that is platform agnostic)
 */
void __init time_init(void)
{
	/*
	 * sets up the timekeeping free-flowing counter which also returns
	 * whether the counter is usable as clocksource
	 */
	if (arc_counter_setup())
		/*
		 * CLK upto 4.29 GHz can be safely represented in 32 bits
		 * because Max 32 bit number is 4,294,967,295
		 */
		clocksource_register_hz(&arc_counter, arc_get_core_freq());

	/* sets up the periodic event timer */
	arc_local_timer_setup(smp_processor_id());

	if (machine_desc->init_time)
		machine_desc->init_time();
=======
	/*
	 * Note that generic IRQ core could have passed @evt for @dev_id if
	 * irq_set_chip_and_handler() asked for handle_percpu_devid_irq()
	 */
	struct clock_event_device *evt = this_cpu_ptr(&arc_clockevent_device);
	int irq_reenable = clockevent_state_periodic(evt);

	/*
	 * Any write to CTRL reg ACks the interrupt, we rewrite the
	 * Count when [N]ot [H]alted bit.
	 * And re-arm it if perioid by [I]nterrupt [E]nable bit
	 */
	write_aux_reg(ARC_REG_TIMER0_CTRL, irq_reenable | TIMER_CTRL_NH);

	evt->event_handler(evt);

	return IRQ_HANDLED;
}


static int arc_timer_starting_cpu(unsigned int cpu)
{
	struct clock_event_device *evt = this_cpu_ptr(&arc_clockevent_device);

	evt->cpumask = cpumask_of(smp_processor_id());

	clockevents_config_and_register(evt, arc_timer_freq, 0, ARC_TIMER_MAX);
	enable_percpu_irq(arc_timer_irq, 0);
	return 0;
}

static int arc_timer_dying_cpu(unsigned int cpu)
{
	disable_percpu_irq(arc_timer_irq);
	return 0;
}

/*
 * clockevent setup for boot CPU
 */
static int __init arc_clockevent_setup(struct device_node *node)
{
	struct clock_event_device *evt = this_cpu_ptr(&arc_clockevent_device);
	int ret;

	arc_timer_irq = irq_of_parse_and_map(node, 0);
	if (arc_timer_irq <= 0) {
		pr_err("clockevent: missing irq");
		return -EINVAL;
	}

	ret = arc_get_timer_clk(node);
	if (ret) {
		pr_err("clockevent: missing clk");
		return ret;
	}

	/* Needs apriori irq_set_percpu_devid() done in intc map function */
	ret = request_percpu_irq(arc_timer_irq, timer_irq_handler,
				 "Timer0 (per-cpu-tick)", evt);
	if (ret) {
		pr_err("clockevent: unable to request irq\n");
		return ret;
	}

	ret = cpuhp_setup_state(CPUHP_AP_ARC_TIMER_STARTING,
				"AP_ARC_TIMER_STARTING",
				arc_timer_starting_cpu,
				arc_timer_dying_cpu);
	if (ret) {
		pr_err("Failed to setup hotplug state");
		return ret;
	}
	return 0;
}

static int __init arc_of_timer_init(struct device_node *np)
{
	static int init_count = 0;
	int ret;

	if (!init_count) {
		init_count = 1;
		ret = arc_clockevent_setup(np);
	} else {
		ret = arc_cs_setup_timer1(np);
	}

	return ret;
}
CLOCKSOURCE_OF_DECLARE(arc_clkevt, "snps,arc-timer", arc_of_timer_init);

/*
 * Called from start_kernel() - boot CPU only
 */
void __init time_init(void)
{
	of_clk_init(NULL);
	clocksource_probe();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
