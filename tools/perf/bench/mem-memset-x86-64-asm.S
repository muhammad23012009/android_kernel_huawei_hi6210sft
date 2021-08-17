#define memset MEMSET /* don't hide glibc's memset() */
#define altinstr_replacement text
#define globl p2align 4; .globl
<<<<<<< HEAD
#define Lmemset_c globl memset_c; memset_c
#define Lmemset_c_e globl memset_c_e; memset_c_e
#include "../../../arch/x86/lib/memset_64.S"
=======
#include "../../arch/x86/lib/memset_64.S"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * We need to provide note.GNU-stack section, saying that we want
 * NOT executable stack. Otherwise the final linking will assume that
 * the ELF stack should not be restricted at all and set it RWX.
 */
.section .note.GNU-stack,"",@progbits
