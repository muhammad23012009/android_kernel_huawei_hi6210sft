#ifndef __LINUX_TC_IPT_H
#define __LINUX_TC_IPT_H

#include <linux/pkt_cls.h>

#define TCA_ACT_IPT 6
<<<<<<< HEAD
=======
#define TCA_ACT_XT 10
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

enum {
	TCA_IPT_UNSPEC,
	TCA_IPT_TABLE,
	TCA_IPT_HOOK,
	TCA_IPT_INDEX,
	TCA_IPT_CNT,
	TCA_IPT_TM,
	TCA_IPT_TARG,
<<<<<<< HEAD
=======
	TCA_IPT_PAD,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__TCA_IPT_MAX
};
#define TCA_IPT_MAX (__TCA_IPT_MAX - 1)
                                                                                
#endif
