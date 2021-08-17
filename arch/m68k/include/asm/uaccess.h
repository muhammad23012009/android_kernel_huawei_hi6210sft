#ifdef __uClinux__
#include <asm/uaccess_no.h>
#else
#include <asm/uaccess_mm.h>
#endif
<<<<<<< HEAD
=======

#ifdef CONFIG_CPU_HAS_NO_UNALIGNED
#include <asm-generic/uaccess-unaligned.h>
#else
#define __get_user_unaligned(x, ptr)	__get_user((x), (ptr))
#define __put_user_unaligned(x, ptr)	__put_user((x), (ptr))
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
