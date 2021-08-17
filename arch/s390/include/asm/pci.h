#ifndef __ASM_S390_PCI_H
#define __ASM_S390_PCI_H

/* must be set before including asm-generic/pci.h */
#define PCI_DMA_BUS_IS_PHYS (0)
/* must be set before including pci_clp.h */
#define PCI_BAR_COUNT	6

<<<<<<< HEAD
#include <asm-generic/pci.h>
#include <asm-generic/pci-dma-compat.h>
#include <asm/pci_clp.h>
#include <asm/pci_debug.h>
=======
#include <linux/pci.h>
#include <linux/mutex.h>
#include <asm-generic/pci.h>
#include <asm/pci_clp.h>
#include <asm/pci_debug.h>
#include <asm/sclp.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define PCIBIOS_MIN_IO		0x1000
#define PCIBIOS_MIN_MEM		0x10000000

#define pcibios_assign_all_busses()	(0)

void __iomem *pci_iomap(struct pci_dev *, int, unsigned long);
void pci_iounmap(struct pci_dev *, void __iomem *);
int pci_domain_nr(struct pci_bus *);
int pci_proc_domain(struct pci_bus *);

<<<<<<< HEAD
/* MSI arch hooks */
#define arch_setup_msi_irqs	arch_setup_msi_irqs
#define arch_teardown_msi_irqs	arch_teardown_msi_irqs

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define ZPCI_BUS_NR			0	/* default bus number */
#define ZPCI_DEVFN			0	/* default device number */

/* PCI Function Controls */
#define ZPCI_FC_FN_ENABLED		0x80
#define ZPCI_FC_ERROR			0x40
#define ZPCI_FC_BLOCKED			0x20
#define ZPCI_FC_DMA_ENABLED		0x10

<<<<<<< HEAD
struct zpci_fmb {
	u32 format	:  8;
	u32 dma_valid	:  1;
	u32		: 23;
	u32 samples;
	u64 last_update;
	/* hardware counters */
=======
#define ZPCI_FMB_DMA_COUNTER_VALID	(1 << 23)

struct zpci_fmb_fmt0 {
	u64 dma_rbytes;
	u64 dma_wbytes;
};

struct zpci_fmb_fmt1 {
	u64 rx_bytes;
	u64 rx_packets;
	u64 tx_bytes;
	u64 tx_packets;
};

struct zpci_fmb_fmt2 {
	u64 consumed_work_units;
	u64 max_work_units;
};

struct zpci_fmb {
	u32 format	: 8;
	u32 fmt_ind	: 24;
	u32 samples;
	u64 last_update;
	/* common counters */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u64 ld_ops;
	u64 st_ops;
	u64 stb_ops;
	u64 rpcit_ops;
<<<<<<< HEAD
	u64 dma_rbytes;
	u64 dma_wbytes;
	/* software counters */
	atomic64_t allocated_pages;
	atomic64_t mapped_pages;
	atomic64_t unmapped_pages;
} __packed __aligned(16);

struct msi_map {
	unsigned long irq;
	struct msi_desc *msi;
	struct hlist_node msi_chain;
};

#define ZPCI_NR_MSI_VECS	64
#define ZPCI_MSI_MASK		(ZPCI_NR_MSI_VECS - 1)
=======
	/* format specific counters */
	union {
		struct zpci_fmb_fmt0 fmt0;
		struct zpci_fmb_fmt1 fmt1;
		struct zpci_fmb_fmt2 fmt2;
	};
} __packed __aligned(128);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

enum zpci_state {
	ZPCI_FN_STATE_RESERVED,
	ZPCI_FN_STATE_STANDBY,
	ZPCI_FN_STATE_CONFIGURED,
	ZPCI_FN_STATE_ONLINE,
	NR_ZPCI_FN_STATES,
};

struct zpci_bar_struct {
<<<<<<< HEAD
	u32		val;		/* bar start & 3 flag bits */
	u8		size;		/* order 2 exponent */
	u16		map_idx;	/* index into bar mapping array */
};

/* Private data per function */
struct zpci_dev {
	struct pci_dev	*pdev;
=======
	struct resource *res;		/* bus resource */
	u32		val;		/* bar start & 3 flag bits */
	u16		map_idx;	/* index into bar mapping array */
	u8		size;		/* order 2 exponent */
};

struct s390_domain;

/* Private data per function */
struct zpci_dev {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct pci_bus	*bus;
	struct list_head entry;		/* list of all zpci_devices, needed for hotplug, etc. */

	enum zpci_state state;
	u32		fid;		/* function ID, used by sclp */
	u32		fh;		/* function handle, used by insn's */
<<<<<<< HEAD
	u16		pchid;		/* physical channel ID */
	u8		pfgid;		/* function group ID */
	u16		domain;

	/* IRQ stuff */
	u64		msi_addr;	/* MSI address */
	struct zdev_irq_map *irq_map;
	struct msi_map *msi_map[ZPCI_NR_MSI_VECS];
=======
	u16		vfn;		/* virtual function number */
	u16		pchid;		/* physical channel ID */
	u8		pfgid;		/* function group ID */
	u8		pft;		/* pci function type */
	u16		domain;

	struct mutex lock;
	u8 pfip[CLP_PFIP_NR_SEGMENTS];	/* pci function internal path */
	u32 uid;			/* user defined id */
	u8 util_str[CLP_UTIL_STR_LEN];	/* utility string */

	/* IRQ stuff */
	u64		msi_addr;	/* MSI address */
	unsigned int	max_msi;	/* maximum number of MSI's */
	struct airq_iv *aibv;		/* adapter interrupt bit vector */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int	aisb;		/* number of the summary bit */

	/* DMA stuff */
	unsigned long	*dma_table;
	spinlock_t	dma_table_lock;
	int		tlb_refresh;

	spinlock_t	iommu_bitmap_lock;
	unsigned long	*iommu_bitmap;
<<<<<<< HEAD
=======
	unsigned long	*lazy_bitmap;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long	iommu_size;
	unsigned long	iommu_pages;
	unsigned int	next_bit;

<<<<<<< HEAD
=======
	char res_name[16];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct zpci_bar_struct bars[PCI_BAR_COUNT];

	u64		start_dma;	/* Start of available DMA addresses */
	u64		end_dma;	/* End of available DMA addresses */
	u64		dma_mask;	/* DMA address space mask */

	/* Function measurement block */
	struct zpci_fmb *fmb;
	u16		fmb_update;	/* update interval */
<<<<<<< HEAD
=======
	/* software counters */
	atomic64_t allocated_pages;
	atomic64_t mapped_pages;
	atomic64_t unmapped_pages;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	enum pci_bus_speed max_bus_speed;

	struct dentry	*debugfs_dev;
	struct dentry	*debugfs_perf;
<<<<<<< HEAD
	struct dentry	*debugfs_debug;
};

struct pci_hp_callback_ops {
	int (*create_slot)	(struct zpci_dev *zdev);
	void (*remove_slot)	(struct zpci_dev *zdev);
=======

	struct s390_domain *s390_domain; /* s390 IOMMU domain data */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static inline bool zdev_enabled(struct zpci_dev *zdev)
{
	return (zdev->fh & (1UL << 31)) ? true : false;
}

<<<<<<< HEAD
=======
extern const struct attribute_group *zpci_attr_groups[];

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* -----------------------------------------------------------------------------
  Prototypes
----------------------------------------------------------------------------- */
/* Base stuff */
<<<<<<< HEAD
struct zpci_dev *zpci_alloc_device(void);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int zpci_create_device(struct zpci_dev *);
int zpci_enable_device(struct zpci_dev *);
int zpci_disable_device(struct zpci_dev *);
void zpci_stop_device(struct zpci_dev *);
<<<<<<< HEAD
void zpci_free_device(struct zpci_dev *);
int zpci_scan_device(struct zpci_dev *);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int zpci_register_ioat(struct zpci_dev *, u8, u64, u64, u64);
int zpci_unregister_ioat(struct zpci_dev *, u8);

/* CLP */
<<<<<<< HEAD
int clp_find_pci_devices(void);
=======
int clp_scan_pci_devices(void);
int clp_rescan_pci_devices(void);
int clp_rescan_pci_devices_simple(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int clp_add_pci_device(u32, u32, int);
int clp_enable_fh(struct zpci_dev *, u8);
int clp_disable_fh(struct zpci_dev *);

<<<<<<< HEAD
/* MSI */
struct msi_desc *__irq_get_msi_desc(unsigned int);
int zpci_msi_set_mask_bits(struct msi_desc *, u32, u32);
int zpci_setup_msi_irq(struct zpci_dev *, struct msi_desc *, unsigned int, int);
void zpci_teardown_msi_irq(struct zpci_dev *, struct msi_desc *);
int zpci_msihash_init(void);
void zpci_msihash_exit(void);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_PCI
/* Error handling and recovery */
void zpci_event_error(void *);
void zpci_event_availability(void *);
<<<<<<< HEAD
#else /* CONFIG_PCI */
static inline void zpci_event_error(void *e) {}
static inline void zpci_event_availability(void *e) {}
#endif /* CONFIG_PCI */

/* Helpers */
struct zpci_dev *get_zdev(struct pci_dev *);
struct zpci_dev *get_zdev_by_fid(u32);
bool zpci_fid_present(u32);

/* sysfs */
int zpci_sysfs_add_device(struct device *);
void zpci_sysfs_remove_device(struct device *);
=======
void zpci_rescan(void);
bool zpci_is_enabled(void);
#else /* CONFIG_PCI */
static inline void zpci_event_error(void *e) {}
static inline void zpci_event_availability(void *e) {}
static inline void zpci_rescan(void) {}
#endif /* CONFIG_PCI */

#ifdef CONFIG_HOTPLUG_PCI_S390
int zpci_init_slot(struct zpci_dev *);
void zpci_exit_slot(struct zpci_dev *);
#else /* CONFIG_HOTPLUG_PCI_S390 */
static inline int zpci_init_slot(struct zpci_dev *zdev)
{
	return 0;
}
static inline void zpci_exit_slot(struct zpci_dev *zdev) {}
#endif /* CONFIG_HOTPLUG_PCI_S390 */

/* Helpers */
static inline struct zpci_dev *to_zpci(struct pci_dev *pdev)
{
	return pdev->sysdata;
}

struct zpci_dev *get_zdev_by_fid(u32);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* DMA */
int zpci_dma_init(void);
void zpci_dma_exit(void);

<<<<<<< HEAD
/* Hotplug */
extern struct mutex zpci_list_lock;
extern struct list_head zpci_list;
extern unsigned int s390_pci_probe;

void zpci_register_hp_ops(struct pci_hp_callback_ops *);
void zpci_deregister_hp_ops(void);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* FMB */
int zpci_fmb_enable_device(struct zpci_dev *);
int zpci_fmb_disable_device(struct zpci_dev *);

/* Debug */
int zpci_debug_init(void);
void zpci_debug_exit(void);
<<<<<<< HEAD
void zpci_debug_init_device(struct zpci_dev *);
void zpci_debug_exit_device(struct zpci_dev *);
void zpci_debug_info(struct zpci_dev *, struct seq_file *);

=======
void zpci_debug_init_device(struct zpci_dev *, const char *);
void zpci_debug_exit_device(struct zpci_dev *);
void zpci_debug_info(struct zpci_dev *, struct seq_file *);

/* Error reporting */
int zpci_report_error(struct pci_dev *, struct zpci_report_error_header *);

#ifdef CONFIG_NUMA

/* Returns the node based on PCI bus */
static inline int __pcibus_to_node(const struct pci_bus *bus)
{
	return NUMA_NO_NODE;
}

static inline const struct cpumask *
cpumask_of_pcibus(const struct pci_bus *bus)
{
	return cpu_online_mask;
}

#endif /* CONFIG_NUMA */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
