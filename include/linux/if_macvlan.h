#ifndef _LINUX_IF_MACVLAN_H
#define _LINUX_IF_MACVLAN_H

#include <linux/if_link.h>
<<<<<<< HEAD
=======
#include <linux/if_vlan.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/list.h>
#include <linux/netdevice.h>
#include <linux/netlink.h>
#include <net/netlink.h>
#include <linux/u64_stats_sync.h>

<<<<<<< HEAD
#if defined(CONFIG_MACVTAP) || defined(CONFIG_MACVTAP_MODULE)
=======
#if IS_ENABLED(CONFIG_MACVTAP)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct socket *macvtap_get_socket(struct file *);
#else
#include <linux/err.h>
#include <linux/errno.h>
struct file;
struct socket;
static inline struct socket *macvtap_get_socket(struct file *f)
{
	return ERR_PTR(-EINVAL);
}
#endif /* CONFIG_MACVTAP */

struct macvlan_port;
struct macvtap_queue;

<<<<<<< HEAD
/**
 *	struct macvlan_pcpu_stats - MACVLAN percpu stats
 *	@rx_packets: number of received packets
 *	@rx_bytes: number of received bytes
 *	@rx_multicast: number of received multicast packets
 *	@tx_packets: number of transmitted packets
 *	@tx_bytes: number of transmitted bytes
 *	@syncp: synchronization point for 64bit counters
 *	@rx_errors: number of rx errors
 *	@tx_dropped: number of tx dropped packets
 */
struct macvlan_pcpu_stats {
	u64			rx_packets;
	u64			rx_bytes;
	u64			rx_multicast;
	u64			tx_packets;
	u64			tx_bytes;
	struct u64_stats_sync	syncp;
	u32			rx_errors;
	u32			tx_dropped;
};

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Maximum times a macvtap device can be opened. This can be used to
 * configure the number of receive queue, e.g. for multiqueue virtio.
 */
<<<<<<< HEAD
#define MAX_MACVTAP_QUEUES	(NR_CPUS < 16 ? NR_CPUS : 16)
=======
#define MAX_MACVTAP_QUEUES	256
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define MACVLAN_MC_FILTER_BITS	8
#define MACVLAN_MC_FILTER_SZ	(1 << MACVLAN_MC_FILTER_BITS)

struct macvlan_dev {
	struct net_device	*dev;
	struct list_head	list;
	struct hlist_node	hlist;
	struct macvlan_port	*port;
	struct net_device	*lowerdev;
<<<<<<< HEAD
	struct macvlan_pcpu_stats __percpu *pcpu_stats;

	DECLARE_BITMAP(mc_filter, MACVLAN_MC_FILTER_SZ);

	enum macvlan_mode	mode;
	u16			flags;
	int (*receive)(struct sk_buff *skb);
	int (*forward)(struct net_device *dev, struct sk_buff *skb);
	struct macvtap_queue	*taps[MAX_MACVTAP_QUEUES];
	int			numvtaps;
	int			minor;
=======
	void			*fwd_priv;
	struct vlan_pcpu_stats __percpu *pcpu_stats;

	DECLARE_BITMAP(mc_filter, MACVLAN_MC_FILTER_SZ);

	netdev_features_t	set_features;
	enum macvlan_mode	mode;
	u16			flags;
	/* This array tracks active taps. */
	struct macvtap_queue	__rcu *taps[MAX_MACVTAP_QUEUES];
	/* This list tracks all taps (both enabled and disabled) */
	struct list_head	queue_list;
	int			numvtaps;
	int			numqueues;
	netdev_features_t	tap_features;
	int			minor;
	int			nest_level;
#ifdef CONFIG_NET_POLL_CONTROLLER
	struct netpoll		*netpoll;
#endif
	unsigned int		macaddr_count;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static inline void macvlan_count_rx(const struct macvlan_dev *vlan,
				    unsigned int len, bool success,
				    bool multicast)
{
	if (likely(success)) {
<<<<<<< HEAD
		struct macvlan_pcpu_stats *pcpu_stats;

		pcpu_stats = this_cpu_ptr(vlan->pcpu_stats);
=======
		struct vlan_pcpu_stats *pcpu_stats;

		pcpu_stats = get_cpu_ptr(vlan->pcpu_stats);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		u64_stats_update_begin(&pcpu_stats->syncp);
		pcpu_stats->rx_packets++;
		pcpu_stats->rx_bytes += len;
		if (multicast)
			pcpu_stats->rx_multicast++;
		u64_stats_update_end(&pcpu_stats->syncp);
<<<<<<< HEAD
=======
		put_cpu_ptr(vlan->pcpu_stats);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else {
		this_cpu_inc(vlan->pcpu_stats->rx_errors);
	}
}

extern void macvlan_common_setup(struct net_device *dev);

extern int macvlan_common_newlink(struct net *src_net, struct net_device *dev,
<<<<<<< HEAD
				  struct nlattr *tb[], struct nlattr *data[],
				  int (*receive)(struct sk_buff *skb),
				  int (*forward)(struct net_device *dev,
						 struct sk_buff *skb));
=======
				  struct nlattr *tb[], struct nlattr *data[]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern void macvlan_count_rx(const struct macvlan_dev *vlan,
			     unsigned int len, bool success,
			     bool multicast);

extern void macvlan_dellink(struct net_device *dev, struct list_head *head);

extern int macvlan_link_register(struct rtnl_link_ops *ops);

<<<<<<< HEAD
extern netdev_tx_t macvlan_start_xmit(struct sk_buff *skb,
				      struct net_device *dev);
=======
#if IS_ENABLED(CONFIG_MACVLAN)
static inline struct net_device *
macvlan_dev_real_dev(const struct net_device *dev)
{
	struct macvlan_dev *macvlan = netdev_priv(dev);

	return macvlan->lowerdev;
}
#else
static inline struct net_device *
macvlan_dev_real_dev(const struct net_device *dev)
{
	BUG();
	return NULL;
}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _LINUX_IF_MACVLAN_H */
