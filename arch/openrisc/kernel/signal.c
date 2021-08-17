/*
 * OpenRISC signal.c
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * Modifications for the OpenRISC architecture:
 * Copyright (C) 2003 Matjaz Breskvar <phoenix@bsemi.com>
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/ptrace.h>
#include <linux/unistd.h>
#include <linux/stddef.h>
#include <linux/tracehook.h>

#include <asm/processor.h>
#include <asm/syscall.h>
#include <asm/ucontext.h>
#include <asm/uaccess.h>

#define DEBUG_SIG 0

struct rt_sigframe {
	struct siginfo info;
	struct ucontext uc;
	unsigned char retcode[16];	/* trampoline code */
};

static int restore_sigcontext(struct pt_regs *regs,
			      struct sigcontext __user *sc)
{
	int err = 0;

	/* Always make any pending restarted system calls return -EINTR */
<<<<<<< HEAD
	current_thread_info()->restart_block.fn = do_no_restart_syscall;
=======
	current->restart_block.fn = do_no_restart_syscall;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Restore the regs from &sc->regs.
	 * (sc is already checked for VERIFY_READ since the sigframe was
	 *  checked in sys_sigreturn previously)
	 */
	err |= __copy_from_user(regs, sc->regs.gpr, 32 * sizeof(unsigned long));
	err |= __copy_from_user(&regs->pc, &sc->regs.pc, sizeof(unsigned long));
	err |= __copy_from_user(&regs->sr, &sc->regs.sr, sizeof(unsigned long));

	/* make sure the SM-bit is cleared so user-mode cannot fool us */
	regs->sr &= ~SPR_SR_SM;

	regs->orig_gpr11 = -1;	/* Avoid syscall restart checks */

	/* TODO: the other ports use regs->orig_XX to disable syscall checks
	 * after this completes, but we don't use that mechanism. maybe we can
	 * use it now ?
	 */

	return err;
}

asmlinkage long _sys_rt_sigreturn(struct pt_regs *regs)
{
	struct rt_sigframe *frame = (struct rt_sigframe __user *)regs->sp;
	sigset_t set;

	/*
	 * Since we stacked the signal on a dword boundary,
	 * then frame should be dword aligned here.  If it's
	 * not, then the user is trying to mess with us.
	 */
	if (((long)frame) & 3)
		goto badframe;

	if (!access_ok(VERIFY_READ, frame, sizeof(*frame)))
		goto badframe;
	if (__copy_from_user(&set, &frame->uc.uc_sigmask, sizeof(set)))
		goto badframe;

	set_current_blocked(&set);

	if (restore_sigcontext(regs, &frame->uc.uc_mcontext))
		goto badframe;

	if (restore_altstack(&frame->uc.uc_stack))
		goto badframe;

	return regs->gpr[11];

badframe:
	force_sig(SIGSEGV, current);
	return 0;
}

/*
 * Set up a signal frame.
 */

static int setup_sigcontext(struct pt_regs *regs, struct sigcontext __user *sc)
{
	int err = 0;

	/* copy the regs */
	/* There should be no need to save callee-saved registers here...
	 * ...but we save them anyway.  Revisit this
	 */
	err |= __copy_to_user(sc->regs.gpr, regs, 32 * sizeof(unsigned long));
	err |= __copy_to_user(&sc->regs.pc, &regs->pc, sizeof(unsigned long));
	err |= __copy_to_user(&sc->regs.sr, &regs->sr, sizeof(unsigned long));

	return err;
}

static inline unsigned long align_sigframe(unsigned long sp)
{
	return sp & ~3UL;
}

/*
 * Work out where the signal frame should go.  It's either on the user stack
 * or the alternate stack.
 */

<<<<<<< HEAD
static inline void __user *get_sigframe(struct k_sigaction *ka,
					struct pt_regs *regs, size_t frame_size)
{
	unsigned long sp = regs->sp;
	int onsigstack = on_sig_stack(sp);

	/* redzone */
	sp -= STACK_FRAME_OVERHEAD;

	/* This is the X/Open sanctioned signal stack switching.  */
	if ((ka->sa.sa_flags & SA_ONSTACK) && !onsigstack) {
		if (current->sas_ss_size)
			sp = current->sas_ss_sp + current->sas_ss_size;
	}

	sp = align_sigframe(sp - frame_size);

	/*
	 * If we are on the alternate signal stack and would overflow it, don't.
	 * Return an always-bogus address instead so we will die with SIGSEGV.
	 */
	if (onsigstack && !likely(on_sig_stack(sp)))
		return (void __user *)-1L;

=======
static inline void __user *get_sigframe(struct ksignal *ksig,
					struct pt_regs *regs, size_t frame_size)
{
	unsigned long sp = regs->sp;

	/* redzone */
	sp -= STACK_FRAME_OVERHEAD;
	sp = sigsp(sp, ksig);
	sp = align_sigframe(sp - frame_size);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return (void __user *)sp;
}

/* grab and setup a signal frame.
 *
 * basically we stack a lot of state info, and arrange for the
 * user-mode program to return to the kernel using either a
 * trampoline which performs the syscall sigreturn, or a provided
 * user-mode trampoline.
 */
<<<<<<< HEAD
static int setup_rt_frame(int sig, struct k_sigaction *ka, siginfo_t *info,
			  sigset_t *set, struct pt_regs *regs)
=======
static int setup_rt_frame(struct ksignal *ksig, sigset_t *set,
			  struct pt_regs *regs)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct rt_sigframe *frame;
	unsigned long return_ip;
	int err = 0;

<<<<<<< HEAD
	frame = get_sigframe(ka, regs, sizeof(*frame));
	if (!access_ok(VERIFY_WRITE, frame, sizeof(*frame)))
		goto give_sigsegv;

	/* Create siginfo.  */
	if (ka->sa.sa_flags & SA_SIGINFO)
		err |= copy_siginfo_to_user(&frame->info, info);
=======
	frame = get_sigframe(ksig, regs, sizeof(*frame));

	if (!access_ok(VERIFY_WRITE, frame, sizeof(*frame)))
		return -EFAULT;

	/* Create siginfo.  */
	if (ksig->ka.sa.sa_flags & SA_SIGINFO)
		err |= copy_siginfo_to_user(&frame->info, &ksig->info);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Create the ucontext.  */
	err |= __put_user(0, &frame->uc.uc_flags);
	err |= __put_user(NULL, &frame->uc.uc_link);
	err |= __save_altstack(&frame->uc.uc_stack, regs->sp);
	err |= setup_sigcontext(regs, &frame->uc.uc_mcontext);

	err |= __copy_to_user(&frame->uc.uc_sigmask, set, sizeof(*set));

	if (err)
<<<<<<< HEAD
		goto give_sigsegv;
=======
		return -EFAULT;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* trampoline - the desired return ip is the retcode itself */
	return_ip = (unsigned long)&frame->retcode;
	/* This is:
		l.ori r11,r0,__NR_sigreturn
		l.sys 1
	 */
	err |= __put_user(0xa960,             (short *)(frame->retcode + 0));
	err |= __put_user(__NR_rt_sigreturn,  (short *)(frame->retcode + 2));
	err |= __put_user(0x20000001, (unsigned long *)(frame->retcode + 4));
	err |= __put_user(0x15000000, (unsigned long *)(frame->retcode + 8));

	if (err)
<<<<<<< HEAD
		goto give_sigsegv;

	/* TODO what is the current->exec_domain stuff and invmap ? */

	/* Set up registers for signal handler */
	regs->pc = (unsigned long)ka->sa.sa_handler; /* what we enter NOW */
	regs->gpr[9] = (unsigned long)return_ip;     /* what we enter LATER */
	regs->gpr[3] = (unsigned long)sig;           /* arg 1: signo */
=======
		return -EFAULT;

	/* Set up registers for signal handler */
	regs->pc = (unsigned long)ksig->ka.sa.sa_handler; /* what we enter NOW */
	regs->gpr[9] = (unsigned long)return_ip;     /* what we enter LATER */
	regs->gpr[3] = (unsigned long)ksig->sig;           /* arg 1: signo */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	regs->gpr[4] = (unsigned long)&frame->info;  /* arg 2: (siginfo_t*) */
	regs->gpr[5] = (unsigned long)&frame->uc;    /* arg 3: ucontext */

	/* actually move the usp to reflect the stacked frame */
	regs->sp = (unsigned long)frame;

	return 0;
<<<<<<< HEAD

give_sigsegv:
	force_sigsegv(sig, current);
	return -EFAULT;
}

static inline void
handle_signal(unsigned long sig,
	      siginfo_t *info, struct k_sigaction *ka,
	      struct pt_regs *regs)
{
	int ret;

	ret = setup_rt_frame(sig, ka, info, sigmask_to_save(), regs);
	if (ret)
		return;

	signal_delivered(sig, info, ka, regs,
				 test_thread_flag(TIF_SINGLESTEP));
=======
}

static inline void
handle_signal(struct ksignal *ksig, struct pt_regs *regs)
{
	int ret;

	ret = setup_rt_frame(ksig, sigmask_to_save(), regs);

	signal_setup_done(ret, ksig, test_thread_flag(TIF_SINGLESTEP));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Note that 'init' is a special process: it doesn't get signals it doesn't
 * want to handle. Thus you cannot kill init even with a SIGKILL even by
 * mistake.
 *
 * Also note that the regs structure given here as an argument, is the latest
 * pushed pt_regs. It may or may not be the same as the first pushed registers
 * when the initial usermode->kernelmode transition took place. Therefore
 * we can use user_mode(regs) to see if we came directly from kernel or user
 * mode below.
 */

int do_signal(struct pt_regs *regs, int syscall)
{
<<<<<<< HEAD
	siginfo_t info;
	int signr;
	struct k_sigaction ka;
=======
	struct ksignal ksig;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long continue_addr = 0;
	unsigned long restart_addr = 0;
	unsigned long retval = 0;
	int restart = 0;

	if (syscall) {
		continue_addr = regs->pc;
		restart_addr = continue_addr - 4;
		retval = regs->gpr[11];

		/*
		 * Setup syscall restart here so that a debugger will
		 * see the already changed PC.
		 */
		switch (retval) {
		case -ERESTART_RESTARTBLOCK:
			restart = -2;
			/* Fall through */
		case -ERESTARTNOHAND:
		case -ERESTARTSYS:
		case -ERESTARTNOINTR:
			restart++;
			regs->gpr[11] = regs->orig_gpr11;
			regs->pc = restart_addr;
			break;
		}
	}

	/*
<<<<<<< HEAD
	 * Get the signal to deliver.  When running under ptrace, at this
	 * point the debugger may change all our registers ...
	 */
	signr = get_signal_to_deliver(&info, &ka, regs, NULL);
	/*
	 * Depending on the signal settings we may need to revert the
	 * decision to restart the system call.  But skip this if a
	 * debugger has chosen to restart at a different PC.
	 */
	if (signr > 0) {
=======
	 * Get the signal to deliver.  During the call to get_signal the
	 * debugger may change all our registers so we may need to revert
	 * the decision to restart the syscall; specifically, if the PC is
	 * changed, don't restart the syscall.
	 */
	if (get_signal(&ksig)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (unlikely(restart) && regs->pc == restart_addr) {
			if (retval == -ERESTARTNOHAND ||
			    retval == -ERESTART_RESTARTBLOCK
			    || (retval == -ERESTARTSYS
<<<<<<< HEAD
			        && !(ka.sa.sa_flags & SA_RESTART))) {
=======
			        && !(ksig.ka.sa.sa_flags & SA_RESTART))) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				/* No automatic restart */
				regs->gpr[11] = -EINTR;
				regs->pc = continue_addr;
			}
		}
<<<<<<< HEAD

		handle_signal(signr, &info, &ka, regs);
=======
		handle_signal(&ksig, regs);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else {
		/* no handler */
		restore_saved_sigmask();
		/*
		 * Restore pt_regs PC as syscall restart will be handled by
		 * kernel without return to userspace
		 */
		if (unlikely(restart) && regs->pc == restart_addr) {
			regs->pc = continue_addr;
			return restart;
		}
	}

	return 0;
}

asmlinkage int
do_work_pending(struct pt_regs *regs, unsigned int thread_flags, int syscall)
{
	do {
		if (likely(thread_flags & _TIF_NEED_RESCHED)) {
			schedule();
		} else {
			if (unlikely(!user_mode(regs)))
				return 0;
			local_irq_enable();
			if (thread_flags & _TIF_SIGPENDING) {
				int restart = do_signal(regs, syscall);
				if (unlikely(restart)) {
					/*
					 * Restart without handlers.
					 * Deal with it without leaving
					 * the kernel space.
					 */
					return restart;
				}
				syscall = 0;
			} else {
				clear_thread_flag(TIF_NOTIFY_RESUME);
				tracehook_notify_resume(regs);
			}
		}
		local_irq_disable();
		thread_flags = current_thread_info()->flags;
	} while (thread_flags & _TIF_WORK_MASK);
	return 0;
}
