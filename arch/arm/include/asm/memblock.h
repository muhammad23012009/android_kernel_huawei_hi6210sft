#ifndef _ASM_ARM_MEMBLOCK_H
#define _ASM_ARM_MEMBLOCK_H

<<<<<<< HEAD
struct meminfo;
struct machine_desc;

extern void arm_memblock_init(struct meminfo *, struct machine_desc *);

=======
struct machine_desc;

void arm_memblock_init(const struct machine_desc *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
phys_addr_t arm_memblock_steal(phys_addr_t size, phys_addr_t align);

#endif
