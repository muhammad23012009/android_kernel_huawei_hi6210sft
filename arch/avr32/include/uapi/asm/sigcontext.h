/*
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD
#ifndef __ASM_AVR32_SIGCONTEXT_H
#define __ASM_AVR32_SIGCONTEXT_H
=======
#ifndef _UAPI__ASM_AVR32_SIGCONTEXT_H
#define _UAPI__ASM_AVR32_SIGCONTEXT_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct sigcontext {
	unsigned long	oldmask;

	/* CPU registers */
	unsigned long	sr;
	unsigned long	pc;
	unsigned long	lr;
	unsigned long	sp;
	unsigned long	r12;
	unsigned long	r11;
	unsigned long	r10;
	unsigned long	r9;
	unsigned long	r8;
	unsigned long	r7;
	unsigned long	r6;
	unsigned long	r5;
	unsigned long	r4;
	unsigned long	r3;
	unsigned long	r2;
	unsigned long	r1;
	unsigned long	r0;
};

<<<<<<< HEAD
#endif /* __ASM_AVR32_SIGCONTEXT_H */
=======
#endif /* _UAPI__ASM_AVR32_SIGCONTEXT_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
