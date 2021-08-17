/*
 * DRM/KMS device registration for TI OMAP platforms
 *
 * Copyright (C) 2012 Texas Instruments
 * Author: Rob Clark <rob.clark@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/platform_data/omap_drm.h>

#include "soc.h"
<<<<<<< HEAD
#include "omap_device.h"
#include "omap_hwmod.h"

#if defined(CONFIG_DRM_OMAP) || (CONFIG_DRM_OMAP_MODULE)
=======
#include "display.h"

#if IS_ENABLED(CONFIG_DRM_OMAP)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static struct omap_drm_platform_data platform_data;

static struct platform_device omap_drm_device = {
	.dev = {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &platform_data,
	},
	.name = "omapdrm",
	.id = 0,
};

<<<<<<< HEAD
static int __init omap_init_drm(void)
{
	struct omap_hwmod *oh = NULL;
	struct platform_device *pdev;

	/* lookup and populate the DMM information, if present - OMAP4+ */
	oh = omap_hwmod_lookup("dmm");

	if (oh) {
		pdev = omap_device_build(oh->name, -1, oh, NULL, 0);
		WARN(IS_ERR(pdev), "Could not build omap_device for %s\n",
			oh->name);
	}

=======
int __init omap_init_drm(void)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	platform_data.omaprev = GET_OMAP_TYPE;

	return platform_device_register(&omap_drm_device);

}
<<<<<<< HEAD

omap_arch_initcall(omap_init_drm);

=======
#else
int __init omap_init_drm(void) { return 0; }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
