#ifndef __RDC321X_MFD_H
#define __RDC321X_MFD_H

#include <linux/types.h>
#include <linux/pci.h>

/* Offsets to be accessed in the southbridge PCI
 * device configuration register */
#define RDC321X_WDT_CTRL	0x44
#define RDC321X_GPIO_CTRL_REG1	0x48
#define RDC321X_GPIO_DATA_REG1	0x4c
#define RDC321X_GPIO_CTRL_REG2	0x84
#define RDC321X_GPIO_DATA_REG2	0x88

<<<<<<< HEAD
#define RDC321X_MAX_GPIO	58
=======
#define RDC321X_NUM_GPIO	59
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct rdc321x_gpio_pdata {
	struct pci_dev *sb_pdev;
	unsigned max_gpios;
};

struct rdc321x_wdt_pdata {
	struct pci_dev *sb_pdev;
};

#endif /* __RDC321X_MFD_H */
