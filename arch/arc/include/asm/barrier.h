/*
<<<<<<< HEAD
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
=======
 * Copyright (C) 2014-15 Synopsys, Inc. (www.synopsys.com)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_BARRIER_H
#define __ASM_BARRIER_H

<<<<<<< HEAD
#ifndef __ASSEMBLY__

/* TODO-vineetg: Need to see what this does, don't we need sync anywhere */
#define mb() __asm__ __volatile__ ("" : : : "memory")
#define rmb() mb()
#define wmb() mb()
#define set_mb(var, value)  do { var = value; mb(); } while (0)
#define set_wmb(var, value) do { var = value; wmb(); } while (0)
#define read_barrier_depends()  mb()

/* TODO-vineetg verify the correctness of macros here */
#ifdef CONFIG_SMP
#define smp_mb()        mb()
#define smp_rmb()       rmb()
#define smp_wmb()       wmb()
#else
#define smp_mb()        barrier()
#define smp_rmb()       barrier()
#define smp_wmb()       barrier()
#endif

#define smp_mb__before_atomic_dec()	barrier()
#define smp_mb__after_atomic_dec()	barrier()
#define smp_mb__before_atomic_inc()	barrier()
#define smp_mb__after_atomic_inc()	barrier()

#define smp_read_barrier_depends()      do { } while (0)

#endif

=======
#ifdef CONFIG_ISA_ARCV2

/*
 * ARCv2 based HS38 cores are in-order issue, but still weakly ordered
 * due to micro-arch buffering/queuing of load/store, cache hit vs. miss ...
 *
 * Explicit barrier provided by DMB instruction
 *  - Operand supports fine grained load/store/load+store semantics
 *  - Ensures that selected memory operation issued before it will complete
 *    before any subsequent memory operation of same type
 *  - DMB guarantees SMP as well as local barrier semantics
 *    (asm-generic/barrier.h ensures sane smp_*mb if not defined here, i.e.
 *    UP: barrier(), SMP: smp_*mb == *mb)
 *  - DSYNC provides DMB+completion_of_cache_bpu_maintenance_ops hence not needed
 *    in the general case. Plus it only provides full barrier.
 */

#define mb()	asm volatile("dmb 3\n" : : : "memory")
#define rmb()	asm volatile("dmb 1\n" : : : "memory")
#define wmb()	asm volatile("dmb 2\n" : : : "memory")

#elif !defined(CONFIG_ARC_PLAT_EZNPS)  /* CONFIG_ISA_ARCOMPACT */

/*
 * ARCompact based cores (ARC700) only have SYNC instruction which is super
 * heavy weight as it flushes the pipeline as well.
 * There are no real SMP implementations of such cores.
 */

#define mb()	asm volatile("sync\n" : : : "memory")

#else	/* CONFIG_ARC_PLAT_EZNPS */

#include <plat/ctop.h>

#define mb()	asm volatile (".word %0" : : "i"(CTOP_INST_SCHD_RW) : "memory")
#define rmb()	asm volatile (".word %0" : : "i"(CTOP_INST_SCHD_RD) : "memory")

#endif

#include <asm-generic/barrier.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
