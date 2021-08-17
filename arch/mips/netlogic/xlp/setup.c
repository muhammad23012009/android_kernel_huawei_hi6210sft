/*
 * Copyright 2003-2011 NetLogic Microsystems, Inc. (NetLogic). All rights
 * reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the NetLogic
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NETLOGIC ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL NETLOGIC OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/serial_8250.h>
#include <linux/pm.h>
#include <linux/bootmem.h>
=======
#include <linux/of_fdt.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm/idle.h>
#include <asm/reboot.h>
#include <asm/time.h>
#include <asm/bootinfo.h>

<<<<<<< HEAD
#include <linux/of_fdt.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/netlogic/haldefs.h>
#include <asm/netlogic/common.h>

#include <asm/netlogic/xlp-hal/iomap.h>
#include <asm/netlogic/xlp-hal/xlp.h>
#include <asm/netlogic/xlp-hal/sys.h>

uint64_t nlm_io_base;
struct nlm_soc_info nlm_nodes[NLM_NR_NODES];
cpumask_t nlm_cpumask = CPU_MASK_CPU0;
unsigned int nlm_threads_per_core;
<<<<<<< HEAD
extern u32 __dtb_xlp_evp_begin[], __dtb_xlp_svp_begin[], __dtb_start[];
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void nlm_linux_exit(void)
{
	uint64_t sysbase = nlm_get_node(0)->sysbase;

<<<<<<< HEAD
	nlm_write_sys_reg(sysbase, SYS_CHIP_RESET, 1);
=======
	if (cpu_is_xlp9xx())
		nlm_write_sys_reg(sysbase, SYS_9XX_CHIP_RESET, 1);
	else
		nlm_write_sys_reg(sysbase, SYS_CHIP_RESET, 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	for ( ; ; )
		cpu_wait();
}

<<<<<<< HEAD
void __init plat_mem_setup(void)
{
	void *fdtp;

	panic_timeout	= 5;
=======
static void nlm_fixup_mem(void)
{
	const int pref_backup = 512;
	int i;

	for (i = 0; i < boot_mem_map.nr_map; i++) {
		if (boot_mem_map.map[i].type != BOOT_MEM_RAM)
			continue;
		boot_mem_map.map[i].size -= pref_backup;
	}
}

static void __init xlp_init_mem_from_bars(void)
{
	uint64_t map[16];
	int i, n;

	n = nlm_get_dram_map(-1, map, ARRAY_SIZE(map));	/* -1 : all nodes */
	for (i = 0; i < n; i += 2) {
		/* exclude 0x1000_0000-0x2000_0000, u-boot device */
		if (map[i] <= 0x10000000 && map[i+1] > 0x10000000)
			map[i+1] = 0x10000000;
		if (map[i] > 0x10000000 && map[i] < 0x20000000)
			map[i] = 0x20000000;

		add_memory_region(map[i], map[i+1] - map[i], BOOT_MEM_RAM);
	}
}

void __init plat_mem_setup(void)
{
#ifdef CONFIG_SMP
	nlm_wakeup_secondary_cpus();

	/* update TLB size after waking up threads */
	current_cpu_data.tlbsize = ((read_c0_config6() >> 16) & 0xffff) + 1;

	register_smp_ops(&nlm_smp_ops);
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	_machine_restart = (void (*)(char *))nlm_linux_exit;
	_machine_halt	= nlm_linux_exit;
	pm_power_off	= nlm_linux_exit;

<<<<<<< HEAD
	/*
	 * If no FDT pointer is passed in, use the built-in FDT.
	 * device_tree_init() does not handle CKSEG0 pointers in
	 * 64-bit, so convert pointer.
	 */
	fdtp = (void *)(long)fw_arg0;
	if (!fdtp) {
		switch (current_cpu_data.processor_id & 0xff00) {
#ifdef CONFIG_DT_XLP_SVP
		case PRID_IMP_NETLOGIC_XLP3XX:
			fdtp = __dtb_xlp_svp_begin;
			break;
#endif
#ifdef CONFIG_DT_XLP_EVP
		case PRID_IMP_NETLOGIC_XLP8XX:
			fdtp = __dtb_xlp_evp_begin;
			break;
#endif
		default:
			/* Pick a built-in if any, and hope for the best */
			fdtp = __dtb_start;
			break;
		}
	}
	fdtp = phys_to_virt(__pa(fdtp));
	early_init_devtree(fdtp);
=======
	/* memory and bootargs from DT */
	xlp_early_init_devtree();

	if (boot_mem_map.nr_map == 0) {
		pr_info("Using DRAM BARs for memory map.\n");
		xlp_init_mem_from_bars();
	}
	/* Calculate and setup wired entries for mapped kernel */
	nlm_fixup_mem();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

const char *get_system_type(void)
{
<<<<<<< HEAD
	return "Netlogic XLP Series";
=======
	switch (read_c0_prid() & PRID_IMP_MASK) {
	case PRID_IMP_NETLOGIC_XLP9XX:
	case PRID_IMP_NETLOGIC_XLP5XX:
	case PRID_IMP_NETLOGIC_XLP2XX:
		return "Broadcom XLPII Series";
	default:
		return "Netlogic XLP Series";
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void __init prom_free_prom_memory(void)
{
	/* Nothing yet */
}

void xlp_mmu_init(void)
{
<<<<<<< HEAD
	/* enable extended TLB and Large Fixed TLB */
	write_c0_config6(read_c0_config6() | 0x24);

	/* set page mask of Fixed TLB in config7 */
	write_c0_config7(PM_DEFAULT_MASK >>
		(13 + (ffz(PM_DEFAULT_MASK >> 13) / 2)));
=======
	u32 conf4;

	if (cpu_is_xlpii()) {
		/* XLPII series has extended pagesize in config 4 */
		conf4 = read_c0_config4() & ~0x1f00u;
		write_c0_config4(conf4 | ((PAGE_SHIFT - 10) / 2 << 8));
	} else {
		/* enable extended TLB and Large Fixed TLB */
		write_c0_config6(read_c0_config6() | 0x24);

		/* set page mask of extended Fixed TLB in config7 */
		write_c0_config7(PM_DEFAULT_MASK >>
			(13 + (ffz(PM_DEFAULT_MASK >> 13) / 2)));
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void nlm_percpu_init(int hwcpuid)
{
}

void __init prom_init(void)
{
<<<<<<< HEAD
	nlm_io_base = CKSEG1ADDR(XLP_DEFAULT_IO_BASE);
	xlp_mmu_init();
	nlm_node_init(0);

#ifdef CONFIG_SMP
	cpumask_setall(&nlm_cpumask);
	nlm_wakeup_secondary_cpus();

	/* update TLB size after waking up threads */
	current_cpu_data.tlbsize = ((read_c0_config6() >> 16) & 0xffff) + 1;

	register_smp_ops(&nlm_smp_ops);
#endif
}

void __init device_tree_init(void)
{
	unsigned long base, size;

	if (!initial_boot_params)
		return;

	base = virt_to_phys((void *)initial_boot_params);
	size = be32_to_cpu(initial_boot_params->totalsize);

	/* Before we do anything, lets reserve the dt blob */
	reserve_bootmem(base, size, BOOTMEM_DEFAULT);

	unflatten_device_tree();

	/* free the space reserved for the dt blob */
	free_bootmem(base, size);
}

static struct of_device_id __initdata xlp_ids[] = {
	{ .compatible = "simple-bus", },
	{},
};

int __init xlp8xx_ds_publish_devices(void)
{
	if (!of_have_populated_dt())
		return 0;
	return of_platform_bus_probe(NULL, xlp_ids, NULL);
}

device_initcall(xlp8xx_ds_publish_devices);
=======
	void *reset_vec;

	nlm_io_base = CKSEG1ADDR(XLP_DEFAULT_IO_BASE);
	nlm_init_boot_cpu();
	xlp_mmu_init();
	nlm_node_init(0);
	xlp_dt_init((void *)(long)fw_arg0);

	/* Update reset entry point with CPU init code */
	reset_vec = (void *)CKSEG1ADDR(RESET_VEC_PHYS);
	memset(reset_vec, 0, RESET_VEC_SIZE);
	memcpy(reset_vec, (void *)nlm_reset_entry,
			(nlm_reset_entry_end - nlm_reset_entry));

#ifdef CONFIG_SMP
	cpumask_setall(&nlm_cpumask);
#endif
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
