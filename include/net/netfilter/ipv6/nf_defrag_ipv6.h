#ifndef _NF_DEFRAG_IPV6_H
#define _NF_DEFRAG_IPV6_H

<<<<<<< HEAD
extern void nf_defrag_ipv6_enable(void);

extern int nf_ct_frag6_init(void);
extern void nf_ct_frag6_cleanup(void);
extern struct sk_buff *nf_ct_frag6_gather(struct sk_buff *skb, u32 user);
extern void nf_ct_frag6_consume_orig(struct sk_buff *skb);
=======
void nf_defrag_ipv6_enable(void);

int nf_ct_frag6_init(void);
void nf_ct_frag6_cleanup(void);
int nf_ct_frag6_gather(struct net *net, struct sk_buff *skb, u32 user);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct inet_frags_ctl;

#endif /* _NF_DEFRAG_IPV6_H */
