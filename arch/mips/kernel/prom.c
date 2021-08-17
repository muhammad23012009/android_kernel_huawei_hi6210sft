/*
 * MIPS support for CONFIG_OF device tree support
 *
 * Copyright (C) 2010 Cisco Systems Inc. <dediao@cisco.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/export.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/bootmem.h>
<<<<<<< HEAD
#include <linux/initrd.h>
#include <linux/debugfs.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>

=======
#include <linux/debugfs.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>

#include <asm/bootinfo.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/page.h>
#include <asm/prom.h>

static char mips_machine_name[64] = "Unknown";

__init void mips_set_machine_name(const char *name)
{
	if (name == NULL)
		return;

<<<<<<< HEAD
	strncpy(mips_machine_name, name, sizeof(mips_machine_name));
=======
	strlcpy(mips_machine_name, name, sizeof(mips_machine_name));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pr_info("MIPS: machine is %s\n", mips_get_machine_name());
}

char *mips_get_machine_name(void)
{
	return mips_machine_name;
}

<<<<<<< HEAD
#ifdef CONFIG_OF
int __init early_init_dt_scan_memory_arch(unsigned long node,
					  const char *uname, int depth,
					  void *data)
{
	return early_init_dt_scan_memory(node, uname, depth, data);
}

=======
#ifdef CONFIG_USE_OF
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void __init early_init_dt_add_memory_arch(u64 base, u64 size)
{
	return add_memory_region(base, size, BOOT_MEM_RAM);
}

void * __init early_init_dt_alloc_memory_arch(u64 size, u64 align)
{
	return __alloc_bootmem(size, align, __pa(MAX_DMA_ADDRESS));
}

<<<<<<< HEAD
#ifdef CONFIG_BLK_DEV_INITRD
void __init early_init_dt_setup_initrd_arch(u64 start, u64 end)
{
	initrd_start = (unsigned long)__va(start);
	initrd_end = (unsigned long)__va(end);
	initrd_below_start_ok = 1;
}
#endif

int __init early_init_dt_scan_model(unsigned long node,	const char *uname,
				    int depth, void *data)
{
	if (!depth) {
		char *model = of_get_flat_dt_prop(node, "model", NULL);

		if (model)
			mips_set_machine_name(model);
	}
	return 0;
}

void __init early_init_devtree(void *params)
{
	/* Setup flat device-tree pointer */
	initial_boot_params = params;

	/* Retrieve various informations from the /chosen node of the
	 * device-tree, including the platform type, initrd location and
	 * size, and more ...
	 */
	of_scan_flat_dt(early_init_dt_scan_chosen, arcs_cmdline);


	/* Scan memory nodes */
	of_scan_flat_dt(early_init_dt_scan_root, NULL);
	of_scan_flat_dt(early_init_dt_scan_memory_arch, NULL);

	/* try to load the mips machine name */
	of_scan_flat_dt(early_init_dt_scan_model, NULL);
}

void __init __dt_setup_arch(struct boot_param_header *bph)
{
	if (be32_to_cpu(bph->magic) != OF_DT_HEADER) {
		pr_err("DTB has bad magic, ignoring builtin OF DTB\n");

		return;
	}

	initial_boot_params = bph;

	early_init_devtree(initial_boot_params);
}
=======
void __init __dt_setup_arch(void *bph)
{
	if (!early_init_dt_scan(bph))
		return;

	mips_set_machine_name(of_flat_dt_get_machine_name());
}

int __init __dt_register_buses(const char *bus0, const char *bus1)
{
	static struct of_device_id of_ids[3];

	if (!of_have_populated_dt())
		panic("device tree not present");

	strlcpy(of_ids[0].compatible, bus0, sizeof(of_ids[0].compatible));
	if (bus1) {
		strlcpy(of_ids[1].compatible, bus1,
			sizeof(of_ids[1].compatible));
	}

	if (of_platform_populate(NULL, of_ids, NULL, NULL))
		panic("failed to populate DT");

	return 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
