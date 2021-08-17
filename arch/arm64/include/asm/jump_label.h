/*
 * Copyright (C) 2013 Huawei Ltd.
 * Author: Jiang Liu <liuj97@gmail.com>
 *
 * Based on arch/arm/include/asm/jump_label.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __ASM_JUMP_LABEL_H
#define __ASM_JUMP_LABEL_H
<<<<<<< HEAD
#include <linux/types.h>
#include <asm/insn.h>

#ifdef __KERNEL__

#define JUMP_LABEL_NOP_SIZE		AARCH64_INSN_SIZE

static __always_inline bool arch_static_branch(struct static_key *key)
{
	asm goto("1: nop\n\t"
=======

#ifndef __ASSEMBLY__

#include <linux/types.h>
#include <asm/insn.h>

#define JUMP_LABEL_NOP_SIZE		AARCH64_INSN_SIZE

static __always_inline bool arch_static_branch(struct static_key *key, bool branch)
{
	asm_volatile_goto("1: nop\n\t"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		 ".pushsection __jump_table,  \"aw\"\n\t"
		 ".align 3\n\t"
		 ".quad 1b, %l[l_yes], %c0\n\t"
		 ".popsection\n\t"
<<<<<<< HEAD
		 :  :  "i"(key) :  : l_yes);
=======
		 :  :  "i"(&((char *)key)[branch]) :  : l_yes);
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
	asm_volatile_goto("1: b %l[l_yes]\n\t"
		 ".pushsection __jump_table,  \"aw\"\n\t"
		 ".align 3\n\t"
		 ".quad 1b, %l[l_yes], %c0\n\t"
		 ".popsection\n\t"
		 :  :  "i"(&((char *)key)[branch]) :  : l_yes);

	return false;
l_yes:
	return true;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

typedef u64 jump_label_t;

struct jump_entry {
	jump_label_t code;
	jump_label_t target;
	jump_label_t key;
};

<<<<<<< HEAD
=======
#endif  /* __ASSEMBLY__ */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif	/* __ASM_JUMP_LABEL_H */
