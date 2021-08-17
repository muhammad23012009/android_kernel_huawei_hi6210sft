/*
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; version 2
 *	of the License.
 *
 */

#include <linux/linkage.h>
<<<<<<< HEAD
#include <asm/alternative-asm.h>
#include <asm/frame.h>
#include <asm/dwarf2.h>

=======
#include <asm/export.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

.text

/*
 * Inputs:
 * %esi : memory location to compare
 * %eax : low 32 bits of old value
 * %edx : high 32 bits of old value
 * %ebx : low 32 bits of new value
 * %ecx : high 32 bits of new value
 */
ENTRY(cmpxchg8b_emu)
<<<<<<< HEAD
CFI_STARTPROC
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#
# Emulate 'cmpxchg8b (%esi)' on UP except we don't
# set the whole ZF thing (caller will just compare
# eax:edx with the expected value)
#
<<<<<<< HEAD
cmpxchg8b_emu:
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pushfl
	cli

	cmpl  (%esi), %eax
<<<<<<< HEAD
	jne not_same
	cmpl 4(%esi), %edx
	jne half_same
=======
	jne .Lnot_same
	cmpl 4(%esi), %edx
	jne .Lhalf_same
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	movl %ebx,  (%esi)
	movl %ecx, 4(%esi)

	popfl
	ret

<<<<<<< HEAD
 not_same:
	movl  (%esi), %eax
 half_same:
=======
.Lnot_same:
	movl  (%esi), %eax
.Lhalf_same:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	movl 4(%esi), %edx

	popfl
	ret

<<<<<<< HEAD
CFI_ENDPROC
ENDPROC(cmpxchg8b_emu)
=======
ENDPROC(cmpxchg8b_emu)
EXPORT_SYMBOL(cmpxchg8b_emu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
