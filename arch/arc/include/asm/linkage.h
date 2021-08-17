/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_LINKAGE_H
#define __ASM_LINKAGE_H

<<<<<<< HEAD
#ifdef __ASSEMBLY__

/* Can't use the ENTRY macro in linux/linkage.h
 * gas considers ';' as comment vs. newline
 */
.macro ARC_ENTRY name
	.global \name
	.align 4
	\name:
.endm

.macro ARC_EXIT name
#define ASM_PREV_SYM_ADDR(name)  .-##name
	.size \ name, ASM_PREV_SYM_ADDR(\name)
.endm
=======
#include <asm/dwarf.h>

#ifdef __ASSEMBLY__

#define ASM_NL		 `	/* use '`' to mark new line in macro */
#define __ALIGN		.align 4
#define __ALIGN_STR	__stringify(__ALIGN)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* annotation for data we want in DCCM - if enabled in .config */
.macro ARCFP_DATA nm
#ifdef CONFIG_ARC_HAS_DCCM
	.section .data.arcfp
#else
	.section .data
#endif
	.global \nm
.endm

/* annotation for data we want in DCCM - if enabled in .config */
.macro ARCFP_CODE
#ifdef CONFIG_ARC_HAS_ICCM
	.section .text.arcfp, "ax",@progbits
#else
	.section .text, "ax",@progbits
#endif
.endm

<<<<<<< HEAD
=======
#define ENTRY_CFI(name)		\
	.globl name ASM_NL	\
	ALIGN ASM_NL 		\
	name: ASM_NL		\
	CFI_STARTPROC ASM_NL

#define END_CFI(name) 		\
	CFI_ENDPROC ASM_NL	\
	.size name, .-name

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else	/* !__ASSEMBLY__ */

#ifdef CONFIG_ARC_HAS_ICCM
#define __arcfp_code __attribute__((__section__(".text.arcfp")))
#else
#define __arcfp_code __attribute__((__section__(".text")))
#endif

#ifdef CONFIG_ARC_HAS_DCCM
#define __arcfp_data __attribute__((__section__(".data.arcfp")))
#else
#define __arcfp_data __attribute__((__section__(".data")))
#endif

#endif /* __ASSEMBLY__ */

#endif
