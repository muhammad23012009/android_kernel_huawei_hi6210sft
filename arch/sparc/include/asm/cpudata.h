#ifndef ___ASM_SPARC_CPUDATA_H
#define ___ASM_SPARC_CPUDATA_H
<<<<<<< HEAD
=======

#ifndef __ASSEMBLY__

#include <linux/threads.h>
#include <linux/percpu.h>

extern const struct seq_operations cpuinfo_op;

#endif /* !(__ASSEMBLY__) */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#if defined(__sparc__) && defined(__arch64__)
#include <asm/cpudata_64.h>
#else
#include <asm/cpudata_32.h>
#endif
#endif
