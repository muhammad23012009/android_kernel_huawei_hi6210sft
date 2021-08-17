#ifndef _LINUX_CONTEXT_TRACKING_H
#define _LINUX_CONTEXT_TRACKING_H

#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/percpu.h>
#include <linux/vtime.h>
#include <asm/ptrace.h>

struct context_tracking {
	/*
	 * When active is false, probes are unset in order
	 * to minimize overhead: TIF flags are cleared
	 * and calls to user_enter/exit are ignored. This
	 * may be further optimized using static keys.
	 */
	bool active;
	enum ctx_state {
		IN_KERNEL = 0,
		IN_USER,
	} state;
};

static inline void __guest_enter(void)
{
	/*
	 * This is running in ioctl context so we can avoid
	 * the call to vtime_account() with its unnecessary idle check.
	 */
	vtime_account_system(current);
	current->flags |= PF_VCPU;
}

static inline void __guest_exit(void)
{
	/*
	 * This is running in ioctl context so we can avoid
	 * the call to vtime_account() with its unnecessary idle check.
	 */
	vtime_account_system(current);
	current->flags &= ~PF_VCPU;
}

#ifdef CONFIG_CONTEXT_TRACKING
DECLARE_PER_CPU(struct context_tracking, context_tracking);

static inline bool context_tracking_in_user(void)
{
	return __this_cpu_read(context_tracking.state) == IN_USER;
}

static inline bool context_tracking_active(void)
{
	return __this_cpu_read(context_tracking.active);
}

extern void user_enter(void);
extern void user_exit(void);

extern void guest_enter(void);
extern void guest_exit(void);
=======
#include <linux/vtime.h>
#include <linux/context_tracking_state.h>
#include <asm/ptrace.h>


#ifdef CONFIG_CONTEXT_TRACKING
extern void context_tracking_cpu_set(int cpu);

/* Called with interrupts disabled.  */
extern void __context_tracking_enter(enum ctx_state state);
extern void __context_tracking_exit(enum ctx_state state);

extern void context_tracking_enter(enum ctx_state state);
extern void context_tracking_exit(enum ctx_state state);
extern void context_tracking_user_enter(void);
extern void context_tracking_user_exit(void);

static inline void user_enter(void)
{
	if (context_tracking_is_enabled())
		context_tracking_enter(CONTEXT_USER);

}
static inline void user_exit(void)
{
	if (context_tracking_is_enabled())
		context_tracking_exit(CONTEXT_USER);
}

/* Called with interrupts disabled.  */
static inline void user_enter_irqoff(void)
{
	if (context_tracking_is_enabled())
		__context_tracking_enter(CONTEXT_USER);

}
static inline void user_exit_irqoff(void)
{
	if (context_tracking_is_enabled())
		__context_tracking_exit(CONTEXT_USER);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline enum ctx_state exception_enter(void)
{
	enum ctx_state prev_ctx;

<<<<<<< HEAD
	prev_ctx = this_cpu_read(context_tracking.state);
	user_exit();
=======
	if (!context_tracking_is_enabled())
		return 0;

	prev_ctx = this_cpu_read(context_tracking.state);
	if (prev_ctx != CONTEXT_KERNEL)
		context_tracking_exit(prev_ctx);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return prev_ctx;
}

static inline void exception_exit(enum ctx_state prev_ctx)
{
<<<<<<< HEAD
	if (prev_ctx == IN_USER)
		user_enter();
}

extern void context_tracking_task_switch(struct task_struct *prev,
					 struct task_struct *next);
#else
static inline bool context_tracking_in_user(void) { return false; }
static inline void user_enter(void) { }
static inline void user_exit(void) { }

static inline void guest_enter(void)
{
	__guest_enter();
=======
	if (context_tracking_is_enabled()) {
		if (prev_ctx != CONTEXT_KERNEL)
			context_tracking_enter(prev_ctx);
	}
}


/**
 * ct_state() - return the current context tracking state if known
 *
 * Returns the current cpu's context tracking state if context tracking
 * is enabled.  If context tracking is disabled, returns
 * CONTEXT_DISABLED.  This should be used primarily for debugging.
 */
static inline enum ctx_state ct_state(void)
{
	return context_tracking_is_enabled() ?
		this_cpu_read(context_tracking.state) : CONTEXT_DISABLED;
}
#else
static inline void user_enter(void) { }
static inline void user_exit(void) { }
static inline void user_enter_irqoff(void) { }
static inline void user_exit_irqoff(void) { }
static inline enum ctx_state exception_enter(void) { return 0; }
static inline void exception_exit(enum ctx_state prev_ctx) { }
static inline enum ctx_state ct_state(void) { return CONTEXT_DISABLED; }
#endif /* !CONFIG_CONTEXT_TRACKING */

#define CT_WARN_ON(cond) WARN_ON(context_tracking_is_enabled() && (cond))

#ifdef CONFIG_CONTEXT_TRACKING_FORCE
extern void context_tracking_init(void);
#else
static inline void context_tracking_init(void) { }
#endif /* CONFIG_CONTEXT_TRACKING_FORCE */


#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
/* must be called with irqs disabled */
static inline void guest_enter_irqoff(void)
{
	if (vtime_accounting_cpu_enabled())
		vtime_guest_enter(current);
	else
		current->flags |= PF_VCPU;

	if (context_tracking_is_enabled())
		__context_tracking_enter(CONTEXT_GUEST);

	/* KVM does not hold any references to rcu protected data when it
	 * switches CPU into a guest mode. In fact switching to a guest mode
	 * is very similar to exiting to userspace from rcu point of view. In
	 * addition CPU may stay in a guest mode for quite a long time (up to
	 * one time slice). Lets treat guest mode as quiescent state, just like
	 * we do with user-mode execution.
	 */
	if (!context_tracking_cpu_is_enabled())
		rcu_virt_note_context_switch(smp_processor_id());
}

static inline void guest_exit_irqoff(void)
{
	if (context_tracking_is_enabled())
		__context_tracking_exit(CONTEXT_GUEST);

	if (vtime_accounting_cpu_enabled())
		vtime_guest_exit(current);
	else
		current->flags &= ~PF_VCPU;
}

#else
static inline void guest_enter_irqoff(void)
{
	/*
	 * This is running in ioctl context so its safe
	 * to assume that it's the stime pending cputime
	 * to flush.
	 */
	vtime_account_system(current);
	current->flags |= PF_VCPU;
	rcu_virt_note_context_switch(smp_processor_id());
}

static inline void guest_exit_irqoff(void)
{
	/* Flush the guest cputime we spent on the guest */
	vtime_account_system(current);
	current->flags &= ~PF_VCPU;
}
#endif /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */

static inline void guest_enter(void)
{
	unsigned long flags;

	local_irq_save(flags);
	guest_enter_irqoff();
	local_irq_restore(flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void guest_exit(void)
{
<<<<<<< HEAD
	__guest_exit();
}

static inline enum ctx_state exception_enter(void) { return 0; }
static inline void exception_exit(enum ctx_state prev_ctx) { }
static inline void context_tracking_task_switch(struct task_struct *prev,
						struct task_struct *next) { }
#endif /* !CONFIG_CONTEXT_TRACKING */
=======
	unsigned long flags;

	local_irq_save(flags);
	guest_exit_irqoff();
	local_irq_restore(flags);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
