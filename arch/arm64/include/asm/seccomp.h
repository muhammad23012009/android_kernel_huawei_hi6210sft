/*
 * arch/arm64/include/asm/seccomp.h
 *
 * Copyright (C) 2014 Linaro Limited
<<<<<<< HEAD
 * Author: AKASHI Takahiro <takahiro.akashi <at> linaro.org>
=======
 * Author: AKASHI Takahiro <takahiro.akashi@linaro.org>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _ASM_SECCOMP_H
#define _ASM_SECCOMP_H

#include <asm/unistd.h>

#ifdef CONFIG_COMPAT
#define __NR_seccomp_read_32		__NR_compat_read
#define __NR_seccomp_write_32		__NR_compat_write
#define __NR_seccomp_exit_32		__NR_compat_exit
#define __NR_seccomp_sigreturn_32	__NR_compat_rt_sigreturn
#endif /* CONFIG_COMPAT */

#include <asm-generic/seccomp.h>

#endif /* _ASM_SECCOMP_H */
