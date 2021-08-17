#ifndef ___ASM_SPARC_MC146818RTC_H
#define ___ASM_SPARC_MC146818RTC_H
<<<<<<< HEAD
=======

#include <linux/spinlock.h>

extern spinlock_t rtc_lock;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#if defined(__sparc__) && defined(__arch64__)
#include <asm/mc146818rtc_64.h>
#else
#include <asm/mc146818rtc_32.h>
#endif
#endif
