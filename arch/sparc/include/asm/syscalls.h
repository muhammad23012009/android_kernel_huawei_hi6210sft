#ifndef _SPARC64_SYSCALLS_H
#define _SPARC64_SYSCALLS_H

struct pt_regs;

<<<<<<< HEAD
extern asmlinkage long sparc_do_fork(unsigned long clone_flags,
				     unsigned long stack_start,
				     struct pt_regs *regs,
				     unsigned long stack_size);
=======
asmlinkage long sparc_do_fork(unsigned long clone_flags,
			      unsigned long stack_start,
			      struct pt_regs *regs,
			      unsigned long stack_size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _SPARC64_SYSCALLS_H */
