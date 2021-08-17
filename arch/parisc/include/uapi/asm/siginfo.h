#ifndef _PARISC_SIGINFO_H
#define _PARISC_SIGINFO_H

#if defined(__LP64__)
#define __ARCH_SI_PREAMBLE_SIZE   (4 * sizeof(int))
#endif

#include <asm-generic/siginfo.h>

<<<<<<< HEAD
#undef NSIGTRAP
#define NSIGTRAP	4

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
