#ifndef __ASM_PARISC_BITSPERLONG_H
#define __ASM_PARISC_BITSPERLONG_H

<<<<<<< HEAD
/*
 * using CONFIG_* outside of __KERNEL__ is wrong,
 * __LP64__ was also removed from headers, so what
 * is the right approach on parisc?
 *	-arnd
 */
#if (defined(__KERNEL__) && defined(CONFIG_64BIT)) || defined (__LP64__)
=======
#if defined(__LP64__)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define __BITS_PER_LONG 64
#else
#define __BITS_PER_LONG 32
#endif

#include <asm-generic/bitsperlong.h>

#endif /* __ASM_PARISC_BITSPERLONG_H */
