#ifndef __NET_NS_HASH_H__
#define __NET_NS_HASH_H__

<<<<<<< HEAD
#include <asm/cache.h>

struct net;

static inline unsigned int net_hash_mix(struct net *net)
{
#ifdef CONFIG_NET_NS
	/*
	 * shift this right to eliminate bits, that are
	 * always zeroed
	 */

	return (unsigned)(((unsigned long)net) >> L1_CACHE_SHIFT);
#else
	return 0;
#endif
=======
#include <net/net_namespace.h>

static inline u32 net_hash_mix(const struct net *net)
{
	return net->hash_mix;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#endif
