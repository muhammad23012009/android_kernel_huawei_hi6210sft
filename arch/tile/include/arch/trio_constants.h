/*
 * Copyright 2012 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */


#ifndef __ARCH_TRIO_CONSTANTS_H__
#define __ARCH_TRIO_CONSTANTS_H__

<<<<<<< HEAD
#define TRIO_NUM_ASIDS 16
=======
#define TRIO_NUM_ASIDS 32
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define TRIO_NUM_TLBS_PER_ASID 16

#define TRIO_NUM_TPIO_REGIONS 8
#define TRIO_LOG2_NUM_TPIO_REGIONS 3

<<<<<<< HEAD
#define TRIO_NUM_MAP_MEM_REGIONS 16
#define TRIO_LOG2_NUM_MAP_MEM_REGIONS 4
=======
#define TRIO_NUM_MAP_MEM_REGIONS 32
#define TRIO_LOG2_NUM_MAP_MEM_REGIONS 5
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define TRIO_NUM_MAP_SQ_REGIONS 8
#define TRIO_LOG2_NUM_MAP_SQ_REGIONS 3

#define TRIO_LOG2_NUM_SQ_FIFO_ENTRIES 6

<<<<<<< HEAD
#define TRIO_NUM_PUSH_DMA_RINGS 32

#define TRIO_NUM_PULL_DMA_RINGS 32
=======
#define TRIO_NUM_PUSH_DMA_RINGS 64

#define TRIO_NUM_PULL_DMA_RINGS 64
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __ARCH_TRIO_CONSTANTS_H__ */
