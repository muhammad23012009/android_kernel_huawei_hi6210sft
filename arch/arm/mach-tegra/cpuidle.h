/*
 * Copyright (c) 2012, NVIDIA Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MACH_TEGRA_CPUIDLE_H
#define __MACH_TEGRA_CPUIDLE_H

<<<<<<< HEAD
#ifdef CONFIG_ARCH_TEGRA_2x_SOC
int tegra20_cpuidle_init(void);
#else
static inline int tegra20_cpuidle_init(void) { return -ENODEV; }
#endif

#ifdef CONFIG_ARCH_TEGRA_3x_SOC
int tegra30_cpuidle_init(void);
#else
static inline int tegra30_cpuidle_init(void) { return -ENODEV; }
#endif

#ifdef CONFIG_ARCH_TEGRA_114_SOC
int tegra114_cpuidle_init(void);
#else
static inline int tegra114_cpuidle_init(void) { return -ENODEV; }
=======
#ifdef CONFIG_CPU_IDLE
int tegra20_cpuidle_init(void);
void tegra20_cpuidle_pcie_irqs_in_use(void);
int tegra30_cpuidle_init(void);
int tegra114_cpuidle_init(void);
void tegra_cpuidle_init(void);
void tegra_cpuidle_pcie_irqs_in_use(void);
#else
static inline void tegra_cpuidle_init(void) {}
static inline void tegra_cpuidle_pcie_irqs_in_use(void) {}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

#endif
