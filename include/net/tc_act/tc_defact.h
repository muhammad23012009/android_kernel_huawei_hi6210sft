#ifndef __NET_TC_DEF_H
#define __NET_TC_DEF_H

#include <net/act_api.h>

struct tcf_defact {
<<<<<<< HEAD
	struct tcf_common	common;
	u32     		tcfd_datalen;
	void    		*tcfd_defdata;
};
#define to_defact(pc) \
	container_of(pc, struct tcf_defact, common)
=======
	struct tc_action	common;
	u32		tcfd_datalen;
	void		*tcfd_defdata;
};
#define to_defact(a) ((struct tcf_defact *)a)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __NET_TC_DEF_H */
