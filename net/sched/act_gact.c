/*
<<<<<<< HEAD
 * net/sched/gact.c	Generic actions
=======
 * net/sched/act_gact.c		Generic actions
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * copyright 	Jamal Hadi Salim (2002-4)
 *
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/rtnetlink.h>
#include <linux/module.h>
#include <linux/init.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>
#include <linux/tc_act/tc_gact.h>
#include <net/tc_act/tc_gact.h>

#define GACT_TAB_MASK	15
<<<<<<< HEAD
static struct tcf_common *tcf_gact_ht[GACT_TAB_MASK + 1];
static u32 gact_idx_gen;
static DEFINE_RWLOCK(gact_lock);

static struct tcf_hashinfo gact_hash_info = {
	.htab	=	tcf_gact_ht,
	.hmask	=	GACT_TAB_MASK,
	.lock	=	&gact_lock,
};
=======

static int gact_net_id;
static struct tc_action_ops act_gact_ops;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_GACT_PROB
static int gact_net_rand(struct tcf_gact *gact)
{
<<<<<<< HEAD
	if (!gact->tcfg_pval || net_random() % gact->tcfg_pval)
=======
	smp_rmb(); /* coupled with smp_wmb() in tcf_gact_init() */
	if (prandom_u32() % gact->tcfg_pval)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return gact->tcf_action;
	return gact->tcfg_paction;
}

static int gact_determ(struct tcf_gact *gact)
{
<<<<<<< HEAD
	if (!gact->tcfg_pval || gact->tcf_bstats.packets % gact->tcfg_pval)
=======
	u32 pack = atomic_inc_return(&gact->packets);

	smp_rmb(); /* coupled with smp_wmb() in tcf_gact_init() */
	if (pack % gact->tcfg_pval)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return gact->tcf_action;
	return gact->tcfg_paction;
}

typedef int (*g_rand)(struct tcf_gact *gact);
static g_rand gact_rand[MAX_RAND] = { NULL, gact_net_rand, gact_determ };
#endif /* CONFIG_GACT_PROB */

static const struct nla_policy gact_policy[TCA_GACT_MAX + 1] = {
	[TCA_GACT_PARMS]	= { .len = sizeof(struct tc_gact) },
	[TCA_GACT_PROB]		= { .len = sizeof(struct tc_gact_p) },
};

static int tcf_gact_init(struct net *net, struct nlattr *nla,
<<<<<<< HEAD
			 struct nlattr *est, struct tc_action *a,
			 int ovr, int bind)
{
	struct nlattr *tb[TCA_GACT_MAX + 1];
	struct tc_gact *parm;
	struct tcf_gact *gact;
	struct tcf_common *pc;
=======
			 struct nlattr *est, struct tc_action **a,
			 int ovr, int bind)
{
	struct tc_action_net *tn = net_generic(net, gact_net_id);
	struct nlattr *tb[TCA_GACT_MAX + 1];
	struct tc_gact *parm;
	struct tcf_gact *gact;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret = 0;
	int err;
#ifdef CONFIG_GACT_PROB
	struct tc_gact_p *p_parm = NULL;
#endif

	if (nla == NULL)
		return -EINVAL;

	err = nla_parse_nested(tb, TCA_GACT_MAX, nla, gact_policy);
	if (err < 0)
		return err;

	if (tb[TCA_GACT_PARMS] == NULL)
		return -EINVAL;
	parm = nla_data(tb[TCA_GACT_PARMS]);

#ifndef CONFIG_GACT_PROB
	if (tb[TCA_GACT_PROB] != NULL)
		return -EOPNOTSUPP;
#else
	if (tb[TCA_GACT_PROB]) {
		p_parm = nla_data(tb[TCA_GACT_PROB]);
		if (p_parm->ptype >= MAX_RAND)
			return -EINVAL;
	}
#endif

<<<<<<< HEAD
	pc = tcf_hash_check(parm->index, a, bind, &gact_hash_info);
	if (!pc) {
		pc = tcf_hash_create(parm->index, est, a, sizeof(*gact),
				     bind, &gact_idx_gen, &gact_hash_info);
		if (IS_ERR(pc))
			return PTR_ERR(pc);
		ret = ACT_P_CREATED;
	} else {
		if (!ovr) {
			tcf_hash_release(pc, bind, &gact_hash_info);
			return -EEXIST;
		}
	}

	gact = to_gact(pc);

	spin_lock_bh(&gact->tcf_lock);
=======
	if (!tcf_hash_check(tn, parm->index, a, bind)) {
		ret = tcf_hash_create(tn, parm->index, est, a,
				      &act_gact_ops, bind, true);
		if (ret)
			return ret;
		ret = ACT_P_CREATED;
	} else {
		if (bind)/* dont override defaults */
			return 0;
		tcf_hash_release(*a, bind);
		if (!ovr)
			return -EEXIST;
	}

	gact = to_gact(*a);

	ASSERT_RTNL();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	gact->tcf_action = parm->action;
#ifdef CONFIG_GACT_PROB
	if (p_parm) {
		gact->tcfg_paction = p_parm->paction;
<<<<<<< HEAD
		gact->tcfg_pval    = p_parm->pval;
		gact->tcfg_ptype   = p_parm->ptype;
	}
#endif
	spin_unlock_bh(&gact->tcf_lock);
	if (ret == ACT_P_CREATED)
		tcf_hash_insert(pc, &gact_hash_info);
	return ret;
}

static int tcf_gact_cleanup(struct tc_action *a, int bind)
{
	struct tcf_gact *gact = a->priv;

	if (gact)
		return tcf_hash_release(&gact->common, bind, &gact_hash_info);
	return 0;
}

static int tcf_gact(struct sk_buff *skb, const struct tc_action *a,
		    struct tcf_result *res)
{
	struct tcf_gact *gact = a->priv;
	int action = TC_ACT_SHOT;

	spin_lock(&gact->tcf_lock);
#ifdef CONFIG_GACT_PROB
	if (gact->tcfg_ptype)
		action = gact_rand[gact->tcfg_ptype](gact);
	else
		action = gact->tcf_action;
#else
	action = gact->tcf_action;
#endif
	gact->tcf_bstats.bytes += qdisc_pkt_len(skb);
	gact->tcf_bstats.packets++;
	if (action == TC_ACT_SHOT)
		gact->tcf_qstats.drops++;
	gact->tcf_tm.lastuse = jiffies;
	spin_unlock(&gact->tcf_lock);
=======
		gact->tcfg_pval    = max_t(u16, 1, p_parm->pval);
		/* Make sure tcfg_pval is written before tcfg_ptype
		 * coupled with smp_rmb() in gact_net_rand() & gact_determ()
		 */
		smp_wmb();
		gact->tcfg_ptype   = p_parm->ptype;
	}
#endif
	if (ret == ACT_P_CREATED)
		tcf_hash_insert(tn, *a);
	return ret;
}

static int tcf_gact(struct sk_buff *skb, const struct tc_action *a,
		    struct tcf_result *res)
{
	struct tcf_gact *gact = to_gact(a);
	int action = READ_ONCE(gact->tcf_action);

#ifdef CONFIG_GACT_PROB
	{
	u32 ptype = READ_ONCE(gact->tcfg_ptype);

	if (ptype)
		action = gact_rand[ptype](gact);
	}
#endif
	bstats_cpu_update(this_cpu_ptr(gact->common.cpu_bstats), skb);
	if (action == TC_ACT_SHOT)
		qstats_drop_inc(this_cpu_ptr(gact->common.cpu_qstats));

	tcf_lastuse_update(&gact->tcf_tm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return action;
}

<<<<<<< HEAD
static int tcf_gact_dump(struct sk_buff *skb, struct tc_action *a, int bind, int ref)
{
	unsigned char *b = skb_tail_pointer(skb);
	struct tcf_gact *gact = a->priv;
=======
static void tcf_gact_stats_update(struct tc_action *a, u64 bytes, u32 packets,
				  u64 lastuse)
{
	struct tcf_gact *gact = to_gact(a);
	int action = READ_ONCE(gact->tcf_action);
	struct tcf_t *tm = &gact->tcf_tm;

	_bstats_cpu_update(this_cpu_ptr(gact->common.cpu_bstats), bytes,
			   packets);
	if (action == TC_ACT_SHOT)
		this_cpu_ptr(gact->common.cpu_qstats)->drops += packets;

	tm->lastuse = max_t(u64, tm->lastuse, lastuse);
}

static int tcf_gact_dump(struct sk_buff *skb, struct tc_action *a,
			 int bind, int ref)
{
	unsigned char *b = skb_tail_pointer(skb);
	struct tcf_gact *gact = to_gact(a);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct tc_gact opt = {
		.index   = gact->tcf_index,
		.refcnt  = gact->tcf_refcnt - ref,
		.bindcnt = gact->tcf_bindcnt - bind,
		.action  = gact->tcf_action,
	};
	struct tcf_t t;

	if (nla_put(skb, TCA_GACT_PARMS, sizeof(opt), &opt))
		goto nla_put_failure;
#ifdef CONFIG_GACT_PROB
	if (gact->tcfg_ptype) {
		struct tc_gact_p p_opt = {
			.paction = gact->tcfg_paction,
			.pval    = gact->tcfg_pval,
			.ptype   = gact->tcfg_ptype,
		};

		if (nla_put(skb, TCA_GACT_PROB, sizeof(p_opt), &p_opt))
			goto nla_put_failure;
	}
#endif
<<<<<<< HEAD
	t.install = jiffies_to_clock_t(jiffies - gact->tcf_tm.install);
	t.lastuse = jiffies_to_clock_t(jiffies - gact->tcf_tm.lastuse);
	t.expires = jiffies_to_clock_t(gact->tcf_tm.expires);
	if (nla_put(skb, TCA_GACT_TM, sizeof(t), &t))
=======
	tcf_tm_dump(&t, &gact->tcf_tm);
	if (nla_put_64bit(skb, TCA_GACT_TM, sizeof(t), &t, TCA_GACT_PAD))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto nla_put_failure;
	return skb->len;

nla_put_failure:
	nlmsg_trim(skb, b);
	return -1;
}

<<<<<<< HEAD
static struct tc_action_ops act_gact_ops = {
	.kind		=	"gact",
	.hinfo		=	&gact_hash_info,
	.type		=	TCA_ACT_GACT,
	.capab		=	TCA_CAP_NONE,
	.owner		=	THIS_MODULE,
	.act		=	tcf_gact,
	.dump		=	tcf_gact_dump,
	.cleanup	=	tcf_gact_cleanup,
	.lookup		=	tcf_hash_search,
	.init		=	tcf_gact_init,
	.walk		=	tcf_generic_walker
=======
static int tcf_gact_walker(struct net *net, struct sk_buff *skb,
			   struct netlink_callback *cb, int type,
			   const struct tc_action_ops *ops)
{
	struct tc_action_net *tn = net_generic(net, gact_net_id);

	return tcf_generic_walker(tn, skb, cb, type, ops);
}

static int tcf_gact_search(struct net *net, struct tc_action **a, u32 index)
{
	struct tc_action_net *tn = net_generic(net, gact_net_id);

	return tcf_hash_search(tn, a, index);
}

static struct tc_action_ops act_gact_ops = {
	.kind		=	"gact",
	.type		=	TCA_ACT_GACT,
	.owner		=	THIS_MODULE,
	.act		=	tcf_gact,
	.stats_update	=	tcf_gact_stats_update,
	.dump		=	tcf_gact_dump,
	.init		=	tcf_gact_init,
	.walk		=	tcf_gact_walker,
	.lookup		=	tcf_gact_search,
	.size		=	sizeof(struct tcf_gact),
};

static __net_init int gact_init_net(struct net *net)
{
	struct tc_action_net *tn = net_generic(net, gact_net_id);

	return tc_action_net_init(tn, &act_gact_ops, GACT_TAB_MASK);
}

static void __net_exit gact_exit_net(struct net *net)
{
	struct tc_action_net *tn = net_generic(net, gact_net_id);

	tc_action_net_exit(tn);
}

static struct pernet_operations gact_net_ops = {
	.init = gact_init_net,
	.exit = gact_exit_net,
	.id   = &gact_net_id,
	.size = sizeof(struct tc_action_net),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

MODULE_AUTHOR("Jamal Hadi Salim(2002-4)");
MODULE_DESCRIPTION("Generic Classifier actions");
MODULE_LICENSE("GPL");

static int __init gact_init_module(void)
{
#ifdef CONFIG_GACT_PROB
	pr_info("GACT probability on\n");
#else
	pr_info("GACT probability NOT on\n");
#endif
<<<<<<< HEAD
	return tcf_register_action(&act_gact_ops);
=======

	return tcf_register_action(&act_gact_ops, &gact_net_ops);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void __exit gact_cleanup_module(void)
{
<<<<<<< HEAD
	tcf_unregister_action(&act_gact_ops);
=======
	tcf_unregister_action(&act_gact_ops, &gact_net_ops);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

module_init(gact_init_module);
module_exit(gact_cleanup_module);
