/*
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD
#ifndef __ASM_AVR32_POSIX_TYPES_H
#define __ASM_AVR32_POSIX_TYPES_H
=======
#ifndef _UAPI__ASM_AVR32_POSIX_TYPES_H
#define _UAPI__ASM_AVR32_POSIX_TYPES_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * This file is generally used by user-level software, so you need to
 * be a little careful about namespace pollution etc.  Also, we cannot
 * assume GCC is being used.
 */

typedef unsigned short  __kernel_mode_t;
#define __kernel_mode_t __kernel_mode_t

typedef unsigned short  __kernel_ipc_pid_t;
#define __kernel_ipc_pid_t __kernel_ipc_pid_t

typedef unsigned long	__kernel_size_t;
typedef long		__kernel_ssize_t;
typedef int             __kernel_ptrdiff_t;
#define __kernel_size_t __kernel_size_t

typedef unsigned short  __kernel_old_uid_t;
typedef unsigned short  __kernel_old_gid_t;
#define __kernel_old_uid_t __kernel_old_uid_t

typedef unsigned short  __kernel_old_dev_t;
#define __kernel_old_dev_t __kernel_old_dev_t

#include <asm-generic/posix_types.h>

<<<<<<< HEAD
#endif /* __ASM_AVR32_POSIX_TYPES_H */
=======
#endif /* _UAPI__ASM_AVR32_POSIX_TYPES_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
