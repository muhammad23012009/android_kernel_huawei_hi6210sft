/*
 * Copyright (c) 2007-2012 Nicira, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/if_arp.h>
#include <linux/if_bridge.h>
#include <linux/if_vlan.h>
#include <linux/kernel.h>
#include <linux/llc.h>
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>
<<<<<<< HEAD

#include <net/llc.h>

#include "datapath.h"
#include "vport-internal_dev.h"
#include "vport-netdev.h"

/* Must be called with rcu_read_lock. */
static void netdev_port_receive(struct vport *vport, struct sk_buff *skb)
{
=======
#include <linux/openvswitch.h>
#include <linux/export.h>

#include <net/ip_tunnels.h>
#include <net/rtnetlink.h>

#include "datapath.h"
#include "vport.h"
#include "vport-internal_dev.h"
#include "vport-netdev.h"

static struct vport_ops ovs_netdev_vport_ops;

/* Must be called with rcu_read_lock. */
static void netdev_port_receive(struct sk_buff *skb)
{
	struct vport *vport;

	vport = ovs_netdev_get_vport(skb->dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (unlikely(!vport))
		goto error;

	if (unlikely(skb_warn_if_lro(skb)))
		goto error;

	/* Make our own copy of the packet.  Otherwise we will mangle the
	 * packet for anyone who came before us (e.g. tcpdump via AF_PACKET).
	 */
	skb = skb_share_check(skb, GFP_ATOMIC);
	if (unlikely(!skb))
		return;

	skb_push(skb, ETH_HLEN);
<<<<<<< HEAD
	ovs_vport_receive(vport, skb);
	return;

=======
	skb_postpush_rcsum(skb, skb->data, ETH_HLEN);
	ovs_vport_receive(vport, skb, skb_tunnel_info(skb));
	return;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
error:
	kfree_skb(skb);
}

/* Called with rcu_read_lock and bottom-halves disabled. */
static rx_handler_result_t netdev_frame_hook(struct sk_buff **pskb)
{
	struct sk_buff *skb = *pskb;
<<<<<<< HEAD
	struct vport *vport;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (unlikely(skb->pkt_type == PACKET_LOOPBACK))
		return RX_HANDLER_PASS;

<<<<<<< HEAD
	vport = ovs_netdev_get_vport(skb->dev);

	netdev_port_receive(vport, skb);

	return RX_HANDLER_CONSUMED;
}

static struct vport *netdev_create(const struct vport_parms *parms)
{
	struct vport *vport;
	struct netdev_vport *netdev_vport;
	int err;

	vport = ovs_vport_alloc(sizeof(struct netdev_vport),
				&ovs_netdev_vport_ops, parms);
	if (IS_ERR(vport)) {
		err = PTR_ERR(vport);
		goto error;
	}

	netdev_vport = netdev_vport_priv(vport);

	netdev_vport->dev = dev_get_by_name(ovs_dp_get_net(vport->dp), parms->name);
	if (!netdev_vport->dev) {
=======
	netdev_port_receive(skb);
	return RX_HANDLER_CONSUMED;
}

static struct net_device *get_dpdev(const struct datapath *dp)
{
	struct vport *local;

	local = ovs_vport_ovsl(dp, OVSP_LOCAL);
	BUG_ON(!local);
	return local->dev;
}

struct vport *ovs_netdev_link(struct vport *vport, const char *name)
{
	int err;

	vport->dev = dev_get_by_name(ovs_dp_get_net(vport->dp), name);
	if (!vport->dev) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		err = -ENODEV;
		goto error_free_vport;
	}

<<<<<<< HEAD
	if (netdev_vport->dev->flags & IFF_LOOPBACK ||
	    netdev_vport->dev->type != ARPHRD_ETHER ||
	    ovs_is_internal_dev(netdev_vport->dev)) {
=======
	if (vport->dev->flags & IFF_LOOPBACK ||
	    vport->dev->type != ARPHRD_ETHER ||
	    ovs_is_internal_dev(vport->dev)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		err = -EINVAL;
		goto error_put;
	}

	rtnl_lock();
<<<<<<< HEAD
	err = netdev_rx_handler_register(netdev_vport->dev, netdev_frame_hook,
					 vport);
	if (err)
		goto error_unlock;

	dev_set_promiscuity(netdev_vport->dev, 1);
	netdev_vport->dev->priv_flags |= IFF_OVS_DATAPATH;
=======
	err = netdev_master_upper_dev_link(vport->dev,
					   get_dpdev(vport->dp), NULL, NULL);
	if (err)
		goto error_unlock;

	err = netdev_rx_handler_register(vport->dev, netdev_frame_hook,
					 vport);
	if (err)
		goto error_master_upper_dev_unlink;

	dev_disable_lro(vport->dev);
	dev_set_promiscuity(vport->dev, 1);
	vport->dev->priv_flags |= IFF_OVS_DATAPATH;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rtnl_unlock();

	return vport;

<<<<<<< HEAD
error_unlock:
	rtnl_unlock();
error_put:
	dev_put(netdev_vport->dev);
error_free_vport:
	ovs_vport_free(vport);
error:
	return ERR_PTR(err);
}

static void free_port_rcu(struct rcu_head *rcu)
{
	struct netdev_vport *netdev_vport = container_of(rcu,
					struct netdev_vport, rcu);

	dev_put(netdev_vport->dev);
	ovs_vport_free(vport_from_priv(netdev_vport));
}

static void netdev_destroy(struct vport *vport)
{
	struct netdev_vport *netdev_vport = netdev_vport_priv(vport);

	rtnl_lock();
	netdev_vport->dev->priv_flags &= ~IFF_OVS_DATAPATH;
	netdev_rx_handler_unregister(netdev_vport->dev);
	dev_set_promiscuity(netdev_vport->dev, -1);
	rtnl_unlock();

	call_rcu(&netdev_vport->rcu, free_port_rcu);
}

const char *ovs_netdev_get_name(const struct vport *vport)
{
	const struct netdev_vport *netdev_vport = netdev_vport_priv(vport);
	return netdev_vport->dev->name;
}

static unsigned int packet_length(const struct sk_buff *skb)
{
	unsigned int length = skb->len - ETH_HLEN;

	if (skb->protocol == htons(ETH_P_8021Q))
		length -= VLAN_HLEN;

	return length;
}

static int netdev_send(struct vport *vport, struct sk_buff *skb)
{
	struct netdev_vport *netdev_vport = netdev_vport_priv(vport);
	int mtu = netdev_vport->dev->mtu;
	int len;

	if (unlikely(packet_length(skb) > mtu && !skb_is_gso(skb))) {
		net_warn_ratelimited("%s: dropped over-mtu packet: %d > %d\n",
				     netdev_vport->dev->name,
				     packet_length(skb), mtu);
		goto error;
	}

	skb->dev = netdev_vport->dev;
	len = skb->len;
	dev_queue_xmit(skb);

	return len;

error:
	kfree_skb(skb);
	ovs_vport_record_error(vport, VPORT_E_TX_DROPPED);
	return 0;
}
=======
error_master_upper_dev_unlink:
	netdev_upper_dev_unlink(vport->dev, get_dpdev(vport->dp));
error_unlock:
	rtnl_unlock();
error_put:
	dev_put(vport->dev);
error_free_vport:
	ovs_vport_free(vport);
	return ERR_PTR(err);
}
EXPORT_SYMBOL_GPL(ovs_netdev_link);

static struct vport *netdev_create(const struct vport_parms *parms)
{
	struct vport *vport;

	vport = ovs_vport_alloc(0, &ovs_netdev_vport_ops, parms);
	if (IS_ERR(vport))
		return vport;

	return ovs_netdev_link(vport, parms->name);
}

static void vport_netdev_free(struct rcu_head *rcu)
{
	struct vport *vport = container_of(rcu, struct vport, rcu);

	if (vport->dev)
		dev_put(vport->dev);
	ovs_vport_free(vport);
}

void ovs_netdev_detach_dev(struct vport *vport)
{
	ASSERT_RTNL();
	vport->dev->priv_flags &= ~IFF_OVS_DATAPATH;
	netdev_rx_handler_unregister(vport->dev);
	netdev_upper_dev_unlink(vport->dev,
				netdev_master_upper_dev_get(vport->dev));
	dev_set_promiscuity(vport->dev, -1);
}
EXPORT_SYMBOL_GPL(ovs_netdev_detach_dev);

static void netdev_destroy(struct vport *vport)
{
	rtnl_lock();
	if (vport->dev->priv_flags & IFF_OVS_DATAPATH)
		ovs_netdev_detach_dev(vport);
	rtnl_unlock();

	call_rcu(&vport->rcu, vport_netdev_free);
}

void ovs_netdev_tunnel_destroy(struct vport *vport)
{
	rtnl_lock();
	if (vport->dev->priv_flags & IFF_OVS_DATAPATH)
		ovs_netdev_detach_dev(vport);

	/* We can be invoked by both explicit vport deletion and
	 * underlying netdev deregistration; delete the link only
	 * if it's not already shutting down.
	 */
	if (vport->dev->reg_state == NETREG_REGISTERED)
		rtnl_delete_link(vport->dev);
	dev_put(vport->dev);
	vport->dev = NULL;
	rtnl_unlock();

	call_rcu(&vport->rcu, vport_netdev_free);
}
EXPORT_SYMBOL_GPL(ovs_netdev_tunnel_destroy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Returns null if this device is not attached to a datapath. */
struct vport *ovs_netdev_get_vport(struct net_device *dev)
{
	if (likely(dev->priv_flags & IFF_OVS_DATAPATH))
		return (struct vport *)
			rcu_dereference_rtnl(dev->rx_handler_data);
	else
		return NULL;
}

<<<<<<< HEAD
const struct vport_ops ovs_netdev_vport_ops = {
	.type		= OVS_VPORT_TYPE_NETDEV,
	.create		= netdev_create,
	.destroy	= netdev_destroy,
	.get_name	= ovs_netdev_get_name,
	.send		= netdev_send,
};
=======
static struct vport_ops ovs_netdev_vport_ops = {
	.type		= OVS_VPORT_TYPE_NETDEV,
	.create		= netdev_create,
	.destroy	= netdev_destroy,
	.send		= dev_queue_xmit,
};

int __init ovs_netdev_init(void)
{
	return ovs_vport_ops_register(&ovs_netdev_vport_ops);
}

void ovs_netdev_exit(void)
{
	ovs_vport_ops_unregister(&ovs_netdev_vport_ops);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
