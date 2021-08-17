/*
 * linux/arch/arm/mach-omap2/common.c
 *
 * Code common to all OMAP2+ machines.
 *
 * Copyright (C) 2009 Texas Instruments
 * Copyright (C) 2010 Nokia Corporation
 * Tony Lindgren <tony@atomide.com>
 * Added OMAP4 support - Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/platform_data/dsp-omap.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "common.h"
#include "omap-secure.h"

/*
 * Stub function for OMAP2 so that common files
 * continue to build when custom builds are used
 */
int __weak omap_secure_ram_reserve_memblock(void)
{
	return 0;
}

void __init omap_reserve(void)
{
<<<<<<< HEAD
	omap_dsp_reserve_sdram_memblock();
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	omap_secure_ram_reserve_memblock();
	omap_barrier_reserve_memblock();
}
