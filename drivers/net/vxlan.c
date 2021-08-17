/*
 * VXLAN: Virtual eXtensible Local Area Network
 *
 * Copyright (c) 2012-2013 Vyatta Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
<<<<<<< HEAD
 *
 * TODO
 *  - IPv6 (not in RFC)
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/types.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/rculist.h>
#include <linux/netdevice.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/igmp.h>
#include <linux/etherdevice.h>
#include <linux/if_ether.h>
#include <linux/hash.h>
=======
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/udp.h>
#include <linux/igmp.h>
#include <linux/if_ether.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/ethtool.h>
#include <net/arp.h>
#include <net/ndisc.h>
#include <net/ip.h>
<<<<<<< HEAD
#include <net/ip_tunnels.h>
#include <net/icmp.h>
#include <net/udp.h>
#include <net/rtnetlink.h>
#include <net/route.h>
#include <net/dsfield.h>
#include <net/inet_ecn.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>

#define VXLAN_VERSION	"0.1"

#define VNI_HASH_BITS	10
#define VNI_HASH_SIZE	(1<<VNI_HASH_BITS)
#define FDB_HASH_BITS	8
#define FDB_HASH_SIZE	(1<<FDB_HASH_BITS)
#define FDB_AGE_DEFAULT 300 /* 5 min */
#define FDB_AGE_INTERVAL (10 * HZ)	/* rescan interval */

#define VXLAN_N_VID	(1u << 24)
#define VXLAN_VID_MASK	(VXLAN_N_VID - 1)
/* IP header + UDP + VXLAN + Ethernet header */
#define VXLAN_HEADROOM (20 + 8 + 8 + 14)

#define VXLAN_FLAGS 0x08000000	/* struct vxlanhdr.vx_flags required value. */

/* VXLAN protocol header */
struct vxlanhdr {
	__be32 vx_flags;
	__be32 vx_vni;
};

/* UDP port for VXLAN traffic.
 * The IANA assigned port is 4789, but the Linux default is 8472
 * for compatability with early adopters.
 */
static unsigned int vxlan_port __read_mostly = 8472;
module_param_named(udp_port, vxlan_port, uint, 0444);
=======
#include <net/icmp.h>
#include <net/rtnetlink.h>
#include <net/inet_ecn.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
#include <net/vxlan.h>

#if IS_ENABLED(CONFIG_IPV6)
#include <net/ip6_tunnel.h>
#include <net/ip6_checksum.h>
#endif

#define VXLAN_VERSION	"0.1"

#define PORT_HASH_BITS	8
#define PORT_HASH_SIZE  (1<<PORT_HASH_BITS)
#define FDB_AGE_DEFAULT 300 /* 5 min */
#define FDB_AGE_INTERVAL (10 * HZ)	/* rescan interval */

/* UDP port for VXLAN traffic.
 * The IANA assigned port is 4789, but the Linux default is 8472
 * for compatibility with early adopters.
 */
static unsigned short vxlan_port __read_mostly = 8472;
module_param_named(udp_port, vxlan_port, ushort, 0444);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_PARM_DESC(udp_port, "Destination UDP port");

static bool log_ecn_error = true;
module_param(log_ecn_error, bool, 0644);
MODULE_PARM_DESC(log_ecn_error, "Log packets received with corrupted ECN");

<<<<<<< HEAD
/* per-net private data for this module */
static unsigned int vxlan_net_id;
struct vxlan_net {
	struct socket	  *sock;	/* UDP encap socket */
	struct hlist_head vni_list[VNI_HASH_SIZE];
};

struct vxlan_rdst {
	struct rcu_head		 rcu;
	__be32			 remote_ip;
	__be16			 remote_port;
	u32			 remote_vni;
	u32			 remote_ifindex;
	struct vxlan_rdst	*remote_next;
=======
static int vxlan_net_id;
static struct rtnl_link_ops vxlan_link_ops;

static const u8 all_zeros_mac[ETH_ALEN + 2];

static int vxlan_sock_add(struct vxlan_dev *vxlan);

static void vxlan_vs_del_dev(struct vxlan_dev *vxlan);

/* per-network namespace private data for this module */
struct vxlan_net {
	struct list_head  vxlan_list;
	struct hlist_head sock_list[PORT_HASH_SIZE];
	spinlock_t	  sock_lock;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* Forwarding table entry */
struct vxlan_fdb {
	struct hlist_node hlist;	/* linked list of entries */
	struct rcu_head	  rcu;
	unsigned long	  updated;	/* jiffies */
	unsigned long	  used;
<<<<<<< HEAD
	struct vxlan_rdst remote;
	u16		  state;	/* see ndm_state */
	u8		  flags;	/* see ndm_flags */
	u8		  eth_addr[ETH_ALEN];
};

/* Pseudo network device */
struct vxlan_dev {
	struct hlist_node hlist;
	struct net_device *dev;
	struct vxlan_rdst default_dst;	/* default destination */
	__be32		  saddr;	/* source address */
	__be16		  dst_port;
	__u16		  port_min;	/* source port range */
	__u16		  port_max;
	__u8		  tos;		/* TOS override */
	__u8		  ttl;
	u32		  flags;	/* VXLAN_F_* below */

	unsigned long	  age_interval;
	struct timer_list age_timer;
	spinlock_t	  hash_lock;
	unsigned int	  addrcnt;
	unsigned int	  addrmax;

	struct hlist_head fdb_head[FDB_HASH_SIZE];
};

#define VXLAN_F_LEARN	0x01
#define VXLAN_F_PROXY	0x02
#define VXLAN_F_RSC	0x04
#define VXLAN_F_L2MISS	0x08
#define VXLAN_F_L3MISS	0x10

/* salt for hash table */
static u32 vxlan_salt __read_mostly;

static inline struct hlist_head *vni_head(struct net *net, u32 id)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);

	return &vn->vni_list[hash_32(id, VNI_HASH_BITS)];
}

/* Look up VNI in a per net namespace table */
static struct vxlan_dev *vxlan_find_vni(struct net *net, u32 id)
{
	struct vxlan_dev *vxlan;

	hlist_for_each_entry_rcu(vxlan, vni_head(net, id), hlist) {
		if (vxlan->default_dst.remote_vni == id)
			return vxlan;
=======
	struct list_head  remotes;
	u8		  eth_addr[ETH_ALEN];
	u16		  state;	/* see ndm_state */
	u8		  flags;	/* see ndm_flags */
};

/* salt for hash table */
static u32 vxlan_salt __read_mostly;

static inline bool vxlan_collect_metadata(struct vxlan_sock *vs)
{
	return vs->flags & VXLAN_F_COLLECT_METADATA ||
	       ip_tunnel_collect_metadata();
}

#if IS_ENABLED(CONFIG_IPV6)
static inline
bool vxlan_addr_equal(const union vxlan_addr *a, const union vxlan_addr *b)
{
	if (a->sa.sa_family != b->sa.sa_family)
		return false;
	if (a->sa.sa_family == AF_INET6)
		return ipv6_addr_equal(&a->sin6.sin6_addr, &b->sin6.sin6_addr);
	else
		return a->sin.sin_addr.s_addr == b->sin.sin_addr.s_addr;
}

static inline bool vxlan_addr_any(const union vxlan_addr *ipa)
{
	if (ipa->sa.sa_family == AF_INET6)
		return ipv6_addr_any(&ipa->sin6.sin6_addr);
	else
		return ipa->sin.sin_addr.s_addr == htonl(INADDR_ANY);
}

static inline bool vxlan_addr_multicast(const union vxlan_addr *ipa)
{
	if (ipa->sa.sa_family == AF_INET6)
		return ipv6_addr_is_multicast(&ipa->sin6.sin6_addr);
	else
		return IN_MULTICAST(ntohl(ipa->sin.sin_addr.s_addr));
}

static int vxlan_nla_get_addr(union vxlan_addr *ip, struct nlattr *nla)
{
	if (nla_len(nla) >= sizeof(struct in6_addr)) {
		ip->sin6.sin6_addr = nla_get_in6_addr(nla);
		ip->sa.sa_family = AF_INET6;
		return 0;
	} else if (nla_len(nla) >= sizeof(__be32)) {
		ip->sin.sin_addr.s_addr = nla_get_in_addr(nla);
		ip->sa.sa_family = AF_INET;
		return 0;
	} else {
		return -EAFNOSUPPORT;
	}
}

static int vxlan_nla_put_addr(struct sk_buff *skb, int attr,
			      const union vxlan_addr *ip)
{
	if (ip->sa.sa_family == AF_INET6)
		return nla_put_in6_addr(skb, attr, &ip->sin6.sin6_addr);
	else
		return nla_put_in_addr(skb, attr, ip->sin.sin_addr.s_addr);
}

#else /* !CONFIG_IPV6 */

static inline
bool vxlan_addr_equal(const union vxlan_addr *a, const union vxlan_addr *b)
{
	return a->sin.sin_addr.s_addr == b->sin.sin_addr.s_addr;
}

static inline bool vxlan_addr_any(const union vxlan_addr *ipa)
{
	return ipa->sin.sin_addr.s_addr == htonl(INADDR_ANY);
}

static inline bool vxlan_addr_multicast(const union vxlan_addr *ipa)
{
	return IN_MULTICAST(ntohl(ipa->sin.sin_addr.s_addr));
}

static int vxlan_nla_get_addr(union vxlan_addr *ip, struct nlattr *nla)
{
	if (nla_len(nla) >= sizeof(struct in6_addr)) {
		return -EAFNOSUPPORT;
	} else if (nla_len(nla) >= sizeof(__be32)) {
		ip->sin.sin_addr.s_addr = nla_get_in_addr(nla);
		ip->sa.sa_family = AF_INET;
		return 0;
	} else {
		return -EAFNOSUPPORT;
	}
}

static int vxlan_nla_put_addr(struct sk_buff *skb, int attr,
			      const union vxlan_addr *ip)
{
	return nla_put_in_addr(skb, attr, ip->sin.sin_addr.s_addr);
}
#endif

/* Virtual Network hash table head */
static inline struct hlist_head *vni_head(struct vxlan_sock *vs, __be32 vni)
{
	return &vs->vni_list[hash_32((__force u32)vni, VNI_HASH_BITS)];
}

/* Socket hash table head */
static inline struct hlist_head *vs_head(struct net *net, __be16 port)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);

	return &vn->sock_list[hash_32(ntohs(port), PORT_HASH_BITS)];
}

/* First remote destination for a forwarding entry.
 * Guaranteed to be non-NULL because remotes are never deleted.
 */
static inline struct vxlan_rdst *first_remote_rcu(struct vxlan_fdb *fdb)
{
	return list_entry_rcu(fdb->remotes.next, struct vxlan_rdst, list);
}

static inline struct vxlan_rdst *first_remote_rtnl(struct vxlan_fdb *fdb)
{
	return list_first_entry(&fdb->remotes, struct vxlan_rdst, list);
}

/* Find VXLAN socket based on network namespace, address family and UDP port
 * and enabled unshareable flags.
 */
static struct vxlan_sock *vxlan_find_sock(struct net *net, sa_family_t family,
					  __be16 port, u32 flags)
{
	struct vxlan_sock *vs;

	flags &= VXLAN_F_RCV_FLAGS;

	hlist_for_each_entry_rcu(vs, vs_head(net, port), hlist) {
		if (inet_sk(vs->sock->sk)->inet_sport == port &&
		    vxlan_get_sk_family(vs) == family &&
		    vs->flags == flags)
			return vs;
	}
	return NULL;
}

static struct vxlan_dev *vxlan_vs_find_vni(struct vxlan_sock *vs, __be32 vni)
{
	struct vxlan_dev_node *node;

	/* For flow based devices, map all packets to VNI 0 */
	if (vs->flags & VXLAN_F_COLLECT_METADATA)
		vni = 0;

	hlist_for_each_entry_rcu(node, vni_head(vs, vni), hlist) {
		if (node->vxlan->default_dst.remote_vni == vni)
			return node->vxlan;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return NULL;
}

<<<<<<< HEAD
/* Fill in neighbour message in skbuff. */
static int vxlan_fdb_info(struct sk_buff *skb, struct vxlan_dev *vxlan,
			   const struct vxlan_fdb *fdb,
			   u32 portid, u32 seq, int type, unsigned int flags,
			   const struct vxlan_rdst *rdst)
=======
/* Look up VNI in a per net namespace table */
static struct vxlan_dev *vxlan_find_vni(struct net *net, __be32 vni,
					sa_family_t family, __be16 port,
					u32 flags)
{
	struct vxlan_sock *vs;

	vs = vxlan_find_sock(net, family, port, flags);
	if (!vs)
		return NULL;

	return vxlan_vs_find_vni(vs, vni);
}

/* Fill in neighbour message in skbuff. */
static int vxlan_fdb_info(struct sk_buff *skb, struct vxlan_dev *vxlan,
			  const struct vxlan_fdb *fdb,
			  u32 portid, u32 seq, int type, unsigned int flags,
			  const struct vxlan_rdst *rdst)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned long now = jiffies;
	struct nda_cacheinfo ci;
	struct nlmsghdr *nlh;
	struct ndmsg *ndm;
	bool send_ip, send_eth;

	nlh = nlmsg_put(skb, portid, seq, type, sizeof(*ndm), flags);
	if (nlh == NULL)
		return -EMSGSIZE;

	ndm = nlmsg_data(nlh);
	memset(ndm, 0, sizeof(*ndm));

	send_eth = send_ip = true;

	if (type == RTM_GETNEIGH) {
		ndm->ndm_family	= AF_INET;
<<<<<<< HEAD
		send_ip = rdst->remote_ip != htonl(INADDR_ANY);
=======
		send_ip = !vxlan_addr_any(&rdst->remote_ip);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		send_eth = !is_zero_ether_addr(fdb->eth_addr);
	} else
		ndm->ndm_family	= AF_BRIDGE;
	ndm->ndm_state = fdb->state;
	ndm->ndm_ifindex = vxlan->dev->ifindex;
	ndm->ndm_flags = fdb->flags;
<<<<<<< HEAD
	ndm->ndm_type = NDA_DST;
=======
	ndm->ndm_type = RTN_UNICAST;

	if (!net_eq(dev_net(vxlan->dev), vxlan->net) &&
	    nla_put_s32(skb, NDA_LINK_NETNSID,
			peernet2id(dev_net(vxlan->dev), vxlan->net)))
		goto nla_put_failure;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (send_eth && nla_put(skb, NDA_LLADDR, ETH_ALEN, &fdb->eth_addr))
		goto nla_put_failure;

<<<<<<< HEAD
	if (send_ip && nla_put_be32(skb, NDA_DST, rdst->remote_ip))
		goto nla_put_failure;

	if (rdst->remote_port && rdst->remote_port != vxlan->dst_port &&
	    nla_put_be16(skb, NDA_PORT, rdst->remote_port))
		goto nla_put_failure;
	if (rdst->remote_vni != vxlan->default_dst.remote_vni &&
	    nla_put_be32(skb, NDA_VNI, rdst->remote_vni))
=======
	if (send_ip && vxlan_nla_put_addr(skb, NDA_DST, &rdst->remote_ip))
		goto nla_put_failure;

	if (rdst->remote_port && rdst->remote_port != vxlan->cfg.dst_port &&
	    nla_put_be16(skb, NDA_PORT, rdst->remote_port))
		goto nla_put_failure;
	if (rdst->remote_vni != vxlan->default_dst.remote_vni &&
	    nla_put_u32(skb, NDA_VNI, be32_to_cpu(rdst->remote_vni)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto nla_put_failure;
	if (rdst->remote_ifindex &&
	    nla_put_u32(skb, NDA_IFINDEX, rdst->remote_ifindex))
		goto nla_put_failure;

	ci.ndm_used	 = jiffies_to_clock_t(now - fdb->used);
	ci.ndm_confirmed = 0;
	ci.ndm_updated	 = jiffies_to_clock_t(now - fdb->updated);
	ci.ndm_refcnt	 = 0;

	if (nla_put(skb, NDA_CACHEINFO, sizeof(ci), &ci))
		goto nla_put_failure;

<<<<<<< HEAD
	return nlmsg_end(skb, nlh);
=======
	nlmsg_end(skb, nlh);
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}

static inline size_t vxlan_nlmsg_size(void)
{
	return NLMSG_ALIGN(sizeof(struct ndmsg))
		+ nla_total_size(ETH_ALEN) /* NDA_LLADDR */
<<<<<<< HEAD
		+ nla_total_size(sizeof(__be32)) /* NDA_DST */
		+ nla_total_size(sizeof(__be16)) /* NDA_PORT */
		+ nla_total_size(sizeof(__be32)) /* NDA_VNI */
		+ nla_total_size(sizeof(__u32)) /* NDA_IFINDEX */
		+ nla_total_size(sizeof(struct nda_cacheinfo));
}

static void vxlan_fdb_notify(struct vxlan_dev *vxlan,
			     const struct vxlan_fdb *fdb, int type)
=======
		+ nla_total_size(sizeof(struct in6_addr)) /* NDA_DST */
		+ nla_total_size(sizeof(__be16)) /* NDA_PORT */
		+ nla_total_size(sizeof(__be32)) /* NDA_VNI */
		+ nla_total_size(sizeof(__u32)) /* NDA_IFINDEX */
		+ nla_total_size(sizeof(__s32)) /* NDA_LINK_NETNSID */
		+ nla_total_size(sizeof(struct nda_cacheinfo));
}

static void vxlan_fdb_notify(struct vxlan_dev *vxlan, struct vxlan_fdb *fdb,
			     struct vxlan_rdst *rd, int type)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct net *net = dev_net(vxlan->dev);
	struct sk_buff *skb;
	int err = -ENOBUFS;

	skb = nlmsg_new(vxlan_nlmsg_size(), GFP_ATOMIC);
	if (skb == NULL)
		goto errout;

<<<<<<< HEAD
	err = vxlan_fdb_info(skb, vxlan, fdb, 0, 0, type, 0, &fdb->remote);
=======
	err = vxlan_fdb_info(skb, vxlan, fdb, 0, 0, type, 0, rd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err < 0) {
		/* -EMSGSIZE implies BUG in vxlan_nlmsg_size() */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout;
	}

	rtnl_notify(skb, net, 0, RTNLGRP_NEIGH, NULL, GFP_ATOMIC);
	return;
errout:
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_NEIGH, err);
}

<<<<<<< HEAD
static void vxlan_ip_miss(struct net_device *dev, __be32 ipa)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_fdb f;

	memset(&f, 0, sizeof f);
	f.state = NUD_STALE;
	f.remote.remote_ip = ipa; /* goes to NDA_DST */
	f.remote.remote_vni = VXLAN_N_VID;

	vxlan_fdb_notify(vxlan, &f, RTM_GETNEIGH);
=======
static void vxlan_ip_miss(struct net_device *dev, union vxlan_addr *ipa)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_fdb f = {
		.state = NUD_STALE,
	};
	struct vxlan_rdst remote = {
		.remote_ip = *ipa, /* goes to NDA_DST */
		.remote_vni = cpu_to_be32(VXLAN_N_VID),
	};

	vxlan_fdb_notify(vxlan, &f, &remote, RTM_GETNEIGH);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void vxlan_fdb_miss(struct vxlan_dev *vxlan, const u8 eth_addr[ETH_ALEN])
{
<<<<<<< HEAD
	struct vxlan_fdb	f;

	memset(&f, 0, sizeof f);
	f.state = NUD_STALE;
	memcpy(f.eth_addr, eth_addr, ETH_ALEN);

	vxlan_fdb_notify(vxlan, &f, RTM_GETNEIGH);
=======
	struct vxlan_fdb f = {
		.state = NUD_STALE,
	};
	struct vxlan_rdst remote = { };

	memcpy(f.eth_addr, eth_addr, ETH_ALEN);

	vxlan_fdb_notify(vxlan, &f, &remote, RTM_GETNEIGH);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Hash Ethernet address */
static u32 eth_hash(const unsigned char *addr)
{
	u64 value = get_unaligned((u64 *)addr);

	/* only want 6 bytes */
#ifdef __BIG_ENDIAN
	value >>= 16;
#else
	value <<= 16;
#endif
	return hash_64(value, FDB_HASH_BITS);
}

/* Hash chain to use given mac address */
static inline struct hlist_head *vxlan_fdb_head(struct vxlan_dev *vxlan,
						const u8 *mac)
{
	return &vxlan->fdb_head[eth_hash(mac)];
}

/* Look up Ethernet address in forwarding table */
static struct vxlan_fdb *__vxlan_find_mac(struct vxlan_dev *vxlan,
					const u8 *mac)
<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct hlist_head *head = vxlan_fdb_head(vxlan, mac);
	struct vxlan_fdb *f;

	hlist_for_each_entry_rcu(f, head, hlist) {
<<<<<<< HEAD
		if (compare_ether_addr(mac, f->eth_addr) == 0)
=======
		if (ether_addr_equal(mac, f->eth_addr))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return f;
	}

	return NULL;
}

static struct vxlan_fdb *vxlan_find_mac(struct vxlan_dev *vxlan,
					const u8 *mac)
{
	struct vxlan_fdb *f;

	f = __vxlan_find_mac(vxlan, mac);
	if (f)
		f->used = jiffies;

	return f;
}

<<<<<<< HEAD
/* Add/update destinations for multicast */
static int vxlan_fdb_append(struct vxlan_fdb *f,
			    __be32 ip, __be16 port, __u32 vni, __u32 ifindex)
{
	struct vxlan_rdst *rd_prev, *rd;

	rd_prev = NULL;
	for (rd = &f->remote; rd; rd = rd->remote_next) {
		if (rd->remote_ip == ip &&
		    rd->remote_port == port &&
		    rd->remote_vni == vni &&
		    rd->remote_ifindex == ifindex)
			return 0;
		rd_prev = rd;
	}
	rd = kmalloc(sizeof(*rd), GFP_ATOMIC);
	if (rd == NULL)
		return -ENOBUFS;
	rd->remote_ip = ip;
	rd->remote_port = port;
	rd->remote_vni = vni;
	rd->remote_ifindex = ifindex;
	rd->remote_next = NULL;
	rd_prev->remote_next = rd;
	return 1;
}

/* Add new entry to forwarding table -- assumes lock held */
static int vxlan_fdb_create(struct vxlan_dev *vxlan,
			    const u8 *mac, __be32 ip,
			    __u16 state, __u16 flags,
			    __be16 port, __u32 vni, __u32 ifindex,
			    __u8 ndm_flags)
{
	struct vxlan_fdb *f;
	int notify = 0;
=======
/* caller should hold vxlan->hash_lock */
static struct vxlan_rdst *vxlan_fdb_find_rdst(struct vxlan_fdb *f,
					      union vxlan_addr *ip, __be16 port,
					      __be32 vni, __u32 ifindex)
{
	struct vxlan_rdst *rd;

	list_for_each_entry(rd, &f->remotes, list) {
		if (vxlan_addr_equal(&rd->remote_ip, ip) &&
		    rd->remote_port == port &&
		    rd->remote_vni == vni &&
		    rd->remote_ifindex == ifindex)
			return rd;
	}

	return NULL;
}

/* Replace destination of unicast mac */
static int vxlan_fdb_replace(struct vxlan_fdb *f,
			     union vxlan_addr *ip, __be16 port, __be32 vni,
			     __u32 ifindex)
{
	struct vxlan_rdst *rd;

	rd = vxlan_fdb_find_rdst(f, ip, port, vni, ifindex);
	if (rd)
		return 0;

	rd = list_first_entry_or_null(&f->remotes, struct vxlan_rdst, list);
	if (!rd)
		return 0;

	dst_cache_reset(&rd->dst_cache);
	rd->remote_ip = *ip;
	rd->remote_port = port;
	rd->remote_vni = vni;
	rd->remote_ifindex = ifindex;
	return 1;
}

/* Add/update destinations for multicast */
static int vxlan_fdb_append(struct vxlan_fdb *f,
			    union vxlan_addr *ip, __be16 port, __be32 vni,
			    __u32 ifindex, struct vxlan_rdst **rdp)
{
	struct vxlan_rdst *rd;

	rd = vxlan_fdb_find_rdst(f, ip, port, vni, ifindex);
	if (rd)
		return 0;

	rd = kmalloc(sizeof(*rd), GFP_ATOMIC);
	if (rd == NULL)
		return -ENOBUFS;

	if (dst_cache_init(&rd->dst_cache, GFP_ATOMIC)) {
		kfree(rd);
		return -ENOBUFS;
	}

	rd->remote_ip = *ip;
	rd->remote_port = port;
	rd->remote_vni = vni;
	rd->remote_ifindex = ifindex;

	list_add_tail_rcu(&rd->list, &f->remotes);

	*rdp = rd;
	return 1;
}

static struct vxlanhdr *vxlan_gro_remcsum(struct sk_buff *skb,
					  unsigned int off,
					  struct vxlanhdr *vh, size_t hdrlen,
					  __be32 vni_field,
					  struct gro_remcsum *grc,
					  bool nopartial)
{
	size_t start, offset;

	if (skb->remcsum_offload)
		return vh;

	if (!NAPI_GRO_CB(skb)->csum_valid)
		return NULL;

	start = vxlan_rco_start(vni_field);
	offset = start + vxlan_rco_offset(vni_field);

	vh = skb_gro_remcsum_process(skb, (void *)vh, off, hdrlen,
				     start, offset, grc, nopartial);

	skb->remcsum_offload = 1;

	return vh;
}

static struct sk_buff **vxlan_gro_receive(struct sock *sk,
					  struct sk_buff **head,
					  struct sk_buff *skb)
{
	struct sk_buff *p, **pp = NULL;
	struct vxlanhdr *vh, *vh2;
	unsigned int hlen, off_vx;
	int flush = 1;
	struct vxlan_sock *vs = rcu_dereference_sk_user_data(sk);
	__be32 flags;
	struct gro_remcsum grc;

	skb_gro_remcsum_init(&grc);

	off_vx = skb_gro_offset(skb);
	hlen = off_vx + sizeof(*vh);
	vh   = skb_gro_header_fast(skb, off_vx);
	if (skb_gro_header_hard(skb, hlen)) {
		vh = skb_gro_header_slow(skb, hlen, off_vx);
		if (unlikely(!vh))
			goto out;
	}

	skb_gro_postpull_rcsum(skb, vh, sizeof(struct vxlanhdr));

	flags = vh->vx_flags;

	if ((flags & VXLAN_HF_RCO) && (vs->flags & VXLAN_F_REMCSUM_RX)) {
		vh = vxlan_gro_remcsum(skb, off_vx, vh, sizeof(struct vxlanhdr),
				       vh->vx_vni, &grc,
				       !!(vs->flags &
					  VXLAN_F_REMCSUM_NOPARTIAL));

		if (!vh)
			goto out;
	}

	skb_gro_pull(skb, sizeof(struct vxlanhdr)); /* pull vxlan header */

	for (p = *head; p; p = p->next) {
		if (!NAPI_GRO_CB(p)->same_flow)
			continue;

		vh2 = (struct vxlanhdr *)(p->data + off_vx);
		if (vh->vx_flags != vh2->vx_flags ||
		    vh->vx_vni != vh2->vx_vni) {
			NAPI_GRO_CB(p)->same_flow = 0;
			continue;
		}
	}

	pp = call_gro_receive(eth_gro_receive, head, skb);
	flush = 0;

out:
	skb_gro_remcsum_cleanup(skb, &grc);
	NAPI_GRO_CB(skb)->flush |= flush;

	return pp;
}

static int vxlan_gro_complete(struct sock *sk, struct sk_buff *skb, int nhoff)
{
	/* Sets 'skb->inner_mac_header' since we are always called with
	 * 'skb->encapsulation' set.
	 */
	return eth_gro_complete(skb, nhoff + sizeof(struct vxlanhdr));
}

/* Add new entry to forwarding table -- assumes lock held */
static int vxlan_fdb_create(struct vxlan_dev *vxlan,
			    const u8 *mac, union vxlan_addr *ip,
			    __u16 state, __u16 flags,
			    __be16 port, __be32 vni, __u32 ifindex,
			    __u8 ndm_flags)
{
	struct vxlan_rdst *rd = NULL;
	struct vxlan_fdb *f;
	int notify = 0;
	int rc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	f = __vxlan_find_mac(vxlan, mac);
	if (f) {
		if (flags & NLM_F_EXCL) {
			netdev_dbg(vxlan->dev,
				   "lost race to create %pM\n", mac);
			return -EEXIST;
		}
		if (f->state != state) {
			f->state = state;
			f->updated = jiffies;
			notify = 1;
		}
		if (f->flags != ndm_flags) {
			f->flags = ndm_flags;
			f->updated = jiffies;
			notify = 1;
		}
<<<<<<< HEAD
		if ((flags & NLM_F_APPEND) &&
		    is_multicast_ether_addr(f->eth_addr)) {
			int rc = vxlan_fdb_append(f, ip, port, vni, ifindex);
=======
		if ((flags & NLM_F_REPLACE)) {
			/* Only change unicasts */
			if (!(is_multicast_ether_addr(f->eth_addr) ||
			     is_zero_ether_addr(f->eth_addr))) {
				notify |= vxlan_fdb_replace(f, ip, port, vni,
							   ifindex);
			} else
				return -EOPNOTSUPP;
		}
		if ((flags & NLM_F_APPEND) &&
		    (is_multicast_ether_addr(f->eth_addr) ||
		     is_zero_ether_addr(f->eth_addr))) {
			rc = vxlan_fdb_append(f, ip, port, vni, ifindex, &rd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			if (rc < 0)
				return rc;
			notify |= rc;
		}
	} else {
		if (!(flags & NLM_F_CREATE))
			return -ENOENT;

<<<<<<< HEAD
		if (vxlan->addrmax && vxlan->addrcnt >= vxlan->addrmax)
			return -ENOSPC;

		netdev_dbg(vxlan->dev, "add %pM -> %pI4\n", mac, &ip);
=======
		if (vxlan->cfg.addrmax &&
		    vxlan->addrcnt >= vxlan->cfg.addrmax)
			return -ENOSPC;

		/* Disallow replace to add a multicast entry */
		if ((flags & NLM_F_REPLACE) &&
		    (is_multicast_ether_addr(mac) || is_zero_ether_addr(mac)))
			return -EOPNOTSUPP;

		netdev_dbg(vxlan->dev, "add %pM -> %pIS\n", mac, ip);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		f = kmalloc(sizeof(*f), GFP_ATOMIC);
		if (!f)
			return -ENOMEM;

		notify = 1;
<<<<<<< HEAD
		f->remote.remote_ip = ip;
		f->remote.remote_port = port;
		f->remote.remote_vni = vni;
		f->remote.remote_ifindex = ifindex;
		f->remote.remote_next = NULL;
		f->state = state;
		f->flags = ndm_flags;
		f->updated = f->used = jiffies;
		memcpy(f->eth_addr, mac, ETH_ALEN);

=======
		f->state = state;
		f->flags = ndm_flags;
		f->updated = f->used = jiffies;
		INIT_LIST_HEAD(&f->remotes);
		memcpy(f->eth_addr, mac, ETH_ALEN);

		rc = vxlan_fdb_append(f, ip, port, vni, ifindex, &rd);
		if (rc < 0) {
			kfree(f);
			return rc;
		}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		++vxlan->addrcnt;
		hlist_add_head_rcu(&f->hlist,
				   vxlan_fdb_head(vxlan, mac));
	}

<<<<<<< HEAD
	if (notify)
		vxlan_fdb_notify(vxlan, f, RTM_NEWNEIGH);
=======
	if (notify) {
		if (rd == NULL)
			rd = first_remote_rtnl(f);
		vxlan_fdb_notify(vxlan, f, rd, RTM_NEWNEIGH);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static void vxlan_fdb_free(struct rcu_head *head)
{
	struct vxlan_fdb *f = container_of(head, struct vxlan_fdb, rcu);
<<<<<<< HEAD

	while (f->remote.remote_next) {
		struct vxlan_rdst *rd = f->remote.remote_next;

		f->remote.remote_next = rd->remote_next;
=======
	struct vxlan_rdst *rd, *nd;

	list_for_each_entry_safe(rd, nd, &f->remotes, list) {
		dst_cache_destroy(&rd->dst_cache);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		kfree(rd);
	}
	kfree(f);
}

static void vxlan_fdb_destroy(struct vxlan_dev *vxlan, struct vxlan_fdb *f)
{
	netdev_dbg(vxlan->dev,
		    "delete %pM\n", f->eth_addr);

	--vxlan->addrcnt;
<<<<<<< HEAD
	vxlan_fdb_notify(vxlan, f, RTM_DELNEIGH);
=======
	vxlan_fdb_notify(vxlan, f, first_remote_rtnl(f), RTM_DELNEIGH);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	hlist_del_rcu(&f->hlist);
	call_rcu(&f->rcu, vxlan_fdb_free);
}

<<<<<<< HEAD
/* Add static entry (via netlink) */
static int vxlan_fdb_add(struct ndmsg *ndm, struct nlattr *tb[],
			 struct net_device *dev,
			 const unsigned char *addr, u16 flags)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct net *net = dev_net(vxlan->dev);
	__be32 ip;
	__be16 port;
	u32 vni, ifindex;
	int err;

	if (!(ndm->ndm_state & (NUD_PERMANENT|NUD_REACHABLE))) {
		pr_info("RTM_NEWNEIGH with invalid state %#x\n",
			ndm->ndm_state);
		return -EINVAL;
	}

	if (tb[NDA_DST] == NULL)
		return -EINVAL;

	if (nla_len(tb[NDA_DST]) != sizeof(__be32))
		return -EAFNOSUPPORT;

	ip = nla_get_be32(tb[NDA_DST]);
=======
static void vxlan_dst_free(struct rcu_head *head)
{
	struct vxlan_rdst *rd = container_of(head, struct vxlan_rdst, rcu);

	dst_cache_destroy(&rd->dst_cache);
	kfree(rd);
}

static void vxlan_fdb_dst_destroy(struct vxlan_dev *vxlan, struct vxlan_fdb *f,
				  struct vxlan_rdst *rd)
{
	list_del_rcu(&rd->list);
	vxlan_fdb_notify(vxlan, f, rd, RTM_DELNEIGH);
	call_rcu(&rd->rcu, vxlan_dst_free);
}

static int vxlan_fdb_parse(struct nlattr *tb[], struct vxlan_dev *vxlan,
			   union vxlan_addr *ip, __be16 *port, __be32 *vni,
			   u32 *ifindex)
{
	struct net *net = dev_net(vxlan->dev);
	int err;

	if (tb[NDA_DST]) {
		err = vxlan_nla_get_addr(ip, tb[NDA_DST]);
		if (err)
			return err;
	} else {
		union vxlan_addr *remote = &vxlan->default_dst.remote_ip;
		if (remote->sa.sa_family == AF_INET) {
			ip->sin.sin_addr.s_addr = htonl(INADDR_ANY);
			ip->sa.sa_family = AF_INET;
#if IS_ENABLED(CONFIG_IPV6)
		} else {
			ip->sin6.sin6_addr = in6addr_any;
			ip->sa.sa_family = AF_INET6;
#endif
		}
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (tb[NDA_PORT]) {
		if (nla_len(tb[NDA_PORT]) != sizeof(__be16))
			return -EINVAL;
<<<<<<< HEAD
		port = nla_get_be16(tb[NDA_PORT]);
	} else
		port = vxlan->dst_port;
=======
		*port = nla_get_be16(tb[NDA_PORT]);
	} else {
		*port = vxlan->cfg.dst_port;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (tb[NDA_VNI]) {
		if (nla_len(tb[NDA_VNI]) != sizeof(u32))
			return -EINVAL;
<<<<<<< HEAD
		vni = nla_get_u32(tb[NDA_VNI]);
	} else
		vni = vxlan->default_dst.remote_vni;
=======
		*vni = cpu_to_be32(nla_get_u32(tb[NDA_VNI]));
	} else {
		*vni = vxlan->default_dst.remote_vni;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (tb[NDA_IFINDEX]) {
		struct net_device *tdev;

		if (nla_len(tb[NDA_IFINDEX]) != sizeof(u32))
			return -EINVAL;
<<<<<<< HEAD
		ifindex = nla_get_u32(tb[NDA_IFINDEX]);
		tdev = dev_get_by_index(net, ifindex);
		if (!tdev)
			return -EADDRNOTAVAIL;
		dev_put(tdev);
	} else
		ifindex = 0;

	spin_lock_bh(&vxlan->hash_lock);
	err = vxlan_fdb_create(vxlan, addr, ip, ndm->ndm_state, flags,
=======
		*ifindex = nla_get_u32(tb[NDA_IFINDEX]);
		tdev = __dev_get_by_index(net, *ifindex);
		if (!tdev)
			return -EADDRNOTAVAIL;
	} else {
		*ifindex = 0;
	}

	return 0;
}

/* Add static entry (via netlink) */
static int vxlan_fdb_add(struct ndmsg *ndm, struct nlattr *tb[],
			 struct net_device *dev,
			 const unsigned char *addr, u16 vid, u16 flags)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	/* struct net *net = dev_net(vxlan->dev); */
	union vxlan_addr ip;
	__be16 port;
	__be32 vni;
	u32 ifindex;
	int err;

	if (!(ndm->ndm_state & (NUD_PERMANENT|NUD_REACHABLE))) {
		pr_info("RTM_NEWNEIGH with invalid state %#x\n",
			ndm->ndm_state);
		return -EINVAL;
	}

	if (tb[NDA_DST] == NULL)
		return -EINVAL;

	err = vxlan_fdb_parse(tb, vxlan, &ip, &port, &vni, &ifindex);
	if (err)
		return err;

	if (vxlan->default_dst.remote_ip.sa.sa_family != ip.sa.sa_family)
		return -EAFNOSUPPORT;

	spin_lock_bh(&vxlan->hash_lock);
	err = vxlan_fdb_create(vxlan, addr, &ip, ndm->ndm_state, flags,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			       port, vni, ifindex, ndm->ndm_flags);
	spin_unlock_bh(&vxlan->hash_lock);

	return err;
}

/* Delete entry (via netlink) */
static int vxlan_fdb_delete(struct ndmsg *ndm, struct nlattr *tb[],
			    struct net_device *dev,
<<<<<<< HEAD
			    const unsigned char *addr)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_fdb *f;
	int err = -ENOENT;

	spin_lock_bh(&vxlan->hash_lock);
	f = vxlan_find_mac(vxlan, addr);
	if (f) {
		vxlan_fdb_destroy(vxlan, f);
		err = 0;
	}
=======
			    const unsigned char *addr, u16 vid)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_fdb *f;
	struct vxlan_rdst *rd = NULL;
	union vxlan_addr ip;
	__be16 port;
	__be32 vni;
	u32 ifindex;
	int err;

	err = vxlan_fdb_parse(tb, vxlan, &ip, &port, &vni, &ifindex);
	if (err)
		return err;

	err = -ENOENT;

	spin_lock_bh(&vxlan->hash_lock);
	f = vxlan_find_mac(vxlan, addr);
	if (!f)
		goto out;

	if (!vxlan_addr_any(&ip)) {
		rd = vxlan_fdb_find_rdst(f, &ip, port, vni, ifindex);
		if (!rd)
			goto out;
	}

	err = 0;

	/* remove a destination if it's not the only one on the list,
	 * otherwise destroy the fdb entry
	 */
	if (rd && !list_is_singular(&f->remotes)) {
		vxlan_fdb_dst_destroy(vxlan, f, rd);
		goto out;
	}

	vxlan_fdb_destroy(vxlan, f);

out:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_unlock_bh(&vxlan->hash_lock);

	return err;
}

/* Dump forwarding table */
static int vxlan_fdb_dump(struct sk_buff *skb, struct netlink_callback *cb,
<<<<<<< HEAD
			  struct net_device *dev, int idx)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	unsigned int h;

	for (h = 0; h < FDB_HASH_SIZE; ++h) {
		struct vxlan_fdb *f;
		int err;

		hlist_for_each_entry_rcu(f, &vxlan->fdb_head[h], hlist) {
			struct vxlan_rdst *rd;
			for (rd = &f->remote; rd; rd = rd->remote_next) {
				if (idx < cb->args[0])
=======
			  struct net_device *dev,
			  struct net_device *filter_dev, int *idx)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	unsigned int h;
	int err = 0;

	for (h = 0; h < FDB_HASH_SIZE; ++h) {
		struct vxlan_fdb *f;

		rcu_read_lock();
		hlist_for_each_entry_rcu(f, &vxlan->fdb_head[h], hlist) {
			struct vxlan_rdst *rd;

			list_for_each_entry_rcu(rd, &f->remotes, list) {
				if (*idx < cb->args[2])
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					goto skip;

				err = vxlan_fdb_info(skb, vxlan, f,
						     NETLINK_CB(cb->skb).portid,
						     cb->nlh->nlmsg_seq,
						     RTM_NEWNEIGH,
						     NLM_F_MULTI, rd);
<<<<<<< HEAD
				if (err < 0)
					break;
skip:
				++idx;
			}
		}
	}

	return idx;
=======
				if (err < 0) {
					rcu_read_unlock();
					goto out;
				}
skip:
				*idx += 1;
			}
		}
		rcu_read_unlock();
	}
out:
	return err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Watch incoming packets to learn mapping between Ethernet address
 * and Tunnel endpoint.
<<<<<<< HEAD
 * Return true if packet is bogus and should be droppped.
 */
static bool vxlan_snoop(struct net_device *dev,
			__be32 src_ip, const u8 *src_mac)
=======
 * Return true if packet is bogus and should be dropped.
 */
static bool vxlan_snoop(struct net_device *dev,
			union vxlan_addr *src_ip, const u8 *src_mac)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_fdb *f;

	f = vxlan_find_mac(vxlan, src_mac);
	if (likely(f)) {
<<<<<<< HEAD
		if (likely(f->remote.remote_ip == src_ip))
			return false;

		/* Don't migrate static entries, drop packets */
		if (f->state & NUD_NOARP)
=======
		struct vxlan_rdst *rdst = first_remote_rcu(f);

		if (likely(vxlan_addr_equal(&rdst->remote_ip, src_ip)))
			return false;

		/* Don't migrate static entries, drop packets */
		if (f->state & (NUD_PERMANENT | NUD_NOARP))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return true;

		if (net_ratelimit())
			netdev_info(dev,
<<<<<<< HEAD
				    "%pM migrated from %pI4 to %pI4\n",
				    src_mac, &f->remote.remote_ip, &src_ip);

		f->remote.remote_ip = src_ip;
		f->updated = jiffies;
=======
				    "%pM migrated from %pIS to %pIS\n",
				    src_mac, &rdst->remote_ip.sa, &src_ip->sa);

		rdst->remote_ip = *src_ip;
		f->updated = jiffies;
		vxlan_fdb_notify(vxlan, f, rdst, RTM_NEWNEIGH);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else {
		/* learned new entry */
		spin_lock(&vxlan->hash_lock);

		/* close off race between vxlan_flush and incoming packets */
		if (netif_running(dev))
			vxlan_fdb_create(vxlan, src_mac, src_ip,
					 NUD_REACHABLE,
					 NLM_F_EXCL|NLM_F_CREATE,
<<<<<<< HEAD
					 vxlan->dst_port,
=======
					 vxlan->cfg.dst_port,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					 vxlan->default_dst.remote_vni,
					 0, NTF_SELF);
		spin_unlock(&vxlan->hash_lock);
	}

	return false;
}

<<<<<<< HEAD

/* See if multicast group is already in use by other ID */
static bool vxlan_group_used(struct vxlan_net *vn,
			     const struct vxlan_dev *this)
{
	const struct vxlan_dev *vxlan;
	unsigned h;

	for (h = 0; h < VNI_HASH_SIZE; ++h)
		hlist_for_each_entry(vxlan, &vn->vni_list[h], hlist) {
			if (vxlan == this)
				continue;

			if (!netif_running(vxlan->dev))
				continue;

			if (vxlan->default_dst.remote_ip == this->default_dst.remote_ip)
				return true;
		}
=======
/* See if multicast group is already in use by other ID */
static bool vxlan_group_used(struct vxlan_net *vn, struct vxlan_dev *dev)
{
	struct vxlan_dev *vxlan;
	struct vxlan_sock *sock4;
#if IS_ENABLED(CONFIG_IPV6)
	struct vxlan_sock *sock6;
#endif
	unsigned short family = dev->default_dst.remote_ip.sa.sa_family;

	sock4 = rtnl_dereference(dev->vn4_sock);

	/* The vxlan_sock is only used by dev, leaving group has
	 * no effect on other vxlan devices.
	 */
	if (family == AF_INET && sock4 && atomic_read(&sock4->refcnt) == 1)
		return false;
#if IS_ENABLED(CONFIG_IPV6)
	sock6 = rtnl_dereference(dev->vn6_sock);
	if (family == AF_INET6 && sock6 && atomic_read(&sock6->refcnt) == 1)
		return false;
#endif

	list_for_each_entry(vxlan, &vn->vxlan_list, next) {
		if (!netif_running(vxlan->dev) || vxlan == dev)
			continue;

		if (family == AF_INET &&
		    rtnl_dereference(vxlan->vn4_sock) != sock4)
			continue;
#if IS_ENABLED(CONFIG_IPV6)
		if (family == AF_INET6 &&
		    rtnl_dereference(vxlan->vn6_sock) != sock6)
			continue;
#endif

		if (!vxlan_addr_equal(&vxlan->default_dst.remote_ip,
				      &dev->default_dst.remote_ip))
			continue;

		if (vxlan->default_dst.remote_ifindex !=
		    dev->default_dst.remote_ifindex)
			continue;

		return true;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return false;
}

<<<<<<< HEAD
/* kernel equivalent to IP_ADD_MEMBERSHIP */
static int vxlan_join_group(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_net *vn = net_generic(dev_net(dev), vxlan_net_id);
	struct sock *sk = vn->sock->sk;
	struct ip_mreqn mreq = {
		.imr_multiaddr.s_addr	= vxlan->default_dst.remote_ip,
		.imr_ifindex		= vxlan->default_dst.remote_ifindex,
	};
	int err;

	/* Already a member of group */
	if (vxlan_group_used(vn, vxlan))
		return 0;

	/* Need to drop RTNL to call multicast join */
	rtnl_unlock();
	lock_sock(sk);
	err = ip_mc_join_group(sk, &mreq);
	release_sock(sk);
	rtnl_lock();

	return err;
}


/* kernel equivalent to IP_DROP_MEMBERSHIP */
static int vxlan_leave_group(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_net *vn = net_generic(dev_net(dev), vxlan_net_id);
	int err = 0;
	struct sock *sk = vn->sock->sk;
	struct ip_mreqn mreq = {
		.imr_multiaddr.s_addr	= vxlan->default_dst.remote_ip,
		.imr_ifindex		= vxlan->default_dst.remote_ifindex,
	};

	/* Only leave group when last vxlan is done. */
	if (vxlan_group_used(vn, vxlan))
		return 0;

	/* Need to drop RTNL to call multicast leave */
	rtnl_unlock();
	lock_sock(sk);
	err = ip_mc_leave_group(sk, &mreq);
	release_sock(sk);
	rtnl_lock();

	return err;
}

/* Callback from net/ipv4/udp.c to receive packets */
static int vxlan_udp_encap_recv(struct sock *sk, struct sk_buff *skb)
{
	struct iphdr *oip;
	struct vxlanhdr *vxh;
	struct vxlan_dev *vxlan;
	struct pcpu_tstats *stats;
	__u32 vni;
	int err;

	/* pop off outer UDP header */
	__skb_pull(skb, sizeof(struct udphdr));

	/* Need Vxlan and inner Ethernet header to be present */
	if (!pskb_may_pull(skb, sizeof(struct vxlanhdr)))
		goto error;

	/* Drop packets with reserved bits set */
	vxh = (struct vxlanhdr *) skb->data;
	if (vxh->vx_flags != htonl(VXLAN_FLAGS) ||
	    (vxh->vx_vni & htonl(0xff))) {
		netdev_dbg(skb->dev, "invalid vxlan flags=%#x vni=%#x\n",
			   ntohl(vxh->vx_flags), ntohl(vxh->vx_vni));
		goto error;
	}

	__skb_pull(skb, sizeof(struct vxlanhdr));

	/* Is this VNI defined? */
	vni = ntohl(vxh->vx_vni) >> 8;
	vxlan = vxlan_find_vni(sock_net(sk), vni);
	if (!vxlan) {
		netdev_dbg(skb->dev, "unknown vni %d\n", vni);
		goto drop;
	}

	if (!pskb_may_pull(skb, ETH_HLEN)) {
		vxlan->dev->stats.rx_length_errors++;
		vxlan->dev->stats.rx_errors++;
		goto drop;
	}

	skb_reset_mac_header(skb);

	/* Re-examine inner Ethernet packet */
	oip = ip_hdr(skb);
	skb->protocol = eth_type_trans(skb, vxlan->dev);

	/* Ignore packet loops (and multicast echo) */
	if (compare_ether_addr(eth_hdr(skb)->h_source,
			       vxlan->dev->dev_addr) == 0)
		goto drop;

	if ((vxlan->flags & VXLAN_F_LEARN) &&
	    vxlan_snoop(skb->dev, oip->saddr, eth_hdr(skb)->h_source))
		goto drop;

	__skb_tunnel_rx(skb, vxlan->dev);
	skb_reset_network_header(skb);

	/* If the NIC driver gave us an encapsulated packet with
	 * CHECKSUM_UNNECESSARY and Rx checksum feature is enabled,
	 * leave the CHECKSUM_UNNECESSARY, the device checksummed it
	 * for us. Otherwise force the upper layers to verify it.
	 */
	if (skb->ip_summed != CHECKSUM_UNNECESSARY || !skb->encapsulation ||
	    !(vxlan->dev->features & NETIF_F_RXCSUM))
		skb->ip_summed = CHECKSUM_NONE;

	skb->encapsulation = 0;

	err = IP_ECN_decapsulate(oip, skb);
	if (unlikely(err)) {
		if (log_ecn_error)
			net_info_ratelimited("non-ECT from %pI4 with TOS=%#x\n",
					     &oip->saddr, oip->tos);
		if (err > 1) {
			++vxlan->dev->stats.rx_frame_errors;
			++vxlan->dev->stats.rx_errors;
			goto drop;
		}
=======
static bool __vxlan_sock_release_prep(struct vxlan_sock *vs)
{
	struct vxlan_net *vn;

	if (!vs)
		return false;
	if (!atomic_dec_and_test(&vs->refcnt))
		return false;

	vn = net_generic(sock_net(vs->sock->sk), vxlan_net_id);
	spin_lock(&vn->sock_lock);
	hlist_del_rcu(&vs->hlist);
	udp_tunnel_notify_del_rx_port(vs->sock,
				      (vs->flags & VXLAN_F_GPE) ?
				      UDP_TUNNEL_TYPE_VXLAN_GPE :
				      UDP_TUNNEL_TYPE_VXLAN);
	spin_unlock(&vn->sock_lock);

	return true;
}

static void vxlan_sock_release(struct vxlan_dev *vxlan)
{
	struct vxlan_sock *sock4 = rtnl_dereference(vxlan->vn4_sock);
#if IS_ENABLED(CONFIG_IPV6)
	struct vxlan_sock *sock6 = rtnl_dereference(vxlan->vn6_sock);

	rcu_assign_pointer(vxlan->vn6_sock, NULL);
#endif

	rcu_assign_pointer(vxlan->vn4_sock, NULL);
	synchronize_net();

	vxlan_vs_del_dev(vxlan);

	if (__vxlan_sock_release_prep(sock4)) {
		udp_tunnel_sock_release(sock4->sock);
		kfree(sock4);
	}

#if IS_ENABLED(CONFIG_IPV6)
	if (__vxlan_sock_release_prep(sock6)) {
		udp_tunnel_sock_release(sock6->sock);
		kfree(sock6);
	}
#endif
}

/* Update multicast group membership when first VNI on
 * multicast address is brought up
 */
static int vxlan_igmp_join(struct vxlan_dev *vxlan)
{
	struct sock *sk;
	union vxlan_addr *ip = &vxlan->default_dst.remote_ip;
	int ifindex = vxlan->default_dst.remote_ifindex;
	int ret = -EINVAL;

	if (ip->sa.sa_family == AF_INET) {
		struct vxlan_sock *sock4 = rtnl_dereference(vxlan->vn4_sock);
		struct ip_mreqn mreq = {
			.imr_multiaddr.s_addr	= ip->sin.sin_addr.s_addr,
			.imr_ifindex		= ifindex,
		};

		sk = sock4->sock->sk;
		lock_sock(sk);
		ret = ip_mc_join_group(sk, &mreq);
		release_sock(sk);
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		struct vxlan_sock *sock6 = rtnl_dereference(vxlan->vn6_sock);

		sk = sock6->sock->sk;
		lock_sock(sk);
		ret = ipv6_stub->ipv6_sock_mc_join(sk, ifindex,
						   &ip->sin6.sin6_addr);
		release_sock(sk);
#endif
	}

	return ret;
}

/* Inverse of vxlan_igmp_join when last VNI is brought down */
static int vxlan_igmp_leave(struct vxlan_dev *vxlan)
{
	struct sock *sk;
	union vxlan_addr *ip = &vxlan->default_dst.remote_ip;
	int ifindex = vxlan->default_dst.remote_ifindex;
	int ret = -EINVAL;

	if (ip->sa.sa_family == AF_INET) {
		struct vxlan_sock *sock4 = rtnl_dereference(vxlan->vn4_sock);
		struct ip_mreqn mreq = {
			.imr_multiaddr.s_addr	= ip->sin.sin_addr.s_addr,
			.imr_ifindex		= ifindex,
		};

		sk = sock4->sock->sk;
		lock_sock(sk);
		ret = ip_mc_leave_group(sk, &mreq);
		release_sock(sk);
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		struct vxlan_sock *sock6 = rtnl_dereference(vxlan->vn6_sock);

		sk = sock6->sock->sk;
		lock_sock(sk);
		ret = ipv6_stub->ipv6_sock_mc_drop(sk, ifindex,
						   &ip->sin6.sin6_addr);
		release_sock(sk);
#endif
	}

	return ret;
}

static bool vxlan_remcsum(struct vxlanhdr *unparsed,
			  struct sk_buff *skb, u32 vxflags)
{
	size_t start, offset;

	if (!(unparsed->vx_flags & VXLAN_HF_RCO) || skb->remcsum_offload)
		goto out;

	start = vxlan_rco_start(unparsed->vx_vni);
	offset = start + vxlan_rco_offset(unparsed->vx_vni);

	if (!pskb_may_pull(skb, offset + sizeof(u16)))
		return false;

	skb_remcsum_process(skb, (void *)(vxlan_hdr(skb) + 1), start, offset,
			    !!(vxflags & VXLAN_F_REMCSUM_NOPARTIAL));
out:
	unparsed->vx_flags &= ~VXLAN_HF_RCO;
	unparsed->vx_vni &= VXLAN_VNI_MASK;
	return true;
}

static void vxlan_parse_gbp_hdr(struct vxlanhdr *unparsed,
				struct sk_buff *skb, u32 vxflags,
				struct vxlan_metadata *md)
{
	struct vxlanhdr_gbp *gbp = (struct vxlanhdr_gbp *)unparsed;
	struct metadata_dst *tun_dst;

	if (!(unparsed->vx_flags & VXLAN_HF_GBP))
		goto out;

	md->gbp = ntohs(gbp->policy_id);

	tun_dst = (struct metadata_dst *)skb_dst(skb);
	if (tun_dst) {
		tun_dst->u.tun_info.key.tun_flags |= TUNNEL_VXLAN_OPT;
		tun_dst->u.tun_info.options_len = sizeof(*md);
	}
	if (gbp->dont_learn)
		md->gbp |= VXLAN_GBP_DONT_LEARN;

	if (gbp->policy_applied)
		md->gbp |= VXLAN_GBP_POLICY_APPLIED;

	/* In flow-based mode, GBP is carried in dst_metadata */
	if (!(vxflags & VXLAN_F_COLLECT_METADATA))
		skb->mark = md->gbp;
out:
	unparsed->vx_flags &= ~VXLAN_GBP_USED_BITS;
}

static bool vxlan_parse_gpe_hdr(struct vxlanhdr *unparsed,
				__be16 *protocol,
				struct sk_buff *skb, u32 vxflags)
{
	struct vxlanhdr_gpe *gpe = (struct vxlanhdr_gpe *)unparsed;

	/* Need to have Next Protocol set for interfaces in GPE mode. */
	if (!gpe->np_applied)
		return false;
	/* "The initial version is 0. If a receiver does not support the
	 * version indicated it MUST drop the packet.
	 */
	if (gpe->version != 0)
		return false;
	/* "When the O bit is set to 1, the packet is an OAM packet and OAM
	 * processing MUST occur." However, we don't implement OAM
	 * processing, thus drop the packet.
	 */
	if (gpe->oam_flag)
		return false;

	switch (gpe->next_protocol) {
	case VXLAN_GPE_NP_IPV4:
		*protocol = htons(ETH_P_IP);
		break;
	case VXLAN_GPE_NP_IPV6:
		*protocol = htons(ETH_P_IPV6);
		break;
	case VXLAN_GPE_NP_ETHERNET:
		*protocol = htons(ETH_P_TEB);
		break;
	default:
		return false;
	}

	unparsed->vx_flags &= ~VXLAN_GPE_USED_BITS;
	return true;
}

static bool vxlan_set_mac(struct vxlan_dev *vxlan,
			  struct vxlan_sock *vs,
			  struct sk_buff *skb)
{
	union vxlan_addr saddr;

	skb_reset_mac_header(skb);
	skb->protocol = eth_type_trans(skb, vxlan->dev);
	skb_postpull_rcsum(skb, eth_hdr(skb), ETH_HLEN);

	/* Ignore packet loops (and multicast echo) */
	if (ether_addr_equal(eth_hdr(skb)->h_source, vxlan->dev->dev_addr))
		return false;

	/* Get address from the outer IP header */
	if (vxlan_get_sk_family(vs) == AF_INET) {
		saddr.sin.sin_addr.s_addr = ip_hdr(skb)->saddr;
		saddr.sa.sa_family = AF_INET;
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		saddr.sin6.sin6_addr = ipv6_hdr(skb)->saddr;
		saddr.sa.sa_family = AF_INET6;
#endif
	}

	if ((vxlan->flags & VXLAN_F_LEARN) &&
	    vxlan_snoop(skb->dev, &saddr, eth_hdr(skb)->h_source))
		return false;

	return true;
}

static bool vxlan_ecn_decapsulate(struct vxlan_sock *vs, void *oiph,
				  struct sk_buff *skb)
{
	int err = 0;

	if (vxlan_get_sk_family(vs) == AF_INET)
		err = IP_ECN_decapsulate(oiph, skb);
#if IS_ENABLED(CONFIG_IPV6)
	else
		err = IP6_ECN_decapsulate(oiph, skb);
#endif

	if (unlikely(err) && log_ecn_error) {
		if (vxlan_get_sk_family(vs) == AF_INET)
			net_info_ratelimited("non-ECT from %pI4 with TOS=%#x\n",
					     &((struct iphdr *)oiph)->saddr,
					     ((struct iphdr *)oiph)->tos);
		else
			net_info_ratelimited("non-ECT from %pI6\n",
					     &((struct ipv6hdr *)oiph)->saddr);
	}
	return err <= 1;
}

/* Callback from net/ipv4/udp.c to receive packets */
static int vxlan_rcv(struct sock *sk, struct sk_buff *skb)
{
	struct pcpu_sw_netstats *stats;
	struct vxlan_dev *vxlan;
	struct vxlan_sock *vs;
	struct vxlanhdr unparsed;
	struct vxlan_metadata _md;
	struct vxlan_metadata *md = &_md;
	__be16 protocol = htons(ETH_P_TEB);
	bool raw_proto = false;
	void *oiph;

	/* Need UDP and VXLAN header to be present */
	if (!pskb_may_pull(skb, VXLAN_HLEN))
		goto drop;

	unparsed = *vxlan_hdr(skb);
	/* VNI flag always required to be set */
	if (!(unparsed.vx_flags & VXLAN_HF_VNI)) {
		netdev_dbg(skb->dev, "invalid vxlan flags=%#x vni=%#x\n",
			   ntohl(vxlan_hdr(skb)->vx_flags),
			   ntohl(vxlan_hdr(skb)->vx_vni));
		/* Return non vxlan pkt */
		goto drop;
	}
	unparsed.vx_flags &= ~VXLAN_HF_VNI;
	unparsed.vx_vni &= ~VXLAN_VNI_MASK;

	vs = rcu_dereference_sk_user_data(sk);
	if (!vs)
		goto drop;

	vxlan = vxlan_vs_find_vni(vs, vxlan_vni(vxlan_hdr(skb)->vx_vni));
	if (!vxlan)
		goto drop;

	/* For backwards compatibility, only allow reserved fields to be
	 * used by VXLAN extensions if explicitly requested.
	 */
	if (vs->flags & VXLAN_F_GPE) {
		if (!vxlan_parse_gpe_hdr(&unparsed, &protocol, skb, vs->flags))
			goto drop;
		raw_proto = true;
	}

	if (__iptunnel_pull_header(skb, VXLAN_HLEN, protocol, raw_proto,
				   !net_eq(vxlan->net, dev_net(vxlan->dev))))
			goto drop;

	if (vxlan_collect_metadata(vs)) {
		__be32 vni = vxlan_vni(vxlan_hdr(skb)->vx_vni);
		struct metadata_dst *tun_dst;

		tun_dst = udp_tun_rx_dst(skb, vxlan_get_sk_family(vs), TUNNEL_KEY,
					 key32_to_tunnel_id(vni), sizeof(*md));

		if (!tun_dst)
			goto drop;

		md = ip_tunnel_info_opts(&tun_dst->u.tun_info);

		skb_dst_set(skb, (struct dst_entry *)tun_dst);
	} else {
		memset(md, 0, sizeof(*md));
	}

	if (vs->flags & VXLAN_F_REMCSUM_RX)
		if (!vxlan_remcsum(&unparsed, skb, vs->flags))
			goto drop;
	if (vs->flags & VXLAN_F_GBP)
		vxlan_parse_gbp_hdr(&unparsed, skb, vs->flags, md);
	/* Note that GBP and GPE can never be active together. This is
	 * ensured in vxlan_dev_configure.
	 */

	if (unparsed.vx_flags || unparsed.vx_vni) {
		/* If there are any unprocessed flags remaining treat
		 * this as a malformed packet. This behavior diverges from
		 * VXLAN RFC (RFC7348) which stipulates that bits in reserved
		 * in reserved fields are to be ignored. The approach here
		 * maintains compatibility with previous stack code, and also
		 * is more robust and provides a little more security in
		 * adding extensions to VXLAN.
		 */
		goto drop;
	}

	if (!raw_proto) {
		if (!vxlan_set_mac(vxlan, vs, skb))
			goto drop;
	} else {
		skb_reset_mac_header(skb);
		skb->dev = vxlan->dev;
		skb->pkt_type = PACKET_HOST;
	}

	oiph = skb_network_header(skb);
	skb_reset_network_header(skb);

	if (!vxlan_ecn_decapsulate(vs, oiph, skb)) {
		++vxlan->dev->stats.rx_frame_errors;
		++vxlan->dev->stats.rx_errors;
		goto drop;
	}

	rcu_read_lock();

	if (unlikely(!(vxlan->dev->flags & IFF_UP))) {
		rcu_read_unlock();
		atomic_long_inc(&vxlan->dev->rx_dropped);
		goto drop;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	stats = this_cpu_ptr(vxlan->dev->tstats);
	u64_stats_update_begin(&stats->syncp);
	stats->rx_packets++;
	stats->rx_bytes += skb->len;
	u64_stats_update_end(&stats->syncp);

<<<<<<< HEAD
	netif_rx(skb);

	return 0;
error:
	/* Put UDP header back */
	__skb_push(skb, sizeof(struct udphdr));

	return 1;
=======
	gro_cells_receive(&vxlan->gro_cells, skb);

	rcu_read_unlock();

	return 0;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
drop:
	/* Consume bad packet */
	kfree_skb(skb);
	return 0;
}

static int arp_reduce(struct net_device *dev, struct sk_buff *skb)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct arphdr *parp;
	u8 *arpptr, *sha;
	__be32 sip, tip;
	struct neighbour *n;

	if (dev->flags & IFF_NOARP)
		goto out;

	if (!pskb_may_pull(skb, arp_hdr_len(dev))) {
		dev->stats.tx_dropped++;
		goto out;
	}
	parp = arp_hdr(skb);

	if ((parp->ar_hrd != htons(ARPHRD_ETHER) &&
	     parp->ar_hrd != htons(ARPHRD_IEEE802)) ||
	    parp->ar_pro != htons(ETH_P_IP) ||
	    parp->ar_op != htons(ARPOP_REQUEST) ||
	    parp->ar_hln != dev->addr_len ||
	    parp->ar_pln != 4)
		goto out;
	arpptr = (u8 *)parp + sizeof(struct arphdr);
	sha = arpptr;
	arpptr += dev->addr_len;	/* sha */
	memcpy(&sip, arpptr, sizeof(sip));
	arpptr += sizeof(sip);
	arpptr += dev->addr_len;	/* tha */
	memcpy(&tip, arpptr, sizeof(tip));

	if (ipv4_is_loopback(tip) ||
	    ipv4_is_multicast(tip))
		goto out;

	n = neigh_lookup(&arp_tbl, &tip, dev);

	if (n) {
		struct vxlan_fdb *f;
		struct sk_buff	*reply;

		if (!(n->nud_state & NUD_CONNECTED)) {
			neigh_release(n);
			goto out;
		}

		f = vxlan_find_mac(vxlan, n->ha);
<<<<<<< HEAD
		if (f && f->remote.remote_ip == htonl(INADDR_ANY)) {
=======
		if (f && vxlan_addr_any(&(first_remote_rcu(f)->remote_ip))) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* bridge-local neighbor */
			neigh_release(n);
			goto out;
		}

		reply = arp_create(ARPOP_REPLY, ETH_P_ARP, sip, dev, tip, sha,
				n->ha, sha);

		neigh_release(n);

		if (reply == NULL)
			goto out;

		skb_reset_mac_header(reply);
		__skb_pull(reply, skb_network_offset(reply));
		reply->ip_summed = CHECKSUM_UNNECESSARY;
		reply->pkt_type = PACKET_HOST;

		if (netif_rx_ni(reply) == NET_RX_DROP)
			dev->stats.rx_dropped++;
<<<<<<< HEAD
	} else if (vxlan->flags & VXLAN_F_L3MISS)
		vxlan_ip_miss(dev, tip);
out:
	consume_skb(skb);
	return NETDEV_TX_OK;
}
=======
	} else if (vxlan->flags & VXLAN_F_L3MISS) {
		union vxlan_addr ipa = {
			.sin.sin_addr.s_addr = tip,
			.sin.sin_family = AF_INET,
		};

		vxlan_ip_miss(dev, &ipa);
	}
out:
	consume_skb(skb);
	return NETDEV_TX_OK;
}

#if IS_ENABLED(CONFIG_IPV6)
static struct sk_buff *vxlan_na_create(struct sk_buff *request,
	struct neighbour *n, bool isrouter)
{
	struct net_device *dev = request->dev;
	struct sk_buff *reply;
	struct nd_msg *ns, *na;
	struct ipv6hdr *pip6;
	u8 *daddr;
	int na_olen = 8; /* opt hdr + ETH_ALEN for target */
	int ns_olen;
	int i, len;

	if (dev == NULL)
		return NULL;

	len = LL_RESERVED_SPACE(dev) + sizeof(struct ipv6hdr) +
		sizeof(*na) + na_olen + dev->needed_tailroom;
	reply = alloc_skb(len, GFP_ATOMIC);
	if (reply == NULL)
		return NULL;

	reply->protocol = htons(ETH_P_IPV6);
	reply->dev = dev;
	skb_reserve(reply, LL_RESERVED_SPACE(request->dev));
	skb_push(reply, sizeof(struct ethhdr));
	skb_reset_mac_header(reply);

	ns = (struct nd_msg *)skb_transport_header(request);

	daddr = eth_hdr(request)->h_source;
	ns_olen = request->len - skb_transport_offset(request) - sizeof(*ns);
	for (i = 0; i < ns_olen-1; i += (ns->opt[i+1]<<3)) {
		if (!ns->opt[i + 1]) {
			kfree_skb(reply);
			return NULL;
		}
		if (ns->opt[i] == ND_OPT_SOURCE_LL_ADDR) {
			daddr = ns->opt + i + sizeof(struct nd_opt_hdr);
			break;
		}
	}

	/* Ethernet header */
	ether_addr_copy(eth_hdr(reply)->h_dest, daddr);
	ether_addr_copy(eth_hdr(reply)->h_source, n->ha);
	eth_hdr(reply)->h_proto = htons(ETH_P_IPV6);
	reply->protocol = htons(ETH_P_IPV6);

	skb_pull(reply, sizeof(struct ethhdr));
	skb_reset_network_header(reply);
	skb_put(reply, sizeof(struct ipv6hdr));

	/* IPv6 header */

	pip6 = ipv6_hdr(reply);
	memset(pip6, 0, sizeof(struct ipv6hdr));
	pip6->version = 6;
	pip6->priority = ipv6_hdr(request)->priority;
	pip6->nexthdr = IPPROTO_ICMPV6;
	pip6->hop_limit = 255;
	pip6->daddr = ipv6_hdr(request)->saddr;
	pip6->saddr = *(struct in6_addr *)n->primary_key;

	skb_pull(reply, sizeof(struct ipv6hdr));
	skb_reset_transport_header(reply);

	na = (struct nd_msg *)skb_put(reply, sizeof(*na) + na_olen);

	/* Neighbor Advertisement */
	memset(na, 0, sizeof(*na)+na_olen);
	na->icmph.icmp6_type = NDISC_NEIGHBOUR_ADVERTISEMENT;
	na->icmph.icmp6_router = isrouter;
	na->icmph.icmp6_override = 1;
	na->icmph.icmp6_solicited = 1;
	na->target = ns->target;
	ether_addr_copy(&na->opt[2], n->ha);
	na->opt[0] = ND_OPT_TARGET_LL_ADDR;
	na->opt[1] = na_olen >> 3;

	na->icmph.icmp6_cksum = csum_ipv6_magic(&pip6->saddr,
		&pip6->daddr, sizeof(*na)+na_olen, IPPROTO_ICMPV6,
		csum_partial(na, sizeof(*na)+na_olen, 0));

	pip6->payload_len = htons(sizeof(*na)+na_olen);

	skb_push(reply, sizeof(struct ipv6hdr));

	reply->ip_summed = CHECKSUM_UNNECESSARY;

	return reply;
}

static int neigh_reduce(struct net_device *dev, struct sk_buff *skb)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct nd_msg *msg;
	const struct ipv6hdr *iphdr;
	const struct in6_addr *saddr, *daddr;
	struct neighbour *n;
	struct inet6_dev *in6_dev;

	rcu_read_lock();
	in6_dev = __in6_dev_get(dev);
	if (!in6_dev)
		goto out;

	iphdr = ipv6_hdr(skb);
	saddr = &iphdr->saddr;
	daddr = &iphdr->daddr;

	msg = (struct nd_msg *)skb_transport_header(skb);
	if (msg->icmph.icmp6_code != 0 ||
	    msg->icmph.icmp6_type != NDISC_NEIGHBOUR_SOLICITATION)
		goto out;

	if (ipv6_addr_loopback(daddr) ||
	    ipv6_addr_is_multicast(&msg->target))
		goto out;

	n = neigh_lookup(ipv6_stub->nd_tbl, &msg->target, dev);

	if (n) {
		struct vxlan_fdb *f;
		struct sk_buff *reply;

		if (!(n->nud_state & NUD_CONNECTED)) {
			neigh_release(n);
			goto out;
		}

		f = vxlan_find_mac(vxlan, n->ha);
		if (f && vxlan_addr_any(&(first_remote_rcu(f)->remote_ip))) {
			/* bridge-local neighbor */
			neigh_release(n);
			goto out;
		}

		reply = vxlan_na_create(skb, n,
					!!(f ? f->flags & NTF_ROUTER : 0));

		neigh_release(n);

		if (reply == NULL)
			goto out;

		if (netif_rx_ni(reply) == NET_RX_DROP)
			dev->stats.rx_dropped++;

	} else if (vxlan->flags & VXLAN_F_L3MISS) {
		union vxlan_addr ipa = {
			.sin6.sin6_addr = msg->target,
			.sin6.sin6_family = AF_INET6,
		};

		vxlan_ip_miss(dev, &ipa);
	}

out:
	rcu_read_unlock();
	consume_skb(skb);
	return NETDEV_TX_OK;
}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static bool route_shortcircuit(struct net_device *dev, struct sk_buff *skb)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct neighbour *n;
<<<<<<< HEAD
	struct iphdr *pip;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (is_multicast_ether_addr(eth_hdr(skb)->h_dest))
		return false;

	n = NULL;
	switch (ntohs(eth_hdr(skb)->h_proto)) {
	case ETH_P_IP:
<<<<<<< HEAD
=======
	{
		struct iphdr *pip;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!pskb_may_pull(skb, sizeof(struct iphdr)))
			return false;
		pip = ip_hdr(skb);
		n = neigh_lookup(&arp_tbl, &pip->daddr, dev);
<<<<<<< HEAD
		break;
=======
		if (!n && (vxlan->flags & VXLAN_F_L3MISS)) {
			union vxlan_addr ipa = {
				.sin.sin_addr.s_addr = pip->daddr,
				.sin.sin_family = AF_INET,
			};

			vxlan_ip_miss(dev, &ipa);
			return false;
		}

		break;
	}
#if IS_ENABLED(CONFIG_IPV6)
	case ETH_P_IPV6:
	{
		struct ipv6hdr *pip6;

		if (!pskb_may_pull(skb, sizeof(struct ipv6hdr)))
			return false;
		pip6 = ipv6_hdr(skb);
		n = neigh_lookup(ipv6_stub->nd_tbl, &pip6->daddr, dev);
		if (!n && (vxlan->flags & VXLAN_F_L3MISS)) {
			union vxlan_addr ipa = {
				.sin6.sin6_addr = pip6->daddr,
				.sin6.sin6_family = AF_INET6,
			};

			vxlan_ip_miss(dev, &ipa);
			return false;
		}

		break;
	}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	default:
		return false;
	}

	if (n) {
		bool diff;

<<<<<<< HEAD
		diff = compare_ether_addr(eth_hdr(skb)->h_dest, n->ha) != 0;
=======
		diff = !ether_addr_equal(eth_hdr(skb)->h_dest, n->ha);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (diff) {
			memcpy(eth_hdr(skb)->h_source, eth_hdr(skb)->h_dest,
				dev->addr_len);
			memcpy(eth_hdr(skb)->h_dest, n->ha, dev->addr_len);
		}
		neigh_release(n);
		return diff;
<<<<<<< HEAD
	} else if (vxlan->flags & VXLAN_F_L3MISS)
		vxlan_ip_miss(dev, pip->daddr);
	return false;
}

static void vxlan_sock_free(struct sk_buff *skb)
{
	sock_put(skb->sk);
}

/* On transmit, associate with the tunnel socket */
static void vxlan_set_owner(struct net_device *dev, struct sk_buff *skb)
{
	struct vxlan_net *vn = net_generic(dev_net(dev), vxlan_net_id);
	struct sock *sk = vn->sock->sk;

	skb_orphan(skb);
	sock_hold(sk);
	skb->sk = sk;
	skb->destructor = vxlan_sock_free;
}

/* Compute source port for outgoing packet
 *   first choice to use L4 flow hash since it will spread
 *     better and maybe available from hardware
 *   secondary choice is to use jhash on the Ethernet header
 */
static __be16 vxlan_src_port(const struct vxlan_dev *vxlan, struct sk_buff *skb)
{
	unsigned int range = (vxlan->port_max - vxlan->port_min) + 1;
	u32 hash;

	hash = skb_get_rxhash(skb);
	if (!hash)
		hash = jhash(skb->data, 2 * ETH_ALEN,
			     (__force u32) skb->protocol);

	return htons((((u64) hash * range) >> 32) + vxlan->port_min);
}

static int handle_offloads(struct sk_buff *skb)
{
	if (skb_is_gso(skb)) {
		int err = skb_unclone(skb, GFP_ATOMIC);
		if (unlikely(err))
			return err;

		skb_shinfo(skb)->gso_type |= SKB_GSO_UDP_TUNNEL;
	} else if (skb->ip_summed != CHECKSUM_PARTIAL)
		skb->ip_summed = CHECKSUM_NONE;

	return 0;
}

=======
	}

	return false;
}

static void vxlan_build_gbp_hdr(struct vxlanhdr *vxh, u32 vxflags,
				struct vxlan_metadata *md)
{
	struct vxlanhdr_gbp *gbp;

	if (!md->gbp)
		return;

	gbp = (struct vxlanhdr_gbp *)vxh;
	vxh->vx_flags |= VXLAN_HF_GBP;

	if (md->gbp & VXLAN_GBP_DONT_LEARN)
		gbp->dont_learn = 1;

	if (md->gbp & VXLAN_GBP_POLICY_APPLIED)
		gbp->policy_applied = 1;

	gbp->policy_id = htons(md->gbp & VXLAN_GBP_ID_MASK);
}

static int vxlan_build_gpe_hdr(struct vxlanhdr *vxh, u32 vxflags,
			       __be16 protocol)
{
	struct vxlanhdr_gpe *gpe = (struct vxlanhdr_gpe *)vxh;

	gpe->np_applied = 1;

	switch (protocol) {
	case htons(ETH_P_IP):
		gpe->next_protocol = VXLAN_GPE_NP_IPV4;
		return 0;
	case htons(ETH_P_IPV6):
		gpe->next_protocol = VXLAN_GPE_NP_IPV6;
		return 0;
	case htons(ETH_P_TEB):
		gpe->next_protocol = VXLAN_GPE_NP_ETHERNET;
		return 0;
	}
	return -EPFNOSUPPORT;
}

static int vxlan_build_skb(struct sk_buff *skb, struct dst_entry *dst,
			   int iphdr_len, __be32 vni,
			   struct vxlan_metadata *md, u32 vxflags,
			   bool udp_sum)
{
	struct vxlanhdr *vxh;
	int min_headroom;
	int err;
	int type = udp_sum ? SKB_GSO_UDP_TUNNEL_CSUM : SKB_GSO_UDP_TUNNEL;
	__be16 inner_protocol = htons(ETH_P_TEB);

	if ((vxflags & VXLAN_F_REMCSUM_TX) &&
	    skb->ip_summed == CHECKSUM_PARTIAL) {
		int csum_start = skb_checksum_start_offset(skb);

		if (csum_start <= VXLAN_MAX_REMCSUM_START &&
		    !(csum_start & VXLAN_RCO_SHIFT_MASK) &&
		    (skb->csum_offset == offsetof(struct udphdr, check) ||
		     skb->csum_offset == offsetof(struct tcphdr, check)))
			type |= SKB_GSO_TUNNEL_REMCSUM;
	}

	min_headroom = LL_RESERVED_SPACE(dst->dev) + dst->header_len
			+ VXLAN_HLEN + iphdr_len
			+ (skb_vlan_tag_present(skb) ? VLAN_HLEN : 0);

	/* Need space for new headers (invalidates iph ptr) */
	err = skb_cow_head(skb, min_headroom);
	if (unlikely(err))
		goto out_free;

	skb = vlan_hwaccel_push_inside(skb);
	if (WARN_ON(!skb))
		return -ENOMEM;

	err = iptunnel_handle_offloads(skb, type);
	if (err)
		goto out_free;

	vxh = (struct vxlanhdr *) __skb_push(skb, sizeof(*vxh));
	vxh->vx_flags = VXLAN_HF_VNI;
	vxh->vx_vni = vxlan_vni_field(vni);

	if (type & SKB_GSO_TUNNEL_REMCSUM) {
		unsigned int start;

		start = skb_checksum_start_offset(skb) - sizeof(struct vxlanhdr);
		vxh->vx_vni |= vxlan_compute_rco(start, skb->csum_offset);
		vxh->vx_flags |= VXLAN_HF_RCO;

		if (!skb_is_gso(skb)) {
			skb->ip_summed = CHECKSUM_NONE;
			skb->encapsulation = 0;
		}
	}

	if (vxflags & VXLAN_F_GBP)
		vxlan_build_gbp_hdr(vxh, vxflags, md);
	if (vxflags & VXLAN_F_GPE) {
		err = vxlan_build_gpe_hdr(vxh, vxflags, skb->protocol);
		if (err < 0)
			goto out_free;
		inner_protocol = skb->protocol;
	}

	skb_set_inner_protocol(skb, inner_protocol);
	return 0;

out_free:
	kfree_skb(skb);
	return err;
}

static struct rtable *vxlan_get_route(struct vxlan_dev *vxlan,
				      struct sk_buff *skb, int oif, u8 tos,
				      __be32 daddr, __be32 *saddr,
				      struct dst_cache *dst_cache,
				      const struct ip_tunnel_info *info)
{
	bool use_cache = ip_tunnel_dst_cache_usable(skb, info);
	struct rtable *rt = NULL;
	struct flowi4 fl4;

	if (tos && !info)
		use_cache = false;
	if (use_cache) {
		rt = dst_cache_get_ip4(dst_cache, saddr);
		if (rt)
			return rt;
	}

	memset(&fl4, 0, sizeof(fl4));
	fl4.flowi4_oif = oif;
	fl4.flowi4_tos = RT_TOS(tos);
	fl4.flowi4_mark = skb->mark;
	fl4.flowi4_proto = IPPROTO_UDP;
	fl4.daddr = daddr;
	fl4.saddr = *saddr;

	rt = ip_route_output_key(vxlan->net, &fl4);
	if (!IS_ERR(rt)) {
		*saddr = fl4.saddr;
		if (use_cache)
			dst_cache_set_ip4(dst_cache, &rt->dst, fl4.saddr);
	}
	return rt;
}

#if IS_ENABLED(CONFIG_IPV6)
static struct dst_entry *vxlan6_get_route(struct vxlan_dev *vxlan,
					  struct sk_buff *skb, int oif, u8 tos,
					  __be32 label,
					  const struct in6_addr *daddr,
					  struct in6_addr *saddr,
					  struct dst_cache *dst_cache,
					  const struct ip_tunnel_info *info)
{
	struct vxlan_sock *sock6 = rcu_dereference(vxlan->vn6_sock);
	bool use_cache = ip_tunnel_dst_cache_usable(skb, info);
	struct dst_entry *ndst;
	struct flowi6 fl6;

	if (!sock6)
		return ERR_PTR(-EIO);

	if (tos && !info)
		use_cache = false;
	if (use_cache) {
		ndst = dst_cache_get_ip6(dst_cache, saddr);
		if (ndst)
			return ndst;
	}

	memset(&fl6, 0, sizeof(fl6));
	fl6.flowi6_oif = oif;
	fl6.daddr = *daddr;
	fl6.saddr = *saddr;
	fl6.flowlabel = ip6_make_flowinfo(RT_TOS(tos), label);
	fl6.flowi6_mark = skb->mark;
	fl6.flowi6_proto = IPPROTO_UDP;

	ndst = ipv6_stub->ipv6_dst_lookup_flow(vxlan->net, sock6->sock->sk,
					       &fl6, NULL);
	if (unlikely(IS_ERR(ndst)))
		return ERR_PTR(-ENETUNREACH);

	*saddr = fl6.saddr;
	if (use_cache)
		dst_cache_set_ip6(dst_cache, ndst, saddr);
	return ndst;
}
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Bypass encapsulation if the destination is local */
static void vxlan_encap_bypass(struct sk_buff *skb, struct vxlan_dev *src_vxlan,
			       struct vxlan_dev *dst_vxlan)
{
<<<<<<< HEAD
	struct pcpu_tstats *tx_stats = this_cpu_ptr(src_vxlan->dev->tstats);
	struct pcpu_tstats *rx_stats = this_cpu_ptr(dst_vxlan->dev->tstats);

=======
	struct pcpu_sw_netstats *tx_stats, *rx_stats;
	union vxlan_addr loopback;
	union vxlan_addr *remote_ip = &dst_vxlan->default_dst.remote_ip;
	struct net_device *dev;
	int len = skb->len;

	tx_stats = this_cpu_ptr(src_vxlan->dev->tstats);
	rx_stats = this_cpu_ptr(dst_vxlan->dev->tstats);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	skb->pkt_type = PACKET_HOST;
	skb->encapsulation = 0;
	skb->dev = dst_vxlan->dev;
	__skb_pull(skb, skb_network_offset(skb));

<<<<<<< HEAD
	if (dst_vxlan->flags & VXLAN_F_LEARN)
		vxlan_snoop(skb->dev, htonl(INADDR_LOOPBACK),
			    eth_hdr(skb)->h_source);

	u64_stats_update_begin(&tx_stats->syncp);
	tx_stats->tx_packets++;
	tx_stats->tx_bytes += skb->len;
=======
	if (remote_ip->sa.sa_family == AF_INET) {
		loopback.sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		loopback.sa.sa_family =  AF_INET;
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		loopback.sin6.sin6_addr = in6addr_loopback;
		loopback.sa.sa_family =  AF_INET6;
#endif
	}

	rcu_read_lock();
	dev = skb->dev;
	if (unlikely(!(dev->flags & IFF_UP))) {
		kfree_skb(skb);
		goto drop;
	}

	if (dst_vxlan->flags & VXLAN_F_LEARN)
		vxlan_snoop(dev, &loopback, eth_hdr(skb)->h_source);

	u64_stats_update_begin(&tx_stats->syncp);
	tx_stats->tx_packets++;
	tx_stats->tx_bytes += len;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u64_stats_update_end(&tx_stats->syncp);

	if (netif_rx(skb) == NET_RX_SUCCESS) {
		u64_stats_update_begin(&rx_stats->syncp);
		rx_stats->rx_packets++;
<<<<<<< HEAD
		rx_stats->rx_bytes += skb->len;
		u64_stats_update_end(&rx_stats->syncp);
	} else {
		skb->dev->stats.rx_dropped++;
	}
}

static netdev_tx_t vxlan_xmit_one(struct sk_buff *skb, struct net_device *dev,
				  struct vxlan_rdst *rdst, bool did_rsc)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct rtable *rt;
	const struct iphdr *old_iph;
	struct iphdr *iph;
	struct vxlanhdr *vxh;
	struct udphdr *uh;
	struct flowi4 fl4;
	__be32 dst;
	__be16 src_port, dst_port;
        u32 vni;
	__be16 df = 0;
	__u8 tos, ttl;

	dst_port = rdst->remote_port ? rdst->remote_port : vxlan->dst_port;
	vni = rdst->remote_vni;
	dst = rdst->remote_ip;

	if (!dst) {
		if (did_rsc) {
			/* short-circuited back to local bridge */
			vxlan_encap_bypass(skb, vxlan, vxlan);
			return NETDEV_TX_OK;
=======
		rx_stats->rx_bytes += len;
		u64_stats_update_end(&rx_stats->syncp);
	} else {
drop:
		dev->stats.rx_dropped++;
	}
	rcu_read_unlock();
}

static void vxlan_xmit_one(struct sk_buff *skb, struct net_device *dev,
			   struct vxlan_rdst *rdst, bool did_rsc)
{
	struct dst_cache *dst_cache;
	struct ip_tunnel_info *info;
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct sock *sk;
	struct rtable *rt = NULL;
	const struct iphdr *old_iph;
	union vxlan_addr *dst;
	union vxlan_addr remote_ip, local_ip;
	struct vxlan_metadata _md;
	struct vxlan_metadata *md = &_md;
	__be16 src_port = 0, dst_port;
	__be32 vni, label;
	__be16 df = 0;
	__u8 tos, ttl;
	int err;
	u32 flags = vxlan->flags;
	bool udp_sum = false;
	bool xnet = !net_eq(vxlan->net, dev_net(vxlan->dev));

	info = skb_tunnel_info(skb);

	rcu_read_lock();
	if (rdst) {
		dst_port = rdst->remote_port ? rdst->remote_port : vxlan->cfg.dst_port;
		vni = rdst->remote_vni;
		dst = &rdst->remote_ip;
		local_ip = vxlan->cfg.saddr;
		dst_cache = &rdst->dst_cache;
	} else {
		if (!info) {
			WARN_ONCE(1, "%s: Missing encapsulation instructions\n",
				  dev->name);
			goto drop;
		}
		dst_port = info->key.tp_dst ? : vxlan->cfg.dst_port;
		vni = tunnel_id_to_key32(info->key.tun_id);
		remote_ip.sa.sa_family = ip_tunnel_info_af(info);
		if (remote_ip.sa.sa_family == AF_INET) {
			remote_ip.sin.sin_addr.s_addr = info->key.u.ipv4.dst;
			local_ip.sin.sin_addr.s_addr = info->key.u.ipv4.src;
		} else {
			remote_ip.sin6.sin6_addr = info->key.u.ipv6.dst;
			local_ip.sin6.sin6_addr = info->key.u.ipv6.src;
		}
		dst = &remote_ip;
		dst_cache = &info->dst_cache;
	}

	if (vxlan_addr_any(dst)) {
		if (did_rsc) {
			/* short-circuited back to local bridge */
			vxlan_encap_bypass(skb, vxlan, vxlan);
			goto out_unlock;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		goto drop;
	}

<<<<<<< HEAD
	if (!skb->encapsulation) {
		skb_reset_inner_headers(skb);
		skb->encapsulation = 1;
	}

	/* Need space for new headers (invalidates iph ptr) */
	if (skb_cow_head(skb, VXLAN_HEADROOM))
		goto drop;

	old_iph = ip_hdr(skb);

	ttl = vxlan->ttl;
	if (!ttl && IN_MULTICAST(ntohl(dst)))
		ttl = 1;

	tos = vxlan->tos;
	if (tos == 1)
		tos = ip_tunnel_get_dsfield(old_iph, skb);

	src_port = vxlan_src_port(vxlan, skb);

	memset(&fl4, 0, sizeof(fl4));
	fl4.flowi4_oif = rdst->remote_ifindex;
	fl4.flowi4_tos = RT_TOS(tos);
	fl4.daddr = dst;
	fl4.saddr = vxlan->saddr;

	rt = ip_route_output_key(dev_net(dev), &fl4);
	if (IS_ERR(rt)) {
		netdev_dbg(dev, "no route to %pI4\n", &dst);
		dev->stats.tx_carrier_errors++;
		goto tx_error;
	}

	if (rt->dst.dev == dev) {
		netdev_dbg(dev, "circular route to %pI4\n", &dst);
		ip_rt_put(rt);
		dev->stats.collisions++;
		goto tx_error;
	}

	/* Bypass encapsulation if the destination is local */
	if (rt->rt_flags & RTCF_LOCAL &&
	    !(rt->rt_flags & (RTCF_BROADCAST | RTCF_MULTICAST))) {
		struct vxlan_dev *dst_vxlan;

		ip_rt_put(rt);
		dst_vxlan = vxlan_find_vni(dev_net(dev), vni);
		if (!dst_vxlan)
			goto tx_error;
		vxlan_encap_bypass(skb, vxlan, dst_vxlan);
		return NETDEV_TX_OK;
	}

	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
	IPCB(skb)->flags &= ~(IPSKB_XFRM_TUNNEL_SIZE | IPSKB_XFRM_TRANSFORMED |
			      IPSKB_REROUTED);
	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);

	vxh = (struct vxlanhdr *) __skb_push(skb, sizeof(*vxh));
	vxh->vx_flags = htonl(VXLAN_FLAGS);
	vxh->vx_vni = htonl(vni << 8);

	__skb_push(skb, sizeof(*uh));
	skb_reset_transport_header(skb);
	uh = udp_hdr(skb);

	uh->dest = dst_port;
	uh->source = src_port;

	uh->len = htons(skb->len);
	uh->check = 0;

	__skb_push(skb, sizeof(*iph));
	skb_reset_network_header(skb);
	iph		= ip_hdr(skb);
	iph->version	= 4;
	iph->ihl	= sizeof(struct iphdr) >> 2;
	iph->frag_off	= df;
	iph->protocol	= IPPROTO_UDP;
	iph->tos	= ip_tunnel_ecn_encap(tos, old_iph, skb);
	iph->daddr	= dst;
	iph->saddr	= fl4.saddr;
	iph->ttl	= ttl ? : ip4_dst_hoplimit(&rt->dst);
	__ip_select_ident(iph, skb_shinfo(skb)->gso_segs ?: 1);

	nf_reset(skb);

	vxlan_set_owner(dev, skb);

	if (handle_offloads(skb))
		goto drop;

	iptunnel_xmit(skb, dev);
	return NETDEV_TX_OK;
=======
	old_iph = ip_hdr(skb);

	ttl = vxlan->cfg.ttl;
	if (!ttl && vxlan_addr_multicast(dst))
		ttl = 1;

	tos = vxlan->cfg.tos;
	if (tos == 1)
		tos = ip_tunnel_get_dsfield(old_iph, skb);

	label = vxlan->cfg.label;
	src_port = udp_flow_src_port(dev_net(dev), skb, vxlan->cfg.port_min,
				     vxlan->cfg.port_max, true);

	if (info) {
		ttl = info->key.ttl;
		tos = info->key.tos;
		label = info->key.label;
		udp_sum = !!(info->key.tun_flags & TUNNEL_CSUM);

		if (info->options_len) {
			if (info->options_len < sizeof(*md))
				goto drop;
			md = ip_tunnel_info_opts(info);
		}
	} else {
		md->gbp = skb->mark;
	}

	if (dst->sa.sa_family == AF_INET) {
		struct vxlan_sock *sock4 = rcu_dereference(vxlan->vn4_sock);

		if (!sock4)
			goto drop;
		sk = sock4->sock->sk;

		rt = vxlan_get_route(vxlan, skb,
				     rdst ? rdst->remote_ifindex : 0, tos,
				     dst->sin.sin_addr.s_addr,
				     &local_ip.sin.sin_addr.s_addr,
				     dst_cache, info);
		if (IS_ERR(rt)) {
			netdev_dbg(dev, "no route to %pI4\n",
				   &dst->sin.sin_addr.s_addr);
			dev->stats.tx_carrier_errors++;
			goto tx_error;
		}

		if (rt->dst.dev == dev) {
			netdev_dbg(dev, "circular route to %pI4\n",
				   &dst->sin.sin_addr.s_addr);
			dev->stats.collisions++;
			goto rt_tx_error;
		}

		/* Bypass encapsulation if the destination is local */
		if (!info && rt->rt_flags & RTCF_LOCAL &&
		    !(rt->rt_flags & (RTCF_BROADCAST | RTCF_MULTICAST))) {
			struct vxlan_dev *dst_vxlan;

			ip_rt_put(rt);
			dst_vxlan = vxlan_find_vni(vxlan->net, vni,
						   dst->sa.sa_family, dst_port,
						   vxlan->flags);
			if (!dst_vxlan)
				goto tx_error;
			vxlan_encap_bypass(skb, vxlan, dst_vxlan);
			goto out_unlock;
		}

		if (!info)
			udp_sum = !(flags & VXLAN_F_UDP_ZERO_CSUM_TX);
		else if (info->key.tun_flags & TUNNEL_DONT_FRAGMENT)
			df = htons(IP_DF);

		tos = ip_tunnel_ecn_encap(tos, old_iph, skb);
		ttl = ttl ? : ip4_dst_hoplimit(&rt->dst);
		err = vxlan_build_skb(skb, &rt->dst, sizeof(struct iphdr),
				      vni, md, flags, udp_sum);
		if (err < 0)
			goto xmit_tx_error;

		udp_tunnel_xmit_skb(rt, sk, skb, local_ip.sin.sin_addr.s_addr,
				    dst->sin.sin_addr.s_addr, tos, ttl, df,
				    src_port, dst_port, xnet, !udp_sum);
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		struct vxlan_sock *sock6 = rcu_dereference(vxlan->vn6_sock);
		struct dst_entry *ndst;
		u32 rt6i_flags;

		if (!sock6)
			goto drop;
		sk = sock6->sock->sk;

		ndst = vxlan6_get_route(vxlan, skb,
					rdst ? rdst->remote_ifindex : 0, tos,
					label, &dst->sin6.sin6_addr,
					&local_ip.sin6.sin6_addr,
					dst_cache, info);
		if (IS_ERR(ndst)) {
			netdev_dbg(dev, "no route to %pI6\n",
				   &dst->sin6.sin6_addr);
			dev->stats.tx_carrier_errors++;
			goto tx_error;
		}

		if (ndst->dev == dev) {
			netdev_dbg(dev, "circular route to %pI6\n",
				   &dst->sin6.sin6_addr);
			dst_release(ndst);
			dev->stats.collisions++;
			goto tx_error;
		}

		/* Bypass encapsulation if the destination is local */
		rt6i_flags = ((struct rt6_info *)ndst)->rt6i_flags;
		if (!info && rt6i_flags & RTF_LOCAL &&
		    !(rt6i_flags & (RTCF_BROADCAST | RTCF_MULTICAST))) {
			struct vxlan_dev *dst_vxlan;

			dst_release(ndst);
			dst_vxlan = vxlan_find_vni(vxlan->net, vni,
						   dst->sa.sa_family, dst_port,
						   vxlan->flags);
			if (!dst_vxlan)
				goto tx_error;
			vxlan_encap_bypass(skb, vxlan, dst_vxlan);
			goto out_unlock;
		}

		if (!info)
			udp_sum = !(flags & VXLAN_F_UDP_ZERO_CSUM6_TX);

		tos = ip_tunnel_ecn_encap(tos, old_iph, skb);
		ttl = ttl ? : ip6_dst_hoplimit(ndst);
		skb_scrub_packet(skb, xnet);
		err = vxlan_build_skb(skb, ndst, sizeof(struct ipv6hdr),
				      vni, md, flags, udp_sum);
		if (err < 0) {
			dst_release(ndst);
			dev->stats.tx_errors++;
			goto out_unlock;
		}
		udp_tunnel6_xmit_skb(ndst, sk, skb, dev,
				     &local_ip.sin6.sin6_addr,
				     &dst->sin6.sin6_addr, tos, ttl,
				     label, src_port, dst_port, !udp_sum);
#endif
	}
out_unlock:
	rcu_read_unlock();
	return;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

drop:
	dev->stats.tx_dropped++;
	goto tx_free;

<<<<<<< HEAD
=======
xmit_tx_error:
	/* skb is already freed. */
	skb = NULL;
rt_tx_error:
	ip_rt_put(rt);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
tx_error:
	dev->stats.tx_errors++;
tx_free:
	dev_kfree_skb(skb);
<<<<<<< HEAD
	return NETDEV_TX_OK;
=======
	rcu_read_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Transmit local packets over Vxlan
 *
 * Outer IP header inherits ECN and DF from inner header.
 * Outer UDP destination is the VXLAN assigned port.
 *           source port is based on hash of flow
 */
static netdev_tx_t vxlan_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
<<<<<<< HEAD
	struct ethhdr *eth;
	bool did_rsc = false;
	struct vxlan_rdst *rdst0, *rdst;
	struct vxlan_fdb *f;
	int rc1, rc;

	skb_reset_mac_header(skb);
	eth = eth_hdr(skb);

	if ((vxlan->flags & VXLAN_F_PROXY) && ntohs(eth->h_proto) == ETH_P_ARP)
		return arp_reduce(dev, skb);

=======
	const struct ip_tunnel_info *info;
	struct ethhdr *eth;
	bool did_rsc = false;
	struct vxlan_rdst *rdst, *fdst = NULL;
	struct vxlan_fdb *f;

	info = skb_tunnel_info(skb);

	skb_reset_mac_header(skb);

	if (vxlan->flags & VXLAN_F_COLLECT_METADATA) {
		if (info && info->mode & IP_TUNNEL_INFO_TX)
			vxlan_xmit_one(skb, dev, NULL, false);
		else
			kfree_skb(skb);
		return NETDEV_TX_OK;
	}

	if (vxlan->flags & VXLAN_F_PROXY) {
		eth = eth_hdr(skb);
		if (ntohs(eth->h_proto) == ETH_P_ARP)
			return arp_reduce(dev, skb);
#if IS_ENABLED(CONFIG_IPV6)
		else if (ntohs(eth->h_proto) == ETH_P_IPV6 &&
			 pskb_may_pull(skb, sizeof(struct ipv6hdr)
				       + sizeof(struct nd_msg)) &&
			 ipv6_hdr(skb)->nexthdr == IPPROTO_ICMPV6) {
				struct nd_msg *msg;

				msg = (struct nd_msg *)skb_transport_header(skb);
				if (msg->icmph.icmp6_code == 0 &&
				    msg->icmph.icmp6_type == NDISC_NEIGHBOUR_SOLICITATION)
					return neigh_reduce(dev, skb);
		}
#endif
	}

	eth = eth_hdr(skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	f = vxlan_find_mac(vxlan, eth->h_dest);
	did_rsc = false;

	if (f && (f->flags & NTF_ROUTER) && (vxlan->flags & VXLAN_F_RSC) &&
<<<<<<< HEAD
	    ntohs(eth->h_proto) == ETH_P_IP) {
=======
	    (ntohs(eth->h_proto) == ETH_P_IP ||
	     ntohs(eth->h_proto) == ETH_P_IPV6)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		did_rsc = route_shortcircuit(dev, skb);
		if (did_rsc)
			f = vxlan_find_mac(vxlan, eth->h_dest);
	}

	if (f == NULL) {
<<<<<<< HEAD
		rdst0 = &vxlan->default_dst;

		if (rdst0->remote_ip == htonl(INADDR_ANY) &&
		    (vxlan->flags & VXLAN_F_L2MISS) &&
		    !is_multicast_ether_addr(eth->h_dest))
			vxlan_fdb_miss(vxlan, eth->h_dest);
	} else
		rdst0 = &f->remote;

	rc = NETDEV_TX_OK;

	/* if there are multiple destinations, send copies */
	for (rdst = rdst0->remote_next; rdst; rdst = rdst->remote_next) {
		struct sk_buff *skb1;

		skb1 = skb_clone(skb, GFP_ATOMIC);
		if (skb1) {
			rc1 = vxlan_xmit_one(skb1, dev, rdst, did_rsc);
			if (rc == NETDEV_TX_OK)
				rc = rc1;
		}
	}

	rc1 = vxlan_xmit_one(skb, dev, rdst0, did_rsc);
	if (rc == NETDEV_TX_OK)
		rc = rc1;
	return rc;
=======
		f = vxlan_find_mac(vxlan, all_zeros_mac);
		if (f == NULL) {
			if ((vxlan->flags & VXLAN_F_L2MISS) &&
			    !is_multicast_ether_addr(eth->h_dest))
				vxlan_fdb_miss(vxlan, eth->h_dest);

			dev->stats.tx_dropped++;
			kfree_skb(skb);
			return NETDEV_TX_OK;
		}
	}

	list_for_each_entry_rcu(rdst, &f->remotes, list) {
		struct sk_buff *skb1;

		if (!fdst) {
			fdst = rdst;
			continue;
		}
		skb1 = skb_clone(skb, GFP_ATOMIC);
		if (skb1)
			vxlan_xmit_one(skb1, dev, rdst, did_rsc);
	}

	if (fdst)
		vxlan_xmit_one(skb, dev, fdst, did_rsc);
	else
		kfree_skb(skb);
	return NETDEV_TX_OK;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Walk the forwarding table and purge stale entries */
static void vxlan_cleanup(unsigned long arg)
{
	struct vxlan_dev *vxlan = (struct vxlan_dev *) arg;
	unsigned long next_timer = jiffies + FDB_AGE_INTERVAL;
	unsigned int h;

	if (!netif_running(vxlan->dev))
		return;

<<<<<<< HEAD
	spin_lock_bh(&vxlan->hash_lock);
	for (h = 0; h < FDB_HASH_SIZE; ++h) {
		struct hlist_node *p, *n;
=======
	for (h = 0; h < FDB_HASH_SIZE; ++h) {
		struct hlist_node *p, *n;

		spin_lock_bh(&vxlan->hash_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		hlist_for_each_safe(p, n, &vxlan->fdb_head[h]) {
			struct vxlan_fdb *f
				= container_of(p, struct vxlan_fdb, hlist);
			unsigned long timeout;

<<<<<<< HEAD
			if (f->state & NUD_PERMANENT)
				continue;

			timeout = f->used + vxlan->age_interval * HZ;
=======
			if (f->state & (NUD_PERMANENT | NUD_NOARP))
				continue;

			timeout = f->used + vxlan->cfg.age_interval * HZ;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (time_before_eq(timeout, jiffies)) {
				netdev_dbg(vxlan->dev,
					   "garbage collect %pM\n",
					   f->eth_addr);
				f->state = NUD_STALE;
				vxlan_fdb_destroy(vxlan, f);
			} else if (time_before(timeout, next_timer))
				next_timer = timeout;
		}
<<<<<<< HEAD
	}
	spin_unlock_bh(&vxlan->hash_lock);

	mod_timer(&vxlan->age_timer, next_timer);
=======
		spin_unlock_bh(&vxlan->hash_lock);
	}

	mod_timer(&vxlan->age_timer, next_timer);
}

static void vxlan_vs_del_dev(struct vxlan_dev *vxlan)
{
	struct vxlan_net *vn = net_generic(vxlan->net, vxlan_net_id);

	spin_lock(&vn->sock_lock);
	hlist_del_init_rcu(&vxlan->hlist4.hlist);
#if IS_ENABLED(CONFIG_IPV6)
	hlist_del_init_rcu(&vxlan->hlist6.hlist);
#endif
	spin_unlock(&vn->sock_lock);
}

static void vxlan_vs_add_dev(struct vxlan_sock *vs, struct vxlan_dev *vxlan,
			     struct vxlan_dev_node *node)
{
	struct vxlan_net *vn = net_generic(vxlan->net, vxlan_net_id);
	__be32 vni = vxlan->default_dst.remote_vni;

	node->vxlan = vxlan;
	spin_lock(&vn->sock_lock);
	hlist_add_head_rcu(&node->hlist, vni_head(vs, vni));
	spin_unlock(&vn->sock_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Setup stats when device is created */
static int vxlan_init(struct net_device *dev)
{
<<<<<<< HEAD
	dev->tstats = alloc_percpu(struct pcpu_tstats);
	if (!dev->tstats)
		return -ENOMEM;

	return 0;
}

=======
	struct vxlan_dev *vxlan = netdev_priv(dev);
	int err;

	dev->tstats = netdev_alloc_pcpu_stats(struct pcpu_sw_netstats);
	if (!dev->tstats)
		return -ENOMEM;

	err = gro_cells_init(&vxlan->gro_cells, dev);
	if (err) {
		free_percpu(dev->tstats);
		return err;
	}

	return 0;
}

static void vxlan_fdb_delete_default(struct vxlan_dev *vxlan)
{
	struct vxlan_fdb *f;

	spin_lock_bh(&vxlan->hash_lock);
	f = __vxlan_find_mac(vxlan, all_zeros_mac);
	if (f)
		vxlan_fdb_destroy(vxlan, f);
	spin_unlock_bh(&vxlan->hash_lock);
}

static void vxlan_uninit(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);

	gro_cells_destroy(&vxlan->gro_cells);

	vxlan_fdb_delete_default(vxlan);

	free_percpu(dev->tstats);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Start ageing timer and join group when device is brought up */
static int vxlan_open(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
<<<<<<< HEAD
	int err;

	if (IN_MULTICAST(ntohl(vxlan->default_dst.remote_ip))) {
		err = vxlan_join_group(dev);
		if (err)
			return err;
	}

	if (vxlan->age_interval)
		mod_timer(&vxlan->age_timer, jiffies + FDB_AGE_INTERVAL);

	return 0;
=======
	int ret;

	ret = vxlan_sock_add(vxlan);
	if (ret < 0)
		return ret;

	if (vxlan_addr_multicast(&vxlan->default_dst.remote_ip)) {
		ret = vxlan_igmp_join(vxlan);
		if (ret == -EADDRINUSE)
			ret = 0;
		if (ret) {
			vxlan_sock_release(vxlan);
			return ret;
		}
	}

	if (vxlan->cfg.age_interval)
		mod_timer(&vxlan->age_timer, jiffies + FDB_AGE_INTERVAL);

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Purge the forwarding table */
static void vxlan_flush(struct vxlan_dev *vxlan)
{
<<<<<<< HEAD
	unsigned h;
=======
	unsigned int h;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_bh(&vxlan->hash_lock);
	for (h = 0; h < FDB_HASH_SIZE; ++h) {
		struct hlist_node *p, *n;
		hlist_for_each_safe(p, n, &vxlan->fdb_head[h]) {
			struct vxlan_fdb *f
				= container_of(p, struct vxlan_fdb, hlist);
<<<<<<< HEAD
			vxlan_fdb_destroy(vxlan, f);
=======
			/* the all_zeros_mac entry is deleted at vxlan_uninit */
			if (!is_zero_ether_addr(f->eth_addr))
				vxlan_fdb_destroy(vxlan, f);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
	spin_unlock_bh(&vxlan->hash_lock);
}

/* Cleanup timer and forwarding table on shutdown */
static int vxlan_stop(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
<<<<<<< HEAD

	if (IN_MULTICAST(ntohl(vxlan->default_dst.remote_ip)))
		vxlan_leave_group(dev);
=======
	struct vxlan_net *vn = net_generic(vxlan->net, vxlan_net_id);
	int ret = 0;

	if (vxlan_addr_multicast(&vxlan->default_dst.remote_ip) &&
	    !vxlan_group_used(vn, vxlan))
		ret = vxlan_igmp_leave(vxlan);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	del_timer_sync(&vxlan->age_timer);

	vxlan_flush(vxlan);
<<<<<<< HEAD

	return 0;
=======
	vxlan_sock_release(vxlan);

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Stub, nothing needs to be done. */
static void vxlan_set_multicast_list(struct net_device *dev)
{
}

<<<<<<< HEAD
static const struct net_device_ops vxlan_netdev_ops = {
	.ndo_init		= vxlan_init,
=======
static int __vxlan_change_mtu(struct net_device *dev,
			      struct net_device *lowerdev,
			      struct vxlan_rdst *dst, int new_mtu, bool strict)
{
	int max_mtu = IP_MAX_MTU;

	if (lowerdev)
		max_mtu = lowerdev->mtu;

	if (dst->remote_ip.sa.sa_family == AF_INET6)
		max_mtu -= VXLAN6_HEADROOM;
	else
		max_mtu -= VXLAN_HEADROOM;

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

static int vxlan_change_mtu(struct net_device *dev, int new_mtu)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_rdst *dst = &vxlan->default_dst;
	struct net_device *lowerdev = __dev_get_by_index(vxlan->net,
							 dst->remote_ifindex);
	return __vxlan_change_mtu(dev, lowerdev, dst, new_mtu, true);
}

static int vxlan_fill_metadata_dst(struct net_device *dev, struct sk_buff *skb)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct ip_tunnel_info *info = skb_tunnel_info(skb);
	__be16 sport, dport;

	sport = udp_flow_src_port(dev_net(dev), skb, vxlan->cfg.port_min,
				  vxlan->cfg.port_max, true);
	dport = info->key.tp_dst ? : vxlan->cfg.dst_port;

	if (ip_tunnel_info_af(info) == AF_INET) {
		struct vxlan_sock *sock4 = rcu_dereference(vxlan->vn4_sock);
		struct rtable *rt;

		if (!sock4)
			return -EINVAL;
		rt = vxlan_get_route(vxlan, skb, 0, info->key.tos,
				     info->key.u.ipv4.dst,
				     &info->key.u.ipv4.src,
				     &info->dst_cache, info);
		if (IS_ERR(rt))
			return PTR_ERR(rt);
		ip_rt_put(rt);
	} else {
#if IS_ENABLED(CONFIG_IPV6)
		struct dst_entry *ndst;

		ndst = vxlan6_get_route(vxlan, skb, 0, info->key.tos,
					info->key.label, &info->key.u.ipv6.dst,
					&info->key.u.ipv6.src,
					&info->dst_cache, info);
		if (IS_ERR(ndst))
			return PTR_ERR(ndst);
		dst_release(ndst);
#else /* !CONFIG_IPV6 */
		return -EPFNOSUPPORT;
#endif
	}
	info->key.tp_src = sport;
	info->key.tp_dst = dport;
	return 0;
}

static const struct net_device_ops vxlan_netdev_ether_ops = {
	.ndo_init		= vxlan_init,
	.ndo_uninit		= vxlan_uninit,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.ndo_open		= vxlan_open,
	.ndo_stop		= vxlan_stop,
	.ndo_start_xmit		= vxlan_xmit,
	.ndo_get_stats64	= ip_tunnel_get_stats64,
	.ndo_set_rx_mode	= vxlan_set_multicast_list,
<<<<<<< HEAD
	.ndo_change_mtu		= eth_change_mtu,
=======
	.ndo_change_mtu		= vxlan_change_mtu,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_set_mac_address	= eth_mac_addr,
	.ndo_fdb_add		= vxlan_fdb_add,
	.ndo_fdb_del		= vxlan_fdb_delete,
	.ndo_fdb_dump		= vxlan_fdb_dump,
<<<<<<< HEAD
=======
	.ndo_fill_metadata_dst	= vxlan_fill_metadata_dst,
};

static const struct net_device_ops vxlan_netdev_raw_ops = {
	.ndo_init		= vxlan_init,
	.ndo_uninit		= vxlan_uninit,
	.ndo_open		= vxlan_open,
	.ndo_stop		= vxlan_stop,
	.ndo_start_xmit		= vxlan_xmit,
	.ndo_get_stats64	= ip_tunnel_get_stats64,
	.ndo_change_mtu		= vxlan_change_mtu,
	.ndo_fill_metadata_dst	= vxlan_fill_metadata_dst,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* Info for udev, that this is a virtual tunnel endpoint */
static struct device_type vxlan_type = {
	.name = "vxlan",
};

<<<<<<< HEAD
static void vxlan_free(struct net_device *dev)
{
	free_percpu(dev->tstats);
	free_netdev(dev);
=======
/* Calls the ndo_udp_tunnel_add of the caller in order to
 * supply the listening VXLAN udp ports. Callers are expected
 * to implement the ndo_udp_tunnel_add.
 */
static void vxlan_push_rx_ports(struct net_device *dev)
{
	struct vxlan_sock *vs;
	struct net *net = dev_net(dev);
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	unsigned int i;

	spin_lock(&vn->sock_lock);
	for (i = 0; i < PORT_HASH_SIZE; ++i) {
		hlist_for_each_entry_rcu(vs, &vn->sock_list[i], hlist)
			udp_tunnel_push_rx_port(dev, vs->sock,
						(vs->flags & VXLAN_F_GPE) ?
						UDP_TUNNEL_TYPE_VXLAN_GPE :
						UDP_TUNNEL_TYPE_VXLAN);
	}
	spin_unlock(&vn->sock_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Initialize the device structure. */
static void vxlan_setup(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
<<<<<<< HEAD
	unsigned h;
	int low, high;

	eth_hw_addr_random(dev);
	ether_setup(dev);
	dev->needed_headroom = ETH_HLEN + VXLAN_HEADROOM;

	dev->netdev_ops = &vxlan_netdev_ops;
	dev->destructor = vxlan_free;
	SET_NETDEV_DEVTYPE(dev, &vxlan_type);

	dev->tx_queue_len = 0;
	dev->features	|= NETIF_F_LLTX;
	dev->features	|= NETIF_F_NETNS_LOCAL;
=======
	unsigned int h;

	eth_hw_addr_random(dev);
	ether_setup(dev);

	dev->destructor = free_netdev;
	SET_NETDEV_DEVTYPE(dev, &vxlan_type);

	dev->features	|= NETIF_F_LLTX;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dev->features	|= NETIF_F_SG | NETIF_F_HW_CSUM;
	dev->features   |= NETIF_F_RXCSUM;
	dev->features   |= NETIF_F_GSO_SOFTWARE;

<<<<<<< HEAD
	dev->hw_features |= NETIF_F_SG | NETIF_F_HW_CSUM | NETIF_F_RXCSUM;
	dev->hw_features |= NETIF_F_GSO_SOFTWARE;
	dev->priv_flags	&= ~IFF_XMIT_DST_RELEASE;
	dev->priv_flags |= IFF_LIVE_ADDR_CHANGE;

=======
	dev->vlan_features = dev->features;
	dev->features |= NETIF_F_HW_VLAN_CTAG_TX | NETIF_F_HW_VLAN_STAG_TX;
	dev->hw_features |= NETIF_F_SG | NETIF_F_HW_CSUM | NETIF_F_RXCSUM;
	dev->hw_features |= NETIF_F_GSO_SOFTWARE;
	dev->hw_features |= NETIF_F_HW_VLAN_CTAG_TX | NETIF_F_HW_VLAN_STAG_TX;
	netif_keep_dst(dev);
	dev->priv_flags |= IFF_NO_QUEUE;

	INIT_LIST_HEAD(&vxlan->next);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_lock_init(&vxlan->hash_lock);

	init_timer_deferrable(&vxlan->age_timer);
	vxlan->age_timer.function = vxlan_cleanup;
	vxlan->age_timer.data = (unsigned long) vxlan;

<<<<<<< HEAD
	inet_get_local_port_range(&low, &high);
	vxlan->port_min = low;
	vxlan->port_max = high;
	vxlan->dst_port = htons(vxlan_port);
=======
	vxlan->cfg.dst_port = htons(vxlan_port);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	vxlan->dev = dev;

	for (h = 0; h < FDB_HASH_SIZE; ++h)
		INIT_HLIST_HEAD(&vxlan->fdb_head[h]);
}

<<<<<<< HEAD
static const struct nla_policy vxlan_policy[IFLA_VXLAN_MAX + 1] = {
	[IFLA_VXLAN_ID]		= { .type = NLA_U32 },
	[IFLA_VXLAN_GROUP]	= { .len = FIELD_SIZEOF(struct iphdr, daddr) },
	[IFLA_VXLAN_LINK]	= { .type = NLA_U32 },
	[IFLA_VXLAN_LOCAL]	= { .len = FIELD_SIZEOF(struct iphdr, saddr) },
	[IFLA_VXLAN_TOS]	= { .type = NLA_U8 },
	[IFLA_VXLAN_TTL]	= { .type = NLA_U8 },
=======
static void vxlan_ether_setup(struct net_device *dev)
{
	dev->priv_flags &= ~IFF_TX_SKB_SHARING;
	dev->priv_flags |= IFF_LIVE_ADDR_CHANGE;
	dev->netdev_ops = &vxlan_netdev_ether_ops;
}

static void vxlan_raw_setup(struct net_device *dev)
{
	dev->header_ops = NULL;
	dev->type = ARPHRD_NONE;
	dev->hard_header_len = 0;
	dev->addr_len = 0;
	dev->flags = IFF_POINTOPOINT | IFF_NOARP | IFF_MULTICAST;
	dev->netdev_ops = &vxlan_netdev_raw_ops;
}

static const struct nla_policy vxlan_policy[IFLA_VXLAN_MAX + 1] = {
	[IFLA_VXLAN_ID]		= { .type = NLA_U32 },
	[IFLA_VXLAN_GROUP]	= { .len = FIELD_SIZEOF(struct iphdr, daddr) },
	[IFLA_VXLAN_GROUP6]	= { .len = sizeof(struct in6_addr) },
	[IFLA_VXLAN_LINK]	= { .type = NLA_U32 },
	[IFLA_VXLAN_LOCAL]	= { .len = FIELD_SIZEOF(struct iphdr, saddr) },
	[IFLA_VXLAN_LOCAL6]	= { .len = sizeof(struct in6_addr) },
	[IFLA_VXLAN_TOS]	= { .type = NLA_U8 },
	[IFLA_VXLAN_TTL]	= { .type = NLA_U8 },
	[IFLA_VXLAN_LABEL]	= { .type = NLA_U32 },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	[IFLA_VXLAN_LEARNING]	= { .type = NLA_U8 },
	[IFLA_VXLAN_AGEING]	= { .type = NLA_U32 },
	[IFLA_VXLAN_LIMIT]	= { .type = NLA_U32 },
	[IFLA_VXLAN_PORT_RANGE] = { .len  = sizeof(struct ifla_vxlan_port_range) },
	[IFLA_VXLAN_PROXY]	= { .type = NLA_U8 },
	[IFLA_VXLAN_RSC]	= { .type = NLA_U8 },
	[IFLA_VXLAN_L2MISS]	= { .type = NLA_U8 },
	[IFLA_VXLAN_L3MISS]	= { .type = NLA_U8 },
<<<<<<< HEAD
	[IFLA_VXLAN_PORT]	= { .type = NLA_U16 },
=======
	[IFLA_VXLAN_COLLECT_METADATA]	= { .type = NLA_U8 },
	[IFLA_VXLAN_PORT]	= { .type = NLA_U16 },
	[IFLA_VXLAN_UDP_CSUM]	= { .type = NLA_U8 },
	[IFLA_VXLAN_UDP_ZERO_CSUM6_TX]	= { .type = NLA_U8 },
	[IFLA_VXLAN_UDP_ZERO_CSUM6_RX]	= { .type = NLA_U8 },
	[IFLA_VXLAN_REMCSUM_TX]	= { .type = NLA_U8 },
	[IFLA_VXLAN_REMCSUM_RX]	= { .type = NLA_U8 },
	[IFLA_VXLAN_GBP]	= { .type = NLA_FLAG, },
	[IFLA_VXLAN_GPE]	= { .type = NLA_FLAG, },
	[IFLA_VXLAN_REMCSUM_NOPARTIAL]	= { .type = NLA_FLAG },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int vxlan_validate(struct nlattr *tb[], struct nlattr *data[])
{
	if (tb[IFLA_ADDRESS]) {
		if (nla_len(tb[IFLA_ADDRESS]) != ETH_ALEN) {
			pr_debug("invalid link address (not ethernet)\n");
			return -EINVAL;
		}

		if (!is_valid_ether_addr(nla_data(tb[IFLA_ADDRESS]))) {
			pr_debug("invalid all zero ethernet address\n");
			return -EADDRNOTAVAIL;
		}
	}

	if (!data)
		return -EINVAL;

	if (data[IFLA_VXLAN_ID]) {
		__u32 id = nla_get_u32(data[IFLA_VXLAN_ID]);
		if (id >= VXLAN_N_VID)
			return -ERANGE;
	}

	if (data[IFLA_VXLAN_PORT_RANGE]) {
		const struct ifla_vxlan_port_range *p
			= nla_data(data[IFLA_VXLAN_PORT_RANGE]);

		if (ntohs(p->high) < ntohs(p->low)) {
			pr_debug("port range %u .. %u not valid\n",
				 ntohs(p->low), ntohs(p->high));
			return -EINVAL;
		}
	}

	return 0;
}

static void vxlan_get_drvinfo(struct net_device *netdev,
			      struct ethtool_drvinfo *drvinfo)
{
	strlcpy(drvinfo->version, VXLAN_VERSION, sizeof(drvinfo->version));
	strlcpy(drvinfo->driver, "vxlan", sizeof(drvinfo->driver));
}

static const struct ethtool_ops vxlan_ethtool_ops = {
	.get_drvinfo	= vxlan_get_drvinfo,
	.get_link	= ethtool_op_get_link,
};

<<<<<<< HEAD
static int vxlan_newlink(struct net *net, struct net_device *dev,
			 struct nlattr *tb[], struct nlattr *data[])
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_rdst *dst = &vxlan->default_dst;
	__u32 vni;
	int err;

	if (!data[IFLA_VXLAN_ID])
		return -EINVAL;

	vni = nla_get_u32(data[IFLA_VXLAN_ID]);
	if (vxlan_find_vni(net, vni)) {
		pr_info("duplicate VNI %u\n", vni);
		return -EEXIST;
	}
	dst->remote_vni = vni;

	if (data[IFLA_VXLAN_GROUP])
		dst->remote_ip = nla_get_be32(data[IFLA_VXLAN_GROUP]);

	if (data[IFLA_VXLAN_LOCAL])
		vxlan->saddr = nla_get_be32(data[IFLA_VXLAN_LOCAL]);

	if (data[IFLA_VXLAN_LINK] &&
	    (dst->remote_ifindex = nla_get_u32(data[IFLA_VXLAN_LINK]))) {
		struct net_device *lowerdev
			 = __dev_get_by_index(net, dst->remote_ifindex);
=======
static struct socket *vxlan_create_sock(struct net *net, bool ipv6,
					__be16 port, u32 flags)
{
	struct socket *sock;
	struct udp_port_cfg udp_conf;
	int err;

	memset(&udp_conf, 0, sizeof(udp_conf));

	if (ipv6) {
		udp_conf.family = AF_INET6;
		udp_conf.use_udp6_rx_checksums =
		    !(flags & VXLAN_F_UDP_ZERO_CSUM6_RX);
		udp_conf.ipv6_v6only = 1;
	} else {
		udp_conf.family = AF_INET;
	}

	udp_conf.local_udp_port = port;

	/* Open UDP socket */
	err = udp_sock_create(net, &udp_conf, &sock);
	if (err < 0)
		return ERR_PTR(err);

	return sock;
}

/* Create new listen socket if needed */
static struct vxlan_sock *vxlan_socket_create(struct net *net, bool ipv6,
					      __be16 port, u32 flags)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	struct vxlan_sock *vs;
	struct socket *sock;
	unsigned int h;
	struct udp_tunnel_sock_cfg tunnel_cfg;

	vs = kzalloc(sizeof(*vs), GFP_KERNEL);
	if (!vs)
		return ERR_PTR(-ENOMEM);

	for (h = 0; h < VNI_HASH_SIZE; ++h)
		INIT_HLIST_HEAD(&vs->vni_list[h]);

	sock = vxlan_create_sock(net, ipv6, port, flags);
	if (IS_ERR(sock)) {
		pr_info("Cannot bind port %d, err=%ld\n", ntohs(port),
			PTR_ERR(sock));
		kfree(vs);
		return ERR_CAST(sock);
	}

	vs->sock = sock;
	atomic_set(&vs->refcnt, 1);
	vs->flags = (flags & VXLAN_F_RCV_FLAGS);

	spin_lock(&vn->sock_lock);
	hlist_add_head_rcu(&vs->hlist, vs_head(net, port));
	udp_tunnel_notify_add_rx_port(sock,
				      (vs->flags & VXLAN_F_GPE) ?
				      UDP_TUNNEL_TYPE_VXLAN_GPE :
				      UDP_TUNNEL_TYPE_VXLAN);
	spin_unlock(&vn->sock_lock);

	/* Mark socket as an encapsulation socket. */
	memset(&tunnel_cfg, 0, sizeof(tunnel_cfg));
	tunnel_cfg.sk_user_data = vs;
	tunnel_cfg.encap_type = 1;
	tunnel_cfg.encap_rcv = vxlan_rcv;
	tunnel_cfg.encap_destroy = NULL;
	tunnel_cfg.gro_receive = vxlan_gro_receive;
	tunnel_cfg.gro_complete = vxlan_gro_complete;

	setup_udp_tunnel_sock(net, sock, &tunnel_cfg);

	return vs;
}

static int __vxlan_sock_add(struct vxlan_dev *vxlan, bool ipv6)
{
	struct vxlan_net *vn = net_generic(vxlan->net, vxlan_net_id);
	struct vxlan_sock *vs = NULL;
	struct vxlan_dev_node *node;

	if (!vxlan->cfg.no_share) {
		spin_lock(&vn->sock_lock);
		vs = vxlan_find_sock(vxlan->net, ipv6 ? AF_INET6 : AF_INET,
				     vxlan->cfg.dst_port, vxlan->flags);
		if (vs && !atomic_add_unless(&vs->refcnt, 1, 0)) {
			spin_unlock(&vn->sock_lock);
			return -EBUSY;
		}
		spin_unlock(&vn->sock_lock);
	}
	if (!vs)
		vs = vxlan_socket_create(vxlan->net, ipv6,
					 vxlan->cfg.dst_port, vxlan->flags);
	if (IS_ERR(vs))
		return PTR_ERR(vs);
#if IS_ENABLED(CONFIG_IPV6)
	if (ipv6) {
		rcu_assign_pointer(vxlan->vn6_sock, vs);
		node = &vxlan->hlist6;
	} else
#endif
	{
		rcu_assign_pointer(vxlan->vn4_sock, vs);
		node = &vxlan->hlist4;
	}
	vxlan_vs_add_dev(vs, vxlan, node);
	return 0;
}

static int vxlan_sock_add(struct vxlan_dev *vxlan)
{
	bool metadata = vxlan->flags & VXLAN_F_COLLECT_METADATA;
	bool ipv6 = vxlan->flags & VXLAN_F_IPV6 || metadata;
	bool ipv4 = !ipv6 || metadata;
	int ret = 0;

	RCU_INIT_POINTER(vxlan->vn4_sock, NULL);
#if IS_ENABLED(CONFIG_IPV6)
	RCU_INIT_POINTER(vxlan->vn6_sock, NULL);
	if (ipv6) {
		ret = __vxlan_sock_add(vxlan, true);
		if (ret < 0 && ret != -EAFNOSUPPORT)
			ipv4 = false;
	}
#endif
	if (ipv4)
		ret = __vxlan_sock_add(vxlan, false);
	if (ret < 0)
		vxlan_sock_release(vxlan);
	return ret;
}

static int vxlan_dev_configure(struct net *src_net, struct net_device *dev,
			       struct vxlan_config *conf)
{
	struct vxlan_net *vn = net_generic(src_net, vxlan_net_id);
	struct vxlan_dev *vxlan = netdev_priv(dev), *tmp;
	struct vxlan_rdst *dst = &vxlan->default_dst;
	unsigned short needed_headroom = ETH_HLEN;
	int err;
	bool use_ipv6 = false;
	__be16 default_port = vxlan->cfg.dst_port;
	struct net_device *lowerdev = NULL;

	if (conf->flags & VXLAN_F_GPE) {
		/* For now, allow GPE only together with COLLECT_METADATA.
		 * This can be relaxed later; in such case, the other side
		 * of the PtP link will have to be provided.
		 */
		if ((conf->flags & ~VXLAN_F_ALLOWED_GPE) ||
		    !(conf->flags & VXLAN_F_COLLECT_METADATA)) {
			pr_info("unsupported combination of extensions\n");
			return -EINVAL;
		}

		vxlan_raw_setup(dev);
	} else {
		vxlan_ether_setup(dev);
	}

	vxlan->net = src_net;

	dst->remote_vni = conf->vni;

	memcpy(&dst->remote_ip, &conf->remote_ip, sizeof(conf->remote_ip));

	/* Unless IPv6 is explicitly requested, assume IPv4 */
	if (!dst->remote_ip.sa.sa_family)
		dst->remote_ip.sa.sa_family = AF_INET;

	if (dst->remote_ip.sa.sa_family == AF_INET6 ||
	    vxlan->cfg.saddr.sa.sa_family == AF_INET6) {
		if (!IS_ENABLED(CONFIG_IPV6))
			return -EPFNOSUPPORT;
		use_ipv6 = true;
		vxlan->flags |= VXLAN_F_IPV6;
	}

	if (conf->label && !use_ipv6) {
		pr_info("label only supported in use with IPv6\n");
		return -EINVAL;
	}

	if (conf->remote_ifindex) {
		lowerdev = __dev_get_by_index(src_net, conf->remote_ifindex);
		dst->remote_ifindex = conf->remote_ifindex;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (!lowerdev) {
			pr_info("ifindex %d does not exist\n", dst->remote_ifindex);
			return -ENODEV;
		}

<<<<<<< HEAD
		if (!tb[IFLA_MTU])
			dev->mtu = lowerdev->mtu - VXLAN_HEADROOM;

		/* update header length based on lower device */
		dev->needed_headroom = lowerdev->hard_header_len +
				       VXLAN_HEADROOM;
	}

	if (data[IFLA_VXLAN_TOS])
		vxlan->tos  = nla_get_u8(data[IFLA_VXLAN_TOS]);

	if (data[IFLA_VXLAN_TTL])
		vxlan->ttl = nla_get_u8(data[IFLA_VXLAN_TTL]);

	if (!data[IFLA_VXLAN_LEARNING] || nla_get_u8(data[IFLA_VXLAN_LEARNING]))
		vxlan->flags |= VXLAN_F_LEARN;

	if (data[IFLA_VXLAN_AGEING])
		vxlan->age_interval = nla_get_u32(data[IFLA_VXLAN_AGEING]);
	else
		vxlan->age_interval = FDB_AGE_DEFAULT;

	if (data[IFLA_VXLAN_PROXY] && nla_get_u8(data[IFLA_VXLAN_PROXY]))
		vxlan->flags |= VXLAN_F_PROXY;

	if (data[IFLA_VXLAN_RSC] && nla_get_u8(data[IFLA_VXLAN_RSC]))
		vxlan->flags |= VXLAN_F_RSC;

	if (data[IFLA_VXLAN_L2MISS] && nla_get_u8(data[IFLA_VXLAN_L2MISS]))
		vxlan->flags |= VXLAN_F_L2MISS;

	if (data[IFLA_VXLAN_L3MISS] && nla_get_u8(data[IFLA_VXLAN_L3MISS]))
		vxlan->flags |= VXLAN_F_L3MISS;

	if (data[IFLA_VXLAN_LIMIT])
		vxlan->addrmax = nla_get_u32(data[IFLA_VXLAN_LIMIT]);
=======
#if IS_ENABLED(CONFIG_IPV6)
		if (use_ipv6) {
			struct inet6_dev *idev = __in6_dev_get(lowerdev);
			if (idev && idev->cnf.disable_ipv6) {
				pr_info("IPv6 is disabled via sysctl\n");
				return -EPERM;
			}
		}
#endif

		if (!conf->mtu)
			dev->mtu = lowerdev->mtu - (use_ipv6 ? VXLAN6_HEADROOM : VXLAN_HEADROOM);

		needed_headroom = lowerdev->hard_header_len;
	} else if (vxlan_addr_multicast(&dst->remote_ip)) {
		pr_info("multicast destination requires interface to be specified\n");
		return -EINVAL;
	}

	if (lowerdev) {
		dev->gso_max_size = lowerdev->gso_max_size;
		dev->gso_max_segs = lowerdev->gso_max_segs;
	}

	if (conf->mtu) {
		err = __vxlan_change_mtu(dev, lowerdev, dst, conf->mtu, false);
		if (err)
			return err;
	}

	if (use_ipv6 || conf->flags & VXLAN_F_COLLECT_METADATA)
		needed_headroom += VXLAN6_HEADROOM;
	else
		needed_headroom += VXLAN_HEADROOM;
	dev->needed_headroom = needed_headroom;

	memcpy(&vxlan->cfg, conf, sizeof(*conf));
	if (!vxlan->cfg.dst_port) {
		if (conf->flags & VXLAN_F_GPE)
			vxlan->cfg.dst_port = htons(4790); /* IANA VXLAN-GPE port */
		else
			vxlan->cfg.dst_port = default_port;
	}
	vxlan->flags |= conf->flags;

	if (!vxlan->cfg.age_interval)
		vxlan->cfg.age_interval = FDB_AGE_DEFAULT;

	list_for_each_entry(tmp, &vn->vxlan_list, next) {
		if (tmp->cfg.vni == conf->vni &&
		    (tmp->default_dst.remote_ip.sa.sa_family == AF_INET6 ||
		     tmp->cfg.saddr.sa.sa_family == AF_INET6) == use_ipv6 &&
		    tmp->cfg.dst_port == vxlan->cfg.dst_port &&
		    (tmp->flags & VXLAN_F_RCV_FLAGS) ==
		    (vxlan->flags & VXLAN_F_RCV_FLAGS)) {
			pr_info("duplicate VNI %u\n", be32_to_cpu(conf->vni));
			return -EEXIST;
		}
	}

	dev->ethtool_ops = &vxlan_ethtool_ops;

	/* create an fdb entry for a valid default destination */
	if (!vxlan_addr_any(&vxlan->default_dst.remote_ip)) {
		err = vxlan_fdb_create(vxlan, all_zeros_mac,
				       &vxlan->default_dst.remote_ip,
				       NUD_REACHABLE|NUD_PERMANENT,
				       NLM_F_EXCL|NLM_F_CREATE,
				       vxlan->cfg.dst_port,
				       vxlan->default_dst.remote_vni,
				       vxlan->default_dst.remote_ifindex,
				       NTF_SELF);
		if (err)
			return err;
	}

	err = register_netdevice(dev);
	if (err) {
		vxlan_fdb_delete_default(vxlan);
		return err;
	}

	list_add(&vxlan->next, &vn->vxlan_list);

	return 0;
}

static int vxlan_newlink(struct net *src_net, struct net_device *dev,
			 struct nlattr *tb[], struct nlattr *data[])
{
	struct vxlan_config conf;

	memset(&conf, 0, sizeof(conf));

	if (data[IFLA_VXLAN_ID])
		conf.vni = cpu_to_be32(nla_get_u32(data[IFLA_VXLAN_ID]));

	if (data[IFLA_VXLAN_GROUP]) {
		conf.remote_ip.sin.sin_addr.s_addr = nla_get_in_addr(data[IFLA_VXLAN_GROUP]);
	} else if (data[IFLA_VXLAN_GROUP6]) {
		if (!IS_ENABLED(CONFIG_IPV6))
			return -EPFNOSUPPORT;

		conf.remote_ip.sin6.sin6_addr = nla_get_in6_addr(data[IFLA_VXLAN_GROUP6]);
		conf.remote_ip.sa.sa_family = AF_INET6;
	}

	if (data[IFLA_VXLAN_LOCAL]) {
		conf.saddr.sin.sin_addr.s_addr = nla_get_in_addr(data[IFLA_VXLAN_LOCAL]);
		conf.saddr.sa.sa_family = AF_INET;
	} else if (data[IFLA_VXLAN_LOCAL6]) {
		if (!IS_ENABLED(CONFIG_IPV6))
			return -EPFNOSUPPORT;

		/* TODO: respect scope id */
		conf.saddr.sin6.sin6_addr = nla_get_in6_addr(data[IFLA_VXLAN_LOCAL6]);
		conf.saddr.sa.sa_family = AF_INET6;
	}

	if (data[IFLA_VXLAN_LINK])
		conf.remote_ifindex = nla_get_u32(data[IFLA_VXLAN_LINK]);

	if (data[IFLA_VXLAN_TOS])
		conf.tos  = nla_get_u8(data[IFLA_VXLAN_TOS]);

	if (data[IFLA_VXLAN_TTL])
		conf.ttl = nla_get_u8(data[IFLA_VXLAN_TTL]);

	if (data[IFLA_VXLAN_LABEL])
		conf.label = nla_get_be32(data[IFLA_VXLAN_LABEL]) &
			     IPV6_FLOWLABEL_MASK;

	if (!data[IFLA_VXLAN_LEARNING] || nla_get_u8(data[IFLA_VXLAN_LEARNING]))
		conf.flags |= VXLAN_F_LEARN;

	if (data[IFLA_VXLAN_AGEING])
		conf.age_interval = nla_get_u32(data[IFLA_VXLAN_AGEING]);

	if (data[IFLA_VXLAN_PROXY] && nla_get_u8(data[IFLA_VXLAN_PROXY]))
		conf.flags |= VXLAN_F_PROXY;

	if (data[IFLA_VXLAN_RSC] && nla_get_u8(data[IFLA_VXLAN_RSC]))
		conf.flags |= VXLAN_F_RSC;

	if (data[IFLA_VXLAN_L2MISS] && nla_get_u8(data[IFLA_VXLAN_L2MISS]))
		conf.flags |= VXLAN_F_L2MISS;

	if (data[IFLA_VXLAN_L3MISS] && nla_get_u8(data[IFLA_VXLAN_L3MISS]))
		conf.flags |= VXLAN_F_L3MISS;

	if (data[IFLA_VXLAN_LIMIT])
		conf.addrmax = nla_get_u32(data[IFLA_VXLAN_LIMIT]);

	if (data[IFLA_VXLAN_COLLECT_METADATA] &&
	    nla_get_u8(data[IFLA_VXLAN_COLLECT_METADATA]))
		conf.flags |= VXLAN_F_COLLECT_METADATA;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (data[IFLA_VXLAN_PORT_RANGE]) {
		const struct ifla_vxlan_port_range *p
			= nla_data(data[IFLA_VXLAN_PORT_RANGE]);
<<<<<<< HEAD
		vxlan->port_min = ntohs(p->low);
		vxlan->port_max = ntohs(p->high);
	}

	if (data[IFLA_VXLAN_PORT])
		vxlan->dst_port = nla_get_be16(data[IFLA_VXLAN_PORT]);

	SET_ETHTOOL_OPS(dev, &vxlan_ethtool_ops);

	err = register_netdevice(dev);
	if (!err)
		hlist_add_head_rcu(&vxlan->hlist, vni_head(net, dst->remote_vni));

	return err;
=======
		conf.port_min = ntohs(p->low);
		conf.port_max = ntohs(p->high);
	}

	if (data[IFLA_VXLAN_PORT])
		conf.dst_port = nla_get_be16(data[IFLA_VXLAN_PORT]);

	if (data[IFLA_VXLAN_UDP_CSUM] &&
	    !nla_get_u8(data[IFLA_VXLAN_UDP_CSUM]))
		conf.flags |= VXLAN_F_UDP_ZERO_CSUM_TX;

	if (data[IFLA_VXLAN_UDP_ZERO_CSUM6_TX] &&
	    nla_get_u8(data[IFLA_VXLAN_UDP_ZERO_CSUM6_TX]))
		conf.flags |= VXLAN_F_UDP_ZERO_CSUM6_TX;

	if (data[IFLA_VXLAN_UDP_ZERO_CSUM6_RX] &&
	    nla_get_u8(data[IFLA_VXLAN_UDP_ZERO_CSUM6_RX]))
		conf.flags |= VXLAN_F_UDP_ZERO_CSUM6_RX;

	if (data[IFLA_VXLAN_REMCSUM_TX] &&
	    nla_get_u8(data[IFLA_VXLAN_REMCSUM_TX]))
		conf.flags |= VXLAN_F_REMCSUM_TX;

	if (data[IFLA_VXLAN_REMCSUM_RX] &&
	    nla_get_u8(data[IFLA_VXLAN_REMCSUM_RX]))
		conf.flags |= VXLAN_F_REMCSUM_RX;

	if (data[IFLA_VXLAN_GBP])
		conf.flags |= VXLAN_F_GBP;

	if (data[IFLA_VXLAN_GPE])
		conf.flags |= VXLAN_F_GPE;

	if (data[IFLA_VXLAN_REMCSUM_NOPARTIAL])
		conf.flags |= VXLAN_F_REMCSUM_NOPARTIAL;

	if (tb[IFLA_MTU])
		conf.mtu = nla_get_u32(tb[IFLA_MTU]);

	return vxlan_dev_configure(src_net, dev, &conf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void vxlan_dellink(struct net_device *dev, struct list_head *head)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);

<<<<<<< HEAD
	hlist_del_rcu(&vxlan->hlist);

=======
	list_del(&vxlan->next);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unregister_netdevice_queue(dev, head);
}

static size_t vxlan_get_size(const struct net_device *dev)
{

	return nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_ID */
<<<<<<< HEAD
		nla_total_size(sizeof(__be32)) +/* IFLA_VXLAN_GROUP */
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_LINK */
		nla_total_size(sizeof(__be32))+	/* IFLA_VXLAN_LOCAL */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_TTL */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_TOS */
=======
		nla_total_size(sizeof(struct in6_addr)) + /* IFLA_VXLAN_GROUP{6} */
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_LINK */
		nla_total_size(sizeof(struct in6_addr)) + /* IFLA_VXLAN_LOCAL{6} */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_TTL */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_TOS */
		nla_total_size(sizeof(__be32)) + /* IFLA_VXLAN_LABEL */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_LEARNING */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_PROXY */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_RSC */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_L2MISS */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_L3MISS */
<<<<<<< HEAD
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_AGEING */
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_LIMIT */
		nla_total_size(sizeof(struct ifla_vxlan_port_range)) +
		nla_total_size(sizeof(__be16))+ /* IFLA_VXLAN_PORT */
=======
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_COLLECT_METADATA */
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_AGEING */
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_LIMIT */
		nla_total_size(sizeof(struct ifla_vxlan_port_range)) +
		nla_total_size(sizeof(__be16)) + /* IFLA_VXLAN_PORT */
		nla_total_size(sizeof(__u8)) + /* IFLA_VXLAN_UDP_CSUM */
		nla_total_size(sizeof(__u8)) + /* IFLA_VXLAN_UDP_ZERO_CSUM6_TX */
		nla_total_size(sizeof(__u8)) + /* IFLA_VXLAN_UDP_ZERO_CSUM6_RX */
		nla_total_size(sizeof(__u8)) + /* IFLA_VXLAN_REMCSUM_TX */
		nla_total_size(sizeof(__u8)) + /* IFLA_VXLAN_REMCSUM_RX */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		0;
}

static int vxlan_fill_info(struct sk_buff *skb, const struct net_device *dev)
{
	const struct vxlan_dev *vxlan = netdev_priv(dev);
	const struct vxlan_rdst *dst = &vxlan->default_dst;
	struct ifla_vxlan_port_range ports = {
<<<<<<< HEAD
		.low =  htons(vxlan->port_min),
		.high = htons(vxlan->port_max),
	};

	if (nla_put_u32(skb, IFLA_VXLAN_ID, dst->remote_vni))
		goto nla_put_failure;

	if (dst->remote_ip && nla_put_be32(skb, IFLA_VXLAN_GROUP, dst->remote_ip))
		goto nla_put_failure;
=======
		.low =  htons(vxlan->cfg.port_min),
		.high = htons(vxlan->cfg.port_max),
	};

	if (nla_put_u32(skb, IFLA_VXLAN_ID, be32_to_cpu(dst->remote_vni)))
		goto nla_put_failure;

	if (!vxlan_addr_any(&dst->remote_ip)) {
		if (dst->remote_ip.sa.sa_family == AF_INET) {
			if (nla_put_in_addr(skb, IFLA_VXLAN_GROUP,
					    dst->remote_ip.sin.sin_addr.s_addr))
				goto nla_put_failure;
#if IS_ENABLED(CONFIG_IPV6)
		} else {
			if (nla_put_in6_addr(skb, IFLA_VXLAN_GROUP6,
					     &dst->remote_ip.sin6.sin6_addr))
				goto nla_put_failure;
#endif
		}
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (dst->remote_ifindex && nla_put_u32(skb, IFLA_VXLAN_LINK, dst->remote_ifindex))
		goto nla_put_failure;

<<<<<<< HEAD
	if (vxlan->saddr && nla_put_be32(skb, IFLA_VXLAN_LOCAL, vxlan->saddr))
		goto nla_put_failure;

	if (nla_put_u8(skb, IFLA_VXLAN_TTL, vxlan->ttl) ||
	    nla_put_u8(skb, IFLA_VXLAN_TOS, vxlan->tos) ||
=======
	if (!vxlan_addr_any(&vxlan->cfg.saddr)) {
		if (vxlan->cfg.saddr.sa.sa_family == AF_INET) {
			if (nla_put_in_addr(skb, IFLA_VXLAN_LOCAL,
					    vxlan->cfg.saddr.sin.sin_addr.s_addr))
				goto nla_put_failure;
#if IS_ENABLED(CONFIG_IPV6)
		} else {
			if (nla_put_in6_addr(skb, IFLA_VXLAN_LOCAL6,
					     &vxlan->cfg.saddr.sin6.sin6_addr))
				goto nla_put_failure;
#endif
		}
	}

	if (nla_put_u8(skb, IFLA_VXLAN_TTL, vxlan->cfg.ttl) ||
	    nla_put_u8(skb, IFLA_VXLAN_TOS, vxlan->cfg.tos) ||
	    nla_put_be32(skb, IFLA_VXLAN_LABEL, vxlan->cfg.label) ||
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	    nla_put_u8(skb, IFLA_VXLAN_LEARNING,
			!!(vxlan->flags & VXLAN_F_LEARN)) ||
	    nla_put_u8(skb, IFLA_VXLAN_PROXY,
			!!(vxlan->flags & VXLAN_F_PROXY)) ||
	    nla_put_u8(skb, IFLA_VXLAN_RSC, !!(vxlan->flags & VXLAN_F_RSC)) ||
	    nla_put_u8(skb, IFLA_VXLAN_L2MISS,
			!!(vxlan->flags & VXLAN_F_L2MISS)) ||
	    nla_put_u8(skb, IFLA_VXLAN_L3MISS,
			!!(vxlan->flags & VXLAN_F_L3MISS)) ||
<<<<<<< HEAD
	    nla_put_u32(skb, IFLA_VXLAN_AGEING, vxlan->age_interval) ||
	    nla_put_u32(skb, IFLA_VXLAN_LIMIT, vxlan->addrmax) ||
	    nla_put_be16(skb, IFLA_VXLAN_PORT, vxlan->dst_port))
=======
	    nla_put_u8(skb, IFLA_VXLAN_COLLECT_METADATA,
		       !!(vxlan->flags & VXLAN_F_COLLECT_METADATA)) ||
	    nla_put_u32(skb, IFLA_VXLAN_AGEING, vxlan->cfg.age_interval) ||
	    nla_put_u32(skb, IFLA_VXLAN_LIMIT, vxlan->cfg.addrmax) ||
	    nla_put_be16(skb, IFLA_VXLAN_PORT, vxlan->cfg.dst_port) ||
	    nla_put_u8(skb, IFLA_VXLAN_UDP_CSUM,
			!(vxlan->flags & VXLAN_F_UDP_ZERO_CSUM_TX)) ||
	    nla_put_u8(skb, IFLA_VXLAN_UDP_ZERO_CSUM6_TX,
			!!(vxlan->flags & VXLAN_F_UDP_ZERO_CSUM6_TX)) ||
	    nla_put_u8(skb, IFLA_VXLAN_UDP_ZERO_CSUM6_RX,
			!!(vxlan->flags & VXLAN_F_UDP_ZERO_CSUM6_RX)) ||
	    nla_put_u8(skb, IFLA_VXLAN_REMCSUM_TX,
			!!(vxlan->flags & VXLAN_F_REMCSUM_TX)) ||
	    nla_put_u8(skb, IFLA_VXLAN_REMCSUM_RX,
			!!(vxlan->flags & VXLAN_F_REMCSUM_RX)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto nla_put_failure;

	if (nla_put(skb, IFLA_VXLAN_PORT_RANGE, sizeof(ports), &ports))
		goto nla_put_failure;

<<<<<<< HEAD
=======
	if (vxlan->flags & VXLAN_F_GBP &&
	    nla_put_flag(skb, IFLA_VXLAN_GBP))
		goto nla_put_failure;

	if (vxlan->flags & VXLAN_F_GPE &&
	    nla_put_flag(skb, IFLA_VXLAN_GPE))
		goto nla_put_failure;

	if (vxlan->flags & VXLAN_F_REMCSUM_NOPARTIAL &&
	    nla_put_flag(skb, IFLA_VXLAN_REMCSUM_NOPARTIAL))
		goto nla_put_failure;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;

nla_put_failure:
	return -EMSGSIZE;
}

<<<<<<< HEAD
=======
static struct net *vxlan_get_link_net(const struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);

	return vxlan->net;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct rtnl_link_ops vxlan_link_ops __read_mostly = {
	.kind		= "vxlan",
	.maxtype	= IFLA_VXLAN_MAX,
	.policy		= vxlan_policy,
	.priv_size	= sizeof(struct vxlan_dev),
	.setup		= vxlan_setup,
	.validate	= vxlan_validate,
	.newlink	= vxlan_newlink,
	.dellink	= vxlan_dellink,
	.get_size	= vxlan_get_size,
	.fill_info	= vxlan_fill_info,
<<<<<<< HEAD
};

static __net_init int vxlan_init_net(struct net *net)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	struct sock *sk;
	struct sockaddr_in vxlan_addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = htonl(INADDR_ANY),
	};
	int rc;
	unsigned h;

	/* Create UDP socket for encapsulation receive. */
	rc = sock_create_kern(AF_INET, SOCK_DGRAM, IPPROTO_UDP, &vn->sock);
	if (rc < 0) {
		pr_debug("UDP socket create failed\n");
		return rc;
	}
	/* Put in proper namespace */
	sk = vn->sock->sk;
	sk_change_net(sk, net);

	vxlan_addr.sin_port = htons(vxlan_port);

	rc = kernel_bind(vn->sock, (struct sockaddr *) &vxlan_addr,
			 sizeof(vxlan_addr));
	if (rc < 0) {
		pr_debug("bind for UDP socket %pI4:%u (%d)\n",
			 &vxlan_addr.sin_addr, ntohs(vxlan_addr.sin_port), rc);
		sk_release_kernel(sk);
		vn->sock = NULL;
		return rc;
	}

	/* Disable multicast loopback */
	inet_sk(sk)->mc_loop = 0;

	/* Mark socket as an encapsulation socket. */
	udp_sk(sk)->encap_type = 1;
	udp_sk(sk)->encap_rcv = vxlan_udp_encap_recv;
	udp_encap_enable();

	for (h = 0; h < VNI_HASH_SIZE; ++h)
		INIT_HLIST_HEAD(&vn->vni_list[h]);
=======
	.get_link_net	= vxlan_get_link_net,
};

struct net_device *vxlan_dev_create(struct net *net, const char *name,
				    u8 name_assign_type,
				    struct vxlan_config *conf)
{
	struct nlattr *tb[IFLA_MAX + 1];
	struct net_device *dev;
	int err;

	memset(&tb, 0, sizeof(tb));

	dev = rtnl_create_link(net, name, name_assign_type,
			       &vxlan_link_ops, tb);
	if (IS_ERR(dev))
		return dev;

	err = vxlan_dev_configure(net, dev, conf);
	if (err < 0) {
		free_netdev(dev);
		return ERR_PTR(err);
	}

	err = rtnl_configure_link(dev, NULL);
	if (err < 0) {
		LIST_HEAD(list_kill);

		vxlan_dellink(dev, &list_kill);
		unregister_netdevice_many(&list_kill);
		return ERR_PTR(err);
	}

	return dev;
}
EXPORT_SYMBOL_GPL(vxlan_dev_create);

static void vxlan_handle_lowerdev_unregister(struct vxlan_net *vn,
					     struct net_device *dev)
{
	struct vxlan_dev *vxlan, *next;
	LIST_HEAD(list_kill);

	list_for_each_entry_safe(vxlan, next, &vn->vxlan_list, next) {
		struct vxlan_rdst *dst = &vxlan->default_dst;

		/* In case we created vxlan device with carrier
		 * and we loose the carrier due to module unload
		 * we also need to remove vxlan device. In other
		 * cases, it's not necessary and remote_ifindex
		 * is 0 here, so no matches.
		 */
		if (dst->remote_ifindex == dev->ifindex)
			vxlan_dellink(vxlan->dev, &list_kill);
	}

	unregister_netdevice_many(&list_kill);
}

static int vxlan_netdevice_event(struct notifier_block *unused,
				 unsigned long event, void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	struct vxlan_net *vn = net_generic(dev_net(dev), vxlan_net_id);

	if (event == NETDEV_UNREGISTER)
		vxlan_handle_lowerdev_unregister(vn, dev);
	else if (event == NETDEV_UDP_TUNNEL_PUSH_INFO)
		vxlan_push_rx_ports(dev);

	return NOTIFY_DONE;
}

static struct notifier_block vxlan_notifier_block __read_mostly = {
	.notifier_call = vxlan_netdevice_event,
};

static __net_init int vxlan_init_net(struct net *net)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	unsigned int h;

	INIT_LIST_HEAD(&vn->vxlan_list);
	spin_lock_init(&vn->sock_lock);

	for (h = 0; h < PORT_HASH_SIZE; ++h)
		INIT_HLIST_HEAD(&vn->sock_list[h]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static __net_exit void vxlan_exit_net(struct net *net)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	struct vxlan_dev *vxlan;
	unsigned h;

	rtnl_lock();
	for (h = 0; h < VNI_HASH_SIZE; ++h)
		hlist_for_each_entry(vxlan, &vn->vni_list[h], hlist)
			dev_close(vxlan->dev);
	rtnl_unlock();

	if (vn->sock) {
		sk_release_kernel(vn->sock->sk);
		vn->sock = NULL;
	}
=======
static void __net_exit vxlan_exit_net(struct net *net)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	struct vxlan_dev *vxlan, *next;
	struct net_device *dev, *aux;
	LIST_HEAD(list);

	rtnl_lock();
	for_each_netdev_safe(net, dev, aux)
		if (dev->rtnl_link_ops == &vxlan_link_ops)
			unregister_netdevice_queue(dev, &list);

	list_for_each_entry_safe(vxlan, next, &vn->vxlan_list, next) {
		/* If vxlan->dev is in the same netns, it has already been added
		 * to the list by the previous loop.
		 */
		if (!net_eq(dev_net(vxlan->dev), net))
			unregister_netdevice_queue(vxlan->dev, &list);
	}

	unregister_netdevice_many(&list);
	rtnl_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static struct pernet_operations vxlan_net_ops = {
	.init = vxlan_init_net,
	.exit = vxlan_exit_net,
	.id   = &vxlan_net_id,
	.size = sizeof(struct vxlan_net),
};

static int __init vxlan_init_module(void)
{
	int rc;

	get_random_bytes(&vxlan_salt, sizeof(vxlan_salt));

<<<<<<< HEAD
	rc = register_pernet_device(&vxlan_net_ops);
	if (rc)
		goto out1;

	rc = rtnl_link_register(&vxlan_link_ops);
	if (rc)
		goto out2;

	return 0;

out2:
	unregister_pernet_device(&vxlan_net_ops);
out1:
	return rc;
}
module_init(vxlan_init_module);
=======
	rc = register_pernet_subsys(&vxlan_net_ops);
	if (rc)
		goto out1;

	rc = register_netdevice_notifier(&vxlan_notifier_block);
	if (rc)
		goto out2;

	rc = rtnl_link_register(&vxlan_link_ops);
	if (rc)
		goto out3;

	return 0;
out3:
	unregister_netdevice_notifier(&vxlan_notifier_block);
out2:
	unregister_pernet_subsys(&vxlan_net_ops);
out1:
	return rc;
}
late_initcall(vxlan_init_module);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void __exit vxlan_cleanup_module(void)
{
	rtnl_link_unregister(&vxlan_link_ops);
<<<<<<< HEAD
	unregister_pernet_device(&vxlan_net_ops);
	rcu_barrier();
=======
	unregister_netdevice_notifier(&vxlan_notifier_block);
	unregister_pernet_subsys(&vxlan_net_ops);
	/* rcu_barrier() is called by netns */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
module_exit(vxlan_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_VERSION(VXLAN_VERSION);
MODULE_AUTHOR("Stephen Hemminger <stephen@networkplumber.org>");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("Driver for VXLAN encapsulated traffic");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_ALIAS_RTNL_LINK("vxlan");
