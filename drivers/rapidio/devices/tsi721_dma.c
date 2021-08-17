/*
 * DMA Engine support for Tsi721 PCIExpress-to-SRIO bridge
 *
<<<<<<< HEAD
 * Copyright 2011 Integrated Device Technology, Inc.
=======
 * Copyright (c) 2011-2014 Integrated Device Technology, Inc.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Alexandre Bounine <alexandre.bounine@idt.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
<<<<<<< HEAD
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA  02111-1307, USA.
=======
 * The full GNU General Public License is included in this distribution in the
 * file called COPYING.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#include <linux/io.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/rio.h>
#include <linux/rio_drv.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/kfifo.h>
<<<<<<< HEAD
#include <linux/delay.h>

#include "tsi721.h"

=======
#include <linux/sched.h>
#include <linux/delay.h>
#include "../../dma/dmaengine.h"

#include "tsi721.h"

#ifdef CONFIG_PCI_MSI
static irqreturn_t tsi721_bdma_msix(int irq, void *ptr);
#endif
static int tsi721_submit_sg(struct tsi721_tx_desc *desc);

static unsigned int dma_desc_per_channel = 128;
module_param(dma_desc_per_channel, uint, S_IRUGO);
MODULE_PARM_DESC(dma_desc_per_channel,
		 "Number of DMA descriptors per channel (default: 128)");

static unsigned int dma_txqueue_sz = 16;
module_param(dma_txqueue_sz, uint, S_IRUGO);
MODULE_PARM_DESC(dma_txqueue_sz,
		 "DMA Transactions Queue Size (default: 16)");

static u8 dma_sel = 0x7f;
module_param(dma_sel, byte, S_IRUGO);
MODULE_PARM_DESC(dma_sel,
		 "DMA Channel Selection Mask (default: 0x7f = all)");

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline struct tsi721_bdma_chan *to_tsi721_chan(struct dma_chan *chan)
{
	return container_of(chan, struct tsi721_bdma_chan, dchan);
}

static inline struct tsi721_device *to_tsi721(struct dma_device *ddev)
{
	return container_of(ddev, struct rio_mport, dma)->priv;
}

static inline
struct tsi721_tx_desc *to_tsi721_desc(struct dma_async_tx_descriptor *txd)
{
	return container_of(txd, struct tsi721_tx_desc, txd);
}

<<<<<<< HEAD
static inline
struct tsi721_tx_desc *tsi721_dma_first_active(
				struct tsi721_bdma_chan *bdma_chan)
{
	return list_first_entry(&bdma_chan->active_list,
				struct tsi721_tx_desc, desc_node);
}

static int tsi721_bdma_ch_init(struct tsi721_bdma_chan *bdma_chan)
=======
static int tsi721_bdma_ch_init(struct tsi721_bdma_chan *bdma_chan, int bd_num)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct tsi721_dma_desc *bd_ptr;
	struct device *dev = bdma_chan->dchan.device->dev;
	u64		*sts_ptr;
	dma_addr_t	bd_phys;
	dma_addr_t	sts_phys;
	int		sts_size;
<<<<<<< HEAD
	int		bd_num = bdma_chan->bd_num;

	dev_dbg(dev, "Init Block DMA Engine, CH%d\n", bdma_chan->id);

	/* Allocate space for DMA descriptors */
	bd_ptr = dma_zalloc_coherent(dev,
				bd_num * sizeof(struct tsi721_dma_desc),
				&bd_phys, GFP_KERNEL);
	if (!bd_ptr)
		return -ENOMEM;

	bdma_chan->bd_phys = bd_phys;
	bdma_chan->bd_base = bd_ptr;

	dev_dbg(dev, "DMA descriptors @ %p (phys = %llx)\n",
		bd_ptr, (unsigned long long)bd_phys);

	/* Allocate space for descriptor status FIFO */
	sts_size = (bd_num >= TSI721_DMA_MINSTSSZ) ?
					bd_num : TSI721_DMA_MINSTSSZ;
	sts_size = roundup_pow_of_two(sts_size);
	sts_ptr = dma_zalloc_coherent(dev,
				     sts_size * sizeof(struct tsi721_dma_sts),
				     &sts_phys, GFP_KERNEL);
	if (!sts_ptr) {
		/* Free space allocated for DMA descriptors */
		dma_free_coherent(dev,
				  bd_num * sizeof(struct tsi721_dma_desc),
=======
#ifdef CONFIG_PCI_MSI
	struct tsi721_device *priv = to_tsi721(bdma_chan->dchan.device);
#endif

	tsi_debug(DMA, &bdma_chan->dchan.dev->device, "DMAC%d", bdma_chan->id);

	/*
	 * Allocate space for DMA descriptors
	 * (add an extra element for link descriptor)
	 */
	bd_ptr = dma_zalloc_coherent(dev,
				(bd_num + 1) * sizeof(struct tsi721_dma_desc),
				&bd_phys, GFP_ATOMIC);
	if (!bd_ptr)
		return -ENOMEM;

	bdma_chan->bd_num = bd_num;
	bdma_chan->bd_phys = bd_phys;
	bdma_chan->bd_base = bd_ptr;

	tsi_debug(DMA, &bdma_chan->dchan.dev->device,
		  "DMAC%d descriptors @ %p (phys = %pad)",
		  bdma_chan->id, bd_ptr, &bd_phys);

	/* Allocate space for descriptor status FIFO */
	sts_size = ((bd_num + 1) >= TSI721_DMA_MINSTSSZ) ?
					(bd_num + 1) : TSI721_DMA_MINSTSSZ;
	sts_size = roundup_pow_of_two(sts_size);
	sts_ptr = dma_zalloc_coherent(dev,
				     sts_size * sizeof(struct tsi721_dma_sts),
				     &sts_phys, GFP_ATOMIC);
	if (!sts_ptr) {
		/* Free space allocated for DMA descriptors */
		dma_free_coherent(dev,
				  (bd_num + 1) * sizeof(struct tsi721_dma_desc),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				  bd_ptr, bd_phys);
		bdma_chan->bd_base = NULL;
		return -ENOMEM;
	}

	bdma_chan->sts_phys = sts_phys;
	bdma_chan->sts_base = sts_ptr;
	bdma_chan->sts_size = sts_size;

<<<<<<< HEAD
	dev_dbg(dev,
		"desc status FIFO @ %p (phys = %llx) size=0x%x\n",
		sts_ptr, (unsigned long long)sts_phys, sts_size);

	/* Initialize DMA descriptors ring */
	bd_ptr[bd_num - 1].type_id = cpu_to_le32(DTYPE3 << 29);
	bd_ptr[bd_num - 1].next_lo = cpu_to_le32((u64)bd_phys &
						 TSI721_DMAC_DPTRL_MASK);
	bd_ptr[bd_num - 1].next_hi = cpu_to_le32((u64)bd_phys >> 32);
=======
	tsi_debug(DMA, &bdma_chan->dchan.dev->device,
		"DMAC%d desc status FIFO @ %p (phys = %pad) size=0x%x",
		bdma_chan->id, sts_ptr, &sts_phys, sts_size);

	/* Initialize DMA descriptors ring using added link descriptor */
	bd_ptr[bd_num].type_id = cpu_to_le32(DTYPE3 << 29);
	bd_ptr[bd_num].next_lo = cpu_to_le32((u64)bd_phys &
						 TSI721_DMAC_DPTRL_MASK);
	bd_ptr[bd_num].next_hi = cpu_to_le32((u64)bd_phys >> 32);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Setup DMA descriptor pointers */
	iowrite32(((u64)bd_phys >> 32),
		bdma_chan->regs + TSI721_DMAC_DPTRH);
	iowrite32(((u64)bd_phys & TSI721_DMAC_DPTRL_MASK),
		bdma_chan->regs + TSI721_DMAC_DPTRL);

	/* Setup descriptor status FIFO */
	iowrite32(((u64)sts_phys >> 32),
		bdma_chan->regs + TSI721_DMAC_DSBH);
	iowrite32(((u64)sts_phys & TSI721_DMAC_DSBL_MASK),
		bdma_chan->regs + TSI721_DMAC_DSBL);
	iowrite32(TSI721_DMAC_DSSZ_SIZE(sts_size),
		bdma_chan->regs + TSI721_DMAC_DSSZ);

	/* Clear interrupt bits */
	iowrite32(TSI721_DMAC_INT_ALL,
		bdma_chan->regs + TSI721_DMAC_INT);

	ioread32(bdma_chan->regs + TSI721_DMAC_INT);

<<<<<<< HEAD
=======
#ifdef CONFIG_PCI_MSI
	/* Request interrupt service if we are in MSI-X mode */
	if (priv->flags & TSI721_USING_MSIX) {
		int rc, idx;

		idx = TSI721_VECT_DMA0_DONE + bdma_chan->id;

		rc = request_irq(priv->msix[idx].vector, tsi721_bdma_msix, 0,
				 priv->msix[idx].irq_name, (void *)bdma_chan);

		if (rc) {
			tsi_debug(DMA, &bdma_chan->dchan.dev->device,
				  "Unable to get MSI-X for DMAC%d-DONE",
				  bdma_chan->id);
			goto err_out;
		}

		idx = TSI721_VECT_DMA0_INT + bdma_chan->id;

		rc = request_irq(priv->msix[idx].vector, tsi721_bdma_msix, 0,
				priv->msix[idx].irq_name, (void *)bdma_chan);

		if (rc)	{
			tsi_debug(DMA, &bdma_chan->dchan.dev->device,
				  "Unable to get MSI-X for DMAC%d-INT",
				  bdma_chan->id);
			free_irq(
				priv->msix[TSI721_VECT_DMA0_DONE +
					    bdma_chan->id].vector,
				(void *)bdma_chan);
		}

err_out:
		if (rc) {
			/* Free space allocated for DMA descriptors */
			dma_free_coherent(dev,
				(bd_num + 1) * sizeof(struct tsi721_dma_desc),
				bd_ptr, bd_phys);
			bdma_chan->bd_base = NULL;

			/* Free space allocated for status descriptors */
			dma_free_coherent(dev,
				sts_size * sizeof(struct tsi721_dma_sts),
				sts_ptr, sts_phys);
			bdma_chan->sts_base = NULL;

			return -EIO;
		}
	}
#endif /* CONFIG_PCI_MSI */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Toggle DMA channel initialization */
	iowrite32(TSI721_DMAC_CTL_INIT,	bdma_chan->regs + TSI721_DMAC_CTL);
	ioread32(bdma_chan->regs + TSI721_DMAC_CTL);
	bdma_chan->wr_count = bdma_chan->wr_count_next = 0;
	bdma_chan->sts_rdptr = 0;
	udelay(10);

	return 0;
}

static int tsi721_bdma_ch_free(struct tsi721_bdma_chan *bdma_chan)
{
	u32 ch_stat;
<<<<<<< HEAD
=======
#ifdef CONFIG_PCI_MSI
	struct tsi721_device *priv = to_tsi721(bdma_chan->dchan.device);
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (bdma_chan->bd_base == NULL)
		return 0;

	/* Check if DMA channel still running */
	ch_stat = ioread32(bdma_chan->regs + TSI721_DMAC_STS);
	if (ch_stat & TSI721_DMAC_STS_RUN)
		return -EFAULT;

	/* Put DMA channel into init state */
	iowrite32(TSI721_DMAC_CTL_INIT,	bdma_chan->regs + TSI721_DMAC_CTL);

<<<<<<< HEAD
	/* Free space allocated for DMA descriptors */
	dma_free_coherent(bdma_chan->dchan.device->dev,
		bdma_chan->bd_num * sizeof(struct tsi721_dma_desc),
=======
#ifdef CONFIG_PCI_MSI
	if (priv->flags & TSI721_USING_MSIX) {
		free_irq(priv->msix[TSI721_VECT_DMA0_DONE +
				    bdma_chan->id].vector, (void *)bdma_chan);
		free_irq(priv->msix[TSI721_VECT_DMA0_INT +
				    bdma_chan->id].vector, (void *)bdma_chan);
	}
#endif /* CONFIG_PCI_MSI */

	/* Free space allocated for DMA descriptors */
	dma_free_coherent(bdma_chan->dchan.device->dev,
		(bdma_chan->bd_num + 1) * sizeof(struct tsi721_dma_desc),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		bdma_chan->bd_base, bdma_chan->bd_phys);
	bdma_chan->bd_base = NULL;

	/* Free space allocated for status FIFO */
	dma_free_coherent(bdma_chan->dchan.device->dev,
		bdma_chan->sts_size * sizeof(struct tsi721_dma_sts),
		bdma_chan->sts_base, bdma_chan->sts_phys);
	bdma_chan->sts_base = NULL;
	return 0;
}

static void
tsi721_bdma_interrupt_enable(struct tsi721_bdma_chan *bdma_chan, int enable)
{
	if (enable) {
		/* Clear pending BDMA channel interrupts */
		iowrite32(TSI721_DMAC_INT_ALL,
			bdma_chan->regs + TSI721_DMAC_INT);
		ioread32(bdma_chan->regs + TSI721_DMAC_INT);
		/* Enable BDMA channel interrupts */
		iowrite32(TSI721_DMAC_INT_ALL,
			bdma_chan->regs + TSI721_DMAC_INTE);
	} else {
		/* Disable BDMA channel interrupts */
		iowrite32(0, bdma_chan->regs + TSI721_DMAC_INTE);
		/* Clear pending BDMA channel interrupts */
		iowrite32(TSI721_DMAC_INT_ALL,
			bdma_chan->regs + TSI721_DMAC_INT);
	}

}

static bool tsi721_dma_is_idle(struct tsi721_bdma_chan *bdma_chan)
{
	u32 sts;

	sts = ioread32(bdma_chan->regs + TSI721_DMAC_STS);
	return ((sts & TSI721_DMAC_STS_RUN) == 0);
}

void tsi721_bdma_handler(struct tsi721_bdma_chan *bdma_chan)
{
	/* Disable BDMA channel interrupts */
	iowrite32(0, bdma_chan->regs + TSI721_DMAC_INTE);
	if (bdma_chan->active)
<<<<<<< HEAD
		tasklet_schedule(&bdma_chan->tasklet);
=======
		tasklet_hi_schedule(&bdma_chan->tasklet);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifdef CONFIG_PCI_MSI
/**
 * tsi721_omsg_msix - MSI-X interrupt handler for BDMA channels
 * @irq: Linux interrupt number
 * @ptr: Pointer to interrupt-specific data (BDMA channel structure)
 *
 * Handles BDMA channel interrupts signaled using MSI-X.
 */
static irqreturn_t tsi721_bdma_msix(int irq, void *ptr)
{
	struct tsi721_bdma_chan *bdma_chan = ptr;

<<<<<<< HEAD
	tsi721_bdma_handler(bdma_chan);
=======
	if (bdma_chan->active)
		tasklet_hi_schedule(&bdma_chan->tasklet);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return IRQ_HANDLED;
}
#endif /* CONFIG_PCI_MSI */

/* Must be called with the spinlock held */
static void tsi721_start_dma(struct tsi721_bdma_chan *bdma_chan)
{
	if (!tsi721_dma_is_idle(bdma_chan)) {
<<<<<<< HEAD
		dev_err(bdma_chan->dchan.device->dev,
			"BUG: Attempt to start non-idle channel\n");
=======
		tsi_err(&bdma_chan->dchan.dev->device,
			"DMAC%d Attempt to start non-idle channel",
			bdma_chan->id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;
	}

	if (bdma_chan->wr_count == bdma_chan->wr_count_next) {
<<<<<<< HEAD
		dev_err(bdma_chan->dchan.device->dev,
			"BUG: Attempt to start DMA with no BDs ready\n");
		return;
	}

	dev_dbg(bdma_chan->dchan.device->dev,
		"tx_chan: %p, chan: %d, regs: %p\n",
		bdma_chan, bdma_chan->dchan.chan_id, bdma_chan->regs);
=======
		tsi_err(&bdma_chan->dchan.dev->device,
			"DMAC%d Attempt to start DMA with no BDs ready %d",
			bdma_chan->id, task_pid_nr(current));
		return;
	}

	tsi_debug(DMA, &bdma_chan->dchan.dev->device, "DMAC%d (wrc=%d) %d",
		  bdma_chan->id, bdma_chan->wr_count_next,
		  task_pid_nr(current));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	iowrite32(bdma_chan->wr_count_next,
		bdma_chan->regs + TSI721_DMAC_DWRCNT);
	ioread32(bdma_chan->regs + TSI721_DMAC_DWRCNT);

	bdma_chan->wr_count = bdma_chan->wr_count_next;
}

<<<<<<< HEAD
static void tsi721_desc_put(struct tsi721_bdma_chan *bdma_chan,
			    struct tsi721_tx_desc *desc)
{
	dev_dbg(bdma_chan->dchan.device->dev,
		"Put desc: %p into free list\n", desc);

	if (desc) {
		spin_lock_bh(&bdma_chan->lock);
		list_splice_init(&desc->tx_list, &bdma_chan->free_list);
		list_add(&desc->desc_node, &bdma_chan->free_list);
		bdma_chan->wr_count_next = bdma_chan->wr_count;
		spin_unlock_bh(&bdma_chan->lock);
	}
}

static
struct tsi721_tx_desc *tsi721_desc_get(struct tsi721_bdma_chan *bdma_chan)
{
	struct tsi721_tx_desc *tx_desc, *_tx_desc;
	struct tsi721_tx_desc *ret = NULL;
	int i;

	spin_lock_bh(&bdma_chan->lock);
	list_for_each_entry_safe(tx_desc, _tx_desc,
				 &bdma_chan->free_list, desc_node) {
		if (async_tx_test_ack(&tx_desc->txd)) {
			list_del(&tx_desc->desc_node);
			ret = tx_desc;
			break;
		}
		dev_dbg(bdma_chan->dchan.device->dev,
			"desc %p not ACKed\n", tx_desc);
	}

	if (ret == NULL) {
		dev_dbg(bdma_chan->dchan.device->dev,
			"%s: unable to obtain tx descriptor\n", __func__);
		goto err_out;
	}

	i = bdma_chan->wr_count_next % bdma_chan->bd_num;
	if (i == bdma_chan->bd_num - 1) {
		i = 0;
		bdma_chan->wr_count_next++; /* skip link descriptor */
	}

	bdma_chan->wr_count_next++;
	tx_desc->txd.phys = bdma_chan->bd_phys +
				i * sizeof(struct tsi721_dma_desc);
	tx_desc->hw_desc = &((struct tsi721_dma_desc *)bdma_chan->bd_base)[i];
err_out:
	spin_unlock_bh(&bdma_chan->lock);

	return ret;
}

static int
tsi721_fill_desc(struct tsi721_bdma_chan *bdma_chan,
	struct tsi721_tx_desc *desc, struct scatterlist *sg,
	enum dma_rtype rtype, u32 sys_size)
{
	struct tsi721_dma_desc *bd_ptr = desc->hw_desc;
	u64 rio_addr;

	if (sg_dma_len(sg) > TSI721_DMAD_BCOUNT1 + 1) {
		dev_err(bdma_chan->dchan.device->dev,
			"SG element is too large\n");
		return -EINVAL;
	}

	dev_dbg(bdma_chan->dchan.device->dev,
		"desc: 0x%llx, addr: 0x%llx len: 0x%x\n",
		(u64)desc->txd.phys, (unsigned long long)sg_dma_address(sg),
		sg_dma_len(sg));

	dev_dbg(bdma_chan->dchan.device->dev,
		"bd_ptr = %p did=%d raddr=0x%llx\n",
		bd_ptr, desc->destid, desc->rio_addr);

	/* Initialize DMA descriptor */
	bd_ptr->type_id = cpu_to_le32((DTYPE1 << 29) |
					(rtype << 19) | desc->destid);
	if (desc->interrupt)
		bd_ptr->type_id |= cpu_to_le32(TSI721_DMAD_IOF);
	bd_ptr->bcount = cpu_to_le32(((desc->rio_addr & 0x3) << 30) |
					(sys_size << 26) | sg_dma_len(sg));
=======
static int
tsi721_desc_fill_init(struct tsi721_tx_desc *desc,
		      struct tsi721_dma_desc *bd_ptr,
		      struct scatterlist *sg, u32 sys_size)
{
	u64 rio_addr;

	if (bd_ptr == NULL)
		return -EINVAL;

	/* Initialize DMA descriptor */
	bd_ptr->type_id = cpu_to_le32((DTYPE1 << 29) |
				      (desc->rtype << 19) | desc->destid);
	bd_ptr->bcount = cpu_to_le32(((desc->rio_addr & 0x3) << 30) |
				     (sys_size << 26));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rio_addr = (desc->rio_addr >> 2) |
				((u64)(desc->rio_addr_u & 0x3) << 62);
	bd_ptr->raddr_lo = cpu_to_le32(rio_addr & 0xffffffff);
	bd_ptr->raddr_hi = cpu_to_le32(rio_addr >> 32);
	bd_ptr->t1.bufptr_lo = cpu_to_le32(
					(u64)sg_dma_address(sg) & 0xffffffff);
	bd_ptr->t1.bufptr_hi = cpu_to_le32((u64)sg_dma_address(sg) >> 32);
	bd_ptr->t1.s_dist = 0;
	bd_ptr->t1.s_size = 0;

	return 0;
}

<<<<<<< HEAD
static void tsi721_dma_chain_complete(struct tsi721_bdma_chan *bdma_chan,
				      struct tsi721_tx_desc *desc)
=======
static int
tsi721_desc_fill_end(struct tsi721_dma_desc *bd_ptr, u32 bcount, bool interrupt)
{
	if (bd_ptr == NULL)
		return -EINVAL;

	/* Update DMA descriptor */
	if (interrupt)
		bd_ptr->type_id |= cpu_to_le32(TSI721_DMAD_IOF);
	bd_ptr->bcount |= cpu_to_le32(bcount & TSI721_DMAD_BCOUNT1);

	return 0;
}

static void tsi721_dma_tx_err(struct tsi721_bdma_chan *bdma_chan,
			      struct tsi721_tx_desc *desc)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct dma_async_tx_descriptor *txd = &desc->txd;
	dma_async_tx_callback callback = txd->callback;
	void *param = txd->callback_param;

<<<<<<< HEAD
	list_splice_init(&desc->tx_list, &bdma_chan->free_list);
	list_move(&desc->desc_node, &bdma_chan->free_list);
	bdma_chan->completed_cookie = txd->cookie;
=======
	list_move(&desc->desc_node, &bdma_chan->free_list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (callback)
		callback(param);
}

<<<<<<< HEAD
static void tsi721_dma_complete_all(struct tsi721_bdma_chan *bdma_chan)
{
	struct tsi721_tx_desc *desc, *_d;
	LIST_HEAD(list);

	BUG_ON(!tsi721_dma_is_idle(bdma_chan));

	if (!list_empty(&bdma_chan->queue))
		tsi721_start_dma(bdma_chan);

	list_splice_init(&bdma_chan->active_list, &list);
	list_splice_init(&bdma_chan->queue, &bdma_chan->active_list);

	list_for_each_entry_safe(desc, _d, &list, desc_node)
		tsi721_dma_chain_complete(bdma_chan, desc);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void tsi721_clr_stat(struct tsi721_bdma_chan *bdma_chan)
{
	u32 srd_ptr;
	u64 *sts_ptr;
	int i, j;

	/* Check and clear descriptor status FIFO entries */
	srd_ptr = bdma_chan->sts_rdptr;
	sts_ptr = bdma_chan->sts_base;
	j = srd_ptr * 8;
	while (sts_ptr[j]) {
		for (i = 0; i < 8 && sts_ptr[j]; i++, j++)
			sts_ptr[j] = 0;

		++srd_ptr;
		srd_ptr %= bdma_chan->sts_size;
		j = srd_ptr * 8;
	}

	iowrite32(srd_ptr, bdma_chan->regs + TSI721_DMAC_DSRP);
	bdma_chan->sts_rdptr = srd_ptr;
}

<<<<<<< HEAD
static void tsi721_advance_work(struct tsi721_bdma_chan *bdma_chan)
{
	if (list_empty(&bdma_chan->active_list) ||
		list_is_singular(&bdma_chan->active_list)) {
		dev_dbg(bdma_chan->dchan.device->dev,
			"%s: Active_list empty\n", __func__);
		tsi721_dma_complete_all(bdma_chan);
	} else {
		dev_dbg(bdma_chan->dchan.device->dev,
			"%s: Active_list NOT empty\n", __func__);
		tsi721_dma_chain_complete(bdma_chan,
					tsi721_dma_first_active(bdma_chan));
		tsi721_start_dma(bdma_chan);
	}
=======
/* Must be called with the channel spinlock held */
static int tsi721_submit_sg(struct tsi721_tx_desc *desc)
{
	struct dma_chan *dchan = desc->txd.chan;
	struct tsi721_bdma_chan *bdma_chan = to_tsi721_chan(dchan);
	u32 sys_size;
	u64 rio_addr;
	dma_addr_t next_addr;
	u32 bcount;
	struct scatterlist *sg;
	unsigned int i;
	int err = 0;
	struct tsi721_dma_desc *bd_ptr = NULL;
	u32 idx, rd_idx;
	u32 add_count = 0;
	struct device *ch_dev = &dchan->dev->device;

	if (!tsi721_dma_is_idle(bdma_chan)) {
		tsi_err(ch_dev, "DMAC%d ERR: Attempt to use non-idle channel",
			bdma_chan->id);
		return -EIO;
	}

	/*
	 * Fill DMA channel's hardware buffer descriptors.
	 * (NOTE: RapidIO destination address is limited to 64 bits for now)
	 */
	rio_addr = desc->rio_addr;
	next_addr = -1;
	bcount = 0;
	sys_size = dma_to_mport(dchan->device)->sys_size;

	rd_idx = ioread32(bdma_chan->regs + TSI721_DMAC_DRDCNT);
	rd_idx %= (bdma_chan->bd_num + 1);

	idx = bdma_chan->wr_count_next % (bdma_chan->bd_num + 1);
	if (idx == bdma_chan->bd_num) {
		/* wrap around link descriptor */
		idx = 0;
		add_count++;
	}

	tsi_debug(DMA, ch_dev, "DMAC%d BD ring status: rdi=%d wri=%d",
		  bdma_chan->id, rd_idx, idx);

	for_each_sg(desc->sg, sg, desc->sg_len, i) {

		tsi_debug(DMAV, ch_dev, "DMAC%d sg%d/%d addr: 0x%llx len: %d",
			bdma_chan->id, i, desc->sg_len,
			(unsigned long long)sg_dma_address(sg), sg_dma_len(sg));

		if (sg_dma_len(sg) > TSI721_BDMA_MAX_BCOUNT) {
			tsi_err(ch_dev, "DMAC%d SG entry %d is too large",
				bdma_chan->id, i);
			err = -EINVAL;
			break;
		}

		/*
		 * If this sg entry forms contiguous block with previous one,
		 * try to merge it into existing DMA descriptor
		 */
		if (next_addr == sg_dma_address(sg) &&
		    bcount + sg_dma_len(sg) <= TSI721_BDMA_MAX_BCOUNT) {
			/* Adjust byte count of the descriptor */
			bcount += sg_dma_len(sg);
			goto entry_done;
		} else if (next_addr != -1) {
			/* Finalize descriptor using total byte count value */
			tsi721_desc_fill_end(bd_ptr, bcount, 0);
			tsi_debug(DMAV, ch_dev,	"DMAC%d prev desc final len: %d",
				  bdma_chan->id, bcount);
		}

		desc->rio_addr = rio_addr;

		if (i && idx == rd_idx) {
			tsi_debug(DMAV, ch_dev,
				  "DMAC%d HW descriptor ring is full @ %d",
				  bdma_chan->id, i);
			desc->sg = sg;
			desc->sg_len -= i;
			break;
		}

		bd_ptr = &((struct tsi721_dma_desc *)bdma_chan->bd_base)[idx];
		err = tsi721_desc_fill_init(desc, bd_ptr, sg, sys_size);
		if (err) {
			tsi_err(ch_dev, "Failed to build desc: err=%d", err);
			break;
		}

		tsi_debug(DMAV, ch_dev, "DMAC%d bd_ptr = %p did=%d raddr=0x%llx",
			  bdma_chan->id, bd_ptr, desc->destid, desc->rio_addr);

		next_addr = sg_dma_address(sg);
		bcount = sg_dma_len(sg);

		add_count++;
		if (++idx == bdma_chan->bd_num) {
			/* wrap around link descriptor */
			idx = 0;
			add_count++;
		}

entry_done:
		if (sg_is_last(sg)) {
			tsi721_desc_fill_end(bd_ptr, bcount, 0);
			tsi_debug(DMAV, ch_dev,
				  "DMAC%d last desc final len: %d",
				  bdma_chan->id, bcount);
			desc->sg_len = 0;
		} else {
			rio_addr += sg_dma_len(sg);
			next_addr += sg_dma_len(sg);
		}
	}

	if (!err)
		bdma_chan->wr_count_next += add_count;

	return err;
}

static void tsi721_advance_work(struct tsi721_bdma_chan *bdma_chan,
				struct tsi721_tx_desc *desc)
{
	int err;

	tsi_debug(DMA, &bdma_chan->dchan.dev->device, "DMAC%d", bdma_chan->id);

	if (!tsi721_dma_is_idle(bdma_chan))
		return;

	/*
	 * If there is no data transfer in progress, fetch new descriptor from
	 * the pending queue.
	*/

	if (desc == NULL && bdma_chan->active_tx == NULL &&
					!list_empty(&bdma_chan->queue)) {
		desc = list_first_entry(&bdma_chan->queue,
					struct tsi721_tx_desc, desc_node);
		list_del_init((&desc->desc_node));
		bdma_chan->active_tx = desc;
	}

	if (desc) {
		err = tsi721_submit_sg(desc);
		if (!err)
			tsi721_start_dma(bdma_chan);
		else {
			tsi721_dma_tx_err(bdma_chan, desc);
			tsi_debug(DMA, &bdma_chan->dchan.dev->device,
				"DMAC%d ERR: tsi721_submit_sg failed with err=%d",
				bdma_chan->id, err);
		}
	}

	tsi_debug(DMA, &bdma_chan->dchan.dev->device, "DMAC%d Exit",
		  bdma_chan->id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void tsi721_dma_tasklet(unsigned long data)
{
	struct tsi721_bdma_chan *bdma_chan = (struct tsi721_bdma_chan *)data;
	u32 dmac_int, dmac_sts;

	dmac_int = ioread32(bdma_chan->regs + TSI721_DMAC_INT);
<<<<<<< HEAD
	dev_dbg(bdma_chan->dchan.device->dev, "%s: DMAC%d_INT = 0x%x\n",
		__func__, bdma_chan->id, dmac_int);
=======
	tsi_debug(DMA, &bdma_chan->dchan.dev->device, "DMAC%d_INT = 0x%x",
		  bdma_chan->id, dmac_int);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Clear channel interrupts */
	iowrite32(dmac_int, bdma_chan->regs + TSI721_DMAC_INT);

	if (dmac_int & TSI721_DMAC_INT_ERR) {
<<<<<<< HEAD
		dmac_sts = ioread32(bdma_chan->regs + TSI721_DMAC_STS);
		dev_err(bdma_chan->dchan.device->dev,
			"%s: DMA ERROR - DMAC%d_STS = 0x%x\n",
			__func__, bdma_chan->id, dmac_sts);
	}

	if (dmac_int & TSI721_DMAC_INT_STFULL) {
		dev_err(bdma_chan->dchan.device->dev,
			"%s: DMAC%d descriptor status FIFO is full\n",
			__func__, bdma_chan->id);
	}

	if (dmac_int & (TSI721_DMAC_INT_DONE | TSI721_DMAC_INT_IOFDONE)) {
		tsi721_clr_stat(bdma_chan);
		spin_lock(&bdma_chan->lock);
		tsi721_advance_work(bdma_chan);
		spin_unlock(&bdma_chan->lock);
	}

=======
		int i = 10000;
		struct tsi721_tx_desc *desc;

		desc = bdma_chan->active_tx;
		dmac_sts = ioread32(bdma_chan->regs + TSI721_DMAC_STS);
		tsi_err(&bdma_chan->dchan.dev->device,
			"DMAC%d_STS = 0x%x did=%d raddr=0x%llx",
			bdma_chan->id, dmac_sts, desc->destid, desc->rio_addr);

		/* Re-initialize DMA channel if possible */

		if ((dmac_sts & TSI721_DMAC_STS_ABORT) == 0)
			goto err_out;

		tsi721_clr_stat(bdma_chan);

		spin_lock(&bdma_chan->lock);

		/* Put DMA channel into init state */
		iowrite32(TSI721_DMAC_CTL_INIT,
			  bdma_chan->regs + TSI721_DMAC_CTL);
		do {
			udelay(1);
			dmac_sts = ioread32(bdma_chan->regs + TSI721_DMAC_STS);
			i--;
		} while ((dmac_sts & TSI721_DMAC_STS_ABORT) && i);

		if (dmac_sts & TSI721_DMAC_STS_ABORT) {
			tsi_err(&bdma_chan->dchan.dev->device,
				"Failed to re-initiate DMAC%d",	bdma_chan->id);
			spin_unlock(&bdma_chan->lock);
			goto err_out;
		}

		/* Setup DMA descriptor pointers */
		iowrite32(((u64)bdma_chan->bd_phys >> 32),
			bdma_chan->regs + TSI721_DMAC_DPTRH);
		iowrite32(((u64)bdma_chan->bd_phys & TSI721_DMAC_DPTRL_MASK),
			bdma_chan->regs + TSI721_DMAC_DPTRL);

		/* Setup descriptor status FIFO */
		iowrite32(((u64)bdma_chan->sts_phys >> 32),
			bdma_chan->regs + TSI721_DMAC_DSBH);
		iowrite32(((u64)bdma_chan->sts_phys & TSI721_DMAC_DSBL_MASK),
			bdma_chan->regs + TSI721_DMAC_DSBL);
		iowrite32(TSI721_DMAC_DSSZ_SIZE(bdma_chan->sts_size),
			bdma_chan->regs + TSI721_DMAC_DSSZ);

		/* Clear interrupt bits */
		iowrite32(TSI721_DMAC_INT_ALL,
			bdma_chan->regs + TSI721_DMAC_INT);

		ioread32(bdma_chan->regs + TSI721_DMAC_INT);

		bdma_chan->wr_count = bdma_chan->wr_count_next = 0;
		bdma_chan->sts_rdptr = 0;
		udelay(10);

		desc = bdma_chan->active_tx;
		desc->status = DMA_ERROR;
		dma_cookie_complete(&desc->txd);
		list_add(&desc->desc_node, &bdma_chan->free_list);
		bdma_chan->active_tx = NULL;
		if (bdma_chan->active)
			tsi721_advance_work(bdma_chan, NULL);
		spin_unlock(&bdma_chan->lock);
	}

	if (dmac_int & TSI721_DMAC_INT_STFULL) {
		tsi_err(&bdma_chan->dchan.dev->device,
			"DMAC%d descriptor status FIFO is full",
			bdma_chan->id);
	}

	if (dmac_int & (TSI721_DMAC_INT_DONE | TSI721_DMAC_INT_IOFDONE)) {
		struct tsi721_tx_desc *desc;

		tsi721_clr_stat(bdma_chan);
		spin_lock(&bdma_chan->lock);
		desc = bdma_chan->active_tx;

		if (desc->sg_len == 0) {
			dma_async_tx_callback callback = NULL;
			void *param = NULL;

			desc->status = DMA_COMPLETE;
			dma_cookie_complete(&desc->txd);
			if (desc->txd.flags & DMA_PREP_INTERRUPT) {
				callback = desc->txd.callback;
				param = desc->txd.callback_param;
			}
			list_add(&desc->desc_node, &bdma_chan->free_list);
			bdma_chan->active_tx = NULL;
			if (bdma_chan->active)
				tsi721_advance_work(bdma_chan, NULL);
			spin_unlock(&bdma_chan->lock);
			if (callback)
				callback(param);
		} else {
			if (bdma_chan->active)
				tsi721_advance_work(bdma_chan,
						    bdma_chan->active_tx);
			spin_unlock(&bdma_chan->lock);
		}
	}
err_out:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Re-Enable BDMA channel interrupts */
	iowrite32(TSI721_DMAC_INT_ALL, bdma_chan->regs + TSI721_DMAC_INTE);
}

static dma_cookie_t tsi721_tx_submit(struct dma_async_tx_descriptor *txd)
{
	struct tsi721_tx_desc *desc = to_tsi721_desc(txd);
	struct tsi721_bdma_chan *bdma_chan = to_tsi721_chan(txd->chan);
	dma_cookie_t cookie;

<<<<<<< HEAD
	spin_lock_bh(&bdma_chan->lock);

	cookie = txd->chan->cookie;
	if (++cookie < 0)
		cookie = 1;
	txd->chan->cookie = cookie;
	txd->cookie = cookie;

	if (list_empty(&bdma_chan->active_list)) {
		list_add_tail(&desc->desc_node, &bdma_chan->active_list);
		tsi721_start_dma(bdma_chan);
	} else {
		list_add_tail(&desc->desc_node, &bdma_chan->queue);
	}

=======
	/* Check if the descriptor is detached from any lists */
	if (!list_empty(&desc->desc_node)) {
		tsi_err(&bdma_chan->dchan.dev->device,
			"DMAC%d wrong state of descriptor %p",
			bdma_chan->id, txd);
		return -EIO;
	}

	spin_lock_bh(&bdma_chan->lock);

	if (!bdma_chan->active) {
		spin_unlock_bh(&bdma_chan->lock);
		return -ENODEV;
	}

	cookie = dma_cookie_assign(txd);
	desc->status = DMA_IN_PROGRESS;
	list_add_tail(&desc->desc_node, &bdma_chan->queue);
	tsi721_advance_work(bdma_chan, NULL);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_unlock_bh(&bdma_chan->lock);
	return cookie;
}

static int tsi721_alloc_chan_resources(struct dma_chan *dchan)
{
	struct tsi721_bdma_chan *bdma_chan = to_tsi721_chan(dchan);
<<<<<<< HEAD
#ifdef CONFIG_PCI_MSI
	struct tsi721_device *priv = to_tsi721(dchan->device);
#endif
	struct tsi721_tx_desc *desc = NULL;
	LIST_HEAD(tmp_list);
	int i;
	int rc;

	if (bdma_chan->bd_base)
		return bdma_chan->bd_num - 1;

	/* Initialize BDMA channel */
	if (tsi721_bdma_ch_init(bdma_chan)) {
		dev_err(dchan->device->dev, "Unable to initialize data DMA"
			" channel %d, aborting\n", bdma_chan->id);
		return -ENOMEM;
	}

	/* Alocate matching number of logical descriptors */
	desc = kcalloc((bdma_chan->bd_num - 1), sizeof(struct tsi721_tx_desc),
			GFP_KERNEL);
	if (!desc) {
		dev_err(dchan->device->dev,
			"Failed to allocate logical descriptors\n");
		rc = -ENOMEM;
		goto err_out;
=======
	struct tsi721_tx_desc *desc = NULL;
	int i;

	tsi_debug(DMA, &dchan->dev->device, "DMAC%d", bdma_chan->id);

	if (bdma_chan->bd_base)
		return dma_txqueue_sz;

	/* Initialize BDMA channel */
	if (tsi721_bdma_ch_init(bdma_chan, dma_desc_per_channel)) {
		tsi_err(&dchan->dev->device, "Unable to initialize DMAC%d",
			bdma_chan->id);
		return -ENODEV;
	}

	/* Allocate queue of transaction descriptors */
	desc = kcalloc(dma_txqueue_sz, sizeof(struct tsi721_tx_desc),
			GFP_ATOMIC);
	if (!desc) {
		tsi_err(&dchan->dev->device,
			"DMAC%d Failed to allocate logical descriptors",
			bdma_chan->id);
		tsi721_bdma_ch_free(bdma_chan);
		return -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	bdma_chan->tx_desc = desc;

<<<<<<< HEAD
	for (i = 0; i < bdma_chan->bd_num - 1; i++) {
		dma_async_tx_descriptor_init(&desc[i].txd, dchan);
		desc[i].txd.tx_submit = tsi721_tx_submit;
		desc[i].txd.flags = DMA_CTRL_ACK;
		INIT_LIST_HEAD(&desc[i].tx_list);
		list_add_tail(&desc[i].desc_node, &tmp_list);
	}

	spin_lock_bh(&bdma_chan->lock);
	list_splice(&tmp_list, &bdma_chan->free_list);
	bdma_chan->completed_cookie = dchan->cookie = 1;
	spin_unlock_bh(&bdma_chan->lock);

#ifdef CONFIG_PCI_MSI
	if (priv->flags & TSI721_USING_MSIX) {
		/* Request interrupt service if we are in MSI-X mode */
		rc = request_irq(
			priv->msix[TSI721_VECT_DMA0_DONE +
				   bdma_chan->id].vector,
			tsi721_bdma_msix, 0,
			priv->msix[TSI721_VECT_DMA0_DONE +
				   bdma_chan->id].irq_name,
			(void *)bdma_chan);

		if (rc) {
			dev_dbg(dchan->device->dev,
				"Unable to allocate MSI-X interrupt for "
				"BDMA%d-DONE\n", bdma_chan->id);
			goto err_out;
		}

		rc = request_irq(priv->msix[TSI721_VECT_DMA0_INT +
					    bdma_chan->id].vector,
				tsi721_bdma_msix, 0,
				priv->msix[TSI721_VECT_DMA0_INT +
					   bdma_chan->id].irq_name,
				(void *)bdma_chan);

		if (rc)	{
			dev_dbg(dchan->device->dev,
				"Unable to allocate MSI-X interrupt for "
				"BDMA%d-INT\n", bdma_chan->id);
			free_irq(
				priv->msix[TSI721_VECT_DMA0_DONE +
					   bdma_chan->id].vector,
				(void *)bdma_chan);
			rc = -EIO;
			goto err_out;
		}
	}
#endif /* CONFIG_PCI_MSI */
=======
	for (i = 0; i < dma_txqueue_sz; i++) {
		dma_async_tx_descriptor_init(&desc[i].txd, dchan);
		desc[i].txd.tx_submit = tsi721_tx_submit;
		desc[i].txd.flags = DMA_CTRL_ACK;
		list_add(&desc[i].desc_node, &bdma_chan->free_list);
	}

	dma_cookie_init(dchan);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	bdma_chan->active = true;
	tsi721_bdma_interrupt_enable(bdma_chan, 1);

<<<<<<< HEAD
	return bdma_chan->bd_num - 1;

err_out:
	kfree(desc);
	tsi721_bdma_ch_free(bdma_chan);
	return rc;
}

static void tsi721_free_chan_resources(struct dma_chan *dchan)
{
	struct tsi721_bdma_chan *bdma_chan = to_tsi721_chan(dchan);
	struct tsi721_device *priv = to_tsi721(dchan->device);
	LIST_HEAD(list);

	dev_dbg(dchan->device->dev, "%s: Entry\n", __func__);

	if (bdma_chan->bd_base == NULL)
		return;

	BUG_ON(!list_empty(&bdma_chan->active_list));
	BUG_ON(!list_empty(&bdma_chan->queue));

	tsi721_bdma_interrupt_enable(bdma_chan, 0);
	bdma_chan->active = false;
=======
	return dma_txqueue_sz;
}

static void tsi721_sync_dma_irq(struct tsi721_bdma_chan *bdma_chan)
{
	struct tsi721_device *priv = to_tsi721(bdma_chan->dchan.device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_PCI_MSI
	if (priv->flags & TSI721_USING_MSIX) {
		synchronize_irq(priv->msix[TSI721_VECT_DMA0_DONE +
					   bdma_chan->id].vector);
		synchronize_irq(priv->msix[TSI721_VECT_DMA0_INT +
					   bdma_chan->id].vector);
	} else
#endif
	synchronize_irq(priv->pdev->irq);
<<<<<<< HEAD

	tasklet_kill(&bdma_chan->tasklet);

	spin_lock_bh(&bdma_chan->lock);
	list_splice_init(&bdma_chan->free_list, &list);
	spin_unlock_bh(&bdma_chan->lock);

#ifdef CONFIG_PCI_MSI
	if (priv->flags & TSI721_USING_MSIX) {
		free_irq(priv->msix[TSI721_VECT_DMA0_DONE +
				    bdma_chan->id].vector, (void *)bdma_chan);
		free_irq(priv->msix[TSI721_VECT_DMA0_INT +
				    bdma_chan->id].vector, (void *)bdma_chan);
	}
#endif /* CONFIG_PCI_MSI */

	tsi721_bdma_ch_free(bdma_chan);
	kfree(bdma_chan->tx_desc);
=======
}

static void tsi721_free_chan_resources(struct dma_chan *dchan)
{
	struct tsi721_bdma_chan *bdma_chan = to_tsi721_chan(dchan);

	tsi_debug(DMA, &dchan->dev->device, "DMAC%d", bdma_chan->id);

	if (bdma_chan->bd_base == NULL)
		return;

	tsi721_bdma_interrupt_enable(bdma_chan, 0);
	bdma_chan->active = false;
	tsi721_sync_dma_irq(bdma_chan);
	tasklet_kill(&bdma_chan->tasklet);
	INIT_LIST_HEAD(&bdma_chan->free_list);
	kfree(bdma_chan->tx_desc);
	tsi721_bdma_ch_free(bdma_chan);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static
enum dma_status tsi721_tx_status(struct dma_chan *dchan, dma_cookie_t cookie,
				 struct dma_tx_state *txstate)
{
	struct tsi721_bdma_chan *bdma_chan = to_tsi721_chan(dchan);
<<<<<<< HEAD
	dma_cookie_t		last_used;
	dma_cookie_t		last_completed;
	int			ret;

	spin_lock_bh(&bdma_chan->lock);
	last_completed = bdma_chan->completed_cookie;
	last_used = dchan->cookie;
	spin_unlock_bh(&bdma_chan->lock);

	ret = dma_async_is_complete(cookie, last_completed, last_used);

	dma_set_tx_state(txstate, last_completed, last_used, 0);

	dev_dbg(dchan->device->dev,
		"%s: exit, ret: %d, last_completed: %d, last_used: %d\n",
		__func__, ret, last_completed, last_used);

	return ret;
=======
	enum dma_status	status;

	spin_lock_bh(&bdma_chan->lock);
	status = dma_cookie_status(dchan, cookie, txstate);
	spin_unlock_bh(&bdma_chan->lock);
	return status;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void tsi721_issue_pending(struct dma_chan *dchan)
{
	struct tsi721_bdma_chan *bdma_chan = to_tsi721_chan(dchan);

<<<<<<< HEAD
	dev_dbg(dchan->device->dev, "%s: Entry\n", __func__);

	if (tsi721_dma_is_idle(bdma_chan)) {
		spin_lock_bh(&bdma_chan->lock);
		tsi721_advance_work(bdma_chan);
		spin_unlock_bh(&bdma_chan->lock);
	} else
		dev_dbg(dchan->device->dev,
			"%s: DMA channel still busy\n", __func__);
=======
	tsi_debug(DMA, &dchan->dev->device, "DMAC%d", bdma_chan->id);

	spin_lock_bh(&bdma_chan->lock);
	if (tsi721_dma_is_idle(bdma_chan) && bdma_chan->active) {
		tsi721_advance_work(bdma_chan, NULL);
	}
	spin_unlock_bh(&bdma_chan->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static
struct dma_async_tx_descriptor *tsi721_prep_rio_sg(struct dma_chan *dchan,
			struct scatterlist *sgl, unsigned int sg_len,
			enum dma_transfer_direction dir, unsigned long flags,
			void *tinfo)
{
	struct tsi721_bdma_chan *bdma_chan = to_tsi721_chan(dchan);
<<<<<<< HEAD
	struct tsi721_tx_desc *desc = NULL;
	struct tsi721_tx_desc *first = NULL;
	struct scatterlist *sg;
	struct rio_dma_ext *rext = tinfo;
	u64 rio_addr = rext->rio_addr; /* limited to 64-bit rio_addr for now */
	unsigned int i;
	u32 sys_size = dma_to_mport(dchan->device)->sys_size;
	enum dma_rtype rtype;

	if (!sgl || !sg_len) {
		dev_err(dchan->device->dev, "%s: No SG list\n", __func__);
		return NULL;
	}

=======
	struct tsi721_tx_desc *desc;
	struct rio_dma_ext *rext = tinfo;
	enum dma_rtype rtype;
	struct dma_async_tx_descriptor *txd = NULL;

	if (!sgl || !sg_len) {
		tsi_err(&dchan->dev->device, "DMAC%d No SG list",
			bdma_chan->id);
		return ERR_PTR(-EINVAL);
	}

	tsi_debug(DMA, &dchan->dev->device, "DMAC%d %s", bdma_chan->id,
		  (dir == DMA_DEV_TO_MEM)?"READ":"WRITE");

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (dir == DMA_DEV_TO_MEM)
		rtype = NREAD;
	else if (dir == DMA_MEM_TO_DEV) {
		switch (rext->wr_type) {
		case RDW_ALL_NWRITE:
			rtype = ALL_NWRITE;
			break;
		case RDW_ALL_NWRITE_R:
			rtype = ALL_NWRITE_R;
			break;
		case RDW_LAST_NWRITE_R:
		default:
			rtype = LAST_NWRITE_R;
			break;
		}
	} else {
<<<<<<< HEAD
		dev_err(dchan->device->dev,
			"%s: Unsupported DMA direction option\n", __func__);
		return NULL;
	}

	for_each_sg(sgl, sg, sg_len, i) {
		int err;

		dev_dbg(dchan->device->dev, "%s: sg #%d\n", __func__, i);
		desc = tsi721_desc_get(bdma_chan);
		if (!desc) {
			dev_err(dchan->device->dev,
				"Not enough descriptors available\n");
			goto err_desc_get;
		}

		if (sg_is_last(sg))
			desc->interrupt = (flags & DMA_PREP_INTERRUPT) != 0;
		else
			desc->interrupt = false;

		desc->destid = rext->destid;
		desc->rio_addr = rio_addr;
		desc->rio_addr_u = 0;

		err = tsi721_fill_desc(bdma_chan, desc, sg, rtype, sys_size);
		if (err) {
			dev_err(dchan->device->dev,
				"Failed to build desc: %d\n", err);
			goto err_desc_get;
		}

		rio_addr += sg_dma_len(sg);

		if (!first)
			first = desc;
		else
			list_add_tail(&desc->desc_node, &first->tx_list);
	}

	first->txd.cookie = -EBUSY;
	desc->txd.flags = flags;

	return &first->txd;

err_desc_get:
	tsi721_desc_put(bdma_chan, first);
	return NULL;
}

static int tsi721_device_control(struct dma_chan *dchan, enum dma_ctrl_cmd cmd,
			     unsigned long arg)
=======
		tsi_err(&dchan->dev->device,
			"DMAC%d Unsupported DMA direction option",
			bdma_chan->id);
		return ERR_PTR(-EINVAL);
	}

	spin_lock_bh(&bdma_chan->lock);

	if (!list_empty(&bdma_chan->free_list)) {
		desc = list_first_entry(&bdma_chan->free_list,
				struct tsi721_tx_desc, desc_node);
		list_del_init(&desc->desc_node);
		desc->destid = rext->destid;
		desc->rio_addr = rext->rio_addr;
		desc->rio_addr_u = 0;
		desc->rtype = rtype;
		desc->sg_len	= sg_len;
		desc->sg	= sgl;
		txd		= &desc->txd;
		txd->flags	= flags;
	}

	spin_unlock_bh(&bdma_chan->lock);

	if (!txd) {
		tsi_debug(DMA, &dchan->dev->device,
			  "DMAC%d free TXD is not available", bdma_chan->id);
		return ERR_PTR(-EBUSY);
	}

	return txd;
}

static int tsi721_terminate_all(struct dma_chan *dchan)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct tsi721_bdma_chan *bdma_chan = to_tsi721_chan(dchan);
	struct tsi721_tx_desc *desc, *_d;
	LIST_HEAD(list);

<<<<<<< HEAD
	dev_dbg(dchan->device->dev, "%s: Entry\n", __func__);

	if (cmd != DMA_TERMINATE_ALL)
		return -ENXIO;

	spin_lock_bh(&bdma_chan->lock);

	/* make sure to stop the transfer */
	iowrite32(TSI721_DMAC_CTL_SUSP, bdma_chan->regs + TSI721_DMAC_CTL);

	list_splice_init(&bdma_chan->active_list, &list);
	list_splice_init(&bdma_chan->queue, &list);

	list_for_each_entry_safe(desc, _d, &list, desc_node)
		tsi721_dma_chain_complete(bdma_chan, desc);
=======
	tsi_debug(DMA, &dchan->dev->device, "DMAC%d", bdma_chan->id);

	spin_lock_bh(&bdma_chan->lock);

	bdma_chan->active = false;

	while (!tsi721_dma_is_idle(bdma_chan)) {

		udelay(5);
#if (0)
		/* make sure to stop the transfer */
		iowrite32(TSI721_DMAC_CTL_SUSP,
			  bdma_chan->regs + TSI721_DMAC_CTL);

		/* Wait until DMA channel stops */
		do {
			dmac_int = ioread32(bdma_chan->regs + TSI721_DMAC_INT);
		} while ((dmac_int & TSI721_DMAC_INT_SUSP) == 0);
#endif
	}

	if (bdma_chan->active_tx)
		list_add(&bdma_chan->active_tx->desc_node, &list);
	list_splice_init(&bdma_chan->queue, &list);

	list_for_each_entry_safe(desc, _d, &list, desc_node)
		tsi721_dma_tx_err(bdma_chan, desc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_unlock_bh(&bdma_chan->lock);

	return 0;
}

<<<<<<< HEAD
int tsi721_register_dma(struct tsi721_device *priv)
{
	int i;
	int nr_channels = TSI721_DMA_MAXCH;
	int err;
	struct rio_mport *mport = priv->mport;

	mport->dma.dev = &priv->pdev->dev;
	mport->dma.chancnt = nr_channels;

	INIT_LIST_HEAD(&mport->dma.channels);

	for (i = 0; i < nr_channels; i++) {
		struct tsi721_bdma_chan *bdma_chan = &priv->bdma[i];

		if (i == TSI721_DMACH_MAINT)
			continue;

		bdma_chan->bd_num = 64;
=======
static void tsi721_dma_stop(struct tsi721_bdma_chan *bdma_chan)
{
	if (!bdma_chan->active)
		return;
	spin_lock_bh(&bdma_chan->lock);
	if (!tsi721_dma_is_idle(bdma_chan)) {
		int timeout = 100000;

		/* stop the transfer in progress */
		iowrite32(TSI721_DMAC_CTL_SUSP,
			  bdma_chan->regs + TSI721_DMAC_CTL);

		/* Wait until DMA channel stops */
		while (!tsi721_dma_is_idle(bdma_chan) && --timeout)
			udelay(1);
	}

	spin_unlock_bh(&bdma_chan->lock);
}

void tsi721_dma_stop_all(struct tsi721_device *priv)
{
	int i;

	for (i = 0; i < TSI721_DMA_MAXCH; i++) {
		if ((i != TSI721_DMACH_MAINT) && (dma_sel & (1 << i)))
			tsi721_dma_stop(&priv->bdma[i]);
	}
}

int tsi721_register_dma(struct tsi721_device *priv)
{
	int i;
	int nr_channels = 0;
	int err;
	struct rio_mport *mport = &priv->mport;

	INIT_LIST_HEAD(&mport->dma.channels);

	for (i = 0; i < TSI721_DMA_MAXCH; i++) {
		struct tsi721_bdma_chan *bdma_chan = &priv->bdma[i];

		if ((i == TSI721_DMACH_MAINT) || (dma_sel & (1 << i)) == 0)
			continue;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		bdma_chan->regs = priv->regs + TSI721_DMAC_BASE(i);

		bdma_chan->dchan.device = &mport->dma;
		bdma_chan->dchan.cookie = 1;
		bdma_chan->dchan.chan_id = i;
		bdma_chan->id = i;
		bdma_chan->active = false;

		spin_lock_init(&bdma_chan->lock);

<<<<<<< HEAD
		INIT_LIST_HEAD(&bdma_chan->active_list);
=======
		bdma_chan->active_tx = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		INIT_LIST_HEAD(&bdma_chan->queue);
		INIT_LIST_HEAD(&bdma_chan->free_list);

		tasklet_init(&bdma_chan->tasklet, tsi721_dma_tasklet,
			     (unsigned long)bdma_chan);
		list_add_tail(&bdma_chan->dchan.device_node,
			      &mport->dma.channels);
<<<<<<< HEAD
	}

=======
		nr_channels++;
	}

	mport->dma.chancnt = nr_channels;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dma_cap_zero(mport->dma.cap_mask);
	dma_cap_set(DMA_PRIVATE, mport->dma.cap_mask);
	dma_cap_set(DMA_SLAVE, mport->dma.cap_mask);

<<<<<<< HEAD
=======
	mport->dma.dev = &priv->pdev->dev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mport->dma.device_alloc_chan_resources = tsi721_alloc_chan_resources;
	mport->dma.device_free_chan_resources = tsi721_free_chan_resources;
	mport->dma.device_tx_status = tsi721_tx_status;
	mport->dma.device_issue_pending = tsi721_issue_pending;
	mport->dma.device_prep_slave_sg = tsi721_prep_rio_sg;
<<<<<<< HEAD
	mport->dma.device_control = tsi721_device_control;

	err = dma_async_device_register(&mport->dma);
	if (err)
		dev_err(&priv->pdev->dev, "Failed to register DMA device\n");

	return err;
}
=======
	mport->dma.device_terminate_all = tsi721_terminate_all;

	err = dma_async_device_register(&mport->dma);
	if (err)
		tsi_err(&priv->pdev->dev, "Failed to register DMA device");

	return err;
}

void tsi721_unregister_dma(struct tsi721_device *priv)
{
	struct rio_mport *mport = &priv->mport;
	struct dma_chan *chan, *_c;
	struct tsi721_bdma_chan *bdma_chan;

	tsi721_dma_stop_all(priv);
	dma_async_device_unregister(&mport->dma);

	list_for_each_entry_safe(chan, _c, &mport->dma.channels,
					device_node) {
		bdma_chan = to_tsi721_chan(chan);
		if (bdma_chan->active) {
			tsi721_bdma_interrupt_enable(bdma_chan, 0);
			bdma_chan->active = false;
			tsi721_sync_dma_irq(bdma_chan);
			tasklet_kill(&bdma_chan->tasklet);
			INIT_LIST_HEAD(&bdma_chan->free_list);
			kfree(bdma_chan->tx_desc);
			tsi721_bdma_ch_free(bdma_chan);
		}

		list_del(&chan->device_node);
	}
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
