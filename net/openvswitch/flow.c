/*
<<<<<<< HEAD
 * Copyright (c) 2007-2011 Nicira, Inc.
=======
 * Copyright (c) 2007-2014 Nicira, Inc.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
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

<<<<<<< HEAD
#include "flow.h"
#include "datapath.h"
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/uaccess.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <net/llc_pdu.h>
#include <linux/kernel.h>
#include <linux/jhash.h>
#include <linux/jiffies.h>
#include <linux/llc.h>
#include <linux/module.h>
#include <linux/in.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD
#include <linux/if_arp.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
=======
#include <linux/cpumask.h>
#include <linux/if_arp.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/mpls.h>
#include <linux/sctp.h>
#include <linux/smp.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/icmpv6.h>
#include <linux/rculist.h>
#include <net/ip.h>
<<<<<<< HEAD
#include <net/ipv6.h>
#include <net/ndisc.h>

static struct kmem_cache *flow_cache;
=======
#include <net/ip_tunnels.h>
#include <net/ipv6.h>
#include <net/mpls.h>
#include <net/ndisc.h>

#include "conntrack.h"
#include "datapath.h"
#include "flow.h"
#include "flow_netlink.h"
#include "vport.h"

u64 ovs_flow_used_time(unsigned long flow_jiffies)
{
	struct timespec cur_ts;
	u64 cur_ms, idle_ms;

	ktime_get_ts(&cur_ts);
	idle_ms = jiffies_to_msecs(jiffies - flow_jiffies);
	cur_ms = (u64)cur_ts.tv_sec * MSEC_PER_SEC +
		 cur_ts.tv_nsec / NSEC_PER_MSEC;

	return cur_ms - idle_ms;
}

#define TCP_FLAGS_BE16(tp) (*(__be16 *)&tcp_flag_word(tp) & htons(0x0FFF))

void ovs_flow_stats_update(struct sw_flow *flow, __be16 tcp_flags,
			   const struct sk_buff *skb)
{
	struct flow_stats *stats;
	int node = numa_node_id();
	int cpu = smp_processor_id();
	int len = skb->len + (skb_vlan_tag_present(skb) ? VLAN_HLEN : 0);

	stats = rcu_dereference(flow->stats[cpu]);

	/* Check if already have CPU-specific stats. */
	if (likely(stats)) {
		spin_lock(&stats->lock);
		/* Mark if we write on the pre-allocated stats. */
		if (cpu == 0 && unlikely(flow->stats_last_writer != cpu))
			flow->stats_last_writer = cpu;
	} else {
		stats = rcu_dereference(flow->stats[0]); /* Pre-allocated. */
		spin_lock(&stats->lock);

		/* If the current CPU is the only writer on the
		 * pre-allocated stats keep using them.
		 */
		if (unlikely(flow->stats_last_writer != cpu)) {
			/* A previous locker may have already allocated the
			 * stats, so we need to check again.  If CPU-specific
			 * stats were already allocated, we update the pre-
			 * allocated stats as we have already locked them.
			 */
			if (likely(flow->stats_last_writer != -1) &&
			    likely(!rcu_access_pointer(flow->stats[cpu]))) {
				/* Try to allocate CPU-specific stats. */
				struct flow_stats *new_stats;

				new_stats =
					kmem_cache_alloc_node(flow_stats_cache,
							      GFP_NOWAIT |
							      __GFP_THISNODE |
							      __GFP_NOWARN |
							      __GFP_NOMEMALLOC,
							      node);
				if (likely(new_stats)) {
					new_stats->used = jiffies;
					new_stats->packet_count = 1;
					new_stats->byte_count = len;
					new_stats->tcp_flags = tcp_flags;
					spin_lock_init(&new_stats->lock);

					rcu_assign_pointer(flow->stats[cpu],
							   new_stats);
					goto unlock;
				}
			}
			flow->stats_last_writer = cpu;
		}
	}

	stats->used = jiffies;
	stats->packet_count++;
	stats->byte_count += len;
	stats->tcp_flags |= tcp_flags;
unlock:
	spin_unlock(&stats->lock);
}

/* Must be called with rcu_read_lock or ovs_mutex. */
void ovs_flow_stats_get(const struct sw_flow *flow,
			struct ovs_flow_stats *ovs_stats,
			unsigned long *used, __be16 *tcp_flags)
{
	int cpu;

	*used = 0;
	*tcp_flags = 0;
	memset(ovs_stats, 0, sizeof(*ovs_stats));

	/* We open code this to make sure cpu 0 is always considered */
	for (cpu = 0; cpu < nr_cpu_ids; cpu = cpumask_next(cpu, cpu_possible_mask)) {
		struct flow_stats *stats = rcu_dereference_ovsl(flow->stats[cpu]);

		if (stats) {
			/* Local CPU may write on non-local stats, so we must
			 * block bottom-halves here.
			 */
			spin_lock_bh(&stats->lock);
			if (!*used || time_after(stats->used, *used))
				*used = stats->used;
			*tcp_flags |= stats->tcp_flags;
			ovs_stats->n_packets += stats->packet_count;
			ovs_stats->n_bytes += stats->byte_count;
			spin_unlock_bh(&stats->lock);
		}
	}
}

/* Called with ovs_mutex. */
void ovs_flow_stats_clear(struct sw_flow *flow)
{
	int cpu;

	/* We open code this to make sure cpu 0 is always considered */
	for (cpu = 0; cpu < nr_cpu_ids; cpu = cpumask_next(cpu, cpu_possible_mask)) {
		struct flow_stats *stats = ovsl_dereference(flow->stats[cpu]);

		if (stats) {
			spin_lock_bh(&stats->lock);
			stats->used = 0;
			stats->packet_count = 0;
			stats->byte_count = 0;
			stats->tcp_flags = 0;
			spin_unlock_bh(&stats->lock);
		}
	}
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int check_header(struct sk_buff *skb, int len)
{
	if (unlikely(skb->len < len))
		return -EINVAL;
	if (unlikely(!pskb_may_pull(skb, len)))
		return -ENOMEM;
	return 0;
}

static bool arphdr_ok(struct sk_buff *skb)
{
	return pskb_may_pull(skb, skb_network_offset(skb) +
				  sizeof(struct arp_eth_header));
}

static int check_iphdr(struct sk_buff *skb)
{
	unsigned int nh_ofs = skb_network_offset(skb);
	unsigned int ip_len;
	int err;

	err = check_header(skb, nh_ofs + sizeof(struct iphdr));
	if (unlikely(err))
		return err;

	ip_len = ip_hdrlen(skb);
	if (unlikely(ip_len < sizeof(struct iphdr) ||
		     skb->len < nh_ofs + ip_len))
		return -EINVAL;

	skb_set_transport_header(skb, nh_ofs + ip_len);
	return 0;
}

static bool tcphdr_ok(struct sk_buff *skb)
{
	int th_ofs = skb_transport_offset(skb);
	int tcp_len;

	if (unlikely(!pskb_may_pull(skb, th_ofs + sizeof(struct tcphdr))))
		return false;

	tcp_len = tcp_hdrlen(skb);
	if (unlikely(tcp_len < sizeof(struct tcphdr) ||
		     skb->len < th_ofs + tcp_len))
		return false;

	return true;
}

static bool udphdr_ok(struct sk_buff *skb)
{
	return pskb_may_pull(skb, skb_transport_offset(skb) +
				  sizeof(struct udphdr));
}

<<<<<<< HEAD
static bool icmphdr_ok(struct sk_buff *skb)
{
	return pskb_may_pull(skb, skb_transport_offset(skb) +
				  sizeof(struct icmphdr));
}

u64 ovs_flow_used_time(unsigned long flow_jiffies)
{
	struct timespec cur_ts;
	u64 cur_ms, idle_ms;

	ktime_get_ts(&cur_ts);
	idle_ms = jiffies_to_msecs(jiffies - flow_jiffies);
	cur_ms = (u64)cur_ts.tv_sec * MSEC_PER_SEC +
		 cur_ts.tv_nsec / NSEC_PER_MSEC;

	return cur_ms - idle_ms;
}

#define SW_FLOW_KEY_OFFSET(field)		\
	(offsetof(struct sw_flow_key, field) +	\
	 FIELD_SIZEOF(struct sw_flow_key, field))

static int parse_ipv6hdr(struct sk_buff *skb, struct sw_flow_key *key,
			 int *key_lenp)
=======
static bool sctphdr_ok(struct sk_buff *skb)
{
	return pskb_may_pull(skb, skb_transport_offset(skb) +
				  sizeof(struct sctphdr));
}

static bool icmphdr_ok(struct sk_buff *skb)
{
	return pskb_may_pull(skb, skb_transport_offset(skb) +
				  sizeof(struct icmphdr));
}

static int parse_ipv6hdr(struct sk_buff *skb, struct sw_flow_key *key)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned int nh_ofs = skb_network_offset(skb);
	unsigned int nh_len;
	int payload_ofs;
	struct ipv6hdr *nh;
	uint8_t nexthdr;
	__be16 frag_off;
	int err;

<<<<<<< HEAD
	*key_lenp = SW_FLOW_KEY_OFFSET(ipv6.label);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	err = check_header(skb, nh_ofs + sizeof(*nh));
	if (unlikely(err))
		return err;

	nh = ipv6_hdr(skb);
	nexthdr = nh->nexthdr;
	payload_ofs = (u8 *)(nh + 1) - skb->data;

	key->ip.proto = NEXTHDR_NONE;
	key->ip.tos = ipv6_get_dsfield(nh);
	key->ip.ttl = nh->hop_limit;
	key->ipv6.label = *(__be32 *)nh & htonl(IPV6_FLOWINFO_FLOWLABEL);
	key->ipv6.addr.src = nh->saddr;
	key->ipv6.addr.dst = nh->daddr;

	payload_ofs = ipv6_skip_exthdr(skb, payload_ofs, &nexthdr, &frag_off);
<<<<<<< HEAD
	if (unlikely(payload_ofs < 0))
		return -EINVAL;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (frag_off) {
		if (frag_off & htons(~0x7))
			key->ip.frag = OVS_FRAG_TYPE_LATER;
		else
			key->ip.frag = OVS_FRAG_TYPE_FIRST;
<<<<<<< HEAD
	}

=======
	} else {
		key->ip.frag = OVS_FRAG_TYPE_NONE;
	}

	/* Delayed handling of error in ipv6_skip_exthdr() as it
	 * always sets frag_off to a valid value which may be
	 * used to set key->ip.frag above.
	 */
	if (unlikely(payload_ofs < 0))
		return -EPROTO;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	nh_len = payload_ofs - nh_ofs;
	skb_set_transport_header(skb, nh_ofs + nh_len);
	key->ip.proto = nexthdr;
	return nh_len;
}

static bool icmp6hdr_ok(struct sk_buff *skb)
{
	return pskb_may_pull(skb, skb_transport_offset(skb) +
				  sizeof(struct icmp6hdr));
}

<<<<<<< HEAD
#define TCP_FLAGS_OFFSET 13
#define TCP_FLAG_MASK 0x3f

void ovs_flow_used(struct sw_flow *flow, struct sk_buff *skb)
{
	u8 tcp_flags = 0;

	if ((flow->key.eth.type == htons(ETH_P_IP) ||
	     flow->key.eth.type == htons(ETH_P_IPV6)) &&
	    flow->key.ip.proto == IPPROTO_TCP &&
	    likely(skb->len >= skb_transport_offset(skb) + sizeof(struct tcphdr))) {
		u8 *tcp = (u8 *)tcp_hdr(skb);
		tcp_flags = *(tcp + TCP_FLAGS_OFFSET) & TCP_FLAG_MASK;
	}

	spin_lock(&flow->lock);
	flow->used = jiffies;
	flow->packet_count++;
	flow->byte_count += skb->len;
	flow->tcp_flags |= tcp_flags;
	spin_unlock(&flow->lock);
}

struct sw_flow_actions *ovs_flow_actions_alloc(const struct nlattr *actions)
{
	int actions_len = nla_len(actions);
	struct sw_flow_actions *sfa;

	if (actions_len > MAX_ACTIONS_BUFSIZE)
		return ERR_PTR(-EINVAL);

	sfa = kmalloc(sizeof(*sfa) + actions_len, GFP_KERNEL);
	if (!sfa)
		return ERR_PTR(-ENOMEM);

	sfa->actions_len = actions_len;
	nla_memcpy(sfa->actions, actions, actions_len);
	return sfa;
}

struct sw_flow *ovs_flow_alloc(void)
{
	struct sw_flow *flow;

	flow = kmem_cache_alloc(flow_cache, GFP_KERNEL);
	if (!flow)
		return ERR_PTR(-ENOMEM);

	spin_lock_init(&flow->lock);
	flow->sf_acts = NULL;

	return flow;
}

static struct hlist_head *find_bucket(struct flow_table *table, u32 hash)
{
	hash = jhash_1word(hash, table->hash_seed);
	return flex_array_get(table->buckets,
				(hash & (table->n_buckets - 1)));
}

static struct flex_array *alloc_buckets(unsigned int n_buckets)
{
	struct flex_array *buckets;
	int i, err;

	buckets = flex_array_alloc(sizeof(struct hlist_head *),
				   n_buckets, GFP_KERNEL);
	if (!buckets)
		return NULL;

	err = flex_array_prealloc(buckets, 0, n_buckets, GFP_KERNEL);
	if (err) {
		flex_array_free(buckets);
		return NULL;
	}

	for (i = 0; i < n_buckets; i++)
		INIT_HLIST_HEAD((struct hlist_head *)
					flex_array_get(buckets, i));

	return buckets;
}

static void free_buckets(struct flex_array *buckets)
{
	flex_array_free(buckets);
}

struct flow_table *ovs_flow_tbl_alloc(int new_size)
{
	struct flow_table *table = kmalloc(sizeof(*table), GFP_KERNEL);

	if (!table)
		return NULL;

	table->buckets = alloc_buckets(new_size);

	if (!table->buckets) {
		kfree(table);
		return NULL;
	}
	table->n_buckets = new_size;
	table->count = 0;
	table->node_ver = 0;
	table->keep_flows = false;
	get_random_bytes(&table->hash_seed, sizeof(u32));

	return table;
}

void ovs_flow_tbl_destroy(struct flow_table *table)
{
	int i;

	if (!table)
		return;

	if (table->keep_flows)
		goto skip_flows;

	for (i = 0; i < table->n_buckets; i++) {
		struct sw_flow *flow;
		struct hlist_head *head = flex_array_get(table->buckets, i);
		struct hlist_node *n;
		int ver = table->node_ver;

		hlist_for_each_entry_safe(flow, n, head, hash_node[ver]) {
			hlist_del_rcu(&flow->hash_node[ver]);
			ovs_flow_free(flow);
		}
	}

skip_flows:
	free_buckets(table->buckets);
	kfree(table);
}

static void flow_tbl_destroy_rcu_cb(struct rcu_head *rcu)
{
	struct flow_table *table = container_of(rcu, struct flow_table, rcu);

	ovs_flow_tbl_destroy(table);
}

void ovs_flow_tbl_deferred_destroy(struct flow_table *table)
{
	if (!table)
		return;

	call_rcu(&table->rcu, flow_tbl_destroy_rcu_cb);
}

struct sw_flow *ovs_flow_tbl_next(struct flow_table *table, u32 *bucket, u32 *last)
{
	struct sw_flow *flow;
	struct hlist_head *head;
	int ver;
	int i;

	ver = table->node_ver;
	while (*bucket < table->n_buckets) {
		i = 0;
		head = flex_array_get(table->buckets, *bucket);
		hlist_for_each_entry_rcu(flow, head, hash_node[ver]) {
			if (i < *last) {
				i++;
				continue;
			}
			*last = i + 1;
			return flow;
		}
		(*bucket)++;
		*last = 0;
	}

	return NULL;
}

static void flow_table_copy_flows(struct flow_table *old, struct flow_table *new)
{
	int old_ver;
	int i;

	old_ver = old->node_ver;
	new->node_ver = !old_ver;

	/* Insert in new table. */
	for (i = 0; i < old->n_buckets; i++) {
		struct sw_flow *flow;
		struct hlist_head *head;

		head = flex_array_get(old->buckets, i);

		hlist_for_each_entry(flow, head, hash_node[old_ver])
			ovs_flow_tbl_insert(new, flow);
	}
	old->keep_flows = true;
}

static struct flow_table *__flow_tbl_rehash(struct flow_table *table, int n_buckets)
{
	struct flow_table *new_table;

	new_table = ovs_flow_tbl_alloc(n_buckets);
	if (!new_table)
		return ERR_PTR(-ENOMEM);

	flow_table_copy_flows(table, new_table);

	return new_table;
}

struct flow_table *ovs_flow_tbl_rehash(struct flow_table *table)
{
	return __flow_tbl_rehash(table, table->n_buckets);
}

struct flow_table *ovs_flow_tbl_expand(struct flow_table *table)
{
	return __flow_tbl_rehash(table, table->n_buckets * 2);
}

void ovs_flow_free(struct sw_flow *flow)
{
	if (unlikely(!flow))
		return;

	kfree((struct sf_flow_acts __force *)flow->sf_acts);
	kmem_cache_free(flow_cache, flow);
}

/* RCU callback used by ovs_flow_deferred_free. */
static void rcu_free_flow_callback(struct rcu_head *rcu)
{
	struct sw_flow *flow = container_of(rcu, struct sw_flow, rcu);

	ovs_flow_free(flow);
}

/* Schedules 'flow' to be freed after the next RCU grace period.
 * The caller must hold rcu_read_lock for this to be sensible. */
void ovs_flow_deferred_free(struct sw_flow *flow)
{
	call_rcu(&flow->rcu, rcu_free_flow_callback);
}

/* Schedules 'sf_acts' to be freed after the next RCU grace period.
 * The caller must hold rcu_read_lock for this to be sensible. */
void ovs_flow_deferred_free_acts(struct sw_flow_actions *sf_acts)
{
	kfree_rcu(sf_acts, rcu);
=======
/**
 * Parse vlan tag from vlan header.
 * Returns ERROR on memory error.
 * Returns 0 if it encounters a non-vlan or incomplete packet.
 * Returns 1 after successfully parsing vlan tag.
 */
static int parse_vlan_tag(struct sk_buff *skb, struct vlan_head *key_vh)
{
	struct vlan_head *vh = (struct vlan_head *)skb->data;

	if (likely(!eth_type_vlan(vh->tpid)))
		return 0;

	if (unlikely(skb->len < sizeof(struct vlan_head) + sizeof(__be16)))
		return 0;

	if (unlikely(!pskb_may_pull(skb, sizeof(struct vlan_head) +
				 sizeof(__be16))))
		return -ENOMEM;

	vh = (struct vlan_head *)skb->data;
	key_vh->tci = vh->tci | htons(VLAN_TAG_PRESENT);
	key_vh->tpid = vh->tpid;

	__skb_pull(skb, sizeof(struct vlan_head));
	return 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int parse_vlan(struct sk_buff *skb, struct sw_flow_key *key)
{
<<<<<<< HEAD
	struct qtag_prefix {
		__be16 eth_type; /* ETH_P_8021Q */
		__be16 tci;
	};
	struct qtag_prefix *qp;

	if (unlikely(skb->len < sizeof(struct qtag_prefix) + sizeof(__be16)))
		return 0;

	if (unlikely(!pskb_may_pull(skb, sizeof(struct qtag_prefix) +
					 sizeof(__be16))))
		return -ENOMEM;

	qp = (struct qtag_prefix *) skb->data;
	key->eth.tci = qp->tci | htons(VLAN_TAG_PRESENT);
	__skb_pull(skb, sizeof(struct qtag_prefix));
=======
	int res;

	key->eth.vlan.tci = 0;
	key->eth.vlan.tpid = 0;
	key->eth.cvlan.tci = 0;
	key->eth.cvlan.tpid = 0;

	if (skb_vlan_tag_present(skb)) {
		key->eth.vlan.tci = htons(skb->vlan_tci);
		key->eth.vlan.tpid = skb->vlan_proto;
	} else {
		/* Parse outer vlan tag in the non-accelerated case. */
		res = parse_vlan_tag(skb, &key->eth.vlan);
		if (res <= 0)
			return res;
	}

	/* Parse inner vlan tag. */
	res = parse_vlan_tag(skb, &key->eth.cvlan);
	if (res <= 0)
		return res;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static __be16 parse_ethertype(struct sk_buff *skb)
{
	struct llc_snap_hdr {
		u8  dsap;  /* Always 0xAA */
		u8  ssap;  /* Always 0xAA */
		u8  ctrl;
		u8  oui[3];
		__be16 ethertype;
	};
	struct llc_snap_hdr *llc;
	__be16 proto;

	proto = *(__be16 *) skb->data;
	__skb_pull(skb, sizeof(__be16));

<<<<<<< HEAD
	if (ntohs(proto) >= ETH_P_802_3_MIN)
=======
	if (eth_proto_is_802_3(proto))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return proto;

	if (skb->len < sizeof(struct llc_snap_hdr))
		return htons(ETH_P_802_2);

	if (unlikely(!pskb_may_pull(skb, sizeof(struct llc_snap_hdr))))
		return htons(0);

	llc = (struct llc_snap_hdr *) skb->data;
	if (llc->dsap != LLC_SAP_SNAP ||
	    llc->ssap != LLC_SAP_SNAP ||
	    (llc->oui[0] | llc->oui[1] | llc->oui[2]) != 0)
		return htons(ETH_P_802_2);

	__skb_pull(skb, sizeof(struct llc_snap_hdr));

<<<<<<< HEAD
	if (ntohs(llc->ethertype) >= ETH_P_802_3_MIN)
=======
	if (eth_proto_is_802_3(llc->ethertype))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return llc->ethertype;

	return htons(ETH_P_802_2);
}

static int parse_icmpv6(struct sk_buff *skb, struct sw_flow_key *key,
<<<<<<< HEAD
			int *key_lenp, int nh_len)
{
	struct icmp6hdr *icmp = icmp6_hdr(skb);
	int error = 0;
	int key_len;
=======
			int nh_len)
{
	struct icmp6hdr *icmp = icmp6_hdr(skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* The ICMPv6 type and code fields use the 16-bit transport port
	 * fields, so we need to store them in 16-bit network byte order.
	 */
<<<<<<< HEAD
	key->ipv6.tp.src = htons(icmp->icmp6_type);
	key->ipv6.tp.dst = htons(icmp->icmp6_code);
	key_len = SW_FLOW_KEY_OFFSET(ipv6.tp);
=======
	key->tp.src = htons(icmp->icmp6_type);
	key->tp.dst = htons(icmp->icmp6_code);
	memset(&key->ipv6.nd, 0, sizeof(key->ipv6.nd));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (icmp->icmp6_code == 0 &&
	    (icmp->icmp6_type == NDISC_NEIGHBOUR_SOLICITATION ||
	     icmp->icmp6_type == NDISC_NEIGHBOUR_ADVERTISEMENT)) {
		int icmp_len = skb->len - skb_transport_offset(skb);
		struct nd_msg *nd;
		int offset;

<<<<<<< HEAD
		key_len = SW_FLOW_KEY_OFFSET(ipv6.nd);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* In order to process neighbor discovery options, we need the
		 * entire packet.
		 */
		if (unlikely(icmp_len < sizeof(*nd)))
<<<<<<< HEAD
			goto out;
		if (unlikely(skb_linearize(skb))) {
			error = -ENOMEM;
			goto out;
		}

		nd = (struct nd_msg *)skb_transport_header(skb);
		key->ipv6.nd.target = nd->target;
		key_len = SW_FLOW_KEY_OFFSET(ipv6.nd);
=======
			return 0;

		if (unlikely(skb_linearize(skb)))
			return -ENOMEM;

		nd = (struct nd_msg *)skb_transport_header(skb);
		key->ipv6.nd.target = nd->target;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		icmp_len -= sizeof(*nd);
		offset = 0;
		while (icmp_len >= 8) {
			struct nd_opt_hdr *nd_opt =
				 (struct nd_opt_hdr *)(nd->opt + offset);
			int opt_len = nd_opt->nd_opt_len * 8;

			if (unlikely(!opt_len || opt_len > icmp_len))
<<<<<<< HEAD
				goto invalid;
=======
				return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			/* Store the link layer address if the appropriate
			 * option is provided.  It is considered an error if
			 * the same link layer option is specified twice.
			 */
			if (nd_opt->nd_opt_type == ND_OPT_SOURCE_LL_ADDR
			    && opt_len == 8) {
				if (unlikely(!is_zero_ether_addr(key->ipv6.nd.sll)))
					goto invalid;
<<<<<<< HEAD
				memcpy(key->ipv6.nd.sll,
				    &nd->opt[offset+sizeof(*nd_opt)], ETH_ALEN);
=======
				ether_addr_copy(key->ipv6.nd.sll,
						&nd->opt[offset+sizeof(*nd_opt)]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			} else if (nd_opt->nd_opt_type == ND_OPT_TARGET_LL_ADDR
				   && opt_len == 8) {
				if (unlikely(!is_zero_ether_addr(key->ipv6.nd.tll)))
					goto invalid;
<<<<<<< HEAD
				memcpy(key->ipv6.nd.tll,
				    &nd->opt[offset+sizeof(*nd_opt)], ETH_ALEN);
=======
				ether_addr_copy(key->ipv6.nd.tll,
						&nd->opt[offset+sizeof(*nd_opt)]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			}

			icmp_len -= opt_len;
			offset += opt_len;
		}
	}

<<<<<<< HEAD
	goto out;
=======
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

invalid:
	memset(&key->ipv6.nd.target, 0, sizeof(key->ipv6.nd.target));
	memset(key->ipv6.nd.sll, 0, sizeof(key->ipv6.nd.sll));
	memset(key->ipv6.nd.tll, 0, sizeof(key->ipv6.nd.tll));

<<<<<<< HEAD
out:
	*key_lenp = key_len;
	return error;
}

/**
 * ovs_flow_extract - extracts a flow key from an Ethernet frame.
 * @skb: sk_buff that contains the frame, with skb->data pointing to the
 * Ethernet header
 * @in_port: port number on which @skb was received.
 * @key: output flow key
 * @key_lenp: length of output flow key
=======
	return 0;
}

/**
 * key_extract - extracts a flow key from an Ethernet frame.
 * @skb: sk_buff that contains the frame, with skb->data pointing to the
 * Ethernet header
 * @key: output flow key
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * The caller must ensure that skb->len >= ETH_HLEN.
 *
 * Returns 0 if successful, otherwise a negative errno value.
 *
 * Initializes @skb header pointers as follows:
 *
 *    - skb->mac_header: the Ethernet header.
 *
 *    - skb->network_header: just past the Ethernet header, or just past the
 *      VLAN header, to the first byte of the Ethernet payload.
 *
<<<<<<< HEAD
 *    - skb->transport_header: If key->dl_type is ETH_P_IP or ETH_P_IPV6
 *      on output, then just past the IP header, if one is present and
 *      of a correct length, otherwise the same as skb->network_header.
 *      For other key->dl_type values it is left untouched.
 */
int ovs_flow_extract(struct sk_buff *skb, u16 in_port, struct sw_flow_key *key,
		 int *key_lenp)
{
	int error = 0;
	int key_len = SW_FLOW_KEY_OFFSET(eth);
	struct ethhdr *eth;

	memset(key, 0, sizeof(*key));

	key->phy.priority = skb->priority;
	key->phy.in_port = in_port;
	key->phy.skb_mark = skb->mark;
=======
 *    - skb->transport_header: If key->eth.type is ETH_P_IP or ETH_P_IPV6
 *      on output, then just past the IP header, if one is present and
 *      of a correct length, otherwise the same as skb->network_header.
 *      For other key->eth.type values it is left untouched.
 */
static int key_extract(struct sk_buff *skb, struct sw_flow_key *key)
{
	int error;
	struct ethhdr *eth;

	/* Flags are always used as part of stats */
	key->tp.flags = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	skb_reset_mac_header(skb);

	/* Link layer.  We are guaranteed to have at least the 14 byte Ethernet
	 * header in the linear data area.
	 */
	eth = eth_hdr(skb);
<<<<<<< HEAD
	memcpy(key->eth.src, eth->h_source, ETH_ALEN);
	memcpy(key->eth.dst, eth->h_dest, ETH_ALEN);

	__skb_pull(skb, 2 * ETH_ALEN);

	if (vlan_tx_tag_present(skb))
		key->eth.tci = htons(skb->vlan_tci);
	else if (eth->h_proto == htons(ETH_P_8021Q))
		if (unlikely(parse_vlan(skb, key)))
			return -ENOMEM;
=======
	ether_addr_copy(key->eth.src, eth->h_source);
	ether_addr_copy(key->eth.dst, eth->h_dest);

	__skb_pull(skb, 2 * ETH_ALEN);
	/* We are going to push all headers that we pull, so no need to
	 * update skb->csum here.
	 */

	if (unlikely(parse_vlan(skb, key)))
		return -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	key->eth.type = parse_ethertype(skb);
	if (unlikely(key->eth.type == htons(0)))
		return -ENOMEM;

	skb_reset_network_header(skb);
<<<<<<< HEAD
=======
	skb_reset_mac_len(skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__skb_push(skb, skb->data - skb_mac_header(skb));

	/* Network layer. */
	if (key->eth.type == htons(ETH_P_IP)) {
		struct iphdr *nh;
		__be16 offset;

<<<<<<< HEAD
		key_len = SW_FLOW_KEY_OFFSET(ipv4.addr);

		error = check_iphdr(skb);
		if (unlikely(error)) {
=======
		error = check_iphdr(skb);
		if (unlikely(error)) {
			memset(&key->ip, 0, sizeof(key->ip));
			memset(&key->ipv4, 0, sizeof(key->ipv4));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (error == -EINVAL) {
				skb->transport_header = skb->network_header;
				error = 0;
			}
<<<<<<< HEAD
			goto out;
=======
			return error;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		nh = ip_hdr(skb);
		key->ipv4.addr.src = nh->saddr;
		key->ipv4.addr.dst = nh->daddr;

		key->ip.proto = nh->protocol;
		key->ip.tos = nh->tos;
		key->ip.ttl = nh->ttl;

		offset = nh->frag_off & htons(IP_OFFSET);
		if (offset) {
			key->ip.frag = OVS_FRAG_TYPE_LATER;
<<<<<<< HEAD
			goto out;
		}
		if (nh->frag_off & htons(IP_MF) ||
			 skb_shinfo(skb)->gso_type & SKB_GSO_UDP)
			key->ip.frag = OVS_FRAG_TYPE_FIRST;

		/* Transport layer. */
		if (key->ip.proto == IPPROTO_TCP) {
			key_len = SW_FLOW_KEY_OFFSET(ipv4.tp);
			if (tcphdr_ok(skb)) {
				struct tcphdr *tcp = tcp_hdr(skb);
				key->ipv4.tp.src = tcp->source;
				key->ipv4.tp.dst = tcp->dest;
			}
		} else if (key->ip.proto == IPPROTO_UDP) {
			key_len = SW_FLOW_KEY_OFFSET(ipv4.tp);
			if (udphdr_ok(skb)) {
				struct udphdr *udp = udp_hdr(skb);
				key->ipv4.tp.src = udp->source;
				key->ipv4.tp.dst = udp->dest;
			}
		} else if (key->ip.proto == IPPROTO_ICMP) {
			key_len = SW_FLOW_KEY_OFFSET(ipv4.tp);
=======
			return 0;
		}
		if (nh->frag_off & htons(IP_MF) ||
			skb_shinfo(skb)->gso_type & SKB_GSO_UDP)
			key->ip.frag = OVS_FRAG_TYPE_FIRST;
		else
			key->ip.frag = OVS_FRAG_TYPE_NONE;

		/* Transport layer. */
		if (key->ip.proto == IPPROTO_TCP) {
			if (tcphdr_ok(skb)) {
				struct tcphdr *tcp = tcp_hdr(skb);
				key->tp.src = tcp->source;
				key->tp.dst = tcp->dest;
				key->tp.flags = TCP_FLAGS_BE16(tcp);
			} else {
				memset(&key->tp, 0, sizeof(key->tp));
			}

		} else if (key->ip.proto == IPPROTO_UDP) {
			if (udphdr_ok(skb)) {
				struct udphdr *udp = udp_hdr(skb);
				key->tp.src = udp->source;
				key->tp.dst = udp->dest;
			} else {
				memset(&key->tp, 0, sizeof(key->tp));
			}
		} else if (key->ip.proto == IPPROTO_SCTP) {
			if (sctphdr_ok(skb)) {
				struct sctphdr *sctp = sctp_hdr(skb);
				key->tp.src = sctp->source;
				key->tp.dst = sctp->dest;
			} else {
				memset(&key->tp, 0, sizeof(key->tp));
			}
		} else if (key->ip.proto == IPPROTO_ICMP) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (icmphdr_ok(skb)) {
				struct icmphdr *icmp = icmp_hdr(skb);
				/* The ICMP type and code fields use the 16-bit
				 * transport port fields, so we need to store
				 * them in 16-bit network byte order. */
<<<<<<< HEAD
				key->ipv4.tp.src = htons(icmp->type);
				key->ipv4.tp.dst = htons(icmp->code);
			}
		}

	} else if ((key->eth.type == htons(ETH_P_ARP) ||
		   key->eth.type == htons(ETH_P_RARP)) && arphdr_ok(skb)) {
		struct arp_eth_header *arp;

		arp = (struct arp_eth_header *)skb_network_header(skb);

		if (arp->ar_hrd == htons(ARPHRD_ETHER)
				&& arp->ar_pro == htons(ETH_P_IP)
				&& arp->ar_hln == ETH_ALEN
				&& arp->ar_pln == 4) {
=======
				key->tp.src = htons(icmp->type);
				key->tp.dst = htons(icmp->code);
			} else {
				memset(&key->tp, 0, sizeof(key->tp));
			}
		}

	} else if (key->eth.type == htons(ETH_P_ARP) ||
		   key->eth.type == htons(ETH_P_RARP)) {
		struct arp_eth_header *arp;
		bool arp_available = arphdr_ok(skb);

		arp = (struct arp_eth_header *)skb_network_header(skb);

		if (arp_available &&
		    arp->ar_hrd == htons(ARPHRD_ETHER) &&
		    arp->ar_pro == htons(ETH_P_IP) &&
		    arp->ar_hln == ETH_ALEN &&
		    arp->ar_pln == 4) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			/* We only match on the lower 8 bits of the opcode. */
			if (ntohs(arp->ar_op) <= 0xff)
				key->ip.proto = ntohs(arp->ar_op);
<<<<<<< HEAD
			memcpy(&key->ipv4.addr.src, arp->ar_sip, sizeof(key->ipv4.addr.src));
			memcpy(&key->ipv4.addr.dst, arp->ar_tip, sizeof(key->ipv4.addr.dst));
			memcpy(key->ipv4.arp.sha, arp->ar_sha, ETH_ALEN);
			memcpy(key->ipv4.arp.tha, arp->ar_tha, ETH_ALEN);
			key_len = SW_FLOW_KEY_OFFSET(ipv4.arp);
=======
			else
				key->ip.proto = 0;

			memcpy(&key->ipv4.addr.src, arp->ar_sip, sizeof(key->ipv4.addr.src));
			memcpy(&key->ipv4.addr.dst, arp->ar_tip, sizeof(key->ipv4.addr.dst));
			ether_addr_copy(key->ipv4.arp.sha, arp->ar_sha);
			ether_addr_copy(key->ipv4.arp.tha, arp->ar_tha);
		} else {
			memset(&key->ip, 0, sizeof(key->ip));
			memset(&key->ipv4, 0, sizeof(key->ipv4));
		}
	} else if (eth_p_mpls(key->eth.type)) {
		size_t stack_len = MPLS_HLEN;

		skb_set_inner_network_header(skb, skb->mac_len);
		while (1) {
			__be32 lse;

			error = check_header(skb, skb->mac_len + stack_len);
			if (unlikely(error))
				return 0;

			memcpy(&lse, skb_inner_network_header(skb), MPLS_HLEN);

			if (stack_len == MPLS_HLEN)
				memcpy(&key->mpls.top_lse, &lse, MPLS_HLEN);

			skb_set_inner_network_header(skb, skb->mac_len + stack_len);
			if (lse & htonl(MPLS_LS_S_MASK))
				break;

			stack_len += MPLS_HLEN;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	} else if (key->eth.type == htons(ETH_P_IPV6)) {
		int nh_len;             /* IPv6 Header + Extensions */

<<<<<<< HEAD
		nh_len = parse_ipv6hdr(skb, key, &key_len);
		if (unlikely(nh_len < 0)) {
			if (nh_len == -EINVAL)
				skb->transport_header = skb->network_header;
			else
				error = nh_len;
			goto out;
		}

		if (key->ip.frag == OVS_FRAG_TYPE_LATER)
			goto out;
=======
		nh_len = parse_ipv6hdr(skb, key);
		if (unlikely(nh_len < 0)) {
			switch (nh_len) {
			case -EINVAL:
				memset(&key->ip, 0, sizeof(key->ip));
				memset(&key->ipv6.addr, 0, sizeof(key->ipv6.addr));
				/* fall-through */
			case -EPROTO:
				skb->transport_header = skb->network_header;
				error = 0;
				break;
			default:
				error = nh_len;
			}
			return error;
		}

		if (key->ip.frag == OVS_FRAG_TYPE_LATER)
			return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (skb_shinfo(skb)->gso_type & SKB_GSO_UDP)
			key->ip.frag = OVS_FRAG_TYPE_FIRST;

		/* Transport layer. */
		if (key->ip.proto == NEXTHDR_TCP) {
<<<<<<< HEAD
			key_len = SW_FLOW_KEY_OFFSET(ipv6.tp);
			if (tcphdr_ok(skb)) {
				struct tcphdr *tcp = tcp_hdr(skb);
				key->ipv6.tp.src = tcp->source;
				key->ipv6.tp.dst = tcp->dest;
			}
		} else if (key->ip.proto == NEXTHDR_UDP) {
			key_len = SW_FLOW_KEY_OFFSET(ipv6.tp);
			if (udphdr_ok(skb)) {
				struct udphdr *udp = udp_hdr(skb);
				key->ipv6.tp.src = udp->source;
				key->ipv6.tp.dst = udp->dest;
			}
		} else if (key->ip.proto == NEXTHDR_ICMP) {
			key_len = SW_FLOW_KEY_OFFSET(ipv6.tp);
			if (icmp6hdr_ok(skb)) {
				error = parse_icmpv6(skb, key, &key_len, nh_len);
				if (error < 0)
					goto out;
			}
		}
	}

out:
	*key_lenp = key_len;
	return error;
}

u32 ovs_flow_hash(const struct sw_flow_key *key, int key_len)
{
	return jhash2((u32 *)key, DIV_ROUND_UP(key_len, sizeof(u32)), 0);
}

struct sw_flow *ovs_flow_tbl_lookup(struct flow_table *table,
				struct sw_flow_key *key, int key_len)
{
	struct sw_flow *flow;
	struct hlist_head *head;
	u32 hash;

	hash = ovs_flow_hash(key, key_len);

	head = find_bucket(table, hash);
	hlist_for_each_entry_rcu(flow, head, hash_node[table->node_ver]) {

		if (flow->hash == hash &&
		    !memcmp(&flow->key, key, key_len)) {
			return flow;
		}
	}
	return NULL;
}

void ovs_flow_tbl_insert(struct flow_table *table, struct sw_flow *flow)
{
	struct hlist_head *head;

	head = find_bucket(table, flow->hash);
	hlist_add_head_rcu(&flow->hash_node[table->node_ver], head);
	table->count++;
}

void ovs_flow_tbl_remove(struct flow_table *table, struct sw_flow *flow)
{
	BUG_ON(table->count == 0);
	hlist_del_rcu(&flow->hash_node[table->node_ver]);
	table->count--;
}

/* The size of the argument for each %OVS_KEY_ATTR_* Netlink attribute.  */
const int ovs_key_lens[OVS_KEY_ATTR_MAX + 1] = {
	[OVS_KEY_ATTR_ENCAP] = -1,
	[OVS_KEY_ATTR_PRIORITY] = sizeof(u32),
	[OVS_KEY_ATTR_IN_PORT] = sizeof(u32),
	[OVS_KEY_ATTR_SKB_MARK] = sizeof(u32),
	[OVS_KEY_ATTR_ETHERNET] = sizeof(struct ovs_key_ethernet),
	[OVS_KEY_ATTR_VLAN] = sizeof(__be16),
	[OVS_KEY_ATTR_ETHERTYPE] = sizeof(__be16),
	[OVS_KEY_ATTR_IPV4] = sizeof(struct ovs_key_ipv4),
	[OVS_KEY_ATTR_IPV6] = sizeof(struct ovs_key_ipv6),
	[OVS_KEY_ATTR_TCP] = sizeof(struct ovs_key_tcp),
	[OVS_KEY_ATTR_UDP] = sizeof(struct ovs_key_udp),
	[OVS_KEY_ATTR_ICMP] = sizeof(struct ovs_key_icmp),
	[OVS_KEY_ATTR_ICMPV6] = sizeof(struct ovs_key_icmpv6),
	[OVS_KEY_ATTR_ARP] = sizeof(struct ovs_key_arp),
	[OVS_KEY_ATTR_ND] = sizeof(struct ovs_key_nd),
};

static int ipv4_flow_from_nlattrs(struct sw_flow_key *swkey, int *key_len,
				  const struct nlattr *a[], u32 *attrs)
{
	const struct ovs_key_icmp *icmp_key;
	const struct ovs_key_tcp *tcp_key;
	const struct ovs_key_udp *udp_key;

	switch (swkey->ip.proto) {
	case IPPROTO_TCP:
		if (!(*attrs & (1 << OVS_KEY_ATTR_TCP)))
			return -EINVAL;
		*attrs &= ~(1 << OVS_KEY_ATTR_TCP);

		*key_len = SW_FLOW_KEY_OFFSET(ipv4.tp);
		tcp_key = nla_data(a[OVS_KEY_ATTR_TCP]);
		swkey->ipv4.tp.src = tcp_key->tcp_src;
		swkey->ipv4.tp.dst = tcp_key->tcp_dst;
		break;

	case IPPROTO_UDP:
		if (!(*attrs & (1 << OVS_KEY_ATTR_UDP)))
			return -EINVAL;
		*attrs &= ~(1 << OVS_KEY_ATTR_UDP);

		*key_len = SW_FLOW_KEY_OFFSET(ipv4.tp);
		udp_key = nla_data(a[OVS_KEY_ATTR_UDP]);
		swkey->ipv4.tp.src = udp_key->udp_src;
		swkey->ipv4.tp.dst = udp_key->udp_dst;
		break;

	case IPPROTO_ICMP:
		if (!(*attrs & (1 << OVS_KEY_ATTR_ICMP)))
			return -EINVAL;
		*attrs &= ~(1 << OVS_KEY_ATTR_ICMP);

		*key_len = SW_FLOW_KEY_OFFSET(ipv4.tp);
		icmp_key = nla_data(a[OVS_KEY_ATTR_ICMP]);
		swkey->ipv4.tp.src = htons(icmp_key->icmp_type);
		swkey->ipv4.tp.dst = htons(icmp_key->icmp_code);
		break;
	}

	return 0;
}

static int ipv6_flow_from_nlattrs(struct sw_flow_key *swkey, int *key_len,
				  const struct nlattr *a[], u32 *attrs)
{
	const struct ovs_key_icmpv6 *icmpv6_key;
	const struct ovs_key_tcp *tcp_key;
	const struct ovs_key_udp *udp_key;

	switch (swkey->ip.proto) {
	case IPPROTO_TCP:
		if (!(*attrs & (1 << OVS_KEY_ATTR_TCP)))
			return -EINVAL;
		*attrs &= ~(1 << OVS_KEY_ATTR_TCP);

		*key_len = SW_FLOW_KEY_OFFSET(ipv6.tp);
		tcp_key = nla_data(a[OVS_KEY_ATTR_TCP]);
		swkey->ipv6.tp.src = tcp_key->tcp_src;
		swkey->ipv6.tp.dst = tcp_key->tcp_dst;
		break;

	case IPPROTO_UDP:
		if (!(*attrs & (1 << OVS_KEY_ATTR_UDP)))
			return -EINVAL;
		*attrs &= ~(1 << OVS_KEY_ATTR_UDP);

		*key_len = SW_FLOW_KEY_OFFSET(ipv6.tp);
		udp_key = nla_data(a[OVS_KEY_ATTR_UDP]);
		swkey->ipv6.tp.src = udp_key->udp_src;
		swkey->ipv6.tp.dst = udp_key->udp_dst;
		break;

	case IPPROTO_ICMPV6:
		if (!(*attrs & (1 << OVS_KEY_ATTR_ICMPV6)))
			return -EINVAL;
		*attrs &= ~(1 << OVS_KEY_ATTR_ICMPV6);

		*key_len = SW_FLOW_KEY_OFFSET(ipv6.tp);
		icmpv6_key = nla_data(a[OVS_KEY_ATTR_ICMPV6]);
		swkey->ipv6.tp.src = htons(icmpv6_key->icmpv6_type);
		swkey->ipv6.tp.dst = htons(icmpv6_key->icmpv6_code);

		if (swkey->ipv6.tp.src == htons(NDISC_NEIGHBOUR_SOLICITATION) ||
		    swkey->ipv6.tp.src == htons(NDISC_NEIGHBOUR_ADVERTISEMENT)) {
			const struct ovs_key_nd *nd_key;

			if (!(*attrs & (1 << OVS_KEY_ATTR_ND)))
				return -EINVAL;
			*attrs &= ~(1 << OVS_KEY_ATTR_ND);

			*key_len = SW_FLOW_KEY_OFFSET(ipv6.nd);
			nd_key = nla_data(a[OVS_KEY_ATTR_ND]);
			memcpy(&swkey->ipv6.nd.target, nd_key->nd_target,
			       sizeof(swkey->ipv6.nd.target));
			memcpy(swkey->ipv6.nd.sll, nd_key->nd_sll, ETH_ALEN);
			memcpy(swkey->ipv6.nd.tll, nd_key->nd_tll, ETH_ALEN);
		}
		break;
	}

	return 0;
}

static int parse_flow_nlattrs(const struct nlattr *attr,
			      const struct nlattr *a[], u32 *attrsp)
{
	const struct nlattr *nla;
	u32 attrs;
	int rem;

	attrs = 0;
	nla_for_each_nested(nla, attr, rem) {
		u16 type = nla_type(nla);
		int expected_len;

		if (type > OVS_KEY_ATTR_MAX || attrs & (1 << type))
			return -EINVAL;

		expected_len = ovs_key_lens[type];
		if (nla_len(nla) != expected_len && expected_len != -1)
			return -EINVAL;

		attrs |= 1 << type;
		a[type] = nla;
	}
	if (rem)
		return -EINVAL;

	*attrsp = attrs;
	return 0;
}

/**
 * ovs_flow_from_nlattrs - parses Netlink attributes into a flow key.
 * @swkey: receives the extracted flow key.
 * @key_lenp: number of bytes used in @swkey.
 * @attr: Netlink attribute holding nested %OVS_KEY_ATTR_* Netlink attribute
 * sequence.
 */
int ovs_flow_from_nlattrs(struct sw_flow_key *swkey, int *key_lenp,
		      const struct nlattr *attr)
{
	const struct nlattr *a[OVS_KEY_ATTR_MAX + 1];
	const struct ovs_key_ethernet *eth_key;
	int key_len;
	u32 attrs;
	int err;

	memset(swkey, 0, sizeof(struct sw_flow_key));
	key_len = SW_FLOW_KEY_OFFSET(eth);

	err = parse_flow_nlattrs(attr, a, &attrs);
	if (err)
		return err;

	/* Metadata attributes. */
	if (attrs & (1 << OVS_KEY_ATTR_PRIORITY)) {
		swkey->phy.priority = nla_get_u32(a[OVS_KEY_ATTR_PRIORITY]);
		attrs &= ~(1 << OVS_KEY_ATTR_PRIORITY);
	}
	if (attrs & (1 << OVS_KEY_ATTR_IN_PORT)) {
		u32 in_port = nla_get_u32(a[OVS_KEY_ATTR_IN_PORT]);
		if (in_port >= DP_MAX_PORTS)
			return -EINVAL;
		swkey->phy.in_port = in_port;
		attrs &= ~(1 << OVS_KEY_ATTR_IN_PORT);
	} else {
		swkey->phy.in_port = DP_MAX_PORTS;
	}
	if (attrs & (1 << OVS_KEY_ATTR_SKB_MARK)) {
		swkey->phy.skb_mark = nla_get_u32(a[OVS_KEY_ATTR_SKB_MARK]);
		attrs &= ~(1 << OVS_KEY_ATTR_SKB_MARK);
	}

	/* Data attributes. */
	if (!(attrs & (1 << OVS_KEY_ATTR_ETHERNET)))
		return -EINVAL;
	attrs &= ~(1 << OVS_KEY_ATTR_ETHERNET);

	eth_key = nla_data(a[OVS_KEY_ATTR_ETHERNET]);
	memcpy(swkey->eth.src, eth_key->eth_src, ETH_ALEN);
	memcpy(swkey->eth.dst, eth_key->eth_dst, ETH_ALEN);

	if (attrs & (1u << OVS_KEY_ATTR_ETHERTYPE) &&
	    nla_get_be16(a[OVS_KEY_ATTR_ETHERTYPE]) == htons(ETH_P_8021Q)) {
		const struct nlattr *encap;
		__be16 tci;

		if (attrs != ((1 << OVS_KEY_ATTR_VLAN) |
			      (1 << OVS_KEY_ATTR_ETHERTYPE) |
			      (1 << OVS_KEY_ATTR_ENCAP)))
			return -EINVAL;

		encap = a[OVS_KEY_ATTR_ENCAP];
		tci = nla_get_be16(a[OVS_KEY_ATTR_VLAN]);
		if (tci & htons(VLAN_TAG_PRESENT)) {
			swkey->eth.tci = tci;

			err = parse_flow_nlattrs(encap, a, &attrs);
			if (err)
				return err;
		} else if (!tci) {
			/* Corner case for truncated 802.1Q header. */
			if (nla_len(encap))
				return -EINVAL;

			swkey->eth.type = htons(ETH_P_8021Q);
			*key_lenp = key_len;
			return 0;
		} else {
			return -EINVAL;
		}
	}

	if (attrs & (1 << OVS_KEY_ATTR_ETHERTYPE)) {
		swkey->eth.type = nla_get_be16(a[OVS_KEY_ATTR_ETHERTYPE]);
		if (ntohs(swkey->eth.type) < ETH_P_802_3_MIN)
			return -EINVAL;
		attrs &= ~(1 << OVS_KEY_ATTR_ETHERTYPE);
	} else {
		swkey->eth.type = htons(ETH_P_802_2);
	}

	if (swkey->eth.type == htons(ETH_P_IP)) {
		const struct ovs_key_ipv4 *ipv4_key;

		if (!(attrs & (1 << OVS_KEY_ATTR_IPV4)))
			return -EINVAL;
		attrs &= ~(1 << OVS_KEY_ATTR_IPV4);

		key_len = SW_FLOW_KEY_OFFSET(ipv4.addr);
		ipv4_key = nla_data(a[OVS_KEY_ATTR_IPV4]);
		if (ipv4_key->ipv4_frag > OVS_FRAG_TYPE_MAX)
			return -EINVAL;
		swkey->ip.proto = ipv4_key->ipv4_proto;
		swkey->ip.tos = ipv4_key->ipv4_tos;
		swkey->ip.ttl = ipv4_key->ipv4_ttl;
		swkey->ip.frag = ipv4_key->ipv4_frag;
		swkey->ipv4.addr.src = ipv4_key->ipv4_src;
		swkey->ipv4.addr.dst = ipv4_key->ipv4_dst;

		if (swkey->ip.frag != OVS_FRAG_TYPE_LATER) {
			err = ipv4_flow_from_nlattrs(swkey, &key_len, a, &attrs);
			if (err)
				return err;
		}
	} else if (swkey->eth.type == htons(ETH_P_IPV6)) {
		const struct ovs_key_ipv6 *ipv6_key;

		if (!(attrs & (1 << OVS_KEY_ATTR_IPV6)))
			return -EINVAL;
		attrs &= ~(1 << OVS_KEY_ATTR_IPV6);

		key_len = SW_FLOW_KEY_OFFSET(ipv6.label);
		ipv6_key = nla_data(a[OVS_KEY_ATTR_IPV6]);
		if (ipv6_key->ipv6_frag > OVS_FRAG_TYPE_MAX)
			return -EINVAL;
		swkey->ipv6.label = ipv6_key->ipv6_label;
		swkey->ip.proto = ipv6_key->ipv6_proto;
		swkey->ip.tos = ipv6_key->ipv6_tclass;
		swkey->ip.ttl = ipv6_key->ipv6_hlimit;
		swkey->ip.frag = ipv6_key->ipv6_frag;
		memcpy(&swkey->ipv6.addr.src, ipv6_key->ipv6_src,
		       sizeof(swkey->ipv6.addr.src));
		memcpy(&swkey->ipv6.addr.dst, ipv6_key->ipv6_dst,
		       sizeof(swkey->ipv6.addr.dst));

		if (swkey->ip.frag != OVS_FRAG_TYPE_LATER) {
			err = ipv6_flow_from_nlattrs(swkey, &key_len, a, &attrs);
			if (err)
				return err;
		}
	} else if (swkey->eth.type == htons(ETH_P_ARP) ||
		   swkey->eth.type == htons(ETH_P_RARP)) {
		const struct ovs_key_arp *arp_key;

		if (!(attrs & (1 << OVS_KEY_ATTR_ARP)))
			return -EINVAL;
		attrs &= ~(1 << OVS_KEY_ATTR_ARP);

		key_len = SW_FLOW_KEY_OFFSET(ipv4.arp);
		arp_key = nla_data(a[OVS_KEY_ATTR_ARP]);
		swkey->ipv4.addr.src = arp_key->arp_sip;
		swkey->ipv4.addr.dst = arp_key->arp_tip;
		if (arp_key->arp_op & htons(0xff00))
			return -EINVAL;
		swkey->ip.proto = ntohs(arp_key->arp_op);
		memcpy(swkey->ipv4.arp.sha, arp_key->arp_sha, ETH_ALEN);
		memcpy(swkey->ipv4.arp.tha, arp_key->arp_tha, ETH_ALEN);
	}

	if (attrs)
		return -EINVAL;
	*key_lenp = key_len;

	return 0;
}

/**
 * ovs_flow_metadata_from_nlattrs - parses Netlink attributes into a flow key.
 * @priority: receives the skb priority
 * @mark: receives the skb mark
 * @in_port: receives the extracted input port.
 * @key: Netlink attribute holding nested %OVS_KEY_ATTR_* Netlink attribute
 * sequence.
 *
 * This parses a series of Netlink attributes that form a flow key, which must
 * take the same form accepted by flow_from_nlattrs(), but only enough of it to
 * get the metadata, that is, the parts of the flow key that cannot be
 * extracted from the packet itself.
 */
int ovs_flow_metadata_from_nlattrs(u32 *priority, u32 *mark, u16 *in_port,
			       const struct nlattr *attr)
{
	const struct nlattr *nla;
	int rem;

	*in_port = DP_MAX_PORTS;
	*priority = 0;
	*mark = 0;

	nla_for_each_nested(nla, attr, rem) {
		int type = nla_type(nla);

		if (type <= OVS_KEY_ATTR_MAX && ovs_key_lens[type] > 0) {
			if (nla_len(nla) != ovs_key_lens[type])
				return -EINVAL;

			switch (type) {
			case OVS_KEY_ATTR_PRIORITY:
				*priority = nla_get_u32(nla);
				break;

			case OVS_KEY_ATTR_IN_PORT:
				if (nla_get_u32(nla) >= DP_MAX_PORTS)
					return -EINVAL;
				*in_port = nla_get_u32(nla);
				break;

			case OVS_KEY_ATTR_SKB_MARK:
				*mark = nla_get_u32(nla);
				break;
			}
		}
	}
	if (rem)
		return -EINVAL;
	return 0;
}

int ovs_flow_to_nlattrs(const struct sw_flow_key *swkey, struct sk_buff *skb)
{
	struct ovs_key_ethernet *eth_key;
	struct nlattr *nla, *encap;

	if (swkey->phy.priority &&
	    nla_put_u32(skb, OVS_KEY_ATTR_PRIORITY, swkey->phy.priority))
		goto nla_put_failure;

	if (swkey->phy.in_port != DP_MAX_PORTS &&
	    nla_put_u32(skb, OVS_KEY_ATTR_IN_PORT, swkey->phy.in_port))
		goto nla_put_failure;

	if (swkey->phy.skb_mark &&
	    nla_put_u32(skb, OVS_KEY_ATTR_SKB_MARK, swkey->phy.skb_mark))
		goto nla_put_failure;

	nla = nla_reserve(skb, OVS_KEY_ATTR_ETHERNET, sizeof(*eth_key));
	if (!nla)
		goto nla_put_failure;
	eth_key = nla_data(nla);
	memcpy(eth_key->eth_src, swkey->eth.src, ETH_ALEN);
	memcpy(eth_key->eth_dst, swkey->eth.dst, ETH_ALEN);

	if (swkey->eth.tci || swkey->eth.type == htons(ETH_P_8021Q)) {
		if (nla_put_be16(skb, OVS_KEY_ATTR_ETHERTYPE, htons(ETH_P_8021Q)) ||
		    nla_put_be16(skb, OVS_KEY_ATTR_VLAN, swkey->eth.tci))
			goto nla_put_failure;
		encap = nla_nest_start(skb, OVS_KEY_ATTR_ENCAP);
		if (!swkey->eth.tci)
			goto unencap;
	} else {
		encap = NULL;
	}

	if (swkey->eth.type == htons(ETH_P_802_2))
		goto unencap;

	if (nla_put_be16(skb, OVS_KEY_ATTR_ETHERTYPE, swkey->eth.type))
		goto nla_put_failure;

	if (swkey->eth.type == htons(ETH_P_IP)) {
		struct ovs_key_ipv4 *ipv4_key;

		nla = nla_reserve(skb, OVS_KEY_ATTR_IPV4, sizeof(*ipv4_key));
		if (!nla)
			goto nla_put_failure;
		ipv4_key = nla_data(nla);
		ipv4_key->ipv4_src = swkey->ipv4.addr.src;
		ipv4_key->ipv4_dst = swkey->ipv4.addr.dst;
		ipv4_key->ipv4_proto = swkey->ip.proto;
		ipv4_key->ipv4_tos = swkey->ip.tos;
		ipv4_key->ipv4_ttl = swkey->ip.ttl;
		ipv4_key->ipv4_frag = swkey->ip.frag;
	} else if (swkey->eth.type == htons(ETH_P_IPV6)) {
		struct ovs_key_ipv6 *ipv6_key;

		nla = nla_reserve(skb, OVS_KEY_ATTR_IPV6, sizeof(*ipv6_key));
		if (!nla)
			goto nla_put_failure;
		ipv6_key = nla_data(nla);
		memcpy(ipv6_key->ipv6_src, &swkey->ipv6.addr.src,
				sizeof(ipv6_key->ipv6_src));
		memcpy(ipv6_key->ipv6_dst, &swkey->ipv6.addr.dst,
				sizeof(ipv6_key->ipv6_dst));
		ipv6_key->ipv6_label = swkey->ipv6.label;
		ipv6_key->ipv6_proto = swkey->ip.proto;
		ipv6_key->ipv6_tclass = swkey->ip.tos;
		ipv6_key->ipv6_hlimit = swkey->ip.ttl;
		ipv6_key->ipv6_frag = swkey->ip.frag;
	} else if (swkey->eth.type == htons(ETH_P_ARP) ||
		   swkey->eth.type == htons(ETH_P_RARP)) {
		struct ovs_key_arp *arp_key;

		nla = nla_reserve(skb, OVS_KEY_ATTR_ARP, sizeof(*arp_key));
		if (!nla)
			goto nla_put_failure;
		arp_key = nla_data(nla);
		memset(arp_key, 0, sizeof(struct ovs_key_arp));
		arp_key->arp_sip = swkey->ipv4.addr.src;
		arp_key->arp_tip = swkey->ipv4.addr.dst;
		arp_key->arp_op = htons(swkey->ip.proto);
		memcpy(arp_key->arp_sha, swkey->ipv4.arp.sha, ETH_ALEN);
		memcpy(arp_key->arp_tha, swkey->ipv4.arp.tha, ETH_ALEN);
	}

	if ((swkey->eth.type == htons(ETH_P_IP) ||
	     swkey->eth.type == htons(ETH_P_IPV6)) &&
	     swkey->ip.frag != OVS_FRAG_TYPE_LATER) {

		if (swkey->ip.proto == IPPROTO_TCP) {
			struct ovs_key_tcp *tcp_key;

			nla = nla_reserve(skb, OVS_KEY_ATTR_TCP, sizeof(*tcp_key));
			if (!nla)
				goto nla_put_failure;
			tcp_key = nla_data(nla);
			if (swkey->eth.type == htons(ETH_P_IP)) {
				tcp_key->tcp_src = swkey->ipv4.tp.src;
				tcp_key->tcp_dst = swkey->ipv4.tp.dst;
			} else if (swkey->eth.type == htons(ETH_P_IPV6)) {
				tcp_key->tcp_src = swkey->ipv6.tp.src;
				tcp_key->tcp_dst = swkey->ipv6.tp.dst;
			}
		} else if (swkey->ip.proto == IPPROTO_UDP) {
			struct ovs_key_udp *udp_key;

			nla = nla_reserve(skb, OVS_KEY_ATTR_UDP, sizeof(*udp_key));
			if (!nla)
				goto nla_put_failure;
			udp_key = nla_data(nla);
			if (swkey->eth.type == htons(ETH_P_IP)) {
				udp_key->udp_src = swkey->ipv4.tp.src;
				udp_key->udp_dst = swkey->ipv4.tp.dst;
			} else if (swkey->eth.type == htons(ETH_P_IPV6)) {
				udp_key->udp_src = swkey->ipv6.tp.src;
				udp_key->udp_dst = swkey->ipv6.tp.dst;
			}
		} else if (swkey->eth.type == htons(ETH_P_IP) &&
			   swkey->ip.proto == IPPROTO_ICMP) {
			struct ovs_key_icmp *icmp_key;

			nla = nla_reserve(skb, OVS_KEY_ATTR_ICMP, sizeof(*icmp_key));
			if (!nla)
				goto nla_put_failure;
			icmp_key = nla_data(nla);
			icmp_key->icmp_type = ntohs(swkey->ipv4.tp.src);
			icmp_key->icmp_code = ntohs(swkey->ipv4.tp.dst);
		} else if (swkey->eth.type == htons(ETH_P_IPV6) &&
			   swkey->ip.proto == IPPROTO_ICMPV6) {
			struct ovs_key_icmpv6 *icmpv6_key;

			nla = nla_reserve(skb, OVS_KEY_ATTR_ICMPV6,
						sizeof(*icmpv6_key));
			if (!nla)
				goto nla_put_failure;
			icmpv6_key = nla_data(nla);
			icmpv6_key->icmpv6_type = ntohs(swkey->ipv6.tp.src);
			icmpv6_key->icmpv6_code = ntohs(swkey->ipv6.tp.dst);

			if (icmpv6_key->icmpv6_type == NDISC_NEIGHBOUR_SOLICITATION ||
			    icmpv6_key->icmpv6_type == NDISC_NEIGHBOUR_ADVERTISEMENT) {
				struct ovs_key_nd *nd_key;

				nla = nla_reserve(skb, OVS_KEY_ATTR_ND, sizeof(*nd_key));
				if (!nla)
					goto nla_put_failure;
				nd_key = nla_data(nla);
				memcpy(nd_key->nd_target, &swkey->ipv6.nd.target,
							sizeof(nd_key->nd_target));
				memcpy(nd_key->nd_sll, swkey->ipv6.nd.sll, ETH_ALEN);
				memcpy(nd_key->nd_tll, swkey->ipv6.nd.tll, ETH_ALEN);
			}
		}
	}

unencap:
	if (encap)
		nla_nest_end(skb, encap);

	return 0;

nla_put_failure:
	return -EMSGSIZE;
}

/* Initializes the flow module.
 * Returns zero if successful or a negative error code. */
int ovs_flow_init(void)
{
	flow_cache = kmem_cache_create("sw_flow", sizeof(struct sw_flow), 0,
					0, NULL);
	if (flow_cache == NULL)
		return -ENOMEM;

	return 0;
}

/* Uninitializes the flow module. */
void ovs_flow_exit(void)
{
	kmem_cache_destroy(flow_cache);
=======
			if (tcphdr_ok(skb)) {
				struct tcphdr *tcp = tcp_hdr(skb);
				key->tp.src = tcp->source;
				key->tp.dst = tcp->dest;
				key->tp.flags = TCP_FLAGS_BE16(tcp);
			} else {
				memset(&key->tp, 0, sizeof(key->tp));
			}
		} else if (key->ip.proto == NEXTHDR_UDP) {
			if (udphdr_ok(skb)) {
				struct udphdr *udp = udp_hdr(skb);
				key->tp.src = udp->source;
				key->tp.dst = udp->dest;
			} else {
				memset(&key->tp, 0, sizeof(key->tp));
			}
		} else if (key->ip.proto == NEXTHDR_SCTP) {
			if (sctphdr_ok(skb)) {
				struct sctphdr *sctp = sctp_hdr(skb);
				key->tp.src = sctp->source;
				key->tp.dst = sctp->dest;
			} else {
				memset(&key->tp, 0, sizeof(key->tp));
			}
		} else if (key->ip.proto == NEXTHDR_ICMP) {
			if (icmp6hdr_ok(skb)) {
				error = parse_icmpv6(skb, key, nh_len);
				if (error)
					return error;
			} else {
				memset(&key->tp, 0, sizeof(key->tp));
			}
		}
	}
	return 0;
}

int ovs_flow_key_update(struct sk_buff *skb, struct sw_flow_key *key)
{
	return key_extract(skb, key);
}

int ovs_flow_key_extract(const struct ip_tunnel_info *tun_info,
			 struct sk_buff *skb, struct sw_flow_key *key)
{
	/* Extract metadata from packet. */
	if (tun_info) {
		key->tun_proto = ip_tunnel_info_af(tun_info);
		memcpy(&key->tun_key, &tun_info->key, sizeof(key->tun_key));

		if (tun_info->options_len) {
			BUILD_BUG_ON((1 << (sizeof(tun_info->options_len) *
						   8)) - 1
					> sizeof(key->tun_opts));

			ip_tunnel_info_opts_get(TUN_METADATA_OPTS(key, tun_info->options_len),
						tun_info);
			key->tun_opts_len = tun_info->options_len;
		} else {
			key->tun_opts_len = 0;
		}
	} else  {
		key->tun_proto = 0;
		key->tun_opts_len = 0;
		memset(&key->tun_key, 0, sizeof(key->tun_key));
	}

	key->phy.priority = skb->priority;
	key->phy.in_port = OVS_CB(skb)->input_vport->port_no;
	key->phy.skb_mark = skb->mark;
	ovs_ct_fill_key(skb, key);
	key->ovs_flow_hash = 0;
	key->recirc_id = 0;

	return key_extract(skb, key);
}

int ovs_flow_key_extract_userspace(struct net *net, const struct nlattr *attr,
				   struct sk_buff *skb,
				   struct sw_flow_key *key, bool log)
{
	int err;

	/* Extract metadata from netlink attributes. */
	err = ovs_nla_get_flow_metadata(net, attr, key, log);
	if (err)
		return err;

	return key_extract(skb, key);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
