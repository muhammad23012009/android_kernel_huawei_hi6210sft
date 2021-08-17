/*
 * include/asm-xtensa/delay.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 *
 */

#ifndef _XTENSA_DELAY_H
#define _XTENSA_DELAY_H

<<<<<<< HEAD
#include <asm/processor.h>
=======
#include <asm/timex.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/param.h>

extern unsigned long loops_per_jiffy;

static inline void __delay(unsigned long loops)
{
<<<<<<< HEAD
	/* 2 cycles per loop. */
	__asm__ __volatile__ ("1: addi %0, %0, -2; bgeui %0, 2, 1b"
			      : "=r" (loops) : "0" (loops));
}

static __inline__ u32 xtensa_get_ccount(void)
{
	u32 ccount;
	asm volatile ("rsr %0, ccount\n" : "=r" (ccount));
	return ccount;
}

/* For SMP/NUMA systems, change boot_cpu_data to something like
 * local_cpu_data->... where local_cpu_data points to the current
 * cpu. */

static __inline__ void udelay (unsigned long usecs)
{
	unsigned long start = xtensa_get_ccount();
	unsigned long cycles = usecs * (loops_per_jiffy / (1000000UL / HZ));

	/* Note: all variables are unsigned (can wrap around)! */
	while (((unsigned long)xtensa_get_ccount()) - start < cycles)
		;
=======
	if (__builtin_constant_p(loops) && loops < 2)
		__asm__ __volatile__ ("nop");
	else if (loops >= 2)
		/* 2 cycles per loop. */
		__asm__ __volatile__ ("1: addi %0, %0, -2; bgeui %0, 2, 1b"
				: "+r" (loops));
}

/* Undefined function to get compile-time error */
void __bad_udelay(void);
void __bad_ndelay(void);

#define __MAX_UDELAY 30000
#define __MAX_NDELAY 30000

static inline void __udelay(unsigned long usecs)
{
	unsigned long start = get_ccount();
	unsigned long cycles = (usecs * (ccount_freq >> 15)) >> 5;

	/* Note: all variables are unsigned (can wrap around)! */
	while (((unsigned long)get_ccount()) - start < cycles)
		cpu_relax();
}

static inline void udelay(unsigned long usec)
{
	if (__builtin_constant_p(usec) && usec >= __MAX_UDELAY)
		__bad_udelay();
	else
		__udelay(usec);
}

static inline void __ndelay(unsigned long nsec)
{
	/*
	 * Inner shift makes sure multiplication doesn't overflow
	 * for legitimate nsec values
	 */
	unsigned long cycles = (nsec * (ccount_freq >> 15)) >> 15;
	__delay(cycles);
}

#define ndelay(n) ndelay(n)

static inline void ndelay(unsigned long nsec)
{
	if (__builtin_constant_p(nsec) && nsec >= __MAX_NDELAY)
		__bad_ndelay();
	else
		__ndelay(nsec);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif
