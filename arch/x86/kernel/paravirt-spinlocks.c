/*
 * Split spinlock implementation out into its own file, so it can be
 * compiled in a FTRACE-compatible way.
 */
#include <linux/spinlock.h>
<<<<<<< HEAD
#include <linux/module.h>

#include <asm/paravirt.h>

static inline void
default_spin_lock_flags(arch_spinlock_t *lock, unsigned long flags)
{
	arch_spin_lock(lock);
=======
#include <linux/export.h>
#include <linux/jump_label.h>

#include <asm/paravirt.h>

__visible void __native_queued_spin_unlock(struct qspinlock *lock)
{
	native_queued_spin_unlock(lock);
}

PV_CALLEE_SAVE_REGS_THUNK(__native_queued_spin_unlock);

bool pv_is_native_spin_unlock(void)
{
	return pv_lock_ops.queued_spin_unlock.func ==
		__raw_callee_save___native_queued_spin_unlock;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

struct pv_lock_ops pv_lock_ops = {
#ifdef CONFIG_SMP
<<<<<<< HEAD
	.spin_is_locked = __ticket_spin_is_locked,
	.spin_is_contended = __ticket_spin_is_contended,

	.spin_lock = __ticket_spin_lock,
	.spin_lock_flags = default_spin_lock_flags,
	.spin_trylock = __ticket_spin_trylock,
	.spin_unlock = __ticket_spin_unlock,
#endif
};
EXPORT_SYMBOL(pv_lock_ops);

=======
	.queued_spin_lock_slowpath = native_queued_spin_lock_slowpath,
	.queued_spin_unlock = PV_CALLEE_SAVE(__native_queued_spin_unlock),
	.wait = paravirt_nop,
	.kick = paravirt_nop,
#endif /* SMP */
};
EXPORT_SYMBOL(pv_lock_ops);

struct static_key paravirt_ticketlocks_enabled = STATIC_KEY_INIT_FALSE;
EXPORT_SYMBOL(paravirt_ticketlocks_enabled);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
