#ifndef __ASM_LIBGCC_H
#define __ASM_LIBGCC_H

#include <asm/byteorder.h>

typedef int word_type __attribute__ ((mode (__word__)));

#ifdef __BIG_ENDIAN
struct DWstruct {
	int high, low;
};
<<<<<<< HEAD
=======

struct TWstruct {
	long long high, low;
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#elif defined(__LITTLE_ENDIAN)
struct DWstruct {
	int low, high;
};
<<<<<<< HEAD
=======

struct TWstruct {
	long long low, high;
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
#error I feel sick.
#endif

typedef union {
	struct DWstruct s;
	long long ll;
} DWunion;

<<<<<<< HEAD
=======
#if defined(CONFIG_64BIT) && defined(CONFIG_CPU_MIPSR6)
typedef int ti_type __attribute__((mode(TI)));

typedef union {
	struct TWstruct s;
	ti_type ti;
} TWunion;
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __ASM_LIBGCC_H */
