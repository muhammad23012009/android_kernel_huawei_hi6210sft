#include <linux/sched.h>
<<<<<<< HEAD
=======
#include <asm/ptrace-abi.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void clear_flushed_tls(struct task_struct *task)
{
}

int arch_copy_tls(struct task_struct *t)
{
	/*
	 * If CLONE_SETTLS is set, we need to save the thread id
	 * (which is argument 5, child_tid, of clone) so it can be set
	 * during context switches.
	 */
	t->thread.arch.fs = t->thread.regs.regs.gp[R8 / sizeof(long)];

	return 0;
}
