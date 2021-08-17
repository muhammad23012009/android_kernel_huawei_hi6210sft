/*
 *  arch/arm/include/asm/prom.h
 *
 *  Copyright (C) 2009 Canonical Ltd. <jeremy.kerr@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef __ASMARM_PROM_H
#define __ASMARM_PROM_H

<<<<<<< HEAD
#define HAVE_ARCH_DEVTREE_FIXUPS

#ifdef CONFIG_OF

extern struct machine_desc *setup_machine_fdt(unsigned int dt_phys);
extern void arm_dt_memblock_reserve(void);
=======
#ifdef CONFIG_OF

extern const struct machine_desc *setup_machine_fdt(unsigned int dt_phys);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern void __init arm_dt_init_cpu_maps(void);

#else /* CONFIG_OF */

<<<<<<< HEAD
static inline struct machine_desc *setup_machine_fdt(unsigned int dt_phys)
=======
static inline const struct machine_desc *setup_machine_fdt(unsigned int dt_phys)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return NULL;
}

<<<<<<< HEAD
static inline void arm_dt_memblock_reserve(void) { }
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline void arm_dt_init_cpu_maps(void) { }

#endif /* CONFIG_OF */
#endif /* ASMARM_PROM_H */
