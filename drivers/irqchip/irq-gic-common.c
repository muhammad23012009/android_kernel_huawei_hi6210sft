/*
 * Copyright (C) 2002 ARM Limited, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/irqchip/arm-gic.h>

#include "irq-gic-common.h"

<<<<<<< HEAD
void gic_configure_irq(unsigned int irq, unsigned int type,
		       void __iomem *base, void (*sync_access)(void))
{
	u32 enablemask = 1 << (irq % 32);
	u32 enableoff = (irq / 32) * 4;
	u32 confmask = 0x2 << ((irq % 16) * 2);
	u32 confoff = (irq / 16) * 4;
	bool enabled = false;
	u32 val;
=======
static DEFINE_RAW_SPINLOCK(irq_controller_lock);

static const struct gic_kvm_info *gic_kvm_info;

const struct gic_kvm_info *gic_get_kvm_info(void)
{
	return gic_kvm_info;
}

void gic_set_kvm_info(const struct gic_kvm_info *info)
{
	BUG_ON(gic_kvm_info != NULL);
	gic_kvm_info = info;
}

void gic_enable_quirks(u32 iidr, const struct gic_quirk *quirks,
		void *data)
{
	for (; quirks->desc; quirks++) {
		if (quirks->iidr != (quirks->mask & iidr))
			continue;
		quirks->init(data);
		pr_info("GIC: enabling workaround for %s\n", quirks->desc);
	}
}

int gic_configure_irq(unsigned int irq, unsigned int type,
		       void __iomem *base, void (*sync_access)(void))
{
	u32 confmask = 0x2 << ((irq % 16) * 2);
	u32 confoff = (irq / 16) * 4;
	u32 val, oldval;
	int ret = 0;
	unsigned long flags;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Read current configuration register, and insert the config
	 * for "irq", depending on "type".
	 */
<<<<<<< HEAD
	val = readl_relaxed(base + GIC_DIST_CONFIG + confoff);
	if (type == IRQ_TYPE_LEVEL_HIGH)
		val &= ~confmask;
	else if (type == IRQ_TYPE_EDGE_RISING)
		val |= confmask;

	/*
	 * As recommended by the spec, disable the interrupt before changing
	 * the configuration
	 */
	if (readl_relaxed(base + GIC_DIST_ENABLE_SET + enableoff) & enablemask) {
		writel_relaxed(enablemask, base + GIC_DIST_ENABLE_CLEAR + enableoff);
		if (sync_access)
			sync_access();
		enabled = true;
=======
	raw_spin_lock_irqsave(&irq_controller_lock, flags);
	val = oldval = readl_relaxed(base + GIC_DIST_CONFIG + confoff);
	if (type & IRQ_TYPE_LEVEL_MASK)
		val &= ~confmask;
	else if (type & IRQ_TYPE_EDGE_BOTH)
		val |= confmask;

	/* If the current configuration is the same, then we are done */
	if (val == oldval) {
		raw_spin_unlock_irqrestore(&irq_controller_lock, flags);
		return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/*
	 * Write back the new configuration, and possibly re-enable
<<<<<<< HEAD
	 * the interrupt.
	 */
	writel_relaxed(val, base + GIC_DIST_CONFIG + confoff);

	if (enabled)
		writel_relaxed(enablemask, base + GIC_DIST_ENABLE_SET + enableoff);

	if (sync_access)
		sync_access();
}

void __init gic_dist_config(void __iomem *base, int gic_irqs,
			    void (*sync_access)(void))
=======
	 * the interrupt. If we fail to write a new configuration for
	 * an SPI then WARN and return an error. If we fail to write the
	 * configuration for a PPI this is most likely because the GIC
	 * does not allow us to set the configuration or we are in a
	 * non-secure mode, and hence it may not be catastrophic.
	 */
	writel_relaxed(val, base + GIC_DIST_CONFIG + confoff);
	if (readl_relaxed(base + GIC_DIST_CONFIG + confoff) != val) {
		if (WARN_ON(irq >= 32))
			ret = -EINVAL;
		else
			pr_warn("GIC: PPI%d is secure or misconfigured\n",
				irq - 16);
	}
	raw_spin_unlock_irqrestore(&irq_controller_lock, flags);

	if (sync_access)
		sync_access();

	return ret;
}

void gic_dist_config(void __iomem *base, int gic_irqs,
		     void (*sync_access)(void))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned int i;

	/*
	 * Set all global interrupts to be level triggered, active low.
	 */
	for (i = 32; i < gic_irqs; i += 16)
<<<<<<< HEAD
		writel_relaxed(0, base + GIC_DIST_CONFIG + i / 4);
=======
		writel_relaxed(GICD_INT_ACTLOW_LVLTRIG,
					base + GIC_DIST_CONFIG + i / 4);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Set priority on all global interrupts.
	 */
	for (i = 32; i < gic_irqs; i += 4)
<<<<<<< HEAD
		writel_relaxed(0xa0a0a0a0, base + GIC_DIST_PRI + i);

	/*
	 * Disable all interrupts.  Leave the PPI and SGIs alone
	 * as they are enabled by redistributor registers.
	 */
	for (i = 32; i < gic_irqs; i += 32)
		writel_relaxed(0xffffffff, base + GIC_DIST_ENABLE_CLEAR + i / 8);
=======
		writel_relaxed(GICD_INT_DEF_PRI_X4, base + GIC_DIST_PRI + i);

	/*
	 * Deactivate and disable all SPIs. Leave the PPI and SGIs
	 * alone as they are in the redistributor registers on GICv3.
	 */
	for (i = 32; i < gic_irqs; i += 32) {
		writel_relaxed(GICD_INT_EN_CLR_X32,
			       base + GIC_DIST_ACTIVE_CLEAR + i / 8);
		writel_relaxed(GICD_INT_EN_CLR_X32,
			       base + GIC_DIST_ENABLE_CLEAR + i / 8);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (sync_access)
		sync_access();
}

void gic_cpu_config(void __iomem *base, void (*sync_access)(void))
{
	int i;

	/*
	 * Deal with the banked PPI and SGI interrupts - disable all
	 * PPI interrupts, ensure all SGI interrupts are enabled.
<<<<<<< HEAD
	 */
	writel_relaxed(0xffff0000, base + GIC_DIST_ENABLE_CLEAR);
	writel_relaxed(0x0000ffff, base + GIC_DIST_ENABLE_SET);
=======
	 * Make sure everything is deactivated.
	 */
	writel_relaxed(GICD_INT_EN_CLR_X32, base + GIC_DIST_ACTIVE_CLEAR);
	writel_relaxed(GICD_INT_EN_CLR_PPI, base + GIC_DIST_ENABLE_CLEAR);
	writel_relaxed(GICD_INT_EN_SET_SGI, base + GIC_DIST_ENABLE_SET);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Set priority on PPI and SGI interrupts
	 */
	for (i = 0; i < 32; i += 4)
<<<<<<< HEAD
		writel_relaxed(0xa0a0a0a0, base + GIC_DIST_PRI + i * 4 / 4);
=======
		writel_relaxed(GICD_INT_DEF_PRI_X4,
					base + GIC_DIST_PRI + i * 4 / 4);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (sync_access)
		sync_access();
}
