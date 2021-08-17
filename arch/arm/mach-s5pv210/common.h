/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Common Header for S5PV210 machines
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_ARM_MACH_S5PV210_COMMON_H
#define __ARCH_ARM_MACH_S5PV210_COMMON_H

<<<<<<< HEAD
void s5pv210_init_io(struct map_desc *mach_desc, int size);
void s5pv210_init_irq(void);

void s5pv210_register_clocks(void);
void s5pv210_setup_clocks(void);

void s5pv210_restart(char mode, const char *cmd);

extern  int s5pv210_init(void);
extern void s5pv210_map_io(void);
extern void s5pv210_init_clocks(int xtal);
extern void s5pv210_init_uarts(struct s3c2410_uartcfg *cfg, int no);
=======
#ifdef CONFIG_PM_SLEEP
u32 exynos_get_eint_wake_mask(void);
void s5pv210_cpu_resume(void);
void s5pv210_pm_init(void);
#else
static inline void s5pv210_pm_init(void) {}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __ARCH_ARM_MACH_S5PV210_COMMON_H */
