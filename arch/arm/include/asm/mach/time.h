/*
 * arch/arm/include/asm/mach/time.h
 *
 * Copyright (C) 2004 MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_ARM_MACH_TIME_H
#define __ASM_ARM_MACH_TIME_H

extern void timer_tick(void);

<<<<<<< HEAD
struct timespec;
typedef void (*clock_access_fn)(struct timespec *);
=======
typedef void (*clock_access_fn)(struct timespec64 *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int register_persistent_clock(clock_access_fn read_boot,
				     clock_access_fn read_persistent);

#endif
