/*
<<<<<<< HEAD
 *  linux/arch/arm/mach-mmp/include/mach/sram.h
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *  SRAM Memory Management
 *
 *  Copyright (c) 2011 Marvell Semiconductors Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 */

<<<<<<< HEAD
#ifndef __ASM_ARCH_SRAM_H
#define __ASM_ARCH_SRAM_H
=======
#ifndef __DMA_MMP_TDMA_H
#define __DMA_MMP_TDMA_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/genalloc.h>

/* ARBITRARY:  SRAM allocations are multiples of this 2^N size */
#define SRAM_GRANULARITY	512

enum sram_type {
	MMP_SRAM_UNDEFINED = 0,
	MMP_ASRAM,
	MMP_ISRAM,
};

struct sram_platdata {
	char *pool_name;
	int granularity;
};

<<<<<<< HEAD
extern struct gen_pool *sram_get_gpool(char *pool_name);

#endif /* __ASM_ARCH_SRAM_H */
=======
#ifdef CONFIG_MMP_SRAM
extern struct gen_pool *sram_get_gpool(char *pool_name);
#else
static inline struct gen_pool *sram_get_gpool(char *pool_name)
{
	return NULL;
}
#endif

#endif /* __DMA_MMP_TDMA_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
