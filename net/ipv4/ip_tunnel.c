/*
 * Copyright (c) 2013 Nicira, Inc.
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

#include <linux/capability.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/if_arp.h>
<<<<<<< HEAD
#include <linux/mroute.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/init.h>
#include <linux/in6.h>
#include <linux/inetdevice.h>
#include <linux/igmp.h>
#include <linux/netfilter_ipv4.h>
#include <linux/etherdevice.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/rculist.h>
<<<<<<< HEAD
=======
#include <linux/err.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <net/sock.h>
#include <net/ip.h>
#include <net/icmp.h>
#include <net/protocol.h>
#include <net/ip_tunnels.h>
#include <net/arp.h>
#include <net/checksum.h>
#include <net/dsfield.h>
#include <net/inet_ecn.h>
#include <net/xfrm.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
#include <net/rtnetlink.h>
<<<<<<< HEAD
=======
#include <net/udp.h>
#include <net/dst_metadata.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#if IS_ENABLED(CONFIG_IPV6)
#include <net/ipv6.h>
#include <net/ip6_fib.h>
#include <net/ip6_route.h>
#endif

<<<<<<< HEAD
static unsigned int ip_tunnel_hash(struct ip_tunnel_net *itn,
				   __be32 key, __be32 remote)
=======
static unsigned int ip_tunnel_hash(__be32 key, __be32 remote)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return hash_32((__force u32)key ^ (__force u32)remote,
			 IP_TNL_HASH_BITS);
}

<<<<<<< HEAD
/* Often modified stats are per cpu, other are shared (netdev->stats) */
struct rtnl_link_stats64 *ip_tunnel_get_stats64(struct net_device *dev,
						struct rtnl_link_stats64 *tot)
{
	int i;

	for_each_possible_cpu(i) {
		const struct pcpu_tstats *tstats = per_cpu_ptr(dev->tstats, i);
		u64 rx_packets, rx_bytes, tx_packets, tx_bytes;
		unsigned int start;

		do {
			start = u64_stats_fetch_begin_bh(&tstats->syncp);
			rx_packets = tstats->rx_packets;
			tx_packets = tstats->tx_packets;
			rx_bytes = tstats->rx_bytes;
			tx_bytes = tstats->tx_bytes;
		} while (u64_stats_fetch_retry_bh(&tstats->syncp, start));

		tot->rx_packets += rx_packets;
		tot->tx_packets += tx_packets;
		tot->rx_bytes   += rx_bytes;
		tot->tx_bytes   += tx_bytes;
	}

	tot->multicast = dev->stats.multicast;

	tot->rx_crc_errors = dev->stats.rx_crc_errors;
	tot->rx_fifo_errors = dev->stats.rx_fifo_errors;
	tot->rx_length_errors = dev->stats.rx_length_errors;
	tot->rx_frame_errors = dev->stats.rx_frame_errors;
	tot->rx_errors = dev->stats.rx_errors;

	tot->tx_fifo_errors = dev->stats.tx_fifo_errors;
	tot->tx_carrier_errors = dev->stats.tx_carrier_errors;
	tot->tx_dropped = dev->stats.tx_dropped;
	tot->tx_aborted_errors = dev->stats.tx_aborted_errors;
	tot->tx_errors = dev->stats.tx_errors;

	tot->collisions  = dev->stats.collisions;

	return tot;
}
EXPORT_SYMBOL_GPL(ip_tunnel_get_stats64);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static bool ip_tunnel_key_match(const struct ip_tunnel_parm *p,
				__be16 flags, __be32 key)
{
	if (p->i_flags & TUNNEL_KEY) {
		if (flags & TUNNEL_KEY)
			return key == p->i_key;
		else
			/* key expected, none present */
			return false;
	} else
		return !(flags & TUNNEL_KEY);
}

/* Fallback tunnel: no source, no destination, no key, no options

   Tunnel hash table:
   We require exact key match i.e. if a key is present in packet
   it will match only tunnel with the same key; if it is not present,
   it will match only keyless tunnel.

   All keysless packets, if not matched configured keyless tunnels
   will match fallback tunnel.
   Given src, dst and key, find appropriate for input tunnel.
*/
struct ip_tunnel *ip_tunnel_lookup(struct ip_tunnel_net *itn,
				   int link, __be16 flags,
				   __be32 remote, __be32 local,
				   __be32 key)
{
<<<<<<< HEAD
	unsigned int hash;
	struct ip_tunnel *t, *cand = NULL;
	struct hlist_head *head;

	hash = ip_tunnel_hash(itn, key, remote);
=======
	struct ip_tunnel *t, *cand = NULL;
	struct hlist_head *head;
	struct net_device *ndev;
	unsigned int hash;

	hash = ip_tunnel_hash(key, remote);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	head = &itn->tunnels[hash];

	hlist_for_each_entry_rcu(t, head, hash_node) {
		if (local != t->parms.iph.saddr ||
		    remote != t->parms.iph.daddr ||
		    !(t->dev->flags & IFF_UP))
			continue;

		if (!ip_tunnel_key_match(&t->parms, flags, key))
			continue;

		if (t->parms.link == link)
			return t;
		else
			cand = t;
	}

	hlist_for_each_entry_rcu(t, head, hash_node) {
		if (remote != t->parms.iph.daddr ||
		    t->parms.iph.saddr != 0 ||
		    !(t->dev->flags & IFF_UP))
			continue;

		if (!ip_tunnel_key_match(&t->parms, flags, key))
			continue;

		if (t->parms.link == link)
			return t;
		else if (!cand)
			cand = t;
	}

<<<<<<< HEAD
	hash = ip_tunnel_hash(itn, key, 0);
=======
	hash = ip_tunnel_hash(key, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	head = &itn->tunnels[hash];

	hlist_for_each_entry_rcu(t, head, hash_node) {
		if ((local != t->parms.iph.saddr || t->parms.iph.daddr != 0) &&
		    (local != t->parms.iph.daddr || !ipv4_is_multicast(local)))
			continue;

		if (!(t->dev->flags & IFF_UP))
			continue;

		if (!ip_tunnel_key_match(&t->parms, flags, key))
			continue;

		if (t->parms.link == link)
			return t;
		else if (!cand)
			cand = t;
	}

<<<<<<< HEAD
	if (flags & TUNNEL_NO_KEY)
		goto skip_key_lookup;

	hlist_for_each_entry_rcu(t, head, hash_node) {
		if (t->parms.i_key != key ||
=======
	hlist_for_each_entry_rcu(t, head, hash_node) {
		if ((!(flags & TUNNEL_NO_KEY) && t->parms.i_key != key) ||
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		    t->parms.iph.saddr != 0 ||
		    t->parms.iph.daddr != 0 ||
		    !(t->dev->flags & IFF_UP))
			continue;

		if (t->parms.link == link)
			return t;
		else if (!cand)
			cand = t;
	}

<<<<<<< HEAD
skip_key_lookup:
	if (cand)
		return cand;

	if (itn->fb_tunnel_dev && itn->fb_tunnel_dev->flags & IFF_UP)
		return netdev_priv(itn->fb_tunnel_dev);

=======
	if (cand)
		return cand;

	t = rcu_dereference(itn->collect_md_tun);
	if (t)
		return t;

	ndev = READ_ONCE(itn->fb_tunnel_dev);
	if (ndev && ndev->flags & IFF_UP)
		return netdev_priv(ndev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return NULL;
}
EXPORT_SYMBOL_GPL(ip_tunnel_lookup);

static struct hlist_head *ip_bucket(struct ip_tunnel_net *itn,
				    struct ip_tunnel_parm *parms)
{
	unsigned int h;
	__be32 remote;
<<<<<<< HEAD
=======
	__be32 i_key = parms->i_key;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (parms->iph.daddr && !ipv4_is_multicast(parms->iph.daddr))
		remote = parms->iph.daddr;
	else
		remote = 0;

<<<<<<< HEAD
	h = ip_tunnel_hash(itn, parms->i_key, remote);
=======
	if (!(parms->i_flags & TUNNEL_KEY) && (parms->i_flags & VTI_ISVTI))
		i_key = 0;

	h = ip_tunnel_hash(i_key, remote);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return &itn->tunnels[h];
}

static void ip_tunnel_add(struct ip_tunnel_net *itn, struct ip_tunnel *t)
{
	struct hlist_head *head = ip_bucket(itn, &t->parms);

<<<<<<< HEAD
	hlist_add_head_rcu(&t->hash_node, head);
}

static void ip_tunnel_del(struct ip_tunnel *t)
{
=======
	if (t->collect_md)
		rcu_assign_pointer(itn->collect_md_tun, t);
	hlist_add_head_rcu(&t->hash_node, head);
}

static void ip_tunnel_del(struct ip_tunnel_net *itn, struct ip_tunnel *t)
{
	if (t->collect_md)
		rcu_assign_pointer(itn->collect_md_tun, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	hlist_del_init_rcu(&t->hash_node);
}

static struct ip_tunnel *ip_tunnel_find(struct ip_tunnel_net *itn,
					struct ip_tunnel_parm *parms,
					int type)
{
	__be32 remote = parms->iph.daddr;
	__be32 local = parms->iph.saddr;
	__be32 key = parms->i_key;
<<<<<<< HEAD
=======
	__be16 flags = parms->i_flags;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int link = parms->link;
	struct ip_tunnel *t = NULL;
	struct hlist_head *head = ip_bucket(itn, parms);

	hlist_for_each_entry_rcu(t, head, hash_node) {
		if (local == t->parms.iph.saddr &&
		    remote == t->parms.iph.daddr &&
<<<<<<< HEAD
		    key == t->parms.i_key &&
		    link == t->parms.link &&
		    type == t->dev->type)
=======
		    link == t->parms.link &&
		    type == t->dev->type &&
		    ip_tunnel_key_match(&t->parms, flags, key))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
	}
	return t;
}

static struct net_device *__ip_tunnel_create(struct net *net,
					     const struct rtnl_link_ops *ops,
					     struct ip_tunnel_parm *parms)
{
	int err;
	struct ip_tunnel *tunnel;
	struct net_device *dev;
	char name[IFNAMSIZ];

<<<<<<< HEAD
	if (parms->name[0])
		strlcpy(name, parms->name, IFNAMSIZ);
	else {
		if (strlen(ops->kind) > (IFNAMSIZ - 3)) {
			err = -E2BIG;
			goto failed;
		}
		strlcpy(name, ops->kind, IFNAMSIZ);
		strncat(name, "%d", 2);
	}

	ASSERT_RTNL();
	dev = alloc_netdev(ops->priv_size, name, ops->setup);
=======
	err = -E2BIG;
	if (parms->name[0]) {
		if (!dev_valid_name(parms->name))
			goto failed;
		strlcpy(name, parms->name, IFNAMSIZ);
	} else {
		if (strlen(ops->kind) > (IFNAMSIZ - 3))
			goto failed;
		strcpy(name, ops->kind);
		strcat(name, "%d");
	}

	ASSERT_RTNL();
	dev = alloc_netdev(ops->priv_size, name, NET_NAME_UNKNOWN, ops->setup);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!dev) {
		err = -ENOMEM;
		goto failed;
	}
	dev_net_set(dev, net);

	dev->rtnl_link_ops = ops;

	tunnel = netdev_priv(dev);
	tunnel->parms = *parms;
<<<<<<< HEAD
=======
	tunnel->net = net;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	err = register_netdevice(dev);
	if (err)
		goto failed_free;

	return dev;

failed_free:
	free_netdev(dev);
failed:
	return ERR_PTR(err);
}

<<<<<<< HEAD
static inline struct rtable *ip_route_output_tunnel(struct net *net,
						    struct flowi4 *fl4,
						    int proto,
						    __be32 daddr, __be32 saddr,
						    __be32 key, __u8 tos, int oif)
=======
static inline void init_tunnel_flow(struct flowi4 *fl4,
				    int proto,
				    __be32 daddr, __be32 saddr,
				    __be32 key, __u8 tos, int oif)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	memset(fl4, 0, sizeof(*fl4));
	fl4->flowi4_oif = oif;
	fl4->daddr = daddr;
	fl4->saddr = saddr;
	fl4->flowi4_tos = tos;
	fl4->flowi4_proto = proto;
	fl4->fl4_gre_key = key;
<<<<<<< HEAD
	return ip_route_output_key(net, fl4);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int ip_tunnel_bind_dev(struct net_device *dev)
{
	struct net_device *tdev = NULL;
	struct ip_tunnel *tunnel = netdev_priv(dev);
	const struct iphdr *iph;
	int hlen = LL_MAX_HEADER;
	int mtu = ETH_DATA_LEN;
	int t_hlen = tunnel->hlen + sizeof(struct iphdr);

	iph = &tunnel->parms.iph;

	/* Guess output device to choose reasonable mtu and needed_headroom */
	if (iph->daddr) {
		struct flowi4 fl4;
		struct rtable *rt;

<<<<<<< HEAD
		rt = ip_route_output_tunnel(dev_net(dev), &fl4,
					    tunnel->parms.iph.protocol,
					    iph->daddr, iph->saddr,
					    tunnel->parms.o_key,
					    RT_TOS(iph->tos),
					    tunnel->parms.link);
=======
		init_tunnel_flow(&fl4, iph->protocol, iph->daddr,
				 iph->saddr, tunnel->parms.o_key,
				 RT_TOS(iph->tos), tunnel->parms.link);
		rt = ip_route_output_key(tunnel->net, &fl4);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!IS_ERR(rt)) {
			tdev = rt->dst.dev;
			ip_rt_put(rt);
		}
		if (dev->type != ARPHRD_ETHER)
			dev->flags |= IFF_POINTOPOINT;
<<<<<<< HEAD
	}

	if (!tdev && tunnel->parms.link)
		tdev = __dev_get_by_index(dev_net(dev), tunnel->parms.link);
=======

		dst_cache_reset(&tunnel->dst_cache);
	}

	if (!tdev && tunnel->parms.link)
		tdev = __dev_get_by_index(tunnel->net, tunnel->parms.link);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (tdev) {
		hlen = tdev->hard_header_len + tdev->needed_headroom;
		mtu = tdev->mtu;
	}
<<<<<<< HEAD
	dev->iflink = tunnel->parms.link;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	dev->needed_headroom = t_hlen + hlen;
	mtu -= (dev->hard_header_len + t_hlen);

<<<<<<< HEAD
	if (mtu < 68)
		mtu = 68;
=======
	if (mtu < IPV4_MIN_MTU)
		mtu = IPV4_MIN_MTU;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return mtu;
}

static struct ip_tunnel *ip_tunnel_create(struct net *net,
					  struct ip_tunnel_net *itn,
					  struct ip_tunnel_parm *parms)
{
<<<<<<< HEAD
	struct ip_tunnel *nt, *fbt;
	struct net_device *dev;

	BUG_ON(!itn->fb_tunnel_dev);
	fbt = netdev_priv(itn->fb_tunnel_dev);
	dev = __ip_tunnel_create(net, itn->fb_tunnel_dev->rtnl_link_ops, parms);
	if (IS_ERR(dev))
		return NULL;
=======
	struct ip_tunnel *nt;
	struct net_device *dev;

	BUG_ON(!itn->fb_tunnel_dev);
	dev = __ip_tunnel_create(net, itn->fb_tunnel_dev->rtnl_link_ops, parms);
	if (IS_ERR(dev))
		return ERR_CAST(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	dev->mtu = ip_tunnel_bind_dev(dev);

	nt = netdev_priv(dev);
	ip_tunnel_add(itn, nt);
	return nt;
}

int ip_tunnel_rcv(struct ip_tunnel *tunnel, struct sk_buff *skb,
<<<<<<< HEAD
		  const struct tnl_ptk_info *tpi, int hdr_len, bool log_ecn_error)
{
	struct pcpu_tstats *tstats;
	const struct iphdr *iph = ip_hdr(skb);
	int err;

	secpath_reset(skb);

	skb->protocol = tpi->proto;

	skb->mac_header = skb->network_header;
	__pskb_pull(skb, hdr_len);
	skb_postpull_rcsum(skb, skb_transport_header(skb), tunnel->hlen);
#ifdef CONFIG_NET_IPGRE_BROADCAST
	if (ipv4_is_multicast(iph->daddr)) {
		/* Looped back packet, drop it! */
		if (rt_is_output_route(skb_rtable(skb)))
			goto drop;
=======
		  const struct tnl_ptk_info *tpi, struct metadata_dst *tun_dst,
		  bool log_ecn_error)
{
	struct pcpu_sw_netstats *tstats;
	const struct iphdr *iph = ip_hdr(skb);
	int err;

#ifdef CONFIG_NET_IPGRE_BROADCAST
	if (ipv4_is_multicast(iph->daddr)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		tunnel->dev->stats.multicast++;
		skb->pkt_type = PACKET_BROADCAST;
	}
#endif

	if ((!(tpi->flags&TUNNEL_CSUM) &&  (tunnel->parms.i_flags&TUNNEL_CSUM)) ||
	     ((tpi->flags&TUNNEL_CSUM) && !(tunnel->parms.i_flags&TUNNEL_CSUM))) {
		tunnel->dev->stats.rx_crc_errors++;
		tunnel->dev->stats.rx_errors++;
		goto drop;
	}

	if (tunnel->parms.i_flags&TUNNEL_SEQ) {
		if (!(tpi->flags&TUNNEL_SEQ) ||
		    (tunnel->i_seqno && (s32)(ntohl(tpi->seq) - tunnel->i_seqno) < 0)) {
			tunnel->dev->stats.rx_fifo_errors++;
			tunnel->dev->stats.rx_errors++;
			goto drop;
		}
		tunnel->i_seqno = ntohl(tpi->seq) + 1;
	}

<<<<<<< HEAD
	/* Warning: All skb pointers will be invalidated! */
	if (tunnel->dev->type == ARPHRD_ETHER) {
		if (!pskb_may_pull(skb, ETH_HLEN)) {
			tunnel->dev->stats.rx_length_errors++;
			tunnel->dev->stats.rx_errors++;
			goto drop;
		}

		iph = ip_hdr(skb);
		skb->protocol = eth_type_trans(skb, tunnel->dev);
		skb_postpull_rcsum(skb, eth_hdr(skb), ETH_HLEN);
	}

	skb->pkt_type = PACKET_HOST;
	__skb_tunnel_rx(skb, tunnel->dev);

	skb_reset_network_header(skb);
=======
	skb_reset_network_header(skb);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	err = IP_ECN_decapsulate(iph, skb);
	if (unlikely(err)) {
		if (log_ecn_error)
			net_info_ratelimited("non-ECT from %pI4 with TOS=%#x\n",
					&iph->saddr, iph->tos);
		if (err > 1) {
			++tunnel->dev->stats.rx_frame_errors;
			++tunnel->dev->stats.rx_errors;
			goto drop;
		}
	}

	tstats = this_cpu_ptr(tunnel->dev->tstats);
	u64_stats_update_begin(&tstats->syncp);
	tstats->rx_packets++;
	tstats->rx_bytes += skb->len;
	u64_stats_update_end(&tstats->syncp);

<<<<<<< HEAD
=======
	skb_scrub_packet(skb, !net_eq(tunnel->net, dev_net(tunnel->dev)));

	if (tunnel->dev->type == ARPHRD_ETHER) {
		skb->protocol = eth_type_trans(skb, tunnel->dev);
		skb_postpull_rcsum(skb, eth_hdr(skb), ETH_HLEN);
	} else {
		skb->dev = tunnel->dev;
	}

	if (tun_dst)
		skb_dst_set(skb, (struct dst_entry *)tun_dst);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	gro_cells_receive(&tunnel->gro_cells, skb);
	return 0;

drop:
	kfree_skb(skb);
	return 0;
}
EXPORT_SYMBOL_GPL(ip_tunnel_rcv);

<<<<<<< HEAD
static int tnl_update_pmtu(struct net_device *dev, struct sk_buff *skb,
			    struct rtable *rt, __be16 df)
=======
int ip_tunnel_encap_add_ops(const struct ip_tunnel_encap_ops *ops,
			    unsigned int num)
{
	if (num >= MAX_IPTUN_ENCAP_OPS)
		return -ERANGE;

	return !cmpxchg((const struct ip_tunnel_encap_ops **)
			&iptun_encaps[num],
			NULL, ops) ? 0 : -1;
}
EXPORT_SYMBOL(ip_tunnel_encap_add_ops);

int ip_tunnel_encap_del_ops(const struct ip_tunnel_encap_ops *ops,
			    unsigned int num)
{
	int ret;

	if (num >= MAX_IPTUN_ENCAP_OPS)
		return -ERANGE;

	ret = (cmpxchg((const struct ip_tunnel_encap_ops **)
		       &iptun_encaps[num],
		       ops, NULL) == ops) ? 0 : -1;

	synchronize_net();

	return ret;
}
EXPORT_SYMBOL(ip_tunnel_encap_del_ops);

int ip_tunnel_encap_setup(struct ip_tunnel *t,
			  struct ip_tunnel_encap *ipencap)
{
	int hlen;

	memset(&t->encap, 0, sizeof(t->encap));

	hlen = ip_encap_hlen(ipencap);
	if (hlen < 0)
		return hlen;

	t->encap.type = ipencap->type;
	t->encap.sport = ipencap->sport;
	t->encap.dport = ipencap->dport;
	t->encap.flags = ipencap->flags;

	t->encap_hlen = hlen;
	t->hlen = t->encap_hlen + t->tun_hlen;

	return 0;
}
EXPORT_SYMBOL_GPL(ip_tunnel_encap_setup);

static int tnl_update_pmtu(struct net_device *dev, struct sk_buff *skb,
			    struct rtable *rt, __be16 df,
			    const struct iphdr *inner_iph)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	int pkt_size = skb->len - tunnel->hlen - dev->hard_header_len;
	int mtu;

	if (df)
		mtu = dst_mtu(&rt->dst) - dev->hard_header_len
					- sizeof(struct iphdr) - tunnel->hlen;
	else
		mtu = skb_dst(skb) ? dst_mtu(skb_dst(skb)) : dev->mtu;

	if (skb_dst(skb))
		skb_dst(skb)->ops->update_pmtu(skb_dst(skb), NULL, skb, mtu);

	if (skb->protocol == htons(ETH_P_IP)) {
		if (!skb_is_gso(skb) &&
<<<<<<< HEAD
		    (df & htons(IP_DF)) && mtu < pkt_size) {
=======
		    (inner_iph->frag_off & htons(IP_DF)) &&
		    mtu < pkt_size) {
			memset(IPCB(skb), 0, sizeof(*IPCB(skb)));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			icmp_send(skb, ICMP_DEST_UNREACH, ICMP_FRAG_NEEDED, htonl(mtu));
			return -E2BIG;
		}
	}
#if IS_ENABLED(CONFIG_IPV6)
	else if (skb->protocol == htons(ETH_P_IPV6)) {
		struct rt6_info *rt6 = (struct rt6_info *)skb_dst(skb);

		if (rt6 && mtu < dst_mtu(skb_dst(skb)) &&
			   mtu >= IPV6_MIN_MTU) {
			if ((tunnel->parms.iph.daddr &&
			    !ipv4_is_multicast(tunnel->parms.iph.daddr)) ||
			    rt6->rt6i_dst.plen == 128) {
				rt6->rt6i_flags |= RTF_MODIFIED;
				dst_metric_set(skb_dst(skb), RTAX_MTU, mtu);
			}
		}

		if (!skb_is_gso(skb) && mtu >= IPV6_MIN_MTU &&
					mtu < pkt_size) {
			icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu);
			return -E2BIG;
		}
	}
#endif
	return 0;
}

<<<<<<< HEAD
void ip_tunnel_xmit(struct sk_buff *skb, struct net_device *dev,
		    const struct iphdr *tnl_params)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	const struct iphdr *inner_iph;
	struct iphdr *iph;
=======
void ip_md_tunnel_xmit(struct sk_buff *skb, struct net_device *dev, u8 proto)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	u32 headroom = sizeof(struct iphdr);
	struct ip_tunnel_info *tun_info;
	const struct ip_tunnel_key *key;
	const struct iphdr *inner_iph;
	struct rtable *rt;
	struct flowi4 fl4;
	__be16 df = 0;
	u8 tos, ttl;

	tun_info = skb_tunnel_info(skb);
	if (unlikely(!tun_info || !(tun_info->mode & IP_TUNNEL_INFO_TX) ||
		     ip_tunnel_info_af(tun_info) != AF_INET))
		goto tx_error;
	key = &tun_info->key;
	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
	inner_iph = (const struct iphdr *)skb_inner_network_header(skb);
	tos = key->tos;
	if (tos == 1) {
		if (skb->protocol == htons(ETH_P_IP))
			tos = inner_iph->tos;
		else if (skb->protocol == htons(ETH_P_IPV6))
			tos = ipv6_get_dsfield((const struct ipv6hdr *)inner_iph);
	}
	init_tunnel_flow(&fl4, proto, key->u.ipv4.dst, key->u.ipv4.src, 0,
			 RT_TOS(tos), tunnel->parms.link);
	if (tunnel->encap.type != TUNNEL_ENCAP_NONE)
		goto tx_error;
	rt = ip_route_output_key(tunnel->net, &fl4);
	if (IS_ERR(rt)) {
		dev->stats.tx_carrier_errors++;
		goto tx_error;
	}
	if (rt->dst.dev == dev) {
		ip_rt_put(rt);
		dev->stats.collisions++;
		goto tx_error;
	}
	tos = ip_tunnel_ecn_encap(tos, inner_iph, skb);
	ttl = key->ttl;
	if (ttl == 0) {
		if (skb->protocol == htons(ETH_P_IP))
			ttl = inner_iph->ttl;
		else if (skb->protocol == htons(ETH_P_IPV6))
			ttl = ((const struct ipv6hdr *)inner_iph)->hop_limit;
		else
			ttl = ip4_dst_hoplimit(&rt->dst);
	}
	if (key->tun_flags & TUNNEL_DONT_FRAGMENT)
		df = htons(IP_DF);
	else if (skb->protocol == htons(ETH_P_IP))
		df = inner_iph->frag_off & htons(IP_DF);
	headroom += LL_RESERVED_SPACE(rt->dst.dev) + rt->dst.header_len;
	if (headroom > dev->needed_headroom)
		dev->needed_headroom = headroom;

	if (skb_cow_head(skb, dev->needed_headroom)) {
		ip_rt_put(rt);
		goto tx_dropped;
	}
	iptunnel_xmit(NULL, rt, skb, fl4.saddr, fl4.daddr, proto, tos, ttl,
		      df, !net_eq(tunnel->net, dev_net(dev)));
	return;
tx_error:
	dev->stats.tx_errors++;
	goto kfree;
tx_dropped:
	dev->stats.tx_dropped++;
kfree:
	kfree_skb(skb);
}
EXPORT_SYMBOL_GPL(ip_md_tunnel_xmit);

void ip_tunnel_xmit(struct sk_buff *skb, struct net_device *dev,
		    const struct iphdr *tnl_params, u8 protocol)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	unsigned int inner_nhdr_len = 0;
	const struct iphdr *inner_iph;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct flowi4 fl4;
	u8     tos, ttl;
	__be16 df;
	struct rtable *rt;		/* Route to the other host */
<<<<<<< HEAD
	struct net_device *tdev;	/* Device to other host */
	unsigned int max_headroom;	/* The extra header space needed */
	__be32 dst;

	inner_iph = (const struct iphdr *)skb_inner_network_header(skb);

	memset(IPCB(skb), 0, sizeof(*IPCB(skb)));
	dst = tnl_params->daddr;
	if (dst == 0) {
		/* NBMA tunnel */

		if (skb_dst(skb) == NULL) {
=======
	unsigned int max_headroom;	/* The extra header space needed */
	__be32 dst;
	bool connected;

	/* ensure we can access the inner net header, for several users below */
	if (skb->protocol == htons(ETH_P_IP))
		inner_nhdr_len = sizeof(struct iphdr);
	else if (skb->protocol == htons(ETH_P_IPV6))
		inner_nhdr_len = sizeof(struct ipv6hdr);
	if (unlikely(!pskb_may_pull(skb, inner_nhdr_len)))
		goto tx_error;

	inner_iph = (const struct iphdr *)skb_inner_network_header(skb);
	connected = (tunnel->parms.iph.daddr != 0);

	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));

	dst = tnl_params->daddr;
	if (dst == 0) {
		/* NBMA tunnel */
		struct ip_tunnel_info *tun_info;

		if (!skb_dst(skb)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			dev->stats.tx_fifo_errors++;
			goto tx_error;
		}

<<<<<<< HEAD
		if (skb->protocol == htons(ETH_P_IP)) {
=======
		tun_info = skb_tunnel_info(skb);
		if (tun_info && (tun_info->mode & IP_TUNNEL_INFO_TX) &&
		    ip_tunnel_info_af(tun_info) == AF_INET &&
		    tun_info->key.u.ipv4.dst)
			dst = tun_info->key.u.ipv4.dst;
		else if (skb->protocol == htons(ETH_P_IP)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			rt = skb_rtable(skb);
			dst = rt_nexthop(rt, inner_iph->daddr);
		}
#if IS_ENABLED(CONFIG_IPV6)
		else if (skb->protocol == htons(ETH_P_IPV6)) {
			const struct in6_addr *addr6;
			struct neighbour *neigh;
			bool do_tx_error_icmp;
			int addr_type;

			neigh = dst_neigh_lookup(skb_dst(skb),
						 &ipv6_hdr(skb)->daddr);
<<<<<<< HEAD
			if (neigh == NULL)
=======
			if (!neigh)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				goto tx_error;

			addr6 = (const struct in6_addr *)&neigh->primary_key;
			addr_type = ipv6_addr_type(addr6);

			if (addr_type == IPV6_ADDR_ANY) {
				addr6 = &ipv6_hdr(skb)->daddr;
				addr_type = ipv6_addr_type(addr6);
			}

			if ((addr_type & IPV6_ADDR_COMPATv4) == 0)
				do_tx_error_icmp = true;
			else {
				do_tx_error_icmp = false;
				dst = addr6->s6_addr32[3];
			}
			neigh_release(neigh);
			if (do_tx_error_icmp)
				goto tx_error_icmp;
		}
#endif
		else
			goto tx_error;
<<<<<<< HEAD
=======

		connected = false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	tos = tnl_params->tos;
	if (tos & 0x1) {
		tos &= ~0x1;
<<<<<<< HEAD
		if (skb->protocol == htons(ETH_P_IP))
			tos = inner_iph->tos;
		else if (skb->protocol == htons(ETH_P_IPV6))
			tos = ipv6_get_dsfield((const struct ipv6hdr *)inner_iph);
	}

	rt = ip_route_output_tunnel(dev_net(dev), &fl4,
				    tunnel->parms.iph.protocol,
				    dst, tnl_params->saddr,
				    tunnel->parms.o_key,
				    RT_TOS(tos),
				    tunnel->parms.link);
	if (IS_ERR(rt)) {
		dev->stats.tx_carrier_errors++;
		goto tx_error;
	}
	tdev = rt->dst.dev;

	if (tdev == dev) {
=======
		if (skb->protocol == htons(ETH_P_IP)) {
			tos = inner_iph->tos;
			connected = false;
		} else if (skb->protocol == htons(ETH_P_IPV6)) {
			tos = ipv6_get_dsfield((const struct ipv6hdr *)inner_iph);
			connected = false;
		}
	}

	init_tunnel_flow(&fl4, protocol, dst, tnl_params->saddr,
			 tunnel->parms.o_key, RT_TOS(tos), tunnel->parms.link);

	if (ip_tunnel_encap(skb, tunnel, &protocol, &fl4) < 0)
		goto tx_error;

	rt = connected ? dst_cache_get_ip4(&tunnel->dst_cache, &fl4.saddr) :
			 NULL;

	if (!rt) {
		rt = ip_route_output_key(tunnel->net, &fl4);

		if (IS_ERR(rt)) {
			dev->stats.tx_carrier_errors++;
			goto tx_error;
		}
		if (connected)
			dst_cache_set_ip4(&tunnel->dst_cache, &rt->dst,
					  fl4.saddr);
	}

	if (rt->dst.dev == dev) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ip_rt_put(rt);
		dev->stats.collisions++;
		goto tx_error;
	}

<<<<<<< HEAD

	if (tnl_update_pmtu(dev, skb, rt, tnl_params->frag_off)) {
=======
	df = tnl_params->frag_off;
	if (skb->protocol == htons(ETH_P_IP) && !tunnel->ignore_df)
		df |= (inner_iph->frag_off & htons(IP_DF));

	if (tnl_update_pmtu(dev, skb, rt, df, inner_iph)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ip_rt_put(rt);
		goto tx_error;
	}

	if (tunnel->err_count > 0) {
		if (time_before(jiffies,
				tunnel->err_time + IPTUNNEL_ERR_TIMEO)) {
			tunnel->err_count--;

<<<<<<< HEAD
			memset(IPCB(skb), 0, sizeof(*IPCB(skb)));
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			dst_link_failure(skb);
		} else
			tunnel->err_count = 0;
	}

<<<<<<< HEAD
=======
	tos = ip_tunnel_ecn_encap(tos, inner_iph, skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ttl = tnl_params->ttl;
	if (ttl == 0) {
		if (skb->protocol == htons(ETH_P_IP))
			ttl = inner_iph->ttl;
#if IS_ENABLED(CONFIG_IPV6)
		else if (skb->protocol == htons(ETH_P_IPV6))
			ttl = ((const struct ipv6hdr *)inner_iph)->hop_limit;
#endif
		else
			ttl = ip4_dst_hoplimit(&rt->dst);
	}

<<<<<<< HEAD
	df = tnl_params->frag_off;
	if (skb->protocol == htons(ETH_P_IP))
		df |= (inner_iph->frag_off&htons(IP_DF));

	max_headroom = LL_RESERVED_SPACE(tdev) + sizeof(struct iphdr)
					       + rt->dst.header_len;
=======
	max_headroom = LL_RESERVED_SPACE(rt->dst.dev) + sizeof(struct iphdr)
			+ rt->dst.header_len + ip_encap_hlen(&tunnel->encap);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (max_headroom > dev->needed_headroom)
		dev->needed_headroom = max_headroom;

	if (skb_cow_head(skb, dev->needed_headroom)) {
<<<<<<< HEAD
		dev->stats.tx_dropped++;
		dev_kfree_skb(skb);
		return;
	}

	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);

	/* Push down and install the IP header. */
	skb_push(skb, sizeof(struct iphdr));
	skb_reset_network_header(skb);

	iph = ip_hdr(skb);
	inner_iph = (const struct iphdr *)skb_inner_network_header(skb);

	iph->version	=	4;
	iph->ihl	=	sizeof(struct iphdr) >> 2;
	iph->frag_off	=	df;
	iph->protocol	=	tnl_params->protocol;
	iph->tos	=	ip_tunnel_ecn_encap(tos, inner_iph, skb);
	iph->daddr	=	fl4.daddr;
	iph->saddr	=	fl4.saddr;
	iph->ttl	=	ttl;
	__ip_select_ident(iph, skb_shinfo(skb)->gso_segs ?: 1);

	iptunnel_xmit(skb, dev);
=======
		ip_rt_put(rt);
		dev->stats.tx_dropped++;
		kfree_skb(skb);
		return;
	}

	iptunnel_xmit(NULL, rt, skb, fl4.saddr, fl4.daddr, protocol, tos, ttl,
		      df, !net_eq(tunnel->net, dev_net(dev)));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return;

#if IS_ENABLED(CONFIG_IPV6)
tx_error_icmp:
	dst_link_failure(skb);
#endif
tx_error:
	dev->stats.tx_errors++;
<<<<<<< HEAD
	dev_kfree_skb(skb);
=======
	kfree_skb(skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(ip_tunnel_xmit);

static void ip_tunnel_update(struct ip_tunnel_net *itn,
			     struct ip_tunnel *t,
			     struct net_device *dev,
			     struct ip_tunnel_parm *p,
			     bool set_mtu)
{
<<<<<<< HEAD
	ip_tunnel_del(t);
=======
	ip_tunnel_del(itn, t);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	t->parms.iph.saddr = p->iph.saddr;
	t->parms.iph.daddr = p->iph.daddr;
	t->parms.i_key = p->i_key;
	t->parms.o_key = p->o_key;
	if (dev->type != ARPHRD_ETHER) {
		memcpy(dev->dev_addr, &p->iph.saddr, 4);
		memcpy(dev->broadcast, &p->iph.daddr, 4);
	}
	ip_tunnel_add(itn, t);

	t->parms.iph.ttl = p->iph.ttl;
	t->parms.iph.tos = p->iph.tos;
	t->parms.iph.frag_off = p->iph.frag_off;

	if (t->parms.link != p->link) {
		int mtu;

		t->parms.link = p->link;
		mtu = ip_tunnel_bind_dev(dev);
		if (set_mtu)
			dev->mtu = mtu;
	}
<<<<<<< HEAD
=======
	dst_cache_reset(&t->dst_cache);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	netdev_state_change(dev);
}

int ip_tunnel_ioctl(struct net_device *dev, struct ip_tunnel_parm *p, int cmd)
{
	int err = 0;
<<<<<<< HEAD
	struct ip_tunnel *t;
	struct net *net = dev_net(dev);
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct ip_tunnel_net *itn = net_generic(net, tunnel->ip_tnl_net_id);
=======
	struct ip_tunnel *t = netdev_priv(dev);
	struct net *net = t->net;
	struct ip_tunnel_net *itn = net_generic(net, t->ip_tnl_net_id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	BUG_ON(!itn->fb_tunnel_dev);
	switch (cmd) {
	case SIOCGETTUNNEL:
<<<<<<< HEAD
		t = NULL;
		if (dev == itn->fb_tunnel_dev)
			t = ip_tunnel_find(itn, p, itn->fb_tunnel_dev->type);
		if (t == NULL)
			t = netdev_priv(dev);
=======
		if (dev == itn->fb_tunnel_dev) {
			t = ip_tunnel_find(itn, p, itn->fb_tunnel_dev->type);
			if (!t)
				t = netdev_priv(dev);
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		memcpy(p, &t->parms, sizeof(*p));
		break;

	case SIOCADDTUNNEL:
	case SIOCCHGTUNNEL:
		err = -EPERM;
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			goto done;
		if (p->iph.ttl)
			p->iph.frag_off |= htons(IP_DF);
<<<<<<< HEAD
		if (!(p->i_flags&TUNNEL_KEY))
			p->i_key = 0;
		if (!(p->o_flags&TUNNEL_KEY))
			p->o_key = 0;

		t = ip_tunnel_find(itn, p, itn->fb_tunnel_dev->type);

		if (!t && (cmd == SIOCADDTUNNEL))
			t = ip_tunnel_create(net, itn, p);

		if (dev != itn->fb_tunnel_dev && cmd == SIOCCHGTUNNEL) {
			if (t != NULL) {
=======
		if (!(p->i_flags & VTI_ISVTI)) {
			if (!(p->i_flags & TUNNEL_KEY))
				p->i_key = 0;
			if (!(p->o_flags & TUNNEL_KEY))
				p->o_key = 0;
		}

		t = ip_tunnel_find(itn, p, itn->fb_tunnel_dev->type);

		if (cmd == SIOCADDTUNNEL) {
			if (!t) {
				t = ip_tunnel_create(net, itn, p);
				err = PTR_ERR_OR_ZERO(t);
				break;
			}

			err = -EEXIST;
			break;
		}
		if (dev != itn->fb_tunnel_dev && cmd == SIOCCHGTUNNEL) {
			if (t) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				if (t->dev != dev) {
					err = -EEXIST;
					break;
				}
			} else {
				unsigned int nflags = 0;

				if (ipv4_is_multicast(p->iph.daddr))
					nflags = IFF_BROADCAST;
				else if (p->iph.daddr)
					nflags = IFF_POINTOPOINT;

				if ((dev->flags^nflags)&(IFF_POINTOPOINT|IFF_BROADCAST)) {
					err = -EINVAL;
					break;
				}

				t = netdev_priv(dev);
			}
		}

		if (t) {
			err = 0;
			ip_tunnel_update(itn, t, dev, p, true);
<<<<<<< HEAD
		} else
			err = (cmd == SIOCADDTUNNEL ? -ENOBUFS : -ENOENT);
=======
		} else {
			err = -ENOENT;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;

	case SIOCDELTUNNEL:
		err = -EPERM;
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			goto done;

		if (dev == itn->fb_tunnel_dev) {
			err = -ENOENT;
			t = ip_tunnel_find(itn, p, itn->fb_tunnel_dev->type);
<<<<<<< HEAD
			if (t == NULL)
=======
			if (!t)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				goto done;
			err = -EPERM;
			if (t == netdev_priv(itn->fb_tunnel_dev))
				goto done;
			dev = t->dev;
		}
		unregister_netdevice(dev);
		err = 0;
		break;

	default:
		err = -EINVAL;
	}

done:
	return err;
}
EXPORT_SYMBOL_GPL(ip_tunnel_ioctl);

<<<<<<< HEAD
int ip_tunnel_change_mtu(struct net_device *dev, int new_mtu)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	int t_hlen = tunnel->hlen + sizeof(struct iphdr);

	if (new_mtu < 68 ||
	    new_mtu > 0xFFF8 - dev->hard_header_len - t_hlen)
		return -EINVAL;
	dev->mtu = new_mtu;
	return 0;
}
=======
int __ip_tunnel_change_mtu(struct net_device *dev, int new_mtu, bool strict)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	int t_hlen = tunnel->hlen + sizeof(struct iphdr);
	int max_mtu = 0xFFF8 - dev->hard_header_len - t_hlen;

	if (new_mtu < 68)
		return -EINVAL;

	if (new_mtu > max_mtu) {
		if (strict)
			return -EINVAL;

		new_mtu = max_mtu;
	}

	dev->mtu = new_mtu;
	return 0;
}
EXPORT_SYMBOL_GPL(__ip_tunnel_change_mtu);

int ip_tunnel_change_mtu(struct net_device *dev, int new_mtu)
{
	return __ip_tunnel_change_mtu(dev, new_mtu, true);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL_GPL(ip_tunnel_change_mtu);

static void ip_tunnel_dev_free(struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);

	gro_cells_destroy(&tunnel->gro_cells);
<<<<<<< HEAD
=======
	dst_cache_destroy(&tunnel->dst_cache);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	free_percpu(dev->tstats);
	free_netdev(dev);
}

void ip_tunnel_dellink(struct net_device *dev, struct list_head *head)
{
<<<<<<< HEAD
	struct net *net = dev_net(dev);
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct ip_tunnel_net *itn;

	itn = net_generic(net, tunnel->ip_tnl_net_id);

	if (itn->fb_tunnel_dev != dev) {
		ip_tunnel_del(netdev_priv(dev));
=======
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct ip_tunnel_net *itn;

	itn = net_generic(tunnel->net, tunnel->ip_tnl_net_id);

	if (itn->fb_tunnel_dev != dev) {
		ip_tunnel_del(itn, netdev_priv(dev));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		unregister_netdevice_queue(dev, head);
	}
}
EXPORT_SYMBOL_GPL(ip_tunnel_dellink);

<<<<<<< HEAD
=======
struct net *ip_tunnel_get_link_net(const struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);

	return tunnel->net;
}
EXPORT_SYMBOL(ip_tunnel_get_link_net);

int ip_tunnel_get_iflink(const struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);

	return tunnel->parms.link;
}
EXPORT_SYMBOL(ip_tunnel_get_iflink);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int ip_tunnel_init_net(struct net *net, int ip_tnl_net_id,
				  struct rtnl_link_ops *ops, char *devname)
{
	struct ip_tunnel_net *itn = net_generic(net, ip_tnl_net_id);
	struct ip_tunnel_parm parms;
<<<<<<< HEAD

	itn->tunnels = kzalloc(IP_TNL_HASH_SIZE * sizeof(struct hlist_head), GFP_KERNEL);
	if (!itn->tunnels)
		return -ENOMEM;
=======
	unsigned int i;

	for (i = 0; i < IP_TNL_HASH_SIZE; i++)
		INIT_HLIST_HEAD(&itn->tunnels[i]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!ops) {
		itn->fb_tunnel_dev = NULL;
		return 0;
	}
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	memset(&parms, 0, sizeof(parms));
	if (devname)
		strlcpy(parms.name, devname, IFNAMSIZ);

	rtnl_lock();
	itn->fb_tunnel_dev = __ip_tunnel_create(net, ops, &parms);
<<<<<<< HEAD
	rtnl_unlock();
	if (IS_ERR(itn->fb_tunnel_dev)) {
		kfree(itn->tunnels);
		return PTR_ERR(itn->fb_tunnel_dev);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(ip_tunnel_init_net);

static void ip_tunnel_destroy(struct ip_tunnel_net *itn, struct list_head *head)
{
	int h;

=======
	/* FB netdevice is special: we have one, and only one per netns.
	 * Allowing to move it to another netns is clearly unsafe.
	 */
	if (!IS_ERR(itn->fb_tunnel_dev)) {
		itn->fb_tunnel_dev->features |= NETIF_F_NETNS_LOCAL;
		itn->fb_tunnel_dev->mtu = ip_tunnel_bind_dev(itn->fb_tunnel_dev);
		ip_tunnel_add(itn, netdev_priv(itn->fb_tunnel_dev));
	}
	rtnl_unlock();

	return PTR_ERR_OR_ZERO(itn->fb_tunnel_dev);
}
EXPORT_SYMBOL_GPL(ip_tunnel_init_net);

static void ip_tunnel_destroy(struct ip_tunnel_net *itn, struct list_head *head,
			      struct rtnl_link_ops *ops)
{
	struct net *net = dev_net(itn->fb_tunnel_dev);
	struct net_device *dev, *aux;
	int h;

	for_each_netdev_safe(net, dev, aux)
		if (dev->rtnl_link_ops == ops)
			unregister_netdevice_queue(dev, head);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	for (h = 0; h < IP_TNL_HASH_SIZE; h++) {
		struct ip_tunnel *t;
		struct hlist_node *n;
		struct hlist_head *thead = &itn->tunnels[h];

		hlist_for_each_entry_safe(t, n, thead, hash_node)
<<<<<<< HEAD
			unregister_netdevice_queue(t->dev, head);
	}
	if (itn->fb_tunnel_dev)
		unregister_netdevice_queue(itn->fb_tunnel_dev, head);
}

void ip_tunnel_delete_net(struct ip_tunnel_net *itn)
=======
			/* If dev is in the same netns, it has already
			 * been added to the list by the previous loop.
			 */
			if (!net_eq(dev_net(t->dev), net))
				unregister_netdevice_queue(t->dev, head);
	}
}

void ip_tunnel_delete_net(struct ip_tunnel_net *itn, struct rtnl_link_ops *ops)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	LIST_HEAD(list);

	rtnl_lock();
<<<<<<< HEAD
	ip_tunnel_destroy(itn, &list);
	unregister_netdevice_many(&list);
	rtnl_unlock();
	kfree(itn->tunnels);
=======
	ip_tunnel_destroy(itn, &list, ops);
	unregister_netdevice_many(&list);
	rtnl_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(ip_tunnel_delete_net);

int ip_tunnel_newlink(struct net_device *dev, struct nlattr *tb[],
		      struct ip_tunnel_parm *p)
{
	struct ip_tunnel *nt;
	struct net *net = dev_net(dev);
	struct ip_tunnel_net *itn;
	int mtu;
	int err;

	nt = netdev_priv(dev);
	itn = net_generic(net, nt->ip_tnl_net_id);

<<<<<<< HEAD
	if (ip_tunnel_find(itn, p, dev->type))
		return -EEXIST;

=======
	if (nt->collect_md) {
		if (rtnl_dereference(itn->collect_md_tun))
			return -EEXIST;
	} else {
		if (ip_tunnel_find(itn, p, dev->type))
			return -EEXIST;
	}

	nt->net = net;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	nt->parms = *p;
	err = register_netdevice(dev);
	if (err)
		goto out;

	if (dev->type == ARPHRD_ETHER && !tb[IFLA_ADDRESS])
		eth_hw_addr_random(dev);

	mtu = ip_tunnel_bind_dev(dev);
	if (!tb[IFLA_MTU])
		dev->mtu = mtu;

	ip_tunnel_add(itn, nt);
<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out:
	return err;
}
EXPORT_SYMBOL_GPL(ip_tunnel_newlink);

int ip_tunnel_changelink(struct net_device *dev, struct nlattr *tb[],
			 struct ip_tunnel_parm *p)
{
<<<<<<< HEAD
	struct ip_tunnel *t, *nt;
	struct net *net = dev_net(dev);
	struct ip_tunnel *tunnel = netdev_priv(dev);
=======
	struct ip_tunnel *t;
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct net *net = tunnel->net;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct ip_tunnel_net *itn = net_generic(net, tunnel->ip_tnl_net_id);

	if (dev == itn->fb_tunnel_dev)
		return -EINVAL;

<<<<<<< HEAD
	nt = netdev_priv(dev);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	t = ip_tunnel_find(itn, p, dev->type);

	if (t) {
		if (t->dev != dev)
			return -EEXIST;
	} else {
<<<<<<< HEAD
		t = nt;
=======
		t = tunnel;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (dev->type != ARPHRD_ETHER) {
			unsigned int nflags = 0;

			if (ipv4_is_multicast(p->iph.daddr))
				nflags = IFF_BROADCAST;
			else if (p->iph.daddr)
				nflags = IFF_POINTOPOINT;

			if ((dev->flags ^ nflags) &
			    (IFF_POINTOPOINT | IFF_BROADCAST))
				return -EINVAL;
		}
	}

	ip_tunnel_update(itn, t, dev, p, !tb[IFLA_MTU]);
	return 0;
}
EXPORT_SYMBOL_GPL(ip_tunnel_changelink);

int ip_tunnel_init(struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct iphdr *iph = &tunnel->parms.iph;
	int err;

	dev->destructor	= ip_tunnel_dev_free;
<<<<<<< HEAD
	dev->tstats = alloc_percpu(struct pcpu_tstats);
	if (!dev->tstats)
		return -ENOMEM;

	err = gro_cells_init(&tunnel->gro_cells, dev);
	if (err) {
=======
	dev->tstats = netdev_alloc_pcpu_stats(struct pcpu_sw_netstats);
	if (!dev->tstats)
		return -ENOMEM;

	err = dst_cache_init(&tunnel->dst_cache, GFP_KERNEL);
	if (err) {
		free_percpu(dev->tstats);
		return err;
	}

	err = gro_cells_init(&tunnel->gro_cells, dev);
	if (err) {
		dst_cache_destroy(&tunnel->dst_cache);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		free_percpu(dev->tstats);
		return err;
	}

	tunnel->dev = dev;
<<<<<<< HEAD
=======
	tunnel->net = dev_net(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	strcpy(tunnel->parms.name, dev->name);
	iph->version		= 4;
	iph->ihl		= 5;

<<<<<<< HEAD
=======
	if (tunnel->collect_md)
		netif_keep_dst(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}
EXPORT_SYMBOL_GPL(ip_tunnel_init);

void ip_tunnel_uninit(struct net_device *dev)
{
<<<<<<< HEAD
	struct net *net = dev_net(dev);
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct ip_tunnel_net *itn;

	itn = net_generic(net, tunnel->ip_tnl_net_id);
	/* fb_tunnel_dev will be unregisted in net-exit call. */
	if (itn->fb_tunnel_dev != dev)
		ip_tunnel_del(netdev_priv(dev));
=======
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct net *net = tunnel->net;
	struct ip_tunnel_net *itn;

	itn = net_generic(net, tunnel->ip_tnl_net_id);
	ip_tunnel_del(itn, netdev_priv(dev));
	if (itn->fb_tunnel_dev == dev)
		WRITE_ONCE(itn->fb_tunnel_dev, NULL);

	dst_cache_reset(&tunnel->dst_cache);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(ip_tunnel_uninit);

/* Do least required initialization, rest of init is done in tunnel_init call */
void ip_tunnel_setup(struct net_device *dev, int net_id)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	tunnel->ip_tnl_net_id = net_id;
}
EXPORT_SYMBOL_GPL(ip_tunnel_setup);

MODULE_LICENSE("GPL");
