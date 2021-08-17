#ifndef _SPARC64_SPARSEMEM_H
#define _SPARC64_SPARSEMEM_H

#ifdef __KERNEL__

<<<<<<< HEAD
#define SECTION_SIZE_BITS       30
#define MAX_PHYSADDR_BITS       42
#define MAX_PHYSMEM_BITS        42
=======
#include <asm/page.h>

#define SECTION_SIZE_BITS       30
#define MAX_PHYSADDR_BITS       MAX_PHYS_ADDRESS_BITS
#define MAX_PHYSMEM_BITS        MAX_PHYS_ADDRESS_BITS
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* !(__KERNEL__) */

#endif /* !(_SPARC64_SPARSEMEM_H) */
