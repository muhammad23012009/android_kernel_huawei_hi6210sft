/*
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

<<<<<<< HEAD
#ifndef __ARCH_BFIN_POSIX_TYPES_H
#define __ARCH_BFIN_POSIX_TYPES_H
=======
#ifndef _UAPI__ARCH_BFIN_POSIX_TYPES_H
#define _UAPI__ARCH_BFIN_POSIX_TYPES_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

typedef unsigned short __kernel_mode_t;
#define __kernel_mode_t __kernel_mode_t

typedef unsigned int __kernel_ipc_pid_t;
#define __kernel_ipc_pid_t __kernel_ipc_pid_t

typedef unsigned long __kernel_size_t;
typedef long __kernel_ssize_t;
typedef int __kernel_ptrdiff_t;
#define __kernel_size_t __kernel_size_t

typedef unsigned short __kernel_old_uid_t;
typedef unsigned short __kernel_old_gid_t;
#define __kernel_old_uid_t __kernel_old_uid_t

typedef unsigned short __kernel_old_dev_t;
#define __kernel_old_dev_t __kernel_old_dev_t

#include <asm-generic/posix_types.h>

<<<<<<< HEAD
#endif
=======
#endif /* _UAPI__ARCH_BFIN_POSIX_TYPES_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
