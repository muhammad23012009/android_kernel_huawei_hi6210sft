/*
 * net/sched/sch_blackhole.c	Black hole queue
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Thomas Graf <tgraf@suug.ch>
 *
 * Note: Quantum tunneling is not supported.
 */

<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/init.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <net/pkt_sched.h>

<<<<<<< HEAD
static int blackhole_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
	qdisc_drop(skb, sch);
	return NET_XMIT_SUCCESS;
=======
static int blackhole_enqueue(struct sk_buff *skb, struct Qdisc *sch,
			     struct sk_buff **to_free)
{
	qdisc_drop(skb, sch, to_free);
	return NET_XMIT_SUCCESS | __NET_XMIT_BYPASS;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static struct sk_buff *blackhole_dequeue(struct Qdisc *sch)
{
	return NULL;
}

static struct Qdisc_ops blackhole_qdisc_ops __read_mostly = {
	.id		= "blackhole",
	.priv_size	= 0,
	.enqueue	= blackhole_enqueue,
	.dequeue	= blackhole_dequeue,
	.peek		= blackhole_dequeue,
	.owner		= THIS_MODULE,
};

<<<<<<< HEAD
static int __init blackhole_module_init(void)
{
	return register_qdisc(&blackhole_qdisc_ops);
}

static void __exit blackhole_module_exit(void)
{
	unregister_qdisc(&blackhole_qdisc_ops);
}

module_init(blackhole_module_init)
module_exit(blackhole_module_exit)

MODULE_LICENSE("GPL");
=======
static int __init blackhole_init(void)
{
	return register_qdisc(&blackhole_qdisc_ops);
}
device_initcall(blackhole_init)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
