#ifndef __NETNS_CONNTRACK_H
#define __NETNS_CONNTRACK_H

#include <linux/list.h>
#include <linux/list_nulls.h>
#include <linux/atomic.h>
<<<<<<< HEAD
#include <linux/netfilter/nf_conntrack_tcp.h>
=======
#include <linux/workqueue.h>
#include <linux/netfilter/nf_conntrack_tcp.h>
#include <linux/seqlock.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct ctl_table_header;
struct nf_conntrack_ecache;

struct nf_proto_net {
#ifdef CONFIG_SYSCTL
	struct ctl_table_header *ctl_table_header;
	struct ctl_table        *ctl_table;
<<<<<<< HEAD
#ifdef CONFIG_NF_CONNTRACK_PROC_COMPAT
	struct ctl_table_header *ctl_compat_header;
	struct ctl_table        *ctl_compat_table;
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
	unsigned int		users;
};

struct nf_generic_net {
	struct nf_proto_net pn;
	unsigned int timeout;
};

struct nf_tcp_net {
	struct nf_proto_net pn;
	unsigned int timeouts[TCP_CONNTRACK_TIMEOUT_MAX];
	unsigned int tcp_loose;
	unsigned int tcp_be_liberal;
	unsigned int tcp_max_retrans;
};

enum udp_conntrack {
	UDP_CT_UNREPLIED,
	UDP_CT_REPLIED,
	UDP_CT_MAX
};

struct nf_udp_net {
	struct nf_proto_net pn;
	unsigned int timeouts[UDP_CT_MAX];
};

struct nf_icmp_net {
	struct nf_proto_net pn;
	unsigned int timeout;
};

struct nf_ip_net {
	struct nf_generic_net   generic;
	struct nf_tcp_net	tcp;
	struct nf_udp_net	udp;
	struct nf_icmp_net	icmp;
	struct nf_icmp_net	icmpv6;
<<<<<<< HEAD
#if defined(CONFIG_SYSCTL) && defined(CONFIG_NF_CONNTRACK_PROC_COMPAT)
	struct ctl_table_header *ctl_table_header;
	struct ctl_table	*ctl_table;
#endif
=======
};

struct ct_pcpu {
	spinlock_t		lock;
	struct hlist_nulls_head unconfirmed;
	struct hlist_nulls_head dying;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct netns_ct {
	atomic_t		count;
	unsigned int		expect_count;
<<<<<<< HEAD
	unsigned int		htable_size;
	struct kmem_cache	*nf_conntrack_cachep;
	struct hlist_nulls_head	*hash;
	struct hlist_head	*expect_hash;
	struct hlist_nulls_head	unconfirmed;
	struct hlist_nulls_head	dying;
	struct hlist_nulls_head tmpl;
	struct ip_conntrack_stat __percpu *stat;
	struct nf_ct_event_notifier __rcu *nf_conntrack_event_cb;
	struct nf_exp_event_notifier __rcu *nf_expect_event_cb;
	int			sysctl_events;
	unsigned int		sysctl_events_retry_timeout;
	int			sysctl_acct;
	int			sysctl_tstamp;
	int			sysctl_checksum;
	unsigned int		sysctl_log_invalid; /* Log invalid packets */
	int			sysctl_auto_assign_helper;
	bool			auto_assign_helper_warned;
=======
#ifdef CONFIG_NF_CONNTRACK_EVENTS
	struct delayed_work ecache_dwork;
	bool ecache_dwork_pending;
#endif
#ifdef CONFIG_SYSCTL
	struct ctl_table_header	*sysctl_header;
	struct ctl_table_header	*acct_sysctl_header;
	struct ctl_table_header	*tstamp_sysctl_header;
	struct ctl_table_header	*event_sysctl_header;
	struct ctl_table_header	*helper_sysctl_header;
#endif
	unsigned int		sysctl_log_invalid; /* Log invalid packets */
	int			sysctl_events;
	int			sysctl_acct;
	int			sysctl_auto_assign_helper;
	bool			auto_assign_helper_warned;
	int			sysctl_tstamp;
	int			sysctl_checksum;

	struct ct_pcpu __percpu *pcpu_lists;
	struct ip_conntrack_stat __percpu *stat;
	struct nf_ct_event_notifier __rcu *nf_conntrack_event_cb;
	struct nf_exp_event_notifier __rcu *nf_expect_event_cb;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct nf_ip_net	nf_ct_proto;
#if defined(CONFIG_NF_CONNTRACK_LABELS)
	unsigned int		labels_used;
	u8			label_words;
#endif
<<<<<<< HEAD
#ifdef CONFIG_NF_NAT_NEEDED
	struct hlist_head	*nat_bysource;
	unsigned int		nat_htable_size;
#endif
#ifdef CONFIG_SYSCTL
	struct ctl_table_header	*sysctl_header;
	struct ctl_table_header	*acct_sysctl_header;
	struct ctl_table_header	*tstamp_sysctl_header;
	struct ctl_table_header	*event_sysctl_header;
	struct ctl_table_header	*helper_sysctl_header;
#endif
	char			*slabname;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
#endif
