/*
 * SELinux support for the XFRM LSM hooks
 *
 * Author : Trent Jaeger, <jaegert@us.ibm.com>
 * Updated : Venkat Yekkirala, <vyekkirala@TrustedCS.com>
 */
#ifndef _SELINUX_XFRM_H_
#define _SELINUX_XFRM_H_

#include <net/flow.h>

int selinux_xfrm_policy_alloc(struct xfrm_sec_ctx **ctxp,
<<<<<<< HEAD
			      struct xfrm_user_sec_ctx *sec_ctx);
=======
			      struct xfrm_user_sec_ctx *uctx,
			      gfp_t gfp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int selinux_xfrm_policy_clone(struct xfrm_sec_ctx *old_ctx,
			      struct xfrm_sec_ctx **new_ctxp);
void selinux_xfrm_policy_free(struct xfrm_sec_ctx *ctx);
int selinux_xfrm_policy_delete(struct xfrm_sec_ctx *ctx);
int selinux_xfrm_state_alloc(struct xfrm_state *x,
<<<<<<< HEAD
	struct xfrm_user_sec_ctx *sec_ctx, u32 secid);
=======
			     struct xfrm_user_sec_ctx *uctx);
int selinux_xfrm_state_alloc_acquire(struct xfrm_state *x,
				     struct xfrm_sec_ctx *polsec, u32 secid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void selinux_xfrm_state_free(struct xfrm_state *x);
int selinux_xfrm_state_delete(struct xfrm_state *x);
int selinux_xfrm_policy_lookup(struct xfrm_sec_ctx *ctx, u32 fl_secid, u8 dir);
int selinux_xfrm_state_pol_flow_match(struct xfrm_state *x,
<<<<<<< HEAD
			struct xfrm_policy *xp, const struct flowi *fl);

/*
 * Extract the security blob from the sock (it's actually on the socket)
 */
static inline struct inode_security_struct *get_sock_isec(struct sock *sk)
{
	if (!sk->sk_socket)
		return NULL;

	return SOCK_INODE(sk->sk_socket)->i_security;
}
=======
				      struct xfrm_policy *xp,
				      const struct flowi *fl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_SECURITY_NETWORK_XFRM
extern atomic_t selinux_xfrm_refcount;

static inline int selinux_xfrm_enabled(void)
{
	return (atomic_read(&selinux_xfrm_refcount) > 0);
}

<<<<<<< HEAD
int selinux_xfrm_sock_rcv_skb(u32 sid, struct sk_buff *skb,
			struct common_audit_data *ad);
int selinux_xfrm_postroute_last(u32 isec_sid, struct sk_buff *skb,
			struct common_audit_data *ad, u8 proto);
=======
int selinux_xfrm_sock_rcv_skb(u32 sk_sid, struct sk_buff *skb,
			      struct common_audit_data *ad);
int selinux_xfrm_postroute_last(u32 sk_sid, struct sk_buff *skb,
				struct common_audit_data *ad, u8 proto);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int selinux_xfrm_decode_session(struct sk_buff *skb, u32 *sid, int ckall);
int selinux_xfrm_skb_sid(struct sk_buff *skb, u32 *sid);

static inline void selinux_xfrm_notify_policyload(void)
{
<<<<<<< HEAD
	atomic_inc(&flow_cache_genid);
	rt_genid_bump(&init_net);
=======
	struct net *net;

	rtnl_lock();
	for_each_net(net) {
		atomic_inc(&net->xfrm.flow_cache_genid);
		rt_genid_bump_all(net);
	}
	rtnl_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#else
static inline int selinux_xfrm_enabled(void)
{
	return 0;
}

<<<<<<< HEAD
static inline int selinux_xfrm_sock_rcv_skb(u32 isec_sid, struct sk_buff *skb,
			struct common_audit_data *ad)
=======
static inline int selinux_xfrm_sock_rcv_skb(u32 sk_sid, struct sk_buff *skb,
					    struct common_audit_data *ad)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return 0;
}

<<<<<<< HEAD
static inline int selinux_xfrm_postroute_last(u32 isec_sid, struct sk_buff *skb,
			struct common_audit_data *ad, u8 proto)
=======
static inline int selinux_xfrm_postroute_last(u32 sk_sid, struct sk_buff *skb,
					      struct common_audit_data *ad,
					      u8 proto)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return 0;
}

<<<<<<< HEAD
static inline int selinux_xfrm_decode_session(struct sk_buff *skb, u32 *sid, int ckall)
=======
static inline int selinux_xfrm_decode_session(struct sk_buff *skb, u32 *sid,
					      int ckall)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	*sid = SECSID_NULL;
	return 0;
}

static inline void selinux_xfrm_notify_policyload(void)
{
}

static inline int selinux_xfrm_skb_sid(struct sk_buff *skb, u32 *sid)
{
	*sid = SECSID_NULL;
	return 0;
}
#endif

#endif /* _SELINUX_XFRM_H_ */
