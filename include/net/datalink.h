#ifndef _NET_INET_DATALINK_H_
#define _NET_INET_DATALINK_H_

struct datalink_proto {
        unsigned char   type[8];

	struct llc_sap   *sap;

        unsigned short  header_length;

        int     (*rcvfunc)(struct sk_buff *, struct net_device *,
                                struct packet_type *, struct net_device *);
	int     (*request)(struct datalink_proto *, struct sk_buff *,
                                        unsigned char *);
	struct list_head node;
};

<<<<<<< HEAD
=======
struct datalink_proto *make_EII_client(void);
void destroy_EII_client(struct datalink_proto *dl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
