/*
 * AHCI SATA platform driver
 *
 * Copyright 2004-2005  Red Hat, Inc.
 *   Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2010  MontaVista Software, LLC.
 *   Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 */

#ifndef _AHCI_PLATFORM_H
#define _AHCI_PLATFORM_H

#include <linux/compiler.h>

struct device;
struct ata_port_info;
<<<<<<< HEAD

struct ahci_platform_data {
	int (*init)(struct device *dev, void __iomem *addr);
	void (*exit)(struct device *dev);
	int (*suspend)(struct device *dev);
	int (*resume)(struct device *dev);
	const struct ata_port_info *ata_port_info;
	unsigned int force_port_map;
	unsigned int mask_port_map;
};
=======
struct ahci_host_priv;
struct platform_device;
struct scsi_host_template;

int ahci_platform_enable_phys(struct ahci_host_priv *hpriv);
void ahci_platform_disable_phys(struct ahci_host_priv *hpriv);
int ahci_platform_enable_clks(struct ahci_host_priv *hpriv);
void ahci_platform_disable_clks(struct ahci_host_priv *hpriv);
int ahci_platform_enable_regulators(struct ahci_host_priv *hpriv);
void ahci_platform_disable_regulators(struct ahci_host_priv *hpriv);
int ahci_platform_enable_resources(struct ahci_host_priv *hpriv);
void ahci_platform_disable_resources(struct ahci_host_priv *hpriv);
struct ahci_host_priv *ahci_platform_get_resources(
	struct platform_device *pdev);
int ahci_platform_init_host(struct platform_device *pdev,
			    struct ahci_host_priv *hpriv,
			    const struct ata_port_info *pi_template,
			    struct scsi_host_template *sht);

int ahci_platform_suspend_host(struct device *dev);
int ahci_platform_resume_host(struct device *dev);
int ahci_platform_suspend(struct device *dev);
int ahci_platform_resume(struct device *dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _AHCI_PLATFORM_H */
