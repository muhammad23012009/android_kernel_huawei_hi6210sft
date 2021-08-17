/*
 * drivers/media/platform/s5p-mfc/s5p_mfc_opr.c
 *
 * Samsung MFC (Multi Function Codec - FIMV) driver
 * This file contains hw related functions.
 *
 * Kamil Debski, Copyright (c) 2012 Samsung Electronics Co., Ltd.
 * http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "s5p_mfc_debug.h"
#include "s5p_mfc_opr.h"
#include "s5p_mfc_opr_v5.h"
#include "s5p_mfc_opr_v6.h"

static struct s5p_mfc_hw_ops *s5p_mfc_ops;

void s5p_mfc_init_hw_ops(struct s5p_mfc_dev *dev)
{
<<<<<<< HEAD
	if (IS_MFCV6(dev)) {
=======
	if (IS_MFCV6_PLUS(dev)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		s5p_mfc_ops = s5p_mfc_init_hw_ops_v6();
		dev->warn_start = S5P_FIMV_ERR_WARNINGS_START_V6;
	} else {
		s5p_mfc_ops = s5p_mfc_init_hw_ops_v5();
		dev->warn_start = S5P_FIMV_ERR_WARNINGS_START;
	}
	dev->mfc_ops = s5p_mfc_ops;
}

<<<<<<< HEAD
int s5p_mfc_alloc_priv_buf(struct device *dev,
					struct s5p_mfc_priv_buf *b)
{

	mfc_debug(3, "Allocating priv: %d\n", b->size);
=======
void s5p_mfc_init_regs(struct s5p_mfc_dev *dev)
{
	if (IS_MFCV6_PLUS(dev))
		dev->mfc_regs = s5p_mfc_init_regs_v6_plus(dev);
}

int s5p_mfc_alloc_priv_buf(struct device *dev, dma_addr_t base,
					struct s5p_mfc_priv_buf *b)
{
	mfc_debug(3, "Allocating priv: %zu\n", b->size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	b->virt = dma_alloc_coherent(dev, b->size, &b->dma, GFP_KERNEL);

	if (!b->virt) {
		mfc_err("Allocating private buffer failed\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	mfc_debug(3, "Allocated addr %p %08x\n", b->virt, b->dma);
=======
	if (b->dma < base) {
		mfc_err("Invaling memory configuration!\n");
		mfc_err("Allocated buffer (%pad) is lower than memory base address (%pad)\n",
			&b->dma, &base);
		dma_free_coherent(dev, b->size, b->virt, b->dma);
		return -ENOMEM;
	}

	mfc_debug(3, "Allocated addr %p %pad\n", b->virt, &b->dma);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

void s5p_mfc_release_priv_buf(struct device *dev,
						struct s5p_mfc_priv_buf *b)
{
	if (b->virt) {
		dma_free_coherent(dev, b->size, b->virt, b->dma);
		b->virt = NULL;
		b->dma = 0;
		b->size = 0;
	}
}

