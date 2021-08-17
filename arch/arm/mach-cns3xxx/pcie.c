/*
 * PCI-E support for CNS3xxx
 *
 * Copyright 2008 Cavium Networks
 *		  Richard Liu <richard.liu@caviumnetworks.com>
 * Copyright 2010 MontaVista Software, LLC.
 *		  Anton Vorontsov <avorontsov@mvista.com>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/bug.h>
#include <linux/pci.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/ptrace.h>
#include <asm/mach/map.h>
#include "cns3xxx.h"
#include "core.h"

<<<<<<< HEAD
enum cns3xxx_access_type {
	CNS3XXX_HOST_TYPE = 0,
	CNS3XXX_CFG0_TYPE,
	CNS3XXX_CFG1_TYPE,
	CNS3XXX_NUM_ACCESS_TYPES,
};

struct cns3xxx_pcie {
	struct map_desc cfg_bases[CNS3XXX_NUM_ACCESS_TYPES];
	unsigned int irqs[2];
	struct resource res_io;
	struct resource res_mem;
	struct hw_pci hw_pci;

	bool linked;
};

static struct cns3xxx_pcie cns3xxx_pcie[]; /* forward decl. */

=======
struct cns3xxx_pcie {
	void __iomem *host_regs; /* PCI config registers for host bridge */
	void __iomem *cfg0_regs; /* PCI Type 0 config registers */
	void __iomem *cfg1_regs; /* PCI Type 1 config registers */
	unsigned int irqs[2];
	struct resource res_io;
	struct resource res_mem;
	int port;
	bool linked;
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct cns3xxx_pcie *sysdata_to_cnspci(void *sysdata)
{
	struct pci_sys_data *root = sysdata;

<<<<<<< HEAD
	return &cns3xxx_pcie[root->domain];
=======
	return root->private_data;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static struct cns3xxx_pcie *pdev_to_cnspci(const struct pci_dev *dev)
{
	return sysdata_to_cnspci(dev->sysdata);
}

static struct cns3xxx_pcie *pbus_to_cnspci(struct pci_bus *bus)
{
	return sysdata_to_cnspci(bus->sysdata);
}

<<<<<<< HEAD
static void __iomem *cns3xxx_pci_cfg_base(struct pci_bus *bus,
				  unsigned int devfn, int where)
=======
static void __iomem *cns3xxx_pci_map_bus(struct pci_bus *bus,
					 unsigned int devfn, int where)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct cns3xxx_pcie *cnspci = pbus_to_cnspci(bus);
	int busno = bus->number;
	int slot = PCI_SLOT(devfn);
<<<<<<< HEAD
	int offset;
	enum cns3xxx_access_type type;
	void __iomem *base;

	/* If there is no link, just show the CNS PCI bridge. */
	if (!cnspci->linked && (busno > 0 || slot > 0))
=======
	void __iomem *base;

	/* If there is no link, just show the CNS PCI bridge. */
	if (!cnspci->linked && busno > 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return NULL;

	/*
	 * The CNS PCI bridge doesn't fit into the PCI hierarchy, though
<<<<<<< HEAD
	 * we still want to access it. For this to work, we must place
	 * the first device on the same bus as the CNS PCI bridge.
	 */
	if (busno == 0) {
		if (slot > 1)
			return NULL;
		type = slot;
	} else {
		type = CNS3XXX_CFG1_TYPE;
	}

	base = (void __iomem *)cnspci->cfg_bases[type].virtual;
	offset = ((busno & 0xf) << 20) | (devfn << 12) | (where & 0xffc);

	return base + offset;
=======
	 * we still want to access it.
	 * We place the host bridge on bus 0, and the directly connected
	 * device on bus 1, slot 0.
	 */
	if (busno == 0) { /* internal PCIe bus, host bridge device */
		if (devfn == 0) /* device# and function# are ignored by hw */
			base = cnspci->host_regs;
		else
			return NULL; /* no such device */

	} else if (busno == 1) { /* directly connected PCIe device */
		if (slot == 0) /* device# is ignored by hw */
			base = cnspci->cfg0_regs;
		else
			return NULL; /* no such device */
	} else /* remote PCI bus */
		base = cnspci->cfg1_regs + ((busno & 0xf) << 20);

	return base + where + (devfn << 12);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int cns3xxx_pci_read_config(struct pci_bus *bus, unsigned int devfn,
				   int where, int size, u32 *val)
{
<<<<<<< HEAD
	u32 v;
	void __iomem *base;
	u32 mask = (0x1ull << (size * 8)) - 1;
	int shift = (where % 4) * 8;

	base = cns3xxx_pci_cfg_base(bus, devfn, where);
	if (!base) {
		*val = 0xffffffff;
		return PCIBIOS_SUCCESSFUL;
	}

	v = __raw_readl(base);

	if (bus->number == 0 && devfn == 0 &&
			(where & 0xffc) == PCI_CLASS_REVISION) {
=======
	int ret;
	u32 mask = (0x1ull << (size * 8)) - 1;
	int shift = (where % 4) * 8;

	ret = pci_generic_config_read32(bus, devfn, where, size, val);

	if (ret == PCIBIOS_SUCCESSFUL && !bus->number && !devfn &&
	    (where & 0xffc) == PCI_CLASS_REVISION)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * RC's class is 0xb, but Linux PCI driver needs 0x604
		 * for a PCIe bridge. So we must fixup the class code
		 * to 0x604 here.
		 */
<<<<<<< HEAD
		v &= 0xff;
		v |= 0x604 << 16;
	}

	*val = (v >> shift) & mask;

	return PCIBIOS_SUCCESSFUL;
}

static int cns3xxx_pci_write_config(struct pci_bus *bus, unsigned int devfn,
				    int where, int size, u32 val)
{
	u32 v;
	void __iomem *base;
	u32 mask = (0x1ull << (size * 8)) - 1;
	int shift = (where % 4) * 8;

	base = cns3xxx_pci_cfg_base(bus, devfn, where);
	if (!base)
		return PCIBIOS_SUCCESSFUL;

	v = __raw_readl(base);

	v &= ~(mask << shift);
	v |= (val & mask) << shift;

	__raw_writel(v, base);

	return PCIBIOS_SUCCESSFUL;
=======
		*val = ((((*val << shift) & 0xff) | (0x604 << 16)) >> shift) & mask;

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int cns3xxx_pci_setup(int nr, struct pci_sys_data *sys)
{
	struct cns3xxx_pcie *cnspci = sysdata_to_cnspci(sys);
	struct resource *res_io = &cnspci->res_io;
	struct resource *res_mem = &cnspci->res_mem;

	BUG_ON(request_resource(&iomem_resource, res_io) ||
	       request_resource(&iomem_resource, res_mem));

	pci_add_resource_offset(&sys->resources, res_io, sys->io_offset);
	pci_add_resource_offset(&sys->resources, res_mem, sys->mem_offset);

	return 1;
}

static struct pci_ops cns3xxx_pcie_ops = {
<<<<<<< HEAD
	.read = cns3xxx_pci_read_config,
	.write = cns3xxx_pci_write_config,
=======
	.map_bus = cns3xxx_pci_map_bus,
	.read = cns3xxx_pci_read_config,
	.write = pci_generic_config_write,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int cns3xxx_pcie_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
	struct cns3xxx_pcie *cnspci = pdev_to_cnspci(dev);
<<<<<<< HEAD
	int irq = cnspci->irqs[slot];
=======
	int irq = cnspci->irqs[!!dev->bus->number];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	pr_info("PCIe map irq: %04d:%02x:%02x.%02x slot %d, pin %d, irq: %d\n",
		pci_domain_nr(dev->bus), dev->bus->number, PCI_SLOT(dev->devfn),
		PCI_FUNC(dev->devfn), slot, pin, irq);

	return irq;
}

static struct cns3xxx_pcie cns3xxx_pcie[] = {
	[0] = {
<<<<<<< HEAD
		.cfg_bases = {
			[CNS3XXX_HOST_TYPE] = {
				.virtual = CNS3XXX_PCIE0_HOST_BASE_VIRT,
				.pfn = __phys_to_pfn(CNS3XXX_PCIE0_HOST_BASE),
				.length = SZ_16M,
				.type = MT_DEVICE,
			},
			[CNS3XXX_CFG0_TYPE] = {
				.virtual = CNS3XXX_PCIE0_CFG0_BASE_VIRT,
				.pfn = __phys_to_pfn(CNS3XXX_PCIE0_CFG0_BASE),
				.length = SZ_16M,
				.type = MT_DEVICE,
			},
			[CNS3XXX_CFG1_TYPE] = {
				.virtual = CNS3XXX_PCIE0_CFG1_BASE_VIRT,
				.pfn = __phys_to_pfn(CNS3XXX_PCIE0_CFG1_BASE),
				.length = SZ_16M,
				.type = MT_DEVICE,
			},
		},
		.res_io = {
			.name = "PCIe0 I/O space",
			.start = CNS3XXX_PCIE0_IO_BASE,
			.end = CNS3XXX_PCIE0_IO_BASE + SZ_16M - 1,
=======
		.host_regs = (void __iomem *)CNS3XXX_PCIE0_HOST_BASE_VIRT,
		.cfg0_regs = (void __iomem *)CNS3XXX_PCIE0_CFG0_BASE_VIRT,
		.cfg1_regs = (void __iomem *)CNS3XXX_PCIE0_CFG1_BASE_VIRT,
		.res_io = {
			.name = "PCIe0 I/O space",
			.start = CNS3XXX_PCIE0_IO_BASE,
			.end = CNS3XXX_PCIE0_CFG0_BASE - 1, /* 16 MiB */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			.flags = IORESOURCE_IO,
		},
		.res_mem = {
			.name = "PCIe0 non-prefetchable",
			.start = CNS3XXX_PCIE0_MEM_BASE,
<<<<<<< HEAD
			.end = CNS3XXX_PCIE0_MEM_BASE + SZ_16M - 1,
			.flags = IORESOURCE_MEM,
		},
		.irqs = { IRQ_CNS3XXX_PCIE0_RC, IRQ_CNS3XXX_PCIE0_DEVICE, },
		.hw_pci = {
			.domain = 0,
			.nr_controllers = 1,
			.ops = &cns3xxx_pcie_ops,
			.setup = cns3xxx_pci_setup,
			.map_irq = cns3xxx_pcie_map_irq,
		},
	},
	[1] = {
		.cfg_bases = {
			[CNS3XXX_HOST_TYPE] = {
				.virtual = CNS3XXX_PCIE1_HOST_BASE_VIRT,
				.pfn = __phys_to_pfn(CNS3XXX_PCIE1_HOST_BASE),
				.length = SZ_16M,
				.type = MT_DEVICE,
			},
			[CNS3XXX_CFG0_TYPE] = {
				.virtual = CNS3XXX_PCIE1_CFG0_BASE_VIRT,
				.pfn = __phys_to_pfn(CNS3XXX_PCIE1_CFG0_BASE),
				.length = SZ_16M,
				.type = MT_DEVICE,
			},
			[CNS3XXX_CFG1_TYPE] = {
				.virtual = CNS3XXX_PCIE1_CFG1_BASE_VIRT,
				.pfn = __phys_to_pfn(CNS3XXX_PCIE1_CFG1_BASE),
				.length = SZ_16M,
				.type = MT_DEVICE,
			},
		},
		.res_io = {
			.name = "PCIe1 I/O space",
			.start = CNS3XXX_PCIE1_IO_BASE,
			.end = CNS3XXX_PCIE1_IO_BASE + SZ_16M - 1,
=======
			.end = CNS3XXX_PCIE0_HOST_BASE - 1, /* 176 MiB */
			.flags = IORESOURCE_MEM,
		},
		.irqs = { IRQ_CNS3XXX_PCIE0_RC, IRQ_CNS3XXX_PCIE0_DEVICE, },
		.port = 0,
	},
	[1] = {
		.host_regs = (void __iomem *)CNS3XXX_PCIE1_HOST_BASE_VIRT,
		.cfg0_regs = (void __iomem *)CNS3XXX_PCIE1_CFG0_BASE_VIRT,
		.cfg1_regs = (void __iomem *)CNS3XXX_PCIE1_CFG1_BASE_VIRT,
		.res_io = {
			.name = "PCIe1 I/O space",
			.start = CNS3XXX_PCIE1_IO_BASE,
			.end = CNS3XXX_PCIE1_CFG0_BASE - 1, /* 16 MiB */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			.flags = IORESOURCE_IO,
		},
		.res_mem = {
			.name = "PCIe1 non-prefetchable",
			.start = CNS3XXX_PCIE1_MEM_BASE,
<<<<<<< HEAD
			.end = CNS3XXX_PCIE1_MEM_BASE + SZ_16M - 1,
			.flags = IORESOURCE_MEM,
		},
		.irqs = { IRQ_CNS3XXX_PCIE1_RC, IRQ_CNS3XXX_PCIE1_DEVICE, },
		.hw_pci = {
			.domain = 1,
			.nr_controllers = 1,
			.ops = &cns3xxx_pcie_ops,
			.setup = cns3xxx_pci_setup,
			.map_irq = cns3xxx_pcie_map_irq,
		},
=======
			.end = CNS3XXX_PCIE1_HOST_BASE - 1, /* 176 MiB */
			.flags = IORESOURCE_MEM,
		},
		.irqs = { IRQ_CNS3XXX_PCIE1_RC, IRQ_CNS3XXX_PCIE1_DEVICE, },
		.port = 1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
};

static void __init cns3xxx_pcie_check_link(struct cns3xxx_pcie *cnspci)
{
<<<<<<< HEAD
	int port = cnspci->hw_pci.domain;
=======
	int port = cnspci->port;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32 reg;
	unsigned long time;

	reg = __raw_readl(MISC_PCIE_CTRL(port));
	/*
	 * Enable Application Request to 1, it will exit L1 automatically,
	 * but when chip back, it will use another clock, still can use 0x1.
	 */
	reg |= 0x3;
	__raw_writel(reg, MISC_PCIE_CTRL(port));

	pr_info("PCIe: Port[%d] Enable PCIe LTSSM\n", port);
	pr_info("PCIe: Port[%d] Check data link layer...", port);

	time = jiffies;
	while (1) {
		reg = __raw_readl(MISC_PCIE_PM_DEBUG(port));
		if (reg & 0x1) {
			pr_info("Link up.\n");
			cnspci->linked = 1;
			break;
		} else if (time_after(jiffies, time + 50)) {
			pr_info("Device not found.\n");
			break;
		}
	}
}

<<<<<<< HEAD
static void __init cns3xxx_pcie_hw_init(struct cns3xxx_pcie *cnspci)
{
	int port = cnspci->hw_pci.domain;
	struct pci_sys_data sd = {
		.domain = port,
	};
	struct pci_bus bus = {
		.number = 0,
		.ops = &cns3xxx_pcie_ops,
		.sysdata = &sd,
	};
	u32 io_base = cnspci->res_io.start >> 16;
	u32 mem_base = cnspci->res_mem.start >> 16;
	u32 host_base = cnspci->cfg_bases[CNS3XXX_HOST_TYPE].pfn;
	u32 cfg0_base = cnspci->cfg_bases[CNS3XXX_CFG0_TYPE].pfn;
	u32 devfn = 0;
	u8 tmp8;
	u16 pos;
	u16 dc;

	host_base = (__pfn_to_phys(host_base) - 1) >> 16;
	cfg0_base = (__pfn_to_phys(cfg0_base) - 1) >> 16;

	pci_bus_write_config_byte(&bus, devfn, PCI_PRIMARY_BUS, 0);
	pci_bus_write_config_byte(&bus, devfn, PCI_SECONDARY_BUS, 1);
	pci_bus_write_config_byte(&bus, devfn, PCI_SUBORDINATE_BUS, 1);

	pci_bus_read_config_byte(&bus, devfn, PCI_PRIMARY_BUS, &tmp8);
	pci_bus_read_config_byte(&bus, devfn, PCI_SECONDARY_BUS, &tmp8);
	pci_bus_read_config_byte(&bus, devfn, PCI_SUBORDINATE_BUS, &tmp8);

	pci_bus_write_config_word(&bus, devfn, PCI_MEMORY_BASE, mem_base);
	pci_bus_write_config_word(&bus, devfn, PCI_MEMORY_LIMIT, host_base);
	pci_bus_write_config_word(&bus, devfn, PCI_IO_BASE_UPPER16, io_base);
	pci_bus_write_config_word(&bus, devfn, PCI_IO_LIMIT_UPPER16, cfg0_base);
=======
static void cns3xxx_write_config(struct cns3xxx_pcie *cnspci,
					 int where, int size, u32 val)
{
	void __iomem *base = cnspci->host_regs + (where & 0xffc);
	u32 v;
	u32 mask = (0x1ull << (size * 8)) - 1;
	int shift = (where % 4) * 8;

	v = readl_relaxed(base);

	v &= ~(mask << shift);
	v |= (val & mask) << shift;

	writel_relaxed(v, base);
	readl_relaxed(base);
}

static void __init cns3xxx_pcie_hw_init(struct cns3xxx_pcie *cnspci)
{
	u16 mem_base  = cnspci->res_mem.start >> 16;
	u16 mem_limit = cnspci->res_mem.end   >> 16;
	u16 io_base   = cnspci->res_io.start  >> 16;
	u16 io_limit  = cnspci->res_io.end    >> 16;

	cns3xxx_write_config(cnspci, PCI_PRIMARY_BUS, 1, 0);
	cns3xxx_write_config(cnspci, PCI_SECONDARY_BUS, 1, 1);
	cns3xxx_write_config(cnspci, PCI_SUBORDINATE_BUS, 1, 1);
	cns3xxx_write_config(cnspci, PCI_MEMORY_BASE, 2, mem_base);
	cns3xxx_write_config(cnspci, PCI_MEMORY_LIMIT, 2, mem_limit);
	cns3xxx_write_config(cnspci, PCI_IO_BASE_UPPER16, 2, io_base);
	cns3xxx_write_config(cnspci, PCI_IO_LIMIT_UPPER16, 2, io_limit);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!cnspci->linked)
		return;

	/* Set Device Max_Read_Request_Size to 128 byte */
<<<<<<< HEAD
	devfn = PCI_DEVFN(1, 0);
	pos = pci_bus_find_capability(&bus, devfn, PCI_CAP_ID_EXP);
	pci_bus_read_config_word(&bus, devfn, pos + PCI_EXP_DEVCTL, &dc);
	dc &= ~(0x3 << 12);	/* Clear Device Control Register [14:12] */
	pci_bus_write_config_word(&bus, devfn, pos + PCI_EXP_DEVCTL, dc);
	pci_bus_read_config_word(&bus, devfn, pos + PCI_EXP_DEVCTL, &dc);
	if (!(dc & (0x3 << 12)))
		pr_info("PCIe: Set Device Max_Read_Request_Size to 128 byte\n");

	/* Disable PCIe0 Interrupt Mask INTA to INTD */
	__raw_writel(~0x3FFF, MISC_PCIE_INT_MASK(port));
=======
	pcie_bus_config = PCIE_BUS_PEER2PEER;

	/* Disable PCIe0 Interrupt Mask INTA to INTD */
	__raw_writel(~0x3FFF, MISC_PCIE_INT_MASK(cnspci->port));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int cns3xxx_pcie_abort_handler(unsigned long addr, unsigned int fsr,
				      struct pt_regs *regs)
{
	if (fsr & (1 << 10))
		regs->ARM_pc += 4;
	return 0;
}

<<<<<<< HEAD
static int __init cns3xxx_pcie_init(void)
{
	int i;
=======
void __init cns3xxx_pcie_init_late(void)
{
	int i;
	void *private_data;
	struct hw_pci hw_pci = {
	       .nr_controllers = 1,
	       .ops = &cns3xxx_pcie_ops,
	       .setup = cns3xxx_pci_setup,
	       .map_irq = cns3xxx_pcie_map_irq,
	       .private_data = &private_data,
	};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	pcibios_min_io = 0;
	pcibios_min_mem = 0;

	hook_fault_code(16 + 6, cns3xxx_pcie_abort_handler, SIGBUS, 0,
			"imprecise external abort");

	for (i = 0; i < ARRAY_SIZE(cns3xxx_pcie); i++) {
<<<<<<< HEAD
		iotable_init(cns3xxx_pcie[i].cfg_bases,
			     ARRAY_SIZE(cns3xxx_pcie[i].cfg_bases));
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		cns3xxx_pwr_clk_en(0x1 << PM_CLK_GATE_REG_OFFSET_PCIE(i));
		cns3xxx_pwr_soft_rst(0x1 << PM_SOFT_RST_REG_OFFST_PCIE(i));
		cns3xxx_pcie_check_link(&cns3xxx_pcie[i]);
		cns3xxx_pcie_hw_init(&cns3xxx_pcie[i]);
<<<<<<< HEAD
		pci_common_init(&cns3xxx_pcie[i].hw_pci);
	}

	pci_assign_unassigned_resources();

	return 0;
}
device_initcall(cns3xxx_pcie_init);
=======
		private_data = &cns3xxx_pcie[i];
		pci_common_init(&hw_pci);
	}

	pci_assign_unassigned_resources();
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
