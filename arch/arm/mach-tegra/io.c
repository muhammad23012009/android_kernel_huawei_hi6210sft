/*
 * arch/arm/mach-tegra/io.c
 *
 * Copyright (C) 2010 Google, Inc.
 *
 * Author:
 *	Colin Cross <ccross@google.com>
 *	Erik Gilling <konkers@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/io.h>

#include <asm/page.h>
#include <asm/mach/map.h>
=======
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>

#include <asm/mach/map.h>
#include <asm/page.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "board.h"
#include "iomap.h"

static struct map_desc tegra_io_desc[] __initdata = {
	{
		.virtual = (unsigned long)IO_PPSB_VIRT,
		.pfn = __phys_to_pfn(IO_PPSB_PHYS),
		.length = IO_PPSB_SIZE,
		.type = MT_DEVICE,
	},
	{
		.virtual = (unsigned long)IO_APB_VIRT,
		.pfn = __phys_to_pfn(IO_APB_PHYS),
		.length = IO_APB_SIZE,
		.type = MT_DEVICE,
	},
	{
		.virtual = (unsigned long)IO_CPU_VIRT,
		.pfn = __phys_to_pfn(IO_CPU_PHYS),
		.length = IO_CPU_SIZE,
		.type = MT_DEVICE,
	},
	{
		.virtual = (unsigned long)IO_IRAM_VIRT,
		.pfn = __phys_to_pfn(IO_IRAM_PHYS),
		.length = IO_IRAM_SIZE,
		.type = MT_DEVICE,
	},
};

void __init tegra_map_common_io(void)
{
	debug_ll_io_init();
	iotable_init(tegra_io_desc, ARRAY_SIZE(tegra_io_desc));
}
