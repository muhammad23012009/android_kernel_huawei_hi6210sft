/*
 * arch/alpha/lib/dec_and_lock.c
 *
 * ll/sc version of atomic_dec_and_lock()
 * 
 */

#include <linux/spinlock.h>
#include <linux/atomic.h>
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

  asm (".text					\n\
	.global _atomic_dec_and_lock		\n\
	.ent _atomic_dec_and_lock		\n\
	.align	4				\n\
_atomic_dec_and_lock:				\n\
	.prologue 0				\n\
1:	ldl_l	$1, 0($16)			\n\
	subl	$1, 1, $1			\n\
	beq	$1, 2f				\n\
	stl_c	$1, 0($16)			\n\
	beq	$1, 4f				\n\
	mb					\n\
	clr	$0				\n\
	ret					\n\
2:	br	$29, 3f				\n\
3:	ldgp	$29, 0($29)			\n\
	br	$atomic_dec_and_lock_1..ng	\n\
	.subsection 2				\n\
4:	br	1b				\n\
	.previous				\n\
	.end _atomic_dec_and_lock");

static int __used atomic_dec_and_lock_1(atomic_t *atomic, spinlock_t *lock)
{
	/* Slow path */
	spin_lock(lock);
	if (atomic_dec_and_test(atomic))
		return 1;
	spin_unlock(lock);
	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(_atomic_dec_and_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
