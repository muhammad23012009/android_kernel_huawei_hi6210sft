#ifndef _UAPI__IP_SET_HASH_H
#define _UAPI__IP_SET_HASH_H

<<<<<<< HEAD
=======
#include <linux/netfilter/ipset/ip_set.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Hash type specific error codes */
enum {
	/* Hash is full */
	IPSET_ERR_HASH_FULL = IPSET_ERR_TYPE_SPECIFIC,
	/* Null-valued element */
	IPSET_ERR_HASH_ELEM,
	/* Invalid protocol */
	IPSET_ERR_INVALID_PROTO,
	/* Protocol missing but must be specified */
	IPSET_ERR_MISSING_PROTO,
	/* Range not supported */
	IPSET_ERR_HASH_RANGE_UNSUPPORTED,
	/* Invalid range */
	IPSET_ERR_HASH_RANGE,
};


#endif /* _UAPI__IP_SET_HASH_H */
