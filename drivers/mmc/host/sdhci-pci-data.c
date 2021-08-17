#include <linux/module.h>
#include <linux/mmc/sdhci-pci-data.h>

struct sdhci_pci_data *(*sdhci_pci_get_data)(struct pci_dev *pdev, int slotno);
EXPORT_SYMBOL_GPL(sdhci_pci_get_data);
<<<<<<< HEAD
=======

int sdhci_pci_spt_drive_strength;
EXPORT_SYMBOL_GPL(sdhci_pci_spt_drive_strength);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
