#ifndef __LINUX_TC_NAT_H
#define __LINUX_TC_NAT_H

#include <linux/pkt_cls.h>
#include <linux/types.h>

#define TCA_ACT_NAT 9

enum {
	TCA_NAT_UNSPEC,
	TCA_NAT_PARMS,
	TCA_NAT_TM,
<<<<<<< HEAD
=======
	TCA_NAT_PAD,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__TCA_NAT_MAX
};
#define TCA_NAT_MAX (__TCA_NAT_MAX - 1)

#define TCA_NAT_FLAG_EGRESS 1

struct tc_nat {
	tc_gen;
	__be32 old_addr;
	__be32 new_addr;
	__be32 mask;
	__u32 flags;
};

#endif
