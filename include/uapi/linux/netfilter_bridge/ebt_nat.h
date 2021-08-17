#ifndef __LINUX_BRIDGE_EBT_NAT_H
#define __LINUX_BRIDGE_EBT_NAT_H

<<<<<<< HEAD
=======
#include <linux/if_ether.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define NAT_ARP_BIT  (0x00000010)
struct ebt_nat_info {
	unsigned char mac[ETH_ALEN];
	/* EBT_ACCEPT, EBT_DROP, EBT_CONTINUE or EBT_RETURN */
	int target;
};
#define EBT_SNAT_TARGET "snat"
#define EBT_DNAT_TARGET "dnat"

#endif
