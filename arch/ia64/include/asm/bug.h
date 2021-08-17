#ifndef _ASM_IA64_BUG_H
#define _ASM_IA64_BUG_H

#ifdef CONFIG_BUG
#define ia64_abort()	__builtin_trap()
<<<<<<< HEAD
#define BUG() do { printk("kernel BUG at %s:%d!\n", __FILE__, __LINE__); ia64_abort(); } while (0)
=======
#define BUG() do {						\
	printk("kernel BUG at %s:%d!\n", __FILE__, __LINE__);	\
	barrier_before_unreachable();				\
	ia64_abort();						\
} while (0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* should this BUG be made generic? */
#define HAVE_ARCH_BUG
#endif

#include <asm-generic/bug.h>

#endif
