#ifndef __OF_IOMMU_H
#define __OF_IOMMU_H

<<<<<<< HEAD
=======
#include <linux/device.h>
#include <linux/iommu.h>
#include <linux/of.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_OF_IOMMU

extern int of_get_dma_window(struct device_node *dn, const char *prefix,
			     int index, unsigned long *busno, dma_addr_t *addr,
			     size_t *size);

<<<<<<< HEAD
=======
extern const struct iommu_ops *of_iommu_configure(struct device *dev,
					struct device_node *master_np);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else

static inline int of_get_dma_window(struct device_node *dn, const char *prefix,
			    int index, unsigned long *busno, dma_addr_t *addr,
			    size_t *size)
{
	return -EINVAL;
}

<<<<<<< HEAD
#endif	/* CONFIG_OF_IOMMU */

=======
static inline const struct iommu_ops *of_iommu_configure(struct device *dev,
					 struct device_node *master_np)
{
	return NULL;
}

#endif	/* CONFIG_OF_IOMMU */

void of_iommu_set_ops(struct device_node *np, const struct iommu_ops *ops);
const struct iommu_ops *of_iommu_get_ops(struct device_node *np);

extern struct of_device_id __iommu_of_table;

typedef int (*of_iommu_init_fn)(struct device_node *);

#define IOMMU_OF_DECLARE(name, compat, fn) \
	_OF_DECLARE(iommu, name, compat, fn, of_iommu_init_fn)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __OF_IOMMU_H */
