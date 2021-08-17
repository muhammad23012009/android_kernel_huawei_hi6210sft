#ifndef _MIPS_SPARSEMEM_H
#define _MIPS_SPARSEMEM_H
#ifdef CONFIG_SPARSEMEM

/*
 * SECTION_SIZE_BITS		2^N: how big each section will be
 * MAX_PHYSMEM_BITS		2^N: how much memory we can have in that space
 */
#if defined(CONFIG_MIPS_HUGE_TLB_SUPPORT) && defined(CONFIG_PAGE_SIZE_64KB)
# define SECTION_SIZE_BITS	29
#else
# define SECTION_SIZE_BITS	28
#endif
<<<<<<< HEAD
#define MAX_PHYSMEM_BITS	35
=======
#define MAX_PHYSMEM_BITS	48
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* CONFIG_SPARSEMEM */
#endif /* _MIPS_SPARSEMEM_H */
