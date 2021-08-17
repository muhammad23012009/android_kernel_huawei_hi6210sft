/* IPv4-specific defines for netfilter. 
 * (C)1998 Rusty Russell -- This code is GPL.
 */
#ifndef __LINUX_IP_NETFILTER_H
#define __LINUX_IP_NETFILTER_H

#include <uapi/linux/netfilter_ipv4.h>

<<<<<<< HEAD
extern int ip_route_me_harder(struct sk_buff *skb, unsigned addr_type);
extern __sum16 nf_ip_checksum(struct sk_buff *skb, unsigned int hook,
				   unsigned int dataoff, u_int8_t protocol);
=======
int ip_route_me_harder(struct net *net, struct sk_buff *skb, unsigned addr_type);
__sum16 nf_ip_checksum(struct sk_buff *skb, unsigned int hook,
		       unsigned int dataoff, u_int8_t protocol);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /*__LINUX_IP_NETFILTER_H*/
