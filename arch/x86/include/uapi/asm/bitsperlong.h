#ifndef __ASM_X86_BITSPERLONG_H
#define __ASM_X86_BITSPERLONG_H

<<<<<<< HEAD
#ifdef __x86_64__
=======
#if defined(__x86_64__) && !defined(__ILP32__)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
# define __BITS_PER_LONG 64
#else
# define __BITS_PER_LONG 32
#endif

#include <asm-generic/bitsperlong.h>

#endif /* __ASM_X86_BITSPERLONG_H */

