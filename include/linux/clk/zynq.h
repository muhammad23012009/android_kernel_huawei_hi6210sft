/*
<<<<<<< HEAD
=======
 * Copyright (C) 2013 Xilinx Inc.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Copyright (C) 2012 National Instruments
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __LINUX_CLK_ZYNQ_H_
#define __LINUX_CLK_ZYNQ_H_

<<<<<<< HEAD
void __init xilinx_zynq_clocks_init(void __iomem *slcr);

=======
#include <linux/spinlock.h>

void zynq_clock_init(void);

struct clk *clk_register_zynq_pll(const char *name, const char *parent,
		void __iomem *pll_ctrl, void __iomem *pll_status, u8 lock_index,
		spinlock_t *lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
