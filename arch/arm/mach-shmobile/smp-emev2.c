/*
 * SMP support for Emma Mobile EV2
 *
 * Copyright (C) 2012  Renesas Solutions Corp.
 * Copyright (C) 2012  Magnus Damm
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
<<<<<<< HEAD
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <mach/common.h>
#include <mach/emev2.h>
#include <asm/smp_plat.h>
#include <asm/smp_scu.h>

#define EMEV2_SCU_BASE 0x1e000000

static int __cpuinit emev2_boot_secondary(unsigned int cpu, struct task_struct *idle)
=======
#include <asm/smp_plat.h>
#include <asm/smp_scu.h>

#include "common.h"
#include "emev2.h"

#define EMEV2_SCU_BASE 0x1e000000
#define EMEV2_SMU_BASE 0xe0110000
#define SMU_GENERAL_REG0 0x7c0

static int emev2_boot_secondary(unsigned int cpu, struct task_struct *idle)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	arch_send_wakeup_ipi_mask(cpumask_of(cpu_logical_map(cpu)));
	return 0;
}

static void __init emev2_smp_prepare_cpus(unsigned int max_cpus)
{
<<<<<<< HEAD
	scu_enable(shmobile_scu_base);

	/* Tell ROM loader about our vector (in headsmp-scu.S) */
	emev2_set_boot_vector(__pa(shmobile_secondary_vector_scu));

	/* enable cache coherency on booting CPU */
	scu_power_mode(shmobile_scu_base, SCU_PM_NORMAL);
}

static void __init emev2_smp_init_cpus(void)
{
	unsigned int ncores;

	/* setup EMEV2 specific SCU base */
	shmobile_scu_base = ioremap(EMEV2_SCU_BASE, PAGE_SIZE);
	emev2_clock_init(); /* need ioremapped SMU */

	ncores = shmobile_scu_base ? scu_get_core_count(shmobile_scu_base) : 1;

	shmobile_smp_init_cpus(ncores);
}

struct smp_operations emev2_smp_ops __initdata = {
	.smp_init_cpus		= emev2_smp_init_cpus,
=======
	void __iomem *smu;

	/* Tell ROM loader about our vector (in headsmp.S) */
	smu = ioremap(EMEV2_SMU_BASE, PAGE_SIZE);
	if (smu) {
		iowrite32(__pa(shmobile_boot_vector), smu + SMU_GENERAL_REG0);
		iounmap(smu);
	}

	/* setup EMEV2 specific SCU bits */
	shmobile_smp_scu_prepare_cpus(EMEV2_SCU_BASE, max_cpus);
}

const struct smp_operations emev2_smp_ops __initconst = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.smp_prepare_cpus	= emev2_smp_prepare_cpus,
	.smp_boot_secondary	= emev2_boot_secondary,
};
