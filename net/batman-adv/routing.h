<<<<<<< HEAD
/* Copyright (C) 2007-2013 B.A.T.M.A.N. contributors:
=======
/* Copyright (C) 2007-2016  B.A.T.M.A.N. contributors:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Marek Lindner, Simon Wunderlich
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
<<<<<<< HEAD
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
=======
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#ifndef _NET_BATMAN_ADV_ROUTING_H_
#define _NET_BATMAN_ADV_ROUTING_H_

<<<<<<< HEAD
void batadv_slide_own_bcast_window(struct batadv_hard_iface *hard_iface);
=======
#include "main.h"

#include <linux/types.h>

struct sk_buff;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
bool batadv_check_management_packet(struct sk_buff *skb,
				    struct batadv_hard_iface *hard_iface,
				    int header_len);
void batadv_update_route(struct batadv_priv *bat_priv,
			 struct batadv_orig_node *orig_node,
<<<<<<< HEAD
=======
			 struct batadv_hard_iface *recv_if,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			 struct batadv_neigh_node *neigh_node);
int batadv_recv_icmp_packet(struct sk_buff *skb,
			    struct batadv_hard_iface *recv_if);
int batadv_recv_unicast_packet(struct sk_buff *skb,
			       struct batadv_hard_iface *recv_if);
<<<<<<< HEAD
int batadv_recv_ucast_frag_packet(struct sk_buff *skb,
				  struct batadv_hard_iface *recv_if);
int batadv_recv_bcast_packet(struct sk_buff *skb,
			     struct batadv_hard_iface *recv_if);
int batadv_recv_vis_packet(struct sk_buff *skb,
			   struct batadv_hard_iface *recv_if);
=======
int batadv_recv_frag_packet(struct sk_buff *skb,
			    struct batadv_hard_iface *iface);
int batadv_recv_bcast_packet(struct sk_buff *skb,
			     struct batadv_hard_iface *recv_if);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int batadv_recv_tt_query(struct sk_buff *skb,
			 struct batadv_hard_iface *recv_if);
int batadv_recv_roam_adv(struct sk_buff *skb,
			 struct batadv_hard_iface *recv_if);
<<<<<<< HEAD
struct batadv_neigh_node *
batadv_find_router(struct batadv_priv *bat_priv,
		   struct batadv_orig_node *orig_node,
		   const struct batadv_hard_iface *recv_if);
void batadv_bonding_candidate_del(struct batadv_orig_node *orig_node,
				  struct batadv_neigh_node *neigh_node);
void batadv_bonding_candidate_add(struct batadv_orig_node *orig_node,
				  struct batadv_neigh_node *neigh_node);
void batadv_bonding_save_primary(const struct batadv_orig_node *orig_node,
				 struct batadv_orig_node *orig_neigh_node,
				 const struct batadv_ogm_packet
				 *batman_ogm_packet);
int batadv_window_protected(struct batadv_priv *bat_priv, int32_t seq_num_diff,
			    unsigned long *last_reset);
=======
int batadv_recv_unicast_tvlv(struct sk_buff *skb,
			     struct batadv_hard_iface *recv_if);
int batadv_recv_unhandled_unicast_packet(struct sk_buff *skb,
					 struct batadv_hard_iface *recv_if);
struct batadv_neigh_node *
batadv_find_router(struct batadv_priv *bat_priv,
		   struct batadv_orig_node *orig_node,
		   struct batadv_hard_iface *recv_if);
bool batadv_window_protected(struct batadv_priv *bat_priv, s32 seq_num_diff,
			     s32 seq_old_max_diff, unsigned long *last_reset,
			     bool *protection_started);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _NET_BATMAN_ADV_ROUTING_H_ */
