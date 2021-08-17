/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _ASM_ARC_SYSCALLS_H
#define _ASM_ARC_SYSCALLS_H  1

<<<<<<< HEAD
#ifdef __KERNEL__

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/types.h>

int sys_clone_wrapper(int, int, int, int, int);
int sys_cacheflush(uint32_t, uint32_t uint32_t);
int sys_arc_settls(void *);
int sys_arc_gettls(void);
<<<<<<< HEAD

#include <asm-generic/syscalls.h>

#endif	/* __KERNEL__ */

=======
int sys_arc_usr_cmpxchg(int *, int, int);

#include <asm-generic/syscalls.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
