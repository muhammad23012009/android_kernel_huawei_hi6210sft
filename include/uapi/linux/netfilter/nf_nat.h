#ifndef _NETFILTER_NF_NAT_H
#define _NETFILTER_NF_NAT_H

#include <linux/netfilter.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>

<<<<<<< HEAD
#define NF_NAT_RANGE_MAP_IPS		1
#define NF_NAT_RANGE_PROTO_SPECIFIED	2
#define NF_NAT_RANGE_PROTO_RANDOM	4
#define NF_NAT_RANGE_PERSISTENT		8
=======
#define NF_NAT_RANGE_MAP_IPS			(1 << 0)
#define NF_NAT_RANGE_PROTO_SPECIFIED		(1 << 1)
#define NF_NAT_RANGE_PROTO_RANDOM		(1 << 2)
#define NF_NAT_RANGE_PERSISTENT			(1 << 3)
#define NF_NAT_RANGE_PROTO_RANDOM_FULLY		(1 << 4)

#define NF_NAT_RANGE_PROTO_RANDOM_ALL		\
	(NF_NAT_RANGE_PROTO_RANDOM | NF_NAT_RANGE_PROTO_RANDOM_FULLY)

#define NF_NAT_RANGE_MASK					\
	(NF_NAT_RANGE_MAP_IPS | NF_NAT_RANGE_PROTO_SPECIFIED |	\
	 NF_NAT_RANGE_PROTO_RANDOM | NF_NAT_RANGE_PERSISTENT |	\
	 NF_NAT_RANGE_PROTO_RANDOM_FULLY)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct nf_nat_ipv4_range {
	unsigned int			flags;
	__be32				min_ip;
	__be32				max_ip;
	union nf_conntrack_man_proto	min;
	union nf_conntrack_man_proto	max;
};

struct nf_nat_ipv4_multi_range_compat {
	unsigned int			rangesize;
	struct nf_nat_ipv4_range	range[1];
};

struct nf_nat_range {
	unsigned int			flags;
	union nf_inet_addr		min_addr;
	union nf_inet_addr		max_addr;
	union nf_conntrack_man_proto	min_proto;
	union nf_conntrack_man_proto	max_proto;
};

#endif /* _NETFILTER_NF_NAT_H */
