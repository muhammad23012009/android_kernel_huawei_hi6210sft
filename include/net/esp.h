#ifndef _NET_ESP_H
#define _NET_ESP_H

#include <linux/skbuff.h>

<<<<<<< HEAD
struct crypto_aead;

struct esp_data {
	/* 0..255 */
	int padlen;

	/* Confidentiality & Integrity */
	struct crypto_aead *aead;
};

extern void *pskb_put(struct sk_buff *skb, struct sk_buff *tail, int len);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct ip_esp_hdr;

static inline struct ip_esp_hdr *ip_esp_hdr(const struct sk_buff *skb)
{
	return (struct ip_esp_hdr *)skb_transport_header(skb);
}

#endif
