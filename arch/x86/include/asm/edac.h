#ifndef _ASM_X86_EDAC_H
#define _ASM_X86_EDAC_H

/* ECC atomic, DMA, SMP and interrupt safe scrub function */

<<<<<<< HEAD
static inline void atomic_scrub(void *va, u32 size)
=======
static inline void edac_atomic_scrub(void *va, u32 size)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	u32 i, *virt_addr = va;

	/*
	 * Very carefully read and write to memory atomically so we
	 * are interrupt, DMA and SMP safe.
	 */
	for (i = 0; i < size / 4; i++, virt_addr++)
		asm volatile("lock; addl $0, %0"::"m" (*virt_addr));
}

#endif /* _ASM_X86_EDAC_H */
