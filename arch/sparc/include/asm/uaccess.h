#ifndef ___ASM_SPARC_UACCESS_H
#define ___ASM_SPARC_UACCESS_H
#if defined(__sparc__) && defined(__arch64__)
#include <asm/uaccess_64.h>
#else
#include <asm/uaccess_32.h>
#endif

#define user_addr_max() \
	(segment_eq(get_fs(), USER_DS) ? TASK_SIZE : ~0UL)

<<<<<<< HEAD
extern long strncpy_from_user(char *dest, const char __user *src, long count);
=======
long strncpy_from_user(char *dest, const char __user *src, long count);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
