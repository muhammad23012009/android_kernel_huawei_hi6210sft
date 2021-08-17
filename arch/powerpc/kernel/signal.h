/*
<<<<<<< HEAD
 *    Copyright (c) 2007 Benjamin Herrenschmidt, IBM Coproration
=======
 *    Copyright (c) 2007 Benjamin Herrenschmidt, IBM Corporation
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *    Extracted from signal_32.c and signal_64.c
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file README.legal in the main directory of
 * this archive for more details.
 */

#ifndef _POWERPC_ARCH_SIGNAL_H
#define _POWERPC_ARCH_SIGNAL_H

extern void do_notify_resume(struct pt_regs *regs, unsigned long thread_info_flags);

<<<<<<< HEAD
extern void __user * get_sigframe(struct k_sigaction *ka, unsigned long sp,
				  size_t frame_size, int is_32);

extern int handle_signal32(unsigned long sig, struct k_sigaction *ka,
			   siginfo_t *info, sigset_t *oldset,
			   struct pt_regs *regs);

extern int handle_rt_signal32(unsigned long sig, struct k_sigaction *ka,
			      siginfo_t *info, sigset_t *oldset,
			      struct pt_regs *regs);

extern unsigned long copy_fpr_to_user(void __user *to,
				      struct task_struct *task);
extern unsigned long copy_transact_fpr_to_user(void __user *to,
					       struct task_struct *task);
extern unsigned long copy_fpr_from_user(struct task_struct *task,
					void __user *from);
extern unsigned long copy_transact_fpr_from_user(struct task_struct *task,
						 void __user *from);
#ifdef CONFIG_VSX
extern unsigned long copy_vsx_to_user(void __user *to,
				      struct task_struct *task);
extern unsigned long copy_transact_vsx_to_user(void __user *to,
					       struct task_struct *task);
extern unsigned long copy_vsx_from_user(struct task_struct *task,
					void __user *from);
extern unsigned long copy_transact_vsx_from_user(struct task_struct *task,
=======
extern void __user *get_sigframe(struct ksignal *ksig, unsigned long sp,
				  size_t frame_size, int is_32);

extern int handle_signal32(struct ksignal *ksig, sigset_t *oldset,
			   struct task_struct *tsk);

extern int handle_rt_signal32(struct ksignal *ksig, sigset_t *oldset,
			      struct task_struct *tsk);

extern unsigned long copy_fpr_to_user(void __user *to,
				      struct task_struct *task);
extern unsigned long copy_ckfpr_to_user(void __user *to,
					       struct task_struct *task);
extern unsigned long copy_fpr_from_user(struct task_struct *task,
					void __user *from);
extern unsigned long copy_ckfpr_from_user(struct task_struct *task,
						 void __user *from);
extern unsigned long get_tm_stackpointer(struct task_struct *tsk);

#ifdef CONFIG_VSX
extern unsigned long copy_vsx_to_user(void __user *to,
				      struct task_struct *task);
extern unsigned long copy_ckvsx_to_user(void __user *to,
					       struct task_struct *task);
extern unsigned long copy_vsx_from_user(struct task_struct *task,
					void __user *from);
extern unsigned long copy_ckvsx_from_user(struct task_struct *task,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						 void __user *from);
#endif

#ifdef CONFIG_PPC64

<<<<<<< HEAD
extern int handle_rt_signal64(int signr, struct k_sigaction *ka,
			      siginfo_t *info, sigset_t *set,
			      struct pt_regs *regs);

#else /* CONFIG_PPC64 */

static inline int handle_rt_signal64(int signr, struct k_sigaction *ka,
				     siginfo_t *info, sigset_t *set,
				     struct pt_regs *regs)
=======
extern int handle_rt_signal64(struct ksignal *ksig, sigset_t *set,
			      struct task_struct *tsk);

#else /* CONFIG_PPC64 */

static inline int handle_rt_signal64(struct ksignal *ksig, sigset_t *set,
				     struct task_struct *tsk)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return -EFAULT;
}

#endif /* !defined(CONFIG_PPC64) */

#endif  /* _POWERPC_ARCH_SIGNAL_H */
