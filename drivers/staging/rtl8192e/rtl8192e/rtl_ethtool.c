/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Based on the r8180 driver, which is:
<<<<<<< HEAD
 * Copyright 2004-2005 Andrea Merello <andreamrl@tiscali.it>, et al.
=======
 * Copyright 2004-2005 Andrea Merello <andrea.merello@gmail.com>, et al.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
<<<<<<< HEAD
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 *****************************************************************************
 */
#include <linux/netdevice.h>
#include <linux/ethtool.h>
#include <linux/delay.h>

#include "rtl_core.h"

<<<<<<< HEAD
static void rtl819x_ethtool_get_drvinfo(struct net_device *dev,
		struct ethtool_drvinfo *info)
=======
static void _rtl92e_ethtool_get_drvinfo(struct net_device *dev,
					struct ethtool_drvinfo *info)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct r8192_priv *priv = rtllib_priv(dev);

	strlcpy(info->driver, DRV_NAME, sizeof(info->driver));
	strlcpy(info->version, DRV_VERSION, sizeof(info->version));
	strlcpy(info->bus_info, pci_name(priv->pdev), sizeof(info->bus_info));
}

<<<<<<< HEAD
static u32 rtl819x_ethtool_get_link(struct net_device *dev)
=======
static u32 _rtl92e_ethtool_get_link(struct net_device *dev)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct r8192_priv *priv = rtllib_priv(dev);

	return ((priv->rtllib->state == RTLLIB_LINKED) ||
		(priv->rtllib->state == RTLLIB_LINKED_SCANNING));
}

const struct ethtool_ops rtl819x_ethtool_ops = {
<<<<<<< HEAD
	.get_drvinfo = rtl819x_ethtool_get_drvinfo,
	.get_link = rtl819x_ethtool_get_link,
=======
	.get_drvinfo = _rtl92e_ethtool_get_drvinfo,
	.get_link = _rtl92e_ethtool_get_link,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
