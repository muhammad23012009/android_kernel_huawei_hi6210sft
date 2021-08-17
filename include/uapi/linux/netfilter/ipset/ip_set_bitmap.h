#ifndef _UAPI__IP_SET_BITMAP_H
#define _UAPI__IP_SET_BITMAP_H

<<<<<<< HEAD
=======
#include <linux/netfilter/ipset/ip_set.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Bitmap type specific error codes */
enum {
	/* The element is out of the range of the set */
	IPSET_ERR_BITMAP_RANGE = IPSET_ERR_TYPE_SPECIFIC,
	/* The range exceeds the size limit of the set type */
	IPSET_ERR_BITMAP_RANGE_SIZE,
};


#endif /* _UAPI__IP_SET_BITMAP_H */
