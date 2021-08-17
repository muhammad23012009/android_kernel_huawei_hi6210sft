/*
 * Cache definitions for the Hexagon architecture
 *
<<<<<<< HEAD
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2010-2011,2014 The Linux Foundation. All rights reserved.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef __ASM_CACHE_H
#define __ASM_CACHE_H

/* Bytes per L1 cache line */
#define L1_CACHE_SHIFT		(5)
#define L1_CACHE_BYTES		(1 << L1_CACHE_SHIFT)

<<<<<<< HEAD
#define __cacheline_aligned	__aligned(L1_CACHE_BYTES)
#define ____cacheline_aligned	__aligned(L1_CACHE_BYTES)

/* See http://kerneltrap.org/node/15100  */
=======
#define ARCH_DMA_MINALIGN	L1_CACHE_BYTES

#define __cacheline_aligned	__aligned(L1_CACHE_BYTES)
#define ____cacheline_aligned	__aligned(L1_CACHE_BYTES)

/* See http://lwn.net/Articles/262554/ */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define __read_mostly

#endif
