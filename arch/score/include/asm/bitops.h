#ifndef _ASM_SCORE_BITOPS_H
#define _ASM_SCORE_BITOPS_H

#include <asm/byteorder.h> /* swab32 */
<<<<<<< HEAD

/*
 * clear_bit() doesn't provide any barrier for the compiler.
 */
#define smp_mb__before_clear_bit()	barrier()
#define smp_mb__after_clear_bit()	barrier()
=======
#include <asm/barrier.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm-generic/bitops.h>
#include <asm-generic/bitops/__fls.h>

#endif /* _ASM_SCORE_BITOPS_H */
