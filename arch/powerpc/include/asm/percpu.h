#ifndef _ASM_POWERPC_PERCPU_H_
#define _ASM_POWERPC_PERCPU_H_
#ifdef __powerpc64__

/*
 * Same as asm-generic/percpu.h, except that we store the per cpu offset
 * in the paca. Based on the x86-64 implementation.
 */

#ifdef CONFIG_SMP

<<<<<<< HEAD
#include <asm/paca.h>

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define __my_cpu_offset local_paca->data_offset

#endif /* CONFIG_SMP */
#endif /* __powerpc64__ */

#include <asm-generic/percpu.h>

<<<<<<< HEAD
=======
#include <asm/paca.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _ASM_POWERPC_PERCPU_H_ */
