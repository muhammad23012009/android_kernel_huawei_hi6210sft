/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _ASM_ARC_UNALIGNED_H
#define _ASM_ARC_UNALIGNED_H

/* ARC700 can't handle unaligned Data accesses. */

#include <asm-generic/unaligned.h>
#include <asm/ptrace.h>

<<<<<<< HEAD
#ifdef CONFIG_ARC_MISALIGN_ACCESS
int misaligned_fixup(unsigned long address, struct pt_regs *regs,
		     unsigned long cause, struct callee_regs *cregs);
#else
static inline int
misaligned_fixup(unsigned long address, struct pt_regs *regs,
		 unsigned long cause, struct callee_regs *cregs)
{
	return 0;
=======
#ifdef CONFIG_ARC_EMUL_UNALIGNED
int misaligned_fixup(unsigned long address, struct pt_regs *regs,
		     struct callee_regs *cregs);
#else
static inline int
misaligned_fixup(unsigned long address, struct pt_regs *regs,
		 struct callee_regs *cregs)
{
	/* Not fixed */
	return 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#endif

#endif /* _ASM_ARC_UNALIGNED_H */
