/*
 * net/tipc/node.h: Include file for TIPC node management routines
 *
<<<<<<< HEAD
 * Copyright (c) 2000-2006, Ericsson AB
 * Copyright (c) 2005, 2010-2011, Wind River Systems
=======
 * Copyright (c) 2000-2006, 2014-2016, Ericsson AB
 * Copyright (c) 2005, 2010-2014, Wind River Systems
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
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

#ifndef _TIPC_NODE_H
#define _TIPC_NODE_H

<<<<<<< HEAD
#include "node_subscr.h"
#include "addr.h"
#include "net.h"
#include "bearer.h"

/*
 * Out-of-range value for node signature
 */
#define INVALID_NODE_SIG 0x10000

/* Flags used to block (re)establishment of contact with a neighboring node */
#define WAIT_PEER_DOWN	0x0001	/* wait to see that peer's links are down */
#define WAIT_NAMES_GONE	0x0002	/* wait for peer's publications to be purged */
#define WAIT_NODE_DOWN	0x0004	/* wait until peer node is declared down */

/**
 * struct tipc_node - TIPC node structure
 * @addr: network address of node
 * @lock: spinlock governing access to structure
 * @hash: links to adjacent nodes in unsorted hash chain
 * @list: links to adjacent nodes in sorted list of cluster's nodes
 * @nsub: list of "node down" subscriptions monitoring node
 * @active_links: pointers to active links to node
 * @links: pointers to all links to node
 * @working_links: number of working links to node (both active and standby)
 * @block_setup: bit mask of conditions preventing link establishment to node
 * @link_cnt: number of links to node
 * @permit_changeover: non-zero if node has redundant links to this system
 * @signature: node instance identifier
 * @bclink: broadcast-related info
 *    @acked: sequence # of last outbound b'cast message acknowledged by node
 *    @last_in: sequence # of last in-sequence b'cast message received from node
 *    @last_sent: sequence # of last b'cast message sent by node
 *    @oos_state: state tracker for handling OOS b'cast messages
 *    @deferred_size: number of OOS b'cast messages in deferred queue
 *    @deferred_head: oldest OOS b'cast message received from node
 *    @deferred_tail: newest OOS b'cast message received from node
 *    @defragm: list of partially reassembled b'cast message fragments from node
 *    @recv_permitted: true if node is allowed to receive b'cast messages
 */
struct tipc_node {
	u32 addr;
	spinlock_t lock;
	struct hlist_node hash;
	struct list_head list;
	struct list_head nsub;
	struct tipc_link *active_links[2];
	struct tipc_link *links[MAX_BEARERS];
	int link_cnt;
	int working_links;
	int block_setup;
	int permit_changeover;
	u32 signature;
	struct {
		u32 acked;
		u32 last_in;
		u32 last_sent;
		u32 oos_state;
		u32 deferred_size;
		struct sk_buff *deferred_head;
		struct sk_buff *deferred_tail;
		struct sk_buff *defragm;
		bool recv_permitted;
	} bclink;
};

extern struct list_head tipc_node_list;

struct tipc_node *tipc_node_find(u32 addr);
struct tipc_node *tipc_node_create(u32 addr);
void tipc_node_delete(struct tipc_node *n_ptr);
void tipc_node_attach_link(struct tipc_node *n_ptr, struct tipc_link *l_ptr);
void tipc_node_detach_link(struct tipc_node *n_ptr, struct tipc_link *l_ptr);
void tipc_node_link_down(struct tipc_node *n_ptr, struct tipc_link *l_ptr);
void tipc_node_link_up(struct tipc_node *n_ptr, struct tipc_link *l_ptr);
int tipc_node_active_links(struct tipc_node *n_ptr);
int tipc_node_redundant_links(struct tipc_node *n_ptr);
int tipc_node_is_up(struct tipc_node *n_ptr);
struct sk_buff *tipc_node_get_links(const void *req_tlv_area, int req_tlv_space);
struct sk_buff *tipc_node_get_nodes(const void *req_tlv_area, int req_tlv_space);

static inline void tipc_node_lock(struct tipc_node *n_ptr)
{
	spin_lock_bh(&n_ptr->lock);
}

static inline void tipc_node_unlock(struct tipc_node *n_ptr)
{
	spin_unlock_bh(&n_ptr->lock);
}

=======
#include "addr.h"
#include "net.h"
#include "bearer.h"
#include "msg.h"

/* Optional capabilities supported by this code version
 */
enum {
	TIPC_BCAST_SYNCH      = (1 << 1),
	TIPC_BCAST_STATE_NACK = (1 << 2),
	TIPC_BLOCK_FLOWCTL    = (1 << 3)
};

#define TIPC_NODE_CAPABILITIES (TIPC_BCAST_SYNCH | \
				TIPC_BCAST_STATE_NACK | \
				TIPC_BLOCK_FLOWCTL)
#define INVALID_BEARER_ID -1

void tipc_node_stop(struct net *net);
void tipc_node_check_dest(struct net *net, u32 onode,
			  struct tipc_bearer *bearer,
			  u16 capabilities, u32 signature,
			  struct tipc_media_addr *maddr,
			  bool *respond, bool *dupl_addr);
void tipc_node_delete_links(struct net *net, int bearer_id);
int tipc_node_get_linkname(struct net *net, u32 bearer_id, u32 node,
			   char *linkname, size_t len);
int tipc_node_xmit(struct net *net, struct sk_buff_head *list, u32 dnode,
		   int selector);
int tipc_node_xmit_skb(struct net *net, struct sk_buff *skb, u32 dest,
		       u32 selector);
void tipc_node_subscribe(struct net *net, struct list_head *subscr, u32 addr);
void tipc_node_unsubscribe(struct net *net, struct list_head *subscr, u32 addr);
void tipc_node_broadcast(struct net *net, struct sk_buff *skb);
int tipc_node_add_conn(struct net *net, u32 dnode, u32 port, u32 peer_port);
void tipc_node_remove_conn(struct net *net, u32 dnode, u32 port);
int tipc_node_get_mtu(struct net *net, u32 addr, u32 sel);
u16 tipc_node_get_capabilities(struct net *net, u32 addr);
int tipc_nl_node_dump(struct sk_buff *skb, struct netlink_callback *cb);
int tipc_nl_node_dump_link(struct sk_buff *skb, struct netlink_callback *cb);
int tipc_nl_node_reset_link_stats(struct sk_buff *skb, struct genl_info *info);
int tipc_nl_node_get_link(struct sk_buff *skb, struct genl_info *info);
int tipc_nl_node_set_link(struct sk_buff *skb, struct genl_info *info);
int tipc_nl_peer_rm(struct sk_buff *skb, struct genl_info *info);

int tipc_nl_node_set_monitor(struct sk_buff *skb, struct genl_info *info);
int tipc_nl_node_get_monitor(struct sk_buff *skb, struct genl_info *info);
int tipc_nl_node_dump_monitor(struct sk_buff *skb, struct netlink_callback *cb);
int tipc_nl_node_dump_monitor_peer(struct sk_buff *skb,
				   struct netlink_callback *cb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
