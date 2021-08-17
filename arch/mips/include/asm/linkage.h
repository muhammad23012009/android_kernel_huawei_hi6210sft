#ifndef __ASM_LINKAGE_H
#define __ASM_LINKAGE_H

#ifdef __ASSEMBLY__
#include <asm/asm.h>
#endif

<<<<<<< HEAD
#define __weak __attribute__((weak))
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define cond_syscall(x) asm(".weak\t" #x "\n" #x "\t=\tsys_ni_syscall")
#define SYSCALL_ALIAS(alias, name)					\
	asm ( #alias " = " #name "\n\t.globl " #alias)

#endif
