#ifndef _SPARC_BUG_H
#define _SPARC_BUG_H

#ifdef CONFIG_BUG
#include <linux/compiler.h>

#ifdef CONFIG_DEBUG_BUGVERBOSE
<<<<<<< HEAD
extern void do_BUG(const char *file, int line);
#define BUG() do {					\
	do_BUG(__FILE__, __LINE__);			\
	__builtin_trap();				\
} while (0)
#else
#define BUG()		__builtin_trap()
=======
void do_BUG(const char *file, int line);
#define BUG() do {					\
	do_BUG(__FILE__, __LINE__);			\
	barrier_before_unreachable();			\
	__builtin_trap();				\
} while (0)
#else
#define BUG() do {					\
	barrier_before_unreachable();			\
	__builtin_trap();				\
} while (0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

#define HAVE_ARCH_BUG
#endif

#include <asm-generic/bug.h>

struct pt_regs;
<<<<<<< HEAD
extern void die_if_kernel(char *str, struct pt_regs *regs) __attribute__ ((noreturn));
=======
void __noreturn die_if_kernel(char *str, struct pt_regs *regs);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
