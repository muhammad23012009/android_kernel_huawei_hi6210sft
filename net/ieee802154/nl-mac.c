/*
<<<<<<< HEAD
 * Netlink inteface for IEEE 802.15.4 stack
=======
 * Netlink interface for IEEE 802.15.4 stack
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Copyright 2007, 2008 Siemens AG
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
<<<<<<< HEAD
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Written by:
 * Sergey Lapin <slapin@ossfans.org>
 * Dmitry Eremin-Solenikov <dbaryshkov@gmail.com>
 * Maxim Osipov <maxim.osipov@siemens.com>
 */

#include <linux/gfp.h>
#include <linux/kernel.h>
#include <linux/if_arp.h>
#include <linux/netdevice.h>
<<<<<<< HEAD
=======
#include <linux/ieee802154.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <net/netlink.h>
#include <net/genetlink.h>
#include <net/sock.h>
#include <linux/nl802154.h>
#include <linux/export.h>
#include <net/af_ieee802154.h>
<<<<<<< HEAD
#include <net/nl802154.h>
#include <net/ieee802154.h>
#include <net/ieee802154_netdev.h>
#include <net/wpan-phy.h>

#include "ieee802154.h"

static struct genl_multicast_group ieee802154_coord_mcgrp = {
	.name		= IEEE802154_MCAST_COORD_NAME,
};

static struct genl_multicast_group ieee802154_beacon_mcgrp = {
	.name		= IEEE802154_MCAST_BEACON_NAME,
};

int ieee802154_nl_assoc_indic(struct net_device *dev,
		struct ieee802154_addr *addr, u8 cap)
{
	struct sk_buff *msg;

	pr_debug("%s\n", __func__);

	if (addr->addr_type != IEEE802154_ADDR_LONG) {
		pr_err("%s: received non-long source address!\n", __func__);
		return -EINVAL;
	}

	msg = ieee802154_nl_create(0, IEEE802154_ASSOCIATE_INDIC);
	if (!msg)
		return -ENOBUFS;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put(msg, IEEE802154_ATTR_HW_ADDR, IEEE802154_ADDR_LEN,
		    dev->dev_addr) ||
	    nla_put(msg, IEEE802154_ATTR_SRC_HW_ADDR, IEEE802154_ADDR_LEN,
		    addr->hwaddr) ||
	    nla_put_u8(msg, IEEE802154_ATTR_CAPABILITY, cap))
		goto nla_put_failure;

	return ieee802154_nl_mcast(msg, ieee802154_coord_mcgrp.id);

nla_put_failure:
	nlmsg_free(msg);
	return -ENOBUFS;
}
EXPORT_SYMBOL(ieee802154_nl_assoc_indic);

int ieee802154_nl_assoc_confirm(struct net_device *dev, u16 short_addr,
		u8 status)
{
	struct sk_buff *msg;

	pr_debug("%s\n", __func__);

	msg = ieee802154_nl_create(0, IEEE802154_ASSOCIATE_CONF);
	if (!msg)
		return -ENOBUFS;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put(msg, IEEE802154_ATTR_HW_ADDR, IEEE802154_ADDR_LEN,
		    dev->dev_addr) ||
	    nla_put_u16(msg, IEEE802154_ATTR_SHORT_ADDR, short_addr) ||
	    nla_put_u8(msg, IEEE802154_ATTR_STATUS, status))
		goto nla_put_failure;
	return ieee802154_nl_mcast(msg, ieee802154_coord_mcgrp.id);

nla_put_failure:
	nlmsg_free(msg);
	return -ENOBUFS;
}
EXPORT_SYMBOL(ieee802154_nl_assoc_confirm);

int ieee802154_nl_disassoc_indic(struct net_device *dev,
		struct ieee802154_addr *addr, u8 reason)
{
	struct sk_buff *msg;

	pr_debug("%s\n", __func__);

	msg = ieee802154_nl_create(0, IEEE802154_DISASSOCIATE_INDIC);
	if (!msg)
		return -ENOBUFS;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put(msg, IEEE802154_ATTR_HW_ADDR, IEEE802154_ADDR_LEN,
		    dev->dev_addr))
		goto nla_put_failure;
	if (addr->addr_type == IEEE802154_ADDR_LONG) {
		if (nla_put(msg, IEEE802154_ATTR_SRC_HW_ADDR, IEEE802154_ADDR_LEN,
			    addr->hwaddr))
			goto nla_put_failure;
	} else {
		if (nla_put_u16(msg, IEEE802154_ATTR_SRC_SHORT_ADDR,
				addr->short_addr))
			goto nla_put_failure;
	}
	if (nla_put_u8(msg, IEEE802154_ATTR_REASON, reason))
		goto nla_put_failure;
	return ieee802154_nl_mcast(msg, ieee802154_coord_mcgrp.id);

nla_put_failure:
	nlmsg_free(msg);
	return -ENOBUFS;
}
EXPORT_SYMBOL(ieee802154_nl_disassoc_indic);

int ieee802154_nl_disassoc_confirm(struct net_device *dev, u8 status)
{
	struct sk_buff *msg;

	pr_debug("%s\n", __func__);

	msg = ieee802154_nl_create(0, IEEE802154_DISASSOCIATE_CONF);
	if (!msg)
		return -ENOBUFS;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put(msg, IEEE802154_ATTR_HW_ADDR, IEEE802154_ADDR_LEN,
		    dev->dev_addr) ||
	    nla_put_u8(msg, IEEE802154_ATTR_STATUS, status))
		goto nla_put_failure;
	return ieee802154_nl_mcast(msg, ieee802154_coord_mcgrp.id);

nla_put_failure:
	nlmsg_free(msg);
	return -ENOBUFS;
}
EXPORT_SYMBOL(ieee802154_nl_disassoc_confirm);

int ieee802154_nl_beacon_indic(struct net_device *dev,
		u16 panid, u16 coord_addr)
{
	struct sk_buff *msg;

	pr_debug("%s\n", __func__);

	msg = ieee802154_nl_create(0, IEEE802154_BEACON_NOTIFY_INDIC);
	if (!msg)
		return -ENOBUFS;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put(msg, IEEE802154_ATTR_HW_ADDR, IEEE802154_ADDR_LEN,
		    dev->dev_addr) ||
	    nla_put_u16(msg, IEEE802154_ATTR_COORD_SHORT_ADDR, coord_addr) ||
	    nla_put_u16(msg, IEEE802154_ATTR_COORD_PAN_ID, panid))
		goto nla_put_failure;
	return ieee802154_nl_mcast(msg, ieee802154_coord_mcgrp.id);

nla_put_failure:
	nlmsg_free(msg);
	return -ENOBUFS;
}
EXPORT_SYMBOL(ieee802154_nl_beacon_indic);

int ieee802154_nl_scan_confirm(struct net_device *dev,
		u8 status, u8 scan_type, u32 unscanned, u8 page,
		u8 *edl/* , struct list_head *pan_desc_list */)
{
	struct sk_buff *msg;

	pr_debug("%s\n", __func__);

	msg = ieee802154_nl_create(0, IEEE802154_SCAN_CONF);
	if (!msg)
		return -ENOBUFS;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put(msg, IEEE802154_ATTR_HW_ADDR, IEEE802154_ADDR_LEN,
		    dev->dev_addr) ||
	    nla_put_u8(msg, IEEE802154_ATTR_STATUS, status) ||
	    nla_put_u8(msg, IEEE802154_ATTR_SCAN_TYPE, scan_type) ||
	    nla_put_u32(msg, IEEE802154_ATTR_CHANNELS, unscanned) ||
	    nla_put_u8(msg, IEEE802154_ATTR_PAGE, page) ||
	    (edl &&
	     nla_put(msg, IEEE802154_ATTR_ED_LIST, 27, edl)))
		goto nla_put_failure;
	return ieee802154_nl_mcast(msg, ieee802154_coord_mcgrp.id);

nla_put_failure:
	nlmsg_free(msg);
	return -ENOBUFS;
}
EXPORT_SYMBOL(ieee802154_nl_scan_confirm);

int ieee802154_nl_start_confirm(struct net_device *dev, u8 status)
=======
#include <net/ieee802154_netdev.h>
#include <net/cfg802154.h>

#include "ieee802154.h"

static int nla_put_hwaddr(struct sk_buff *msg, int type, __le64 hwaddr,
			  int padattr)
{
	return nla_put_u64_64bit(msg, type, swab64((__force u64)hwaddr),
				 padattr);
}

static __le64 nla_get_hwaddr(const struct nlattr *nla)
{
	return ieee802154_devaddr_from_raw(nla_data(nla));
}

static int nla_put_shortaddr(struct sk_buff *msg, int type, __le16 addr)
{
	return nla_put_u16(msg, type, le16_to_cpu(addr));
}

static __le16 nla_get_shortaddr(const struct nlattr *nla)
{
	return cpu_to_le16(nla_get_u16(nla));
}

static int ieee802154_nl_start_confirm(struct net_device *dev, u8 status)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct sk_buff *msg;

	pr_debug("%s\n", __func__);

	msg = ieee802154_nl_create(0, IEEE802154_START_CONF);
	if (!msg)
		return -ENOBUFS;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put(msg, IEEE802154_ATTR_HW_ADDR, IEEE802154_ADDR_LEN,
		    dev->dev_addr) ||
	    nla_put_u8(msg, IEEE802154_ATTR_STATUS, status))
		goto nla_put_failure;
<<<<<<< HEAD
	return ieee802154_nl_mcast(msg, ieee802154_coord_mcgrp.id);
=======
	return ieee802154_nl_mcast(msg, IEEE802154_COORD_MCGRP);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

nla_put_failure:
	nlmsg_free(msg);
	return -ENOBUFS;
}
<<<<<<< HEAD
EXPORT_SYMBOL(ieee802154_nl_start_confirm);

static int ieee802154_nl_fill_iface(struct sk_buff *msg, u32 portid,
	u32 seq, int flags, struct net_device *dev)
{
	void *hdr;
	struct wpan_phy *phy;
=======

static int ieee802154_nl_fill_iface(struct sk_buff *msg, u32 portid,
				    u32 seq, int flags, struct net_device *dev)
{
	void *hdr;
	struct wpan_phy *phy;
	struct ieee802154_mlme_ops *ops;
	__le16 short_addr, pan_id;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	pr_debug("%s\n", __func__);

	hdr = genlmsg_put(msg, 0, seq, &nl802154_family, flags,
<<<<<<< HEAD
		IEEE802154_LIST_IFACE);
	if (!hdr)
		goto out;

	phy = ieee802154_mlme_ops(dev)->get_phy(dev);
	BUG_ON(!phy);
=======
			  IEEE802154_LIST_IFACE);
	if (!hdr)
		goto out;

	ops = ieee802154_mlme_ops(dev);
	phy = dev->ieee802154_ptr->wpan_phy;
	BUG_ON(!phy);
	get_device(&phy->dev);

	rtnl_lock();
	short_addr = dev->ieee802154_ptr->short_addr;
	pan_id = dev->ieee802154_ptr->pan_id;
	rtnl_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_string(msg, IEEE802154_ATTR_PHY_NAME, wpan_phy_name(phy)) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put(msg, IEEE802154_ATTR_HW_ADDR, IEEE802154_ADDR_LEN,
		    dev->dev_addr) ||
<<<<<<< HEAD
	    nla_put_u16(msg, IEEE802154_ATTR_SHORT_ADDR,
			ieee802154_mlme_ops(dev)->get_short_addr(dev)) ||
	    nla_put_u16(msg, IEEE802154_ATTR_PAN_ID,
			ieee802154_mlme_ops(dev)->get_pan_id(dev)))
		goto nla_put_failure;
	wpan_phy_put(phy);
	return genlmsg_end(msg, hdr);
=======
	    nla_put_shortaddr(msg, IEEE802154_ATTR_SHORT_ADDR, short_addr) ||
	    nla_put_shortaddr(msg, IEEE802154_ATTR_PAN_ID, pan_id))
		goto nla_put_failure;

	if (ops->get_mac_params) {
		struct ieee802154_mac_params params;

		rtnl_lock();
		ops->get_mac_params(dev, &params);
		rtnl_unlock();

		if (nla_put_s8(msg, IEEE802154_ATTR_TXPOWER,
			       params.transmit_power / 100) ||
		    nla_put_u8(msg, IEEE802154_ATTR_LBT_ENABLED, params.lbt) ||
		    nla_put_u8(msg, IEEE802154_ATTR_CCA_MODE,
			       params.cca.mode) ||
		    nla_put_s32(msg, IEEE802154_ATTR_CCA_ED_LEVEL,
				params.cca_ed_level / 100) ||
		    nla_put_u8(msg, IEEE802154_ATTR_CSMA_RETRIES,
			       params.csma_retries) ||
		    nla_put_u8(msg, IEEE802154_ATTR_CSMA_MIN_BE,
			       params.min_be) ||
		    nla_put_u8(msg, IEEE802154_ATTR_CSMA_MAX_BE,
			       params.max_be) ||
		    nla_put_s8(msg, IEEE802154_ATTR_FRAME_RETRIES,
			       params.frame_retries))
			goto nla_put_failure;
	}

	wpan_phy_put(phy);
	genlmsg_end(msg, hdr);
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

nla_put_failure:
	wpan_phy_put(phy);
	genlmsg_cancel(msg, hdr);
out:
	return -EMSGSIZE;
}

/* Requests from userspace */
static struct net_device *ieee802154_nl_get_dev(struct genl_info *info)
{
	struct net_device *dev;

	if (info->attrs[IEEE802154_ATTR_DEV_NAME]) {
		char name[IFNAMSIZ + 1];
<<<<<<< HEAD
		nla_strlcpy(name, info->attrs[IEEE802154_ATTR_DEV_NAME],
				sizeof(name));
		dev = dev_get_by_name(&init_net, name);
	} else if (info->attrs[IEEE802154_ATTR_DEV_INDEX])
		dev = dev_get_by_index(&init_net,
			nla_get_u32(info->attrs[IEEE802154_ATTR_DEV_INDEX]));
	else
		return NULL;
=======

		nla_strlcpy(name, info->attrs[IEEE802154_ATTR_DEV_NAME],
			    sizeof(name));
		dev = dev_get_by_name(&init_net, name);
	} else if (info->attrs[IEEE802154_ATTR_DEV_INDEX]) {
		dev = dev_get_by_index(&init_net,
			nla_get_u32(info->attrs[IEEE802154_ATTR_DEV_INDEX]));
	} else {
		return NULL;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!dev)
		return NULL;

	if (dev->type != ARPHRD_IEEE802154) {
		dev_put(dev);
		return NULL;
	}

	return dev;
}

<<<<<<< HEAD
static int ieee802154_associate_req(struct sk_buff *skb,
		struct genl_info *info)
=======
int ieee802154_associate_req(struct sk_buff *skb, struct genl_info *info)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct net_device *dev;
	struct ieee802154_addr addr;
	u8 page;
	int ret = -EOPNOTSUPP;

	if (!info->attrs[IEEE802154_ATTR_CHANNEL] ||
	    !info->attrs[IEEE802154_ATTR_COORD_PAN_ID] ||
	    (!info->attrs[IEEE802154_ATTR_COORD_HW_ADDR] &&
		!info->attrs[IEEE802154_ATTR_COORD_SHORT_ADDR]) ||
	    !info->attrs[IEEE802154_ATTR_CAPABILITY])
		return -EINVAL;

	dev = ieee802154_nl_get_dev(info);
	if (!dev)
		return -ENODEV;
	if (!ieee802154_mlme_ops(dev)->assoc_req)
		goto out;

	if (info->attrs[IEEE802154_ATTR_COORD_HW_ADDR]) {
<<<<<<< HEAD
		addr.addr_type = IEEE802154_ADDR_LONG;
		nla_memcpy(addr.hwaddr,
				info->attrs[IEEE802154_ATTR_COORD_HW_ADDR],
				IEEE802154_ADDR_LEN);
	} else {
		addr.addr_type = IEEE802154_ADDR_SHORT;
		addr.short_addr = nla_get_u16(
				info->attrs[IEEE802154_ATTR_COORD_SHORT_ADDR]);
	}
	addr.pan_id = nla_get_u16(info->attrs[IEEE802154_ATTR_COORD_PAN_ID]);
=======
		addr.mode = IEEE802154_ADDR_LONG;
		addr.extended_addr = nla_get_hwaddr(
				info->attrs[IEEE802154_ATTR_COORD_HW_ADDR]);
	} else {
		addr.mode = IEEE802154_ADDR_SHORT;
		addr.short_addr = nla_get_shortaddr(
				info->attrs[IEEE802154_ATTR_COORD_SHORT_ADDR]);
	}
	addr.pan_id = nla_get_shortaddr(
			info->attrs[IEEE802154_ATTR_COORD_PAN_ID]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (info->attrs[IEEE802154_ATTR_PAGE])
		page = nla_get_u8(info->attrs[IEEE802154_ATTR_PAGE]);
	else
		page = 0;

	ret = ieee802154_mlme_ops(dev)->assoc_req(dev, &addr,
			nla_get_u8(info->attrs[IEEE802154_ATTR_CHANNEL]),
			page,
			nla_get_u8(info->attrs[IEEE802154_ATTR_CAPABILITY]));

out:
	dev_put(dev);
	return ret;
}

<<<<<<< HEAD
static int ieee802154_associate_resp(struct sk_buff *skb,
		struct genl_info *info)
=======
int ieee802154_associate_resp(struct sk_buff *skb, struct genl_info *info)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct net_device *dev;
	struct ieee802154_addr addr;
	int ret = -EOPNOTSUPP;

	if (!info->attrs[IEEE802154_ATTR_STATUS] ||
	    !info->attrs[IEEE802154_ATTR_DEST_HW_ADDR] ||
	    !info->attrs[IEEE802154_ATTR_DEST_SHORT_ADDR])
		return -EINVAL;

	dev = ieee802154_nl_get_dev(info);
	if (!dev)
		return -ENODEV;
	if (!ieee802154_mlme_ops(dev)->assoc_resp)
		goto out;

<<<<<<< HEAD
	addr.addr_type = IEEE802154_ADDR_LONG;
	nla_memcpy(addr.hwaddr, info->attrs[IEEE802154_ATTR_DEST_HW_ADDR],
			IEEE802154_ADDR_LEN);
	addr.pan_id = ieee802154_mlme_ops(dev)->get_pan_id(dev);


	ret = ieee802154_mlme_ops(dev)->assoc_resp(dev, &addr,
		nla_get_u16(info->attrs[IEEE802154_ATTR_DEST_SHORT_ADDR]),
=======
	addr.mode = IEEE802154_ADDR_LONG;
	addr.extended_addr = nla_get_hwaddr(
			info->attrs[IEEE802154_ATTR_DEST_HW_ADDR]);
	rtnl_lock();
	addr.pan_id = dev->ieee802154_ptr->pan_id;
	rtnl_unlock();

	ret = ieee802154_mlme_ops(dev)->assoc_resp(dev, &addr,
		nla_get_shortaddr(info->attrs[IEEE802154_ATTR_DEST_SHORT_ADDR]),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		nla_get_u8(info->attrs[IEEE802154_ATTR_STATUS]));

out:
	dev_put(dev);
	return ret;
}

<<<<<<< HEAD
static int ieee802154_disassociate_req(struct sk_buff *skb,
		struct genl_info *info)
=======
int ieee802154_disassociate_req(struct sk_buff *skb, struct genl_info *info)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct net_device *dev;
	struct ieee802154_addr addr;
	int ret = -EOPNOTSUPP;

	if ((!info->attrs[IEEE802154_ATTR_DEST_HW_ADDR] &&
<<<<<<< HEAD
		!info->attrs[IEEE802154_ATTR_DEST_SHORT_ADDR]) ||
=======
	    !info->attrs[IEEE802154_ATTR_DEST_SHORT_ADDR]) ||
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	    !info->attrs[IEEE802154_ATTR_REASON])
		return -EINVAL;

	dev = ieee802154_nl_get_dev(info);
	if (!dev)
		return -ENODEV;
	if (!ieee802154_mlme_ops(dev)->disassoc_req)
		goto out;

	if (info->attrs[IEEE802154_ATTR_DEST_HW_ADDR]) {
<<<<<<< HEAD
		addr.addr_type = IEEE802154_ADDR_LONG;
		nla_memcpy(addr.hwaddr,
				info->attrs[IEEE802154_ATTR_DEST_HW_ADDR],
				IEEE802154_ADDR_LEN);
	} else {
		addr.addr_type = IEEE802154_ADDR_SHORT;
		addr.short_addr = nla_get_u16(
				info->attrs[IEEE802154_ATTR_DEST_SHORT_ADDR]);
	}
	addr.pan_id = ieee802154_mlme_ops(dev)->get_pan_id(dev);
=======
		addr.mode = IEEE802154_ADDR_LONG;
		addr.extended_addr = nla_get_hwaddr(
				info->attrs[IEEE802154_ATTR_DEST_HW_ADDR]);
	} else {
		addr.mode = IEEE802154_ADDR_SHORT;
		addr.short_addr = nla_get_shortaddr(
				info->attrs[IEEE802154_ATTR_DEST_SHORT_ADDR]);
	}
	rtnl_lock();
	addr.pan_id = dev->ieee802154_ptr->pan_id;
	rtnl_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = ieee802154_mlme_ops(dev)->disassoc_req(dev, &addr,
			nla_get_u8(info->attrs[IEEE802154_ATTR_REASON]));

out:
	dev_put(dev);
	return ret;
}

<<<<<<< HEAD
/*
 * PANid, channel, beacon_order = 15, superframe_order = 15,
 * PAN_coordinator, battery_life_extension = 0,
 * coord_realignment = 0, security_enable = 0
*/
static int ieee802154_start_req(struct sk_buff *skb, struct genl_info *info)
=======
/* PANid, channel, beacon_order = 15, superframe_order = 15,
 * PAN_coordinator, battery_life_extension = 0,
 * coord_realignment = 0, security_enable = 0
*/
int ieee802154_start_req(struct sk_buff *skb, struct genl_info *info)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct net_device *dev;
	struct ieee802154_addr addr;

	u8 channel, bcn_ord, sf_ord;
	u8 page;
	int pan_coord, blx, coord_realign;
<<<<<<< HEAD
	int ret = -EOPNOTSUPP;
=======
	int ret = -EBUSY;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!info->attrs[IEEE802154_ATTR_COORD_PAN_ID] ||
	    !info->attrs[IEEE802154_ATTR_COORD_SHORT_ADDR] ||
	    !info->attrs[IEEE802154_ATTR_CHANNEL] ||
	    !info->attrs[IEEE802154_ATTR_BCN_ORD] ||
	    !info->attrs[IEEE802154_ATTR_SF_ORD] ||
	    !info->attrs[IEEE802154_ATTR_PAN_COORD] ||
	    !info->attrs[IEEE802154_ATTR_BAT_EXT] ||
	    !info->attrs[IEEE802154_ATTR_COORD_REALIGN]
	 )
		return -EINVAL;

	dev = ieee802154_nl_get_dev(info);
	if (!dev)
		return -ENODEV;
<<<<<<< HEAD
	if (!ieee802154_mlme_ops(dev)->start_req)
		goto out;

	addr.addr_type = IEEE802154_ADDR_SHORT;
	addr.short_addr = nla_get_u16(
			info->attrs[IEEE802154_ATTR_COORD_SHORT_ADDR]);
	addr.pan_id = nla_get_u16(info->attrs[IEEE802154_ATTR_COORD_PAN_ID]);
=======

	if (netif_running(dev))
		goto out;

	if (!ieee802154_mlme_ops(dev)->start_req) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	addr.mode = IEEE802154_ADDR_SHORT;
	addr.short_addr = nla_get_shortaddr(
			info->attrs[IEEE802154_ATTR_COORD_SHORT_ADDR]);
	addr.pan_id = nla_get_shortaddr(
			info->attrs[IEEE802154_ATTR_COORD_PAN_ID]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	channel = nla_get_u8(info->attrs[IEEE802154_ATTR_CHANNEL]);
	bcn_ord = nla_get_u8(info->attrs[IEEE802154_ATTR_BCN_ORD]);
	sf_ord = nla_get_u8(info->attrs[IEEE802154_ATTR_SF_ORD]);
	pan_coord = nla_get_u8(info->attrs[IEEE802154_ATTR_PAN_COORD]);
	blx = nla_get_u8(info->attrs[IEEE802154_ATTR_BAT_EXT]);
	coord_realign = nla_get_u8(info->attrs[IEEE802154_ATTR_COORD_REALIGN]);

	if (info->attrs[IEEE802154_ATTR_PAGE])
		page = nla_get_u8(info->attrs[IEEE802154_ATTR_PAGE]);
	else
		page = 0;

<<<<<<< HEAD

	if (addr.short_addr == IEEE802154_ADDR_BROADCAST) {
=======
	if (addr.short_addr == cpu_to_le16(IEEE802154_ADDR_BROADCAST)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ieee802154_nl_start_confirm(dev, IEEE802154_NO_SHORT_ADDRESS);
		dev_put(dev);
		return -EINVAL;
	}

<<<<<<< HEAD
	ret = ieee802154_mlme_ops(dev)->start_req(dev, &addr, channel, page,
		bcn_ord, sf_ord, pan_coord, blx, coord_realign);
=======
	rtnl_lock();
	ret = ieee802154_mlme_ops(dev)->start_req(dev, &addr, channel, page,
		bcn_ord, sf_ord, pan_coord, blx, coord_realign);
	rtnl_unlock();

	/* FIXME: add validation for unused parameters to be sane
	 * for SoftMAC
	 */
	ieee802154_nl_start_confirm(dev, IEEE802154_SUCCESS);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

out:
	dev_put(dev);
	return ret;
}

<<<<<<< HEAD
static int ieee802154_scan_req(struct sk_buff *skb, struct genl_info *info)
=======
int ieee802154_scan_req(struct sk_buff *skb, struct genl_info *info)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct net_device *dev;
	int ret = -EOPNOTSUPP;
	u8 type;
	u32 channels;
	u8 duration;
	u8 page;

	if (!info->attrs[IEEE802154_ATTR_SCAN_TYPE] ||
	    !info->attrs[IEEE802154_ATTR_CHANNELS] ||
	    !info->attrs[IEEE802154_ATTR_DURATION])
		return -EINVAL;

	dev = ieee802154_nl_get_dev(info);
	if (!dev)
		return -ENODEV;
	if (!ieee802154_mlme_ops(dev)->scan_req)
		goto out;

	type = nla_get_u8(info->attrs[IEEE802154_ATTR_SCAN_TYPE]);
	channels = nla_get_u32(info->attrs[IEEE802154_ATTR_CHANNELS]);
	duration = nla_get_u8(info->attrs[IEEE802154_ATTR_DURATION]);

	if (info->attrs[IEEE802154_ATTR_PAGE])
		page = nla_get_u8(info->attrs[IEEE802154_ATTR_PAGE]);
	else
		page = 0;

<<<<<<< HEAD

	ret = ieee802154_mlme_ops(dev)->scan_req(dev, type, channels, page,
			duration);
=======
	ret = ieee802154_mlme_ops(dev)->scan_req(dev, type, channels,
						 page, duration);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

out:
	dev_put(dev);
	return ret;
}

<<<<<<< HEAD
static int ieee802154_list_iface(struct sk_buff *skb,
	struct genl_info *info)
{
	/* Request for interface name, index, type, IEEE address,
	   PAN Id, short address */
=======
int ieee802154_list_iface(struct sk_buff *skb, struct genl_info *info)
{
	/* Request for interface name, index, type, IEEE address,
	 * PAN Id, short address
	 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct sk_buff *msg;
	struct net_device *dev = NULL;
	int rc = -ENOBUFS;

	pr_debug("%s\n", __func__);

	dev = ieee802154_nl_get_dev(info);
	if (!dev)
		return -ENODEV;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		goto out_dev;

	rc = ieee802154_nl_fill_iface(msg, info->snd_portid, info->snd_seq,
<<<<<<< HEAD
			0, dev);
=======
				      0, dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (rc < 0)
		goto out_free;

	dev_put(dev);

	return genlmsg_reply(msg, info);
out_free:
	nlmsg_free(msg);
out_dev:
	dev_put(dev);
	return rc;
<<<<<<< HEAD

}

static int ieee802154_dump_iface(struct sk_buff *skb,
	struct netlink_callback *cb)
=======
}

int ieee802154_dump_iface(struct sk_buff *skb, struct netlink_callback *cb)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct net *net = sock_net(skb->sk);
	struct net_device *dev;
	int idx;
	int s_idx = cb->args[0];

	pr_debug("%s\n", __func__);

	idx = 0;
	for_each_netdev(net, dev) {
<<<<<<< HEAD
		if (idx < s_idx || (dev->type != ARPHRD_IEEE802154))
			goto cont;

		if (ieee802154_nl_fill_iface(skb, NETLINK_CB(cb->skb).portid,
			cb->nlh->nlmsg_seq, NLM_F_MULTI, dev) < 0)
=======
		if (idx < s_idx || dev->type != ARPHRD_IEEE802154)
			goto cont;

		if (ieee802154_nl_fill_iface(skb, NETLINK_CB(cb->skb).portid,
					     cb->nlh->nlmsg_seq,
					     NLM_F_MULTI, dev) < 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
cont:
		idx++;
	}
	cb->args[0] = idx;

	return skb->len;
}

<<<<<<< HEAD
static struct genl_ops ieee802154_coordinator_ops[] = {
	IEEE802154_OP(IEEE802154_ASSOCIATE_REQ, ieee802154_associate_req),
	IEEE802154_OP(IEEE802154_ASSOCIATE_RESP, ieee802154_associate_resp),
	IEEE802154_OP(IEEE802154_DISASSOCIATE_REQ, ieee802154_disassociate_req),
	IEEE802154_OP(IEEE802154_SCAN_REQ, ieee802154_scan_req),
	IEEE802154_OP(IEEE802154_START_REQ, ieee802154_start_req),
	IEEE802154_DUMP(IEEE802154_LIST_IFACE, ieee802154_list_iface,
							ieee802154_dump_iface),
};

/*
 * No need to unregister as family unregistration will do it.
 */
int nl802154_mac_register(void)
{
	int i;
	int rc;

	rc = genl_register_mc_group(&nl802154_family,
			&ieee802154_coord_mcgrp);
	if (rc)
		return rc;

	rc = genl_register_mc_group(&nl802154_family,
			&ieee802154_beacon_mcgrp);
	if (rc)
		return rc;

	for (i = 0; i < ARRAY_SIZE(ieee802154_coordinator_ops); i++) {
		rc = genl_register_ops(&nl802154_family,
				&ieee802154_coordinator_ops[i]);
		if (rc)
			return rc;
	}

	return 0;
}
=======
int ieee802154_set_macparams(struct sk_buff *skb, struct genl_info *info)
{
	struct net_device *dev = NULL;
	struct ieee802154_mlme_ops *ops;
	struct ieee802154_mac_params params;
	struct wpan_phy *phy;
	int rc = -EINVAL;

	pr_debug("%s\n", __func__);

	dev = ieee802154_nl_get_dev(info);
	if (!dev)
		return -ENODEV;

	ops = ieee802154_mlme_ops(dev);

	if (!ops->get_mac_params || !ops->set_mac_params) {
		rc = -EOPNOTSUPP;
		goto out;
	}

	if (netif_running(dev)) {
		rc = -EBUSY;
		goto out;
	}

	if (!info->attrs[IEEE802154_ATTR_LBT_ENABLED] &&
	    !info->attrs[IEEE802154_ATTR_CCA_MODE] &&
	    !info->attrs[IEEE802154_ATTR_CCA_ED_LEVEL] &&
	    !info->attrs[IEEE802154_ATTR_CSMA_RETRIES] &&
	    !info->attrs[IEEE802154_ATTR_CSMA_MIN_BE] &&
	    !info->attrs[IEEE802154_ATTR_CSMA_MAX_BE] &&
	    !info->attrs[IEEE802154_ATTR_FRAME_RETRIES])
		goto out;

	phy = dev->ieee802154_ptr->wpan_phy;
	get_device(&phy->dev);

	rtnl_lock();
	ops->get_mac_params(dev, &params);

	if (info->attrs[IEEE802154_ATTR_TXPOWER])
		params.transmit_power = nla_get_s8(info->attrs[IEEE802154_ATTR_TXPOWER]) * 100;

	if (info->attrs[IEEE802154_ATTR_LBT_ENABLED])
		params.lbt = nla_get_u8(info->attrs[IEEE802154_ATTR_LBT_ENABLED]);

	if (info->attrs[IEEE802154_ATTR_CCA_MODE])
		params.cca.mode = nla_get_u8(info->attrs[IEEE802154_ATTR_CCA_MODE]);

	if (info->attrs[IEEE802154_ATTR_CCA_ED_LEVEL])
		params.cca_ed_level = nla_get_s32(info->attrs[IEEE802154_ATTR_CCA_ED_LEVEL]) * 100;

	if (info->attrs[IEEE802154_ATTR_CSMA_RETRIES])
		params.csma_retries = nla_get_u8(info->attrs[IEEE802154_ATTR_CSMA_RETRIES]);

	if (info->attrs[IEEE802154_ATTR_CSMA_MIN_BE])
		params.min_be = nla_get_u8(info->attrs[IEEE802154_ATTR_CSMA_MIN_BE]);

	if (info->attrs[IEEE802154_ATTR_CSMA_MAX_BE])
		params.max_be = nla_get_u8(info->attrs[IEEE802154_ATTR_CSMA_MAX_BE]);

	if (info->attrs[IEEE802154_ATTR_FRAME_RETRIES])
		params.frame_retries = nla_get_s8(info->attrs[IEEE802154_ATTR_FRAME_RETRIES]);

	rc = ops->set_mac_params(dev, &params);
	rtnl_unlock();

	wpan_phy_put(phy);
	dev_put(dev);

	return 0;

out:
	dev_put(dev);
	return rc;
}

static int
ieee802154_llsec_parse_key_id(struct genl_info *info,
			      struct ieee802154_llsec_key_id *desc)
{
	memset(desc, 0, sizeof(*desc));

	if (!info->attrs[IEEE802154_ATTR_LLSEC_KEY_MODE])
		return -EINVAL;

	desc->mode = nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_KEY_MODE]);

	if (desc->mode == IEEE802154_SCF_KEY_IMPLICIT) {
		if (!info->attrs[IEEE802154_ATTR_PAN_ID])
			return -EINVAL;

		desc->device_addr.pan_id = nla_get_shortaddr(info->attrs[IEEE802154_ATTR_PAN_ID]);

		if (info->attrs[IEEE802154_ATTR_SHORT_ADDR]) {
			desc->device_addr.mode = IEEE802154_ADDR_SHORT;
			desc->device_addr.short_addr = nla_get_shortaddr(info->attrs[IEEE802154_ATTR_SHORT_ADDR]);
		} else {
			if (!info->attrs[IEEE802154_ATTR_HW_ADDR])
				return -EINVAL;

			desc->device_addr.mode = IEEE802154_ADDR_LONG;
			desc->device_addr.extended_addr = nla_get_hwaddr(info->attrs[IEEE802154_ATTR_HW_ADDR]);
		}
	}

	if (desc->mode != IEEE802154_SCF_KEY_IMPLICIT &&
	    !info->attrs[IEEE802154_ATTR_LLSEC_KEY_ID])
		return -EINVAL;

	if (desc->mode == IEEE802154_SCF_KEY_SHORT_INDEX &&
	    !info->attrs[IEEE802154_ATTR_LLSEC_KEY_SOURCE_SHORT])
		return -EINVAL;

	if (desc->mode == IEEE802154_SCF_KEY_HW_INDEX &&
	    !info->attrs[IEEE802154_ATTR_LLSEC_KEY_SOURCE_EXTENDED])
		return -EINVAL;

	if (desc->mode != IEEE802154_SCF_KEY_IMPLICIT)
		desc->id = nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_KEY_ID]);

	switch (desc->mode) {
	case IEEE802154_SCF_KEY_SHORT_INDEX:
	{
		u32 source = nla_get_u32(info->attrs[IEEE802154_ATTR_LLSEC_KEY_SOURCE_SHORT]);

		desc->short_source = cpu_to_le32(source);
		break;
	}
	case IEEE802154_SCF_KEY_HW_INDEX:
		desc->extended_source = nla_get_hwaddr(info->attrs[IEEE802154_ATTR_LLSEC_KEY_SOURCE_EXTENDED]);
		break;
	}

	return 0;
}

static int
ieee802154_llsec_fill_key_id(struct sk_buff *msg,
			     const struct ieee802154_llsec_key_id *desc)
{
	if (nla_put_u8(msg, IEEE802154_ATTR_LLSEC_KEY_MODE, desc->mode))
		return -EMSGSIZE;

	if (desc->mode == IEEE802154_SCF_KEY_IMPLICIT) {
		if (nla_put_shortaddr(msg, IEEE802154_ATTR_PAN_ID,
				      desc->device_addr.pan_id))
			return -EMSGSIZE;

		if (desc->device_addr.mode == IEEE802154_ADDR_SHORT &&
		    nla_put_shortaddr(msg, IEEE802154_ATTR_SHORT_ADDR,
				      desc->device_addr.short_addr))
			return -EMSGSIZE;

		if (desc->device_addr.mode == IEEE802154_ADDR_LONG &&
		    nla_put_hwaddr(msg, IEEE802154_ATTR_HW_ADDR,
				   desc->device_addr.extended_addr,
				   IEEE802154_ATTR_PAD))
			return -EMSGSIZE;
	}

	if (desc->mode != IEEE802154_SCF_KEY_IMPLICIT &&
	    nla_put_u8(msg, IEEE802154_ATTR_LLSEC_KEY_ID, desc->id))
		return -EMSGSIZE;

	if (desc->mode == IEEE802154_SCF_KEY_SHORT_INDEX &&
	    nla_put_u32(msg, IEEE802154_ATTR_LLSEC_KEY_SOURCE_SHORT,
			le32_to_cpu(desc->short_source)))
		return -EMSGSIZE;

	if (desc->mode == IEEE802154_SCF_KEY_HW_INDEX &&
	    nla_put_hwaddr(msg, IEEE802154_ATTR_LLSEC_KEY_SOURCE_EXTENDED,
			   desc->extended_source, IEEE802154_ATTR_PAD))
		return -EMSGSIZE;

	return 0;
}

int ieee802154_llsec_getparams(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *msg;
	struct net_device *dev = NULL;
	int rc = -ENOBUFS;
	struct ieee802154_mlme_ops *ops;
	void *hdr;
	struct ieee802154_llsec_params params;

	pr_debug("%s\n", __func__);

	dev = ieee802154_nl_get_dev(info);
	if (!dev)
		return -ENODEV;

	ops = ieee802154_mlme_ops(dev);
	if (!ops->llsec) {
		rc = -EOPNOTSUPP;
		goto out_dev;
	}

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		goto out_dev;

	hdr = genlmsg_put(msg, 0, info->snd_seq, &nl802154_family, 0,
			  IEEE802154_LLSEC_GETPARAMS);
	if (!hdr)
		goto out_free;

	rc = ops->llsec->get_params(dev, &params);
	if (rc < 0)
		goto out_free;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put_u8(msg, IEEE802154_ATTR_LLSEC_ENABLED, params.enabled) ||
	    nla_put_u8(msg, IEEE802154_ATTR_LLSEC_SECLEVEL, params.out_level) ||
	    nla_put_u32(msg, IEEE802154_ATTR_LLSEC_FRAME_COUNTER,
			be32_to_cpu(params.frame_counter)) ||
	    ieee802154_llsec_fill_key_id(msg, &params.out_key)) {
		rc = -ENOBUFS;
		goto out_free;
	}

	dev_put(dev);

	return ieee802154_nl_reply(msg, info);
out_free:
	nlmsg_free(msg);
out_dev:
	dev_put(dev);
	return rc;
}

int ieee802154_llsec_setparams(struct sk_buff *skb, struct genl_info *info)
{
	struct net_device *dev = NULL;
	int rc = -EINVAL;
	struct ieee802154_mlme_ops *ops;
	struct ieee802154_llsec_params params;
	int changed = 0;

	pr_debug("%s\n", __func__);

	dev = ieee802154_nl_get_dev(info);
	if (!dev)
		return -ENODEV;

	if (!info->attrs[IEEE802154_ATTR_LLSEC_ENABLED] &&
	    !info->attrs[IEEE802154_ATTR_LLSEC_KEY_MODE] &&
	    !info->attrs[IEEE802154_ATTR_LLSEC_SECLEVEL])
		goto out;

	ops = ieee802154_mlme_ops(dev);
	if (!ops->llsec) {
		rc = -EOPNOTSUPP;
		goto out;
	}

	if (info->attrs[IEEE802154_ATTR_LLSEC_SECLEVEL] &&
	    nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_SECLEVEL]) > 7)
		goto out;

	if (info->attrs[IEEE802154_ATTR_LLSEC_ENABLED]) {
		params.enabled = nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_ENABLED]);
		changed |= IEEE802154_LLSEC_PARAM_ENABLED;
	}

	if (info->attrs[IEEE802154_ATTR_LLSEC_KEY_MODE]) {
		if (ieee802154_llsec_parse_key_id(info, &params.out_key))
			goto out;

		changed |= IEEE802154_LLSEC_PARAM_OUT_KEY;
	}

	if (info->attrs[IEEE802154_ATTR_LLSEC_SECLEVEL]) {
		params.out_level = nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_SECLEVEL]);
		changed |= IEEE802154_LLSEC_PARAM_OUT_LEVEL;
	}

	if (info->attrs[IEEE802154_ATTR_LLSEC_FRAME_COUNTER]) {
		u32 fc = nla_get_u32(info->attrs[IEEE802154_ATTR_LLSEC_FRAME_COUNTER]);

		params.frame_counter = cpu_to_be32(fc);
		changed |= IEEE802154_LLSEC_PARAM_FRAME_COUNTER;
	}

	rc = ops->llsec->set_params(dev, &params, changed);

	dev_put(dev);

	return rc;
out:
	dev_put(dev);
	return rc;
}

struct llsec_dump_data {
	struct sk_buff *skb;
	int s_idx, s_idx2;
	int portid;
	int nlmsg_seq;
	struct net_device *dev;
	struct ieee802154_mlme_ops *ops;
	struct ieee802154_llsec_table *table;
};

static int
ieee802154_llsec_dump_table(struct sk_buff *skb, struct netlink_callback *cb,
			    int (*step)(struct llsec_dump_data *))
{
	struct net *net = sock_net(skb->sk);
	struct net_device *dev;
	struct llsec_dump_data data;
	int idx = 0;
	int first_dev = cb->args[0];
	int rc;

	for_each_netdev(net, dev) {
		if (idx < first_dev || dev->type != ARPHRD_IEEE802154)
			goto skip;

		data.ops = ieee802154_mlme_ops(dev);
		if (!data.ops->llsec)
			goto skip;

		data.skb = skb;
		data.s_idx = cb->args[1];
		data.s_idx2 = cb->args[2];
		data.dev = dev;
		data.portid = NETLINK_CB(cb->skb).portid;
		data.nlmsg_seq = cb->nlh->nlmsg_seq;

		data.ops->llsec->lock_table(dev);
		data.ops->llsec->get_table(data.dev, &data.table);
		rc = step(&data);
		data.ops->llsec->unlock_table(dev);

		if (rc < 0)
			break;

skip:
		idx++;
	}
	cb->args[0] = idx;

	return skb->len;
}

static int
ieee802154_nl_llsec_change(struct sk_buff *skb, struct genl_info *info,
			   int (*fn)(struct net_device*, struct genl_info*))
{
	struct net_device *dev = NULL;
	int rc = -EINVAL;

	dev = ieee802154_nl_get_dev(info);
	if (!dev)
		return -ENODEV;

	if (!ieee802154_mlme_ops(dev)->llsec)
		rc = -EOPNOTSUPP;
	else
		rc = fn(dev, info);

	dev_put(dev);
	return rc;
}

static int
ieee802154_llsec_parse_key(struct genl_info *info,
			   struct ieee802154_llsec_key *key)
{
	u8 frames;
	u32 commands[256 / 32];

	memset(key, 0, sizeof(*key));

	if (!info->attrs[IEEE802154_ATTR_LLSEC_KEY_USAGE_FRAME_TYPES] ||
	    !info->attrs[IEEE802154_ATTR_LLSEC_KEY_BYTES])
		return -EINVAL;

	frames = nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_KEY_USAGE_FRAME_TYPES]);
	if ((frames & BIT(IEEE802154_FC_TYPE_MAC_CMD)) &&
	    !info->attrs[IEEE802154_ATTR_LLSEC_KEY_USAGE_COMMANDS])
		return -EINVAL;

	if (info->attrs[IEEE802154_ATTR_LLSEC_KEY_USAGE_COMMANDS]) {
		nla_memcpy(commands,
			   info->attrs[IEEE802154_ATTR_LLSEC_KEY_USAGE_COMMANDS],
			   256 / 8);

		if (commands[0] || commands[1] || commands[2] || commands[3] ||
		    commands[4] || commands[5] || commands[6] ||
		    commands[7] >= BIT(IEEE802154_CMD_GTS_REQ + 1))
			return -EINVAL;

		key->cmd_frame_ids = commands[7];
	}

	key->frame_types = frames;

	nla_memcpy(key->key, info->attrs[IEEE802154_ATTR_LLSEC_KEY_BYTES],
		   IEEE802154_LLSEC_KEY_SIZE);

	return 0;
}

static int llsec_add_key(struct net_device *dev, struct genl_info *info)
{
	struct ieee802154_mlme_ops *ops = ieee802154_mlme_ops(dev);
	struct ieee802154_llsec_key key;
	struct ieee802154_llsec_key_id id;

	if (ieee802154_llsec_parse_key(info, &key) ||
	    ieee802154_llsec_parse_key_id(info, &id))
		return -EINVAL;

	return ops->llsec->add_key(dev, &id, &key);
}

int ieee802154_llsec_add_key(struct sk_buff *skb, struct genl_info *info)
{
	if ((info->nlhdr->nlmsg_flags & (NLM_F_CREATE | NLM_F_EXCL)) !=
	    (NLM_F_CREATE | NLM_F_EXCL))
		return -EINVAL;

	return ieee802154_nl_llsec_change(skb, info, llsec_add_key);
}

static int llsec_remove_key(struct net_device *dev, struct genl_info *info)
{
	struct ieee802154_mlme_ops *ops = ieee802154_mlme_ops(dev);
	struct ieee802154_llsec_key_id id;

	if (ieee802154_llsec_parse_key_id(info, &id))
		return -EINVAL;

	return ops->llsec->del_key(dev, &id);
}

int ieee802154_llsec_del_key(struct sk_buff *skb, struct genl_info *info)
{
	return ieee802154_nl_llsec_change(skb, info, llsec_remove_key);
}

static int
ieee802154_nl_fill_key(struct sk_buff *msg, u32 portid, u32 seq,
		       const struct ieee802154_llsec_key_entry *key,
		       const struct net_device *dev)
{
	void *hdr;
	u32 commands[256 / 32];

	hdr = genlmsg_put(msg, 0, seq, &nl802154_family, NLM_F_MULTI,
			  IEEE802154_LLSEC_LIST_KEY);
	if (!hdr)
		goto out;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    ieee802154_llsec_fill_key_id(msg, &key->id) ||
	    nla_put_u8(msg, IEEE802154_ATTR_LLSEC_KEY_USAGE_FRAME_TYPES,
		       key->key->frame_types))
		goto nla_put_failure;

	if (key->key->frame_types & BIT(IEEE802154_FC_TYPE_MAC_CMD)) {
		memset(commands, 0, sizeof(commands));
		commands[7] = key->key->cmd_frame_ids;
		if (nla_put(msg, IEEE802154_ATTR_LLSEC_KEY_USAGE_COMMANDS,
			    sizeof(commands), commands))
			goto nla_put_failure;
	}

	if (nla_put(msg, IEEE802154_ATTR_LLSEC_KEY_BYTES,
		    IEEE802154_LLSEC_KEY_SIZE, key->key->key))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	return 0;

nla_put_failure:
	genlmsg_cancel(msg, hdr);
out:
	return -EMSGSIZE;
}

static int llsec_iter_keys(struct llsec_dump_data *data)
{
	struct ieee802154_llsec_key_entry *pos;
	int rc = 0, idx = 0;

	list_for_each_entry(pos, &data->table->keys, list) {
		if (idx++ < data->s_idx)
			continue;

		if (ieee802154_nl_fill_key(data->skb, data->portid,
					   data->nlmsg_seq, pos, data->dev)) {
			rc = -EMSGSIZE;
			break;
		}

		data->s_idx++;
	}

	return rc;
}

int ieee802154_llsec_dump_keys(struct sk_buff *skb, struct netlink_callback *cb)
{
	return ieee802154_llsec_dump_table(skb, cb, llsec_iter_keys);
}

static int
llsec_parse_dev(struct genl_info *info,
		struct ieee802154_llsec_device *dev)
{
	memset(dev, 0, sizeof(*dev));

	if (!info->attrs[IEEE802154_ATTR_LLSEC_FRAME_COUNTER] ||
	    !info->attrs[IEEE802154_ATTR_HW_ADDR] ||
	    !info->attrs[IEEE802154_ATTR_LLSEC_DEV_OVERRIDE] ||
	    !info->attrs[IEEE802154_ATTR_LLSEC_DEV_KEY_MODE] ||
	    (!!info->attrs[IEEE802154_ATTR_PAN_ID] !=
	     !!info->attrs[IEEE802154_ATTR_SHORT_ADDR]))
		return -EINVAL;

	if (info->attrs[IEEE802154_ATTR_PAN_ID]) {
		dev->pan_id = nla_get_shortaddr(info->attrs[IEEE802154_ATTR_PAN_ID]);
		dev->short_addr = nla_get_shortaddr(info->attrs[IEEE802154_ATTR_SHORT_ADDR]);
	} else {
		dev->short_addr = cpu_to_le16(IEEE802154_ADDR_UNDEF);
	}

	dev->hwaddr = nla_get_hwaddr(info->attrs[IEEE802154_ATTR_HW_ADDR]);
	dev->frame_counter = nla_get_u32(info->attrs[IEEE802154_ATTR_LLSEC_FRAME_COUNTER]);
	dev->seclevel_exempt = !!nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_DEV_OVERRIDE]);
	dev->key_mode = nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_DEV_KEY_MODE]);

	if (dev->key_mode >= __IEEE802154_LLSEC_DEVKEY_MAX)
		return -EINVAL;

	return 0;
}

static int llsec_add_dev(struct net_device *dev, struct genl_info *info)
{
	struct ieee802154_mlme_ops *ops = ieee802154_mlme_ops(dev);
	struct ieee802154_llsec_device desc;

	if (llsec_parse_dev(info, &desc))
		return -EINVAL;

	return ops->llsec->add_dev(dev, &desc);
}

int ieee802154_llsec_add_dev(struct sk_buff *skb, struct genl_info *info)
{
	if ((info->nlhdr->nlmsg_flags & (NLM_F_CREATE | NLM_F_EXCL)) !=
	    (NLM_F_CREATE | NLM_F_EXCL))
		return -EINVAL;

	return ieee802154_nl_llsec_change(skb, info, llsec_add_dev);
}

static int llsec_del_dev(struct net_device *dev, struct genl_info *info)
{
	struct ieee802154_mlme_ops *ops = ieee802154_mlme_ops(dev);
	__le64 devaddr;

	if (!info->attrs[IEEE802154_ATTR_HW_ADDR])
		return -EINVAL;

	devaddr = nla_get_hwaddr(info->attrs[IEEE802154_ATTR_HW_ADDR]);

	return ops->llsec->del_dev(dev, devaddr);
}

int ieee802154_llsec_del_dev(struct sk_buff *skb, struct genl_info *info)
{
	return ieee802154_nl_llsec_change(skb, info, llsec_del_dev);
}

static int
ieee802154_nl_fill_dev(struct sk_buff *msg, u32 portid, u32 seq,
		       const struct ieee802154_llsec_device *desc,
		       const struct net_device *dev)
{
	void *hdr;

	hdr = genlmsg_put(msg, 0, seq, &nl802154_family, NLM_F_MULTI,
			  IEEE802154_LLSEC_LIST_DEV);
	if (!hdr)
		goto out;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put_shortaddr(msg, IEEE802154_ATTR_PAN_ID, desc->pan_id) ||
	    nla_put_shortaddr(msg, IEEE802154_ATTR_SHORT_ADDR,
			      desc->short_addr) ||
	    nla_put_hwaddr(msg, IEEE802154_ATTR_HW_ADDR, desc->hwaddr,
			   IEEE802154_ATTR_PAD) ||
	    nla_put_u32(msg, IEEE802154_ATTR_LLSEC_FRAME_COUNTER,
			desc->frame_counter) ||
	    nla_put_u8(msg, IEEE802154_ATTR_LLSEC_DEV_OVERRIDE,
		       desc->seclevel_exempt) ||
	    nla_put_u8(msg, IEEE802154_ATTR_LLSEC_DEV_KEY_MODE, desc->key_mode))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	return 0;

nla_put_failure:
	genlmsg_cancel(msg, hdr);
out:
	return -EMSGSIZE;
}

static int llsec_iter_devs(struct llsec_dump_data *data)
{
	struct ieee802154_llsec_device *pos;
	int rc = 0, idx = 0;

	list_for_each_entry(pos, &data->table->devices, list) {
		if (idx++ < data->s_idx)
			continue;

		if (ieee802154_nl_fill_dev(data->skb, data->portid,
					   data->nlmsg_seq, pos, data->dev)) {
			rc = -EMSGSIZE;
			break;
		}

		data->s_idx++;
	}

	return rc;
}

int ieee802154_llsec_dump_devs(struct sk_buff *skb, struct netlink_callback *cb)
{
	return ieee802154_llsec_dump_table(skb, cb, llsec_iter_devs);
}

static int llsec_add_devkey(struct net_device *dev, struct genl_info *info)
{
	struct ieee802154_mlme_ops *ops = ieee802154_mlme_ops(dev);
	struct ieee802154_llsec_device_key key;
	__le64 devaddr;

	if (!info->attrs[IEEE802154_ATTR_LLSEC_FRAME_COUNTER] ||
	    !info->attrs[IEEE802154_ATTR_HW_ADDR] ||
	    ieee802154_llsec_parse_key_id(info, &key.key_id))
		return -EINVAL;

	devaddr = nla_get_hwaddr(info->attrs[IEEE802154_ATTR_HW_ADDR]);
	key.frame_counter = nla_get_u32(info->attrs[IEEE802154_ATTR_LLSEC_FRAME_COUNTER]);

	return ops->llsec->add_devkey(dev, devaddr, &key);
}

int ieee802154_llsec_add_devkey(struct sk_buff *skb, struct genl_info *info)
{
	if ((info->nlhdr->nlmsg_flags & (NLM_F_CREATE | NLM_F_EXCL)) !=
	    (NLM_F_CREATE | NLM_F_EXCL))
		return -EINVAL;

	return ieee802154_nl_llsec_change(skb, info, llsec_add_devkey);
}

static int llsec_del_devkey(struct net_device *dev, struct genl_info *info)
{
	struct ieee802154_mlme_ops *ops = ieee802154_mlme_ops(dev);
	struct ieee802154_llsec_device_key key;
	__le64 devaddr;

	if (!info->attrs[IEEE802154_ATTR_HW_ADDR] ||
	    ieee802154_llsec_parse_key_id(info, &key.key_id))
		return -EINVAL;

	devaddr = nla_get_hwaddr(info->attrs[IEEE802154_ATTR_HW_ADDR]);

	return ops->llsec->del_devkey(dev, devaddr, &key);
}

int ieee802154_llsec_del_devkey(struct sk_buff *skb, struct genl_info *info)
{
	return ieee802154_nl_llsec_change(skb, info, llsec_del_devkey);
}

static int
ieee802154_nl_fill_devkey(struct sk_buff *msg, u32 portid, u32 seq,
			  __le64 devaddr,
			  const struct ieee802154_llsec_device_key *devkey,
			  const struct net_device *dev)
{
	void *hdr;

	hdr = genlmsg_put(msg, 0, seq, &nl802154_family, NLM_F_MULTI,
			  IEEE802154_LLSEC_LIST_DEVKEY);
	if (!hdr)
		goto out;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put_hwaddr(msg, IEEE802154_ATTR_HW_ADDR, devaddr,
			   IEEE802154_ATTR_PAD) ||
	    nla_put_u32(msg, IEEE802154_ATTR_LLSEC_FRAME_COUNTER,
			devkey->frame_counter) ||
	    ieee802154_llsec_fill_key_id(msg, &devkey->key_id))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	return 0;

nla_put_failure:
	genlmsg_cancel(msg, hdr);
out:
	return -EMSGSIZE;
}

static int llsec_iter_devkeys(struct llsec_dump_data *data)
{
	struct ieee802154_llsec_device *dpos;
	struct ieee802154_llsec_device_key *kpos;
	int rc = 0, idx = 0, idx2;

	list_for_each_entry(dpos, &data->table->devices, list) {
		if (idx++ < data->s_idx)
			continue;

		idx2 = 0;

		list_for_each_entry(kpos, &dpos->keys, list) {
			if (idx2++ < data->s_idx2)
				continue;

			if (ieee802154_nl_fill_devkey(data->skb, data->portid,
						      data->nlmsg_seq,
						      dpos->hwaddr, kpos,
						      data->dev)) {
				return rc = -EMSGSIZE;
			}

			data->s_idx2++;
		}

		data->s_idx++;
	}

	return rc;
}

int ieee802154_llsec_dump_devkeys(struct sk_buff *skb,
				  struct netlink_callback *cb)
{
	return ieee802154_llsec_dump_table(skb, cb, llsec_iter_devkeys);
}

static int
llsec_parse_seclevel(struct genl_info *info,
		     struct ieee802154_llsec_seclevel *sl)
{
	memset(sl, 0, sizeof(*sl));

	if (!info->attrs[IEEE802154_ATTR_LLSEC_FRAME_TYPE] ||
	    !info->attrs[IEEE802154_ATTR_LLSEC_SECLEVELS] ||
	    !info->attrs[IEEE802154_ATTR_LLSEC_DEV_OVERRIDE])
		return -EINVAL;

	sl->frame_type = nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_FRAME_TYPE]);
	if (sl->frame_type == IEEE802154_FC_TYPE_MAC_CMD) {
		if (!info->attrs[IEEE802154_ATTR_LLSEC_CMD_FRAME_ID])
			return -EINVAL;

		sl->cmd_frame_id = nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_CMD_FRAME_ID]);
	}

	sl->sec_levels = nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_SECLEVELS]);
	sl->device_override = nla_get_u8(info->attrs[IEEE802154_ATTR_LLSEC_DEV_OVERRIDE]);

	return 0;
}

static int llsec_add_seclevel(struct net_device *dev, struct genl_info *info)
{
	struct ieee802154_mlme_ops *ops = ieee802154_mlme_ops(dev);
	struct ieee802154_llsec_seclevel sl;

	if (llsec_parse_seclevel(info, &sl))
		return -EINVAL;

	return ops->llsec->add_seclevel(dev, &sl);
}

int ieee802154_llsec_add_seclevel(struct sk_buff *skb, struct genl_info *info)
{
	if ((info->nlhdr->nlmsg_flags & (NLM_F_CREATE | NLM_F_EXCL)) !=
	    (NLM_F_CREATE | NLM_F_EXCL))
		return -EINVAL;

	return ieee802154_nl_llsec_change(skb, info, llsec_add_seclevel);
}

static int llsec_del_seclevel(struct net_device *dev, struct genl_info *info)
{
	struct ieee802154_mlme_ops *ops = ieee802154_mlme_ops(dev);
	struct ieee802154_llsec_seclevel sl;

	if (llsec_parse_seclevel(info, &sl))
		return -EINVAL;

	return ops->llsec->del_seclevel(dev, &sl);
}

int ieee802154_llsec_del_seclevel(struct sk_buff *skb, struct genl_info *info)
{
	return ieee802154_nl_llsec_change(skb, info, llsec_del_seclevel);
}

static int
ieee802154_nl_fill_seclevel(struct sk_buff *msg, u32 portid, u32 seq,
			    const struct ieee802154_llsec_seclevel *sl,
			    const struct net_device *dev)
{
	void *hdr;

	hdr = genlmsg_put(msg, 0, seq, &nl802154_family, NLM_F_MULTI,
			  IEEE802154_LLSEC_LIST_SECLEVEL);
	if (!hdr)
		goto out;

	if (nla_put_string(msg, IEEE802154_ATTR_DEV_NAME, dev->name) ||
	    nla_put_u32(msg, IEEE802154_ATTR_DEV_INDEX, dev->ifindex) ||
	    nla_put_u8(msg, IEEE802154_ATTR_LLSEC_FRAME_TYPE, sl->frame_type) ||
	    nla_put_u8(msg, IEEE802154_ATTR_LLSEC_SECLEVELS, sl->sec_levels) ||
	    nla_put_u8(msg, IEEE802154_ATTR_LLSEC_DEV_OVERRIDE,
		       sl->device_override))
		goto nla_put_failure;

	if (sl->frame_type == IEEE802154_FC_TYPE_MAC_CMD &&
	    nla_put_u8(msg, IEEE802154_ATTR_LLSEC_CMD_FRAME_ID,
		       sl->cmd_frame_id))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	return 0;

nla_put_failure:
	genlmsg_cancel(msg, hdr);
out:
	return -EMSGSIZE;
}

static int llsec_iter_seclevels(struct llsec_dump_data *data)
{
	struct ieee802154_llsec_seclevel *pos;
	int rc = 0, idx = 0;

	list_for_each_entry(pos, &data->table->security_levels, list) {
		if (idx++ < data->s_idx)
			continue;

		if (ieee802154_nl_fill_seclevel(data->skb, data->portid,
						data->nlmsg_seq, pos,
						data->dev)) {
			rc = -EMSGSIZE;
			break;
		}

		data->s_idx++;
	}

	return rc;
}

int ieee802154_llsec_dump_seclevels(struct sk_buff *skb,
				    struct netlink_callback *cb)
{
	return ieee802154_llsec_dump_table(skb, cb, llsec_iter_seclevels);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
