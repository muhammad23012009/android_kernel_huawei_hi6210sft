#ifndef LINUX_BCMA_SOC_H_
#define LINUX_BCMA_SOC_H_

#include <linux/bcma/bcma.h>

struct bcma_soc {
	struct bcma_bus bus;
<<<<<<< HEAD
	struct bcma_device core_cc;
	struct bcma_device core_mips;
};

int __init bcma_host_soc_register(struct bcma_soc *soc);
=======
};

int __init bcma_host_soc_register(struct bcma_soc *soc);
int __init bcma_host_soc_init(struct bcma_soc *soc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int bcma_bus_register(struct bcma_bus *bus);

#endif /* LINUX_BCMA_SOC_H_ */
