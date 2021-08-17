#ifndef ___ASM_SPARC_AUXIO_H
#define ___ASM_SPARC_AUXIO_H
<<<<<<< HEAD
=======

#ifndef __ASSEMBLY__

extern void __iomem *auxio_register;

#endif /* ifndef __ASSEMBLY__ */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#if defined(__sparc__) && defined(__arch64__)
#include <asm/auxio_64.h>
#else
#include <asm/auxio_32.h>
#endif
#endif
