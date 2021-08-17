/*
 * net/tipc/name_distr.c: TIPC name distribution code
 *
<<<<<<< HEAD
 * Copyright (c) 2000-2006, Ericsson AB
=======
 * Copyright (c) 2000-2006, 2014, Ericsson AB
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Copyright (c) 2005, 2010-2011, Wind River Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "core.h"
#include "link.h"
#include "name_distr.h"

<<<<<<< HEAD
#define ITEM_SIZE sizeof(struct distr_item)

/**
 * struct distr_item - publication info distributed to other nodes
 * @type: name sequence type
 * @lower: name sequence lower bound
 * @upper: name sequence upper bound
 * @ref: publishing port reference
 * @key: publication key
 *
 * ===> All fields are stored in network byte order. <===
 *
 * First 3 fields identify (name or) name sequence being published.
 * Reference field uniquely identifies port that published name sequence.
 * Key field uniquely identifies publication, in the event a port has
 * multiple publications of the same name sequence.
 *
 * Note: There is no field that identifies the publishing node because it is
 * the same for all items contained within a publication message.
 */
struct distr_item {
	__be32 type;
	__be32 lower;
	__be32 upper;
	__be32 ref;
	__be32 key;
};

/**
 * struct publ_list - list of publications made by this node
 * @list: circular list of publications
 * @list_size: number of entries in list
 */
struct publ_list {
	struct list_head list;
	u32 size;
};

static struct publ_list publ_zone = {
	.list = LIST_HEAD_INIT(publ_zone.list),
	.size = 0,
};

static struct publ_list publ_cluster = {
	.list = LIST_HEAD_INIT(publ_cluster.list),
	.size = 0,
};

static struct publ_list publ_node = {
	.list = LIST_HEAD_INIT(publ_node.list),
	.size = 0,
};

static struct publ_list *publ_lists[] = {
	NULL,
	&publ_zone,	/* publ_lists[TIPC_ZONE_SCOPE]		*/
	&publ_cluster,	/* publ_lists[TIPC_CLUSTER_SCOPE]	*/
	&publ_node	/* publ_lists[TIPC_NODE_SCOPE]		*/
};


=======
int sysctl_tipc_named_timeout __read_mostly = 2000;

struct distr_queue_item {
	struct distr_item i;
	u32 dtype;
	u32 node;
	unsigned long expires;
	struct list_head next;
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * publ_to_item - add publication info to a publication message
 */
static void publ_to_item(struct distr_item *i, struct publication *p)
{
	i->type = htonl(p->type);
	i->lower = htonl(p->lower);
	i->upper = htonl(p->upper);
	i->ref = htonl(p->ref);
	i->key = htonl(p->key);
}

/**
 * named_prepare_buf - allocate & initialize a publication message
<<<<<<< HEAD
 */
static struct sk_buff *named_prepare_buf(u32 type, u32 size, u32 dest)
{
	struct sk_buff *buf = tipc_buf_acquire(INT_H_SIZE + size);
=======
 *
 * The buffer returned is of size INT_H_SIZE + payload size
 */
static struct sk_buff *named_prepare_buf(struct net *net, u32 type, u32 size,
					 u32 dest)
{
	struct tipc_net *tn = net_generic(net, tipc_net_id);
	struct sk_buff *buf = tipc_buf_acquire(INT_H_SIZE + size, GFP_ATOMIC);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct tipc_msg *msg;

	if (buf != NULL) {
		msg = buf_msg(buf);
<<<<<<< HEAD
		tipc_msg_init(msg, NAME_DISTRIBUTOR, type, INT_H_SIZE, dest);
=======
		tipc_msg_init(tn->own_addr, msg, NAME_DISTRIBUTOR, type,
			      INT_H_SIZE, dest);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		msg_set_size(msg, INT_H_SIZE + size);
	}
	return buf;
}

<<<<<<< HEAD
static void named_cluster_distribute(struct sk_buff *buf)
{
	struct sk_buff *buf_copy;
	struct tipc_node *n_ptr;

	list_for_each_entry(n_ptr, &tipc_node_list, list) {
		if (tipc_node_active_links(n_ptr)) {
			buf_copy = skb_copy(buf, GFP_ATOMIC);
			if (!buf_copy)
				break;
			msg_set_destnode(buf_msg(buf_copy), n_ptr->addr);
			tipc_link_send(buf_copy, n_ptr->addr, n_ptr->addr);
		}
	}

	kfree_skb(buf);
}

/**
 * tipc_named_publish - tell other nodes about a new publication by this node
 */
void tipc_named_publish(struct publication *publ)
{
	struct sk_buff *buf;
	struct distr_item *item;

	list_add_tail(&publ->local_list, &publ_lists[publ->scope]->list);
	publ_lists[publ->scope]->size++;

	if (publ->scope == TIPC_NODE_SCOPE)
		return;

	buf = named_prepare_buf(PUBLICATION, ITEM_SIZE, 0);
	if (!buf) {
		pr_warn("Publication distribution failure\n");
		return;
=======
/**
 * tipc_named_publish - tell other nodes about a new publication by this node
 */
struct sk_buff *tipc_named_publish(struct net *net, struct publication *publ)
{
	struct tipc_net *tn = net_generic(net, tipc_net_id);
	struct sk_buff *buf;
	struct distr_item *item;

	list_add_tail_rcu(&publ->local_list,
			  &tn->nametbl->publ_list[publ->scope]);

	if (publ->scope == TIPC_NODE_SCOPE)
		return NULL;

	buf = named_prepare_buf(net, PUBLICATION, ITEM_SIZE, 0);
	if (!buf) {
		pr_warn("Publication distribution failure\n");
		return NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	item = (struct distr_item *)msg_data(buf_msg(buf));
	publ_to_item(item, publ);
<<<<<<< HEAD
	named_cluster_distribute(buf);
=======
	return buf;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * tipc_named_withdraw - tell other nodes about a withdrawn publication by this node
 */
<<<<<<< HEAD
void tipc_named_withdraw(struct publication *publ)
=======
struct sk_buff *tipc_named_withdraw(struct net *net, struct publication *publ)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct sk_buff *buf;
	struct distr_item *item;

	list_del(&publ->local_list);
<<<<<<< HEAD
	publ_lists[publ->scope]->size--;

	if (publ->scope == TIPC_NODE_SCOPE)
		return;

	buf = named_prepare_buf(WITHDRAWAL, ITEM_SIZE, 0);
	if (!buf) {
		pr_warn("Withdrawal distribution failure\n");
		return;
=======

	if (publ->scope == TIPC_NODE_SCOPE)
		return NULL;

	buf = named_prepare_buf(net, WITHDRAWAL, ITEM_SIZE, 0);
	if (!buf) {
		pr_warn("Withdrawal distribution failure\n");
		return NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	item = (struct distr_item *)msg_data(buf_msg(buf));
	publ_to_item(item, publ);
<<<<<<< HEAD
	named_cluster_distribute(buf);
}

/*
 * named_distribute - prepare name info for bulk distribution to another node
 */
static void named_distribute(struct list_head *message_list, u32 node,
			     struct publ_list *pls, u32 max_item_buf)
{
	struct publication *publ;
	struct sk_buff *buf = NULL;
	struct distr_item *item = NULL;
	u32 left = 0;
	u32 rest = pls->size * ITEM_SIZE;

	list_for_each_entry(publ, &pls->list, local_list) {
		if (!buf) {
			left = (rest <= max_item_buf) ? rest : max_item_buf;
			rest -= left;
			buf = named_prepare_buf(PUBLICATION, left, node);
			if (!buf) {
				pr_warn("Bulk publication failure\n");
				return;
			}
			item = (struct distr_item *)msg_data(buf_msg(buf));
		}
		publ_to_item(item, publ);
		item++;
		left -= ITEM_SIZE;
		if (!left) {
			list_add_tail((struct list_head *)buf, message_list);
			buf = NULL;
		}
	}
=======
	return buf;
}

/**
 * named_distribute - prepare name info for bulk distribution to another node
 * @list: list of messages (buffers) to be returned from this function
 * @dnode: node to be updated
 * @pls: linked list of publication items to be packed into buffer chain
 */
static void named_distribute(struct net *net, struct sk_buff_head *list,
			     u32 dnode, struct list_head *pls)
{
	struct publication *publ;
	struct sk_buff *skb = NULL;
	struct distr_item *item = NULL;
	u32 msg_dsz = ((tipc_node_get_mtu(net, dnode, 0) - INT_H_SIZE) /
			ITEM_SIZE) * ITEM_SIZE;
	u32 msg_rem = msg_dsz;

	list_for_each_entry(publ, pls, local_list) {
		/* Prepare next buffer: */
		if (!skb) {
			skb = named_prepare_buf(net, PUBLICATION, msg_rem,
						dnode);
			if (!skb) {
				pr_warn("Bulk publication failure\n");
				return;
			}
			msg_set_bc_ack_invalid(buf_msg(skb), true);
			item = (struct distr_item *)msg_data(buf_msg(skb));
		}

		/* Pack publication into message: */
		publ_to_item(item, publ);
		item++;
		msg_rem -= ITEM_SIZE;

		/* Append full buffer to list: */
		if (!msg_rem) {
			__skb_queue_tail(list, skb);
			skb = NULL;
			msg_rem = msg_dsz;
		}
	}
	if (skb) {
		msg_set_size(buf_msg(skb), INT_H_SIZE + (msg_dsz - msg_rem));
		skb_trim(skb, INT_H_SIZE + (msg_dsz - msg_rem));
		__skb_queue_tail(list, skb);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * tipc_named_node_up - tell specified node about all publications by this node
 */
<<<<<<< HEAD
void tipc_named_node_up(unsigned long nodearg)
{
	struct tipc_node *n_ptr;
	struct tipc_link *l_ptr;
	struct list_head message_list;
	u32 node = (u32)nodearg;
	u32 max_item_buf = 0;

	/* compute maximum amount of publication data to send per message */
	read_lock_bh(&tipc_net_lock);
	n_ptr = tipc_node_find(node);
	if (n_ptr) {
		tipc_node_lock(n_ptr);
		l_ptr = n_ptr->active_links[0];
		if (l_ptr)
			max_item_buf = ((l_ptr->max_pkt - INT_H_SIZE) /
				ITEM_SIZE) * ITEM_SIZE;
		tipc_node_unlock(n_ptr);
	}
	read_unlock_bh(&tipc_net_lock);
	if (!max_item_buf)
		return;

	/* create list of publication messages, then send them as a unit */
	INIT_LIST_HEAD(&message_list);

	read_lock_bh(&tipc_nametbl_lock);
	named_distribute(&message_list, node, &publ_cluster, max_item_buf);
	named_distribute(&message_list, node, &publ_zone, max_item_buf);
	read_unlock_bh(&tipc_nametbl_lock);

	tipc_link_send_names(&message_list, node);
}

/**
 * named_purge_publ - remove publication associated with a failed node
=======
void tipc_named_node_up(struct net *net, u32 dnode)
{
	struct tipc_net *tn = net_generic(net, tipc_net_id);
	struct sk_buff_head head;

	__skb_queue_head_init(&head);

	rcu_read_lock();
	named_distribute(net, &head, dnode,
			 &tn->nametbl->publ_list[TIPC_CLUSTER_SCOPE]);
	named_distribute(net, &head, dnode,
			 &tn->nametbl->publ_list[TIPC_ZONE_SCOPE]);
	rcu_read_unlock();

	tipc_node_xmit(net, &head, dnode, 0);
}

/**
 * tipc_publ_purge - remove publication associated with a failed node
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Invoked for each publication issued by a newly failed node.
 * Removes publication structure from name table & deletes it.
 */
<<<<<<< HEAD
static void named_purge_publ(struct publication *publ)
{
	struct publication *p;

	write_lock_bh(&tipc_nametbl_lock);
	p = tipc_nametbl_remove_publ(publ->type, publ->lower,
				     publ->node, publ->ref, publ->key);
	if (p)
		tipc_nodesub_unsubscribe(&p->subscr);
	write_unlock_bh(&tipc_nametbl_lock);
=======
static void tipc_publ_purge(struct net *net, struct publication *publ, u32 addr)
{
	struct tipc_net *tn = net_generic(net, tipc_net_id);
	struct publication *p;

	spin_lock_bh(&tn->nametbl_lock);
	p = tipc_nametbl_remove_publ(net, publ->type, publ->lower,
				     publ->node, publ->ref, publ->key);
	if (p)
		tipc_node_unsubscribe(net, &p->nodesub_list, addr);
	spin_unlock_bh(&tn->nametbl_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (p != publ) {
		pr_err("Unable to remove publication from failed node\n"
		       " (type=%u, lower=%u, node=0x%x, ref=%u, key=%u)\n",
		       publ->type, publ->lower, publ->node, publ->ref,
		       publ->key);
	}

<<<<<<< HEAD
	kfree(p);
}

/**
 * tipc_named_recv - process name table update message sent by another node
 */
void tipc_named_recv(struct sk_buff *buf)
{
	struct publication *publ;
	struct tipc_msg *msg = buf_msg(buf);
	struct distr_item *item = (struct distr_item *)msg_data(msg);
	u32 count = msg_data_sz(msg) / ITEM_SIZE;

	write_lock_bh(&tipc_nametbl_lock);
	while (count--) {
		if (msg_type(msg) == PUBLICATION) {
			publ = tipc_nametbl_insert_publ(ntohl(item->type),
							ntohl(item->lower),
							ntohl(item->upper),
							TIPC_CLUSTER_SCOPE,
							msg_orignode(msg),
							ntohl(item->ref),
							ntohl(item->key));
			if (publ) {
				tipc_nodesub_subscribe(&publ->subscr,
						       msg_orignode(msg),
						       publ,
						       (net_ev_handler)
						       named_purge_publ);
			}
		} else if (msg_type(msg) == WITHDRAWAL) {
			publ = tipc_nametbl_remove_publ(ntohl(item->type),
							ntohl(item->lower),
							msg_orignode(msg),
							ntohl(item->ref),
							ntohl(item->key));

			if (publ) {
				tipc_nodesub_unsubscribe(&publ->subscr);
				kfree(publ);
			} else {
				pr_err("Unable to remove publication by node 0x%x\n"
				       " (type=%u, lower=%u, ref=%u, key=%u)\n",
				       msg_orignode(msg), ntohl(item->type),
				       ntohl(item->lower), ntohl(item->ref),
				       ntohl(item->key));
			}
		} else {
			pr_warn("Unrecognized name table message received\n");
		}
		item++;
	}
	write_unlock_bh(&tipc_nametbl_lock);
	kfree_skb(buf);
=======
	if (p)
		kfree_rcu(p, rcu);
}

/**
 * tipc_dist_queue_purge - remove deferred updates from a node that went down
 */
static void tipc_dist_queue_purge(struct net *net, u32 addr)
{
	struct tipc_net *tn = net_generic(net, tipc_net_id);
	struct distr_queue_item *e, *tmp;

	spin_lock_bh(&tn->nametbl_lock);
	list_for_each_entry_safe(e, tmp, &tn->dist_queue, next) {
		if (e->node != addr)
			continue;
		list_del(&e->next);
		kfree(e);
	}
	spin_unlock_bh(&tn->nametbl_lock);
}

void tipc_publ_notify(struct net *net, struct list_head *nsub_list, u32 addr)
{
	struct publication *publ, *tmp;

	list_for_each_entry_safe(publ, tmp, nsub_list, nodesub_list)
		tipc_publ_purge(net, publ, addr);
	tipc_dist_queue_purge(net, addr);
}

/**
 * tipc_update_nametbl - try to process a nametable update and notify
 *			 subscribers
 *
 * tipc_nametbl_lock must be held.
 * Returns the publication item if successful, otherwise NULL.
 */
static bool tipc_update_nametbl(struct net *net, struct distr_item *i,
				u32 node, u32 dtype)
{
	struct publication *publ = NULL;

	if (dtype == PUBLICATION) {
		publ = tipc_nametbl_insert_publ(net, ntohl(i->type),
						ntohl(i->lower),
						ntohl(i->upper),
						TIPC_CLUSTER_SCOPE, node,
						ntohl(i->ref), ntohl(i->key));
		if (publ) {
			tipc_node_subscribe(net, &publ->nodesub_list, node);
			return true;
		}
	} else if (dtype == WITHDRAWAL) {
		publ = tipc_nametbl_remove_publ(net, ntohl(i->type),
						ntohl(i->lower),
						node, ntohl(i->ref),
						ntohl(i->key));
		if (publ) {
			tipc_node_unsubscribe(net, &publ->nodesub_list, node);
			kfree_rcu(publ, rcu);
			return true;
		}
	} else {
		pr_warn("Unrecognized name table message received\n");
	}
	return false;
}

/**
 * tipc_named_add_backlog - add a failed name table update to the backlog
 *
 */
static void tipc_named_add_backlog(struct net *net, struct distr_item *i,
				   u32 type, u32 node)
{
	struct distr_queue_item *e;
	struct tipc_net *tn = net_generic(net, tipc_net_id);
	unsigned long now = get_jiffies_64();

	e = kzalloc(sizeof(*e), GFP_ATOMIC);
	if (!e)
		return;
	e->dtype = type;
	e->node = node;
	e->expires = now + msecs_to_jiffies(sysctl_tipc_named_timeout);
	memcpy(e, i, sizeof(*i));
	list_add_tail(&e->next, &tn->dist_queue);
}

/**
 * tipc_named_process_backlog - try to process any pending name table updates
 * from the network.
 */
void tipc_named_process_backlog(struct net *net)
{
	struct distr_queue_item *e, *tmp;
	struct tipc_net *tn = net_generic(net, tipc_net_id);
	char addr[16];
	unsigned long now = get_jiffies_64();

	list_for_each_entry_safe(e, tmp, &tn->dist_queue, next) {
		if (time_after(e->expires, now)) {
			if (!tipc_update_nametbl(net, &e->i, e->node, e->dtype))
				continue;
		} else {
			tipc_addr_string_fill(addr, e->node);
			pr_warn_ratelimited("Dropping name table update (%d) of {%u, %u, %u} from %s key=%u\n",
					    e->dtype, ntohl(e->i.type),
					    ntohl(e->i.lower),
					    ntohl(e->i.upper),
					    addr, ntohl(e->i.key));
		}
		list_del(&e->next);
		kfree(e);
	}
}

/**
 * tipc_named_rcv - process name table update messages sent by another node
 */
void tipc_named_rcv(struct net *net, struct sk_buff_head *inputq)
{
	struct tipc_net *tn = net_generic(net, tipc_net_id);
	struct tipc_msg *msg;
	struct distr_item *item;
	uint count;
	u32 node;
	struct sk_buff *skb;
	int mtype;

	spin_lock_bh(&tn->nametbl_lock);
	for (skb = skb_dequeue(inputq); skb; skb = skb_dequeue(inputq)) {
		skb_linearize(skb);
		msg = buf_msg(skb);
		mtype = msg_type(msg);
		item = (struct distr_item *)msg_data(msg);
		count = msg_data_sz(msg) / ITEM_SIZE;
		node = msg_orignode(msg);
		while (count--) {
			if (!tipc_update_nametbl(net, item, node, mtype))
				tipc_named_add_backlog(net, item, mtype, node);
			item++;
		}
		kfree_skb(skb);
		tipc_named_process_backlog(net);
	}
	spin_unlock_bh(&tn->nametbl_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * tipc_named_reinit - re-initialize local publications
 *
 * This routine is called whenever TIPC networking is enabled.
 * All name table entries published by this node are updated to reflect
 * the node's new network address.
 */
<<<<<<< HEAD
void tipc_named_reinit(void)
{
	struct publication *publ;
	int scope;

	write_lock_bh(&tipc_nametbl_lock);

	for (scope = TIPC_ZONE_SCOPE; scope <= TIPC_NODE_SCOPE; scope++)
		list_for_each_entry(publ, &publ_lists[scope]->list, local_list)
			publ->node = tipc_own_addr;

	write_unlock_bh(&tipc_nametbl_lock);
=======
void tipc_named_reinit(struct net *net)
{
	struct tipc_net *tn = net_generic(net, tipc_net_id);
	struct publication *publ;
	int scope;

	spin_lock_bh(&tn->nametbl_lock);

	for (scope = TIPC_ZONE_SCOPE; scope <= TIPC_NODE_SCOPE; scope++)
		list_for_each_entry_rcu(publ, &tn->nametbl->publ_list[scope],
					local_list)
			publ->node = tn->own_addr;

	spin_unlock_bh(&tn->nametbl_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
