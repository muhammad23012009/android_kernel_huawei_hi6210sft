/*
 * bitops.h: Bit string operations on the V9.
 *
 * Copyright 1996, 1997 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef _SPARC64_BITOPS_H
#define _SPARC64_BITOPS_H

#ifndef _LINUX_BITOPS_H
#error only <linux/bitops.h> can be included directly
#endif

#include <linux/compiler.h>
#include <asm/byteorder.h>
<<<<<<< HEAD

extern int test_and_set_bit(unsigned long nr, volatile unsigned long *addr);
extern int test_and_clear_bit(unsigned long nr, volatile unsigned long *addr);
extern int test_and_change_bit(unsigned long nr, volatile unsigned long *addr);
extern void set_bit(unsigned long nr, volatile unsigned long *addr);
extern void clear_bit(unsigned long nr, volatile unsigned long *addr);
extern void change_bit(unsigned long nr, volatile unsigned long *addr);

#include <asm-generic/bitops/non-atomic.h>

#define smp_mb__before_clear_bit()	barrier()
#define smp_mb__after_clear_bit()	barrier()

=======
#include <asm/barrier.h>

int test_and_set_bit(unsigned long nr, volatile unsigned long *addr);
int test_and_clear_bit(unsigned long nr, volatile unsigned long *addr);
int test_and_change_bit(unsigned long nr, volatile unsigned long *addr);
void set_bit(unsigned long nr, volatile unsigned long *addr);
void clear_bit(unsigned long nr, volatile unsigned long *addr);
void change_bit(unsigned long nr, volatile unsigned long *addr);

#include <asm-generic/bitops/non-atomic.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/bitops/fls.h>
#include <asm-generic/bitops/__fls.h>
#include <asm-generic/bitops/fls64.h>

#ifdef __KERNEL__

<<<<<<< HEAD
extern int ffs(int x);
extern unsigned long __ffs(unsigned long);
=======
int ffs(int x);
unsigned long __ffs(unsigned long);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm-generic/bitops/ffz.h>
#include <asm-generic/bitops/sched.h>

/*
 * hweightN: returns the hamming weight (i.e. the number
 * of bits set) of a N-bit word
 */

<<<<<<< HEAD
extern unsigned long __arch_hweight64(__u64 w);
extern unsigned int __arch_hweight32(unsigned int w);
extern unsigned int __arch_hweight16(unsigned int w);
extern unsigned int __arch_hweight8(unsigned int w);
=======
unsigned long __arch_hweight64(__u64 w);
unsigned int __arch_hweight32(unsigned int w);
unsigned int __arch_hweight16(unsigned int w);
unsigned int __arch_hweight8(unsigned int w);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm-generic/bitops/const_hweight.h>
#include <asm-generic/bitops/lock.h>
#endif /* __KERNEL__ */

#include <asm-generic/bitops/find.h>

#ifdef __KERNEL__

#include <asm-generic/bitops/le.h>

#include <asm-generic/bitops/ext2-atomic-setbit.h>

#endif /* __KERNEL__ */

#endif /* defined(_SPARC64_BITOPS_H) */
