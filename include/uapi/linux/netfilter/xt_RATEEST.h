#ifndef _XT_RATEEST_TARGET_H
#define _XT_RATEEST_TARGET_H

#include <linux/types.h>
<<<<<<< HEAD
=======
#include <linux/if.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct xt_rateest_target_info {
	char			name[IFNAMSIZ];
	__s8			interval;
	__u8		ewma_log;

	/* Used internally by the kernel */
	struct xt_rateest	*est __attribute__((aligned(8)));
};

#endif /* _XT_RATEEST_TARGET_H */
