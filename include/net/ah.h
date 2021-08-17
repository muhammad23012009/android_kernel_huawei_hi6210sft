#ifndef _NET_AH_H
#define _NET_AH_H

#include <linux/skbuff.h>

<<<<<<< HEAD
/* This is the maximum truncated ICV length that we know of. */
#define MAX_AH_AUTH_LEN	64

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct crypto_ahash;

struct ah_data {
	int			icv_full_len;
	int			icv_trunc_len;

	struct crypto_ahash	*ahash;
};

struct ip_auth_hdr;

static inline struct ip_auth_hdr *ip_auth_hdr(const struct sk_buff *skb)
{
	return (struct ip_auth_hdr *)skb_transport_header(skb);
}

#endif
