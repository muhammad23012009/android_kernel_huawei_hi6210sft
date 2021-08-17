/*
 * Copyright (c) 2002-3 Patrick Mochel
 * Copyright (c) 2002-3 Open Source Development Labs
 *
 * This file is released under the GPLv2
 */

#include <linux/device.h>
#include <linux/init.h>
#include <linux/memory.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "base.h"

/**
 * driver_init - initialize driver model.
 *
 * Call the driver model init functions to initialize their
 * subsystems. Called early from init/main.c.
 */
void __init driver_init(void)
{
	/* These are the core pieces */
	devtmpfs_init();
	devices_init();
	buses_init();
	classes_init();
	firmware_init();
	hypervisor_init();

	/* These are also core pieces, but must come after the
	 * core core pieces.
	 */
	platform_bus_init();
	cpu_dev_init();
	memory_dev_init();
<<<<<<< HEAD
=======
	container_dev_init();
	of_core_init();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
