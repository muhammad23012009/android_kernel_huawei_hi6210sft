#ifndef _XT_SOCKET_H
#define _XT_SOCKET_H

#include <linux/types.h>

enum {
	XT_SOCKET_TRANSPARENT = 1 << 0,
<<<<<<< HEAD
=======
	XT_SOCKET_NOWILDCARD = 1 << 1,
	XT_SOCKET_RESTORESKMARK = 1 << 2,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct xt_socket_mtinfo1 {
	__u8 flags;
};
<<<<<<< HEAD

void xt_socket_put_sk(struct sock *sk);
struct sock *xt_socket_get4_sk(const struct sk_buff *skb,
			       struct xt_action_param *par);
struct sock *xt_socket_get6_sk(const struct sk_buff *skb,
			       struct xt_action_param *par);
=======
#define XT_SOCKET_FLAGS_V1 XT_SOCKET_TRANSPARENT

struct xt_socket_mtinfo2 {
	__u8 flags;
};
#define XT_SOCKET_FLAGS_V2 (XT_SOCKET_TRANSPARENT | XT_SOCKET_NOWILDCARD)

struct xt_socket_mtinfo3 {
	__u8 flags;
};
#define XT_SOCKET_FLAGS_V3 (XT_SOCKET_TRANSPARENT \
			   | XT_SOCKET_NOWILDCARD \
			   | XT_SOCKET_RESTORESKMARK)

struct sock *xt_socket_lookup_slow_v4(struct net *net,
				      const struct sk_buff *skb,
				      const struct net_device *indev);
struct sock *xt_socket_lookup_slow_v6(struct net *net,
				      const struct sk_buff *skb,
				      const struct net_device *indev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _XT_SOCKET_H */
