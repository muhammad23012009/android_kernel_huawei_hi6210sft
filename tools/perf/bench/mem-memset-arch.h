
<<<<<<< HEAD
#ifdef ARCH_X86_64

#define MEMSET_FN(fn, name, desc)		\
	extern void *fn(void *, int, size_t);
=======
#ifdef HAVE_ARCH_X86_64_SUPPORT

#define MEMSET_FN(fn, name, desc)		\
	void *fn(void *, int, size_t);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "mem-memset-x86-64-asm-def.h"

#undef MEMSET_FN

#endif

