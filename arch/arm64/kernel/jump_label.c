/*
 * Copyright (C) 2013 Huawei Ltd.
 * Author: Jiang Liu <liuj97@gmail.com>
 *
 * Based on arch/arm/kernel/jump_label.c
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
#include <linux/kernel.h>
#include <linux/jump_label.h>
#include <asm/insn.h>

#ifdef HAVE_JUMP_LABEL

<<<<<<< HEAD
static void __arch_jump_label_transform(struct jump_entry *entry,
					enum jump_label_type type,
					bool is_static)
=======
void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	void *addr = (void *)entry->code;
	u32 insn;

<<<<<<< HEAD
	if (type == JUMP_LABEL_ENABLE) {
=======
	if (type == JUMP_LABEL_JMP) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		insn = aarch64_insn_gen_branch_imm(entry->code,
						   entry->target,
						   AARCH64_INSN_BRANCH_NOLINK);
	} else {
		insn = aarch64_insn_gen_nop();
	}

<<<<<<< HEAD
	if (is_static)
		aarch64_insn_patch_text_nosync(addr, insn);
	else
		aarch64_insn_patch_text(&addr, &insn, 1);
}

void arch_jump_label_transform(struct jump_entry *entry,
			       enum jump_label_type type)
{
	__arch_jump_label_transform(entry, type, false);
=======
	aarch64_insn_patch_text(&addr, &insn, 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void arch_jump_label_transform_static(struct jump_entry *entry,
				      enum jump_label_type type)
{
<<<<<<< HEAD
	__arch_jump_label_transform(entry, type, true);
=======
	/*
	 * We use the architected A64 NOP in arch_static_branch, so there's no
	 * need to patch an identical A64 NOP over the top of it here. The core
	 * will call arch_jump_label_transform from a module notifier if the
	 * NOP needs to be replaced by a branch.
	 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif	/* HAVE_JUMP_LABEL */
