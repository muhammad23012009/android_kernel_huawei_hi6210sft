/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2012 Tensilica Inc.
 */

#ifndef _XTENSA_SYSTEM_H
#define _XTENSA_SYSTEM_H

<<<<<<< HEAD
#define smp_read_barrier_depends() do { } while(0)
#define read_barrier_depends() do { } while(0)

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define mb()  ({ __asm__ __volatile__("memw" : : : "memory"); })
#define rmb() barrier()
#define wmb() mb()

<<<<<<< HEAD
#ifdef CONFIG_SMP
#error smp_* not defined
#else
#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#endif

#define set_mb(var, value)	do { var = value; mb(); } while (0)
=======
#define __smp_mb__before_atomic()		barrier()
#define __smp_mb__after_atomic()		barrier()

#include <asm-generic/barrier.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _XTENSA_SYSTEM_H */
