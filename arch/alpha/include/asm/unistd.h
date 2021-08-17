#ifndef _ALPHA_UNISTD_H
#define _ALPHA_UNISTD_H

#include <uapi/asm/unistd.h>

<<<<<<< HEAD

#define NR_SYSCALLS			506
=======
#define NR_SYSCALLS			514
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define __ARCH_WANT_OLD_READDIR
#define __ARCH_WANT_STAT64
#define __ARCH_WANT_SYS_GETHOSTNAME
#define __ARCH_WANT_SYS_FADVISE64
#define __ARCH_WANT_SYS_GETPGRP
#define __ARCH_WANT_SYS_OLD_GETRLIMIT
#define __ARCH_WANT_SYS_OLDUMOUNT
#define __ARCH_WANT_SYS_SIGPENDING
#define __ARCH_WANT_SYS_FORK
#define __ARCH_WANT_SYS_VFORK
#define __ARCH_WANT_SYS_CLONE

#endif /* _ALPHA_UNISTD_H */
