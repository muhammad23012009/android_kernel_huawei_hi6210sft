/*
 * Pull in the generic implementation for the mutex fastpath.
 *
 * TODO: implement optimized primitives instead, or leave the generic
 * implementation in place, or pick the atomic_xchg() based generic
 * implementation. (see asm-generic/mutex-xchg.h for details)
 */

#include <asm-generic/mutex-dec.h>
<<<<<<< HEAD

#define arch_mutex_cpu_relax()	barrier()
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
