/* linux/arch/arm/plat-samsung/cpu.c
 *
 * Copyright (c) 2009-2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Samsung CPU Support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>

<<<<<<< HEAD

#include <mach/map.h>
=======
#include <plat/map-base.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <plat/cpu.h>

unsigned long samsung_cpu_id;
static unsigned int samsung_cpu_rev;

unsigned int samsung_rev(void)
{
	return samsung_cpu_rev;
}
EXPORT_SYMBOL(samsung_rev);

<<<<<<< HEAD
void __init s3c24xx_init_cpu(void)
{
	/* nothing here yet */

	samsung_cpu_rev = 0;
}

void __init s3c64xx_init_cpu(void)
{
	samsung_cpu_id = __raw_readl(S3C_VA_SYS + 0x118);
=======
void __init s3c64xx_init_cpu(void)
{
	samsung_cpu_id = readl_relaxed(S3C_VA_SYS + 0x118);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!samsung_cpu_id) {
		/*
		 * S3C6400 has the ID register in a different place,
		 * and needs a write before it can be read.
		 */
<<<<<<< HEAD
		__raw_writel(0x0, S3C_VA_SYS + 0xA1C);
		samsung_cpu_id = __raw_readl(S3C_VA_SYS + 0xA1C);
	}

	samsung_cpu_rev = 0;
}

void __init s5p_init_cpu(void __iomem *cpuid_addr)
{
	samsung_cpu_id = __raw_readl(cpuid_addr);
	samsung_cpu_rev = samsung_cpu_id & 0xFF;
=======
		writel_relaxed(0x0, S3C_VA_SYS + 0xA1C);
		samsung_cpu_id = readl_relaxed(S3C_VA_SYS + 0xA1C);
	}

	samsung_cpu_rev = 0;

	pr_info("Samsung CPU ID: 0x%08lx\n", samsung_cpu_id);
}

void __init s5p_init_cpu(const void __iomem *cpuid_addr)
{
	samsung_cpu_id = readl_relaxed(cpuid_addr);
	samsung_cpu_rev = samsung_cpu_id & 0xFF;

	pr_info("Samsung CPU ID: 0x%08lx\n", samsung_cpu_id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
