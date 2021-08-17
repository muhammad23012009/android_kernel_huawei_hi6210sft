
<<<<<<< HEAD
#ifdef ARCH_X86_64

#define MEMCPY_FN(fn, name, desc)		\
	extern void *fn(void *, const void *, size_t);
=======
#ifdef HAVE_ARCH_X86_64_SUPPORT

#define MEMCPY_FN(fn, name, desc)		\
	void *fn(void *, const void *, size_t);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "mem-memcpy-x86-64-asm-def.h"

#undef MEMCPY_FN

#endif

