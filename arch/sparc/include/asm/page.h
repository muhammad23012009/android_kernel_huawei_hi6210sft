#ifndef ___ASM_SPARC_PAGE_H
#define ___ASM_SPARC_PAGE_H
<<<<<<< HEAD
=======

#define page_to_phys(page)	(page_to_pfn(page) << PAGE_SHIFT)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#if defined(__sparc__) && defined(__arch64__)
#include <asm/page_64.h>
#else
#include <asm/page_32.h>
#endif
#endif
