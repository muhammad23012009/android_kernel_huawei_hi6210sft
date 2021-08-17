/*
 * Copyright (C) 2012 Synopsys, Inc. (www.synopsys.com)
 *
 * Based on reduced version of METAG
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#include <linux/init.h>
#include <linux/reboot.h>
#include <linux/memblock.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
<<<<<<< HEAD
#include <asm/prom.h>
#include <asm/clk.h>
#include <asm/mach_desc.h>

/* called from unflatten_device_tree() to bootstrap devicetree itself */
void * __init early_init_dt_alloc_memory_arch(u64 size, u64 align)
{
	return __va(memblock_alloc(size, align));
=======
#include <asm/mach_desc.h>

#ifdef CONFIG_SERIAL_EARLYCON

static unsigned int __initdata arc_base_baud;

unsigned int __init arc_early_base_baud(void)
{
	return arc_base_baud/16;
}

static void __init arc_set_early_base_baud(unsigned long dt_root)
{
	if (of_flat_dt_is_compatible(dt_root, "abilis,arc-tb10x"))
		arc_base_baud = 166666666;	/* Fixed 166.6MHz clk (TB10x) */
	else if (of_flat_dt_is_compatible(dt_root, "snps,arc-sdp"))
		arc_base_baud = 33333333;	/* Fixed 33MHz clk (AXS10x) */
	else if (of_flat_dt_is_compatible(dt_root, "ezchip,arc-nps"))
		arc_base_baud = 800000000;      /* Fixed 800MHz clk (NPS) */
	else
		arc_base_baud = 50000000;	/* Fixed default 50MHz */
}
#else
#define arc_set_early_base_baud(dt_root)
#endif

static const void * __init arch_get_next_mach(const char *const **match)
{
	static const struct machine_desc *mdesc = __arch_info_begin;
	const struct machine_desc *m = mdesc;

	if (m >= __arch_info_end)
		return NULL;

	mdesc++;
	*match = m->dt_compat;
	return m;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * setup_machine_fdt - Machine setup when an dtb was passed to the kernel
 * @dt:		virtual address pointer to dt blob
 *
 * If a dtb was passed to the kernel, then use it to choose the correct
 * machine_desc and to setup the system.
 */
<<<<<<< HEAD
struct machine_desc * __init setup_machine_fdt(void *dt)
{
	struct boot_param_header *devtree = dt;
	struct machine_desc *mdesc = NULL, *mdesc_best = NULL;
	unsigned int score, mdesc_score = ~1;
	unsigned long dt_root;
	const char *model, *compat;
	void *clk;
	char manufacturer[16];
	int len;

	/* check device tree validity */
	if (be32_to_cpu(devtree->magic) != OF_DT_HEADER)
		return NULL;

	initial_boot_params = devtree;
	dt_root = of_get_flat_dt_root();

	/*
	 * The kernel could be multi-platform enabled, thus could have many
	 * "baked-in" machine descriptors. Search thru all for the best
	 * "compatible" string match.
	 */
	for_each_machine_desc(mdesc) {
		score = of_flat_dt_match(dt_root, mdesc->dt_compat);
		if (score > 0 && score < mdesc_score) {
			mdesc_best = mdesc;
			mdesc_score = score;
		}
	}
	if (!mdesc_best) {
		const char *prop;
		long size;

		pr_err("\n unrecognized device tree list:\n[ ");

		prop = of_get_flat_dt_prop(dt_root, "compatible", &size);
		if (prop) {
			while (size > 0) {
				printk("'%s' ", prop);
				size -= strlen(prop) + 1;
				prop += strlen(prop) + 1;
			}
		}
		printk("]\n\n");

		machine_halt();
	}

	/* compat = "<manufacturer>,<model>" */
	compat =  mdesc_best->dt_compat[0];

	model = strchr(compat, ',');
	if (model)
		model++;

	strlcpy(manufacturer, compat, model ? model - compat : strlen(compat));

	pr_info("Board \"%s\" from %s (Manufacturer)\n", model, manufacturer);

	/* Retrieve various information from the /chosen node */
	of_scan_flat_dt(early_init_dt_scan_chosen, boot_command_line);

	/* Initialize {size,address}-cells info */
	of_scan_flat_dt(early_init_dt_scan_root, NULL);

	/* Setup memory, calling early_init_dt_add_memory_arch */
	of_scan_flat_dt(early_init_dt_scan_memory, NULL);

	clk = of_get_flat_dt_prop(dt_root, "clock-frequency", &len);
	if (clk)
		arc_set_core_freq(of_read_ulong(clk, len/4));

	return mdesc_best;
}

/*
 * Copy the flattened DT out of .init since unflattening doesn't copy strings
 * and the normal DT APIs refs them from orig flat DT
 */
void __init copy_devtree(void)
{
	void *alloc = early_init_dt_alloc_memory_arch(
			be32_to_cpu(initial_boot_params->totalsize), 64);
	if (alloc) {
		memcpy(alloc, initial_boot_params,
				be32_to_cpu(initial_boot_params->totalsize));
		initial_boot_params = alloc;
	}
=======
const struct machine_desc * __init setup_machine_fdt(void *dt)
{
	const struct machine_desc *mdesc;
	unsigned long dt_root;

	if (!early_init_dt_scan(dt))
		return NULL;

	mdesc = of_flat_dt_match_machine(NULL, arch_get_next_mach);
	if (!mdesc)
		machine_halt();

	dt_root = of_get_flat_dt_root();
	arc_set_early_base_baud(dt_root);

	return mdesc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
