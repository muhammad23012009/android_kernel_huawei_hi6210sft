/*
 * net/sched/sch_prio.c	Simple 3-band priority "scheduler".
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 * Fixes:       19990609: J Hadi Salim <hadi@nortelnetworks.com>:
 *              Init --  EINVAL when opt undefined
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>


struct prio_sched_data {
	int bands;
<<<<<<< HEAD
	struct tcf_proto *filter_list;
=======
	struct tcf_proto __rcu *filter_list;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u8  prio2band[TC_PRIO_MAX+1];
	struct Qdisc *queues[TCQ_PRIO_BANDS];
};


static struct Qdisc *
prio_classify(struct sk_buff *skb, struct Qdisc *sch, int *qerr)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	u32 band = skb->priority;
	struct tcf_result res;
<<<<<<< HEAD
=======
	struct tcf_proto *fl;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int err;

	*qerr = NET_XMIT_SUCCESS | __NET_XMIT_BYPASS;
	if (TC_H_MAJ(skb->priority) != sch->handle) {
<<<<<<< HEAD
		err = tc_classify(skb, q->filter_list, &res);
=======
		fl = rcu_dereference_bh(q->filter_list);
		err = tc_classify(skb, fl, &res, false);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_NET_CLS_ACT
		switch (err) {
		case TC_ACT_STOLEN:
		case TC_ACT_QUEUED:
			*qerr = NET_XMIT_SUCCESS | __NET_XMIT_STOLEN;
		case TC_ACT_SHOT:
			return NULL;
		}
#endif
<<<<<<< HEAD
		if (!q->filter_list || err < 0) {
=======
		if (!fl || err < 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (TC_H_MAJ(band))
				band = 0;
			return q->queues[q->prio2band[band & TC_PRIO_MAX]];
		}
		band = res.classid;
	}
	band = TC_H_MIN(band) - 1;
	if (band >= q->bands)
		return q->queues[q->prio2band[0]];

	return q->queues[band];
}

static int
<<<<<<< HEAD
prio_enqueue(struct sk_buff *skb, struct Qdisc *sch)
=======
prio_enqueue(struct sk_buff *skb, struct Qdisc *sch, struct sk_buff **to_free)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct Qdisc *qdisc;
	int ret;

	qdisc = prio_classify(skb, sch, &ret);
#ifdef CONFIG_NET_CLS_ACT
	if (qdisc == NULL) {

		if (ret & __NET_XMIT_BYPASS)
<<<<<<< HEAD
			sch->qstats.drops++;
=======
			qdisc_qstats_drop(sch);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		kfree_skb(skb);
		return ret;
	}
#endif

<<<<<<< HEAD
	ret = qdisc_enqueue(skb, qdisc);
	if (ret == NET_XMIT_SUCCESS) {
=======
	ret = qdisc_enqueue(skb, qdisc, to_free);
	if (ret == NET_XMIT_SUCCESS) {
		qdisc_qstats_backlog_inc(sch, skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		sch->q.qlen++;
		return NET_XMIT_SUCCESS;
	}
	if (net_xmit_drop_count(ret))
<<<<<<< HEAD
		sch->qstats.drops++;
=======
		qdisc_qstats_drop(sch);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static struct sk_buff *prio_peek(struct Qdisc *sch)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	int prio;

	for (prio = 0; prio < q->bands; prio++) {
		struct Qdisc *qdisc = q->queues[prio];
		struct sk_buff *skb = qdisc->ops->peek(qdisc);
		if (skb)
			return skb;
	}
	return NULL;
}

static struct sk_buff *prio_dequeue(struct Qdisc *sch)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	int prio;

	for (prio = 0; prio < q->bands; prio++) {
		struct Qdisc *qdisc = q->queues[prio];
		struct sk_buff *skb = qdisc_dequeue_peeked(qdisc);
		if (skb) {
			qdisc_bstats_update(sch, skb);
<<<<<<< HEAD
=======
			qdisc_qstats_backlog_dec(sch, skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			sch->q.qlen--;
			return skb;
		}
	}
	return NULL;

}

<<<<<<< HEAD
static unsigned int prio_drop(struct Qdisc *sch)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	int prio;
	unsigned int len;
	struct Qdisc *qdisc;

	for (prio = q->bands-1; prio >= 0; prio--) {
		qdisc = q->queues[prio];
		if (qdisc->ops->drop && (len = qdisc->ops->drop(qdisc)) != 0) {
			sch->q.qlen--;
			return len;
		}
	}
	return 0;
}


=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void
prio_reset(struct Qdisc *sch)
{
	int prio;
	struct prio_sched_data *q = qdisc_priv(sch);

	for (prio = 0; prio < q->bands; prio++)
		qdisc_reset(q->queues[prio]);
<<<<<<< HEAD
=======
	sch->qstats.backlog = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sch->q.qlen = 0;
}

static void
prio_destroy(struct Qdisc *sch)
{
	int prio;
	struct prio_sched_data *q = qdisc_priv(sch);

	tcf_destroy_chain(&q->filter_list);
	for (prio = 0; prio < q->bands; prio++)
		qdisc_destroy(q->queues[prio]);
}

static int prio_tune(struct Qdisc *sch, struct nlattr *opt)
{
	struct prio_sched_data *q = qdisc_priv(sch);
<<<<<<< HEAD
	struct tc_prio_qopt *qopt;
	int i;
=======
	struct Qdisc *queues[TCQ_PRIO_BANDS];
	int oldbands = q->bands, i;
	struct tc_prio_qopt *qopt;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (nla_len(opt) < sizeof(*qopt))
		return -EINVAL;
	qopt = nla_data(opt);

	if (qopt->bands > TCQ_PRIO_BANDS || qopt->bands < 2)
		return -EINVAL;

	for (i = 0; i <= TC_PRIO_MAX; i++) {
		if (qopt->priomap[i] >= qopt->bands)
			return -EINVAL;
	}

<<<<<<< HEAD
=======
	/* Before commit, make sure we can allocate all new qdiscs */
	for (i = oldbands; i < qopt->bands; i++) {
		queues[i] = qdisc_create_dflt(sch->dev_queue, &pfifo_qdisc_ops,
					      TC_H_MAKE(sch->handle, i + 1));
		if (!queues[i]) {
			while (i > oldbands)
				qdisc_destroy(queues[--i]);
			return -ENOMEM;
		}
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sch_tree_lock(sch);
	q->bands = qopt->bands;
	memcpy(q->prio2band, qopt->priomap, TC_PRIO_MAX+1);

<<<<<<< HEAD
	for (i = q->bands; i < TCQ_PRIO_BANDS; i++) {
		struct Qdisc *child = q->queues[i];
		q->queues[i] = &noop_qdisc;
		if (child != &noop_qdisc) {
			qdisc_tree_decrease_qlen(child, child->q.qlen);
			qdisc_destroy(child);
		}
	}
	sch_tree_unlock(sch);

	for (i = 0; i < q->bands; i++) {
		if (q->queues[i] == &noop_qdisc) {
			struct Qdisc *child, *old;

			child = qdisc_create_dflt(sch->dev_queue,
						  &pfifo_qdisc_ops,
						  TC_H_MAKE(sch->handle, i + 1));
			if (child) {
				sch_tree_lock(sch);
				old = q->queues[i];
				q->queues[i] = child;

				if (old != &noop_qdisc) {
					qdisc_tree_decrease_qlen(old,
								 old->q.qlen);
					qdisc_destroy(old);
				}
				sch_tree_unlock(sch);
			}
		}
	}
=======
	for (i = q->bands; i < oldbands; i++) {
		struct Qdisc *child = q->queues[i];

		qdisc_tree_reduce_backlog(child, child->q.qlen,
					  child->qstats.backlog);
		qdisc_destroy(child);
	}

	for (i = oldbands; i < q->bands; i++)
		q->queues[i] = queues[i];

	sch_tree_unlock(sch);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static int prio_init(struct Qdisc *sch, struct nlattr *opt)
{
<<<<<<< HEAD
	struct prio_sched_data *q = qdisc_priv(sch);
	int i;

	for (i = 0; i < TCQ_PRIO_BANDS; i++)
		q->queues[i] = &noop_qdisc;

	if (opt == NULL) {
		return -EINVAL;
	} else {
		int err;

		if ((err = prio_tune(sch, opt)) != 0)
			return err;
	}
	return 0;
=======
	if (!opt)
		return -EINVAL;

	return prio_tune(sch, opt);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int prio_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	unsigned char *b = skb_tail_pointer(skb);
	struct tc_prio_qopt opt;

	opt.bands = q->bands;
	memcpy(&opt.priomap, q->prio2band, TC_PRIO_MAX + 1);

	if (nla_put(skb, TCA_OPTIONS, sizeof(opt), &opt))
		goto nla_put_failure;

	return skb->len;

nla_put_failure:
	nlmsg_trim(skb, b);
	return -1;
}

static int prio_graft(struct Qdisc *sch, unsigned long arg, struct Qdisc *new,
		      struct Qdisc **old)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	unsigned long band = arg - 1;

<<<<<<< HEAD
	if (new == NULL)
		new = &noop_qdisc;

	sch_tree_lock(sch);
	*old = q->queues[band];
	q->queues[band] = new;
	qdisc_tree_decrease_qlen(*old, (*old)->q.qlen);
	qdisc_reset(*old);
	sch_tree_unlock(sch);

=======
	if (!new) {
		new = qdisc_create_dflt(sch->dev_queue, &pfifo_qdisc_ops,
					TC_H_MAKE(sch->handle, arg));
		if (!new)
			new = &noop_qdisc;
		else
			qdisc_hash_add(new);
	}

	*old = qdisc_replace(sch, new, &q->queues[band]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static struct Qdisc *
prio_leaf(struct Qdisc *sch, unsigned long arg)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	unsigned long band = arg - 1;

	return q->queues[band];
}

static unsigned long prio_get(struct Qdisc *sch, u32 classid)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	unsigned long band = TC_H_MIN(classid);

	if (band - 1 >= q->bands)
		return 0;
	return band;
}

static unsigned long prio_bind(struct Qdisc *sch, unsigned long parent, u32 classid)
{
	return prio_get(sch, classid);
}


static void prio_put(struct Qdisc *q, unsigned long cl)
{
}

static int prio_dump_class(struct Qdisc *sch, unsigned long cl, struct sk_buff *skb,
			   struct tcmsg *tcm)
{
	struct prio_sched_data *q = qdisc_priv(sch);

	tcm->tcm_handle |= TC_H_MIN(cl);
	tcm->tcm_info = q->queues[cl-1]->handle;
	return 0;
}

static int prio_dump_class_stats(struct Qdisc *sch, unsigned long cl,
				 struct gnet_dump *d)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	struct Qdisc *cl_q;

	cl_q = q->queues[cl - 1];
<<<<<<< HEAD
	cl_q->qstats.qlen = cl_q->q.qlen;
	if (gnet_stats_copy_basic(d, &cl_q->bstats) < 0 ||
	    gnet_stats_copy_queue(d, &cl_q->qstats) < 0)
=======
	if (gnet_stats_copy_basic(qdisc_root_sleeping_running(sch),
				  d, cl_q->cpu_bstats, &cl_q->bstats) < 0 ||
	    gnet_stats_copy_queue(d, NULL, &cl_q->qstats, cl_q->q.qlen) < 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -1;

	return 0;
}

static void prio_walk(struct Qdisc *sch, struct qdisc_walker *arg)
{
	struct prio_sched_data *q = qdisc_priv(sch);
	int prio;

	if (arg->stop)
		return;

	for (prio = 0; prio < q->bands; prio++) {
		if (arg->count < arg->skip) {
			arg->count++;
			continue;
		}
		if (arg->fn(sch, prio + 1, arg) < 0) {
			arg->stop = 1;
			break;
		}
		arg->count++;
	}
}

<<<<<<< HEAD
static struct tcf_proto **prio_find_tcf(struct Qdisc *sch, unsigned long cl)
=======
static struct tcf_proto __rcu **prio_find_tcf(struct Qdisc *sch,
					      unsigned long cl)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct prio_sched_data *q = qdisc_priv(sch);

	if (cl)
		return NULL;
	return &q->filter_list;
}

static const struct Qdisc_class_ops prio_class_ops = {
	.graft		=	prio_graft,
	.leaf		=	prio_leaf,
	.get		=	prio_get,
	.put		=	prio_put,
	.walk		=	prio_walk,
	.tcf_chain	=	prio_find_tcf,
	.bind_tcf	=	prio_bind,
	.unbind_tcf	=	prio_put,
	.dump		=	prio_dump_class,
	.dump_stats	=	prio_dump_class_stats,
};

static struct Qdisc_ops prio_qdisc_ops __read_mostly = {
	.next		=	NULL,
	.cl_ops		=	&prio_class_ops,
	.id		=	"prio",
	.priv_size	=	sizeof(struct prio_sched_data),
	.enqueue	=	prio_enqueue,
	.dequeue	=	prio_dequeue,
	.peek		=	prio_peek,
<<<<<<< HEAD
	.drop		=	prio_drop,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.init		=	prio_init,
	.reset		=	prio_reset,
	.destroy	=	prio_destroy,
	.change		=	prio_tune,
	.dump		=	prio_dump,
	.owner		=	THIS_MODULE,
};

static int __init prio_module_init(void)
{
	return register_qdisc(&prio_qdisc_ops);
}

static void __exit prio_module_exit(void)
{
	unregister_qdisc(&prio_qdisc_ops);
}

module_init(prio_module_init)
module_exit(prio_module_exit)

MODULE_LICENSE("GPL");
