/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2011 by Kevin Cernekee (cernekee@gmail.com)
 *
 * Definitions for BMIPS processors
 */
#ifndef _ASM_BMIPS_H
#define _ASM_BMIPS_H

#include <linux/compiler.h>
#include <linux/linkage.h>
#include <asm/addrspace.h>
#include <asm/mipsregs.h>
#include <asm/hazards.h>

/* NOTE: the CBR register returns a PA, and it can be above 0xff00_0000 */
#define BMIPS_GET_CBR()			((void __iomem *)(CKSEG1 | \
					 (unsigned long) \
					 ((read_c0_brcm_cbr() >> 18) << 18)))

#define BMIPS_RAC_CONFIG		0x00000000
#define BMIPS_RAC_ADDRESS_RANGE		0x00000004
#define BMIPS_RAC_CONFIG_1		0x00000008
#define BMIPS_L2_CONFIG			0x0000000c
#define BMIPS_LMB_CONTROL		0x0000001c
#define BMIPS_SYSTEM_BASE		0x00000020
#define BMIPS_PERF_GLOBAL_CONTROL	0x00020000
#define BMIPS_PERF_CONTROL_0		0x00020004
#define BMIPS_PERF_CONTROL_1		0x00020008
#define BMIPS_PERF_COUNTER_0		0x00020010
#define BMIPS_PERF_COUNTER_1		0x00020014
#define BMIPS_PERF_COUNTER_2		0x00020018
#define BMIPS_PERF_COUNTER_3		0x0002001c
#define BMIPS_RELO_VECTOR_CONTROL_0	0x00030000
#define BMIPS_RELO_VECTOR_CONTROL_1	0x00038000

#define BMIPS_NMI_RESET_VEC		0x80000000
#define BMIPS_WARM_RESTART_VEC		0x80000380

#define ZSCM_REG_BASE			0x97000000

#if !defined(__ASSEMBLY__)

#include <linux/cpumask.h>
#include <asm/r4kcache.h>
<<<<<<< HEAD

extern struct plat_smp_ops bmips_smp_ops;
extern char bmips_reset_nmi_vec;
extern char bmips_reset_nmi_vec_end;
extern char bmips_smp_movevec;
extern char bmips_smp_int_vec;
extern char bmips_smp_int_vec_end;
=======
#include <asm/smp-ops.h>

extern struct plat_smp_ops bmips43xx_smp_ops;
extern struct plat_smp_ops bmips5000_smp_ops;

static inline int register_bmips_smp_ops(void)
{
#if IS_ENABLED(CONFIG_CPU_BMIPS) && IS_ENABLED(CONFIG_SMP)
	switch (current_cpu_type()) {
	case CPU_BMIPS32:
	case CPU_BMIPS3300:
		return register_up_smp_ops();
	case CPU_BMIPS4350:
	case CPU_BMIPS4380:
		register_smp_ops(&bmips43xx_smp_ops);
		break;
	case CPU_BMIPS5000:
		register_smp_ops(&bmips5000_smp_ops);
		break;
	default:
		return -ENODEV;
	}

	return 0;
#else
	return -ENODEV;
#endif
}

extern char bmips_reset_nmi_vec[];
extern char bmips_reset_nmi_vec_end[];
extern char bmips_smp_movevec[];
extern char bmips_smp_int_vec[];
extern char bmips_smp_int_vec_end[];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern int bmips_smp_enabled;
extern int bmips_cpu_offset;
extern cpumask_t bmips_booted_mask;
<<<<<<< HEAD

extern void bmips_ebase_setup(void);
extern asmlinkage void plat_wired_tlb_setup(void);
=======
extern unsigned long bmips_tp1_irqs;

extern void bmips_ebase_setup(void);
extern asmlinkage void plat_wired_tlb_setup(void);
extern void bmips_cpu_setup(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline unsigned long bmips_read_zscm_reg(unsigned int offset)
{
	unsigned long ret;

<<<<<<< HEAD
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"cache %1, 0(%2)\n"
		"sync\n"
		"_ssnop\n"
		"_ssnop\n"
		"_ssnop\n"
		"_ssnop\n"
		"_ssnop\n"
		"_ssnop\n"
		"_ssnop\n"
		"mfc0 %0, $28, 3\n"
		"_ssnop\n"
		".set pop\n"
		: "=&r" (ret)
		: "i" (Index_Load_Tag_S), "r" (ZSCM_REG_BASE + offset)
		: "memory");
=======
	barrier();
	cache_op(Index_Load_Tag_S, ZSCM_REG_BASE + offset);
	__sync();
	_ssnop();
	_ssnop();
	_ssnop();
	_ssnop();
	_ssnop();
	_ssnop();
	_ssnop();
	ret = read_c0_ddatalo();
	_ssnop();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static inline void bmips_write_zscm_reg(unsigned int offset, unsigned long data)
{
<<<<<<< HEAD
	__asm__ __volatile__(
		".set push\n"
		".set noreorder\n"
		"mtc0 %0, $28, 3\n"
		"_ssnop\n"
		"_ssnop\n"
		"_ssnop\n"
		"cache %1, 0(%2)\n"
		"_ssnop\n"
		"_ssnop\n"
		"_ssnop\n"
		: /* no outputs */
		: "r" (data),
		  "i" (Index_Store_Tag_S), "r" (ZSCM_REG_BASE + offset)
		: "memory");
=======
	write_c0_ddatalo(data);
	_ssnop();
	_ssnop();
	_ssnop();
	cache_op(Index_Store_Tag_S, ZSCM_REG_BASE + offset);
	_ssnop();
	_ssnop();
	_ssnop();
	barrier();
}

static inline void bmips_post_dma_flush(struct device *dev)
{
	void __iomem *cbr = BMIPS_GET_CBR();
	u32 cfg;

	if (boot_cpu_type() != CPU_BMIPS3300 &&
	    boot_cpu_type() != CPU_BMIPS4350 &&
	    boot_cpu_type() != CPU_BMIPS4380)
		return;

	/* Flush stale data out of the readahead cache */
	cfg = __raw_readl(cbr + BMIPS_RAC_CONFIG);
	__raw_writel(cfg | 0x100, cbr + BMIPS_RAC_CONFIG);
	__raw_readl(cbr + BMIPS_RAC_CONFIG);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif /* !defined(__ASSEMBLY__) */

#endif /* _ASM_BMIPS_H */
