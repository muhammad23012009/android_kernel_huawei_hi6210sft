#ifndef __NET_TC_IPT_H
#define __NET_TC_IPT_H

#include <net/act_api.h>

struct xt_entry_target;

struct tcf_ipt {
<<<<<<< HEAD
	struct tcf_common	common;
=======
	struct tc_action	common;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32			tcfi_hook;
	char			*tcfi_tname;
	struct xt_entry_target	*tcfi_t;
};
<<<<<<< HEAD
#define to_ipt(pc) \
	container_of(pc, struct tcf_ipt, common)
=======
#define to_ipt(a) ((struct tcf_ipt *)a)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __NET_TC_IPT_H */
