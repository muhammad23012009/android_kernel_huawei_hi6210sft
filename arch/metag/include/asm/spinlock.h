#ifndef __ASM_SPINLOCK_H
#define __ASM_SPINLOCK_H

<<<<<<< HEAD
=======
#include <asm/barrier.h>
#include <asm/processor.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_METAG_ATOMICITY_LOCK1
#include <asm/spinlock_lock1.h>
#else
#include <asm/spinlock_lnkget.h>
#endif

<<<<<<< HEAD
#define arch_spin_unlock_wait(lock) \
	do { while (arch_spin_is_locked(lock)) cpu_relax(); } while (0)
=======
/*
 * both lock1 and lnkget are test-and-set spinlocks with 0 unlocked and 1
 * locked.
 */

static inline void arch_spin_unlock_wait(arch_spinlock_t *lock)
{
	smp_cond_load_acquire(&lock->lock, !VAL);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define arch_spin_lock_flags(lock, flags) arch_spin_lock(lock)

#define	arch_read_lock_flags(lock, flags) arch_read_lock(lock)
#define	arch_write_lock_flags(lock, flags) arch_write_lock(lock)

#define arch_spin_relax(lock)	cpu_relax()
#define arch_read_relax(lock)	cpu_relax()
#define arch_write_relax(lock)	cpu_relax()

#endif /* __ASM_SPINLOCK_H */
