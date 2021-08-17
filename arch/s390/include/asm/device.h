/*
 * Arch specific extensions to struct device
 *
 * This file is released under the GPLv2
 */
<<<<<<< HEAD
#include <asm-generic/device.h>

=======
struct dev_archdata {
	struct dma_map_ops *dma_ops;
};

struct pdev_archdata {
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
