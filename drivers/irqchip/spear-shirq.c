/*
 * SPEAr platform shared irq layer source file
 *
 * Copyright (C) 2009-2012 ST Microelectronics
<<<<<<< HEAD
 * Viresh Kumar <viresh.linux@gmail.com>
 *
 * Copyright (C) 2012 ST Microelectronics
 * Shiraz Hashim <shiraz.hashim@st.com>
=======
 * Viresh Kumar <vireshk@kernel.org>
 *
 * Copyright (C) 2012 ST Microelectronics
 * Shiraz Hashim <shiraz.linux.kernel@gmail.com>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/err.h>
#include <linux/export.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/irqdomain.h>
#include <linux/irqchip/spear-shirq.h>
=======
#include <linux/irqchip.h>
#include <linux/irqdomain.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/spinlock.h>

<<<<<<< HEAD
#include "irqchip.h"

static DEFINE_SPINLOCK(lock);
=======
/*
 * struct spear_shirq: shared irq structure
 *
 * base:	Base register address
 * status_reg:	Status register offset for chained interrupt handler
 * mask_reg:	Mask register offset for irq chip
 * mask:	Mask to apply to the status register
 * virq_base:	Base virtual interrupt number
 * nr_irqs:	Number of interrupts handled by this block
 * offset:	Bit offset of the first interrupt
 * irq_chip:	Interrupt controller chip used for this instance,
 *		if NULL group is disabled, but accounted
 */
struct spear_shirq {
	void __iomem		*base;
	u32			status_reg;
	u32			mask_reg;
	u32			mask;
	u32			virq_base;
	u32			nr_irqs;
	u32			offset;
	struct irq_chip		*irq_chip;
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* spear300 shared irq registers offsets and masks */
#define SPEAR300_INT_ENB_MASK_REG	0x54
#define SPEAR300_INT_STS_MASK_REG	0x58

<<<<<<< HEAD
static struct spear_shirq spear300_shirq_ras1 = {
	.irq_nr = 9,
	.irq_bit_off = 0,
	.regs = {
		.enb_reg = SPEAR300_INT_ENB_MASK_REG,
		.status_reg = SPEAR300_INT_STS_MASK_REG,
		.clear_reg = -1,
	},
=======
static DEFINE_RAW_SPINLOCK(shirq_lock);

static void shirq_irq_mask(struct irq_data *d)
{
	struct spear_shirq *shirq = irq_data_get_irq_chip_data(d);
	u32 val, shift = d->irq - shirq->virq_base + shirq->offset;
	u32 __iomem *reg = shirq->base + shirq->mask_reg;

	raw_spin_lock(&shirq_lock);
	val = readl(reg) & ~(0x1 << shift);
	writel(val, reg);
	raw_spin_unlock(&shirq_lock);
}

static void shirq_irq_unmask(struct irq_data *d)
{
	struct spear_shirq *shirq = irq_data_get_irq_chip_data(d);
	u32 val, shift = d->irq - shirq->virq_base + shirq->offset;
	u32 __iomem *reg = shirq->base + shirq->mask_reg;

	raw_spin_lock(&shirq_lock);
	val = readl(reg) | (0x1 << shift);
	writel(val, reg);
	raw_spin_unlock(&shirq_lock);
}

static struct irq_chip shirq_chip = {
	.name		= "spear-shirq",
	.irq_mask	= shirq_irq_mask,
	.irq_unmask	= shirq_irq_unmask,
};

static struct spear_shirq spear300_shirq_ras1 = {
	.offset		= 0,
	.nr_irqs	= 9,
	.mask		= ((0x1 << 9) - 1) << 0,
	.irq_chip	= &shirq_chip,
	.status_reg	= SPEAR300_INT_STS_MASK_REG,
	.mask_reg	= SPEAR300_INT_ENB_MASK_REG,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static struct spear_shirq *spear300_shirq_blocks[] = {
	&spear300_shirq_ras1,
};

/* spear310 shared irq registers offsets and masks */
#define SPEAR310_INT_STS_MASK_REG	0x04

static struct spear_shirq spear310_shirq_ras1 = {
<<<<<<< HEAD
	.irq_nr = 8,
	.irq_bit_off = 0,
	.regs = {
		.enb_reg = -1,
		.status_reg = SPEAR310_INT_STS_MASK_REG,
		.clear_reg = -1,
	},
};

static struct spear_shirq spear310_shirq_ras2 = {
	.irq_nr = 5,
	.irq_bit_off = 8,
	.regs = {
		.enb_reg = -1,
		.status_reg = SPEAR310_INT_STS_MASK_REG,
		.clear_reg = -1,
	},
};

static struct spear_shirq spear310_shirq_ras3 = {
	.irq_nr = 1,
	.irq_bit_off = 13,
	.regs = {
		.enb_reg = -1,
		.status_reg = SPEAR310_INT_STS_MASK_REG,
		.clear_reg = -1,
	},
};

static struct spear_shirq spear310_shirq_intrcomm_ras = {
	.irq_nr = 3,
	.irq_bit_off = 14,
	.regs = {
		.enb_reg = -1,
		.status_reg = SPEAR310_INT_STS_MASK_REG,
		.clear_reg = -1,
	},
=======
	.offset		= 0,
	.nr_irqs	= 8,
	.mask		= ((0x1 << 8) - 1) << 0,
	.irq_chip	= &dummy_irq_chip,
	.status_reg	= SPEAR310_INT_STS_MASK_REG,
};

static struct spear_shirq spear310_shirq_ras2 = {
	.offset		= 8,
	.nr_irqs	= 5,
	.mask		= ((0x1 << 5) - 1) << 8,
	.irq_chip	= &dummy_irq_chip,
	.status_reg	= SPEAR310_INT_STS_MASK_REG,
};

static struct spear_shirq spear310_shirq_ras3 = {
	.offset		= 13,
	.nr_irqs	= 1,
	.mask		= ((0x1 << 1) - 1) << 13,
	.irq_chip	= &dummy_irq_chip,
	.status_reg	= SPEAR310_INT_STS_MASK_REG,
};

static struct spear_shirq spear310_shirq_intrcomm_ras = {
	.offset		= 14,
	.nr_irqs	= 3,
	.mask		= ((0x1 << 3) - 1) << 14,
	.irq_chip	= &dummy_irq_chip,
	.status_reg	= SPEAR310_INT_STS_MASK_REG,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static struct spear_shirq *spear310_shirq_blocks[] = {
	&spear310_shirq_ras1,
	&spear310_shirq_ras2,
	&spear310_shirq_ras3,
	&spear310_shirq_intrcomm_ras,
};

/* spear320 shared irq registers offsets and masks */
#define SPEAR320_INT_STS_MASK_REG		0x04
#define SPEAR320_INT_CLR_MASK_REG		0x04
#define SPEAR320_INT_ENB_MASK_REG		0x08

<<<<<<< HEAD
static struct spear_shirq spear320_shirq_ras1 = {
	.irq_nr = 3,
	.irq_bit_off = 7,
	.regs = {
		.enb_reg = -1,
		.status_reg = SPEAR320_INT_STS_MASK_REG,
		.clear_reg = SPEAR320_INT_CLR_MASK_REG,
		.reset_to_clear = 1,
	},
};

static struct spear_shirq spear320_shirq_ras2 = {
	.irq_nr = 1,
	.irq_bit_off = 10,
	.regs = {
		.enb_reg = -1,
		.status_reg = SPEAR320_INT_STS_MASK_REG,
		.clear_reg = SPEAR320_INT_CLR_MASK_REG,
		.reset_to_clear = 1,
	},
};

static struct spear_shirq spear320_shirq_ras3 = {
	.irq_nr = 7,
	.irq_bit_off = 0,
	.invalid_irq = 1,
	.regs = {
		.enb_reg = SPEAR320_INT_ENB_MASK_REG,
		.reset_to_enb = 1,
		.status_reg = SPEAR320_INT_STS_MASK_REG,
		.clear_reg = SPEAR320_INT_CLR_MASK_REG,
		.reset_to_clear = 1,
	},
};

static struct spear_shirq spear320_shirq_intrcomm_ras = {
	.irq_nr = 11,
	.irq_bit_off = 11,
	.regs = {
		.enb_reg = -1,
		.status_reg = SPEAR320_INT_STS_MASK_REG,
		.clear_reg = SPEAR320_INT_CLR_MASK_REG,
		.reset_to_clear = 1,
	},
=======
static struct spear_shirq spear320_shirq_ras3 = {
	.offset		= 0,
	.nr_irqs	= 7,
	.mask		= ((0x1 << 7) - 1) << 0,
};

static struct spear_shirq spear320_shirq_ras1 = {
	.offset		= 7,
	.nr_irqs	= 3,
	.mask		= ((0x1 << 3) - 1) << 7,
	.irq_chip	= &dummy_irq_chip,
	.status_reg	= SPEAR320_INT_STS_MASK_REG,
};

static struct spear_shirq spear320_shirq_ras2 = {
	.offset		= 10,
	.nr_irqs	= 1,
	.mask		= ((0x1 << 1) - 1) << 10,
	.irq_chip	= &dummy_irq_chip,
	.status_reg	= SPEAR320_INT_STS_MASK_REG,
};

static struct spear_shirq spear320_shirq_intrcomm_ras = {
	.offset		= 11,
	.nr_irqs	= 11,
	.mask		= ((0x1 << 11) - 1) << 11,
	.irq_chip	= &dummy_irq_chip,
	.status_reg	= SPEAR320_INT_STS_MASK_REG,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static struct spear_shirq *spear320_shirq_blocks[] = {
	&spear320_shirq_ras3,
	&spear320_shirq_ras1,
	&spear320_shirq_ras2,
	&spear320_shirq_intrcomm_ras,
};

<<<<<<< HEAD
static void shirq_irq_mask_unmask(struct irq_data *d, bool mask)
{
	struct spear_shirq *shirq = irq_data_get_irq_chip_data(d);
	u32 val, offset = d->irq - shirq->irq_base;
	unsigned long flags;

	if (shirq->regs.enb_reg == -1)
		return;

	spin_lock_irqsave(&lock, flags);
	val = readl(shirq->base + shirq->regs.enb_reg);

	if (mask ^ shirq->regs.reset_to_enb)
		val &= ~(0x1 << shirq->irq_bit_off << offset);
	else
		val |= 0x1 << shirq->irq_bit_off << offset;

	writel(val, shirq->base + shirq->regs.enb_reg);
	spin_unlock_irqrestore(&lock, flags);

}

static void shirq_irq_mask(struct irq_data *d)
{
	shirq_irq_mask_unmask(d, 1);
}

static void shirq_irq_unmask(struct irq_data *d)
{
	shirq_irq_mask_unmask(d, 0);
}

static struct irq_chip shirq_chip = {
	.name		= "spear-shirq",
	.irq_ack	= shirq_irq_mask,
	.irq_mask	= shirq_irq_mask,
	.irq_unmask	= shirq_irq_unmask,
};

static void shirq_handler(unsigned irq, struct irq_desc *desc)
{
	u32 i, j, val, mask, tmp;
	struct irq_chip *chip;
	struct spear_shirq *shirq = irq_get_handler_data(irq);

	chip = irq_get_chip(irq);
	chip->irq_ack(&desc->irq_data);

	mask = ((0x1 << shirq->irq_nr) - 1) << shirq->irq_bit_off;
	while ((val = readl(shirq->base + shirq->regs.status_reg) &
				mask)) {

		val >>= shirq->irq_bit_off;
		for (i = 0, j = 1; i < shirq->irq_nr; i++, j <<= 1) {

			if (!(j & val))
				continue;

			generic_handle_irq(shirq->irq_base + i);

			/* clear interrupt */
			if (shirq->regs.clear_reg == -1)
				continue;

			tmp = readl(shirq->base + shirq->regs.clear_reg);
			if (shirq->regs.reset_to_clear)
				tmp &= ~(j << shirq->irq_bit_off);
			else
				tmp |= (j << shirq->irq_bit_off);
			writel(tmp, shirq->base + shirq->regs.clear_reg);
		}
	}
	chip->irq_unmask(&desc->irq_data);
}

static void __init spear_shirq_register(struct spear_shirq *shirq)
{
	int i;

	if (shirq->invalid_irq)
		return;

	irq_set_chained_handler(shirq->irq, shirq_handler);
	for (i = 0; i < shirq->irq_nr; i++) {
		irq_set_chip_and_handler(shirq->irq_base + i,
					 &shirq_chip, handle_simple_irq);
		set_irq_flags(shirq->irq_base + i, IRQF_VALID);
		irq_set_chip_data(shirq->irq_base + i, shirq);
	}

	irq_set_handler_data(shirq->irq, shirq);
=======
static void shirq_handler(struct irq_desc *desc)
{
	struct spear_shirq *shirq = irq_desc_get_handler_data(desc);
	u32 pend;

	pend = readl(shirq->base + shirq->status_reg) & shirq->mask;
	pend >>= shirq->offset;

	while (pend) {
		int irq = __ffs(pend);

		pend &= ~(0x1 << irq);
		generic_handle_irq(shirq->virq_base + irq);
	}
}

static void __init spear_shirq_register(struct spear_shirq *shirq,
					int parent_irq)
{
	int i;

	if (!shirq->irq_chip)
		return;

	irq_set_chained_handler_and_data(parent_irq, shirq_handler, shirq);

	for (i = 0; i < shirq->nr_irqs; i++) {
		irq_set_chip_and_handler(shirq->virq_base + i,
					 shirq->irq_chip, handle_simple_irq);
		irq_set_chip_data(shirq->virq_base + i, shirq);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int __init shirq_init(struct spear_shirq **shirq_blocks, int block_nr,
		struct device_node *np)
{
<<<<<<< HEAD
	int i, irq_base, hwirq = 0, irq_nr = 0;
	static struct irq_domain *shirq_domain;
=======
	int i, parent_irq, virq_base, hwirq = 0, nr_irqs = 0;
	struct irq_domain *shirq_domain;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	void __iomem *base;

	base = of_iomap(np, 0);
	if (!base) {
		pr_err("%s: failed to map shirq registers\n", __func__);
		return -ENXIO;
	}

	for (i = 0; i < block_nr; i++)
<<<<<<< HEAD
		irq_nr += shirq_blocks[i]->irq_nr;

	irq_base = irq_alloc_descs(-1, 0, irq_nr, 0);
	if (IS_ERR_VALUE(irq_base)) {
=======
		nr_irqs += shirq_blocks[i]->nr_irqs;

	virq_base = irq_alloc_descs(-1, 0, nr_irqs, 0);
	if (virq_base < 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		pr_err("%s: irq desc alloc failed\n", __func__);
		goto err_unmap;
	}

<<<<<<< HEAD
	shirq_domain = irq_domain_add_legacy(np, irq_nr, irq_base, 0,
=======
	shirq_domain = irq_domain_add_legacy(np, nr_irqs, virq_base, 0,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			&irq_domain_simple_ops, NULL);
	if (WARN_ON(!shirq_domain)) {
		pr_warn("%s: irq domain init failed\n", __func__);
		goto err_free_desc;
	}

	for (i = 0; i < block_nr; i++) {
		shirq_blocks[i]->base = base;
<<<<<<< HEAD
		shirq_blocks[i]->irq_base = irq_find_mapping(shirq_domain,
				hwirq);
		shirq_blocks[i]->irq = irq_of_parse_and_map(np, i);

		spear_shirq_register(shirq_blocks[i]);
		hwirq += shirq_blocks[i]->irq_nr;
=======
		shirq_blocks[i]->virq_base = irq_find_mapping(shirq_domain,
				hwirq);

		parent_irq = irq_of_parse_and_map(np, i);
		spear_shirq_register(shirq_blocks[i], parent_irq);
		hwirq += shirq_blocks[i]->nr_irqs;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return 0;

err_free_desc:
<<<<<<< HEAD
	irq_free_descs(irq_base, irq_nr);
=======
	irq_free_descs(virq_base, nr_irqs);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
err_unmap:
	iounmap(base);
	return -ENXIO;
}

<<<<<<< HEAD
int __init spear300_shirq_of_init(struct device_node *np,
		struct device_node *parent)
=======
static int __init spear300_shirq_of_init(struct device_node *np,
					 struct device_node *parent)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return shirq_init(spear300_shirq_blocks,
			ARRAY_SIZE(spear300_shirq_blocks), np);
}
IRQCHIP_DECLARE(spear300_shirq, "st,spear300-shirq", spear300_shirq_of_init);

<<<<<<< HEAD
int __init spear310_shirq_of_init(struct device_node *np,
		struct device_node *parent)
=======
static int __init spear310_shirq_of_init(struct device_node *np,
					 struct device_node *parent)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return shirq_init(spear310_shirq_blocks,
			ARRAY_SIZE(spear310_shirq_blocks), np);
}
IRQCHIP_DECLARE(spear310_shirq, "st,spear310-shirq", spear310_shirq_of_init);

<<<<<<< HEAD
int __init spear320_shirq_of_init(struct device_node *np,
		struct device_node *parent)
=======
static int __init spear320_shirq_of_init(struct device_node *np,
					 struct device_node *parent)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return shirq_init(spear320_shirq_blocks,
			ARRAY_SIZE(spear320_shirq_blocks), np);
}
IRQCHIP_DECLARE(spear320_shirq, "st,spear320-shirq", spear320_shirq_of_init);
