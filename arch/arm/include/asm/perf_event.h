/*
 *  linux/arch/arm/include/asm/perf_event.h
 *
 *  Copyright (C) 2009 picoChip Designs Ltd, Jamie Iles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __ARM_PERF_EVENT_H__
#define __ARM_PERF_EVENT_H__

<<<<<<< HEAD
/*
 * The ARMv7 CPU PMU supports up to 32 event counters.
 */
#define ARMPMU_MAX_HWEVENTS		32

#define HW_OP_UNSUPPORTED		0xFFFF
#define C(_x)				PERF_COUNT_HW_CACHE_##_x
#define CACHE_OP_UNSUPPORTED		0xFFFF

#ifdef CONFIG_HW_PERF_EVENTS
=======
#ifdef CONFIG_PERF_EVENTS
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct pt_regs;
extern unsigned long perf_instruction_pointer(struct pt_regs *regs);
extern unsigned long perf_misc_flags(struct pt_regs *regs);
#define perf_misc_flags(regs)	perf_misc_flags(regs)
#endif

<<<<<<< HEAD
=======
#define perf_arch_fetch_caller_regs(regs, __ip) { \
	(regs)->ARM_pc = (__ip); \
	(regs)->ARM_fp = (unsigned long) __builtin_frame_address(0); \
	(regs)->ARM_sp = current_stack_pointer; \
	(regs)->ARM_cpsr = SVC_MODE; \
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __ARM_PERF_EVENT_H__ */
