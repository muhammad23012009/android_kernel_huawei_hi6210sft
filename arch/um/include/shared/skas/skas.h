/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#ifndef __SKAS_H
#define __SKAS_H

#include <sysdep/ptrace.h>

extern int userspace_pid[];
<<<<<<< HEAD
extern int proc_mm, ptrace_faultinfo, ptrace_ldt;
extern int skas_needs_stub;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern int user_thread(unsigned long stack, int flags);
extern void new_thread_handler(void);
extern void handle_syscall(struct uml_pt_regs *regs);
<<<<<<< HEAD
extern int new_mm(unsigned long stack);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern long execute_syscall_skas(void *r);
extern unsigned long current_stub_stack(void);

#endif
