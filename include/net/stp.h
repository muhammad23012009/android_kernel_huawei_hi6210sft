#ifndef _NET_STP_H
#define _NET_STP_H

struct stp_proto {
	unsigned char	group_address[ETH_ALEN];
	void		(*rcv)(const struct stp_proto *, struct sk_buff *,
			       struct net_device *);
	void		*data;
};

<<<<<<< HEAD
extern int stp_proto_register(const struct stp_proto *proto);
extern void stp_proto_unregister(const struct stp_proto *proto);
=======
int stp_proto_register(const struct stp_proto *proto);
void stp_proto_unregister(const struct stp_proto *proto);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _NET_STP_H */
