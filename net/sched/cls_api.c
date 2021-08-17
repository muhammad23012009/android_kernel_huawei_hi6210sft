/*
 * net/sched/cls_api.c	Packet classifier API.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 * Changes:
 *
 * Eduardo J. Blanco <ejbs@netlabs.com.uy> :990222: kmod support
 *
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <net/net_namespace.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>
#include <net/pkt_cls.h>

/* The list of all installed classifier types */
<<<<<<< HEAD

static struct tcf_proto_ops *tcf_proto_base __read_mostly;
=======
static LIST_HEAD(tcf_proto_base);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Protects list of registered TC modules. It is pure SMP lock. */
static DEFINE_RWLOCK(cls_mod_lock);

/* Find classifier type by string name */

static const struct tcf_proto_ops *tcf_proto_lookup_ops(struct nlattr *kind)
{
<<<<<<< HEAD
	const struct tcf_proto_ops *t = NULL;

	if (kind) {
		read_lock(&cls_mod_lock);
		for (t = tcf_proto_base; t; t = t->next) {
			if (nla_strcmp(kind, t->kind) == 0) {
				if (!try_module_get(t->owner))
					t = NULL;
=======
	const struct tcf_proto_ops *t, *res = NULL;

	if (kind) {
		read_lock(&cls_mod_lock);
		list_for_each_entry(t, &tcf_proto_base, head) {
			if (nla_strcmp(kind, t->kind) == 0) {
				if (try_module_get(t->owner))
					res = t;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				break;
			}
		}
		read_unlock(&cls_mod_lock);
	}
<<<<<<< HEAD
	return t;
=======
	return res;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Register(unregister) new classifier type */

int register_tcf_proto_ops(struct tcf_proto_ops *ops)
{
<<<<<<< HEAD
	struct tcf_proto_ops *t, **tp;
	int rc = -EEXIST;

	write_lock(&cls_mod_lock);
	for (tp = &tcf_proto_base; (t = *tp) != NULL; tp = &t->next)
		if (!strcmp(ops->kind, t->kind))
			goto out;

	ops->next = NULL;
	*tp = ops;
=======
	struct tcf_proto_ops *t;
	int rc = -EEXIST;

	write_lock(&cls_mod_lock);
	list_for_each_entry(t, &tcf_proto_base, head)
		if (!strcmp(ops->kind, t->kind))
			goto out;

	list_add_tail(&ops->head, &tcf_proto_base);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rc = 0;
out:
	write_unlock(&cls_mod_lock);
	return rc;
}
EXPORT_SYMBOL(register_tcf_proto_ops);

int unregister_tcf_proto_ops(struct tcf_proto_ops *ops)
{
<<<<<<< HEAD
	struct tcf_proto_ops *t, **tp;
	int rc = -ENOENT;

	write_lock(&cls_mod_lock);
	for (tp = &tcf_proto_base; (t = *tp) != NULL; tp = &t->next)
		if (t == ops)
			break;

	if (!t)
		goto out;
	*tp = t->next;
	rc = 0;
out:
=======
	struct tcf_proto_ops *t;
	int rc = -ENOENT;

	/* Wait for outstanding call_rcu()s, if any, from a
	 * tcf_proto_ops's destroy() handler.
	 */
	rcu_barrier();

	write_lock(&cls_mod_lock);
	list_for_each_entry(t, &tcf_proto_base, head) {
		if (t == ops) {
			list_del(&t->head);
			rc = 0;
			break;
		}
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	write_unlock(&cls_mod_lock);
	return rc;
}
EXPORT_SYMBOL(unregister_tcf_proto_ops);

static int tfilter_notify(struct net *net, struct sk_buff *oskb,
			  struct nlmsghdr *n, struct tcf_proto *tp,
<<<<<<< HEAD
			  unsigned long fh, int event);

=======
			  unsigned long fh, int event, bool unicast);

static void tfilter_notify_chain(struct net *net, struct sk_buff *oskb,
				 struct nlmsghdr *n,
				 struct tcf_proto __rcu **chain, int event)
{
	struct tcf_proto __rcu **it_chain;
	struct tcf_proto *tp;

	for (it_chain = chain; (tp = rtnl_dereference(*it_chain)) != NULL;
	     it_chain = &tp->next)
		tfilter_notify(net, oskb, n, tp, 0, event, false);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Select new prio value from the range, managed by kernel. */

static inline u32 tcf_auto_prio(struct tcf_proto *tp)
{
	u32 first = TC_H_MAKE(0xC0000000U, 0U);

	if (tp)
		first = tp->prio - 1;

	return first;
}

/* Add/change/delete/get a filter node */

static int tc_ctl_tfilter(struct sk_buff *skb, struct nlmsghdr *n)
{
	struct net *net = sock_net(skb->sk);
	struct nlattr *tca[TCA_MAX + 1];
<<<<<<< HEAD
	spinlock_t *root_lock;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct tcmsg *t;
	u32 protocol;
	u32 prio;
	u32 nprio;
	u32 parent;
	struct net_device *dev;
	struct Qdisc  *q;
<<<<<<< HEAD
	struct tcf_proto **back, **chain;
=======
	struct tcf_proto __rcu **back;
	struct tcf_proto __rcu **chain;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct tcf_proto *tp;
	const struct tcf_proto_ops *tp_ops;
	const struct Qdisc_class_ops *cops;
	unsigned long cl;
	unsigned long fh;
	int err;
	int tp_created;

<<<<<<< HEAD
	if ((n->nlmsg_type != RTM_GETTFILTER) && !netlink_capable(skb, CAP_NET_ADMIN))
=======
	if ((n->nlmsg_type != RTM_GETTFILTER) &&
	    !netlink_ns_capable(skb, net->user_ns, CAP_NET_ADMIN))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EPERM;

replay:
	tp_created = 0;

	err = nlmsg_parse(n, sizeof(*t), tca, TCA_MAX, NULL);
	if (err < 0)
		return err;

	t = nlmsg_data(n);
	protocol = TC_H_MIN(t->tcm_info);
	prio = TC_H_MAJ(t->tcm_info);
	nprio = prio;
	parent = t->tcm_parent;
	cl = 0;

	if (prio == 0) {
<<<<<<< HEAD
		/* If no priority is given, user wants we allocated it. */
		if (n->nlmsg_type != RTM_NEWTFILTER ||
		    !(n->nlmsg_flags & NLM_F_CREATE))
			return -ENOENT;
		prio = TC_H_MAKE(0x80000000U, 0U);
=======
		switch (n->nlmsg_type) {
		case RTM_DELTFILTER:
			if (protocol || t->tcm_handle || tca[TCA_KIND])
				return -ENOENT;
			break;
		case RTM_NEWTFILTER:
			/* If no priority is provided by the user,
			 * we allocate one.
			 */
			if (n->nlmsg_flags & NLM_F_CREATE) {
				prio = TC_H_MAKE(0x80000000U, 0U);
				break;
			}
			/* fall-through */
		default:
			return -ENOENT;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Find head of filter chain. */

	/* Find link */
	dev = __dev_get_by_index(net, t->tcm_ifindex);
	if (dev == NULL)
		return -ENODEV;

	/* Find qdisc */
	if (!parent) {
		q = dev->qdisc;
		parent = q->handle;
	} else {
		q = qdisc_lookup(dev, TC_H_MAJ(t->tcm_parent));
		if (q == NULL)
			return -EINVAL;
	}

	/* Is it classful? */
	cops = q->ops->cl_ops;
	if (!cops)
		return -EINVAL;

	if (cops->tcf_chain == NULL)
		return -EOPNOTSUPP;

	/* Do we search for filter, attached to class? */
	if (TC_H_MIN(parent)) {
		cl = cops->get(q, parent);
		if (cl == 0)
			return -ENOENT;
	}

	/* And the last stroke */
	chain = cops->tcf_chain(q, cl);
	err = -EINVAL;
	if (chain == NULL)
		goto errout;
<<<<<<< HEAD

	/* Check the chain for existence of proto-tcf with this priority */
	for (back = chain; (tp = *back) != NULL; back = &tp->next) {
=======
	if (n->nlmsg_type == RTM_DELTFILTER && prio == 0) {
		tfilter_notify_chain(net, skb, n, chain, RTM_DELTFILTER);
		tcf_destroy_chain(chain);
		err = 0;
		goto errout;
	}

	/* Check the chain for existence of proto-tcf with this priority */
	for (back = chain;
	     (tp = rtnl_dereference(*back)) != NULL;
	     back = &tp->next) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (tp->prio >= prio) {
			if (tp->prio == prio) {
				if (!nprio ||
				    (tp->protocol != protocol && protocol))
					goto errout;
			} else
				tp = NULL;
			break;
		}
	}

<<<<<<< HEAD
	root_lock = qdisc_root_sleeping_lock(q);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (tp == NULL) {
		/* Proto-tcf does not exist, create new one */

		if (tca[TCA_KIND] == NULL || !protocol)
			goto errout;

		err = -ENOENT;
		if (n->nlmsg_type != RTM_NEWTFILTER ||
		    !(n->nlmsg_flags & NLM_F_CREATE))
			goto errout;


		/* Create new proto tcf */

		err = -ENOBUFS;
		tp = kzalloc(sizeof(*tp), GFP_KERNEL);
		if (tp == NULL)
			goto errout;
		err = -ENOENT;
		tp_ops = tcf_proto_lookup_ops(tca[TCA_KIND]);
		if (tp_ops == NULL) {
#ifdef CONFIG_MODULES
			struct nlattr *kind = tca[TCA_KIND];
			char name[IFNAMSIZ];

			if (kind != NULL &&
			    nla_strlcpy(name, kind, IFNAMSIZ) < IFNAMSIZ) {
				rtnl_unlock();
				request_module("cls_%s", name);
				rtnl_lock();
				tp_ops = tcf_proto_lookup_ops(kind);
				/* We dropped the RTNL semaphore in order to
				 * perform the module load.  So, even if we
				 * succeeded in loading the module we have to
				 * replay the request.  We indicate this using
				 * -EAGAIN.
				 */
				if (tp_ops != NULL) {
					module_put(tp_ops->owner);
					err = -EAGAIN;
				}
			}
#endif
			kfree(tp);
			goto errout;
		}
		tp->ops = tp_ops;
		tp->protocol = protocol;
<<<<<<< HEAD
		tp->prio = nprio ? : TC_H_MAJ(tcf_auto_prio(*back));
=======
		tp->prio = nprio ? :
			       TC_H_MAJ(tcf_auto_prio(rtnl_dereference(*back)));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		tp->q = q;
		tp->classify = tp_ops->classify;
		tp->classid = parent;

		err = tp_ops->init(tp);
		if (err != 0) {
			module_put(tp_ops->owner);
			kfree(tp);
			goto errout;
		}

		tp_created = 1;

	} else if (tca[TCA_KIND] && nla_strcmp(tca[TCA_KIND], tp->ops->kind))
		goto errout;

	fh = tp->ops->get(tp, t->tcm_handle);

	if (fh == 0) {
		if (n->nlmsg_type == RTM_DELTFILTER && t->tcm_handle == 0) {
<<<<<<< HEAD
			spin_lock_bh(root_lock);
			*back = tp->next;
			spin_unlock_bh(root_lock);

			tfilter_notify(net, skb, n, tp, fh, RTM_DELTFILTER);
			tcf_destroy(tp);
=======
			struct tcf_proto *next = rtnl_dereference(tp->next);

			RCU_INIT_POINTER(*back, next);

			tfilter_notify(net, skb, n, tp, fh,
				       RTM_DELTFILTER, false);
			tcf_destroy(tp, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			err = 0;
			goto errout;
		}

		err = -ENOENT;
		if (n->nlmsg_type != RTM_NEWTFILTER ||
		    !(n->nlmsg_flags & NLM_F_CREATE))
			goto errout;
	} else {
		switch (n->nlmsg_type) {
		case RTM_NEWTFILTER:
			err = -EEXIST;
			if (n->nlmsg_flags & NLM_F_EXCL) {
				if (tp_created)
<<<<<<< HEAD
					tcf_destroy(tp);
=======
					tcf_destroy(tp, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				goto errout;
			}
			break;
		case RTM_DELTFILTER:
			err = tp->ops->delete(tp, fh);
<<<<<<< HEAD
			if (err == 0)
				tfilter_notify(net, skb, n, tp, fh, RTM_DELTFILTER);
			goto errout;
		case RTM_GETTFILTER:
			err = tfilter_notify(net, skb, n, tp, fh, RTM_NEWTFILTER);
=======
			if (err == 0) {
				struct tcf_proto *next = rtnl_dereference(tp->next);

				tfilter_notify(net, skb, n, tp,
					       t->tcm_handle,
					       RTM_DELTFILTER, false);
				if (tcf_destroy(tp, false))
					RCU_INIT_POINTER(*back, next);
			}
			goto errout;
		case RTM_GETTFILTER:
			err = tfilter_notify(net, skb, n, tp, fh,
					     RTM_NEWTFILTER, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto errout;
		default:
			err = -EINVAL;
			goto errout;
		}
	}

<<<<<<< HEAD
	err = tp->ops->change(net, skb, tp, cl, t->tcm_handle, tca, &fh);
	if (err == 0) {
		if (tp_created) {
			spin_lock_bh(root_lock);
			tp->next = *back;
			*back = tp;
			spin_unlock_bh(root_lock);
		}
		tfilter_notify(net, skb, n, tp, fh, RTM_NEWTFILTER);
	} else {
		if (tp_created)
			tcf_destroy(tp);
=======
	err = tp->ops->change(net, skb, tp, cl, t->tcm_handle, tca, &fh,
			      n->nlmsg_flags & NLM_F_CREATE ? TCA_ACT_NOREPLACE : TCA_ACT_REPLACE);
	if (err == 0) {
		if (tp_created) {
			RCU_INIT_POINTER(tp->next, rtnl_dereference(*back));
			rcu_assign_pointer(*back, tp);
		}
		tfilter_notify(net, skb, n, tp, fh, RTM_NEWTFILTER, false);
	} else {
		if (tp_created)
			tcf_destroy(tp, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

errout:
	if (cl)
		cops->put(q, cl);
	if (err == -EAGAIN)
		/* Replay the request. */
		goto replay;
	return err;
}

<<<<<<< HEAD
static int tcf_fill_node(struct sk_buff *skb, struct tcf_proto *tp,
			 unsigned long fh, u32 portid, u32 seq, u16 flags, int event)
=======
static int tcf_fill_node(struct net *net, struct sk_buff *skb,
			 struct tcf_proto *tp, unsigned long fh, u32 portid,
			 u32 seq, u16 flags, int event)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct tcmsg *tcm;
	struct nlmsghdr  *nlh;
	unsigned char *b = skb_tail_pointer(skb);

	nlh = nlmsg_put(skb, portid, seq, event, sizeof(*tcm), flags);
	if (!nlh)
		goto out_nlmsg_trim;
	tcm = nlmsg_data(nlh);
	tcm->tcm_family = AF_UNSPEC;
	tcm->tcm__pad1 = 0;
	tcm->tcm__pad2 = 0;
	tcm->tcm_ifindex = qdisc_dev(tp->q)->ifindex;
	tcm->tcm_parent = tp->classid;
	tcm->tcm_info = TC_H_MAKE(tp->prio, tp->protocol);
	if (nla_put_string(skb, TCA_KIND, tp->ops->kind))
		goto nla_put_failure;
	tcm->tcm_handle = fh;
	if (RTM_DELTFILTER != event) {
		tcm->tcm_handle = 0;
<<<<<<< HEAD
		if (tp->ops->dump && tp->ops->dump(tp, fh, skb, tcm) < 0)
=======
		if (tp->ops->dump && tp->ops->dump(net, tp, fh, skb, tcm) < 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto nla_put_failure;
	}
	nlh->nlmsg_len = skb_tail_pointer(skb) - b;
	return skb->len;

out_nlmsg_trim:
nla_put_failure:
	nlmsg_trim(skb, b);
	return -1;
}

static int tfilter_notify(struct net *net, struct sk_buff *oskb,
			  struct nlmsghdr *n, struct tcf_proto *tp,
<<<<<<< HEAD
			  unsigned long fh, int event)
=======
			  unsigned long fh, int event, bool unicast)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct sk_buff *skb;
	u32 portid = oskb ? NETLINK_CB(oskb).portid : 0;

	skb = alloc_skb(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb)
		return -ENOBUFS;

<<<<<<< HEAD
	if (tcf_fill_node(skb, tp, fh, portid, n->nlmsg_seq, 0, event) <= 0) {
=======
	if (tcf_fill_node(net, skb, tp, fh, portid, n->nlmsg_seq,
			  n->nlmsg_flags, event) <= 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		kfree_skb(skb);
		return -EINVAL;
	}

<<<<<<< HEAD
=======
	if (unicast)
		return netlink_unicast(net->rtnl, skb, portid, MSG_DONTWAIT);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return rtnetlink_send(skb, net, portid, RTNLGRP_TC,
			      n->nlmsg_flags & NLM_F_ECHO);
}

struct tcf_dump_args {
	struct tcf_walker w;
	struct sk_buff *skb;
	struct netlink_callback *cb;
};

static int tcf_node_dump(struct tcf_proto *tp, unsigned long n,
			 struct tcf_walker *arg)
{
	struct tcf_dump_args *a = (void *)arg;
<<<<<<< HEAD

	return tcf_fill_node(a->skb, tp, n, NETLINK_CB(a->cb->skb).portid,
			     a->cb->nlh->nlmsg_seq, NLM_F_MULTI, RTM_NEWTFILTER);
=======
	struct net *net = sock_net(a->skb->sk);

	return tcf_fill_node(net, a->skb, tp, n, NETLINK_CB(a->cb->skb).portid,
			     a->cb->nlh->nlmsg_seq, NLM_F_MULTI,
			     RTM_NEWTFILTER);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* called with RTNL */
static int tc_dump_tfilter(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct net *net = sock_net(skb->sk);
	int t;
	int s_t;
	struct net_device *dev;
	struct Qdisc *q;
<<<<<<< HEAD
	struct tcf_proto *tp, **chain;
=======
	struct tcf_proto *tp, __rcu **chain;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct tcmsg *tcm = nlmsg_data(cb->nlh);
	unsigned long cl = 0;
	const struct Qdisc_class_ops *cops;
	struct tcf_dump_args arg;

	if (nlmsg_len(cb->nlh) < sizeof(*tcm))
		return skb->len;
	dev = __dev_get_by_index(net, tcm->tcm_ifindex);
	if (!dev)
		return skb->len;

	if (!tcm->tcm_parent)
		q = dev->qdisc;
	else
		q = qdisc_lookup(dev, TC_H_MAJ(tcm->tcm_parent));
	if (!q)
		goto out;
	cops = q->ops->cl_ops;
	if (!cops)
		goto errout;
	if (cops->tcf_chain == NULL)
		goto errout;
	if (TC_H_MIN(tcm->tcm_parent)) {
		cl = cops->get(q, tcm->tcm_parent);
		if (cl == 0)
			goto errout;
	}
	chain = cops->tcf_chain(q, cl);
	if (chain == NULL)
		goto errout;

	s_t = cb->args[0];

<<<<<<< HEAD
	for (tp = *chain, t = 0; tp; tp = tp->next, t++) {
=======
	for (tp = rtnl_dereference(*chain), t = 0;
	     tp; tp = rtnl_dereference(tp->next), t++) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (t < s_t)
			continue;
		if (TC_H_MAJ(tcm->tcm_info) &&
		    TC_H_MAJ(tcm->tcm_info) != tp->prio)
			continue;
		if (TC_H_MIN(tcm->tcm_info) &&
		    TC_H_MIN(tcm->tcm_info) != tp->protocol)
			continue;
		if (t > s_t)
<<<<<<< HEAD
			memset(&cb->args[1], 0, sizeof(cb->args)-sizeof(cb->args[0]));
		if (cb->args[1] == 0) {
			if (tcf_fill_node(skb, tp, 0, NETLINK_CB(cb->skb).portid,
=======
			memset(&cb->args[1], 0,
			       sizeof(cb->args)-sizeof(cb->args[0]));
		if (cb->args[1] == 0) {
			if (tcf_fill_node(net, skb, tp, 0,
					  NETLINK_CB(cb->skb).portid,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					  cb->nlh->nlmsg_seq, NLM_F_MULTI,
					  RTM_NEWTFILTER) <= 0)
				break;

			cb->args[1] = 1;
		}
		if (tp->ops->walk == NULL)
			continue;
		arg.w.fn = tcf_node_dump;
		arg.skb = skb;
		arg.cb = cb;
		arg.w.stop = 0;
		arg.w.skip = cb->args[1] - 1;
		arg.w.count = 0;
		tp->ops->walk(tp, &arg.w);
		cb->args[1] = arg.w.count + 1;
		if (arg.w.stop)
			break;
	}

	cb->args[0] = t;

errout:
	if (cl)
		cops->put(q, cl);
out:
	return skb->len;
}

<<<<<<< HEAD
void tcf_exts_destroy(struct tcf_proto *tp, struct tcf_exts *exts)
{
#ifdef CONFIG_NET_CLS_ACT
	if (exts->action) {
		tcf_action_destroy(exts->action, TCA_ACT_UNBIND);
		exts->action = NULL;
	}
=======
void tcf_exts_destroy(struct tcf_exts *exts)
{
#ifdef CONFIG_NET_CLS_ACT
	LIST_HEAD(actions);

	tcf_exts_to_list(exts, &actions);
	tcf_action_destroy(&actions, TCA_ACT_UNBIND);
	kfree(exts->actions);
	exts->nr_actions = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
}
EXPORT_SYMBOL(tcf_exts_destroy);

int tcf_exts_validate(struct net *net, struct tcf_proto *tp, struct nlattr **tb,
<<<<<<< HEAD
		  struct nlattr *rate_tlv, struct tcf_exts *exts,
		  const struct tcf_ext_map *map)
{
	memset(exts, 0, sizeof(*exts));

=======
		      struct nlattr *rate_tlv, struct tcf_exts *exts, bool ovr)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_NET_CLS_ACT
	{
		struct tc_action *act;

<<<<<<< HEAD
		if (map->police && tb[map->police]) {
			act = tcf_action_init_1(net, tb[map->police], rate_tlv,
						"police", TCA_ACT_NOREPLACE,
						TCA_ACT_BIND);
			if (IS_ERR(act))
				return PTR_ERR(act);

			act->type = TCA_OLD_COMPAT;
			exts->action = act;
		} else if (map->action && tb[map->action]) {
			act = tcf_action_init(net, tb[map->action], rate_tlv,
					      NULL, TCA_ACT_NOREPLACE,
					      TCA_ACT_BIND);
			if (IS_ERR(act))
				return PTR_ERR(act);

			exts->action = act;
		}
	}
#else
	if ((map->action && tb[map->action]) ||
	    (map->police && tb[map->police]))
=======
		if (exts->police && tb[exts->police]) {
			act = tcf_action_init_1(net, tb[exts->police], rate_tlv,
						"police", ovr, TCA_ACT_BIND);
			if (IS_ERR(act))
				return PTR_ERR(act);

			act->type = exts->type = TCA_OLD_COMPAT;
			exts->actions[0] = act;
			exts->nr_actions = 1;
		} else if (exts->action && tb[exts->action]) {
			LIST_HEAD(actions);
			int err, i = 0;

			err = tcf_action_init(net, tb[exts->action], rate_tlv,
					      NULL, ovr, TCA_ACT_BIND,
					      &actions);
			if (err)
				return err;
			list_for_each_entry(act, &actions, list)
				exts->actions[i++] = act;
			exts->nr_actions = i;
		}
	}
#else
	if ((exts->action && tb[exts->action]) ||
	    (exts->police && tb[exts->police]))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EOPNOTSUPP;
#endif

	return 0;
}
EXPORT_SYMBOL(tcf_exts_validate);

void tcf_exts_change(struct tcf_proto *tp, struct tcf_exts *dst,
		     struct tcf_exts *src)
{
#ifdef CONFIG_NET_CLS_ACT
<<<<<<< HEAD
	if (src->action) {
		struct tc_action *act;
		tcf_tree_lock(tp);
		act = dst->action;
		dst->action = src->action;
		tcf_tree_unlock(tp);
		if (act)
			tcf_action_destroy(act, TCA_ACT_UNBIND);
	}
=======
	struct tcf_exts old = *dst;

	tcf_tree_lock(tp);
	dst->nr_actions = src->nr_actions;
	dst->actions = src->actions;
	dst->type = src->type;
	tcf_tree_unlock(tp);

	tcf_exts_destroy(&old);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
}
EXPORT_SYMBOL(tcf_exts_change);

<<<<<<< HEAD
int tcf_exts_dump(struct sk_buff *skb, struct tcf_exts *exts,
		  const struct tcf_ext_map *map)
{
#ifdef CONFIG_NET_CLS_ACT
	if (map->action && exts->action) {
=======
#ifdef CONFIG_NET_CLS_ACT
static struct tc_action *tcf_exts_first_act(struct tcf_exts *exts)
{
	if (exts->nr_actions == 0)
		return NULL;
	else
		return exts->actions[0];
}
#endif

int tcf_exts_dump(struct sk_buff *skb, struct tcf_exts *exts)
{
#ifdef CONFIG_NET_CLS_ACT
	struct nlattr *nest;

	if (exts->action && exts->nr_actions) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * again for backward compatible mode - we want
		 * to work with both old and new modes of entering
		 * tc data even if iproute2  was newer - jhs
		 */
<<<<<<< HEAD
		struct nlattr *nest;

		if (exts->action->type != TCA_OLD_COMPAT) {
			nest = nla_nest_start(skb, map->action);
			if (nest == NULL)
				goto nla_put_failure;
			if (tcf_action_dump(skb, exts->action, 0, 0) < 0)
				goto nla_put_failure;
			nla_nest_end(skb, nest);
		} else if (map->police) {
			nest = nla_nest_start(skb, map->police);
			if (nest == NULL)
				goto nla_put_failure;
			if (tcf_action_dump_old(skb, exts->action, 0, 0) < 0)
=======
		if (exts->type != TCA_OLD_COMPAT) {
			LIST_HEAD(actions);

			nest = nla_nest_start(skb, exts->action);
			if (nest == NULL)
				goto nla_put_failure;

			tcf_exts_to_list(exts, &actions);
			if (tcf_action_dump(skb, &actions, 0, 0) < 0)
				goto nla_put_failure;
			nla_nest_end(skb, nest);
		} else if (exts->police) {
			struct tc_action *act = tcf_exts_first_act(exts);
			nest = nla_nest_start(skb, exts->police);
			if (nest == NULL || !act)
				goto nla_put_failure;
			if (tcf_action_dump_old(skb, act, 0, 0) < 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				goto nla_put_failure;
			nla_nest_end(skb, nest);
		}
	}
<<<<<<< HEAD
#endif
	return 0;
nla_put_failure: __attribute__ ((unused))
	return -1;
=======
	return 0;

nla_put_failure:
	nla_nest_cancel(skb, nest);
	return -1;
#else
	return 0;
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL(tcf_exts_dump);


<<<<<<< HEAD
int tcf_exts_dump_stats(struct sk_buff *skb, struct tcf_exts *exts,
			const struct tcf_ext_map *map)
{
#ifdef CONFIG_NET_CLS_ACT
	if (exts->action)
		if (tcf_action_copy_stats(skb, exts->action, 1) < 0)
			goto nla_put_failure;
#endif
	return 0;
nla_put_failure: __attribute__ ((unused))
	return -1;
=======
int tcf_exts_dump_stats(struct sk_buff *skb, struct tcf_exts *exts)
{
#ifdef CONFIG_NET_CLS_ACT
	struct tc_action *a = tcf_exts_first_act(exts);
	if (a != NULL && tcf_action_copy_stats(skb, a, 1) < 0)
		return -1;
#endif
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL(tcf_exts_dump_stats);

static int __init tc_filter_init(void)
{
	rtnl_register(PF_UNSPEC, RTM_NEWTFILTER, tc_ctl_tfilter, NULL, NULL);
	rtnl_register(PF_UNSPEC, RTM_DELTFILTER, tc_ctl_tfilter, NULL, NULL);
	rtnl_register(PF_UNSPEC, RTM_GETTFILTER, tc_ctl_tfilter,
		      tc_dump_tfilter, NULL);

	return 0;
}

subsys_initcall(tc_filter_init);
