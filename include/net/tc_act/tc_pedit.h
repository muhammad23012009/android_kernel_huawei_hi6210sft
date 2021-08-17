#ifndef __NET_TC_PED_H
#define __NET_TC_PED_H

#include <net/act_api.h>

struct tcf_pedit {
<<<<<<< HEAD
	struct tcf_common	common;
=======
	struct tc_action	common;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned char		tcfp_nkeys;
	unsigned char		tcfp_flags;
	struct tc_pedit_key	*tcfp_keys;
};
<<<<<<< HEAD
#define to_pedit(pc) \
	container_of(pc, struct tcf_pedit, common)
=======
#define to_pedit(a) ((struct tcf_pedit *)a)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __NET_TC_PED_H */
