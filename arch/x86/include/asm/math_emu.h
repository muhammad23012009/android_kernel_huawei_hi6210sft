#ifndef _ASM_X86_MATH_EMU_H
#define _ASM_X86_MATH_EMU_H

#include <asm/ptrace.h>
<<<<<<< HEAD
#include <asm/vm86.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* This structure matches the layout of the data saved to the stack
   following a device-not-present interrupt, part of it saved
   automatically by the 80386/80486.
   */
struct math_emu_info {
	long ___orig_eip;
<<<<<<< HEAD
	union {
		struct pt_regs *regs;
		struct kernel_vm86_regs *vm86;
	};
=======
	struct pt_regs *regs;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
#endif /* _ASM_X86_MATH_EMU_H */
