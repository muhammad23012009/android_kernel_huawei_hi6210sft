#ifndef __NET_TC_NAT_H
#define __NET_TC_NAT_H

#include <linux/types.h>
#include <net/act_api.h>

struct tcf_nat {
<<<<<<< HEAD
	struct tcf_common common;
=======
	struct tc_action common;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	__be32 old_addr;
	__be32 new_addr;
	__be32 mask;
	u32 flags;
};

<<<<<<< HEAD
static inline struct tcf_nat *to_tcf_nat(struct tcf_common *pc)
{
	return container_of(pc, struct tcf_nat, common);
}
=======
#define to_tcf_nat(a) ((struct tcf_nat *)a)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __NET_TC_NAT_H */
