/*
 *	IPV6 GSO/GRO offload support
 *	Linux INET6 implementation
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 *      TCPv6 GSO/GRO support
 */
#include <linux/skbuff.h>
#include <net/protocol.h>
#include <net/tcp.h>
#include <net/ip6_checksum.h>
#include "ip6_offload.h"

<<<<<<< HEAD
static int tcp_v6_gso_send_check(struct sk_buff *skb)
{
	const struct ipv6hdr *ipv6h;
	struct tcphdr *th;

	if (!pskb_may_pull(skb, sizeof(*th)))
		return -EINVAL;

	ipv6h = ipv6_hdr(skb);
	th = tcp_hdr(skb);

	th->check = 0;
	skb->ip_summed = CHECKSUM_PARTIAL;
	__tcp_v6_send_check(skb, &ipv6h->saddr, &ipv6h->daddr);
	return 0;
}

static struct sk_buff **tcp6_gro_receive(struct sk_buff **head,
					 struct sk_buff *skb)
{
	const struct ipv6hdr *iph = skb_gro_network_header(skb);
	__wsum wsum;
	__sum16 sum;

	switch (skb->ip_summed) {
	case CHECKSUM_COMPLETE:
		if (!tcp_v6_check(skb_gro_len(skb), &iph->saddr, &iph->daddr,
				  skb->csum)) {
			skb->ip_summed = CHECKSUM_UNNECESSARY;
			break;
		}
flush:
		NAPI_GRO_CB(skb)->flush = 1;
		return NULL;

	case CHECKSUM_NONE:
		wsum = ~csum_unfold(csum_ipv6_magic(&iph->saddr, &iph->daddr,
						    skb_gro_len(skb),
						    IPPROTO_TCP, 0));
		sum = csum_fold(skb_checksum(skb,
					     skb_gro_offset(skb),
					     skb_gro_len(skb),
					     wsum));
		if (sum)
			goto flush;

		skb->ip_summed = CHECKSUM_UNNECESSARY;
		break;
=======
static struct sk_buff **tcp6_gro_receive(struct sk_buff **head,
					 struct sk_buff *skb)
{
	/* Don't bother verifying checksum if we're going to flush anyway. */
	if (!NAPI_GRO_CB(skb)->flush &&
	    skb_gro_checksum_validate(skb, IPPROTO_TCP,
				      ip6_gro_compute_pseudo)) {
		NAPI_GRO_CB(skb)->flush = 1;
		return NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return tcp_gro_receive(head, skb);
}

<<<<<<< HEAD
static int tcp6_gro_complete(struct sk_buff *skb)
=======
static int tcp6_gro_complete(struct sk_buff *skb, int thoff)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	const struct ipv6hdr *iph = ipv6_hdr(skb);
	struct tcphdr *th = tcp_hdr(skb);

<<<<<<< HEAD
	th->check = ~tcp_v6_check(skb->len - skb_transport_offset(skb),
				  &iph->saddr, &iph->daddr, 0);
	skb_shinfo(skb)->gso_type = SKB_GSO_TCPV6;
=======
	th->check = ~tcp_v6_check(skb->len - thoff, &iph->saddr,
				  &iph->daddr, 0);
	skb_shinfo(skb)->gso_type |= SKB_GSO_TCPV6;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return tcp_gro_complete(skb);
}

<<<<<<< HEAD
static const struct net_offload tcpv6_offload = {
	.callbacks = {
		.gso_send_check	=	tcp_v6_gso_send_check,
		.gso_segment	=	tcp_tso_segment,
=======
static struct sk_buff *tcp6_gso_segment(struct sk_buff *skb,
					netdev_features_t features)
{
	struct tcphdr *th;

	if (!(skb_shinfo(skb)->gso_type & SKB_GSO_TCPV6))
		return ERR_PTR(-EINVAL);

	if (!pskb_may_pull(skb, sizeof(*th)))
		return ERR_PTR(-EINVAL);

	if (unlikely(skb->ip_summed != CHECKSUM_PARTIAL)) {
		const struct ipv6hdr *ipv6h = ipv6_hdr(skb);
		struct tcphdr *th = tcp_hdr(skb);

		/* Set up pseudo header, usually expect stack to have done
		 * this.
		 */

		th->check = 0;
		skb->ip_summed = CHECKSUM_PARTIAL;
		__tcp_v6_send_check(skb, &ipv6h->saddr, &ipv6h->daddr);
	}

	return tcp_gso_segment(skb, features);
}
static const struct net_offload tcpv6_offload = {
	.callbacks = {
		.gso_segment	=	tcp6_gso_segment,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.gro_receive	=	tcp6_gro_receive,
		.gro_complete	=	tcp6_gro_complete,
	},
};

int __init tcpv6_offload_init(void)
{
	return inet6_add_offload(&tcpv6_offload, IPPROTO_TCP);
}
