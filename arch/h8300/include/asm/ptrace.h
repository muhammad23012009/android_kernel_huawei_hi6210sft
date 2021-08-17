#ifndef _H8300_PTRACE_H
#define _H8300_PTRACE_H

#include <uapi/asm/ptrace.h>

#ifndef __ASSEMBLY__
<<<<<<< HEAD
#if defined(CONFIG_CPU_H8S)
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifndef PS_S
#define PS_S  (0x10)
#endif

<<<<<<< HEAD
#if defined(__H8300H__)
#define H8300_REGS_NO 11
#endif
#if defined(__H8300S__)
#define H8300_REGS_NO 12
#endif

/* Find the stack offset for a register, relative to thread.esp0. */
#define PT_REG(reg)	((long)&((struct pt_regs *)0)->reg)

=======
#if defined(CONFIG_CPU_H8300H)
#define H8300_REGS_NO 11
#endif
#if defined(CONFIG_CPU_H8S)
#define H8300_REGS_NO 12
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define arch_has_single_step()	(1)

#define user_mode(regs) (!((regs)->ccr & PS_S))
#define instruction_pointer(regs) ((regs)->pc)
#define profile_pc(regs) instruction_pointer(regs)
<<<<<<< HEAD
=======
#define user_stack_pointer(regs) ((regs)->sp)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define current_pt_regs() ((struct pt_regs *) \
	(THREAD_SIZE + (unsigned long)current_thread_info()) - 1)
#define signal_pt_regs() ((struct pt_regs *)current->thread.esp0)
#define current_user_stack_pointer() rdusp()
<<<<<<< HEAD
=======
#define task_pt_regs(task) \
	((struct pt_regs *) (task_stack_page(task) + THREAD_SIZE) - 1)

extern long h8300_get_reg(struct task_struct *task, int regno);
extern int h8300_put_reg(struct task_struct *task, int regno,
			 unsigned long data);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __ASSEMBLY__ */
#endif /* _H8300_PTRACE_H */
