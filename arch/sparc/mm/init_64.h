#ifndef _SPARC64_MM_INIT_H
#define _SPARC64_MM_INIT_H

<<<<<<< HEAD
=======
#include <asm/page.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Most of the symbols in this file are defined in init.c and
 * marked non-static so that assembler code can get at them.
 */

<<<<<<< HEAD
#define MAX_PHYS_ADDRESS	(1UL << 41UL)
#define KPTE_BITMAP_CHUNK_SZ		(256UL * 1024UL * 1024UL)
#define KPTE_BITMAP_BYTES	\
	((MAX_PHYS_ADDRESS / KPTE_BITMAP_CHUNK_SZ) / 4)
#define VALID_ADDR_BITMAP_CHUNK_SZ	(4UL * 1024UL * 1024UL)
#define VALID_ADDR_BITMAP_BYTES	\
	((MAX_PHYS_ADDRESS / VALID_ADDR_BITMAP_CHUNK_SZ) / 8)

extern unsigned long kern_linear_pte_xor[4];
extern unsigned long kpte_linear_bitmap[KPTE_BITMAP_BYTES / sizeof(unsigned long)];
=======
#define MAX_PHYS_ADDRESS	(1UL << MAX_PHYS_ADDRESS_BITS)

extern unsigned long kern_linear_pte_xor[4];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern unsigned int sparc64_highest_unlocked_tlb_ent;
extern unsigned long sparc64_kern_pri_context;
extern unsigned long sparc64_kern_pri_nuc_bits;
extern unsigned long sparc64_kern_sec_context;
<<<<<<< HEAD
extern void mmu_info(struct seq_file *m);
=======
void mmu_info(struct seq_file *m);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct linux_prom_translation {
	unsigned long virt;
	unsigned long size;
	unsigned long data;
};

/* Exported for kernel TLB miss handling in ktlb.S */
extern struct linux_prom_translation prom_trans[512];
extern unsigned int prom_trans_ents;

/* Exported for SMP bootup purposes. */
extern unsigned long kern_locked_tte_data;

<<<<<<< HEAD
extern void prom_world(int enter);

#ifdef CONFIG_SPARSEMEM_VMEMMAP
#define VMEMMAP_CHUNK_SHIFT	22
#define VMEMMAP_CHUNK		(1UL << VMEMMAP_CHUNK_SHIFT)
#define VMEMMAP_CHUNK_MASK	~(VMEMMAP_CHUNK - 1UL)
#define VMEMMAP_ALIGN(x)	(((x)+VMEMMAP_CHUNK-1UL)&VMEMMAP_CHUNK_MASK)

#define VMEMMAP_SIZE	((((1UL << MAX_PHYSADDR_BITS) >> PAGE_SHIFT) * \
			  sizeof(struct page)) >> VMEMMAP_CHUNK_SHIFT)
extern unsigned long vmemmap_table[VMEMMAP_SIZE];
#endif
=======
void prom_world(int enter);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _SPARC64_MM_INIT_H */
