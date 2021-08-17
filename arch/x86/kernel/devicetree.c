/*
 * Architecture specific OF callbacks.
 */
#include <linux/bootmem.h>
#include <linux/export.h>
#include <linux/io.h>
<<<<<<< HEAD
#include <linux/irqdomain.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
<<<<<<< HEAD
=======
#include <linux/libfdt.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/slab.h>
#include <linux/pci.h>
#include <linux/of_pci.h>
#include <linux/initrd.h>

<<<<<<< HEAD
#include <asm/hpet.h>
#include <asm/apic.h>
#include <asm/pci_x86.h>
=======
#include <asm/irqdomain.h>
#include <asm/hpet.h>
#include <asm/apic.h>
#include <asm/pci_x86.h>
#include <asm/setup.h>
#include <asm/i8259.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

__initdata u64 initial_dtb;
char __initdata cmd_line[COMMAND_LINE_SIZE];

int __initdata of_ioapic;

<<<<<<< HEAD
unsigned long pci_address_to_pio(phys_addr_t address)
{
	/*
	 * The ioport address can be directly used by inX / outX
	 */
	BUG_ON(address >= (1 << 16));
	return (unsigned long)address;
}
EXPORT_SYMBOL_GPL(pci_address_to_pio);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void __init early_init_dt_scan_chosen_arch(unsigned long node)
{
	BUG();
}

void __init early_init_dt_add_memory_arch(u64 base, u64 size)
{
	BUG();
}

void * __init early_init_dt_alloc_memory_arch(u64 size, u64 align)
{
	return __alloc_bootmem(size, align, __pa(MAX_DMA_ADDRESS));
}

<<<<<<< HEAD
#ifdef CONFIG_BLK_DEV_INITRD
void __init early_init_dt_setup_initrd_arch(u64 start, u64 end)
{
	initrd_start = (unsigned long)__va(start);
	initrd_end = (unsigned long)__va(end);
	initrd_below_start_ok = 1;
}
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void __init add_dtb(u64 data)
{
	initial_dtb = data + offsetof(struct setup_data, data);
}

/*
 * CE4100 ids. Will be moved to machine_device_initcall() once we have it.
 */
static struct of_device_id __initdata ce4100_ids[] = {
	{ .compatible = "intel,ce4100-cp", },
	{ .compatible = "isa", },
	{ .compatible = "pci", },
	{},
};

static int __init add_bus_probe(void)
{
	if (!of_have_populated_dt())
		return 0;

	return of_platform_bus_probe(NULL, ce4100_ids, NULL);
}
<<<<<<< HEAD
module_init(add_bus_probe);
=======
device_initcall(add_bus_probe);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_PCI
struct device_node *pcibios_get_phb_of_node(struct pci_bus *bus)
{
	struct device_node *np;

	for_each_node_by_type(np, "pci") {
		const void *prop;
		unsigned int bus_min;

		prop = of_get_property(np, "bus-range", NULL);
		if (!prop)
			continue;
		bus_min = be32_to_cpup(prop);
		if (bus->number == bus_min)
			return np;
	}
	return NULL;
}

static int x86_of_pci_irq_enable(struct pci_dev *dev)
{
<<<<<<< HEAD
	struct of_irq oirq;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32 virq;
	int ret;
	u8 pin;

	ret = pci_read_config_byte(dev, PCI_INTERRUPT_PIN, &pin);
	if (ret)
		return ret;
	if (!pin)
		return 0;

<<<<<<< HEAD
	ret = of_irq_map_pci(dev, &oirq);
	if (ret)
		return ret;

	virq = irq_create_of_mapping(oirq.controller, oirq.specifier,
			oirq.size);
=======
	virq = of_irq_parse_and_map_pci(dev, 0, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (virq == 0)
		return -EINVAL;
	dev->irq = virq;
	return 0;
}

static void x86_of_pci_irq_disable(struct pci_dev *dev)
{
}

<<<<<<< HEAD
void __cpuinit x86_of_pci_init(void)
=======
void x86_of_pci_init(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	pcibios_enable_irq = x86_of_pci_irq_enable;
	pcibios_disable_irq = x86_of_pci_irq_disable;
}
#endif

static void __init dtb_setup_hpet(void)
{
#ifdef CONFIG_HPET_TIMER
	struct device_node *dn;
	struct resource r;
	int ret;

	dn = of_find_compatible_node(NULL, NULL, "intel,ce4100-hpet");
	if (!dn)
		return;
	ret = of_address_to_resource(dn, 0, &r);
	if (ret) {
		WARN_ON(1);
		return;
	}
	hpet_address = r.start;
#endif
}

static void __init dtb_lapic_setup(void)
{
#ifdef CONFIG_X86_LOCAL_APIC
	struct device_node *dn;
	struct resource r;
	int ret;

	dn = of_find_compatible_node(NULL, NULL, "intel,ce4100-lapic");
	if (!dn)
		return;

	ret = of_address_to_resource(dn, 0, &r);
	if (WARN_ON(ret))
		return;

	/* Did the boot loader setup the local APIC ? */
<<<<<<< HEAD
	if (!cpu_has_apic) {
=======
	if (!boot_cpu_has(X86_FEATURE_APIC)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (apic_force_enable(r.start))
			return;
	}
	smp_found_config = 1;
	pic_mode = 1;
	register_lapic_address(r.start);
	generic_processor_info(boot_cpu_physical_apicid,
			       GET_APIC_VERSION(apic_read(APIC_LVR)));
#endif
}

#ifdef CONFIG_X86_IO_APIC
static unsigned int ioapic_id;

<<<<<<< HEAD
=======
struct of_ioapic_type {
	u32 out_type;
	u32 trigger;
	u32 polarity;
};

static struct of_ioapic_type of_ioapic_type[] =
{
	{
		.out_type	= IRQ_TYPE_EDGE_RISING,
		.trigger	= IOAPIC_EDGE,
		.polarity	= 1,
	},
	{
		.out_type	= IRQ_TYPE_LEVEL_LOW,
		.trigger	= IOAPIC_LEVEL,
		.polarity	= 0,
	},
	{
		.out_type	= IRQ_TYPE_LEVEL_HIGH,
		.trigger	= IOAPIC_LEVEL,
		.polarity	= 1,
	},
	{
		.out_type	= IRQ_TYPE_EDGE_FALLING,
		.trigger	= IOAPIC_EDGE,
		.polarity	= 0,
	},
};

static int dt_irqdomain_alloc(struct irq_domain *domain, unsigned int virq,
			      unsigned int nr_irqs, void *arg)
{
	struct irq_fwspec *fwspec = (struct irq_fwspec *)arg;
	struct of_ioapic_type *it;
	struct irq_alloc_info tmp;
	int type_index;

	if (WARN_ON(fwspec->param_count < 2))
		return -EINVAL;

	type_index = fwspec->param[1];
	if (type_index >= ARRAY_SIZE(of_ioapic_type))
		return -EINVAL;

	it = &of_ioapic_type[type_index];
	ioapic_set_alloc_attr(&tmp, NUMA_NO_NODE, it->trigger, it->polarity);
	tmp.ioapic_id = mpc_ioapic_id(mp_irqdomain_ioapic_idx(domain));
	tmp.ioapic_pin = fwspec->param[0];

	return mp_irqdomain_alloc(domain, virq, nr_irqs, &tmp);
}

static const struct irq_domain_ops ioapic_irq_domain_ops = {
	.alloc		= dt_irqdomain_alloc,
	.free		= mp_irqdomain_free,
	.activate	= mp_irqdomain_activate,
	.deactivate	= mp_irqdomain_deactivate,
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void __init dtb_add_ioapic(struct device_node *dn)
{
	struct resource r;
	int ret;
<<<<<<< HEAD
=======
	struct ioapic_domain_cfg cfg = {
		.type = IOAPIC_DOMAIN_DYNAMIC,
		.ops = &ioapic_irq_domain_ops,
		.dev = dn,
	};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = of_address_to_resource(dn, 0, &r);
	if (ret) {
		printk(KERN_ERR "Can't obtain address from node %s.\n",
				dn->full_name);
		return;
	}
<<<<<<< HEAD
	mp_register_ioapic(++ioapic_id, r.start, gsi_top);
=======
	mp_register_ioapic(++ioapic_id, r.start, gsi_top, &cfg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void __init dtb_ioapic_setup(void)
{
	struct device_node *dn;

	for_each_compatible_node(dn, NULL, "intel,ce4100-ioapic")
		dtb_add_ioapic(dn);

	if (nr_ioapics) {
		of_ioapic = 1;
		return;
	}
	printk(KERN_ERR "Error: No information about IO-APIC in OF.\n");
}
#else
static void __init dtb_ioapic_setup(void) {}
#endif

static void __init dtb_apic_setup(void)
{
	dtb_lapic_setup();
	dtb_ioapic_setup();
}

#ifdef CONFIG_OF_FLATTREE
static void __init x86_flattree_get_config(void)
{
	u32 size, map_len;
<<<<<<< HEAD
	void *new_dtb;
=======
	void *dt;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!initial_dtb)
		return;

<<<<<<< HEAD
	map_len = max(PAGE_SIZE - (initial_dtb & ~PAGE_MASK),
			(u64)sizeof(struct boot_param_header));

	initial_boot_params = early_memremap(initial_dtb, map_len);
	size = be32_to_cpu(initial_boot_params->totalsize);
	if (map_len < size) {
		early_iounmap(initial_boot_params, map_len);
		initial_boot_params = early_memremap(initial_dtb, size);
		map_len = size;
	}

	new_dtb = alloc_bootmem(size);
	memcpy(new_dtb, initial_boot_params, size);
	early_iounmap(initial_boot_params, map_len);

	initial_boot_params = new_dtb;

	/* root level address cells */
	of_scan_flat_dt(early_init_dt_scan_root, NULL);

	unflatten_device_tree();
=======
	map_len = max(PAGE_SIZE - (initial_dtb & ~PAGE_MASK), (u64)128);

	dt = early_memremap(initial_dtb, map_len);
	size = fdt_totalsize(dt);
	if (map_len < size) {
		early_memunmap(dt, map_len);
		dt = early_memremap(initial_dtb, size);
		map_len = size;
	}

	early_init_dt_verify(dt);
	unflatten_and_copy_device_tree();
	early_memunmap(dt, map_len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#else
static inline void x86_flattree_get_config(void) { }
#endif

void __init x86_dtb_init(void)
{
	x86_flattree_get_config();

	if (!of_have_populated_dt())
		return;

	dtb_setup_hpet();
	dtb_apic_setup();
}
<<<<<<< HEAD

#ifdef CONFIG_X86_IO_APIC

struct of_ioapic_type {
	u32 out_type;
	u32 trigger;
	u32 polarity;
};

static struct of_ioapic_type of_ioapic_type[] =
{
	{
		.out_type	= IRQ_TYPE_EDGE_RISING,
		.trigger	= IOAPIC_EDGE,
		.polarity	= 1,
	},
	{
		.out_type	= IRQ_TYPE_LEVEL_LOW,
		.trigger	= IOAPIC_LEVEL,
		.polarity	= 0,
	},
	{
		.out_type	= IRQ_TYPE_LEVEL_HIGH,
		.trigger	= IOAPIC_LEVEL,
		.polarity	= 1,
	},
	{
		.out_type	= IRQ_TYPE_EDGE_FALLING,
		.trigger	= IOAPIC_EDGE,
		.polarity	= 0,
	},
};

static int ioapic_xlate(struct irq_domain *domain,
			struct device_node *controller,
			const u32 *intspec, u32 intsize,
			irq_hw_number_t *out_hwirq, u32 *out_type)
{
	struct io_apic_irq_attr attr;
	struct of_ioapic_type *it;
	u32 line, idx;
	int rc;

	if (WARN_ON(intsize < 2))
		return -EINVAL;

	line = intspec[0];

	if (intspec[1] >= ARRAY_SIZE(of_ioapic_type))
		return -EINVAL;

	it = &of_ioapic_type[intspec[1]];

	idx = (u32) domain->host_data;
	set_io_apic_irq_attr(&attr, idx, line, it->trigger, it->polarity);

	rc = io_apic_setup_irq_pin_once(irq_find_mapping(domain, line),
					cpu_to_node(0), &attr);
	if (rc)
		return rc;

	*out_hwirq = line;
	*out_type = it->out_type;
	return 0;
}

const struct irq_domain_ops ioapic_irq_domain_ops = {
	.xlate = ioapic_xlate,
};

static void dt_add_ioapic_domain(unsigned int ioapic_num,
		struct device_node *np)
{
	struct irq_domain *id;
	struct mp_ioapic_gsi *gsi_cfg;
	int ret;
	int num;

	gsi_cfg = mp_ioapic_gsi_routing(ioapic_num);
	num = gsi_cfg->gsi_end - gsi_cfg->gsi_base + 1;

	id = irq_domain_add_linear(np, num, &ioapic_irq_domain_ops,
			(void *)ioapic_num);
	BUG_ON(!id);
	if (gsi_cfg->gsi_base == 0) {
		/*
		 * The first NR_IRQS_LEGACY irq descs are allocated in
		 * early_irq_init() and need just a mapping. The
		 * remaining irqs need both. All of them are preallocated
		 * and assigned so we can keep the 1:1 mapping which the ioapic
		 * is having.
		 */
		ret = irq_domain_associate_many(id, 0, 0, NR_IRQS_LEGACY);
		if (ret)
			pr_err("Error mapping legacy IRQs: %d\n", ret);

		if (num > NR_IRQS_LEGACY) {
			ret = irq_create_strict_mappings(id, NR_IRQS_LEGACY,
					NR_IRQS_LEGACY, num - NR_IRQS_LEGACY);
			if (ret)
				pr_err("Error creating mapping for the "
						"remaining IRQs: %d\n", ret);
		}
		irq_set_default_host(id);
	} else {
		ret = irq_create_strict_mappings(id, gsi_cfg->gsi_base, 0, num);
		if (ret)
			pr_err("Error creating IRQ mapping: %d\n", ret);
	}
}

static void __init ioapic_add_ofnode(struct device_node *np)
{
	struct resource r;
	int i, ret;

	ret = of_address_to_resource(np, 0, &r);
	if (ret) {
		printk(KERN_ERR "Failed to obtain address for %s\n",
				np->full_name);
		return;
	}

	for (i = 0; i < nr_ioapics; i++) {
		if (r.start == mpc_ioapic_addr(i)) {
			dt_add_ioapic_domain(i, np);
			return;
		}
	}
	printk(KERN_ERR "IOxAPIC at %s is not registered.\n", np->full_name);
}

void __init x86_add_irq_domains(void)
{
	struct device_node *dp;

	if (!of_have_populated_dt())
		return;

	for_each_node_with_property(dp, "interrupt-controller") {
		if (of_device_is_compatible(dp, "intel,ce4100-ioapic"))
			ioapic_add_ofnode(dp);
	}
}
#else
void __init x86_add_irq_domains(void) { }
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
