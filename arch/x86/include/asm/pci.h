#ifndef _ASM_X86_PCI_H
#define _ASM_X86_PCI_H

#include <linux/mm.h> /* for struct page */
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>
<<<<<<< HEAD
#include <asm/scatterlist.h>
=======
#include <linux/scatterlist.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/io.h>
#include <asm/x86_init.h>

#ifdef __KERNEL__

struct pci_sysdata {
	int		domain;		/* PCI domain */
	int		node;		/* NUMA node */
#ifdef CONFIG_ACPI
<<<<<<< HEAD
	void		*acpi;		/* ACPI-specific data */
=======
	struct acpi_device *companion;	/* ACPI companion device */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
#ifdef CONFIG_X86_64
	void		*iommu;		/* IOMMU private data */
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_PCI_MSI_IRQ_DOMAIN
	void		*fwnode;	/* IRQ domain for MSI assignment */
#endif
#if IS_ENABLED(CONFIG_VMD)
	bool vmd_domain;		/* True if in Intel VMD domain */
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

extern int pci_routeirq;
extern int noioapicquirk;
extern int noioapicreroute;

<<<<<<< HEAD
/* scan a bus after allocating a pci_sysdata for it */
extern struct pci_bus *pci_scan_bus_on_node(int busno, struct pci_ops *ops,
					    int node);
extern struct pci_bus *pci_scan_bus_with_sysdata(int busno);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_PCI

#ifdef CONFIG_PCI_DOMAINS
static inline int pci_domain_nr(struct pci_bus *bus)
{
	struct pci_sysdata *sd = bus->sysdata;
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return sd->domain;
}

static inline int pci_proc_domain(struct pci_bus *bus)
{
	return pci_domain_nr(bus);
}
#endif

<<<<<<< HEAD
=======
#ifdef CONFIG_PCI_MSI_IRQ_DOMAIN
static inline void *_pci_root_bus_fwnode(struct pci_bus *bus)
{
	struct pci_sysdata *sd = bus->sysdata;

	return sd->fwnode;
}

#define pci_root_bus_fwnode	_pci_root_bus_fwnode
#endif

static inline bool is_vmd(struct pci_bus *bus)
{
#if IS_ENABLED(CONFIG_VMD)
	struct pci_sysdata *sd = bus->sysdata;

	return sd->vmd_domain;
#else
	return false;
#endif
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Can be used to override the logic in pci_scan_bus for skipping
   already-configured bus numbers - to be used for buggy BIOSes
   or architectures with incomplete PCI setup by the loader */

extern unsigned int pcibios_assign_all_busses(void);
extern int pci_legacy_init(void);
# ifdef CONFIG_ACPI
#  define x86_default_pci_init pci_acpi_init
# else
#  define x86_default_pci_init pci_legacy_init
# endif
#else
# define pcibios_assign_all_busses()	0
# define x86_default_pci_init		NULL
#endif

extern unsigned long pci_mem_start;
#define PCIBIOS_MIN_IO		0x1000
#define PCIBIOS_MIN_MEM		(pci_mem_start)

#define PCIBIOS_MIN_CARDBUS_IO	0x4000

extern int pcibios_enabled;
void pcibios_config_init(void);
<<<<<<< HEAD
struct pci_bus *pcibios_scan_root(int bus);

void pcibios_set_master(struct pci_dev *dev);
void pcibios_penalize_isa_irq(int irq, int active);
=======
void pcibios_scan_root(int bus);

void pcibios_set_master(struct pci_dev *dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct irq_routing_table *pcibios_get_irq_routing_table(void);
int pcibios_set_irq_routing(struct pci_dev *dev, int pin, int irq);


#define HAVE_PCI_MMAP
extern int pci_mmap_page_range(struct pci_dev *dev, struct vm_area_struct *vma,
			       enum pci_mmap_state mmap_state,
			       int write_combine);


#ifdef CONFIG_PCI
extern void early_quirks(void);
<<<<<<< HEAD
static inline void pci_dma_burst_advice(struct pci_dev *pdev,
					enum pci_dma_burst_strategy *strat,
					unsigned long *strategy_parameter)
{
	*strat = PCI_DMA_BURST_INFINITY;
	*strategy_parameter = ~0UL;
}
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
static inline void early_quirks(void) { }
#endif

extern void pci_iommu_alloc(void);

#ifdef CONFIG_PCI_MSI
<<<<<<< HEAD
/* MSI arch specific hooks */
static inline int x86_setup_msi_irqs(struct pci_dev *dev, int nvec, int type)
{
	return x86_msi.setup_msi_irqs(dev, nvec, type);
}

static inline void x86_teardown_msi_irqs(struct pci_dev *dev)
{
	x86_msi.teardown_msi_irqs(dev);
}

static inline void x86_teardown_msi_irq(unsigned int irq)
{
	x86_msi.teardown_msi_irq(irq);
}
static inline void x86_restore_msi_irqs(struct pci_dev *dev, int irq)
{
	x86_msi.restore_msi_irqs(dev, irq);
}
#define arch_setup_msi_irqs x86_setup_msi_irqs
#define arch_teardown_msi_irqs x86_teardown_msi_irqs
#define arch_teardown_msi_irq x86_teardown_msi_irq
#define arch_restore_msi_irqs x86_restore_msi_irqs
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* implemented in arch/x86/kernel/apic/io_apic. */
struct msi_desc;
int native_setup_msi_irqs(struct pci_dev *dev, int nvec, int type);
void native_teardown_msi_irq(unsigned int irq);
<<<<<<< HEAD
void native_restore_msi_irqs(struct pci_dev *dev, int irq);
int setup_msi_irq(struct pci_dev *dev, struct msi_desc *msidesc,
		  unsigned int irq_base, unsigned int irq_offset);
/* default to the implementation in drivers/lib/msi.c */
#define HAVE_DEFAULT_MSI_TEARDOWN_IRQS
#define HAVE_DEFAULT_MSI_RESTORE_IRQS
void default_teardown_msi_irqs(struct pci_dev *dev);
void default_restore_msi_irqs(struct pci_dev *dev, int irq);
#else
#define native_setup_msi_irqs		NULL
#define native_teardown_msi_irq		NULL
#define default_teardown_msi_irqs	NULL
#define default_restore_msi_irqs	NULL
=======
void native_restore_msi_irqs(struct pci_dev *dev);
#else
#define native_setup_msi_irqs		NULL
#define native_teardown_msi_irq		NULL
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

#define PCI_DMA_BUS_IS_PHYS (dma_ops->is_phys)

#endif  /* __KERNEL__ */

#ifdef CONFIG_X86_64
#include <asm/pci_64.h>
#endif

<<<<<<< HEAD
/* implement the pci_ DMA API in terms of the generic device dma_ one */
#include <asm-generic/pci-dma-compat.h>

/* generic pci stuff */
#include <asm-generic/pci.h>
#define PCIBIOS_MAX_MEM_32 0xffffffff
=======
/* generic pci stuff */
#include <asm-generic/pci.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_NUMA
/* Returns the node based on pci bus */
static inline int __pcibus_to_node(const struct pci_bus *bus)
{
	const struct pci_sysdata *sd = bus->sysdata;

	return sd->node;
}

static inline const struct cpumask *
cpumask_of_pcibus(const struct pci_bus *bus)
{
	int node;

	node = __pcibus_to_node(bus);
	return (node == -1) ? cpu_online_mask :
			      cpumask_of_node(node);
}
#endif

struct pci_setup_rom {
	struct setup_data data;
	uint16_t vendor;
	uint16_t devid;
	uint64_t pcilen;
	unsigned long segment;
	unsigned long bus;
	unsigned long device;
	unsigned long function;
	uint8_t romdata[0];
};

#endif /* _ASM_X86_PCI_H */
