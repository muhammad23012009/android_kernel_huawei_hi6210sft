<<<<<<< HEAD
/* Copyright (C) 2009-2013 B.A.T.M.A.N. contributors:
=======
/* Copyright (C) 2009-2016  B.A.T.M.A.N. contributors:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Marek Lindner
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
 */

#include "main.h"
#include "sysfs.h"
#include "gateway_client.h"
#include "gateway_common.h"
#include "hard-interface.h"
#include "originator.h"
#include "translation-table.h"
#include "routing.h"
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/udp.h>
#include <linux/if_vlan.h>

/* This is the offset of the options field in a dhcp packet starting at
 * the beginning of the dhcp header
 */
#define BATADV_DHCP_OPTIONS_OFFSET 240
#define BATADV_DHCP_REQUEST 3

static void batadv_gw_node_free_ref(struct batadv_gw_node *gw_node)
{
	if (atomic_dec_and_test(&gw_node->refcount))
		kfree_rcu(gw_node, rcu);
}

static struct batadv_gw_node *
=======
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "gateway_client.h"
#include "main.h"

#include <linux/atomic.h>
#include <linux/byteorder/generic.h>
#include <linux/errno.h>
#include <linux/etherdevice.h>
#include <linux/fs.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/kernel.h>
#include <linux/kref.h>
#include <linux/list.h>
#include <linux/lockdep.h>
#include <linux/netdevice.h>
#include <linux/netlink.h>
#include <linux/rculist.h>
#include <linux/rcupdate.h>
#include <linux/seq_file.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/stddef.h>
#include <linux/udp.h>
#include <net/sock.h>
#include <uapi/linux/batman_adv.h>

#include "gateway_common.h"
#include "hard-interface.h"
#include "log.h"
#include "netlink.h"
#include "originator.h"
#include "packet.h"
#include "routing.h"
#include "soft-interface.h"
#include "sysfs.h"
#include "translation-table.h"

/* These are the offsets of the "hw type" and "hw address length" in the dhcp
 * packet starting at the beginning of the dhcp header
 */
#define BATADV_DHCP_HTYPE_OFFSET	1
#define BATADV_DHCP_HLEN_OFFSET		2
/* Value of htype representing Ethernet */
#define BATADV_DHCP_HTYPE_ETHERNET	0x01
/* This is the offset of the "chaddr" field in the dhcp packet starting at the
 * beginning of the dhcp header
 */
#define BATADV_DHCP_CHADDR_OFFSET	28

/**
 * batadv_gw_node_release - release gw_node from lists and queue for free after
 *  rcu grace period
 * @ref: kref pointer of the gw_node
 */
static void batadv_gw_node_release(struct kref *ref)
{
	struct batadv_gw_node *gw_node;

	gw_node = container_of(ref, struct batadv_gw_node, refcount);

	batadv_orig_node_put(gw_node->orig_node);
	kfree_rcu(gw_node, rcu);
}

/**
 * batadv_gw_node_put - decrement the gw_node refcounter and possibly release it
 * @gw_node: gateway node to free
 */
void batadv_gw_node_put(struct batadv_gw_node *gw_node)
{
	kref_put(&gw_node->refcount, batadv_gw_node_release);
}

struct batadv_gw_node *
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
batadv_gw_get_selected_gw_node(struct batadv_priv *bat_priv)
{
	struct batadv_gw_node *gw_node;

	rcu_read_lock();
	gw_node = rcu_dereference(bat_priv->gw.curr_gw);
	if (!gw_node)
		goto out;

<<<<<<< HEAD
	if (!atomic_inc_not_zero(&gw_node->refcount))
=======
	if (!kref_get_unless_zero(&gw_node->refcount))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		gw_node = NULL;

out:
	rcu_read_unlock();
	return gw_node;
}

struct batadv_orig_node *
batadv_gw_get_selected_orig(struct batadv_priv *bat_priv)
{
	struct batadv_gw_node *gw_node;
	struct batadv_orig_node *orig_node = NULL;

	gw_node = batadv_gw_get_selected_gw_node(bat_priv);
	if (!gw_node)
		goto out;

	rcu_read_lock();
	orig_node = gw_node->orig_node;
	if (!orig_node)
		goto unlock;

<<<<<<< HEAD
	if (!atomic_inc_not_zero(&orig_node->refcount))
=======
	if (!kref_get_unless_zero(&orig_node->refcount))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		orig_node = NULL;

unlock:
	rcu_read_unlock();
out:
	if (gw_node)
<<<<<<< HEAD
		batadv_gw_node_free_ref(gw_node);
=======
		batadv_gw_node_put(gw_node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return orig_node;
}

static void batadv_gw_select(struct batadv_priv *bat_priv,
			     struct batadv_gw_node *new_gw_node)
{
	struct batadv_gw_node *curr_gw_node;

	spin_lock_bh(&bat_priv->gw.list_lock);

<<<<<<< HEAD
	if (new_gw_node && !atomic_inc_not_zero(&new_gw_node->refcount))
		new_gw_node = NULL;
=======
	if (new_gw_node)
		kref_get(&new_gw_node->refcount);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	curr_gw_node = rcu_dereference_protected(bat_priv->gw.curr_gw, 1);
	rcu_assign_pointer(bat_priv->gw.curr_gw, new_gw_node);

	if (curr_gw_node)
<<<<<<< HEAD
		batadv_gw_node_free_ref(curr_gw_node);
=======
		batadv_gw_node_put(curr_gw_node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_unlock_bh(&bat_priv->gw.list_lock);
}

<<<<<<< HEAD
void batadv_gw_deselect(struct batadv_priv *bat_priv)
=======
/**
 * batadv_gw_reselect - force a gateway reselection
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Set a flag to remind the GW component to perform a new gateway reselection.
 * However this function does not ensure that the current gateway is going to be
 * deselected. The reselection mechanism may elect the same gateway once again.
 *
 * This means that invoking batadv_gw_reselect() does not guarantee a gateway
 * change and therefore a uevent is not necessarily expected.
 */
void batadv_gw_reselect(struct batadv_priv *bat_priv)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	atomic_set(&bat_priv->gw.reselect, 1);
}

<<<<<<< HEAD
static struct batadv_gw_node *
batadv_gw_get_best_gw_node(struct batadv_priv *bat_priv)
{
	struct batadv_neigh_node *router;
	struct batadv_gw_node *gw_node, *curr_gw = NULL;
	uint32_t max_gw_factor = 0, tmp_gw_factor = 0;
	uint32_t gw_divisor;
	uint8_t max_tq = 0;
	int down, up;
	uint8_t tq_avg;
	struct batadv_orig_node *orig_node;

	gw_divisor = BATADV_TQ_LOCAL_WINDOW_SIZE * BATADV_TQ_LOCAL_WINDOW_SIZE;
	gw_divisor *= 64;

	rcu_read_lock();
	hlist_for_each_entry_rcu(gw_node, &bat_priv->gw.list, list) {
		if (gw_node->deleted)
			continue;

		orig_node = gw_node->orig_node;
		router = batadv_orig_node_get_router(orig_node);
		if (!router)
			continue;

		if (!atomic_inc_not_zero(&gw_node->refcount))
			goto next;

		tq_avg = router->tq_avg;

		switch (atomic_read(&bat_priv->gw_sel_class)) {
		case 1: /* fast connection */
			batadv_gw_bandwidth_to_kbit(orig_node->gw_flags,
						    &down, &up);

			tmp_gw_factor = tq_avg * tq_avg * down * 100 * 100;
			tmp_gw_factor /= gw_divisor;

			if ((tmp_gw_factor > max_gw_factor) ||
			    ((tmp_gw_factor == max_gw_factor) &&
			     (tq_avg > max_tq))) {
				if (curr_gw)
					batadv_gw_node_free_ref(curr_gw);
				curr_gw = gw_node;
				atomic_inc(&curr_gw->refcount);
			}
			break;

		default: /* 2:  stable connection (use best statistic)
			  * 3:  fast-switch (use best statistic but change as
			  *     soon as a better gateway appears)
			  * XX: late-switch (use best statistic but change as
			  *     soon as a better gateway appears which has
			  *     $routing_class more tq points)
			  */
			if (tq_avg > max_tq) {
				if (curr_gw)
					batadv_gw_node_free_ref(curr_gw);
				curr_gw = gw_node;
				atomic_inc(&curr_gw->refcount);
			}
			break;
		}

		if (tq_avg > max_tq)
			max_tq = tq_avg;

		if (tmp_gw_factor > max_gw_factor)
			max_gw_factor = tmp_gw_factor;

		batadv_gw_node_free_ref(gw_node);

next:
		batadv_neigh_node_free_ref(router);
	}
	rcu_read_unlock();

	return curr_gw;
=======
/**
 * batadv_gw_check_client_stop - check if client mode has been switched off
 * @bat_priv: the bat priv with all the soft interface information
 *
 * This function assumes the caller has checked that the gw state *is actually
 * changing*. This function is not supposed to be called when there is no state
 * change.
 */
void batadv_gw_check_client_stop(struct batadv_priv *bat_priv)
{
	struct batadv_gw_node *curr_gw;

	if (atomic_read(&bat_priv->gw.mode) != BATADV_GW_MODE_CLIENT)
		return;

	curr_gw = batadv_gw_get_selected_gw_node(bat_priv);
	if (!curr_gw)
		return;

	/* deselect the current gateway so that next time that client mode is
	 * enabled a proper GW_ADD event can be sent
	 */
	batadv_gw_select(bat_priv, NULL);

	/* if batman-adv is switching the gw client mode off and a gateway was
	 * already selected, send a DEL uevent
	 */
	batadv_throw_uevent(bat_priv, BATADV_UEV_GW, BATADV_UEV_DEL, NULL);

	batadv_gw_node_put(curr_gw);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void batadv_gw_election(struct batadv_priv *bat_priv)
{
<<<<<<< HEAD
	struct batadv_gw_node *curr_gw = NULL, *next_gw = NULL;
	struct batadv_neigh_node *router = NULL;
	char gw_addr[18] = { '\0' };

	/* The batman daemon checks here if we already passed a full originator
	 * cycle in order to make sure we don't choose the first gateway we
	 * hear about. This check is based on the daemon's uptime which we
	 * don't have.
	 */
	if (atomic_read(&bat_priv->gw_mode) != BATADV_GW_MODE_CLIENT)
=======
	struct batadv_gw_node *curr_gw = NULL;
	struct batadv_gw_node *next_gw = NULL;
	struct batadv_neigh_node *router = NULL;
	struct batadv_neigh_ifinfo *router_ifinfo = NULL;
	char gw_addr[18] = { '\0' };

	if (atomic_read(&bat_priv->gw.mode) != BATADV_GW_MODE_CLIENT)
		goto out;

	if (!bat_priv->algo_ops->gw.get_best_gw_node)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto out;

	curr_gw = batadv_gw_get_selected_gw_node(bat_priv);

	if (!batadv_atomic_dec_not_zero(&bat_priv->gw.reselect) && curr_gw)
		goto out;

<<<<<<< HEAD
	next_gw = batadv_gw_get_best_gw_node(bat_priv);
=======
	/* if gw.reselect is set to 1 it means that a previous call to
	 * gw.is_eligible() said that we have a new best GW, therefore it can
	 * now be picked from the list and selected
	 */
	next_gw = bat_priv->algo_ops->gw.get_best_gw_node(bat_priv);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (curr_gw == next_gw)
		goto out;

	if (next_gw) {
		sprintf(gw_addr, "%pM", next_gw->orig_node->orig);

<<<<<<< HEAD
		router = batadv_orig_node_get_router(next_gw->orig_node);
		if (!router) {
			batadv_gw_deselect(bat_priv);
=======
		router = batadv_orig_router_get(next_gw->orig_node,
						BATADV_IF_DEFAULT);
		if (!router) {
			batadv_gw_reselect(bat_priv);
			goto out;
		}

		router_ifinfo = batadv_neigh_ifinfo_get(router,
							BATADV_IF_DEFAULT);
		if (!router_ifinfo) {
			batadv_gw_reselect(bat_priv);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		}
	}

	if ((curr_gw) && (!next_gw)) {
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "Removing selected gateway - no gateway in range\n");
		batadv_throw_uevent(bat_priv, BATADV_UEV_GW, BATADV_UEV_DEL,
				    NULL);
	} else if ((!curr_gw) && (next_gw)) {
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
<<<<<<< HEAD
			   "Adding route to gateway %pM (gw_flags: %i, tq: %i)\n",
			   next_gw->orig_node->orig,
			   next_gw->orig_node->gw_flags, router->tq_avg);
=======
			   "Adding route to gateway %pM (bandwidth: %u.%u/%u.%u MBit, tq: %i)\n",
			   next_gw->orig_node->orig,
			   next_gw->bandwidth_down / 10,
			   next_gw->bandwidth_down % 10,
			   next_gw->bandwidth_up / 10,
			   next_gw->bandwidth_up % 10,
			   router_ifinfo->bat_iv.tq_avg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		batadv_throw_uevent(bat_priv, BATADV_UEV_GW, BATADV_UEV_ADD,
				    gw_addr);
	} else {
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
<<<<<<< HEAD
			   "Changing route to gateway %pM (gw_flags: %i, tq: %i)\n",
			   next_gw->orig_node->orig,
			   next_gw->orig_node->gw_flags, router->tq_avg);
=======
			   "Changing route to gateway %pM (bandwidth: %u.%u/%u.%u MBit, tq: %i)\n",
			   next_gw->orig_node->orig,
			   next_gw->bandwidth_down / 10,
			   next_gw->bandwidth_down % 10,
			   next_gw->bandwidth_up / 10,
			   next_gw->bandwidth_up % 10,
			   router_ifinfo->bat_iv.tq_avg);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		batadv_throw_uevent(bat_priv, BATADV_UEV_GW, BATADV_UEV_CHANGE,
				    gw_addr);
	}

	batadv_gw_select(bat_priv, next_gw);

out:
	if (curr_gw)
<<<<<<< HEAD
		batadv_gw_node_free_ref(curr_gw);
	if (next_gw)
		batadv_gw_node_free_ref(next_gw);
	if (router)
		batadv_neigh_node_free_ref(router);
=======
		batadv_gw_node_put(curr_gw);
	if (next_gw)
		batadv_gw_node_put(next_gw);
	if (router)
		batadv_neigh_node_put(router);
	if (router_ifinfo)
		batadv_neigh_ifinfo_put(router_ifinfo);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void batadv_gw_check_election(struct batadv_priv *bat_priv,
			      struct batadv_orig_node *orig_node)
{
	struct batadv_orig_node *curr_gw_orig;
<<<<<<< HEAD
	struct batadv_neigh_node *router_gw = NULL, *router_orig = NULL;
	uint8_t gw_tq_avg, orig_tq_avg;

	curr_gw_orig = batadv_gw_get_selected_orig(bat_priv);
	if (!curr_gw_orig)
		goto deselect;

	router_gw = batadv_orig_node_get_router(curr_gw_orig);
	if (!router_gw)
		goto deselect;
=======

	/* abort immediately if the routing algorithm does not support gateway
	 * election
	 */
	if (!bat_priv->algo_ops->gw.is_eligible)
		return;

	curr_gw_orig = batadv_gw_get_selected_orig(bat_priv);
	if (!curr_gw_orig)
		goto reselect;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* this node already is the gateway */
	if (curr_gw_orig == orig_node)
		goto out;

<<<<<<< HEAD
	router_orig = batadv_orig_node_get_router(orig_node);
	if (!router_orig)
		goto out;

	gw_tq_avg = router_gw->tq_avg;
	orig_tq_avg = router_orig->tq_avg;

	/* the TQ value has to be better */
	if (orig_tq_avg < gw_tq_avg)
		goto out;

	/* if the routing class is greater than 3 the value tells us how much
	 * greater the TQ value of the new gateway must be
	 */
	if ((atomic_read(&bat_priv->gw_sel_class) > 3) &&
	    (orig_tq_avg - gw_tq_avg < atomic_read(&bat_priv->gw_sel_class)))
		goto out;

	batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
		   "Restarting gateway selection: better gateway found (tq curr: %i, tq new: %i)\n",
		   gw_tq_avg, orig_tq_avg);

deselect:
	batadv_gw_deselect(bat_priv);
out:
	if (curr_gw_orig)
		batadv_orig_node_free_ref(curr_gw_orig);
	if (router_gw)
		batadv_neigh_node_free_ref(router_gw);
	if (router_orig)
		batadv_neigh_node_free_ref(router_orig);

	return;
}

static void batadv_gw_node_add(struct batadv_priv *bat_priv,
			       struct batadv_orig_node *orig_node,
			       uint8_t new_gwflags)
{
	struct batadv_gw_node *gw_node;
	int down, up;
=======
	if (!bat_priv->algo_ops->gw.is_eligible(bat_priv, curr_gw_orig,
						orig_node))
		goto out;

reselect:
	batadv_gw_reselect(bat_priv);
out:
	if (curr_gw_orig)
		batadv_orig_node_put(curr_gw_orig);
}

/**
 * batadv_gw_node_add - add gateway node to list of available gateways
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: originator announcing gateway capabilities
 * @gateway: announced bandwidth information
 *
 * Has to be called with the appropriate locks being acquired
 * (gw.list_lock).
 */
static void batadv_gw_node_add(struct batadv_priv *bat_priv,
			       struct batadv_orig_node *orig_node,
			       struct batadv_tvlv_gateway_data *gateway)
{
	struct batadv_gw_node *gw_node;

	lockdep_assert_held(&bat_priv->gw.list_lock);

	if (gateway->bandwidth_down == 0)
		return;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	gw_node = kzalloc(sizeof(*gw_node), GFP_ATOMIC);
	if (!gw_node)
		return;

<<<<<<< HEAD
	INIT_HLIST_NODE(&gw_node->list);
	gw_node->orig_node = orig_node;
	atomic_set(&gw_node->refcount, 1);

	spin_lock_bh(&bat_priv->gw.list_lock);
	hlist_add_head_rcu(&gw_node->list, &bat_priv->gw.list);
	spin_unlock_bh(&bat_priv->gw.list_lock);

	batadv_gw_bandwidth_to_kbit(new_gwflags, &down, &up);
	batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
		   "Found new gateway %pM -> gw_class: %i - %i%s/%i%s\n",
		   orig_node->orig, new_gwflags,
		   (down > 2048 ? down / 1024 : down),
		   (down > 2048 ? "MBit" : "KBit"),
		   (up > 2048 ? up / 1024 : up),
		   (up > 2048 ? "MBit" : "KBit"));
}

void batadv_gw_node_update(struct batadv_priv *bat_priv,
			   struct batadv_orig_node *orig_node,
			   uint8_t new_gwflags)
{
	struct batadv_gw_node *gw_node, *curr_gw;

	/* Note: We don't need a NULL check here, since curr_gw never gets
	 * dereferenced. If curr_gw is NULL we also should not exit as we may
	 * have this gateway in our list (duplication check!) even though we
	 * have no currently selected gateway.
	 */
	curr_gw = batadv_gw_get_selected_gw_node(bat_priv);

	rcu_read_lock();
	hlist_for_each_entry_rcu(gw_node, &bat_priv->gw.list, list) {
		if (gw_node->orig_node != orig_node)
			continue;

		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "Gateway class of originator %pM changed from %i to %i\n",
			   orig_node->orig, gw_node->orig_node->gw_flags,
			   new_gwflags);

		gw_node->deleted = 0;

		if (new_gwflags == BATADV_NO_FLAGS) {
			gw_node->deleted = jiffies;
			batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
				   "Gateway %pM removed from gateway list\n",
				   orig_node->orig);

			if (gw_node == curr_gw)
				goto deselect;
		}

		goto unlock;
	}

	if (new_gwflags == BATADV_NO_FLAGS)
		goto unlock;

	batadv_gw_node_add(bat_priv, orig_node, new_gwflags);
	goto unlock;

deselect:
	batadv_gw_deselect(bat_priv);
unlock:
	rcu_read_unlock();

	if (curr_gw)
		batadv_gw_node_free_ref(curr_gw);
=======
	kref_init(&gw_node->refcount);
	INIT_HLIST_NODE(&gw_node->list);
	kref_get(&orig_node->refcount);
	gw_node->orig_node = orig_node;
	gw_node->bandwidth_down = ntohl(gateway->bandwidth_down);
	gw_node->bandwidth_up = ntohl(gateway->bandwidth_up);

	kref_get(&gw_node->refcount);
	hlist_add_head_rcu(&gw_node->list, &bat_priv->gw.list);

	batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
		   "Found new gateway %pM -> gw bandwidth: %u.%u/%u.%u MBit\n",
		   orig_node->orig,
		   ntohl(gateway->bandwidth_down) / 10,
		   ntohl(gateway->bandwidth_down) % 10,
		   ntohl(gateway->bandwidth_up) / 10,
		   ntohl(gateway->bandwidth_up) % 10);

	/* don't return reference to new gw_node */
	batadv_gw_node_put(gw_node);
}

/**
 * batadv_gw_node_get - retrieve gateway node from list of available gateways
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: originator announcing gateway capabilities
 *
 * Return: gateway node if found or NULL otherwise.
 */
struct batadv_gw_node *batadv_gw_node_get(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig_node)
{
	struct batadv_gw_node *gw_node_tmp, *gw_node = NULL;

	rcu_read_lock();
	hlist_for_each_entry_rcu(gw_node_tmp, &bat_priv->gw.list, list) {
		if (gw_node_tmp->orig_node != orig_node)
			continue;

		if (!kref_get_unless_zero(&gw_node_tmp->refcount))
			continue;

		gw_node = gw_node_tmp;
		break;
	}
	rcu_read_unlock();

	return gw_node;
}

/**
 * batadv_gw_node_update - update list of available gateways with changed
 *  bandwidth information
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: originator announcing gateway capabilities
 * @gateway: announced bandwidth information
 */
void batadv_gw_node_update(struct batadv_priv *bat_priv,
			   struct batadv_orig_node *orig_node,
			   struct batadv_tvlv_gateway_data *gateway)
{
	struct batadv_gw_node *gw_node, *curr_gw = NULL;

	spin_lock_bh(&bat_priv->gw.list_lock);
	gw_node = batadv_gw_node_get(bat_priv, orig_node);
	if (!gw_node) {
		batadv_gw_node_add(bat_priv, orig_node, gateway);
		spin_unlock_bh(&bat_priv->gw.list_lock);
		goto out;
	}
	spin_unlock_bh(&bat_priv->gw.list_lock);

	if ((gw_node->bandwidth_down == ntohl(gateway->bandwidth_down)) &&
	    (gw_node->bandwidth_up == ntohl(gateway->bandwidth_up)))
		goto out;

	batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
		   "Gateway bandwidth of originator %pM changed from %u.%u/%u.%u MBit to %u.%u/%u.%u MBit\n",
		   orig_node->orig,
		   gw_node->bandwidth_down / 10,
		   gw_node->bandwidth_down % 10,
		   gw_node->bandwidth_up / 10,
		   gw_node->bandwidth_up % 10,
		   ntohl(gateway->bandwidth_down) / 10,
		   ntohl(gateway->bandwidth_down) % 10,
		   ntohl(gateway->bandwidth_up) / 10,
		   ntohl(gateway->bandwidth_up) % 10);

	gw_node->bandwidth_down = ntohl(gateway->bandwidth_down);
	gw_node->bandwidth_up = ntohl(gateway->bandwidth_up);

	if (ntohl(gateway->bandwidth_down) == 0) {
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "Gateway %pM removed from gateway list\n",
			   orig_node->orig);

		/* Note: We don't need a NULL check here, since curr_gw never
		 * gets dereferenced.
		 */
		spin_lock_bh(&bat_priv->gw.list_lock);
		if (!hlist_unhashed(&gw_node->list)) {
			hlist_del_init_rcu(&gw_node->list);
			batadv_gw_node_put(gw_node);
		}
		spin_unlock_bh(&bat_priv->gw.list_lock);

		curr_gw = batadv_gw_get_selected_gw_node(bat_priv);
		if (gw_node == curr_gw)
			batadv_gw_reselect(bat_priv);

		if (curr_gw)
			batadv_gw_node_put(curr_gw);
	}

out:
	if (gw_node)
		batadv_gw_node_put(gw_node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void batadv_gw_node_delete(struct batadv_priv *bat_priv,
			   struct batadv_orig_node *orig_node)
{
<<<<<<< HEAD
	batadv_gw_node_update(bat_priv, orig_node, 0);
}

void batadv_gw_node_purge(struct batadv_priv *bat_priv)
{
	struct batadv_gw_node *gw_node, *curr_gw;
	struct hlist_node *node_tmp;
	unsigned long timeout = msecs_to_jiffies(2 * BATADV_PURGE_TIMEOUT);
	int do_deselect = 0;

	curr_gw = batadv_gw_get_selected_gw_node(bat_priv);

	spin_lock_bh(&bat_priv->gw.list_lock);

	hlist_for_each_entry_safe(gw_node, node_tmp,
				  &bat_priv->gw.list, list) {
		if (((!gw_node->deleted) ||
		     (time_before(jiffies, gw_node->deleted + timeout))) &&
		    atomic_read(&bat_priv->mesh_state) == BATADV_MESH_ACTIVE)
			continue;

		if (curr_gw == gw_node)
			do_deselect = 1;

		hlist_del_rcu(&gw_node->list);
		batadv_gw_node_free_ref(gw_node);
	}

	spin_unlock_bh(&bat_priv->gw.list_lock);

	/* gw_deselect() needs to acquire the gw_list_lock */
	if (do_deselect)
		batadv_gw_deselect(bat_priv);

	if (curr_gw)
		batadv_gw_node_free_ref(curr_gw);
}

/* fails if orig_node has no router */
static int batadv_write_buffer_text(struct batadv_priv *bat_priv,
				    struct seq_file *seq,
				    const struct batadv_gw_node *gw_node)
{
	struct batadv_gw_node *curr_gw;
	struct batadv_neigh_node *router;
	int down, up, ret = -1;

	batadv_gw_bandwidth_to_kbit(gw_node->orig_node->gw_flags, &down, &up);

	router = batadv_orig_node_get_router(gw_node->orig_node);
	if (!router)
		goto out;

	curr_gw = batadv_gw_get_selected_gw_node(bat_priv);

	ret = seq_printf(seq, "%s %pM (%3i) %pM [%10s]: %3i - %i%s/%i%s\n",
			 (curr_gw == gw_node ? "=>" : "  "),
			 gw_node->orig_node->orig,
			 router->tq_avg, router->addr,
			 router->if_incoming->net_dev->name,
			 gw_node->orig_node->gw_flags,
			 (down > 2048 ? down / 1024 : down),
			 (down > 2048 ? "MBit" : "KBit"),
			 (up > 2048 ? up / 1024 : up),
			 (up > 2048 ? "MBit" : "KBit"));

	batadv_neigh_node_free_ref(router);
	if (curr_gw)
		batadv_gw_node_free_ref(curr_gw);
out:
	return ret;
}

=======
	struct batadv_tvlv_gateway_data gateway;

	gateway.bandwidth_down = 0;
	gateway.bandwidth_up = 0;

	batadv_gw_node_update(bat_priv, orig_node, &gateway);
}

void batadv_gw_node_free(struct batadv_priv *bat_priv)
{
	struct batadv_gw_node *gw_node;
	struct hlist_node *node_tmp;

	spin_lock_bh(&bat_priv->gw.list_lock);
	hlist_for_each_entry_safe(gw_node, node_tmp,
				  &bat_priv->gw.list, list) {
		hlist_del_init_rcu(&gw_node->list);
		batadv_gw_node_put(gw_node);
	}
	spin_unlock_bh(&bat_priv->gw.list_lock);
}

#ifdef CONFIG_BATMAN_ADV_DEBUGFS
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int batadv_gw_client_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct batadv_priv *bat_priv = netdev_priv(net_dev);
	struct batadv_hard_iface *primary_if;
<<<<<<< HEAD
	struct batadv_gw_node *gw_node;
	int gw_count = 0;

	primary_if = batadv_seq_print_text_primary_if_get(seq);
	if (!primary_if)
		goto out;

	seq_printf(seq,
		   "      %-12s (%s/%i) %17s [%10s]: gw_class ... [B.A.T.M.A.N. adv %s, MainIF/MAC: %s/%pM (%s)]\n",
		   "Gateway", "#", BATADV_TQ_MAX_VALUE, "Nexthop", "outgoingIF",
		   BATADV_SOURCE_VERSION, primary_if->net_dev->name,
		   primary_if->net_dev->dev_addr, net_dev->name);

	rcu_read_lock();
	hlist_for_each_entry_rcu(gw_node, &bat_priv->gw.list, list) {
		if (gw_node->deleted)
			continue;

		/* fails if orig_node has no router */
		if (batadv_write_buffer_text(bat_priv, seq, gw_node) < 0)
			continue;

		gw_count++;
	}
	rcu_read_unlock();

	if (gw_count == 0)
		seq_puts(seq, "No gateways in range ...\n");

out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	return 0;
}

static bool batadv_is_type_dhcprequest(struct sk_buff *skb, int header_len)
{
	int ret = false;
	unsigned char *p;
	int pkt_len;

	if (skb_linearize(skb) < 0)
		goto out;

	pkt_len = skb_headlen(skb);

	if (pkt_len < header_len + BATADV_DHCP_OPTIONS_OFFSET + 1)
		goto out;

	p = skb->data + header_len + BATADV_DHCP_OPTIONS_OFFSET;
	pkt_len -= header_len + BATADV_DHCP_OPTIONS_OFFSET + 1;

	/* Access the dhcp option lists. Each entry is made up by:
	 * - octet 1: option type
	 * - octet 2: option data len (only if type != 255 and 0)
	 * - octet 3: option data
	 */
	while (*p != 255 && !ret) {
		/* p now points to the first octet: option type */
		if (*p == 53) {
			/* type 53 is the message type option.
			 * Jump the len octet and go to the data octet
			 */
			if (pkt_len < 2)
				goto out;
			p += 2;

			/* check if the message type is what we need */
			if (*p == BATADV_DHCP_REQUEST)
				ret = true;
			break;
		} else if (*p == 0) {
			/* option type 0 (padding), just go forward */
			if (pkt_len < 1)
				goto out;
			pkt_len--;
			p++;
		} else {
			/* This is any other option. So we get the length... */
			if (pkt_len < 1)
				goto out;
			pkt_len--;
			p++;

			/* ...and then we jump over the data */
			if (pkt_len < 1 + (*p))
				goto out;
			pkt_len -= 1 + (*p);
			p += 1 + (*p);
		}
	}
out:
	return ret;
}

bool batadv_gw_is_dhcp_target(struct sk_buff *skb, unsigned int *header_len)
{
=======

	primary_if = batadv_seq_print_text_primary_if_get(seq);
	if (!primary_if)
		return 0;

	seq_printf(seq, "[B.A.T.M.A.N. adv %s, MainIF/MAC: %s/%pM (%s %s)]\n",
		   BATADV_SOURCE_VERSION, primary_if->net_dev->name,
		   primary_if->net_dev->dev_addr, net_dev->name,
		   bat_priv->algo_ops->name);

	batadv_hardif_put(primary_if);

	if (!bat_priv->algo_ops->gw.print) {
		seq_puts(seq,
			 "No printing function for this routing protocol\n");
		return 0;
	}

	bat_priv->algo_ops->gw.print(bat_priv, seq);

	return 0;
}
#endif

/**
 * batadv_gw_dump - Dump gateways into a message
 * @msg: Netlink message to dump into
 * @cb: Control block containing additional options
 *
 * Return: Error code, or length of message
 */
int batadv_gw_dump(struct sk_buff *msg, struct netlink_callback *cb)
{
	struct batadv_hard_iface *primary_if = NULL;
	struct net *net = sock_net(cb->skb->sk);
	struct net_device *soft_iface;
	struct batadv_priv *bat_priv;
	int ifindex;
	int ret;

	ifindex = batadv_netlink_get_ifindex(cb->nlh,
					     BATADV_ATTR_MESH_IFINDEX);
	if (!ifindex)
		return -EINVAL;

	soft_iface = dev_get_by_index(net, ifindex);
	if (!soft_iface || !batadv_softif_is_valid(soft_iface)) {
		ret = -ENODEV;
		goto out;
	}

	bat_priv = netdev_priv(soft_iface);

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if || primary_if->if_status != BATADV_IF_ACTIVE) {
		ret = -ENOENT;
		goto out;
	}

	if (!bat_priv->algo_ops->gw.dump) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	bat_priv->algo_ops->gw.dump(msg, cb, bat_priv);

	ret = msg->len;

out:
	if (primary_if)
		batadv_hardif_put(primary_if);
	if (soft_iface)
		dev_put(soft_iface);

	return ret;
}

/**
 * batadv_gw_dhcp_recipient_get - check if a packet is a DHCP message
 * @skb: the packet to check
 * @header_len: a pointer to the batman-adv header size
 * @chaddr: buffer where the client address will be stored. Valid
 *  only if the function returns BATADV_DHCP_TO_CLIENT
 *
 * This function may re-allocate the data buffer of the skb passed as argument.
 *
 * Return:
 * - BATADV_DHCP_NO if the packet is not a dhcp message or if there was an error
 *   while parsing it
 * - BATADV_DHCP_TO_SERVER if this is a message going to the DHCP server
 * - BATADV_DHCP_TO_CLIENT if this is a message going to a DHCP client
 */
enum batadv_dhcp_recipient
batadv_gw_dhcp_recipient_get(struct sk_buff *skb, unsigned int *header_len,
			     u8 *chaddr)
{
	enum batadv_dhcp_recipient ret = BATADV_DHCP_NO;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct ethhdr *ethhdr;
	struct iphdr *iphdr;
	struct ipv6hdr *ipv6hdr;
	struct udphdr *udphdr;
<<<<<<< HEAD

	/* check for ethernet header */
	if (!pskb_may_pull(skb, *header_len + ETH_HLEN))
		return false;
	ethhdr = (struct ethhdr *)skb->data;
	*header_len += ETH_HLEN;

	/* check for initial vlan header */
	if (ntohs(ethhdr->h_proto) == ETH_P_8021Q) {
		if (!pskb_may_pull(skb, *header_len + VLAN_HLEN))
			return false;
		ethhdr = (struct ethhdr *)(skb->data + VLAN_HLEN);
=======
	struct vlan_ethhdr *vhdr;
	int chaddr_offset;
	__be16 proto;
	u8 *p;

	/* check for ethernet header */
	if (!pskb_may_pull(skb, *header_len + ETH_HLEN))
		return BATADV_DHCP_NO;

	ethhdr = eth_hdr(skb);
	proto = ethhdr->h_proto;
	*header_len += ETH_HLEN;

	/* check for initial vlan header */
	if (proto == htons(ETH_P_8021Q)) {
		if (!pskb_may_pull(skb, *header_len + VLAN_HLEN))
			return BATADV_DHCP_NO;

		vhdr = vlan_eth_hdr(skb);
		proto = vhdr->h_vlan_encapsulated_proto;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		*header_len += VLAN_HLEN;
	}

	/* check for ip header */
<<<<<<< HEAD
	switch (ntohs(ethhdr->h_proto)) {
	case ETH_P_IP:
		if (!pskb_may_pull(skb, *header_len + sizeof(*iphdr)))
			return false;
=======
	switch (proto) {
	case htons(ETH_P_IP):
		if (!pskb_may_pull(skb, *header_len + sizeof(*iphdr)))
			return BATADV_DHCP_NO;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		iphdr = (struct iphdr *)(skb->data + *header_len);
		*header_len += iphdr->ihl * 4;

		/* check for udp header */
		if (iphdr->protocol != IPPROTO_UDP)
<<<<<<< HEAD
			return false;

		break;
	case ETH_P_IPV6:
		if (!pskb_may_pull(skb, *header_len + sizeof(*ipv6hdr)))
			return false;
=======
			return BATADV_DHCP_NO;

		break;
	case htons(ETH_P_IPV6):
		if (!pskb_may_pull(skb, *header_len + sizeof(*ipv6hdr)))
			return BATADV_DHCP_NO;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ipv6hdr = (struct ipv6hdr *)(skb->data + *header_len);
		*header_len += sizeof(*ipv6hdr);

		/* check for udp header */
		if (ipv6hdr->nexthdr != IPPROTO_UDP)
<<<<<<< HEAD
			return false;

		break;
	default:
		return false;
	}

	if (!pskb_may_pull(skb, *header_len + sizeof(*udphdr)))
		return false;
=======
			return BATADV_DHCP_NO;

		break;
	default:
		return BATADV_DHCP_NO;
	}

	if (!pskb_may_pull(skb, *header_len + sizeof(*udphdr)))
		return BATADV_DHCP_NO;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	udphdr = (struct udphdr *)(skb->data + *header_len);
	*header_len += sizeof(*udphdr);

	/* check for bootp port */
<<<<<<< HEAD
	if ((ntohs(ethhdr->h_proto) == ETH_P_IP) &&
	    (ntohs(udphdr->dest) != 67))
		return false;

	if ((ntohs(ethhdr->h_proto) == ETH_P_IPV6) &&
	    (ntohs(udphdr->dest) != 547))
		return false;

	return true;
}

bool batadv_gw_out_of_range(struct batadv_priv *bat_priv,
			    struct sk_buff *skb, struct ethhdr *ethhdr)
{
	struct batadv_neigh_node *neigh_curr = NULL, *neigh_old = NULL;
	struct batadv_orig_node *orig_dst_node = NULL;
	struct batadv_gw_node *curr_gw = NULL;
	bool ret, out_of_range = false;
	unsigned int header_len = 0;
	uint8_t curr_tq_avg;

	ret = batadv_gw_is_dhcp_target(skb, &header_len);
	if (!ret)
		goto out;

	orig_dst_node = batadv_transtable_search(bat_priv, ethhdr->h_source,
						 ethhdr->h_dest);
	if (!orig_dst_node)
		goto out;

	if (!orig_dst_node->gw_flags)
		goto out;

	ret = batadv_is_type_dhcprequest(skb, header_len);
	if (!ret)
		goto out;

	switch (atomic_read(&bat_priv->gw_mode)) {
=======
	switch (proto) {
	case htons(ETH_P_IP):
		if (udphdr->dest == htons(67))
			ret = BATADV_DHCP_TO_SERVER;
		else if (udphdr->source == htons(67))
			ret = BATADV_DHCP_TO_CLIENT;
		break;
	case htons(ETH_P_IPV6):
		if (udphdr->dest == htons(547))
			ret = BATADV_DHCP_TO_SERVER;
		else if (udphdr->source == htons(547))
			ret = BATADV_DHCP_TO_CLIENT;
		break;
	}

	chaddr_offset = *header_len + BATADV_DHCP_CHADDR_OFFSET;
	/* store the client address if the message is going to a client */
	if (ret == BATADV_DHCP_TO_CLIENT) {
		if (!pskb_may_pull(skb, chaddr_offset + ETH_ALEN))
			return BATADV_DHCP_NO;

		/* check if the DHCP packet carries an Ethernet DHCP */
		p = skb->data + *header_len + BATADV_DHCP_HTYPE_OFFSET;
		if (*p != BATADV_DHCP_HTYPE_ETHERNET)
			return BATADV_DHCP_NO;

		/* check if the DHCP packet carries a valid Ethernet address */
		p = skb->data + *header_len + BATADV_DHCP_HLEN_OFFSET;
		if (*p != ETH_ALEN)
			return BATADV_DHCP_NO;

		ether_addr_copy(chaddr, skb->data + chaddr_offset);
	}

	return ret;
}

/**
 * batadv_gw_out_of_range - check if the dhcp request destination is the best gw
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the outgoing packet
 *
 * Check if the skb is a DHCP request and if it is sent to the current best GW
 * server. Due to topology changes it may be the case that the GW server
 * previously selected is not the best one anymore.
 *
 * This call might reallocate skb data.
 * Must be invoked only when the DHCP packet is going TO a DHCP SERVER.
 *
 * Return: true if the packet destination is unicast and it is not the best gw,
 * false otherwise.
 */
bool batadv_gw_out_of_range(struct batadv_priv *bat_priv,
			    struct sk_buff *skb)
{
	struct batadv_neigh_node *neigh_curr = NULL;
	struct batadv_neigh_node *neigh_old = NULL;
	struct batadv_orig_node *orig_dst_node = NULL;
	struct batadv_gw_node *gw_node = NULL;
	struct batadv_gw_node *curr_gw = NULL;
	struct batadv_neigh_ifinfo *curr_ifinfo, *old_ifinfo;
	struct ethhdr *ethhdr = (struct ethhdr *)skb->data;
	bool out_of_range = false;
	u8 curr_tq_avg;
	unsigned short vid;

	vid = batadv_get_vid(skb, 0);

	if (is_multicast_ether_addr(ethhdr->h_dest))
		goto out;

	orig_dst_node = batadv_transtable_search(bat_priv, ethhdr->h_source,
						 ethhdr->h_dest, vid);
	if (!orig_dst_node)
		goto out;

	gw_node = batadv_gw_node_get(bat_priv, orig_dst_node);
	if (!gw_node)
		goto out;

	switch (atomic_read(&bat_priv->gw.mode)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case BATADV_GW_MODE_SERVER:
		/* If we are a GW then we are our best GW. We can artificially
		 * set the tq towards ourself as the maximum value
		 */
		curr_tq_avg = BATADV_TQ_MAX_VALUE;
		break;
	case BATADV_GW_MODE_CLIENT:
		curr_gw = batadv_gw_get_selected_gw_node(bat_priv);
		if (!curr_gw)
			goto out;

		/* packet is going to our gateway */
		if (curr_gw->orig_node == orig_dst_node)
			goto out;

		/* If the dhcp packet has been sent to a different gw,
		 * we have to evaluate whether the old gw is still
		 * reliable enough
		 */
		neigh_curr = batadv_find_router(bat_priv, curr_gw->orig_node,
						NULL);
		if (!neigh_curr)
			goto out;

<<<<<<< HEAD
		curr_tq_avg = neigh_curr->tq_avg;
=======
		curr_ifinfo = batadv_neigh_ifinfo_get(neigh_curr,
						      BATADV_IF_DEFAULT);
		if (!curr_ifinfo)
			goto out;

		curr_tq_avg = curr_ifinfo->bat_iv.tq_avg;
		batadv_neigh_ifinfo_put(curr_ifinfo);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	case BATADV_GW_MODE_OFF:
	default:
		goto out;
	}

	neigh_old = batadv_find_router(bat_priv, orig_dst_node, NULL);
	if (!neigh_old)
		goto out;

<<<<<<< HEAD
	if (curr_tq_avg - neigh_old->tq_avg > BATADV_GW_THRESHOLD)
		out_of_range = true;

out:
	if (orig_dst_node)
		batadv_orig_node_free_ref(orig_dst_node);
	if (curr_gw)
		batadv_gw_node_free_ref(curr_gw);
	if (neigh_old)
		batadv_neigh_node_free_ref(neigh_old);
	if (neigh_curr)
		batadv_neigh_node_free_ref(neigh_curr);
=======
	old_ifinfo = batadv_neigh_ifinfo_get(neigh_old, BATADV_IF_DEFAULT);
	if (!old_ifinfo)
		goto out;

	if ((curr_tq_avg - old_ifinfo->bat_iv.tq_avg) > BATADV_GW_THRESHOLD)
		out_of_range = true;
	batadv_neigh_ifinfo_put(old_ifinfo);

out:
	if (orig_dst_node)
		batadv_orig_node_put(orig_dst_node);
	if (curr_gw)
		batadv_gw_node_put(curr_gw);
	if (gw_node)
		batadv_gw_node_put(gw_node);
	if (neigh_old)
		batadv_neigh_node_put(neigh_old);
	if (neigh_curr)
		batadv_neigh_node_put(neigh_curr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return out_of_range;
}
