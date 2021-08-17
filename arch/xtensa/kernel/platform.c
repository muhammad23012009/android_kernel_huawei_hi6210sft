/*
 * arch/xtensa/kernel/platform.c
 *
 * Default platform functions.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005 Tensilica Inc.
 *
 * Chris Zankel <chris@zankel.net>
 */

#include <linux/types.h>
#include <linux/pci.h>
#include <linux/time.h>
#include <asm/platform.h>
#include <asm/timex.h>
#include <asm/param.h>		/* HZ */

#define _F(r,f,a,b)							\
	r __platform_##f a b;                                   	\
	r platform_##f a __attribute__((weak, alias("__platform_"#f)))

/*
 * Default functions that are used if no platform specific function is defined.
 * (Please, refer to include/asm-xtensa/platform.h for more information)
 */

_F(void, setup, (char** cmd), { });
<<<<<<< HEAD
_F(void, init_irq, (void), { });
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
_F(void, restart, (void), { while(1); });
_F(void, halt, (void), { while(1); });
_F(void, power_off, (void), { while(1); });
_F(void, idle, (void), { __asm__ __volatile__ ("waiti 0" ::: "memory"); });
_F(void, heartbeat, (void), { });
_F(int,  pcibios_fixup, (void), { return 0; });
_F(void, pcibios_init, (void), { });

#ifdef CONFIG_XTENSA_CALIBRATE_CCOUNT
_F(void, calibrate_ccount, (void),
{
	pr_err("ERROR: Cannot calibrate cpu frequency! Assuming 10MHz.\n");
<<<<<<< HEAD
	ccount_per_jiffy = 10 * (1000000UL/HZ);
=======
	ccount_freq = 10 * 1000000UL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
});
#endif
