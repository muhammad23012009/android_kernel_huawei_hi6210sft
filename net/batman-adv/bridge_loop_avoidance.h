<<<<<<< HEAD
/* Copyright (C) 2011-2013 B.A.T.M.A.N. contributors:
=======
/* Copyright (C) 2011-2016  B.A.T.M.A.N. contributors:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Simon Wunderlich
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

#ifndef _NET_BATMAN_ADV_BLA_H_
#define _NET_BATMAN_ADV_BLA_H_

<<<<<<< HEAD
#ifdef CONFIG_BATMAN_ADV_BLA
int batadv_bla_rx(struct batadv_priv *bat_priv, struct sk_buff *skb, short vid,
		  bool is_bcast);
int batadv_bla_tx(struct batadv_priv *bat_priv, struct sk_buff *skb, short vid);
int batadv_bla_is_backbone_gw(struct sk_buff *skb,
			      struct batadv_orig_node *orig_node, int hdr_size);
int batadv_bla_claim_table_seq_print_text(struct seq_file *seq, void *offset);
int batadv_bla_backbone_table_seq_print_text(struct seq_file *seq,
					     void *offset);
int batadv_bla_is_backbone_gw_orig(struct batadv_priv *bat_priv, uint8_t *orig);
int batadv_bla_check_bcast_duplist(struct batadv_priv *bat_priv,
				   struct sk_buff *skb);
void batadv_bla_update_orig_address(struct batadv_priv *bat_priv,
				    struct batadv_hard_iface *primary_if,
				    struct batadv_hard_iface *oldif);
int batadv_bla_init(struct batadv_priv *bat_priv);
void batadv_bla_free(struct batadv_priv *bat_priv);

#define BATADV_BLA_CRC_INIT	0
#else /* ifdef CONFIG_BATMAN_ADV_BLA */

static inline int batadv_bla_rx(struct batadv_priv *bat_priv,
				struct sk_buff *skb, short vid, bool is_bcast)
{
	return 0;
}

static inline int batadv_bla_tx(struct batadv_priv *bat_priv,
				struct sk_buff *skb, short vid)
{
	return 0;
}

static inline int batadv_bla_is_backbone_gw(struct sk_buff *skb,
					    struct batadv_orig_node *orig_node,
					    int hdr_size)
{
	return 0;
=======
#include "main.h"

#include <linux/types.h>

struct net_device;
struct netlink_callback;
struct seq_file;
struct sk_buff;

#ifdef CONFIG_BATMAN_ADV_BLA
bool batadv_bla_rx(struct batadv_priv *bat_priv, struct sk_buff *skb,
		   unsigned short vid, int packet_type);
bool batadv_bla_tx(struct batadv_priv *bat_priv, struct sk_buff *skb,
		   unsigned short vid);
bool batadv_bla_is_backbone_gw(struct sk_buff *skb,
			       struct batadv_orig_node *orig_node,
			       int hdr_size);
int batadv_bla_claim_table_seq_print_text(struct seq_file *seq, void *offset);
int batadv_bla_claim_dump(struct sk_buff *msg, struct netlink_callback *cb);
int batadv_bla_backbone_table_seq_print_text(struct seq_file *seq,
					     void *offset);
int batadv_bla_backbone_dump(struct sk_buff *msg, struct netlink_callback *cb);
bool batadv_bla_is_backbone_gw_orig(struct batadv_priv *bat_priv, u8 *orig,
				    unsigned short vid);
bool batadv_bla_check_bcast_duplist(struct batadv_priv *bat_priv,
				    struct sk_buff *skb);
void batadv_bla_update_orig_address(struct batadv_priv *bat_priv,
				    struct batadv_hard_iface *primary_if,
				    struct batadv_hard_iface *oldif);
void batadv_bla_status_update(struct net_device *net_dev);
int batadv_bla_init(struct batadv_priv *bat_priv);
void batadv_bla_free(struct batadv_priv *bat_priv);
int batadv_bla_claim_dump(struct sk_buff *msg, struct netlink_callback *cb);
#define BATADV_BLA_CRC_INIT	0
#else /* ifdef CONFIG_BATMAN_ADV_BLA */

static inline bool batadv_bla_rx(struct batadv_priv *bat_priv,
				 struct sk_buff *skb, unsigned short vid,
				 int packet_type)
{
	return false;
}

static inline bool batadv_bla_tx(struct batadv_priv *bat_priv,
				 struct sk_buff *skb, unsigned short vid)
{
	return false;
}

static inline bool batadv_bla_is_backbone_gw(struct sk_buff *skb,
					     struct batadv_orig_node *orig_node,
					     int hdr_size)
{
	return false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline int batadv_bla_claim_table_seq_print_text(struct seq_file *seq,
							void *offset)
{
	return 0;
}

static inline int batadv_bla_backbone_table_seq_print_text(struct seq_file *seq,
							   void *offset)
{
	return 0;
}

<<<<<<< HEAD
static inline int batadv_bla_is_backbone_gw_orig(struct batadv_priv *bat_priv,
						 uint8_t *orig)
{
	return 0;
}

static inline int
batadv_bla_check_bcast_duplist(struct batadv_priv *bat_priv,
			       struct sk_buff *skb)
{
	return 0;
=======
static inline bool batadv_bla_is_backbone_gw_orig(struct batadv_priv *bat_priv,
						  u8 *orig, unsigned short vid)
{
	return false;
}

static inline bool
batadv_bla_check_bcast_duplist(struct batadv_priv *bat_priv,
			       struct sk_buff *skb)
{
	return false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void
batadv_bla_update_orig_address(struct batadv_priv *bat_priv,
			       struct batadv_hard_iface *primary_if,
			       struct batadv_hard_iface *oldif)
{
}

static inline int batadv_bla_init(struct batadv_priv *bat_priv)
{
	return 1;
}

static inline void batadv_bla_free(struct batadv_priv *bat_priv)
{
}

<<<<<<< HEAD
=======
static inline int batadv_bla_claim_dump(struct sk_buff *msg,
					struct netlink_callback *cb)
{
	return -EOPNOTSUPP;
}

static inline int batadv_bla_backbone_dump(struct sk_buff *msg,
					   struct netlink_callback *cb)
{
	return -EOPNOTSUPP;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* ifdef CONFIG_BATMAN_ADV_BLA */

#endif /* ifndef _NET_BATMAN_ADV_BLA_H_ */
