#ifndef LINUX_PCI_ATS_H
#define LINUX_PCI_ATS_H

#include <linux/pci.h>

<<<<<<< HEAD
/* Address Translation Service */
struct pci_ats {
	int pos;        /* capability position */
	int stu;        /* Smallest Translation Unit */
	int qdep;       /* Invalidate Queue Depth */
	int ref_cnt;    /* Physical Function reference count */
	unsigned int is_enabled:1;      /* Enable bit is set */
};

#ifdef CONFIG_PCI_ATS

int pci_enable_ats(struct pci_dev *dev, int ps);
void pci_disable_ats(struct pci_dev *dev);
int pci_ats_queue_depth(struct pci_dev *dev);

/**
 * pci_ats_enabled - query the ATS status
 * @dev: the PCI device
 *
 * Returns 1 if ATS capability is enabled, or 0 if not.
 */
static inline int pci_ats_enabled(struct pci_dev *dev)
{
	return dev->ats && dev->ats->is_enabled;
}

#else /* CONFIG_PCI_ATS */

static inline int pci_enable_ats(struct pci_dev *dev, int ps)
{
	return -ENODEV;
}

static inline void pci_disable_ats(struct pci_dev *dev)
{
}

static inline int pci_ats_queue_depth(struct pci_dev *dev)
{
	return -ENODEV;
}

static inline int pci_ats_enabled(struct pci_dev *dev)
{
	return 0;
}

#endif /* CONFIG_PCI_ATS */

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_PCI_PRI

int pci_enable_pri(struct pci_dev *pdev, u32 reqs);
void pci_disable_pri(struct pci_dev *pdev);
<<<<<<< HEAD
bool pci_pri_enabled(struct pci_dev *pdev);
int pci_reset_pri(struct pci_dev *pdev);
bool pci_pri_stopped(struct pci_dev *pdev);
int pci_pri_status(struct pci_dev *pdev);
=======
int pci_reset_pri(struct pci_dev *pdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#else /* CONFIG_PCI_PRI */

static inline int pci_enable_pri(struct pci_dev *pdev, u32 reqs)
{
	return -ENODEV;
}

static inline void pci_disable_pri(struct pci_dev *pdev)
{
}

<<<<<<< HEAD
static inline bool pci_pri_enabled(struct pci_dev *pdev)
{
	return false;
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline int pci_reset_pri(struct pci_dev *pdev)
{
	return -ENODEV;
}

<<<<<<< HEAD
static inline bool pci_pri_stopped(struct pci_dev *pdev)
{
	return true;
}

static inline int pci_pri_status(struct pci_dev *pdev)
{
	return -ENODEV;
}
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* CONFIG_PCI_PRI */

#ifdef CONFIG_PCI_PASID

int pci_enable_pasid(struct pci_dev *pdev, int features);
void pci_disable_pasid(struct pci_dev *pdev);
int pci_pasid_features(struct pci_dev *pdev);
int pci_max_pasids(struct pci_dev *pdev);

#else  /* CONFIG_PCI_PASID */

static inline int pci_enable_pasid(struct pci_dev *pdev, int features)
{
	return -EINVAL;
}

static inline void pci_disable_pasid(struct pci_dev *pdev)
{
}

static inline int pci_pasid_features(struct pci_dev *pdev)
{
	return -EINVAL;
}

static inline int pci_max_pasids(struct pci_dev *pdev)
{
	return -EINVAL;
}

#endif /* CONFIG_PCI_PASID */


#endif /* LINUX_PCI_ATS_H*/
