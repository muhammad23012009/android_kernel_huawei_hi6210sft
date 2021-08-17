#ifndef _XT_TEE_TARGET_H
#define _XT_TEE_TARGET_H

<<<<<<< HEAD
=======
#include <linux/netfilter.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct xt_tee_tginfo {
	union nf_inet_addr gw;
	char oif[16];

	/* used internally by the kernel */
	struct xt_tee_priv *priv __attribute__((aligned(8)));
};

#endif /* _XT_TEE_TARGET_H */
