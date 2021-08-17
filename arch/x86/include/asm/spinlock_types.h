#ifndef _ASM_X86_SPINLOCK_TYPES_H
#define _ASM_X86_SPINLOCK_TYPES_H

<<<<<<< HEAD
#ifndef __LINUX_SPINLOCK_TYPES_H
# error "please don't include this file directly"
#endif

#include <linux/types.h>

#if (CONFIG_NR_CPUS < 256)
=======
#include <linux/types.h>

#ifdef CONFIG_PARAVIRT_SPINLOCKS
#define __TICKET_LOCK_INC	2
#define TICKET_SLOWPATH_FLAG	((__ticket_t)1)
#else
#define __TICKET_LOCK_INC	1
#define TICKET_SLOWPATH_FLAG	((__ticket_t)0)
#endif

#if (CONFIG_NR_CPUS < (256 / __TICKET_LOCK_INC))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
typedef u8  __ticket_t;
typedef u16 __ticketpair_t;
#else
typedef u16 __ticket_t;
typedef u32 __ticketpair_t;
#endif

<<<<<<< HEAD
#define TICKET_SHIFT	(sizeof(__ticket_t) * 8)

typedef struct arch_spinlock {
	union {
		__ticketpair_t head_tail;
		struct __raw_tickets {
			__ticket_t head, tail;
		} tickets;
	};
} arch_spinlock_t;

#define __ARCH_SPIN_LOCK_UNLOCKED	{ { 0 } }

#include <asm/rwlock.h>
=======
#define TICKET_LOCK_INC	((__ticket_t)__TICKET_LOCK_INC)

#define TICKET_SHIFT	(sizeof(__ticket_t) * 8)

#include <asm-generic/qspinlock_types.h>

#include <asm-generic/qrwlock_types.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _ASM_X86_SPINLOCK_TYPES_H */
