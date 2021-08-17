#ifndef _ASM_METAG_BUG_H
#define _ASM_METAG_BUG_H

#include <asm-generic/bug.h>

struct pt_regs;

extern const char *trap_name(int trapno);
<<<<<<< HEAD
extern void die(const char *str, struct pt_regs *regs, long err,
		unsigned long addr) __attribute__ ((noreturn));
=======
extern void __noreturn die(const char *str, struct pt_regs *regs, long err,
		unsigned long addr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
