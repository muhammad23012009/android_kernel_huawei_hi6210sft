/*
 *    Out of line spinlock code.
 *
 *    Copyright IBM Corp. 2004, 2006
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

#include <linux/types.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <asm/io.h>

<<<<<<< HEAD
int spin_retry = 1000;
=======
int spin_retry = -1;

static int __init spin_retry_init(void)
{
	if (spin_retry < 0)
		spin_retry = MACHINE_HAS_CAD ? 10 : 1000;
	return 0;
}
early_initcall(spin_retry_init);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * spin_retry= parameter
 */
static int __init spin_retry_setup(char *str)
{
	spin_retry = simple_strtoul(str, &str, 0);
	return 1;
}
__setup("spin_retry=", spin_retry_setup);

<<<<<<< HEAD
void arch_spin_lock_wait(arch_spinlock_t *lp)
{
	int count = spin_retry;
	unsigned int cpu = ~smp_processor_id();
	unsigned int owner;

	while (1) {
		owner = lp->owner_cpu;
		if (!owner || smp_vcpu_scheduled(~owner)) {
			for (count = spin_retry; count > 0; count--) {
				if (arch_spin_is_locked(lp))
					continue;
				if (_raw_compare_and_swap(&lp->owner_cpu, 0,
							  cpu) == 0)
					return;
			}
			if (MACHINE_IS_LPAR)
				continue;
		}
		owner = lp->owner_cpu;
		if (owner)
			smp_yield_cpu(~owner);
		if (_raw_compare_and_swap(&lp->owner_cpu, 0, cpu) == 0)
			return;
=======
static inline void _raw_compare_and_delay(unsigned int *lock, unsigned int old)
{
	asm(".insn rsy,0xeb0000000022,%0,0,%1" : : "d" (old), "Q" (*lock));
}

static inline int cpu_is_preempted(int cpu)
{
	if (test_cpu_flag_of(CIF_ENABLED_WAIT, cpu))
		return 0;
	if (smp_vcpu_scheduled(cpu))
		return 0;
	return 1;
}

void arch_spin_lock_wait(arch_spinlock_t *lp)
{
	unsigned int cpu = SPINLOCK_LOCKVAL;
	unsigned int owner;
	int count, first_diag;

	first_diag = 1;
	while (1) {
		owner = ACCESS_ONCE(lp->lock);
		/* Try to get the lock if it is free. */
		if (!owner) {
			if (_raw_compare_and_swap(&lp->lock, 0, cpu))
				return;
			continue;
		}
		/* First iteration: check if the lock owner is running. */
		if (first_diag && cpu_is_preempted(~owner)) {
			smp_yield_cpu(~owner);
			first_diag = 0;
			continue;
		}
		/* Loop for a while on the lock value. */
		count = spin_retry;
		do {
			if (MACHINE_HAS_CAD)
				_raw_compare_and_delay(&lp->lock, owner);
			owner = ACCESS_ONCE(lp->lock);
		} while (owner && count-- > 0);
		if (!owner)
			continue;
		/*
		 * For multiple layers of hypervisors, e.g. z/VM + LPAR
		 * yield the CPU unconditionally. For LPAR rely on the
		 * sense running status.
		 */
		if (!MACHINE_IS_LPAR || cpu_is_preempted(~owner)) {
			smp_yield_cpu(~owner);
			first_diag = 0;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}
EXPORT_SYMBOL(arch_spin_lock_wait);

void arch_spin_lock_wait_flags(arch_spinlock_t *lp, unsigned long flags)
{
<<<<<<< HEAD
	int count = spin_retry;
	unsigned int cpu = ~smp_processor_id();
	unsigned int owner;

	local_irq_restore(flags);
	while (1) {
		owner = lp->owner_cpu;
		if (!owner || smp_vcpu_scheduled(~owner)) {
			for (count = spin_retry; count > 0; count--) {
				if (arch_spin_is_locked(lp))
					continue;
				local_irq_disable();
				if (_raw_compare_and_swap(&lp->owner_cpu, 0,
							  cpu) == 0)
					return;
				local_irq_restore(flags);
			}
			if (MACHINE_IS_LPAR)
				continue;
		}
		owner = lp->owner_cpu;
		if (owner)
			smp_yield_cpu(~owner);
		local_irq_disable();
		if (_raw_compare_and_swap(&lp->owner_cpu, 0, cpu) == 0)
			return;
		local_irq_restore(flags);
=======
	unsigned int cpu = SPINLOCK_LOCKVAL;
	unsigned int owner;
	int count, first_diag;

	local_irq_restore(flags);
	first_diag = 1;
	while (1) {
		owner = ACCESS_ONCE(lp->lock);
		/* Try to get the lock if it is free. */
		if (!owner) {
			local_irq_disable();
			if (_raw_compare_and_swap(&lp->lock, 0, cpu))
				return;
			local_irq_restore(flags);
			continue;
		}
		/* Check if the lock owner is running. */
		if (first_diag && cpu_is_preempted(~owner)) {
			smp_yield_cpu(~owner);
			first_diag = 0;
			continue;
		}
		/* Loop for a while on the lock value. */
		count = spin_retry;
		do {
			if (MACHINE_HAS_CAD)
				_raw_compare_and_delay(&lp->lock, owner);
			owner = ACCESS_ONCE(lp->lock);
		} while (owner && count-- > 0);
		if (!owner)
			continue;
		/*
		 * For multiple layers of hypervisors, e.g. z/VM + LPAR
		 * yield the CPU unconditionally. For LPAR rely on the
		 * sense running status.
		 */
		if (!MACHINE_IS_LPAR || cpu_is_preempted(~owner)) {
			smp_yield_cpu(~owner);
			first_diag = 0;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}
EXPORT_SYMBOL(arch_spin_lock_wait_flags);

int arch_spin_trylock_retry(arch_spinlock_t *lp)
{
<<<<<<< HEAD
	unsigned int cpu = ~smp_processor_id();
	int count;

	for (count = spin_retry; count > 0; count--) {
		if (arch_spin_is_locked(lp))
			continue;
		if (_raw_compare_and_swap(&lp->owner_cpu, 0, cpu) == 0)
			return 1;
=======
	unsigned int cpu = SPINLOCK_LOCKVAL;
	unsigned int owner;
	int count;

	for (count = spin_retry; count > 0; count--) {
		owner = ACCESS_ONCE(lp->lock);
		/* Try to get the lock if it is free. */
		if (!owner) {
			if (_raw_compare_and_swap(&lp->lock, 0, cpu))
				return 1;
		} else if (MACHINE_HAS_CAD)
			_raw_compare_and_delay(&lp->lock, owner);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return 0;
}
EXPORT_SYMBOL(arch_spin_trylock_retry);

<<<<<<< HEAD
void arch_spin_relax(arch_spinlock_t *lock)
{
	unsigned int cpu = lock->owner_cpu;
	if (cpu != 0) {
		if (MACHINE_IS_VM || MACHINE_IS_KVM ||
		    !smp_vcpu_scheduled(~cpu))
			smp_yield_cpu(~cpu);
	}
}
EXPORT_SYMBOL(arch_spin_relax);

void _raw_read_lock_wait(arch_rwlock_t *rw)
{
	unsigned int old;
	int count = spin_retry;

	while (1) {
		if (count-- <= 0) {
			smp_yield();
			count = spin_retry;
		}
		if (!arch_read_can_lock(rw))
			continue;
		old = rw->lock & 0x7fffffffU;
		if (_raw_compare_and_swap(&rw->lock, old, old + 1) == old)
			return;
	}
}
EXPORT_SYMBOL(_raw_read_lock_wait);

void _raw_read_lock_wait_flags(arch_rwlock_t *rw, unsigned long flags)
{
	unsigned int old;
	int count = spin_retry;

	local_irq_restore(flags);
	while (1) {
		if (count-- <= 0) {
			smp_yield();
			count = spin_retry;
		}
		if (!arch_read_can_lock(rw))
			continue;
		old = rw->lock & 0x7fffffffU;
		local_irq_disable();
		if (_raw_compare_and_swap(&rw->lock, old, old + 1) == old)
			return;
	}
}
EXPORT_SYMBOL(_raw_read_lock_wait_flags);
=======
void _raw_read_lock_wait(arch_rwlock_t *rw)
{
	unsigned int owner, old;
	int count = spin_retry;

#ifdef CONFIG_HAVE_MARCH_Z196_FEATURES
	__RAW_LOCK(&rw->lock, -1, __RAW_OP_ADD);
#endif
	owner = 0;
	while (1) {
		if (count-- <= 0) {
			if (owner && cpu_is_preempted(~owner))
				smp_yield_cpu(~owner);
			count = spin_retry;
		}
		old = ACCESS_ONCE(rw->lock);
		owner = ACCESS_ONCE(rw->owner);
		if ((int) old < 0) {
			if (MACHINE_HAS_CAD)
				_raw_compare_and_delay(&rw->lock, old);
			continue;
		}
		if (_raw_compare_and_swap(&rw->lock, old, old + 1))
			return;
	}
}
EXPORT_SYMBOL(_raw_read_lock_wait);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int _raw_read_trylock_retry(arch_rwlock_t *rw)
{
	unsigned int old;
	int count = spin_retry;

	while (count-- > 0) {
<<<<<<< HEAD
		if (!arch_read_can_lock(rw))
			continue;
		old = rw->lock & 0x7fffffffU;
		if (_raw_compare_and_swap(&rw->lock, old, old + 1) == old)
=======
		old = ACCESS_ONCE(rw->lock);
		if ((int) old < 0) {
			if (MACHINE_HAS_CAD)
				_raw_compare_and_delay(&rw->lock, old);
			continue;
		}
		if (_raw_compare_and_swap(&rw->lock, old, old + 1))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return 1;
	}
	return 0;
}
EXPORT_SYMBOL(_raw_read_trylock_retry);

<<<<<<< HEAD
void _raw_write_lock_wait(arch_rwlock_t *rw)
{
	int count = spin_retry;

	while (1) {
		if (count-- <= 0) {
			smp_yield();
			count = spin_retry;
		}
		if (!arch_write_can_lock(rw))
			continue;
		if (_raw_compare_and_swap(&rw->lock, 0, 0x80000000) == 0)
			return;
=======
#ifdef CONFIG_HAVE_MARCH_Z196_FEATURES

void _raw_write_lock_wait(arch_rwlock_t *rw, unsigned int prev)
{
	unsigned int owner, old;
	int count = spin_retry;

	owner = 0;
	while (1) {
		if (count-- <= 0) {
			if (owner && cpu_is_preempted(~owner))
				smp_yield_cpu(~owner);
			count = spin_retry;
		}
		old = ACCESS_ONCE(rw->lock);
		owner = ACCESS_ONCE(rw->owner);
		smp_mb();
		if ((int) old >= 0) {
			prev = __RAW_LOCK(&rw->lock, 0x80000000, __RAW_OP_OR);
			old = prev;
		}
		if ((old & 0x7fffffff) == 0 && (int) prev >= 0)
			break;
		if (MACHINE_HAS_CAD)
			_raw_compare_and_delay(&rw->lock, old);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}
EXPORT_SYMBOL(_raw_write_lock_wait);

<<<<<<< HEAD
void _raw_write_lock_wait_flags(arch_rwlock_t *rw, unsigned long flags)
{
	int count = spin_retry;

	local_irq_restore(flags);
	while (1) {
		if (count-- <= 0) {
			smp_yield();
			count = spin_retry;
		}
		if (!arch_write_can_lock(rw))
			continue;
		local_irq_disable();
		if (_raw_compare_and_swap(&rw->lock, 0, 0x80000000) == 0)
			return;
	}
}
EXPORT_SYMBOL(_raw_write_lock_wait_flags);

int _raw_write_trylock_retry(arch_rwlock_t *rw)
{
	int count = spin_retry;

	while (count-- > 0) {
		if (!arch_write_can_lock(rw))
			continue;
		if (_raw_compare_and_swap(&rw->lock, 0, 0x80000000) == 0)
=======
#else /* CONFIG_HAVE_MARCH_Z196_FEATURES */

void _raw_write_lock_wait(arch_rwlock_t *rw)
{
	unsigned int owner, old, prev;
	int count = spin_retry;

	prev = 0x80000000;
	owner = 0;
	while (1) {
		if (count-- <= 0) {
			if (owner && cpu_is_preempted(~owner))
				smp_yield_cpu(~owner);
			count = spin_retry;
		}
		old = ACCESS_ONCE(rw->lock);
		owner = ACCESS_ONCE(rw->owner);
		if ((int) old >= 0 &&
		    _raw_compare_and_swap(&rw->lock, old, old | 0x80000000))
			prev = old;
		else
			smp_mb();
		if ((old & 0x7fffffff) == 0 && (int) prev >= 0)
			break;
		if (MACHINE_HAS_CAD)
			_raw_compare_and_delay(&rw->lock, old);
	}
}
EXPORT_SYMBOL(_raw_write_lock_wait);

#endif /* CONFIG_HAVE_MARCH_Z196_FEATURES */

int _raw_write_trylock_retry(arch_rwlock_t *rw)
{
	unsigned int old;
	int count = spin_retry;

	while (count-- > 0) {
		old = ACCESS_ONCE(rw->lock);
		if (old) {
			if (MACHINE_HAS_CAD)
				_raw_compare_and_delay(&rw->lock, old);
			continue;
		}
		if (_raw_compare_and_swap(&rw->lock, 0, 0x80000000))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return 1;
	}
	return 0;
}
EXPORT_SYMBOL(_raw_write_trylock_retry);
<<<<<<< HEAD
=======

void arch_lock_relax(unsigned int cpu)
{
	if (!cpu)
		return;
	if (MACHINE_IS_LPAR && !cpu_is_preempted(~cpu))
		return;
	smp_yield_cpu(~cpu);
}
EXPORT_SYMBOL(arch_lock_relax);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
