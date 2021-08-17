#ifndef _XT_MAC_H
#define _XT_MAC_H

<<<<<<< HEAD
=======
#include <linux/if_ether.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct xt_mac_info {
    unsigned char srcaddr[ETH_ALEN];
    int invert;
};
#endif /*_XT_MAC_H*/
