#ifndef __NET_TC_CSUM_H
#define __NET_TC_CSUM_H

#include <linux/types.h>
#include <net/act_api.h>

struct tcf_csum {
<<<<<<< HEAD
	struct tcf_common common;

	u32 update_flags;
};
#define to_tcf_csum(pc) \
	container_of(pc,struct tcf_csum,common)
=======
	struct tc_action common;

	u32 update_flags;
};
#define to_tcf_csum(a) ((struct tcf_csum *)a)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __NET_TC_CSUM_H */
