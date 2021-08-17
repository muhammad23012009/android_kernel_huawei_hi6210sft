#ifndef _ASM_SPARC_JUMP_LABEL_H
#define _ASM_SPARC_JUMP_LABEL_H

<<<<<<< HEAD
#ifdef __KERNEL__
=======
#ifndef __ASSEMBLY__
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/types.h>

#define JUMP_LABEL_NOP_SIZE 4

<<<<<<< HEAD
static __always_inline bool arch_static_branch(struct static_key *key)
{
		asm_volatile_goto("1:\n\t"
			 "nop\n\t"
			 "nop\n\t"
			 ".pushsection __jump_table,  \"aw\"\n\t"
			 ".align 4\n\t"
			 ".word 1b, %l[l_yes], %c0\n\t"
			 ".popsection \n\t"
			 : :  "i" (key) : : l_yes);
=======
static __always_inline bool arch_static_branch(struct static_key *key, bool branch)
{
	asm_volatile_goto("1:\n\t"
		 "nop\n\t"
		 "nop\n\t"
		 ".pushsection __jump_table,  \"aw\"\n\t"
		 ".align 4\n\t"
		 ".word 1b, %l[l_yes], %c0\n\t"
		 ".popsection \n\t"
		 : :  "i" (&((char *)key)[branch]) : : l_yes);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return false;
l_yes:
	return true;
}

<<<<<<< HEAD
#endif /* __KERNEL__ */
=======
static __always_inline bool arch_static_branch_jump(struct static_key *key, bool branch)
{
	asm_volatile_goto("1:\n\t"
		 "b %l[l_yes]\n\t"
		 "nop\n\t"
		 ".pushsection __jump_table,  \"aw\"\n\t"
		 ".align 4\n\t"
		 ".word 1b, %l[l_yes], %c0\n\t"
		 ".popsection \n\t"
		 : :  "i" (&((char *)key)[branch]) : : l_yes);

	return false;
l_yes:
	return true;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

typedef u32 jump_label_t;

struct jump_entry {
	jump_label_t code;
	jump_label_t target;
	jump_label_t key;
};

<<<<<<< HEAD
=======
#endif  /* __ASSEMBLY__ */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
