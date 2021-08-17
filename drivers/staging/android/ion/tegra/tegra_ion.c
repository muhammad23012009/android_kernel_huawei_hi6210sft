/*
 * drivers/gpu/tegra/tegra_ion.c
 *
 * Copyright (C) 2011 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/err.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/platform_device.h>
#include <linux/slab.h>
#include "../ion.h"
#include "../ion_priv.h"

<<<<<<< HEAD
struct ion_device *idev;
struct ion_mapper *tegra_user_mapper;
int num_heaps;
struct ion_heap **heaps;

int tegra_ion_probe(struct platform_device *pdev)
=======
static struct ion_device *idev;
static int num_heaps;
static struct ion_heap **heaps;

static int tegra_ion_probe(struct platform_device *pdev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct ion_platform_data *pdata = pdev->dev.platform_data;
	int err;
	int i;

	num_heaps = pdata->nr;

<<<<<<< HEAD
	heaps = kzalloc(sizeof(struct ion_heap *) * pdata->nr, GFP_KERNEL);

	idev = ion_device_create(NULL);
	if (IS_ERR_OR_NULL(idev)) {
		kfree(heaps);
		return PTR_ERR(idev);
	}
=======
	heaps = devm_kcalloc(&pdev->dev, pdata->nr,
			     sizeof(struct ion_heap *), GFP_KERNEL);

	idev = ion_device_create(NULL);
	if (IS_ERR(idev))
		return PTR_ERR(idev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* create the heaps as specified in the board file */
	for (i = 0; i < num_heaps; i++) {
		struct ion_platform_heap *heap_data = &pdata->heaps[i];

		heaps[i] = ion_heap_create(heap_data);
		if (IS_ERR_OR_NULL(heaps[i])) {
			err = PTR_ERR(heaps[i]);
			goto err;
		}
		ion_device_add_heap(idev, heaps[i]);
	}
	platform_set_drvdata(pdev, idev);
	return 0;
err:
<<<<<<< HEAD
	for (i = 0; i < num_heaps; i++) {
		if (heaps[i])
			ion_heap_destroy(heaps[i]);
	}
	kfree(heaps);
	return err;
}

int tegra_ion_remove(struct platform_device *pdev)
=======
	for (i = 0; i < num_heaps; ++i)
		ion_heap_destroy(heaps[i]);
	return err;
}

static int tegra_ion_remove(struct platform_device *pdev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct ion_device *idev = platform_get_drvdata(pdev);
	int i;

	ion_device_destroy(idev);
	for (i = 0; i < num_heaps; i++)
		ion_heap_destroy(heaps[i]);
<<<<<<< HEAD
	kfree(heaps);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static struct platform_driver ion_driver = {
	.probe = tegra_ion_probe,
	.remove = tegra_ion_remove,
	.driver = { .name = "ion-tegra" }
};

<<<<<<< HEAD
static int __init ion_init(void)
{
	return platform_driver_register(&ion_driver);
}

static void __exit ion_exit(void)
{
	platform_driver_unregister(&ion_driver);
}

module_init(ion_init);
module_exit(ion_exit);
=======
module_platform_driver(ion_driver);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

