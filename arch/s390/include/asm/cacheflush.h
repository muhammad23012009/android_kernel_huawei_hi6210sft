#ifndef _S390_CACHEFLUSH_H
#define _S390_CACHEFLUSH_H

/* Caches aren't brain-dead on the s390. */
#include <asm-generic/cacheflush.h>

<<<<<<< HEAD
#ifdef CONFIG_DEBUG_PAGEALLOC
void kernel_map_pages(struct page *page, int numpages, int enable);
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int set_memory_ro(unsigned long addr, int numpages);
int set_memory_rw(unsigned long addr, int numpages);
int set_memory_nx(unsigned long addr, int numpages);
int set_memory_x(unsigned long addr, int numpages);

#endif /* _S390_CACHEFLUSH_H */
