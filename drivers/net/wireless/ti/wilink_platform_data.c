/*
 * This file is part of wl12xx
 *
 * Copyright (C) 2010-2011 Texas Instruments, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <linux/module.h>
#include <linux/err.h>
#include <linux/wl12xx.h>

<<<<<<< HEAD
static struct wl12xx_platform_data *platform_data;

int __init wl12xx_set_platform_data(const struct wl12xx_platform_data *data)
{
	if (platform_data)
=======
static struct wl1251_platform_data *wl1251_platform_data;

int __init wl1251_set_platform_data(const struct wl1251_platform_data *data)
{
	if (wl1251_platform_data)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EBUSY;
	if (!data)
		return -EINVAL;

<<<<<<< HEAD
	platform_data = kmemdup(data, sizeof(*data), GFP_KERNEL);
	if (!platform_data)
=======
	wl1251_platform_data = kmemdup(data, sizeof(*data), GFP_KERNEL);
	if (!wl1251_platform_data)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -ENOMEM;

	return 0;
}

<<<<<<< HEAD
struct wl12xx_platform_data *wl12xx_get_platform_data(void)
{
	if (!platform_data)
		return ERR_PTR(-ENODEV);

	return platform_data;
}
EXPORT_SYMBOL(wl12xx_get_platform_data);
=======
struct wl1251_platform_data *wl1251_get_platform_data(void)
{
	if (!wl1251_platform_data)
		return ERR_PTR(-ENODEV);

	return wl1251_platform_data;
}
EXPORT_SYMBOL(wl1251_get_platform_data);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
