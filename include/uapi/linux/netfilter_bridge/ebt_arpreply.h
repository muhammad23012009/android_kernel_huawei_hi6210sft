#ifndef __LINUX_BRIDGE_EBT_ARPREPLY_H
#define __LINUX_BRIDGE_EBT_ARPREPLY_H

<<<<<<< HEAD
=======
#include <linux/if_ether.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct ebt_arpreply_info {
	unsigned char mac[ETH_ALEN];
	int target;
};
#define EBT_ARPREPLY_TARGET "arpreply"

#endif
