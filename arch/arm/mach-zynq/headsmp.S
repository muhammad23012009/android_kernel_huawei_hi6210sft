/*
 * Copyright (c) 2013 Steffen Trumtrar <s.trumtrar@pengutronix.de>
 * Copyright (c) 2012-2013 Xilinx
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <linux/init.h>
<<<<<<< HEAD

	__CPUINIT

ENTRY(zynq_secondary_trampoline)
	ldr	r0, [pc]
=======
#include <asm/assembler.h>

ENTRY(zynq_secondary_trampoline)
ARM_BE8(setend	be)				@ ensure we are in BE8 mode
	ldr	r0, zynq_secondary_trampoline_jump
ARM_BE8(rev	r0, r0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bx	r0
.globl zynq_secondary_trampoline_jump
zynq_secondary_trampoline_jump:
	/* Space for jumping address */
<<<<<<< HEAD
	.word	/* cpu 1 */
.globl zynq_secondary_trampoline_end
zynq_secondary_trampoline_end:

=======
	.word	0	/* cpu 1 */
.globl zynq_secondary_trampoline_end
zynq_secondary_trampoline_end:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
ENDPROC(zynq_secondary_trampoline)
