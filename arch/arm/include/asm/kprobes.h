/*
 * arch/arm/include/asm/kprobes.h
 *
 * Copyright (C) 2006, 2007 Motorola Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#ifndef _ARM_KPROBES_H
#define _ARM_KPROBES_H

#include <linux/types.h>
#include <linux/ptrace.h>
<<<<<<< HEAD
#include <linux/percpu.h>

#define __ARCH_WANT_KPROBES_INSN_SLOT
#define MAX_INSN_SIZE			2
#define MAX_STACK_SIZE			64	/* 32 would probably be OK */
=======
#include <linux/notifier.h>

#define __ARCH_WANT_KPROBES_INSN_SLOT
#define MAX_INSN_SIZE			2
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define flush_insn_slot(p)		do { } while (0)
#define kretprobe_blacklist_size	0

typedef u32 kprobe_opcode_t;
<<<<<<< HEAD

struct kprobe;
typedef void (kprobe_insn_handler_t)(struct kprobe *, struct pt_regs *);
typedef unsigned long (kprobe_check_cc)(unsigned long);
typedef void (kprobe_insn_singlestep_t)(struct kprobe *, struct pt_regs *);
typedef void (kprobe_insn_fn_t)(void);

/* Architecture specific copy of original instruction. */
struct arch_specific_insn {
	kprobe_opcode_t			*insn;
	kprobe_insn_handler_t		*insn_handler;
	kprobe_check_cc			*insn_check_cc;
	kprobe_insn_singlestep_t	*insn_singlestep;
	kprobe_insn_fn_t		*insn_fn;
};
=======
struct kprobe;
#include <asm/probes.h>

#define	arch_specific_insn	arch_probes_insn
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct prev_kprobe {
	struct kprobe *kp;
	unsigned int status;
};

/* per-cpu kprobe control block */
struct kprobe_ctlblk {
	unsigned int kprobe_status;
	struct prev_kprobe prev_kprobe;
	struct pt_regs jprobe_saved_regs;
	char jprobes_stack[MAX_STACK_SIZE];
};

void arch_remove_kprobe(struct kprobe *);
int kprobe_fault_handler(struct pt_regs *regs, unsigned int fsr);
int kprobe_exceptions_notify(struct notifier_block *self,
			     unsigned long val, void *data);

<<<<<<< HEAD
=======
/* optinsn template addresses */
extern __visible kprobe_opcode_t optprobe_template_entry;
extern __visible kprobe_opcode_t optprobe_template_val;
extern __visible kprobe_opcode_t optprobe_template_call;
extern __visible kprobe_opcode_t optprobe_template_end;
extern __visible kprobe_opcode_t optprobe_template_sub_sp;
extern __visible kprobe_opcode_t optprobe_template_add_sp;
extern __visible kprobe_opcode_t optprobe_template_restore_begin;
extern __visible kprobe_opcode_t optprobe_template_restore_orig_insn;
extern __visible kprobe_opcode_t optprobe_template_restore_end;

#define MAX_OPTIMIZED_LENGTH	4
#define MAX_OPTINSN_SIZE				\
	((unsigned long)&optprobe_template_end -	\
	 (unsigned long)&optprobe_template_entry)
#define RELATIVEJUMP_SIZE	4

struct arch_optimized_insn {
	/*
	 * copy of the original instructions.
	 * Different from x86, ARM kprobe_opcode_t is u32.
	 */
#define MAX_COPIED_INSN	DIV_ROUND_UP(RELATIVEJUMP_SIZE, sizeof(kprobe_opcode_t))
	kprobe_opcode_t copied_insn[MAX_COPIED_INSN];
	/* detour code buffer */
	kprobe_opcode_t *insn;
	/*
	 * We always copy one instruction on ARM,
	 * so size will always be 4, and unlike x86, there is no
	 * need for a size field.
	 */
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _ARM_KPROBES_H */
