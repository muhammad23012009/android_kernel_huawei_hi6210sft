/*
 * linux/arch/unicore32/mm/proc-syms.c
 *
 * Code specific to PKUnity SoC and UniCore ISA
 *
 * Copyright (C) 2001-2010 GUAN Xue-tao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/mm.h>

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>
#include <asm/page.h>

EXPORT_SYMBOL(cpu_dcache_clean_area);
EXPORT_SYMBOL(cpu_set_pte);

<<<<<<< HEAD
=======
EXPORT_SYMBOL(__cpuc_coherent_kern_range);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL(__cpuc_dma_flush_range);
EXPORT_SYMBOL(__cpuc_dma_clean_range);
