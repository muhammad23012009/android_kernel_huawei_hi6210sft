<<<<<<< HEAD
#define memcpy MEMCPY /* don't hide glibc's memcpy() */
#define altinstr_replacement text
#define globl p2align 4; .globl
#define Lmemcpy_c globl memcpy_c; memcpy_c
#define Lmemcpy_c_e globl memcpy_c_e; memcpy_c_e
#include "../../../arch/x86/lib/memcpy_64.S"
=======

/* Various wrappers to make the kernel .S file build in user-space: */

#define memcpy MEMCPY /* don't hide glibc's memcpy() */
#define altinstr_replacement text
#define globl p2align 4; .globl
#define _ASM_EXTABLE_FAULT(x, y)

#include "../../arch/x86/lib/memcpy_64.S"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * We need to provide note.GNU-stack section, saying that we want
 * NOT executable stack. Otherwise the final linking will assume that
 * the ELF stack should not be restricted at all and set it RWX.
 */
.section .note.GNU-stack,"",@progbits
