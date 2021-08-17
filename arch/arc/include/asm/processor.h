/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * vineetg: March 2009
 *  -Implemented task_pt_regs( )
 *
 * Amit Bhor, Sameer Dhavale, Ashwin Chaugule: Codito Technologies 2004
 */

#ifndef __ASM_ARC_PROCESSOR_H
#define __ASM_ARC_PROCESSOR_H

<<<<<<< HEAD
#ifdef __KERNEL__

#ifndef __ASSEMBLY__

#include <asm/arcregs.h>	/* for STATUS_E1_MASK et all */
=======
#ifndef __ASSEMBLY__

#include <asm/ptrace.h>

#ifdef CONFIG_ARC_FPU_SAVE_RESTORE
/* These DPFP regs need to be saved/restored across ctx-sw */
struct arc_fpu {
	struct {
		unsigned int l, h;
	} aux_dpfp[2];
};
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Arch specific stuff which needs to be saved per task.
 * However these items are not so important so as to earn a place in
 * struct thread_info
 */
struct thread_struct {
	unsigned long ksp;	/* kernel mode stack pointer */
	unsigned long callee_reg;	/* pointer to callee regs */
	unsigned long fault_address;	/* dbls as brkpt holder as well */
<<<<<<< HEAD
	unsigned long cause_code;	/* Exception Cause Code (ECR) */
#ifdef CONFIG_ARC_CURR_IN_REG
	unsigned long user_r25;
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_ARC_FPU_SAVE_RESTORE
	struct arc_fpu fpu;
#endif
};

#define INIT_THREAD  {                          \
	.ksp = sizeof(init_stack) + (unsigned long) init_stack, \
}

/* Forward declaration, a strange C thing */
struct task_struct;

<<<<<<< HEAD
/*
 * Return saved PC of a blocked thread.
 */
unsigned long thread_saved_pc(struct task_struct *t);

#define task_pt_regs(p) \
	((struct pt_regs *)(THREAD_SIZE - 4 + (void *)task_stack_page(p)) - 1)

/* Free all resources held by a thread. */
#define release_thread(thread) do { } while (0)

/* Prepare to copy thread state - unlazy all lazy status */
#define prepare_to_copy(tsk)    do { } while (0)

=======
#define task_pt_regs(p) \
	((struct pt_regs *)(THREAD_SIZE + (void *)task_stack_page(p)) - 1)

/* Free all resources held by a thread */
#define release_thread(thread) do { } while (0)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * A lot of busy-wait loops in SMP are based off of non-volatile data otherwise
 * get optimised away by gcc
 */
<<<<<<< HEAD
#ifdef CONFIG_SMP
#define cpu_relax()	__asm__ __volatile__ ("" : : : "memory")
#else
#define cpu_relax()	do { } while (0)
=======
#ifndef CONFIG_EZNPS_MTM_EXT

#define cpu_relax()		barrier()
#define cpu_relax_lowlatency()	cpu_relax()

#else

#define cpu_relax()     \
	__asm__ __volatile__ (".word %0" : : "i"(CTOP_INST_SCHD_RW) : "memory")

#define cpu_relax_lowlatency()	barrier()

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

#define copy_segments(tsk, mm)      do { } while (0)
#define release_segments(mm)        do { } while (0)

#define KSTK_EIP(tsk)   (task_pt_regs(tsk)->ret)
<<<<<<< HEAD

/*
 * Where abouts of Task's sp, fp, blink when it was last seen in kernel mode.
 * These can't be derived from pt_regs as that would give correp user-mode val
 */
#define KSTK_ESP(tsk)   (tsk->thread.ksp)
#define KSTK_BLINK(tsk) (*((unsigned int *)((KSTK_ESP(tsk)) + (13+1+1)*4)))
#define KSTK_FP(tsk)    (*((unsigned int *)((KSTK_ESP(tsk)) + (13+1)*4)))

/*
 * Do necessary setup to start up a newly executed thread.
 *
 * E1,E2 so that Interrupts are enabled in user mode
 * L set, so Loop inhibited to begin with
 * lp_start and lp_end seeded with bogus non-zero values so to easily catch
 * the ARC700 sr to lp_start hardware bug
 */
#define start_thread(_regs, _pc, _usp)				\
do {								\
	set_fs(USER_DS); /* reads from user space */		\
	(_regs)->ret = (_pc);					\
	/* Interrupts enabled in User Mode */			\
	(_regs)->status32 = STATUS_U_MASK | STATUS_L_MASK	\
		| STATUS_E1_MASK | STATUS_E2_MASK;		\
	(_regs)->sp = (_usp);					\
	/* bogus seed values for debugging */			\
	(_regs)->lp_start = 0x10;				\
	(_regs)->lp_end = 0x80;					\
} while (0)
=======
#define KSTK_ESP(tsk)   (task_pt_regs(tsk)->sp)

/*
 * Where about of Task's sp, fp, blink when it was last seen in kernel mode.
 * Look in process.c for details of kernel stack layout
 */
#define TSK_K_ESP(tsk)		(tsk->thread.ksp)

#define TSK_K_REG(tsk, off)	(*((unsigned long *)(TSK_K_ESP(tsk) + \
					sizeof(struct callee_regs) + off)))

#define TSK_K_BLINK(tsk)	TSK_K_REG(tsk, 4)
#define TSK_K_FP(tsk)		TSK_K_REG(tsk, 0)

#define thread_saved_pc(tsk)	TSK_K_BLINK(tsk)

extern void start_thread(struct pt_regs * regs, unsigned long pc,
			 unsigned long usp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern unsigned int get_wchan(struct task_struct *p);

/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 * Should the PC register be read instead ? This macro does not seem to
 * be used in many places so this wont be all that bad.
 */
#define current_text_addr() ({ __label__ _l; _l: &&_l; })

#endif /* !__ASSEMBLY__ */

<<<<<<< HEAD
/* Kernels Virtual memory area.
 * Unlike other architectures(MIPS, sh, cris ) ARC 700 does not have a
 * "kernel translated" region (like KSEG2 in MIPS). So we use a upper part
 * of the translated bottom 2GB for kernel virtual memory and protect
 * these pages from user accesses by disabling Ru, Eu and Wu.
 */
#define VMALLOC_SIZE	(0x10000000)	/* 256M */
#define VMALLOC_START	(PAGE_OFFSET - VMALLOC_SIZE)
#define VMALLOC_END	(PAGE_OFFSET)

/* Most of the architectures seem to be keeping some kind of padding between
 * userspace TASK_SIZE and PAGE_OFFSET. i.e TASK_SIZE != PAGE_OFFSET.
 */
#define USER_KERNEL_GUTTER    0x10000000

/* User address space:
 * On ARC700, CPU allows the entire lower half of 32 bit address space to be
 * translated. Thus potentially 2G (0:0x7FFF_FFFF) could be User vaddr space.
 * However we steal 256M for kernel addr (0x7000_0000:0x7FFF_FFFF) and another
 * 256M (0x6000_0000:0x6FFF_FFFF) is gutter between user/kernel spaces
 * Thus total User vaddr space is (0:0x5FFF_FFFF)
 */
#define TASK_SIZE	(PAGE_OFFSET - VMALLOC_SIZE - USER_KERNEL_GUTTER)

#define STACK_TOP       TASK_SIZE
=======
/*
 * Default System Memory Map on ARC
 *
 * ---------------------------- (lower 2G, Translated) -------------------------
 * 0x0000_0000		0x5FFF_FFFF	(user vaddr: TASK_SIZE)
 * 0x6000_0000		0x6FFF_FFFF	(reserved gutter between U/K)
 * 0x7000_0000		0x7FFF_FFFF	(kvaddr: vmalloc/modules/pkmap..)
 *
 * PAGE_OFFSET ---------------- (Upper 2G, Untranslated) -----------------------
 * 0x8000_0000		0xBFFF_FFFF	(kernel direct mapped)
 * 0xC000_0000		0xFFFF_FFFF	(peripheral uncached space)
 * -----------------------------------------------------------------------------
 */

#define TASK_SIZE	0x60000000

#define VMALLOC_START	(PAGE_OFFSET - (CONFIG_ARC_KVADDR_SIZE << 20))

/* 1 PGDIR_SIZE each for fixmap/pkmap, 2 PGDIR_SIZE gutter (see asm/highmem.h) */
#define VMALLOC_SIZE	((CONFIG_ARC_KVADDR_SIZE << 20) - PGDIR_SIZE * 4)

#define VMALLOC_END	(VMALLOC_START + VMALLOC_SIZE)

#define USER_KERNEL_GUTTER    (VMALLOC_START - TASK_SIZE)

#ifdef CONFIG_ARC_PLAT_EZNPS
/* NPS architecture defines special window of 129M in user address space for
 * special memory areas, when accessing this window the MMU do not use TLB.
 * Instead MMU direct the access to:
 * 0x57f00000:0x57ffffff -- 1M of closely coupled memory (aka CMEM)
 * 0x58000000:0x5fffffff -- 16 huge pages, 8M each, with fixed map (aka FMTs)
 *
 * CMEM - is the fastest memory we got and its size is 16K.
 * FMT  - is used to map either to internal/external memory.
 * Internal memory is the second fast memory and its size is 16M
 * External memory is the biggest memory (16G) and also the slowest.
 *
 * STACK_TOP need to be PMD align (21bit) that is why we supply 0x57e00000.
 */
#define STACK_TOP       0x57e00000
#else
#define STACK_TOP       TASK_SIZE
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define STACK_TOP_MAX   STACK_TOP

/* This decides where the kernel will search for a free chunk of vm
 * space during mmap's.
 */
#define TASK_UNMAPPED_BASE      (TASK_SIZE / 3)

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __ASM_ARC_PROCESSOR_H */
