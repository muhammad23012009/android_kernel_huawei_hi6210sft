/*
 * Copyright (C) 2010 Michael Neuling IBM Corporation
 *
 * Driver for the pseries hardware RNG for POWER7+ and above
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/module.h>
#include <linux/hw_random.h>
#include <asm/vio.h>

<<<<<<< HEAD
#define MODULE_NAME "pseries-rng"

static int pseries_rng_data_read(struct hwrng *rng, u32 *data)
{
	if (plpar_hcall(H_RANDOM, (unsigned long *)data) != H_SUCCESS) {
		printk(KERN_ERR "pseries rng hcall error\n");
		return 0;
	}
	return 8;
=======

static int pseries_rng_read(struct hwrng *rng, void *data, size_t max, bool wait)
{
	u64 buffer[PLPAR_HCALL_BUFSIZE];
	size_t size = max < 8 ? max : 8;
	int rc;

	rc = plpar_hcall(H_RANDOM, (unsigned long *)buffer);
	if (rc != H_SUCCESS) {
		pr_err_ratelimited("H_RANDOM call failed %d\n", rc);
		return -EIO;
	}
	memcpy(data, buffer, size);

	/* The hypervisor interface returns 64 bits */
	return size;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * pseries_rng_get_desired_dma - Return desired DMA allocate for CMO operations
 *
 * This is a required function for a driver to operate in a CMO environment
 * but this device does not make use of DMA allocations, return 0.
 *
 * Return value:
 *	Number of bytes of IO data the driver will need to perform well -> 0
 */
static unsigned long pseries_rng_get_desired_dma(struct vio_dev *vdev)
{
	return 0;
};

static struct hwrng pseries_rng = {
<<<<<<< HEAD
	.name		= MODULE_NAME,
	.data_read	= pseries_rng_data_read,
};

static int __init pseries_rng_probe(struct vio_dev *dev,
=======
	.name		= KBUILD_MODNAME,
	.read		= pseries_rng_read,
};

static int pseries_rng_probe(struct vio_dev *dev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		const struct vio_device_id *id)
{
	return hwrng_register(&pseries_rng);
}

<<<<<<< HEAD
static int __exit pseries_rng_remove(struct vio_dev *dev)
=======
static int pseries_rng_remove(struct vio_dev *dev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	hwrng_unregister(&pseries_rng);
	return 0;
}

static struct vio_device_id pseries_rng_driver_ids[] = {
	{ "ibm,random-v1", "ibm,random"},
	{ "", "" }
};
MODULE_DEVICE_TABLE(vio, pseries_rng_driver_ids);

static struct vio_driver pseries_rng_driver = {
<<<<<<< HEAD
	.name = MODULE_NAME,
=======
	.name = KBUILD_MODNAME,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.probe = pseries_rng_probe,
	.remove = pseries_rng_remove,
	.get_desired_dma = pseries_rng_get_desired_dma,
	.id_table = pseries_rng_driver_ids
};

static int __init rng_init(void)
{
<<<<<<< HEAD
	printk(KERN_INFO "Registering IBM pSeries RNG driver\n");
=======
	pr_info("Registering IBM pSeries RNG driver\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return vio_register_driver(&pseries_rng_driver);
}

module_init(rng_init);

static void __exit rng_exit(void)
{
	vio_unregister_driver(&pseries_rng_driver);
}
module_exit(rng_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michael Neuling <mikey@neuling.org>");
MODULE_DESCRIPTION("H/W RNG driver for IBM pSeries processors");
