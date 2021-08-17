/*
 *  linux/arch/arm/lib/copy_to_user.S
 *
 *  Author:	Nicolas Pitre
 *  Created:	Sep 29, 2005
 *  Copyright:	MontaVista Software, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/linkage.h>
#include <asm/assembler.h>
<<<<<<< HEAD
=======
#include <asm/unwind.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Prototype:
 *
<<<<<<< HEAD
 *	size_t __copy_to_user(void *to, const void *from, size_t n)
=======
 *	size_t arm_copy_to_user(void *to, const void *from, size_t n)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Purpose:
 *
 *	copy a block to user memory from kernel memory
 *
 * Params:
 *
 *	to = user memory
 *	from = kernel memory
 *	n = number of bytes to copy
 *
 * Return value:
 *
 *	Number of bytes NOT copied.
 */

#define LDR1W_SHIFT	0
#ifndef CONFIG_THUMB2_KERNEL
#define STR1W_SHIFT	0
#else
#define STR1W_SHIFT	1
#endif

	.macro ldr1w ptr reg abort
	W(ldr) \reg, [\ptr], #4
	.endm

	.macro ldr4w ptr reg1 reg2 reg3 reg4 abort
	ldmia \ptr!, {\reg1, \reg2, \reg3, \reg4}
	.endm

	.macro ldr8w ptr reg1 reg2 reg3 reg4 reg5 reg6 reg7 reg8 abort
	ldmia \ptr!, {\reg1, \reg2, \reg3, \reg4, \reg5, \reg6, \reg7, \reg8}
	.endm

	.macro ldr1b ptr reg cond=al abort
	ldr\cond\()b \reg, [\ptr], #1
	.endm

	.macro str1w ptr reg abort
	strusr	\reg, \ptr, 4, abort=\abort
	.endm

	.macro str8w ptr reg1 reg2 reg3 reg4 reg5 reg6 reg7 reg8 abort
	str1w \ptr, \reg1, \abort
	str1w \ptr, \reg2, \abort
	str1w \ptr, \reg3, \abort
	str1w \ptr, \reg4, \abort
	str1w \ptr, \reg5, \abort
	str1w \ptr, \reg6, \abort
	str1w \ptr, \reg7, \abort
	str1w \ptr, \reg8, \abort
	.endm

	.macro str1b ptr reg cond=al abort
	strusr	\reg, \ptr, 1, \cond, abort=\abort
	.endm

	.macro enter reg1 reg2
	mov	r3, #0
	stmdb	sp!, {r0, r2, r3, \reg1, \reg2}
	.endm

<<<<<<< HEAD
=======
	.macro usave reg1 reg2
	UNWIND(	.save {r0, r2, r3, \reg1, \reg2}	)
	.endm

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.macro exit reg1 reg2
	add	sp, sp, #8
	ldmfd	sp!, {r0, \reg1, \reg2}
	.endm

	.text

ENTRY(__copy_to_user_std)
<<<<<<< HEAD
WEAK(__copy_to_user)

#include "copy_template.S"

ENDPROC(__copy_to_user)
ENDPROC(__copy_to_user_std)

	.pushsection .fixup,"ax"
=======
WEAK(arm_copy_to_user)
#ifdef CONFIG_CPU_SPECTRE
	get_thread_info r3
	ldr	r3, [r3, #TI_ADDR_LIMIT]
	uaccess_mask_range_ptr r0, r2, r3, ip
#endif

#include "copy_template.S"

ENDPROC(arm_copy_to_user)
ENDPROC(__copy_to_user_std)

	.pushsection .text.fixup,"ax"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.align 0
	copy_abort_preamble
	ldmfd	sp!, {r1, r2, r3}
	sub	r0, r0, r1
	rsb	r0, r0, r2
	copy_abort_end
	.popsection
<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
