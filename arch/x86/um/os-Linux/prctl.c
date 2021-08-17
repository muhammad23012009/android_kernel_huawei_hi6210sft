/*
 * Copyright (C) 2007 Jeff Dike (jdike@{addtoit.com,linux.intel.com})
 * Licensed under the GPL
 */

#include <sys/ptrace.h>
<<<<<<< HEAD
#include <linux/ptrace.h>
=======
#include <asm/ptrace.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int os_arch_prctl(int pid, int code, unsigned long *addr)
{
        return ptrace(PTRACE_ARCH_PRCTL, pid, (unsigned long) addr, code);
}
