#include <linux/module.h>
#include <linux/linkage.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/user.h>
#include <linux/elfcore.h>
#include <linux/in6.h>
#include <linux/interrupt.h>

#include <asm/setup.h>
#include <asm/pgalloc.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/checksum.h>
#include <asm/current.h>
#include <asm/gpio.h>

//asmlinkage long long __ashrdi3 (long long, int);
//asmlinkage long long __lshrdi3 (long long, int);
extern char h8300_debug_device[];

/* platform dependent support */

EXPORT_SYMBOL(strnlen);
EXPORT_SYMBOL(strrchr);
EXPORT_SYMBOL(strstr);
EXPORT_SYMBOL(strchr);
EXPORT_SYMBOL(strcat);
EXPORT_SYMBOL(strlen);
EXPORT_SYMBOL(strcmp);
EXPORT_SYMBOL(strncmp);

EXPORT_SYMBOL(ip_fast_csum);

EXPORT_SYMBOL(enable_irq);
EXPORT_SYMBOL(disable_irq);

/* Networking helper routines. */
EXPORT_SYMBOL(csum_partial_copy_nocheck);

/* The following are special because they're not called
   explicitly (the C compiler generates them).  Fortunately,
   their interface isn't gonna change any time soon now, so
   it's OK to leave it out of version control.  */
//EXPORT_SYMBOL(__ashrdi3);
//EXPORT_SYMBOL(__lshrdi3);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memcmp);
EXPORT_SYMBOL(memscan);
EXPORT_SYMBOL(memmove);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * libgcc functions - functions that are used internally by the
 * compiler...  (prototypes are not correct though, but that
 * doesn't really matter since they're not versioned).
 */
<<<<<<< HEAD
extern void __gcc_bcmp(void);
extern void __ashldi3(void);
extern void __ashrdi3(void);
extern void __cmpdi2(void);
extern void __divdi3(void);
extern void __divsi3(void);
extern void __lshrdi3(void);
extern void __moddi3(void);
extern void __modsi3(void);
extern void __muldi3(void);
extern void __mulsi3(void);
extern void __negdi2(void);
extern void __ucmpdi2(void);
extern void __udivdi3(void);
extern void __udivmoddi4(void);
extern void __udivsi3(void);
extern void __umoddi3(void);
extern void __umodsi3(void);

        /* gcc lib functions */
EXPORT_SYMBOL(__gcc_bcmp);
EXPORT_SYMBOL(__ashldi3);
EXPORT_SYMBOL(__ashrdi3);
EXPORT_SYMBOL(__cmpdi2);
EXPORT_SYMBOL(__divdi3);
EXPORT_SYMBOL(__divsi3);
EXPORT_SYMBOL(__lshrdi3);
EXPORT_SYMBOL(__moddi3);
EXPORT_SYMBOL(__modsi3);
EXPORT_SYMBOL(__muldi3);
EXPORT_SYMBOL(__mulsi3);
EXPORT_SYMBOL(__negdi2);
EXPORT_SYMBOL(__ucmpdi2);
EXPORT_SYMBOL(__udivdi3);
EXPORT_SYMBOL(__udivmoddi4);
EXPORT_SYMBOL(__udivsi3);
EXPORT_SYMBOL(__umoddi3);
EXPORT_SYMBOL(__umodsi3);

EXPORT_SYMBOL(h8300_reserved_gpio);
EXPORT_SYMBOL(h8300_free_gpio);
EXPORT_SYMBOL(h8300_set_gpio_dir);
=======
asmlinkage long __ucmpdi2(long long, long long);
asmlinkage long long __ashldi3(long long, int);
asmlinkage long long __ashrdi3(long long, int);
asmlinkage long long __lshrdi3(long long, int);
asmlinkage long __divsi3(long, long);
asmlinkage long __modsi3(long, long);
asmlinkage unsigned long __umodsi3(unsigned long, unsigned long);
asmlinkage long long __muldi3(long long, long long);
asmlinkage long __mulsi3(long, long);
asmlinkage long __udivsi3(long, long);
asmlinkage void *memcpy(void *, const void *, size_t);
asmlinkage void *memset(void *, int, size_t);
asmlinkage long strncpy_from_user(void *to, void *from, size_t n);

	/* gcc lib functions */
EXPORT_SYMBOL(__ucmpdi2);
EXPORT_SYMBOL(__ashldi3);
EXPORT_SYMBOL(__ashrdi3);
EXPORT_SYMBOL(__lshrdi3);
EXPORT_SYMBOL(__divsi3);
EXPORT_SYMBOL(__modsi3);
EXPORT_SYMBOL(__umodsi3);
EXPORT_SYMBOL(__muldi3);
EXPORT_SYMBOL(__mulsi3);
EXPORT_SYMBOL(__udivsi3);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(strncpy_from_user);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
