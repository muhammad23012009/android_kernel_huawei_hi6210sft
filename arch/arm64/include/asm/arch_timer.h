/*
 * arch/arm64/include/asm/arch_timer.h
 *
 * Copyright (C) 2012 ARM Ltd.
 * Author: Marc Zyngier <marc.zyngier@arm.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __ASM_ARCH_TIMER_H
#define __ASM_ARCH_TIMER_H

#include <asm/barrier.h>
<<<<<<< HEAD

#include <linux/init.h>
=======
#include <asm/sysreg.h>

#include <linux/bug.h>
#include <linux/init.h>
#include <linux/jump_label.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/types.h>

#include <clocksource/arm_arch_timer.h>

<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_FSL_ERRATUM_A008585)
extern struct static_key_false arch_timer_read_ool_enabled;
#define needs_fsl_a008585_workaround() \
	static_branch_unlikely(&arch_timer_read_ool_enabled)
#else
#define needs_fsl_a008585_workaround()  false
#endif

u32 __fsl_a008585_read_cntp_tval_el0(void);
u32 __fsl_a008585_read_cntv_tval_el0(void);
u64 __fsl_a008585_read_cntvct_el0(void);

/*
 * The number of retries is an arbitrary value well beyond the highest number
 * of iterations the loop has been observed to take.
 */
#define __fsl_a008585_read_reg(reg) ({			\
	u64 _old, _new;					\
	int _retries = 200;				\
							\
	do {						\
		_old = read_sysreg(reg);		\
		_new = read_sysreg(reg);		\
		_retries--;				\
	} while (unlikely(_old != _new) && _retries);	\
							\
	WARN_ON_ONCE(!_retries);			\
	_new;						\
})

#define arch_timer_reg_read_stable(reg) 		\
({							\
	u64 _val;					\
	if (needs_fsl_a008585_workaround())		\
		_val = __fsl_a008585_read_##reg();	\
	else						\
		_val = read_sysreg(reg);		\
	_val;						\
})

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * These register accessors are marked inline so the compiler can
 * nicely work out which register we want, and chuck away the rest of
 * the code.
 */
static __always_inline
void arch_timer_reg_write_cp15(int access, enum arch_timer_reg reg, u32 val)
{
	if (access == ARCH_TIMER_PHYS_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
<<<<<<< HEAD
			asm volatile("msr cntp_ctl_el0,  %0" : : "r" (val));
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("msr cntp_tval_el0, %0" : : "r" (val));
=======
			write_sysreg(val, cntp_ctl_el0);
			break;
		case ARCH_TIMER_REG_TVAL:
			write_sysreg(val, cntp_tval_el0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		}
	} else if (access == ARCH_TIMER_VIRT_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
<<<<<<< HEAD
			asm volatile("msr cntv_ctl_el0,  %0" : : "r" (val));
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("msr cntv_tval_el0, %0" : : "r" (val));
=======
			write_sysreg(val, cntv_ctl_el0);
			break;
		case ARCH_TIMER_REG_TVAL:
			write_sysreg(val, cntv_tval_el0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		}
	}

	isb();
}

static __always_inline
u32 arch_timer_reg_read_cp15(int access, enum arch_timer_reg reg)
{
<<<<<<< HEAD
	u32 val;

	if (access == ARCH_TIMER_PHYS_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mrs %0,  cntp_ctl_el0" : "=r" (val));
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("mrs %0, cntp_tval_el0" : "=r" (val));
			break;
=======
	if (access == ARCH_TIMER_PHYS_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			return read_sysreg(cntp_ctl_el0);
		case ARCH_TIMER_REG_TVAL:
			return arch_timer_reg_read_stable(cntp_tval_el0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	} else if (access == ARCH_TIMER_VIRT_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
<<<<<<< HEAD
			asm volatile("mrs %0,  cntv_ctl_el0" : "=r" (val));
			break;
		case ARCH_TIMER_REG_TVAL:
			asm volatile("mrs %0, cntv_tval_el0" : "=r" (val));
			break;
		}
	}

	return val;
=======
			return read_sysreg(cntv_ctl_el0);
		case ARCH_TIMER_REG_TVAL:
			return arch_timer_reg_read_stable(cntv_tval_el0);
		}
	}

	BUG();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline u32 arch_timer_get_cntfrq(void)
{
<<<<<<< HEAD
	u32 val;
	asm volatile("mrs %0,   cntfrq_el0" : "=r" (val));
	return val;
=======
	return read_sysreg(cntfrq_el0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline u32 arch_timer_get_cntkctl(void)
{
<<<<<<< HEAD
	u32 cntkctl;
	asm volatile("mrs	%0, cntkctl_el1" : "=r" (cntkctl));
	return cntkctl;
=======
	return read_sysreg(cntkctl_el1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void arch_timer_set_cntkctl(u32 cntkctl)
{
<<<<<<< HEAD
	asm volatile("msr	cntkctl_el1, %0" : : "r" (cntkctl));
}

static inline void __cpuinit arch_counter_set_user_access(void)
{
	u32 cntkctl = arch_timer_get_cntkctl();

	/* Disable user access to the timers and the physical counter */
	/* Also disable virtual event stream */
	cntkctl &= ~(ARCH_TIMER_USR_PT_ACCESS_EN
			| ARCH_TIMER_USR_VT_ACCESS_EN
			| ARCH_TIMER_VIRT_EVT_EN
			| ARCH_TIMER_USR_PCT_ACCESS_EN);

	/* Enable user access to the virtual counter */
	cntkctl |= ARCH_TIMER_USR_VCT_ACCESS_EN;

	arch_timer_set_cntkctl(cntkctl);
}

static inline void arch_timer_evtstrm_enable(int divider)
{
	u32 cntkctl = arch_timer_get_cntkctl();
	cntkctl &= ~ARCH_TIMER_EVT_TRIGGER_MASK;
	/* Set the divider and enable virtual event stream */
	cntkctl |= (divider << ARCH_TIMER_EVT_TRIGGER_SHIFT)
			| ARCH_TIMER_VIRT_EVT_EN;
	arch_timer_set_cntkctl(cntkctl);
	elf_hwcap |= HWCAP_EVTSTRM;
#ifdef CONFIG_COMPAT
	compat_elf_hwcap |= COMPAT_HWCAP_EVTSTRM;
#endif
=======
	write_sysreg(cntkctl, cntkctl_el1);
}

static inline u64 arch_counter_get_cntpct(void)
{
	/*
	 * AArch64 kernel and user space mandate the use of CNTVCT.
	 */
	BUG();
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline u64 arch_counter_get_cntvct(void)
{
<<<<<<< HEAD
	u64 cval;

	isb();
	asm volatile("mrs %0, cntvct_el0" : "=r" (cval));

	return cval;
=======
	isb();
	return arch_timer_reg_read_stable(cntvct_el0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline int arch_timer_arch_init(void)
{
	return 0;
}

#endif
