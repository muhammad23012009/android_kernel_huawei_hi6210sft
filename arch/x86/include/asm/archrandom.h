/*
 * This file is part of the Linux kernel.
 *
<<<<<<< HEAD
 * Copyright (c) 2011, Intel Corporation
=======
 * Copyright (c) 2011-2014, Intel Corporation
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Authors: Fenghua Yu <fenghua.yu@intel.com>,
 *          H. Peter Anvin <hpa@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef ASM_X86_ARCHRANDOM_H
#define ASM_X86_ARCHRANDOM_H

#include <asm/processor.h>
#include <asm/cpufeature.h>
<<<<<<< HEAD
#include <asm/alternative.h>
#include <asm/nops.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define RDRAND_RETRY_LOOPS	10

#define RDRAND_INT	".byte 0x0f,0xc7,0xf0"
<<<<<<< HEAD
#ifdef CONFIG_X86_64
# define RDRAND_LONG	".byte 0x48,0x0f,0xc7,0xf0"
#else
# define RDRAND_LONG	RDRAND_INT
#endif

#ifdef CONFIG_ARCH_RANDOM

#define GET_RANDOM(name, type, rdrand, nop)			\
static inline int name(type *v)					\
{								\
	int ok;							\
	alternative_io("movl $0, %0\n\t"			\
		       nop,					\
		       "\n1: " rdrand "\n\t"			\
		       "jc 2f\n\t"				\
		       "decl %0\n\t"                            \
		       "jnz 1b\n\t"                             \
		       "2:",                                    \
		       X86_FEATURE_RDRAND,                      \
		       ASM_OUTPUT2("=r" (ok), "=a" (*v)),       \
		       "0" (RDRAND_RETRY_LOOPS));		\
	return ok;						\
}

#ifdef CONFIG_X86_64

GET_RANDOM(arch_get_random_long, unsigned long, RDRAND_LONG, ASM_NOP5);
GET_RANDOM(arch_get_random_int, unsigned int, RDRAND_INT, ASM_NOP4);

#else

GET_RANDOM(arch_get_random_long, unsigned long, RDRAND_LONG, ASM_NOP3);
GET_RANDOM(arch_get_random_int, unsigned int, RDRAND_INT, ASM_NOP3);

#endif /* CONFIG_X86_64 */

#endif  /* CONFIG_ARCH_RANDOM */

extern void x86_init_rdrand(struct cpuinfo_x86 *c);

=======
#define RDSEED_INT	".byte 0x0f,0xc7,0xf8"
#ifdef CONFIG_X86_64
# define RDRAND_LONG	".byte 0x48,0x0f,0xc7,0xf0"
# define RDSEED_LONG	".byte 0x48,0x0f,0xc7,0xf8"
#else
# define RDRAND_LONG	RDRAND_INT
# define RDSEED_LONG	RDSEED_INT
#endif

/* Unconditional execution of RDRAND and RDSEED */

static inline bool rdrand_long(unsigned long *v)
{
	bool ok;
	unsigned int retry = RDRAND_RETRY_LOOPS;
	do {
		asm volatile(RDRAND_LONG
			     CC_SET(c)
			     : CC_OUT(c) (ok), "=a" (*v));
		if (ok)
			return true;
	} while (--retry);
	return false;
}

static inline bool rdrand_int(unsigned int *v)
{
	bool ok;
	unsigned int retry = RDRAND_RETRY_LOOPS;
	do {
		asm volatile(RDRAND_INT
			     CC_SET(c)
			     : CC_OUT(c) (ok), "=a" (*v));
		if (ok)
			return true;
	} while (--retry);
	return false;
}

static inline bool rdseed_long(unsigned long *v)
{
	bool ok;
	asm volatile(RDSEED_LONG
		     CC_SET(c)
		     : CC_OUT(c) (ok), "=a" (*v));
	return ok;
}

static inline bool rdseed_int(unsigned int *v)
{
	bool ok;
	asm volatile(RDSEED_INT
		     CC_SET(c)
		     : CC_OUT(c) (ok), "=a" (*v));
	return ok;
}

/* Conditional execution based on CPU type */
#define arch_has_random()	static_cpu_has(X86_FEATURE_RDRAND)
#define arch_has_random_seed()	static_cpu_has(X86_FEATURE_RDSEED)

/*
 * These are the generic interfaces; they must not be declared if the
 * stubs in <linux/random.h> are to be invoked,
 * i.e. CONFIG_ARCH_RANDOM is not defined.
 */
#ifdef CONFIG_ARCH_RANDOM

static inline bool arch_get_random_long(unsigned long *v)
{
	return arch_has_random() ? rdrand_long(v) : false;
}

static inline bool arch_get_random_int(unsigned int *v)
{
	return arch_has_random() ? rdrand_int(v) : false;
}

static inline bool arch_get_random_seed_long(unsigned long *v)
{
	return arch_has_random_seed() ? rdseed_long(v) : false;
}

static inline bool arch_get_random_seed_int(unsigned int *v)
{
	return arch_has_random_seed() ? rdseed_int(v) : false;
}

extern void x86_init_rdrand(struct cpuinfo_x86 *c);

#else  /* !CONFIG_ARCH_RANDOM */

static inline void x86_init_rdrand(struct cpuinfo_x86 *c) { }

#endif  /* !CONFIG_ARCH_RANDOM */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* ASM_X86_ARCHRANDOM_H */
