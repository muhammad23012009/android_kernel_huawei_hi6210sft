/*
 * ACPI support for platform bus type.
 *
 * Copyright (C) 2012, Intel Corporation
 * Authors: Mika Westerberg <mika.westerberg@linux.intel.com>
 *          Mathias Nyman <mathias.nyman@linux.intel.com>
 *          Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/acpi.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/dma-mapping.h>
#include <linux/pci.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/platform_device.h>

#include "internal.h"

ACPI_MODULE_NAME("platform");

<<<<<<< HEAD
/*
 * The following ACPI IDs are known to be suitable for representing as
 * platform devices.
 */
static const struct acpi_device_id acpi_platform_device_ids[] = {

	{ "PNP0D40" },

	{ }
};
=======
static const struct acpi_device_id forbidden_id_list[] = {
	{"PNP0000",  0},	/* PIC */
	{"PNP0100",  0},	/* Timer */
	{"PNP0200",  0},	/* AT DMA Controller */
	{"ACPI0009", 0},	/* IOxAPIC */
	{"ACPI000A", 0},	/* IOAPIC */
	{"SMB0001",  0},	/* ACPI SMBUS virtual device */
	{"", 0},
};

static void acpi_platform_fill_resource(struct acpi_device *adev,
	const struct resource *src, struct resource *dest)
{
	struct device *parent;

	*dest = *src;

	/*
	 * If the device has parent we need to take its resources into
	 * account as well because this device might consume part of those.
	 */
	parent = acpi_get_first_physical_node(adev->parent);
	if (parent && dev_is_pci(parent))
		dest->parent = pci_find_resource(to_pci_dev(parent), dest);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * acpi_create_platform_device - Create platform device for ACPI device node
 * @adev: ACPI device node to create a platform device for.
<<<<<<< HEAD
 * @id: ACPI device ID used to match @adev.
=======
 * @properties: Optional collection of build-in properties.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Check if the given @adev can be represented as a platform device and, if
 * that's the case, create and register a platform device, populate its common
 * resources and returns a pointer to it.  Otherwise, return %NULL.
 *
 * Name of the platform device will be the same as @adev's.
 */
<<<<<<< HEAD
int acpi_create_platform_device(struct acpi_device *adev,
				const struct acpi_device_id *id)
{
	struct platform_device *pdev = NULL;
	struct acpi_device *acpi_parent;
	struct platform_device_info pdevinfo;
	struct resource_list_entry *rentry;
	struct list_head resource_list;
	struct resource *resources;
=======
struct platform_device *acpi_create_platform_device(struct acpi_device *adev,
					struct property_entry *properties)
{
	struct platform_device *pdev = NULL;
	struct platform_device_info pdevinfo;
	struct resource_entry *rentry;
	struct list_head resource_list;
	struct resource *resources = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int count;

	/* If the ACPI node already has a physical device attached, skip it. */
	if (adev->physical_node_count)
<<<<<<< HEAD
		return 0;

	INIT_LIST_HEAD(&resource_list);
	count = acpi_dev_get_resources(adev, &resource_list, NULL, NULL);
	if (count <= 0)
		return 0;

	resources = kmalloc(count * sizeof(struct resource), GFP_KERNEL);
	if (!resources) {
		dev_err(&adev->dev, "No memory for resources\n");
		acpi_dev_free_resource_list(&resource_list);
		return -ENOMEM;
	}
	count = 0;
	list_for_each_entry(rentry, &resource_list, node)
		resources[count++] = rentry->res;

	acpi_dev_free_resource_list(&resource_list);
=======
		return NULL;

	if (!acpi_match_device_ids(adev, forbidden_id_list))
		return ERR_PTR(-EINVAL);

	INIT_LIST_HEAD(&resource_list);
	count = acpi_dev_get_resources(adev, &resource_list, NULL, NULL);
	if (count < 0) {
		return NULL;
	} else if (count > 0) {
		resources = kzalloc(count * sizeof(struct resource),
				    GFP_KERNEL);
		if (!resources) {
			dev_err(&adev->dev, "No memory for resources\n");
			acpi_dev_free_resource_list(&resource_list);
			return ERR_PTR(-ENOMEM);
		}
		count = 0;
		list_for_each_entry(rentry, &resource_list, node)
			acpi_platform_fill_resource(adev, rentry->res,
						    &resources[count++]);

		acpi_dev_free_resource_list(&resource_list);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	memset(&pdevinfo, 0, sizeof(pdevinfo));
	/*
	 * If the ACPI node has a parent and that parent has a physical device
	 * attached to it, that physical device should be the parent of the
	 * platform device we are about to create.
	 */
<<<<<<< HEAD
	pdevinfo.parent = NULL;
	acpi_parent = adev->parent;
	if (acpi_parent) {
		struct acpi_device_physical_node *entry;
		struct list_head *list;

		mutex_lock(&acpi_parent->physical_node_lock);
		list = &acpi_parent->physical_node_list;
		if (!list_empty(list)) {
			entry = list_first_entry(list,
					struct acpi_device_physical_node,
					node);
			pdevinfo.parent = entry->dev;
		}
		mutex_unlock(&acpi_parent->physical_node_lock);
	}
=======
	pdevinfo.parent = adev->parent ?
		acpi_get_first_physical_node(adev->parent) : NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pdevinfo.name = dev_name(&adev->dev);
	pdevinfo.id = -1;
	pdevinfo.res = resources;
	pdevinfo.num_res = count;
<<<<<<< HEAD
	pdevinfo.acpi_node.handle = adev->handle;
	pdev = platform_device_register_full(&pdevinfo);
	if (IS_ERR(pdev)) {
		dev_err(&adev->dev, "platform device creation failed: %ld\n",
			PTR_ERR(pdev));
		pdev = NULL;
	} else {
		dev_dbg(&adev->dev, "created platform device %s\n",
			dev_name(&pdev->dev));
	}

	kfree(resources);
	return 1;
}

static struct acpi_scan_handler platform_handler = {
	.ids = acpi_platform_device_ids,
	.attach = acpi_create_platform_device,
};

void __init acpi_platform_init(void)
{
	acpi_scan_add_handler(&platform_handler);
}
=======
	pdevinfo.fwnode = acpi_fwnode_handle(adev);
	pdevinfo.properties = properties;

	if (acpi_dma_supported(adev))
		pdevinfo.dma_mask = DMA_BIT_MASK(32);
	else
		pdevinfo.dma_mask = 0;

	pdev = platform_device_register_full(&pdevinfo);
	if (IS_ERR(pdev))
		dev_err(&adev->dev, "platform device creation failed: %ld\n",
			PTR_ERR(pdev));
	else
		dev_dbg(&adev->dev, "created platform device %s\n",
			dev_name(&pdev->dev));

	kfree(resources);
	return pdev;
}
EXPORT_SYMBOL_GPL(acpi_create_platform_device);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
