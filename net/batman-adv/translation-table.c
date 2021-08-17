<<<<<<< HEAD
/* Copyright (C) 2007-2013 B.A.T.M.A.N. contributors:
=======
/* Copyright (C) 2007-2016  B.A.T.M.A.N. contributors:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Marek Lindner, Simon Wunderlich, Antonio Quartulli
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
#include "translation-table.h"
#include "soft-interface.h"
#include "hard-interface.h"
#include "send.h"
#include "hash.h"
#include "originator.h"
#include "routing.h"
#include "bridge_loop_avoidance.h"

#include <linux/crc16.h>
=======
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "translation-table.h"
#include "main.h"

#include <linux/atomic.h>
#include <linux/bitops.h>
#include <linux/bug.h>
#include <linux/byteorder/generic.h>
#include <linux/cache.h>
#include <linux/compiler.h>
#include <linux/crc32c.h>
#include <linux/errno.h>
#include <linux/etherdevice.h>
#include <linux/fs.h>
#include <linux/if_ether.h>
#include <linux/init.h>
#include <linux/jhash.h>
#include <linux/jiffies.h>
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
#include <linux/string.h>
#include <linux/workqueue.h>
#include <net/genetlink.h>
#include <net/netlink.h>
#include <net/sock.h>
#include <uapi/linux/batman_adv.h>

#include "bridge_loop_avoidance.h"
#include "hard-interface.h"
#include "hash.h"
#include "log.h"
#include "multicast.h"
#include "netlink.h"
#include "originator.h"
#include "packet.h"
#include "soft-interface.h"
#include "tvlv.h"

static struct kmem_cache *batadv_tl_cache __read_mostly;
static struct kmem_cache *batadv_tg_cache __read_mostly;
static struct kmem_cache *batadv_tt_orig_cache __read_mostly;
static struct kmem_cache *batadv_tt_change_cache __read_mostly;
static struct kmem_cache *batadv_tt_req_cache __read_mostly;
static struct kmem_cache *batadv_tt_roam_cache __read_mostly;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* hash class keys */
static struct lock_class_key batadv_tt_local_hash_lock_class_key;
static struct lock_class_key batadv_tt_global_hash_lock_class_key;

<<<<<<< HEAD
static void batadv_send_roam_adv(struct batadv_priv *bat_priv, uint8_t *client,
=======
static void batadv_send_roam_adv(struct batadv_priv *bat_priv, u8 *client,
				 unsigned short vid,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				 struct batadv_orig_node *orig_node);
static void batadv_tt_purge(struct work_struct *work);
static void
batadv_tt_global_del_orig_list(struct batadv_tt_global_entry *tt_global_entry);
static void batadv_tt_global_del(struct batadv_priv *bat_priv,
				 struct batadv_orig_node *orig_node,
				 const unsigned char *addr,
<<<<<<< HEAD
				 const char *message, bool roaming);

/* returns 1 if they are the same mac addr */
static int batadv_compare_tt(const struct hlist_node *node, const void *data2)
{
	const void *data1 = container_of(node, struct batadv_tt_common_entry,
					 hash_entry);

	return (memcmp(data1, data2, ETH_ALEN) == 0 ? 1 : 0);
}

static struct batadv_tt_common_entry *
batadv_tt_hash_find(struct batadv_hashtable *hash, const void *data)
{
	struct hlist_head *head;
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_common_entry *tt_common_entry_tmp = NULL;
	uint32_t index;
=======
				 unsigned short vid, const char *message,
				 bool roaming);

/**
 * batadv_compare_tt - check if two TT entries are the same
 * @node: the list element pointer of the first TT entry
 * @data2: pointer to the tt_common_entry of the second TT entry
 *
 * Compare the MAC address and the VLAN ID of the two TT entries and check if
 * they are the same TT client.
 * Return: true if the two TT clients are the same, false otherwise
 */
static bool batadv_compare_tt(const struct hlist_node *node, const void *data2)
{
	const void *data1 = container_of(node, struct batadv_tt_common_entry,
					 hash_entry);
	const struct batadv_tt_common_entry *tt1 = data1;
	const struct batadv_tt_common_entry *tt2 = data2;

	return (tt1->vid == tt2->vid) && batadv_compare_eth(data1, data2);
}

/**
 * batadv_choose_tt - return the index of the tt entry in the hash table
 * @data: pointer to the tt_common_entry object to map
 * @size: the size of the hash table
 *
 * Return: the hash index where the object represented by 'data' should be
 * stored at.
 */
static inline u32 batadv_choose_tt(const void *data, u32 size)
{
	struct batadv_tt_common_entry *tt;
	u32 hash = 0;

	tt = (struct batadv_tt_common_entry *)data;
	hash = jhash(&tt->addr, ETH_ALEN, hash);
	hash = jhash(&tt->vid, sizeof(tt->vid), hash);

	return hash % size;
}

/**
 * batadv_tt_hash_find - look for a client in the given hash table
 * @hash: the hash table to search
 * @addr: the mac address of the client to look for
 * @vid: VLAN identifier
 *
 * Return: a pointer to the tt_common struct belonging to the searched client if
 * found, NULL otherwise.
 */
static struct batadv_tt_common_entry *
batadv_tt_hash_find(struct batadv_hashtable *hash, const u8 *addr,
		    unsigned short vid)
{
	struct hlist_head *head;
	struct batadv_tt_common_entry to_search, *tt, *tt_tmp = NULL;
	u32 index;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!hash)
		return NULL;

<<<<<<< HEAD
	index = batadv_choose_orig(data, hash->size);
	head = &hash->table[index];

	rcu_read_lock();
	hlist_for_each_entry_rcu(tt_common_entry, head, hash_entry) {
		if (!batadv_compare_eth(tt_common_entry, data))
			continue;

		if (!atomic_inc_not_zero(&tt_common_entry->refcount))
			continue;

		tt_common_entry_tmp = tt_common_entry;
=======
	ether_addr_copy(to_search.addr, addr);
	to_search.vid = vid;

	index = batadv_choose_tt(&to_search, hash->size);
	head = &hash->table[index];

	rcu_read_lock();
	hlist_for_each_entry_rcu(tt, head, hash_entry) {
		if (!batadv_compare_eth(tt, addr))
			continue;

		if (tt->vid != vid)
			continue;

		if (!kref_get_unless_zero(&tt->refcount))
			continue;

		tt_tmp = tt;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	}
	rcu_read_unlock();

<<<<<<< HEAD
	return tt_common_entry_tmp;
}

static struct batadv_tt_local_entry *
batadv_tt_local_hash_find(struct batadv_priv *bat_priv, const void *data)
=======
	return tt_tmp;
}

/**
 * batadv_tt_local_hash_find - search the local table for a given client
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client to look for
 * @vid: VLAN identifier
 *
 * Return: a pointer to the corresponding tt_local_entry struct if the client is
 * found, NULL otherwise.
 */
static struct batadv_tt_local_entry *
batadv_tt_local_hash_find(struct batadv_priv *bat_priv, const u8 *addr,
			  unsigned short vid)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_local_entry *tt_local_entry = NULL;

<<<<<<< HEAD
	tt_common_entry = batadv_tt_hash_find(bat_priv->tt.local_hash, data);
=======
	tt_common_entry = batadv_tt_hash_find(bat_priv->tt.local_hash, addr,
					      vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (tt_common_entry)
		tt_local_entry = container_of(tt_common_entry,
					      struct batadv_tt_local_entry,
					      common);
	return tt_local_entry;
}

<<<<<<< HEAD
static struct batadv_tt_global_entry *
batadv_tt_global_hash_find(struct batadv_priv *bat_priv, const void *data)
=======
/**
 * batadv_tt_global_hash_find - search the global table for a given client
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client to look for
 * @vid: VLAN identifier
 *
 * Return: a pointer to the corresponding tt_global_entry struct if the client
 * is found, NULL otherwise.
 */
static struct batadv_tt_global_entry *
batadv_tt_global_hash_find(struct batadv_priv *bat_priv, const u8 *addr,
			   unsigned short vid)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_global_entry *tt_global_entry = NULL;

<<<<<<< HEAD
	tt_common_entry = batadv_tt_hash_find(bat_priv->tt.global_hash, data);
=======
	tt_common_entry = batadv_tt_hash_find(bat_priv->tt.global_hash, addr,
					      vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (tt_common_entry)
		tt_global_entry = container_of(tt_common_entry,
					       struct batadv_tt_global_entry,
					       common);
	return tt_global_entry;
}

<<<<<<< HEAD
static void
batadv_tt_local_entry_free_ref(struct batadv_tt_local_entry *tt_local_entry)
{
	if (atomic_dec_and_test(&tt_local_entry->common.refcount))
		kfree_rcu(tt_local_entry, common.rcu);
}

static void batadv_tt_global_entry_free_rcu(struct rcu_head *rcu)
{
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_global_entry *tt_global_entry;

	tt_common_entry = container_of(rcu, struct batadv_tt_common_entry, rcu);
	tt_global_entry = container_of(tt_common_entry,
				       struct batadv_tt_global_entry, common);

	kfree(tt_global_entry);
}

static void
batadv_tt_global_entry_free_ref(struct batadv_tt_global_entry *tt_global_entry)
{
	if (atomic_dec_and_test(&tt_global_entry->common.refcount)) {
		batadv_tt_global_del_orig_list(tt_global_entry);
		call_rcu(&tt_global_entry->common.rcu,
			 batadv_tt_global_entry_free_rcu);
	}
}

=======
/**
 * batadv_tt_local_entry_free_rcu - free the tt_local_entry
 * @rcu: rcu pointer of the tt_local_entry
 */
static void batadv_tt_local_entry_free_rcu(struct rcu_head *rcu)
{
	struct batadv_tt_local_entry *tt_local_entry;

	tt_local_entry = container_of(rcu, struct batadv_tt_local_entry,
				      common.rcu);

	kmem_cache_free(batadv_tl_cache, tt_local_entry);
}

/**
 * batadv_tt_local_entry_release - release tt_local_entry from lists and queue
 *  for free after rcu grace period
 * @ref: kref pointer of the nc_node
 */
static void batadv_tt_local_entry_release(struct kref *ref)
{
	struct batadv_tt_local_entry *tt_local_entry;

	tt_local_entry = container_of(ref, struct batadv_tt_local_entry,
				      common.refcount);

	batadv_softif_vlan_put(tt_local_entry->vlan);

	call_rcu(&tt_local_entry->common.rcu, batadv_tt_local_entry_free_rcu);
}

/**
 * batadv_tt_local_entry_put - decrement the tt_local_entry refcounter and
 *  possibly release it
 * @tt_local_entry: tt_local_entry to be free'd
 */
static void
batadv_tt_local_entry_put(struct batadv_tt_local_entry *tt_local_entry)
{
	kref_put(&tt_local_entry->common.refcount,
		 batadv_tt_local_entry_release);
}

/**
 * batadv_tt_global_entry_free_rcu - free the tt_global_entry
 * @rcu: rcu pointer of the tt_global_entry
 */
static void batadv_tt_global_entry_free_rcu(struct rcu_head *rcu)
{
	struct batadv_tt_global_entry *tt_global_entry;

	tt_global_entry = container_of(rcu, struct batadv_tt_global_entry,
				       common.rcu);

	kmem_cache_free(batadv_tg_cache, tt_global_entry);
}

/**
 * batadv_tt_global_entry_release - release tt_global_entry from lists and queue
 *  for free after rcu grace period
 * @ref: kref pointer of the nc_node
 */
static void batadv_tt_global_entry_release(struct kref *ref)
{
	struct batadv_tt_global_entry *tt_global_entry;

	tt_global_entry = container_of(ref, struct batadv_tt_global_entry,
				       common.refcount);

	batadv_tt_global_del_orig_list(tt_global_entry);

	call_rcu(&tt_global_entry->common.rcu, batadv_tt_global_entry_free_rcu);
}

/**
 * batadv_tt_global_entry_put - decrement the tt_global_entry refcounter and
 *  possibly release it
 * @tt_global_entry: tt_global_entry to be free'd
 */
static void
batadv_tt_global_entry_put(struct batadv_tt_global_entry *tt_global_entry)
{
	kref_put(&tt_global_entry->common.refcount,
		 batadv_tt_global_entry_release);
}

/**
 * batadv_tt_global_hash_count - count the number of orig entries
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client to count entries for
 * @vid: VLAN identifier
 *
 * Return: the number of originators advertising the given address/data
 * (excluding ourself).
 */
int batadv_tt_global_hash_count(struct batadv_priv *bat_priv,
				const u8 *addr, unsigned short vid)
{
	struct batadv_tt_global_entry *tt_global_entry;
	int count;

	tt_global_entry = batadv_tt_global_hash_find(bat_priv, addr, vid);
	if (!tt_global_entry)
		return 0;

	count = atomic_read(&tt_global_entry->orig_list_count);
	batadv_tt_global_entry_put(tt_global_entry);

	return count;
}

/**
 * batadv_tt_local_size_mod - change the size by v of the local table identified
 *  by vid
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the VLAN identifier of the sub-table to change
 * @v: the amount to sum to the local table size
 */
static void batadv_tt_local_size_mod(struct batadv_priv *bat_priv,
				     unsigned short vid, int v)
{
	struct batadv_softif_vlan *vlan;

	vlan = batadv_softif_vlan_get(bat_priv, vid);
	if (!vlan)
		return;

	atomic_add(v, &vlan->tt.num_entries);

	batadv_softif_vlan_put(vlan);
}

/**
 * batadv_tt_local_size_inc - increase by one the local table size for the given
 *  vid
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the VLAN identifier
 */
static void batadv_tt_local_size_inc(struct batadv_priv *bat_priv,
				     unsigned short vid)
{
	batadv_tt_local_size_mod(bat_priv, vid, 1);
}

/**
 * batadv_tt_local_size_dec - decrease by one the local table size for the given
 *  vid
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the VLAN identifier
 */
static void batadv_tt_local_size_dec(struct batadv_priv *bat_priv,
				     unsigned short vid)
{
	batadv_tt_local_size_mod(bat_priv, vid, -1);
}

/**
 * batadv_tt_global_size_mod - change the size by v of the global table
 *  for orig_node identified by vid
 * @orig_node: the originator for which the table has to be modified
 * @vid: the VLAN identifier
 * @v: the amount to sum to the global table size
 */
static void batadv_tt_global_size_mod(struct batadv_orig_node *orig_node,
				      unsigned short vid, int v)
{
	struct batadv_orig_node_vlan *vlan;

	vlan = batadv_orig_node_vlan_new(orig_node, vid);
	if (!vlan)
		return;

	if (atomic_add_return(v, &vlan->tt.num_entries) == 0) {
		spin_lock_bh(&orig_node->vlan_list_lock);
		if (!hlist_unhashed(&vlan->list)) {
			hlist_del_init_rcu(&vlan->list);
			batadv_orig_node_vlan_put(vlan);
		}
		spin_unlock_bh(&orig_node->vlan_list_lock);
	}

	batadv_orig_node_vlan_put(vlan);
}

/**
 * batadv_tt_global_size_inc - increase by one the global table size for the
 *  given vid
 * @orig_node: the originator which global table size has to be decreased
 * @vid: the vlan identifier
 */
static void batadv_tt_global_size_inc(struct batadv_orig_node *orig_node,
				      unsigned short vid)
{
	batadv_tt_global_size_mod(orig_node, vid, 1);
}

/**
 * batadv_tt_global_size_dec - decrease by one the global table size for the
 *  given vid
 * @orig_node: the originator which global table size has to be decreased
 * @vid: the vlan identifier
 */
static void batadv_tt_global_size_dec(struct batadv_orig_node *orig_node,
				      unsigned short vid)
{
	batadv_tt_global_size_mod(orig_node, vid, -1);
}

/**
 * batadv_tt_orig_list_entry_free_rcu - free the orig_entry
 * @rcu: rcu pointer of the orig_entry
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void batadv_tt_orig_list_entry_free_rcu(struct rcu_head *rcu)
{
	struct batadv_tt_orig_list_entry *orig_entry;

	orig_entry = container_of(rcu, struct batadv_tt_orig_list_entry, rcu);

<<<<<<< HEAD
	/* We are in an rcu callback here, therefore we cannot use
	 * batadv_orig_node_free_ref() and its call_rcu():
	 * An rcu_barrier() wouldn't wait for that to finish
	 */
	batadv_orig_node_free_ref_now(orig_entry->orig_node);
	kfree(orig_entry);
}

static void
batadv_tt_orig_list_entry_free_ref(struct batadv_tt_orig_list_entry *orig_entry)
{
	if (!atomic_dec_and_test(&orig_entry->refcount))
		return;
	/* to avoid race conditions, immediately decrease the tt counter */
	atomic_dec(&orig_entry->orig_node->tt_size);
	call_rcu(&orig_entry->rcu, batadv_tt_orig_list_entry_free_rcu);
}

static void batadv_tt_local_event(struct batadv_priv *bat_priv,
				  const uint8_t *addr, uint8_t flags)
{
	struct batadv_tt_change_node *tt_change_node, *entry, *safe;
	bool event_removed = false;
	bool del_op_requested, del_op_entry;

	tt_change_node = kmalloc(sizeof(*tt_change_node), GFP_ATOMIC);

=======
	kmem_cache_free(batadv_tt_orig_cache, orig_entry);
}

/**
 * batadv_tt_orig_list_entry_release - release tt orig entry from lists and
 *  queue for free after rcu grace period
 * @ref: kref pointer of the tt orig entry
 */
static void batadv_tt_orig_list_entry_release(struct kref *ref)
{
	struct batadv_tt_orig_list_entry *orig_entry;

	orig_entry = container_of(ref, struct batadv_tt_orig_list_entry,
				  refcount);

	batadv_orig_node_put(orig_entry->orig_node);
	call_rcu(&orig_entry->rcu, batadv_tt_orig_list_entry_free_rcu);
}

/**
 * batadv_tt_orig_list_entry_put - decrement the tt orig entry refcounter and
 *  possibly release it
 * @orig_entry: tt orig entry to be free'd
 */
static void
batadv_tt_orig_list_entry_put(struct batadv_tt_orig_list_entry *orig_entry)
{
	kref_put(&orig_entry->refcount, batadv_tt_orig_list_entry_release);
}

/**
 * batadv_tt_local_event - store a local TT event (ADD/DEL)
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_local_entry: the TT entry involved in the event
 * @event_flags: flags to store in the event structure
 */
static void batadv_tt_local_event(struct batadv_priv *bat_priv,
				  struct batadv_tt_local_entry *tt_local_entry,
				  u8 event_flags)
{
	struct batadv_tt_change_node *tt_change_node, *entry, *safe;
	struct batadv_tt_common_entry *common = &tt_local_entry->common;
	u8 flags = common->flags | event_flags;
	bool event_removed = false;
	bool del_op_requested, del_op_entry;

	tt_change_node = kmem_cache_alloc(batadv_tt_change_cache, GFP_ATOMIC);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!tt_change_node)
		return;

	tt_change_node->change.flags = flags;
<<<<<<< HEAD
	memcpy(tt_change_node->change.addr, addr, ETH_ALEN);
=======
	memset(tt_change_node->change.reserved, 0,
	       sizeof(tt_change_node->change.reserved));
	ether_addr_copy(tt_change_node->change.addr, common->addr);
	tt_change_node->change.vid = htons(common->vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	del_op_requested = flags & BATADV_TT_CLIENT_DEL;

	/* check for ADD+DEL or DEL+ADD events */
	spin_lock_bh(&bat_priv->tt.changes_list_lock);
	list_for_each_entry_safe(entry, safe, &bat_priv->tt.changes_list,
				 list) {
<<<<<<< HEAD
		if (!batadv_compare_eth(entry->change.addr, addr))
=======
		if (!batadv_compare_eth(entry->change.addr, common->addr))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;

		/* DEL+ADD in the same orig interval have no effect and can be
		 * removed to avoid silly behaviour on the receiver side. The
		 * other way around (ADD+DEL) can happen in case of roaming of
		 * a client still in the NEW state. Roaming of NEW clients is
		 * now possible due to automatically recognition of "temporary"
		 * clients
		 */
		del_op_entry = entry->change.flags & BATADV_TT_CLIENT_DEL;
		if (!del_op_requested && del_op_entry)
			goto del;
		if (del_op_requested && !del_op_entry)
			goto del;
<<<<<<< HEAD
		continue;
del:
		list_del(&entry->list);
		kfree(entry);
		kfree(tt_change_node);
=======

		/* this is a second add in the same originator interval. It
		 * means that flags have been changed: update them!
		 */
		if (!del_op_requested && !del_op_entry)
			entry->change.flags = flags;

		continue;
del:
		list_del(&entry->list);
		kmem_cache_free(batadv_tt_change_cache, entry);
		kmem_cache_free(batadv_tt_change_cache, tt_change_node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		event_removed = true;
		goto unlock;
	}

	/* track the change in the OGMinterval list */
	list_add_tail(&tt_change_node->list, &bat_priv->tt.changes_list);

unlock:
	spin_unlock_bh(&bat_priv->tt.changes_list_lock);

	if (event_removed)
		atomic_dec(&bat_priv->tt.local_changes);
	else
		atomic_inc(&bat_priv->tt.local_changes);
}

<<<<<<< HEAD
int batadv_tt_len(int changes_num)
{
	return changes_num * sizeof(struct batadv_tt_change);
=======
/**
 * batadv_tt_len - compute length in bytes of given number of tt changes
 * @changes_num: number of tt changes
 *
 * Return: computed length in bytes.
 */
static int batadv_tt_len(int changes_num)
{
	return changes_num * sizeof(struct batadv_tvlv_tt_change);
}

/**
 * batadv_tt_entries - compute the number of entries fitting in tt_len bytes
 * @tt_len: available space
 *
 * Return: the number of entries.
 */
static u16 batadv_tt_entries(u16 tt_len)
{
	return tt_len / batadv_tt_len(1);
}

/**
 * batadv_tt_local_table_transmit_size - calculates the local translation table
 *  size when transmitted over the air
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: local translation table size in bytes.
 */
static int batadv_tt_local_table_transmit_size(struct batadv_priv *bat_priv)
{
	u16 num_vlan = 0;
	u16 tt_local_entries = 0;
	struct batadv_softif_vlan *vlan;
	int hdr_size;

	rcu_read_lock();
	hlist_for_each_entry_rcu(vlan, &bat_priv->softif_vlan_list, list) {
		num_vlan++;
		tt_local_entries += atomic_read(&vlan->tt.num_entries);
	}
	rcu_read_unlock();

	/* header size of tvlv encapsulated tt response payload */
	hdr_size = sizeof(struct batadv_unicast_tvlv_packet);
	hdr_size += sizeof(struct batadv_tvlv_hdr);
	hdr_size += sizeof(struct batadv_tvlv_tt_data);
	hdr_size += num_vlan * sizeof(struct batadv_tvlv_tt_vlan_data);

	return hdr_size + batadv_tt_len(tt_local_entries);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int batadv_tt_local_init(struct batadv_priv *bat_priv)
{
	if (bat_priv->tt.local_hash)
		return 0;

	bat_priv->tt.local_hash = batadv_hash_new(1024);

	if (!bat_priv->tt.local_hash)
		return -ENOMEM;

	batadv_hash_set_lock_class(bat_priv->tt.local_hash,
				   &batadv_tt_local_hash_lock_class_key);

	return 0;
}

static void batadv_tt_global_free(struct batadv_priv *bat_priv,
				  struct batadv_tt_global_entry *tt_global,
				  const char *message)
{
<<<<<<< HEAD
	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Deleting global tt entry %pM: %s\n",
		   tt_global->common.addr, message);

	batadv_hash_remove(bat_priv->tt.global_hash, batadv_compare_tt,
			   batadv_choose_orig, tt_global->common.addr);
	batadv_tt_global_entry_free_ref(tt_global);
}

void batadv_tt_local_add(struct net_device *soft_iface, const uint8_t *addr,
			 int ifindex)
{
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	struct batadv_tt_local_entry *tt_local;
	struct batadv_tt_global_entry *tt_global;
	struct hlist_head *head;
	struct batadv_tt_orig_list_entry *orig_entry;
	int hash_added;
	bool roamed_back = false;

	tt_local = batadv_tt_local_hash_find(bat_priv, addr);
	tt_global = batadv_tt_global_hash_find(bat_priv, addr);
=======
	struct batadv_tt_global_entry *tt_removed_entry;
	struct hlist_node *tt_removed_node;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Deleting global tt entry %pM (vid: %d): %s\n",
		   tt_global->common.addr,
		   BATADV_PRINT_VID(tt_global->common.vid), message);

	tt_removed_node = batadv_hash_remove(bat_priv->tt.global_hash,
					     batadv_compare_tt,
					     batadv_choose_tt,
					     &tt_global->common);
	if (!tt_removed_node)
		return;

	/* drop reference of remove hash entry */
	tt_removed_entry = hlist_entry(tt_removed_node,
				       struct batadv_tt_global_entry,
				       common.hash_entry);
	batadv_tt_global_entry_put(tt_removed_entry);
}

/**
 * batadv_tt_local_add - add a new client to the local table or update an
 *  existing client
 * @soft_iface: netdev struct of the mesh interface
 * @addr: the mac address of the client to add
 * @vid: VLAN identifier
 * @ifindex: index of the interface where the client is connected to (useful to
 *  identify wireless clients)
 * @mark: the value contained in the skb->mark field of the received packet (if
 *  any)
 *
 * Return: true if the client was successfully added, false otherwise.
 */
bool batadv_tt_local_add(struct net_device *soft_iface, const u8 *addr,
			 unsigned short vid, int ifindex, u32 mark)
{
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	struct batadv_tt_local_entry *tt_local;
	struct batadv_tt_global_entry *tt_global = NULL;
	struct net *net = dev_net(soft_iface);
	struct batadv_softif_vlan *vlan;
	struct net_device *in_dev = NULL;
	struct hlist_head *head;
	struct batadv_tt_orig_list_entry *orig_entry;
	int hash_added, table_size, packet_size_max;
	bool ret = false;
	bool roamed_back = false;
	u8 remote_flags;
	u32 match_mark;

	if (ifindex != BATADV_NULL_IFINDEX)
		in_dev = dev_get_by_index(net, ifindex);

	tt_local = batadv_tt_local_hash_find(bat_priv, addr, vid);

	if (!is_multicast_ether_addr(addr))
		tt_global = batadv_tt_global_hash_find(bat_priv, addr, vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (tt_local) {
		tt_local->last_seen = jiffies;
		if (tt_local->common.flags & BATADV_TT_CLIENT_PENDING) {
			batadv_dbg(BATADV_DBG_TT, bat_priv,
<<<<<<< HEAD
				   "Re-adding pending client %pM\n", addr);
=======
				   "Re-adding pending client %pM (vid: %d)\n",
				   addr, BATADV_PRINT_VID(vid));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* whatever the reason why the PENDING flag was set,
			 * this is a client which was enqueued to be removed in
			 * this orig_interval. Since it popped up again, the
			 * flag can be reset like it was never enqueued
			 */
			tt_local->common.flags &= ~BATADV_TT_CLIENT_PENDING;
			goto add_event;
		}

		if (tt_local->common.flags & BATADV_TT_CLIENT_ROAM) {
			batadv_dbg(BATADV_DBG_TT, bat_priv,
<<<<<<< HEAD
				   "Roaming client %pM came back to its original location\n",
				   addr);
=======
				   "Roaming client %pM (vid: %d) came back to its original location\n",
				   addr, BATADV_PRINT_VID(vid));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* the ROAM flag is set because this client roamed away
			 * and the node got a roaming_advertisement message. Now
			 * that the client popped up again at its original
			 * location such flag can be unset
			 */
			tt_local->common.flags &= ~BATADV_TT_CLIENT_ROAM;
			roamed_back = true;
		}
		goto check_roaming;
	}

<<<<<<< HEAD
	tt_local = kmalloc(sizeof(*tt_local), GFP_ATOMIC);
	if (!tt_local)
		goto out;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Creating new local tt entry: %pM (ttvn: %d)\n", addr,
		   (uint8_t)atomic_read(&bat_priv->tt.vn));

	memcpy(tt_local->common.addr, addr, ETH_ALEN);
=======
	/* Ignore the client if we cannot send it in a full table response. */
	table_size = batadv_tt_local_table_transmit_size(bat_priv);
	table_size += batadv_tt_len(1);
	packet_size_max = atomic_read(&bat_priv->packet_size_max);
	if (table_size > packet_size_max) {
		net_ratelimited_function(batadv_info, soft_iface,
					 "Local translation table size (%i) exceeds maximum packet size (%i); Ignoring new local tt entry: %pM\n",
					 table_size, packet_size_max, addr);
		goto out;
	}

	tt_local = kmem_cache_alloc(batadv_tl_cache, GFP_ATOMIC);
	if (!tt_local)
		goto out;

	/* increase the refcounter of the related vlan */
	vlan = batadv_softif_vlan_get(bat_priv, vid);
	if (!vlan) {
		net_ratelimited_function(batadv_info, soft_iface,
					 "adding TT local entry %pM to non-existent VLAN %d\n",
					 addr, BATADV_PRINT_VID(vid));
		kmem_cache_free(batadv_tl_cache, tt_local);
		tt_local = NULL;
		goto out;
	}

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Creating new local tt entry: %pM (vid: %d, ttvn: %d)\n",
		   addr, BATADV_PRINT_VID(vid),
		   (u8)atomic_read(&bat_priv->tt.vn));

	ether_addr_copy(tt_local->common.addr, addr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* The local entry has to be marked as NEW to avoid to send it in
	 * a full table response going out before the next ttvn increment
	 * (consistency check)
	 */
	tt_local->common.flags = BATADV_TT_CLIENT_NEW;
<<<<<<< HEAD
	if (batadv_is_wifi_iface(ifindex))
		tt_local->common.flags |= BATADV_TT_CLIENT_WIFI;
	atomic_set(&tt_local->common.refcount, 2);
	tt_local->last_seen = jiffies;
	tt_local->common.added_at = tt_local->last_seen;

	/* the batman interface mac address should never be purged */
	if (batadv_compare_eth(addr, soft_iface->dev_addr))
		tt_local->common.flags |= BATADV_TT_CLIENT_NOPURGE;

	hash_added = batadv_hash_add(bat_priv->tt.local_hash, batadv_compare_tt,
				     batadv_choose_orig, &tt_local->common,
=======
	tt_local->common.vid = vid;
	if (batadv_is_wifi_netdev(in_dev))
		tt_local->common.flags |= BATADV_TT_CLIENT_WIFI;
	kref_init(&tt_local->common.refcount);
	tt_local->last_seen = jiffies;
	tt_local->common.added_at = tt_local->last_seen;
	tt_local->vlan = vlan;

	/* the batman interface mac and multicast addresses should never be
	 * purged
	 */
	if (batadv_compare_eth(addr, soft_iface->dev_addr) ||
	    is_multicast_ether_addr(addr))
		tt_local->common.flags |= BATADV_TT_CLIENT_NOPURGE;

	kref_get(&tt_local->common.refcount);
	hash_added = batadv_hash_add(bat_priv->tt.local_hash, batadv_compare_tt,
				     batadv_choose_tt, &tt_local->common,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				     &tt_local->common.hash_entry);

	if (unlikely(hash_added != 0)) {
		/* remove the reference for the hash */
<<<<<<< HEAD
		batadv_tt_local_entry_free_ref(tt_local);
=======
		batadv_tt_local_entry_put(tt_local);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto out;
	}

add_event:
<<<<<<< HEAD
	batadv_tt_local_event(bat_priv, addr, tt_local->common.flags);
=======
	batadv_tt_local_event(bat_priv, tt_local, BATADV_NO_FLAGS);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

check_roaming:
	/* Check whether it is a roaming, but don't do anything if the roaming
	 * process has already been handled
	 */
	if (tt_global && !(tt_global->common.flags & BATADV_TT_CLIENT_ROAM)) {
		/* These node are probably going to update their tt table */
		head = &tt_global->orig_list;
		rcu_read_lock();
		hlist_for_each_entry_rcu(orig_entry, head, list) {
			batadv_send_roam_adv(bat_priv, tt_global->common.addr,
<<<<<<< HEAD
=======
					     tt_global->common.vid,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					     orig_entry->orig_node);
		}
		rcu_read_unlock();
		if (roamed_back) {
			batadv_tt_global_free(bat_priv, tt_global,
					      "Roaming canceled");
			tt_global = NULL;
		} else {
			/* The global entry has to be marked as ROAMING and
			 * has to be kept for consistency purpose
			 */
			tt_global->common.flags |= BATADV_TT_CLIENT_ROAM;
			tt_global->roam_at = jiffies;
		}
	}

<<<<<<< HEAD
out:
	if (tt_local)
		batadv_tt_local_entry_free_ref(tt_local);
	if (tt_global)
		batadv_tt_global_entry_free_ref(tt_global);
}

static void batadv_tt_realloc_packet_buff(unsigned char **packet_buff,
					  int *packet_buff_len,
					  int min_packet_len,
					  int new_packet_len)
{
	unsigned char *new_buff;

	new_buff = kmalloc(new_packet_len, GFP_ATOMIC);

	/* keep old buffer if kmalloc should fail */
	if (new_buff) {
		memcpy(new_buff, *packet_buff, min_packet_len);
		kfree(*packet_buff);
		*packet_buff = new_buff;
		*packet_buff_len = new_packet_len;
	}
}

static void batadv_tt_prepare_packet_buff(struct batadv_priv *bat_priv,
					  unsigned char **packet_buff,
					  int *packet_buff_len,
					  int min_packet_len)
{
	int req_len;

	req_len = min_packet_len;
	req_len += batadv_tt_len(atomic_read(&bat_priv->tt.local_changes));

	/* if we have too many changes for one packet don't send any
	 * and wait for the tt table request which will be fragmented
	 */
	if (req_len > bat_priv->soft_iface->mtu)
		req_len = min_packet_len;

	batadv_tt_realloc_packet_buff(packet_buff, packet_buff_len,
				      min_packet_len, req_len);
}

static int batadv_tt_changes_fill_buff(struct batadv_priv *bat_priv,
				       unsigned char **packet_buff,
				       int *packet_buff_len,
				       int min_packet_len)
{
	struct batadv_tt_change_node *entry, *safe;
	int count = 0, tot_changes = 0, new_len;
	unsigned char *tt_buff;

	batadv_tt_prepare_packet_buff(bat_priv, packet_buff,
				      packet_buff_len, min_packet_len);

	new_len = *packet_buff_len - min_packet_len;
	tt_buff = *packet_buff + min_packet_len;

	if (new_len > 0)
		tot_changes = new_len / batadv_tt_len(1);
=======
	/* store the current remote flags before altering them. This helps
	 * understanding is flags are changing or not
	 */
	remote_flags = tt_local->common.flags & BATADV_TT_REMOTE_MASK;

	if (batadv_is_wifi_netdev(in_dev))
		tt_local->common.flags |= BATADV_TT_CLIENT_WIFI;
	else
		tt_local->common.flags &= ~BATADV_TT_CLIENT_WIFI;

	/* check the mark in the skb: if it's equal to the configured
	 * isolation_mark, it means the packet is coming from an isolated
	 * non-mesh client
	 */
	match_mark = (mark & bat_priv->isolation_mark_mask);
	if (bat_priv->isolation_mark_mask &&
	    match_mark == bat_priv->isolation_mark)
		tt_local->common.flags |= BATADV_TT_CLIENT_ISOLA;
	else
		tt_local->common.flags &= ~BATADV_TT_CLIENT_ISOLA;

	/* if any "dynamic" flag has been modified, resend an ADD event for this
	 * entry so that all the nodes can get the new flags
	 */
	if (remote_flags ^ (tt_local->common.flags & BATADV_TT_REMOTE_MASK))
		batadv_tt_local_event(bat_priv, tt_local, BATADV_NO_FLAGS);

	ret = true;
out:
	if (in_dev)
		dev_put(in_dev);
	if (tt_local)
		batadv_tt_local_entry_put(tt_local);
	if (tt_global)
		batadv_tt_global_entry_put(tt_global);
	return ret;
}

/**
 * batadv_tt_prepare_tvlv_global_data - prepare the TVLV TT header to send
 *  within a TT Response directed to another node
 * @orig_node: originator for which the TT data has to be prepared
 * @tt_data: uninitialised pointer to the address of the TVLV buffer
 * @tt_change: uninitialised pointer to the address of the area where the TT
 *  changed can be stored
 * @tt_len: pointer to the length to reserve to the tt_change. if -1 this
 *  function reserves the amount of space needed to send the entire global TT
 *  table. In case of success the value is updated with the real amount of
 *  reserved bytes
 * Allocate the needed amount of memory for the entire TT TVLV and write its
 * header made up by one tvlv_tt_data object and a series of tvlv_tt_vlan_data
 * objects, one per active VLAN served by the originator node.
 *
 * Return: the size of the allocated buffer or 0 in case of failure.
 */
static u16
batadv_tt_prepare_tvlv_global_data(struct batadv_orig_node *orig_node,
				   struct batadv_tvlv_tt_data **tt_data,
				   struct batadv_tvlv_tt_change **tt_change,
				   s32 *tt_len)
{
	u16 num_vlan = 0;
	u16 num_entries = 0;
	u16 change_offset;
	u16 tvlv_len;
	struct batadv_tvlv_tt_vlan_data *tt_vlan;
	struct batadv_orig_node_vlan *vlan;
	u8 *tt_change_ptr;

	spin_lock_bh(&orig_node->vlan_list_lock);
	hlist_for_each_entry_rcu(vlan, &orig_node->vlan_list, list) {
		num_vlan++;
		num_entries += atomic_read(&vlan->tt.num_entries);
	}

	change_offset = sizeof(**tt_data);
	change_offset += num_vlan * sizeof(*tt_vlan);

	/* if tt_len is negative, allocate the space needed by the full table */
	if (*tt_len < 0)
		*tt_len = batadv_tt_len(num_entries);

	tvlv_len = *tt_len;
	tvlv_len += change_offset;

	*tt_data = kmalloc(tvlv_len, GFP_ATOMIC);
	if (!*tt_data) {
		*tt_len = 0;
		goto out;
	}

	(*tt_data)->flags = BATADV_NO_FLAGS;
	(*tt_data)->ttvn = atomic_read(&orig_node->last_ttvn);
	(*tt_data)->num_vlan = htons(num_vlan);

	tt_vlan = (struct batadv_tvlv_tt_vlan_data *)(*tt_data + 1);
	hlist_for_each_entry_rcu(vlan, &orig_node->vlan_list, list) {
		tt_vlan->vid = htons(vlan->vid);
		tt_vlan->crc = htonl(vlan->tt.crc);
		tt_vlan->reserved = 0;

		tt_vlan++;
	}

	tt_change_ptr = (u8 *)*tt_data + change_offset;
	*tt_change = (struct batadv_tvlv_tt_change *)tt_change_ptr;

out:
	spin_unlock_bh(&orig_node->vlan_list_lock);
	return tvlv_len;
}

/**
 * batadv_tt_prepare_tvlv_local_data - allocate and prepare the TT TVLV for this
 *  node
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: uninitialised pointer to the address of the TVLV buffer
 * @tt_change: uninitialised pointer to the address of the area where the TT
 *  changes can be stored
 * @tt_len: pointer to the length to reserve to the tt_change. if -1 this
 *  function reserves the amount of space needed to send the entire local TT
 *  table. In case of success the value is updated with the real amount of
 *  reserved bytes
 *
 * Allocate the needed amount of memory for the entire TT TVLV and write its
 * header made up by one tvlv_tt_data object and a series of tvlv_tt_vlan_data
 * objects, one per active VLAN.
 *
 * Return: the size of the allocated buffer or 0 in case of failure.
 */
static u16
batadv_tt_prepare_tvlv_local_data(struct batadv_priv *bat_priv,
				  struct batadv_tvlv_tt_data **tt_data,
				  struct batadv_tvlv_tt_change **tt_change,
				  s32 *tt_len)
{
	struct batadv_tvlv_tt_vlan_data *tt_vlan;
	struct batadv_softif_vlan *vlan;
	u16 num_vlan = 0;
	u16 vlan_entries = 0;
	u16 total_entries = 0;
	u16 tvlv_len;
	u8 *tt_change_ptr;
	int change_offset;

	spin_lock_bh(&bat_priv->softif_vlan_list_lock);
	hlist_for_each_entry_rcu(vlan, &bat_priv->softif_vlan_list, list) {
		vlan_entries = atomic_read(&vlan->tt.num_entries);
		if (vlan_entries < 1)
			continue;

		num_vlan++;
		total_entries += vlan_entries;
	}

	change_offset = sizeof(**tt_data);
	change_offset += num_vlan * sizeof(*tt_vlan);

	/* if tt_len is negative, allocate the space needed by the full table */
	if (*tt_len < 0)
		*tt_len = batadv_tt_len(total_entries);

	tvlv_len = *tt_len;
	tvlv_len += change_offset;

	*tt_data = kmalloc(tvlv_len, GFP_ATOMIC);
	if (!*tt_data) {
		tvlv_len = 0;
		goto out;
	}

	(*tt_data)->flags = BATADV_NO_FLAGS;
	(*tt_data)->ttvn = atomic_read(&bat_priv->tt.vn);
	(*tt_data)->num_vlan = htons(num_vlan);

	tt_vlan = (struct batadv_tvlv_tt_vlan_data *)(*tt_data + 1);
	hlist_for_each_entry_rcu(vlan, &bat_priv->softif_vlan_list, list) {
		vlan_entries = atomic_read(&vlan->tt.num_entries);
		if (vlan_entries < 1)
			continue;

		tt_vlan->vid = htons(vlan->vid);
		tt_vlan->crc = htonl(vlan->tt.crc);
		tt_vlan->reserved = 0;

		tt_vlan++;
	}

	tt_change_ptr = (u8 *)*tt_data + change_offset;
	*tt_change = (struct batadv_tvlv_tt_change *)tt_change_ptr;

out:
	spin_unlock_bh(&bat_priv->softif_vlan_list_lock);
	return tvlv_len;
}

/**
 * batadv_tt_tvlv_container_update - update the translation table tvlv container
 *  after local tt changes have been committed
 * @bat_priv: the bat priv with all the soft interface information
 */
static void batadv_tt_tvlv_container_update(struct batadv_priv *bat_priv)
{
	struct batadv_tt_change_node *entry, *safe;
	struct batadv_tvlv_tt_data *tt_data;
	struct batadv_tvlv_tt_change *tt_change;
	int tt_diff_len, tt_change_len = 0;
	int tt_diff_entries_num = 0;
	int tt_diff_entries_count = 0;
	u16 tvlv_len;

	tt_diff_entries_num = atomic_read(&bat_priv->tt.local_changes);
	tt_diff_len = batadv_tt_len(tt_diff_entries_num);

	/* if we have too many changes for one packet don't send any
	 * and wait for the tt table request which will be fragmented
	 */
	if (tt_diff_len > bat_priv->soft_iface->mtu)
		tt_diff_len = 0;

	tvlv_len = batadv_tt_prepare_tvlv_local_data(bat_priv, &tt_data,
						     &tt_change, &tt_diff_len);
	if (!tvlv_len)
		return;

	tt_data->flags = BATADV_TT_OGM_DIFF;

	if (tt_diff_len == 0)
		goto container_register;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_bh(&bat_priv->tt.changes_list_lock);
	atomic_set(&bat_priv->tt.local_changes, 0);

	list_for_each_entry_safe(entry, safe, &bat_priv->tt.changes_list,
				 list) {
<<<<<<< HEAD
		if (count < tot_changes) {
			memcpy(tt_buff + batadv_tt_len(count),
			       &entry->change, sizeof(struct batadv_tt_change));
			count++;
		}
		list_del(&entry->list);
		kfree(entry);
=======
		if (tt_diff_entries_count < tt_diff_entries_num) {
			memcpy(tt_change + tt_diff_entries_count,
			       &entry->change,
			       sizeof(struct batadv_tvlv_tt_change));
			tt_diff_entries_count++;
		}
		list_del(&entry->list);
		kmem_cache_free(batadv_tt_change_cache, entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	spin_unlock_bh(&bat_priv->tt.changes_list_lock);

	/* Keep the buffer for possible tt_request */
	spin_lock_bh(&bat_priv->tt.last_changeset_lock);
	kfree(bat_priv->tt.last_changeset);
	bat_priv->tt.last_changeset_len = 0;
	bat_priv->tt.last_changeset = NULL;
<<<<<<< HEAD
	/* check whether this new OGM has no changes due to size problems */
	if (new_len > 0) {
		/* if kmalloc() fails we will reply with the full table
		 * instead of providing the diff
		 */
		bat_priv->tt.last_changeset = kmalloc(new_len, GFP_ATOMIC);
		if (bat_priv->tt.last_changeset) {
			memcpy(bat_priv->tt.last_changeset, tt_buff, new_len);
			bat_priv->tt.last_changeset_len = new_len;
=======
	tt_change_len = batadv_tt_len(tt_diff_entries_count);
	/* check whether this new OGM has no changes due to size problems */
	if (tt_diff_entries_count > 0) {
		/* if kmalloc() fails we will reply with the full table
		 * instead of providing the diff
		 */
		bat_priv->tt.last_changeset = kzalloc(tt_diff_len, GFP_ATOMIC);
		if (bat_priv->tt.last_changeset) {
			memcpy(bat_priv->tt.last_changeset,
			       tt_change, tt_change_len);
			bat_priv->tt.last_changeset_len = tt_diff_len;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
	spin_unlock_bh(&bat_priv->tt.last_changeset_lock);

<<<<<<< HEAD
	return count;
}

=======
container_register:
	batadv_tvlv_container_register(bat_priv, BATADV_TVLV_TT, 1, tt_data,
				       tvlv_len);
	kfree(tt_data);
}

#ifdef CONFIG_BATMAN_ADV_DEBUGFS
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int batadv_tt_local_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct batadv_priv *bat_priv = netdev_priv(net_dev);
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_local_entry *tt_local;
	struct batadv_hard_iface *primary_if;
	struct hlist_head *head;
<<<<<<< HEAD
	uint32_t i;
=======
	u32 i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int last_seen_secs;
	int last_seen_msecs;
	unsigned long last_seen_jiffies;
	bool no_purge;
<<<<<<< HEAD
	uint16_t np_flag = BATADV_TT_CLIENT_NOPURGE;
=======
	u16 np_flag = BATADV_TT_CLIENT_NOPURGE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	primary_if = batadv_seq_print_text_primary_if_get(seq);
	if (!primary_if)
		goto out;

	seq_printf(seq,
<<<<<<< HEAD
		   "Locally retrieved addresses (from %s) announced via TT (TTVN: %u CRC: %#.4x):\n",
		   net_dev->name, (uint8_t)atomic_read(&bat_priv->tt.vn),
		   bat_priv->tt.local_crc);
	seq_printf(seq, "       %-13s %-7s %-10s\n", "Client", "Flags",
		   "Last seen");
=======
		   "Locally retrieved addresses (from %s) announced via TT (TTVN: %u):\n",
		   net_dev->name, (u8)atomic_read(&bat_priv->tt.vn));
	seq_puts(seq,
		 "       Client         VID Flags    Last seen (CRC       )\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		hlist_for_each_entry_rcu(tt_common_entry,
					 head, hash_entry) {
			tt_local = container_of(tt_common_entry,
						struct batadv_tt_local_entry,
						common);
			last_seen_jiffies = jiffies - tt_local->last_seen;
			last_seen_msecs = jiffies_to_msecs(last_seen_jiffies);
			last_seen_secs = last_seen_msecs / 1000;
			last_seen_msecs = last_seen_msecs % 1000;

			no_purge = tt_common_entry->flags & np_flag;
<<<<<<< HEAD

			seq_printf(seq, " * %pM [%c%c%c%c%c] %3u.%03u\n",
				   tt_common_entry->addr,
				   (tt_common_entry->flags &
				    BATADV_TT_CLIENT_ROAM ? 'R' : '.'),
				   no_purge ? 'P' : '.',
				   (tt_common_entry->flags &
				    BATADV_TT_CLIENT_NEW ? 'N' : '.'),
				   (tt_common_entry->flags &
				    BATADV_TT_CLIENT_PENDING ? 'X' : '.'),
				   (tt_common_entry->flags &
				    BATADV_TT_CLIENT_WIFI ? 'W' : '.'),
				   no_purge ? 0 : last_seen_secs,
				   no_purge ? 0 : last_seen_msecs);
=======
			seq_printf(seq,
				   " * %pM %4i [%c%c%c%c%c%c] %3u.%03u   (%#.8x)\n",
				   tt_common_entry->addr,
				   BATADV_PRINT_VID(tt_common_entry->vid),
				   ((tt_common_entry->flags &
				     BATADV_TT_CLIENT_ROAM) ? 'R' : '.'),
				   no_purge ? 'P' : '.',
				   ((tt_common_entry->flags &
				     BATADV_TT_CLIENT_NEW) ? 'N' : '.'),
				   ((tt_common_entry->flags &
				     BATADV_TT_CLIENT_PENDING) ? 'X' : '.'),
				   ((tt_common_entry->flags &
				     BATADV_TT_CLIENT_WIFI) ? 'W' : '.'),
				   ((tt_common_entry->flags &
				     BATADV_TT_CLIENT_ISOLA) ? 'I' : '.'),
				   no_purge ? 0 : last_seen_secs,
				   no_purge ? 0 : last_seen_msecs,
				   tt_local->vlan->tt.crc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		rcu_read_unlock();
	}
out:
	if (primary_if)
<<<<<<< HEAD
		batadv_hardif_free_ref(primary_if);
	return 0;
}

static void
batadv_tt_local_set_pending(struct batadv_priv *bat_priv,
			    struct batadv_tt_local_entry *tt_local_entry,
			    uint16_t flags, const char *message)
{
	batadv_tt_local_event(bat_priv, tt_local_entry->common.addr,
			      tt_local_entry->common.flags | flags);
=======
		batadv_hardif_put(primary_if);
	return 0;
}
#endif

/**
 * batadv_tt_local_dump_entry - Dump one TT local entry into a message
 * @msg :Netlink message to dump into
 * @portid: Port making netlink request
 * @seq: Sequence number of netlink message
 * @bat_priv: The bat priv with all the soft interface information
 * @common: tt local & tt global common data
 *
 * Return: Error code, or 0 on success
 */
static int
batadv_tt_local_dump_entry(struct sk_buff *msg, u32 portid, u32 seq,
			   struct batadv_priv *bat_priv,
			   struct batadv_tt_common_entry *common)
{
	void *hdr;
	struct batadv_softif_vlan *vlan;
	struct batadv_tt_local_entry *local;
	unsigned int last_seen_msecs;
	u32 crc;

	local = container_of(common, struct batadv_tt_local_entry, common);
	last_seen_msecs = jiffies_to_msecs(jiffies - local->last_seen);

	vlan = batadv_softif_vlan_get(bat_priv, common->vid);
	if (!vlan)
		return 0;

	crc = vlan->tt.crc;

	batadv_softif_vlan_put(vlan);

	hdr = genlmsg_put(msg, portid, seq, &batadv_netlink_family,
			  NLM_F_MULTI,
			  BATADV_CMD_GET_TRANSTABLE_LOCAL);
	if (!hdr)
		return -ENOBUFS;

	if (nla_put(msg, BATADV_ATTR_TT_ADDRESS, ETH_ALEN, common->addr) ||
	    nla_put_u32(msg, BATADV_ATTR_TT_CRC32, crc) ||
	    nla_put_u16(msg, BATADV_ATTR_TT_VID, common->vid) ||
	    nla_put_u32(msg, BATADV_ATTR_TT_FLAGS, common->flags))
		goto nla_put_failure;

	if (!(common->flags & BATADV_TT_CLIENT_NOPURGE) &&
	    nla_put_u32(msg, BATADV_ATTR_LAST_SEEN_MSECS, last_seen_msecs))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	return 0;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

/**
 * batadv_tt_local_dump_bucket - Dump one TT local bucket into a message
 * @msg: Netlink message to dump into
 * @portid: Port making netlink request
 * @seq: Sequence number of netlink message
 * @bat_priv: The bat priv with all the soft interface information
 * @head: Pointer to the list containing the local tt entries
 * @idx_s: Number of entries to skip
 *
 * Return: Error code, or 0 on success
 */
static int
batadv_tt_local_dump_bucket(struct sk_buff *msg, u32 portid, u32 seq,
			    struct batadv_priv *bat_priv,
			    struct hlist_head *head, int *idx_s)
{
	struct batadv_tt_common_entry *common;
	int idx = 0;

	rcu_read_lock();
	hlist_for_each_entry_rcu(common, head, hash_entry) {
		if (idx++ < *idx_s)
			continue;

		if (batadv_tt_local_dump_entry(msg, portid, seq, bat_priv,
					       common)) {
			rcu_read_unlock();
			*idx_s = idx - 1;
			return -EMSGSIZE;
		}
	}
	rcu_read_unlock();

	*idx_s = 0;
	return 0;
}

/**
 * batadv_tt_local_dump - Dump TT local entries into a message
 * @msg: Netlink message to dump into
 * @cb: Parameters from query
 *
 * Return: Error code, or 0 on success
 */
int batadv_tt_local_dump(struct sk_buff *msg, struct netlink_callback *cb)
{
	struct net *net = sock_net(cb->skb->sk);
	struct net_device *soft_iface;
	struct batadv_priv *bat_priv;
	struct batadv_hard_iface *primary_if = NULL;
	struct batadv_hashtable *hash;
	struct hlist_head *head;
	int ret;
	int ifindex;
	int bucket = cb->args[0];
	int idx = cb->args[1];
	int portid = NETLINK_CB(cb->skb).portid;

	ifindex = batadv_netlink_get_ifindex(cb->nlh, BATADV_ATTR_MESH_IFINDEX);
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

	hash = bat_priv->tt.local_hash;

	while (bucket < hash->size) {
		head = &hash->table[bucket];

		if (batadv_tt_local_dump_bucket(msg, portid, cb->nlh->nlmsg_seq,
						bat_priv, head, &idx))
			break;

		bucket++;
	}

	ret = msg->len;

 out:
	if (primary_if)
		batadv_hardif_put(primary_if);
	if (soft_iface)
		dev_put(soft_iface);

	cb->args[0] = bucket;
	cb->args[1] = idx;

	return ret;
}

static void
batadv_tt_local_set_pending(struct batadv_priv *bat_priv,
			    struct batadv_tt_local_entry *tt_local_entry,
			    u16 flags, const char *message)
{
	batadv_tt_local_event(bat_priv, tt_local_entry, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* The local client has to be marked as "pending to be removed" but has
	 * to be kept in the table in order to send it in a full table
	 * response issued before the net ttvn increment (consistency check)
	 */
	tt_local_entry->common.flags |= BATADV_TT_CLIENT_PENDING;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
<<<<<<< HEAD
		   "Local tt entry (%pM) pending to be removed: %s\n",
		   tt_local_entry->common.addr, message);
=======
		   "Local tt entry (%pM, vid: %d) pending to be removed: %s\n",
		   tt_local_entry->common.addr,
		   BATADV_PRINT_VID(tt_local_entry->common.vid), message);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * batadv_tt_local_remove - logically remove an entry from the local table
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the MAC address of the client to remove
<<<<<<< HEAD
 * @message: message to append to the log on deletion
 * @roaming: true if the deletion is due to a roaming event
 *
 * Returns the flags assigned to the local entry before being deleted
 */
uint16_t batadv_tt_local_remove(struct batadv_priv *bat_priv,
				const uint8_t *addr, const char *message,
				bool roaming)
{
	struct batadv_tt_local_entry *tt_local_entry;
	uint16_t flags, curr_flags = BATADV_NO_FLAGS;

	tt_local_entry = batadv_tt_local_hash_find(bat_priv, addr);
=======
 * @vid: VLAN identifier
 * @message: message to append to the log on deletion
 * @roaming: true if the deletion is due to a roaming event
 *
 * Return: the flags assigned to the local entry before being deleted
 */
u16 batadv_tt_local_remove(struct batadv_priv *bat_priv, const u8 *addr,
			   unsigned short vid, const char *message,
			   bool roaming)
{
	struct batadv_tt_local_entry *tt_removed_entry;
	struct batadv_tt_local_entry *tt_local_entry;
	u16 flags, curr_flags = BATADV_NO_FLAGS;
	struct hlist_node *tt_removed_node;

	tt_local_entry = batadv_tt_local_hash_find(bat_priv, addr, vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!tt_local_entry)
		goto out;

	curr_flags = tt_local_entry->common.flags;

	flags = BATADV_TT_CLIENT_DEL;
	/* if this global entry addition is due to a roaming, the node has to
	 * mark the local entry as "roamed" in order to correctly reroute
	 * packets later
	 */
	if (roaming) {
		flags |= BATADV_TT_CLIENT_ROAM;
		/* mark the local client as ROAMed */
		tt_local_entry->common.flags |= BATADV_TT_CLIENT_ROAM;
	}

	if (!(tt_local_entry->common.flags & BATADV_TT_CLIENT_NEW)) {
		batadv_tt_local_set_pending(bat_priv, tt_local_entry, flags,
					    message);
		goto out;
	}
	/* if this client has been added right now, it is possible to
	 * immediately purge it
	 */
<<<<<<< HEAD
	batadv_tt_local_event(bat_priv, tt_local_entry->common.addr,
			      curr_flags | BATADV_TT_CLIENT_DEL);
	hlist_del_rcu(&tt_local_entry->common.hash_entry);
	batadv_tt_local_entry_free_ref(tt_local_entry);

out:
	if (tt_local_entry)
		batadv_tt_local_entry_free_ref(tt_local_entry);
=======
	batadv_tt_local_event(bat_priv, tt_local_entry, BATADV_TT_CLIENT_DEL);

	tt_removed_node = batadv_hash_remove(bat_priv->tt.local_hash,
					     batadv_compare_tt,
					     batadv_choose_tt,
					     &tt_local_entry->common);
	if (!tt_removed_node)
		goto out;

	/* drop reference of remove hash entry */
	tt_removed_entry = hlist_entry(tt_removed_node,
				       struct batadv_tt_local_entry,
				       common.hash_entry);
	batadv_tt_local_entry_put(tt_removed_entry);

out:
	if (tt_local_entry)
		batadv_tt_local_entry_put(tt_local_entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return curr_flags;
}

<<<<<<< HEAD
static void batadv_tt_local_purge_list(struct batadv_priv *bat_priv,
				       struct hlist_head *head)
=======
/**
 * batadv_tt_local_purge_list - purge inactive tt local entries
 * @bat_priv: the bat priv with all the soft interface information
 * @head: pointer to the list containing the local tt entries
 * @timeout: parameter deciding whether a given tt local entry is considered
 *  inactive or not
 */
static void batadv_tt_local_purge_list(struct batadv_priv *bat_priv,
				       struct hlist_head *head,
				       int timeout)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct batadv_tt_local_entry *tt_local_entry;
	struct batadv_tt_common_entry *tt_common_entry;
	struct hlist_node *node_tmp;

	hlist_for_each_entry_safe(tt_common_entry, node_tmp, head,
				  hash_entry) {
		tt_local_entry = container_of(tt_common_entry,
					      struct batadv_tt_local_entry,
					      common);
		if (tt_local_entry->common.flags & BATADV_TT_CLIENT_NOPURGE)
			continue;

		/* entry already marked for deletion */
		if (tt_local_entry->common.flags & BATADV_TT_CLIENT_PENDING)
			continue;

<<<<<<< HEAD
		if (!batadv_has_timed_out(tt_local_entry->last_seen,
					  BATADV_TT_LOCAL_TIMEOUT))
=======
		if (!batadv_has_timed_out(tt_local_entry->last_seen, timeout))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;

		batadv_tt_local_set_pending(bat_priv, tt_local_entry,
					    BATADV_TT_CLIENT_DEL, "timed out");
	}
}

<<<<<<< HEAD
static void batadv_tt_local_purge(struct batadv_priv *bat_priv)
=======
/**
 * batadv_tt_local_purge - purge inactive tt local entries
 * @bat_priv: the bat priv with all the soft interface information
 * @timeout: parameter deciding whether a given tt local entry is considered
 *  inactive or not
 */
static void batadv_tt_local_purge(struct batadv_priv *bat_priv,
				  int timeout)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
	struct hlist_head *head;
	spinlock_t *list_lock; /* protects write access to the hash lists */
<<<<<<< HEAD
	uint32_t i;
=======
	u32 i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
<<<<<<< HEAD
		batadv_tt_local_purge_list(bat_priv, head);
=======
		batadv_tt_local_purge_list(bat_priv, head, timeout);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		spin_unlock_bh(list_lock);
	}
}

static void batadv_tt_local_table_free(struct batadv_priv *bat_priv)
{
	struct batadv_hashtable *hash;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_local_entry *tt_local;
	struct hlist_node *node_tmp;
	struct hlist_head *head;
<<<<<<< HEAD
	uint32_t i;
=======
	u32 i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!bat_priv->tt.local_hash)
		return;

	hash = bat_priv->tt.local_hash;

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
		hlist_for_each_entry_safe(tt_common_entry, node_tmp,
					  head, hash_entry) {
			hlist_del_rcu(&tt_common_entry->hash_entry);
			tt_local = container_of(tt_common_entry,
						struct batadv_tt_local_entry,
						common);
<<<<<<< HEAD
			batadv_tt_local_entry_free_ref(tt_local);
=======

			batadv_tt_local_entry_put(tt_local);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		spin_unlock_bh(list_lock);
	}

	batadv_hash_destroy(hash);

	bat_priv->tt.local_hash = NULL;
}

static int batadv_tt_global_init(struct batadv_priv *bat_priv)
{
	if (bat_priv->tt.global_hash)
		return 0;

	bat_priv->tt.global_hash = batadv_hash_new(1024);

	if (!bat_priv->tt.global_hash)
		return -ENOMEM;

	batadv_hash_set_lock_class(bat_priv->tt.global_hash,
				   &batadv_tt_global_hash_lock_class_key);

	return 0;
}

static void batadv_tt_changes_list_free(struct batadv_priv *bat_priv)
{
	struct batadv_tt_change_node *entry, *safe;

	spin_lock_bh(&bat_priv->tt.changes_list_lock);

	list_for_each_entry_safe(entry, safe, &bat_priv->tt.changes_list,
				 list) {
		list_del(&entry->list);
<<<<<<< HEAD
		kfree(entry);
=======
		kmem_cache_free(batadv_tt_change_cache, entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	atomic_set(&bat_priv->tt.local_changes, 0);
	spin_unlock_bh(&bat_priv->tt.changes_list_lock);
}

<<<<<<< HEAD
/* retrieves the orig_tt_list_entry belonging to orig_node from the
 * batadv_tt_global_entry list
 *
 * returns it with an increased refcounter, NULL if not found
=======
/**
 * batadv_tt_global_orig_entry_find - find a TT orig_list_entry
 * @entry: the TT global entry where the orig_list_entry has to be
 *  extracted from
 * @orig_node: the originator for which the orig_list_entry has to be found
 *
 * retrieve the orig_tt_list_entry belonging to orig_node from the
 * batadv_tt_global_entry list
 *
 * Return: it with an increased refcounter, NULL if not found
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static struct batadv_tt_orig_list_entry *
batadv_tt_global_orig_entry_find(const struct batadv_tt_global_entry *entry,
				 const struct batadv_orig_node *orig_node)
{
	struct batadv_tt_orig_list_entry *tmp_orig_entry, *orig_entry = NULL;
	const struct hlist_head *head;

	rcu_read_lock();
	head = &entry->orig_list;
	hlist_for_each_entry_rcu(tmp_orig_entry, head, list) {
		if (tmp_orig_entry->orig_node != orig_node)
			continue;
<<<<<<< HEAD
		if (!atomic_inc_not_zero(&tmp_orig_entry->refcount))
=======
		if (!kref_get_unless_zero(&tmp_orig_entry->refcount))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;

		orig_entry = tmp_orig_entry;
		break;
	}
	rcu_read_unlock();

	return orig_entry;
}

<<<<<<< HEAD
/* find out if an orig_node is already in the list of a tt_global_entry.
 * returns true if found, false otherwise
 */
static bool
batadv_tt_global_entry_has_orig(const struct batadv_tt_global_entry *entry,
				const struct batadv_orig_node *orig_node)
=======
/**
 * batadv_tt_global_entry_has_orig - check if a TT global entry is also handled
 *  by a given originator
 * @entry: the TT global entry to check
 * @orig_node: the originator to search in the list
 * @flags: a pointer to store TT flags for the given @entry received
 *  from @orig_node
 *
 * find out if an orig_node is already in the list of a tt_global_entry.
 *
 * Return: true if found, false otherwise
 */
static bool
batadv_tt_global_entry_has_orig(const struct batadv_tt_global_entry *entry,
				const struct batadv_orig_node *orig_node,
				u8 *flags)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct batadv_tt_orig_list_entry *orig_entry;
	bool found = false;

	orig_entry = batadv_tt_global_orig_entry_find(entry, orig_node);
	if (orig_entry) {
		found = true;
<<<<<<< HEAD
		batadv_tt_orig_list_entry_free_ref(orig_entry);
=======

		if (flags)
			*flags = orig_entry->flags;

		batadv_tt_orig_list_entry_put(orig_entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return found;
}

<<<<<<< HEAD
static void
batadv_tt_global_orig_entry_add(struct batadv_tt_global_entry *tt_global,
				struct batadv_orig_node *orig_node, int ttvn)
{
	struct batadv_tt_orig_list_entry *orig_entry;

=======
/**
 * batadv_tt_global_sync_flags - update TT sync flags
 * @tt_global: the TT global entry to update sync flags in
 *
 * Updates the sync flag bits in the tt_global flag attribute with a logical
 * OR of all sync flags from any of its TT orig entries.
 */
static void
batadv_tt_global_sync_flags(struct batadv_tt_global_entry *tt_global)
{
	struct batadv_tt_orig_list_entry *orig_entry;
	const struct hlist_head *head;
	u16 flags = BATADV_NO_FLAGS;

	rcu_read_lock();
	head = &tt_global->orig_list;
	hlist_for_each_entry_rcu(orig_entry, head, list)
		flags |= orig_entry->flags;
	rcu_read_unlock();

	flags |= tt_global->common.flags & (~BATADV_TT_SYNC_MASK);
	tt_global->common.flags = flags;
}

/**
 * batadv_tt_global_orig_entry_add - add or update a TT orig entry
 * @tt_global: the TT global entry to add an orig entry in
 * @orig_node: the originator to add an orig entry for
 * @ttvn: translation table version number of this changeset
 * @flags: TT sync flags
 */
static void
batadv_tt_global_orig_entry_add(struct batadv_tt_global_entry *tt_global,
				struct batadv_orig_node *orig_node, int ttvn,
				u8 flags)
{
	struct batadv_tt_orig_list_entry *orig_entry;

	spin_lock_bh(&tt_global->list_lock);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	orig_entry = batadv_tt_global_orig_entry_find(tt_global, orig_node);
	if (orig_entry) {
		/* refresh the ttvn: the current value could be a bogus one that
		 * was added during a "temporary client detection"
		 */
		orig_entry->ttvn = ttvn;
<<<<<<< HEAD
		goto out;
	}

	orig_entry = kzalloc(sizeof(*orig_entry), GFP_ATOMIC);
=======
		orig_entry->flags = flags;
		goto sync_flags;
	}

	orig_entry = kmem_cache_zalloc(batadv_tt_orig_cache, GFP_ATOMIC);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!orig_entry)
		goto out;

	INIT_HLIST_NODE(&orig_entry->list);
<<<<<<< HEAD
	atomic_inc(&orig_node->refcount);
	atomic_inc(&orig_node->tt_size);
	orig_entry->orig_node = orig_node;
	orig_entry->ttvn = ttvn;
	atomic_set(&orig_entry->refcount, 2);

	spin_lock_bh(&tt_global->list_lock);
	hlist_add_head_rcu(&orig_entry->list,
			   &tt_global->orig_list);
	spin_unlock_bh(&tt_global->list_lock);
out:
	if (orig_entry)
		batadv_tt_orig_list_entry_free_ref(orig_entry);
}

/* caller must hold orig_node refcount */
int batadv_tt_global_add(struct batadv_priv *bat_priv,
			 struct batadv_orig_node *orig_node,
			 const unsigned char *tt_addr, uint8_t flags,
			 uint8_t ttvn)
{
	struct batadv_tt_global_entry *tt_global_entry;
	struct batadv_tt_local_entry *tt_local_entry;
	int ret = 0;
	int hash_added;
	struct batadv_tt_common_entry *common;
	uint16_t local_flags;

	tt_global_entry = batadv_tt_global_hash_find(bat_priv, tt_addr);
	tt_local_entry = batadv_tt_local_hash_find(bat_priv, tt_addr);
=======
	kref_get(&orig_node->refcount);
	batadv_tt_global_size_inc(orig_node, tt_global->common.vid);
	orig_entry->orig_node = orig_node;
	orig_entry->ttvn = ttvn;
	orig_entry->flags = flags;
	kref_init(&orig_entry->refcount);

	kref_get(&orig_entry->refcount);
	hlist_add_head_rcu(&orig_entry->list,
			   &tt_global->orig_list);
	atomic_inc(&tt_global->orig_list_count);

sync_flags:
	batadv_tt_global_sync_flags(tt_global);
out:
	if (orig_entry)
		batadv_tt_orig_list_entry_put(orig_entry);

	spin_unlock_bh(&tt_global->list_lock);
}

/**
 * batadv_tt_global_add - add a new TT global entry or update an existing one
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the originator announcing the client
 * @tt_addr: the mac address of the non-mesh client
 * @vid: VLAN identifier
 * @flags: TT flags that have to be set for this non-mesh client
 * @ttvn: the tt version number ever announcing this non-mesh client
 *
 * Add a new TT global entry for the given originator. If the entry already
 * exists add a new reference to the given originator (a global entry can have
 * references to multiple originators) and adjust the flags attribute to reflect
 * the function argument.
 * If a TT local entry exists for this non-mesh client remove it.
 *
 * The caller must hold orig_node refcount.
 *
 * Return: true if the new entry has been added, false otherwise
 */
static bool batadv_tt_global_add(struct batadv_priv *bat_priv,
				 struct batadv_orig_node *orig_node,
				 const unsigned char *tt_addr,
				 unsigned short vid, u16 flags, u8 ttvn)
{
	struct batadv_tt_global_entry *tt_global_entry;
	struct batadv_tt_local_entry *tt_local_entry;
	bool ret = false;
	int hash_added;
	struct batadv_tt_common_entry *common;
	u16 local_flags;

	/* ignore global entries from backbone nodes */
	if (batadv_bla_is_backbone_gw_orig(bat_priv, orig_node->orig, vid))
		return true;

	tt_global_entry = batadv_tt_global_hash_find(bat_priv, tt_addr, vid);
	tt_local_entry = batadv_tt_local_hash_find(bat_priv, tt_addr, vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* if the node already has a local client for this entry, it has to wait
	 * for a roaming advertisement instead of manually messing up the global
	 * table
	 */
	if ((flags & BATADV_TT_CLIENT_TEMP) && tt_local_entry &&
	    !(tt_local_entry->common.flags & BATADV_TT_CLIENT_NEW))
		goto out;

	if (!tt_global_entry) {
<<<<<<< HEAD
		tt_global_entry = kzalloc(sizeof(*tt_global_entry), GFP_ATOMIC);
=======
		tt_global_entry = kmem_cache_zalloc(batadv_tg_cache,
						    GFP_ATOMIC);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!tt_global_entry)
			goto out;

		common = &tt_global_entry->common;
<<<<<<< HEAD
		memcpy(common->addr, tt_addr, ETH_ALEN);

		common->flags = flags;
=======
		ether_addr_copy(common->addr, tt_addr);
		common->vid = vid;

		if (!is_multicast_ether_addr(common->addr))
			common->flags = flags & (~BATADV_TT_SYNC_MASK);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		tt_global_entry->roam_at = 0;
		/* node must store current time in case of roaming. This is
		 * needed to purge this entry out on timeout (if nobody claims
		 * it)
		 */
		if (flags & BATADV_TT_CLIENT_ROAM)
			tt_global_entry->roam_at = jiffies;
<<<<<<< HEAD
		atomic_set(&common->refcount, 2);
		common->added_at = jiffies;

		INIT_HLIST_HEAD(&tt_global_entry->orig_list);
		spin_lock_init(&tt_global_entry->list_lock);

		hash_added = batadv_hash_add(bat_priv->tt.global_hash,
					     batadv_compare_tt,
					     batadv_choose_orig, common,
=======
		kref_init(&common->refcount);
		common->added_at = jiffies;

		INIT_HLIST_HEAD(&tt_global_entry->orig_list);
		atomic_set(&tt_global_entry->orig_list_count, 0);
		spin_lock_init(&tt_global_entry->list_lock);

		kref_get(&common->refcount);
		hash_added = batadv_hash_add(bat_priv->tt.global_hash,
					     batadv_compare_tt,
					     batadv_choose_tt, common,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					     &common->hash_entry);

		if (unlikely(hash_added != 0)) {
			/* remove the reference for the hash */
<<<<<<< HEAD
			batadv_tt_global_entry_free_ref(tt_global_entry);
=======
			batadv_tt_global_entry_put(tt_global_entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out_remove;
		}
	} else {
		common = &tt_global_entry->common;
		/* If there is already a global entry, we can use this one for
		 * our processing.
		 * But if we are trying to add a temporary client then here are
		 * two options at this point:
		 * 1) the global client is not a temporary client: the global
		 *    client has to be left as it is, temporary information
		 *    should never override any already known client state
		 * 2) the global client is a temporary client: purge the
		 *    originator list and add the new one orig_entry
		 */
		if (flags & BATADV_TT_CLIENT_TEMP) {
			if (!(common->flags & BATADV_TT_CLIENT_TEMP))
				goto out;
			if (batadv_tt_global_entry_has_orig(tt_global_entry,
<<<<<<< HEAD
							    orig_node))
=======
							    orig_node, NULL))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				goto out_remove;
			batadv_tt_global_del_orig_list(tt_global_entry);
			goto add_orig_entry;
		}

		/* if the client was temporary added before receiving the first
<<<<<<< HEAD
		 * OGM announcing it, we have to clear the TEMP flag
		 */
		common->flags &= ~BATADV_TT_CLIENT_TEMP;

		/* the change can carry possible "attribute" flags like the
		 * TT_CLIENT_WIFI, therefore they have to be copied in the
		 * client entry
		 */
		tt_global_entry->common.flags |= flags;
=======
		 * OGM announcing it, we have to clear the TEMP flag. Also,
		 * remove the previous temporary orig node and re-add it
		 * if required. If the orig entry changed, the new one which
		 * is a non-temporary entry is preferred.
		 */
		if (common->flags & BATADV_TT_CLIENT_TEMP) {
			batadv_tt_global_del_orig_list(tt_global_entry);
			common->flags &= ~BATADV_TT_CLIENT_TEMP;
		}

		/* the change can carry possible "attribute" flags like the
		 * TT_CLIENT_TEMP, therefore they have to be copied in the
		 * client entry
		 */
		if (!is_multicast_ether_addr(common->addr))
			common->flags |= flags & (~BATADV_TT_SYNC_MASK);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* If there is the BATADV_TT_CLIENT_ROAM flag set, there is only
		 * one originator left in the list and we previously received a
		 * delete + roaming change for this originator.
		 *
		 * We should first delete the old originator before adding the
		 * new one.
		 */
		if (common->flags & BATADV_TT_CLIENT_ROAM) {
			batadv_tt_global_del_orig_list(tt_global_entry);
			common->flags &= ~BATADV_TT_CLIENT_ROAM;
			tt_global_entry->roam_at = 0;
		}
	}
add_orig_entry:
	/* add the new orig_entry (if needed) or update it */
<<<<<<< HEAD
	batadv_tt_global_orig_entry_add(tt_global_entry, orig_node, ttvn);

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Creating new global tt entry: %pM (via %pM)\n",
		   common->addr, orig_node->orig);
	ret = 1;

out_remove:

	/* remove address from local hash if present */
	local_flags = batadv_tt_local_remove(bat_priv, tt_addr,
=======
	batadv_tt_global_orig_entry_add(tt_global_entry, orig_node, ttvn,
					flags & BATADV_TT_SYNC_MASK);

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Creating new global tt entry: %pM (vid: %d, via %pM)\n",
		   common->addr, BATADV_PRINT_VID(common->vid),
		   orig_node->orig);
	ret = true;

out_remove:
	/* Do not remove multicast addresses from the local hash on
	 * global additions
	 */
	if (is_multicast_ether_addr(tt_addr))
		goto out;

	/* remove address from local hash if present */
	local_flags = batadv_tt_local_remove(bat_priv, tt_addr, vid,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					     "global tt received",
					     flags & BATADV_TT_CLIENT_ROAM);
	tt_global_entry->common.flags |= local_flags & BATADV_TT_CLIENT_WIFI;

	if (!(flags & BATADV_TT_CLIENT_ROAM))
		/* this is a normal global add. Therefore the client is not in a
		 * roaming state anymore.
		 */
		tt_global_entry->common.flags &= ~BATADV_TT_CLIENT_ROAM;

out:
	if (tt_global_entry)
<<<<<<< HEAD
		batadv_tt_global_entry_free_ref(tt_global_entry);
	if (tt_local_entry)
		batadv_tt_local_entry_free_ref(tt_local_entry);
	return ret;
}

/* batadv_transtable_best_orig - Get best originator list entry from tt entry
 * @tt_global_entry: global translation table entry to be analyzed
 *
 * This functon assumes the caller holds rcu_read_lock().
 * Returns best originator list entry or NULL on errors.
 */
static struct batadv_tt_orig_list_entry *
batadv_transtable_best_orig(struct batadv_tt_global_entry *tt_global_entry)
{
	struct batadv_neigh_node *router = NULL;
	struct hlist_head *head;
	struct batadv_tt_orig_list_entry *orig_entry, *best_entry = NULL;
	int best_tq = 0;

	head = &tt_global_entry->orig_list;
	hlist_for_each_entry_rcu(orig_entry, head, list) {
		router = batadv_orig_node_get_router(orig_entry->orig_node);
		if (!router)
			continue;

		if (router->tq_avg > best_tq) {
			best_entry = orig_entry;
			best_tq = router->tq_avg;
		}

		batadv_neigh_node_free_ref(router);
	}

	return best_entry;
}

/* batadv_tt_global_print_entry - print all orig nodes who announce the address
 * for this global entry
 * @tt_global_entry: global translation table entry to be printed
 * @seq: debugfs table seq_file struct
 *
 * This functon assumes the caller holds rcu_read_lock().
 */
static void
batadv_tt_global_print_entry(struct batadv_tt_global_entry *tt_global_entry,
			     struct seq_file *seq)
{
	struct hlist_head *head;
	struct batadv_tt_orig_list_entry *orig_entry, *best_entry;
	struct batadv_tt_common_entry *tt_common_entry;
	uint16_t flags;
	uint8_t last_ttvn;

	tt_common_entry = &tt_global_entry->common;
	flags = tt_common_entry->flags;

	best_entry = batadv_transtable_best_orig(tt_global_entry);
	if (best_entry) {
		last_ttvn = atomic_read(&best_entry->orig_node->last_ttvn);
		seq_printf(seq,
			   " %c %pM  (%3u) via %pM     (%3u)   (%#.4x) [%c%c%c]\n",
			   '*', tt_global_entry->common.addr,
			   best_entry->ttvn, best_entry->orig_node->orig,
			   last_ttvn, best_entry->orig_node->tt_crc,
			   (flags & BATADV_TT_CLIENT_ROAM ? 'R' : '.'),
			   (flags & BATADV_TT_CLIENT_WIFI ? 'W' : '.'),
			   (flags & BATADV_TT_CLIENT_TEMP ? 'T' : '.'));
	}

	head = &tt_global_entry->orig_list;

	hlist_for_each_entry_rcu(orig_entry, head, list) {
		if (best_entry == orig_entry)
			continue;

		last_ttvn = atomic_read(&orig_entry->orig_node->last_ttvn);
		seq_printf(seq,	" %c %pM  (%3u) via %pM     (%3u)   [%c%c%c]\n",
			   '+', tt_global_entry->common.addr,
			   orig_entry->ttvn, orig_entry->orig_node->orig,
			   last_ttvn,
			   (flags & BATADV_TT_CLIENT_ROAM ? 'R' : '.'),
			   (flags & BATADV_TT_CLIENT_WIFI ? 'W' : '.'),
			   (flags & BATADV_TT_CLIENT_TEMP ? 'T' : '.'));
	}
}

int batadv_tt_global_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct batadv_priv *bat_priv = netdev_priv(net_dev);
	struct batadv_hashtable *hash = bat_priv->tt.global_hash;
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_global_entry *tt_global;
	struct batadv_hard_iface *primary_if;
	struct hlist_head *head;
	uint32_t i;

	primary_if = batadv_seq_print_text_primary_if_get(seq);
	if (!primary_if)
		goto out;

	seq_printf(seq,
		   "Globally announced TT entries received via the mesh %s\n",
		   net_dev->name);
	seq_printf(seq, "       %-13s %s       %-15s %s (%-6s) %s\n",
		   "Client", "(TTVN)", "Originator", "(Curr TTVN)", "CRC",
		   "Flags");

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		hlist_for_each_entry_rcu(tt_common_entry,
					 head, hash_entry) {
			tt_global = container_of(tt_common_entry,
						 struct batadv_tt_global_entry,
						 common);
			batadv_tt_global_print_entry(tt_global, seq);
		}
		rcu_read_unlock();
	}
out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	return 0;
=======
		batadv_tt_global_entry_put(tt_global_entry);
	if (tt_local_entry)
		batadv_tt_local_entry_put(tt_local_entry);
	return ret;
}

/**
 * batadv_transtable_best_orig - Get best originator list entry from tt entry
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_global_entry: global translation table entry to be analyzed
 *
 * This functon assumes the caller holds rcu_read_lock().
 * Return: best originator list entry or NULL on errors.
 */
static struct batadv_tt_orig_list_entry *
batadv_transtable_best_orig(struct batadv_priv *bat_priv,
			    struct batadv_tt_global_entry *tt_global_entry)
{
	struct batadv_neigh_node *router, *best_router = NULL;
	struct batadv_algo_ops *bao = bat_priv->algo_ops;
	struct hlist_head *head;
	struct batadv_tt_orig_list_entry *orig_entry, *best_entry = NULL;

	head = &tt_global_entry->orig_list;
	hlist_for_each_entry_rcu(orig_entry, head, list) {
		router = batadv_orig_router_get(orig_entry->orig_node,
						BATADV_IF_DEFAULT);
		if (!router)
			continue;

		if (best_router &&
		    bao->neigh.cmp(router, BATADV_IF_DEFAULT, best_router,
				   BATADV_IF_DEFAULT) <= 0) {
			batadv_neigh_node_put(router);
			continue;
		}

		/* release the refcount for the "old" best */
		if (best_router)
			batadv_neigh_node_put(best_router);

		best_entry = orig_entry;
		best_router = router;
	}

	if (best_router)
		batadv_neigh_node_put(best_router);

	return best_entry;
}

#ifdef CONFIG_BATMAN_ADV_DEBUGFS
/**
 * batadv_tt_global_print_entry - print all orig nodes who announce the address
 *  for this global entry
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_global_entry: global translation table entry to be printed
 * @seq: debugfs table seq_file struct
 *
 * This functon assumes the caller holds rcu_read_lock().
 */
static void
batadv_tt_global_print_entry(struct batadv_priv *bat_priv,
			     struct batadv_tt_global_entry *tt_global_entry,
			     struct seq_file *seq)
{
	struct batadv_tt_orig_list_entry *orig_entry, *best_entry;
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_orig_node_vlan *vlan;
	struct hlist_head *head;
	u8 last_ttvn;
	u16 flags;

	tt_common_entry = &tt_global_entry->common;
	flags = tt_common_entry->flags;

	best_entry = batadv_transtable_best_orig(bat_priv, tt_global_entry);
	if (best_entry) {
		vlan = batadv_orig_node_vlan_get(best_entry->orig_node,
						 tt_common_entry->vid);
		if (!vlan) {
			seq_printf(seq,
				   " * Cannot retrieve VLAN %d for originator %pM\n",
				   BATADV_PRINT_VID(tt_common_entry->vid),
				   best_entry->orig_node->orig);
			goto print_list;
		}

		last_ttvn = atomic_read(&best_entry->orig_node->last_ttvn);
		seq_printf(seq,
			   " %c %pM %4i   (%3u) via %pM     (%3u)   (%#.8x) [%c%c%c%c]\n",
			   '*', tt_global_entry->common.addr,
			   BATADV_PRINT_VID(tt_global_entry->common.vid),
			   best_entry->ttvn, best_entry->orig_node->orig,
			   last_ttvn, vlan->tt.crc,
			   ((flags & BATADV_TT_CLIENT_ROAM) ? 'R' : '.'),
			   ((flags & BATADV_TT_CLIENT_WIFI) ? 'W' : '.'),
			   ((flags & BATADV_TT_CLIENT_ISOLA) ? 'I' : '.'),
			   ((flags & BATADV_TT_CLIENT_TEMP) ? 'T' : '.'));

		batadv_orig_node_vlan_put(vlan);
	}

print_list:
	head = &tt_global_entry->orig_list;

	hlist_for_each_entry_rcu(orig_entry, head, list) {
		if (best_entry == orig_entry)
			continue;

		vlan = batadv_orig_node_vlan_get(orig_entry->orig_node,
						 tt_common_entry->vid);
		if (!vlan) {
			seq_printf(seq,
				   " + Cannot retrieve VLAN %d for originator %pM\n",
				   BATADV_PRINT_VID(tt_common_entry->vid),
				   orig_entry->orig_node->orig);
			continue;
		}

		last_ttvn = atomic_read(&orig_entry->orig_node->last_ttvn);
		seq_printf(seq,
			   " %c %pM %4d   (%3u) via %pM     (%3u)   (%#.8x) [%c%c%c%c]\n",
			   '+', tt_global_entry->common.addr,
			   BATADV_PRINT_VID(tt_global_entry->common.vid),
			   orig_entry->ttvn, orig_entry->orig_node->orig,
			   last_ttvn, vlan->tt.crc,
			   ((flags & BATADV_TT_CLIENT_ROAM) ? 'R' : '.'),
			   ((flags & BATADV_TT_CLIENT_WIFI) ? 'W' : '.'),
			   ((flags & BATADV_TT_CLIENT_ISOLA) ? 'I' : '.'),
			   ((flags & BATADV_TT_CLIENT_TEMP) ? 'T' : '.'));

		batadv_orig_node_vlan_put(vlan);
	}
}

int batadv_tt_global_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct batadv_priv *bat_priv = netdev_priv(net_dev);
	struct batadv_hashtable *hash = bat_priv->tt.global_hash;
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_global_entry *tt_global;
	struct batadv_hard_iface *primary_if;
	struct hlist_head *head;
	u32 i;

	primary_if = batadv_seq_print_text_primary_if_get(seq);
	if (!primary_if)
		goto out;

	seq_printf(seq,
		   "Globally announced TT entries received via the mesh %s\n",
		   net_dev->name);
	seq_puts(seq,
		 "       Client         VID  (TTVN)       Originator      (Curr TTVN) (CRC       ) Flags\n");

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		hlist_for_each_entry_rcu(tt_common_entry,
					 head, hash_entry) {
			tt_global = container_of(tt_common_entry,
						 struct batadv_tt_global_entry,
						 common);
			batadv_tt_global_print_entry(bat_priv, tt_global, seq);
		}
		rcu_read_unlock();
	}
out:
	if (primary_if)
		batadv_hardif_put(primary_if);
	return 0;
}
#endif

/**
 * batadv_tt_global_dump_subentry - Dump all TT local entries into a message
 * @msg: Netlink message to dump into
 * @portid: Port making netlink request
 * @seq: Sequence number of netlink message
 * @common: tt local & tt global common data
 * @orig: Originator node announcing a non-mesh client
 * @best: Is the best originator for the TT entry
 *
 * Return: Error code, or 0 on success
 */
static int
batadv_tt_global_dump_subentry(struct sk_buff *msg, u32 portid, u32 seq,
			       struct batadv_tt_common_entry *common,
			       struct batadv_tt_orig_list_entry *orig,
			       bool best)
{
	u16 flags = (common->flags & (~BATADV_TT_SYNC_MASK)) | orig->flags;
	void *hdr;
	struct batadv_orig_node_vlan *vlan;
	u8 last_ttvn;
	u32 crc;

	vlan = batadv_orig_node_vlan_get(orig->orig_node,
					 common->vid);
	if (!vlan)
		return 0;

	crc = vlan->tt.crc;

	batadv_orig_node_vlan_put(vlan);

	hdr = genlmsg_put(msg, portid, seq, &batadv_netlink_family,
			  NLM_F_MULTI,
			  BATADV_CMD_GET_TRANSTABLE_GLOBAL);
	if (!hdr)
		return -ENOBUFS;

	last_ttvn = atomic_read(&orig->orig_node->last_ttvn);

	if (nla_put(msg, BATADV_ATTR_TT_ADDRESS, ETH_ALEN, common->addr) ||
	    nla_put(msg, BATADV_ATTR_ORIG_ADDRESS, ETH_ALEN,
		    orig->orig_node->orig) ||
	    nla_put_u8(msg, BATADV_ATTR_TT_TTVN, orig->ttvn) ||
	    nla_put_u8(msg, BATADV_ATTR_TT_LAST_TTVN, last_ttvn) ||
	    nla_put_u32(msg, BATADV_ATTR_TT_CRC32, crc) ||
	    nla_put_u16(msg, BATADV_ATTR_TT_VID, common->vid) ||
	    nla_put_u32(msg, BATADV_ATTR_TT_FLAGS, flags))
		goto nla_put_failure;

	if (best && nla_put_flag(msg, BATADV_ATTR_FLAG_BEST))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	return 0;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

/**
 * batadv_tt_global_dump_entry - Dump one TT global entry into a message
 * @msg: Netlink message to dump into
 * @portid: Port making netlink request
 * @seq: Sequence number of netlink message
 * @bat_priv: The bat priv with all the soft interface information
 * @common: tt local & tt global common data
 * @sub_s: Number of entries to skip
 *
 * This function assumes the caller holds rcu_read_lock().
 *
 * Return: Error code, or 0 on success
 */
static int
batadv_tt_global_dump_entry(struct sk_buff *msg, u32 portid, u32 seq,
			    struct batadv_priv *bat_priv,
			    struct batadv_tt_common_entry *common, int *sub_s)
{
	struct batadv_tt_orig_list_entry *orig_entry, *best_entry;
	struct batadv_tt_global_entry *global;
	struct hlist_head *head;
	int sub = 0;
	bool best;

	global = container_of(common, struct batadv_tt_global_entry, common);
	best_entry = batadv_transtable_best_orig(bat_priv, global);
	head = &global->orig_list;

	hlist_for_each_entry_rcu(orig_entry, head, list) {
		if (sub++ < *sub_s)
			continue;

		best = (orig_entry == best_entry);

		if (batadv_tt_global_dump_subentry(msg, portid, seq, common,
						   orig_entry, best)) {
			*sub_s = sub - 1;
			return -EMSGSIZE;
		}
	}

	*sub_s = 0;
	return 0;
}

/**
 * batadv_tt_global_dump_bucket - Dump one TT local bucket into a message
 * @msg: Netlink message to dump into
 * @portid: Port making netlink request
 * @seq: Sequence number of netlink message
 * @bat_priv: The bat priv with all the soft interface information
 * @head: Pointer to the list containing the global tt entries
 * @idx_s: Number of entries to skip
 * @sub: Number of entries to skip
 *
 * Return: Error code, or 0 on success
 */
static int
batadv_tt_global_dump_bucket(struct sk_buff *msg, u32 portid, u32 seq,
			     struct batadv_priv *bat_priv,
			     struct hlist_head *head, int *idx_s, int *sub)
{
	struct batadv_tt_common_entry *common;
	int idx = 0;

	rcu_read_lock();
	hlist_for_each_entry_rcu(common, head, hash_entry) {
		if (idx++ < *idx_s)
			continue;

		if (batadv_tt_global_dump_entry(msg, portid, seq, bat_priv,
						common, sub)) {
			rcu_read_unlock();
			*idx_s = idx - 1;
			return -EMSGSIZE;
		}
	}
	rcu_read_unlock();

	*idx_s = 0;
	*sub = 0;
	return 0;
}

/**
 * batadv_tt_global_dump -  Dump TT global entries into a message
 * @msg: Netlink message to dump into
 * @cb: Parameters from query
 *
 * Return: Error code, or length of message on success
 */
int batadv_tt_global_dump(struct sk_buff *msg, struct netlink_callback *cb)
{
	struct net *net = sock_net(cb->skb->sk);
	struct net_device *soft_iface;
	struct batadv_priv *bat_priv;
	struct batadv_hard_iface *primary_if = NULL;
	struct batadv_hashtable *hash;
	struct hlist_head *head;
	int ret;
	int ifindex;
	int bucket = cb->args[0];
	int idx = cb->args[1];
	int sub = cb->args[2];
	int portid = NETLINK_CB(cb->skb).portid;

	ifindex = batadv_netlink_get_ifindex(cb->nlh, BATADV_ATTR_MESH_IFINDEX);
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

	hash = bat_priv->tt.global_hash;

	while (bucket < hash->size) {
		head = &hash->table[bucket];

		if (batadv_tt_global_dump_bucket(msg, portid,
						 cb->nlh->nlmsg_seq, bat_priv,
						 head, &idx, &sub))
			break;

		bucket++;
	}

	ret = msg->len;

 out:
	if (primary_if)
		batadv_hardif_put(primary_if);
	if (soft_iface)
		dev_put(soft_iface);

	cb->args[0] = bucket;
	cb->args[1] = idx;
	cb->args[2] = sub;

	return ret;
}

/**
 * _batadv_tt_global_del_orig_entry - remove and free an orig_entry
 * @tt_global_entry: the global entry to remove the orig_entry from
 * @orig_entry: the orig entry to remove and free
 *
 * Remove an orig_entry from its list in the given tt_global_entry and
 * free this orig_entry afterwards.
 *
 * Caller must hold tt_global_entry->list_lock and ensure orig_entry->list is
 * part of a list.
 */
static void
_batadv_tt_global_del_orig_entry(struct batadv_tt_global_entry *tt_global_entry,
				 struct batadv_tt_orig_list_entry *orig_entry)
{
	lockdep_assert_held(&tt_global_entry->list_lock);

	batadv_tt_global_size_dec(orig_entry->orig_node,
				  tt_global_entry->common.vid);
	atomic_dec(&tt_global_entry->orig_list_count);
	/* requires holding tt_global_entry->list_lock and orig_entry->list
	 * being part of a list
	 */
	hlist_del_rcu(&orig_entry->list);
	batadv_tt_orig_list_entry_put(orig_entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* deletes the orig list of a tt_global_entry */
static void
batadv_tt_global_del_orig_list(struct batadv_tt_global_entry *tt_global_entry)
{
	struct hlist_head *head;
	struct hlist_node *safe;
	struct batadv_tt_orig_list_entry *orig_entry;

	spin_lock_bh(&tt_global_entry->list_lock);
	head = &tt_global_entry->orig_list;
<<<<<<< HEAD
	hlist_for_each_entry_safe(orig_entry, safe, head, list) {
		hlist_del_rcu(&orig_entry->list);
		batadv_tt_orig_list_entry_free_ref(orig_entry);
	}
	spin_unlock_bh(&tt_global_entry->list_lock);
}

static void
batadv_tt_global_del_orig_entry(struct batadv_priv *bat_priv,
				struct batadv_tt_global_entry *tt_global_entry,
				struct batadv_orig_node *orig_node,
				const char *message)
=======
	hlist_for_each_entry_safe(orig_entry, safe, head, list)
		_batadv_tt_global_del_orig_entry(tt_global_entry, orig_entry);
	spin_unlock_bh(&tt_global_entry->list_lock);
}

/**
 * batadv_tt_global_del_orig_node - remove orig_node from a global tt entry
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_global_entry: the global entry to remove the orig_node from
 * @orig_node: the originator announcing the client
 * @message: message to append to the log on deletion
 *
 * Remove the given orig_node and its according orig_entry from the given
 * global tt entry.
 */
static void
batadv_tt_global_del_orig_node(struct batadv_priv *bat_priv,
			       struct batadv_tt_global_entry *tt_global_entry,
			       struct batadv_orig_node *orig_node,
			       const char *message)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct hlist_head *head;
	struct hlist_node *safe;
	struct batadv_tt_orig_list_entry *orig_entry;
<<<<<<< HEAD
=======
	unsigned short vid;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_bh(&tt_global_entry->list_lock);
	head = &tt_global_entry->orig_list;
	hlist_for_each_entry_safe(orig_entry, safe, head, list) {
		if (orig_entry->orig_node == orig_node) {
<<<<<<< HEAD
			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "Deleting %pM from global tt entry %pM: %s\n",
				   orig_node->orig,
				   tt_global_entry->common.addr, message);
			hlist_del_rcu(&orig_entry->list);
			batadv_tt_orig_list_entry_free_ref(orig_entry);
=======
			vid = tt_global_entry->common.vid;
			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "Deleting %pM from global tt entry %pM (vid: %d): %s\n",
				   orig_node->orig,
				   tt_global_entry->common.addr,
				   BATADV_PRINT_VID(vid), message);
			_batadv_tt_global_del_orig_entry(tt_global_entry,
							 orig_entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
	spin_unlock_bh(&tt_global_entry->list_lock);
}

/* If the client is to be deleted, we check if it is the last origantor entry
 * within tt_global entry. If yes, we set the BATADV_TT_CLIENT_ROAM flag and the
 * timer, otherwise we simply remove the originator scheduled for deletion.
 */
static void
batadv_tt_global_del_roaming(struct batadv_priv *bat_priv,
			     struct batadv_tt_global_entry *tt_global_entry,
			     struct batadv_orig_node *orig_node,
			     const char *message)
{
	bool last_entry = true;
	struct hlist_head *head;
	struct batadv_tt_orig_list_entry *orig_entry;

	/* no local entry exists, case 1:
	 * Check if this is the last one or if other entries exist.
	 */

	rcu_read_lock();
	head = &tt_global_entry->orig_list;
	hlist_for_each_entry_rcu(orig_entry, head, list) {
		if (orig_entry->orig_node != orig_node) {
			last_entry = false;
			break;
		}
	}
	rcu_read_unlock();

	if (last_entry) {
		/* its the last one, mark for roaming. */
		tt_global_entry->common.flags |= BATADV_TT_CLIENT_ROAM;
		tt_global_entry->roam_at = jiffies;
	} else
		/* there is another entry, we can simply delete this
		 * one and can still use the other one.
		 */
<<<<<<< HEAD
		batadv_tt_global_del_orig_entry(bat_priv, tt_global_entry,
						orig_node, message);
}



static void batadv_tt_global_del(struct batadv_priv *bat_priv,
				 struct batadv_orig_node *orig_node,
				 const unsigned char *addr,
=======
		batadv_tt_global_del_orig_node(bat_priv, tt_global_entry,
					       orig_node, message);
}

/**
 * batadv_tt_global_del - remove a client from the global table
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: an originator serving this client
 * @addr: the mac address of the client
 * @vid: VLAN identifier
 * @message: a message explaining the reason for deleting the client to print
 *  for debugging purpose
 * @roaming: true if the deletion has been triggered by a roaming event
 */
static void batadv_tt_global_del(struct batadv_priv *bat_priv,
				 struct batadv_orig_node *orig_node,
				 const unsigned char *addr, unsigned short vid,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				 const char *message, bool roaming)
{
	struct batadv_tt_global_entry *tt_global_entry;
	struct batadv_tt_local_entry *local_entry = NULL;

<<<<<<< HEAD
	tt_global_entry = batadv_tt_global_hash_find(bat_priv, addr);
=======
	tt_global_entry = batadv_tt_global_hash_find(bat_priv, addr, vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!tt_global_entry)
		goto out;

	if (!roaming) {
<<<<<<< HEAD
		batadv_tt_global_del_orig_entry(bat_priv, tt_global_entry,
						orig_node, message);
=======
		batadv_tt_global_del_orig_node(bat_priv, tt_global_entry,
					       orig_node, message);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (hlist_empty(&tt_global_entry->orig_list))
			batadv_tt_global_free(bat_priv, tt_global_entry,
					      message);

		goto out;
	}

	/* if we are deleting a global entry due to a roam
	 * event, there are two possibilities:
	 * 1) the client roamed from node A to node B => if there
	 *    is only one originator left for this client, we mark
	 *    it with BATADV_TT_CLIENT_ROAM, we start a timer and we
	 *    wait for node B to claim it. In case of timeout
	 *    the entry is purged.
	 *
	 *    If there are other originators left, we directly delete
	 *    the originator.
	 * 2) the client roamed to us => we can directly delete
	 *    the global entry, since it is useless now.
	 */
	local_entry = batadv_tt_local_hash_find(bat_priv,
<<<<<<< HEAD
						tt_global_entry->common.addr);
=======
						tt_global_entry->common.addr,
						vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (local_entry) {
		/* local entry exists, case 2: client roamed to us. */
		batadv_tt_global_del_orig_list(tt_global_entry);
		batadv_tt_global_free(bat_priv, tt_global_entry, message);
	} else
		/* no local entry exists, case 1: check for roaming */
		batadv_tt_global_del_roaming(bat_priv, tt_global_entry,
					     orig_node, message);

<<<<<<< HEAD

out:
	if (tt_global_entry)
		batadv_tt_global_entry_free_ref(tt_global_entry);
	if (local_entry)
		batadv_tt_local_entry_free_ref(local_entry);
}

void batadv_tt_global_del_orig(struct batadv_priv *bat_priv,
			       struct batadv_orig_node *orig_node,
=======
out:
	if (tt_global_entry)
		batadv_tt_global_entry_put(tt_global_entry);
	if (local_entry)
		batadv_tt_local_entry_put(local_entry);
}

/**
 * batadv_tt_global_del_orig - remove all the TT global entries belonging to the
 *  given originator matching the provided vid
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the originator owning the entries to remove
 * @match_vid: the VLAN identifier to match. If negative all the entries will be
 *  removed
 * @message: debug message to print as "reason"
 */
void batadv_tt_global_del_orig(struct batadv_priv *bat_priv,
			       struct batadv_orig_node *orig_node,
			       s32 match_vid,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			       const char *message)
{
	struct batadv_tt_global_entry *tt_global;
	struct batadv_tt_common_entry *tt_common_entry;
<<<<<<< HEAD
	uint32_t i;
=======
	u32 i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct batadv_hashtable *hash = bat_priv->tt.global_hash;
	struct hlist_node *safe;
	struct hlist_head *head;
	spinlock_t *list_lock; /* protects write access to the hash lists */
<<<<<<< HEAD
=======
	unsigned short vid;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!hash)
		return;

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
		hlist_for_each_entry_safe(tt_common_entry, safe,
					  head, hash_entry) {
<<<<<<< HEAD
=======
			/* remove only matching entries */
			if (match_vid >= 0 && tt_common_entry->vid != match_vid)
				continue;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			tt_global = container_of(tt_common_entry,
						 struct batadv_tt_global_entry,
						 common);

<<<<<<< HEAD
			batadv_tt_global_del_orig_entry(bat_priv, tt_global,
							orig_node, message);

			if (hlist_empty(&tt_global->orig_list)) {
				batadv_dbg(BATADV_DBG_TT, bat_priv,
					   "Deleting global tt entry %pM: %s\n",
					   tt_global->common.addr, message);
				hlist_del_rcu(&tt_common_entry->hash_entry);
				batadv_tt_global_entry_free_ref(tt_global);
=======
			batadv_tt_global_del_orig_node(bat_priv, tt_global,
						       orig_node, message);

			if (hlist_empty(&tt_global->orig_list)) {
				vid = tt_global->common.vid;
				batadv_dbg(BATADV_DBG_TT, bat_priv,
					   "Deleting global tt entry %pM (vid: %d): %s\n",
					   tt_global->common.addr,
					   BATADV_PRINT_VID(vid), message);
				hlist_del_rcu(&tt_common_entry->hash_entry);
				batadv_tt_global_entry_put(tt_global);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			}
		}
		spin_unlock_bh(list_lock);
	}
<<<<<<< HEAD
	orig_node->tt_initialised = false;
=======
	clear_bit(BATADV_ORIG_CAPA_HAS_TT, &orig_node->capa_initialized);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static bool batadv_tt_global_to_purge(struct batadv_tt_global_entry *tt_global,
				      char **msg)
{
	bool purge = false;
	unsigned long roam_timeout = BATADV_TT_CLIENT_ROAM_TIMEOUT;
	unsigned long temp_timeout = BATADV_TT_CLIENT_TEMP_TIMEOUT;

	if ((tt_global->common.flags & BATADV_TT_CLIENT_ROAM) &&
	    batadv_has_timed_out(tt_global->roam_at, roam_timeout)) {
		purge = true;
		*msg = "Roaming timeout\n";
	}

	if ((tt_global->common.flags & BATADV_TT_CLIENT_TEMP) &&
	    batadv_has_timed_out(tt_global->common.added_at, temp_timeout)) {
		purge = true;
		*msg = "Temporary client timeout\n";
	}

	return purge;
}

static void batadv_tt_global_purge(struct batadv_priv *bat_priv)
{
	struct batadv_hashtable *hash = bat_priv->tt.global_hash;
	struct hlist_head *head;
	struct hlist_node *node_tmp;
	spinlock_t *list_lock; /* protects write access to the hash lists */
<<<<<<< HEAD
	uint32_t i;
=======
	u32 i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	char *msg = NULL;
	struct batadv_tt_common_entry *tt_common;
	struct batadv_tt_global_entry *tt_global;

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
		hlist_for_each_entry_safe(tt_common, node_tmp, head,
					  hash_entry) {
			tt_global = container_of(tt_common,
						 struct batadv_tt_global_entry,
						 common);

			if (!batadv_tt_global_to_purge(tt_global, &msg))
				continue;

			batadv_dbg(BATADV_DBG_TT, bat_priv,
<<<<<<< HEAD
				   "Deleting global tt entry (%pM): %s\n",
				   tt_global->common.addr, msg);

			hlist_del_rcu(&tt_common->hash_entry);

			batadv_tt_global_entry_free_ref(tt_global);
=======
				   "Deleting global tt entry %pM (vid: %d): %s\n",
				   tt_global->common.addr,
				   BATADV_PRINT_VID(tt_global->common.vid),
				   msg);

			hlist_del_rcu(&tt_common->hash_entry);

			batadv_tt_global_entry_put(tt_global);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		spin_unlock_bh(list_lock);
	}
}

static void batadv_tt_global_table_free(struct batadv_priv *bat_priv)
{
	struct batadv_hashtable *hash;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_global_entry *tt_global;
	struct hlist_node *node_tmp;
	struct hlist_head *head;
<<<<<<< HEAD
	uint32_t i;
=======
	u32 i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!bat_priv->tt.global_hash)
		return;

	hash = bat_priv->tt.global_hash;

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
		hlist_for_each_entry_safe(tt_common_entry, node_tmp,
					  head, hash_entry) {
			hlist_del_rcu(&tt_common_entry->hash_entry);
			tt_global = container_of(tt_common_entry,
						 struct batadv_tt_global_entry,
						 common);
<<<<<<< HEAD
			batadv_tt_global_entry_free_ref(tt_global);
=======
			batadv_tt_global_entry_put(tt_global);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		spin_unlock_bh(list_lock);
	}

	batadv_hash_destroy(hash);

	bat_priv->tt.global_hash = NULL;
}

static bool
_batadv_is_ap_isolated(struct batadv_tt_local_entry *tt_local_entry,
		       struct batadv_tt_global_entry *tt_global_entry)
{
	bool ret = false;

	if (tt_local_entry->common.flags & BATADV_TT_CLIENT_WIFI &&
	    tt_global_entry->common.flags & BATADV_TT_CLIENT_WIFI)
		ret = true;

<<<<<<< HEAD
	return ret;
}

struct batadv_orig_node *batadv_transtable_search(struct batadv_priv *bat_priv,
						  const uint8_t *src,
						  const uint8_t *addr)
=======
	/* check if the two clients are marked as isolated */
	if (tt_local_entry->common.flags & BATADV_TT_CLIENT_ISOLA &&
	    tt_global_entry->common.flags & BATADV_TT_CLIENT_ISOLA)
		ret = true;

	return ret;
}

/**
 * batadv_transtable_search - get the mesh destination for a given client
 * @bat_priv: the bat priv with all the soft interface information
 * @src: mac address of the source client
 * @addr: mac address of the destination client
 * @vid: VLAN identifier
 *
 * Return: a pointer to the originator that was selected as destination in the
 * mesh for contacting the client 'addr', NULL otherwise.
 * In case of multiple originators serving the same client, the function returns
 * the best one (best in terms of metric towards the destination node).
 *
 * If the two clients are AP isolated the function returns NULL.
 */
struct batadv_orig_node *batadv_transtable_search(struct batadv_priv *bat_priv,
						  const u8 *src,
						  const u8 *addr,
						  unsigned short vid)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct batadv_tt_local_entry *tt_local_entry = NULL;
	struct batadv_tt_global_entry *tt_global_entry = NULL;
	struct batadv_orig_node *orig_node = NULL;
	struct batadv_tt_orig_list_entry *best_entry;

<<<<<<< HEAD
	if (src && atomic_read(&bat_priv->ap_isolation)) {
		tt_local_entry = batadv_tt_local_hash_find(bat_priv, src);
=======
	if (src && batadv_vlan_ap_isola_get(bat_priv, vid)) {
		tt_local_entry = batadv_tt_local_hash_find(bat_priv, src, vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!tt_local_entry ||
		    (tt_local_entry->common.flags & BATADV_TT_CLIENT_PENDING))
			goto out;
	}

<<<<<<< HEAD
	tt_global_entry = batadv_tt_global_hash_find(bat_priv, addr);
=======
	tt_global_entry = batadv_tt_global_hash_find(bat_priv, addr, vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!tt_global_entry)
		goto out;

	/* check whether the clients should not communicate due to AP
	 * isolation
	 */
	if (tt_local_entry &&
	    _batadv_is_ap_isolated(tt_local_entry, tt_global_entry))
		goto out;

	rcu_read_lock();
<<<<<<< HEAD
	best_entry = batadv_transtable_best_orig(tt_global_entry);
	/* found anything? */
	if (best_entry)
		orig_node = best_entry->orig_node;
	if (orig_node && !atomic_inc_not_zero(&orig_node->refcount))
=======
	best_entry = batadv_transtable_best_orig(bat_priv, tt_global_entry);
	/* found anything? */
	if (best_entry)
		orig_node = best_entry->orig_node;
	if (orig_node && !kref_get_unless_zero(&orig_node->refcount))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		orig_node = NULL;
	rcu_read_unlock();

out:
	if (tt_global_entry)
<<<<<<< HEAD
		batadv_tt_global_entry_free_ref(tt_global_entry);
	if (tt_local_entry)
		batadv_tt_local_entry_free_ref(tt_local_entry);
=======
		batadv_tt_global_entry_put(tt_global_entry);
	if (tt_local_entry)
		batadv_tt_local_entry_put(tt_local_entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return orig_node;
}

<<<<<<< HEAD
/* Calculates the checksum of the local table of a given orig_node */
static uint16_t batadv_tt_global_crc(struct batadv_priv *bat_priv,
				     struct batadv_orig_node *orig_node)
{
	uint16_t total = 0, total_one;
	struct batadv_hashtable *hash = bat_priv->tt.global_hash;
	struct batadv_tt_common_entry *tt_common;
	struct batadv_tt_global_entry *tt_global;
	struct hlist_head *head;
	uint32_t i;
	int j;
=======
/**
 * batadv_tt_global_crc - calculates the checksum of the local table belonging
 *  to the given orig_node
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: originator for which the CRC should be computed
 * @vid: VLAN identifier for which the CRC32 has to be computed
 *
 * This function computes the checksum for the global table corresponding to a
 * specific originator. In particular, the checksum is computed as follows: For
 * each client connected to the originator the CRC32C of the MAC address and the
 * VID is computed and then all the CRC32Cs of the various clients are xor'ed
 * together.
 *
 * The idea behind is that CRC32C should be used as much as possible in order to
 * produce a unique hash of the table, but since the order which is used to feed
 * the CRC32C function affects the result and since every node in the network
 * probably sorts the clients differently, the hash function cannot be directly
 * computed over the entire table. Hence the CRC32C is used only on
 * the single client entry, while all the results are then xor'ed together
 * because the XOR operation can combine them all while trying to reduce the
 * noise as much as possible.
 *
 * Return: the checksum of the global table of a given originator.
 */
static u32 batadv_tt_global_crc(struct batadv_priv *bat_priv,
				struct batadv_orig_node *orig_node,
				unsigned short vid)
{
	struct batadv_hashtable *hash = bat_priv->tt.global_hash;
	struct batadv_tt_orig_list_entry *tt_orig;
	struct batadv_tt_common_entry *tt_common;
	struct batadv_tt_global_entry *tt_global;
	struct hlist_head *head;
	u32 i, crc_tmp, crc = 0;
	u8 flags;
	__be16 tmp_vid;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		hlist_for_each_entry_rcu(tt_common, head, hash_entry) {
			tt_global = container_of(tt_common,
						 struct batadv_tt_global_entry,
						 common);
<<<<<<< HEAD
=======
			/* compute the CRC only for entries belonging to the
			 * VLAN identified by the vid passed as parameter
			 */
			if (tt_common->vid != vid)
				continue;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* Roaming clients are in the global table for
			 * consistency only. They don't have to be
			 * taken into account while computing the
			 * global crc
			 */
			if (tt_common->flags & BATADV_TT_CLIENT_ROAM)
				continue;
			/* Temporary clients have not been announced yet, so
			 * they have to be skipped while computing the global
			 * crc
			 */
			if (tt_common->flags & BATADV_TT_CLIENT_TEMP)
				continue;

			/* find out if this global entry is announced by this
			 * originator
			 */
<<<<<<< HEAD
			if (!batadv_tt_global_entry_has_orig(tt_global,
							     orig_node))
				continue;

			total_one = 0;
			for (j = 0; j < ETH_ALEN; j++)
				total_one = crc16_byte(total_one,
						       tt_common->addr[j]);
			total ^= total_one;
=======
			tt_orig = batadv_tt_global_orig_entry_find(tt_global,
								   orig_node);
			if (!tt_orig)
				continue;

			/* use network order to read the VID: this ensures that
			 * every node reads the bytes in the same order.
			 */
			tmp_vid = htons(tt_common->vid);
			crc_tmp = crc32c(0, &tmp_vid, sizeof(tmp_vid));

			/* compute the CRC on flags that have to be kept in sync
			 * among nodes
			 */
			flags = tt_orig->flags;
			crc_tmp = crc32c(crc_tmp, &flags, sizeof(flags));

			crc ^= crc32c(crc_tmp, tt_common->addr, ETH_ALEN);

			batadv_tt_orig_list_entry_put(tt_orig);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		rcu_read_unlock();
	}

<<<<<<< HEAD
	return total;
}

/* Calculates the checksum of the local table */
static uint16_t batadv_tt_local_crc(struct batadv_priv *bat_priv)
{
	uint16_t total = 0, total_one;
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
	struct batadv_tt_common_entry *tt_common;
	struct hlist_head *head;
	uint32_t i;
	int j;
=======
	return crc;
}

/**
 * batadv_tt_local_crc - calculates the checksum of the local table
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: VLAN identifier for which the CRC32 has to be computed
 *
 * For details about the computation, please refer to the documentation for
 * batadv_tt_global_crc().
 *
 * Return: the checksum of the local table
 */
static u32 batadv_tt_local_crc(struct batadv_priv *bat_priv,
			       unsigned short vid)
{
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
	struct batadv_tt_common_entry *tt_common;
	struct hlist_head *head;
	u32 i, crc_tmp, crc = 0;
	u8 flags;
	__be16 tmp_vid;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		hlist_for_each_entry_rcu(tt_common, head, hash_entry) {
<<<<<<< HEAD
=======
			/* compute the CRC only for entries belonging to the
			 * VLAN identified by vid
			 */
			if (tt_common->vid != vid)
				continue;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* not yet committed clients have not to be taken into
			 * account while computing the CRC
			 */
			if (tt_common->flags & BATADV_TT_CLIENT_NEW)
				continue;
<<<<<<< HEAD
			total_one = 0;
			for (j = 0; j < ETH_ALEN; j++)
				total_one = crc16_byte(total_one,
						       tt_common->addr[j]);
			total ^= total_one;
=======

			/* use network order to read the VID: this ensures that
			 * every node reads the bytes in the same order.
			 */
			tmp_vid = htons(tt_common->vid);
			crc_tmp = crc32c(0, &tmp_vid, sizeof(tmp_vid));

			/* compute the CRC on flags that have to be kept in sync
			 * among nodes
			 */
			flags = tt_common->flags & BATADV_TT_SYNC_MASK;
			crc_tmp = crc32c(crc_tmp, &flags, sizeof(flags));

			crc ^= crc32c(crc_tmp, tt_common->addr, ETH_ALEN);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		rcu_read_unlock();
	}

<<<<<<< HEAD
	return total;
=======
	return crc;
}

/**
 * batadv_tt_req_node_release - free tt_req node entry
 * @ref: kref pointer of the tt req_node entry
 */
static void batadv_tt_req_node_release(struct kref *ref)
{
	struct batadv_tt_req_node *tt_req_node;

	tt_req_node = container_of(ref, struct batadv_tt_req_node, refcount);

	kmem_cache_free(batadv_tt_req_cache, tt_req_node);
}

/**
 * batadv_tt_req_node_put - decrement the tt_req_node refcounter and
 *  possibly release it
 * @tt_req_node: tt_req_node to be free'd
 */
static void batadv_tt_req_node_put(struct batadv_tt_req_node *tt_req_node)
{
	kref_put(&tt_req_node->refcount, batadv_tt_req_node_release);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void batadv_tt_req_list_free(struct batadv_priv *bat_priv)
{
<<<<<<< HEAD
	struct batadv_tt_req_node *node, *safe;

	spin_lock_bh(&bat_priv->tt.req_list_lock);

	list_for_each_entry_safe(node, safe, &bat_priv->tt.req_list, list) {
		list_del(&node->list);
		kfree(node);
=======
	struct batadv_tt_req_node *node;
	struct hlist_node *safe;

	spin_lock_bh(&bat_priv->tt.req_list_lock);

	hlist_for_each_entry_safe(node, safe, &bat_priv->tt.req_list, list) {
		hlist_del_init(&node->list);
		batadv_tt_req_node_put(node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	spin_unlock_bh(&bat_priv->tt.req_list_lock);
}

static void batadv_tt_save_orig_buffer(struct batadv_priv *bat_priv,
				       struct batadv_orig_node *orig_node,
<<<<<<< HEAD
				       const unsigned char *tt_buff,
				       uint8_t tt_num_changes)
{
	uint16_t tt_buff_len = batadv_tt_len(tt_num_changes);

=======
				       const void *tt_buff,
				       u16 tt_buff_len)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Replace the old buffer only if I received something in the
	 * last OGM (the OGM could carry no changes)
	 */
	spin_lock_bh(&orig_node->tt_buff_lock);
	if (tt_buff_len > 0) {
		kfree(orig_node->tt_buff);
		orig_node->tt_buff_len = 0;
		orig_node->tt_buff = kmalloc(tt_buff_len, GFP_ATOMIC);
		if (orig_node->tt_buff) {
			memcpy(orig_node->tt_buff, tt_buff, tt_buff_len);
			orig_node->tt_buff_len = tt_buff_len;
		}
	}
	spin_unlock_bh(&orig_node->tt_buff_lock);
}

static void batadv_tt_req_purge(struct batadv_priv *bat_priv)
{
<<<<<<< HEAD
	struct batadv_tt_req_node *node, *safe;

	spin_lock_bh(&bat_priv->tt.req_list_lock);
	list_for_each_entry_safe(node, safe, &bat_priv->tt.req_list, list) {
		if (batadv_has_timed_out(node->issued_at,
					 BATADV_TT_REQUEST_TIMEOUT)) {
			list_del(&node->list);
			kfree(node);
=======
	struct batadv_tt_req_node *node;
	struct hlist_node *safe;

	spin_lock_bh(&bat_priv->tt.req_list_lock);
	hlist_for_each_entry_safe(node, safe, &bat_priv->tt.req_list, list) {
		if (batadv_has_timed_out(node->issued_at,
					 BATADV_TT_REQUEST_TIMEOUT)) {
			hlist_del_init(&node->list);
			batadv_tt_req_node_put(node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
	spin_unlock_bh(&bat_priv->tt.req_list_lock);
}

<<<<<<< HEAD
/* returns the pointer to the new tt_req_node struct if no request
 * has already been issued for this orig_node, NULL otherwise
 */
static struct batadv_tt_req_node *
batadv_new_tt_req_node(struct batadv_priv *bat_priv,
=======
/**
 * batadv_tt_req_node_new - search and possibly create a tt_req_node object
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: orig node this request is being issued for
 *
 * Return: the pointer to the new tt_req_node struct if no request
 * has already been issued for this orig_node, NULL otherwise.
 */
static struct batadv_tt_req_node *
batadv_tt_req_node_new(struct batadv_priv *bat_priv,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		       struct batadv_orig_node *orig_node)
{
	struct batadv_tt_req_node *tt_req_node_tmp, *tt_req_node = NULL;

	spin_lock_bh(&bat_priv->tt.req_list_lock);
<<<<<<< HEAD
	list_for_each_entry(tt_req_node_tmp, &bat_priv->tt.req_list, list) {
=======
	hlist_for_each_entry(tt_req_node_tmp, &bat_priv->tt.req_list, list) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (batadv_compare_eth(tt_req_node_tmp, orig_node) &&
		    !batadv_has_timed_out(tt_req_node_tmp->issued_at,
					  BATADV_TT_REQUEST_TIMEOUT))
			goto unlock;
	}

<<<<<<< HEAD
	tt_req_node = kmalloc(sizeof(*tt_req_node), GFP_ATOMIC);
	if (!tt_req_node)
		goto unlock;

	memcpy(tt_req_node->addr, orig_node->orig, ETH_ALEN);
	tt_req_node->issued_at = jiffies;

	list_add(&tt_req_node->list, &bat_priv->tt.req_list);
=======
	tt_req_node = kmem_cache_alloc(batadv_tt_req_cache, GFP_ATOMIC);
	if (!tt_req_node)
		goto unlock;

	kref_init(&tt_req_node->refcount);
	ether_addr_copy(tt_req_node->addr, orig_node->orig);
	tt_req_node->issued_at = jiffies;

	kref_get(&tt_req_node->refcount);
	hlist_add_head(&tt_req_node->list, &bat_priv->tt.req_list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
unlock:
	spin_unlock_bh(&bat_priv->tt.req_list_lock);
	return tt_req_node;
}

<<<<<<< HEAD
/* data_ptr is useless here, but has to be kept to respect the prototype */
static int batadv_tt_local_valid_entry(const void *entry_ptr,
				       const void *data_ptr)
=======
/**
 * batadv_tt_local_valid() - verify local tt entry and get flags
 * @entry_ptr: to be checked local tt entry
 * @data_ptr: not used but definition required to satisfy the callback prototype
 * @flags: a pointer to store TT flags for this client to
 *
 * Checks the validity of the given local TT entry. If it is, then the provided
 * flags pointer is updated.
 *
 * Return: true if the entry is a valid, false otherwise.
 */
static bool batadv_tt_local_valid(const void *entry_ptr,
				  const void *data_ptr,
				  u8 *flags)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	const struct batadv_tt_common_entry *tt_common_entry = entry_ptr;

	if (tt_common_entry->flags & BATADV_TT_CLIENT_NEW)
<<<<<<< HEAD
		return 0;
	return 1;
}

static int batadv_tt_global_valid(const void *entry_ptr,
				  const void *data_ptr)
=======
		return false;

	if (flags)
		*flags = tt_common_entry->flags;

	return true;
}

/**
 * batadv_tt_global_valid() - verify global tt entry and get flags
 * @entry_ptr: to be checked global tt entry
 * @data_ptr: an orig_node object (may be NULL)
 * @flags: a pointer to store TT flags for this client to
 *
 * Checks the validity of the given global TT entry. If it is, then the provided
 * flags pointer is updated either with the common (summed) TT flags if data_ptr
 * is NULL or the specific, per originator TT flags otherwise.
 *
 * Return: true if the entry is a valid, false otherwise.
 */
static bool batadv_tt_global_valid(const void *entry_ptr,
				   const void *data_ptr,
				   u8 *flags)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	const struct batadv_tt_common_entry *tt_common_entry = entry_ptr;
	const struct batadv_tt_global_entry *tt_global_entry;
	const struct batadv_orig_node *orig_node = data_ptr;

	if (tt_common_entry->flags & BATADV_TT_CLIENT_ROAM ||
	    tt_common_entry->flags & BATADV_TT_CLIENT_TEMP)
<<<<<<< HEAD
		return 0;
=======
		return false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	tt_global_entry = container_of(tt_common_entry,
				       struct batadv_tt_global_entry,
				       common);

<<<<<<< HEAD
	return batadv_tt_global_entry_has_orig(tt_global_entry, orig_node);
}

static struct sk_buff *
batadv_tt_response_fill_table(uint16_t tt_len, uint8_t ttvn,
			      struct batadv_hashtable *hash,
			      struct batadv_priv *bat_priv,
			      int (*valid_cb)(const void *, const void *),
			      void *cb_data)
{
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_query_packet *tt_response;
	struct batadv_tt_change *tt_change;
	struct hlist_head *head;
	struct sk_buff *skb = NULL;
	uint16_t tt_tot, tt_count;
	ssize_t tt_query_size = sizeof(struct batadv_tt_query_packet);
	uint32_t i;
	size_t len;

	if (tt_query_size + tt_len > bat_priv->soft_iface->mtu) {
		tt_len = bat_priv->soft_iface->mtu - tt_query_size;
		tt_len -= tt_len % sizeof(struct batadv_tt_change);
	}
	tt_tot = tt_len / sizeof(struct batadv_tt_change);

	len = tt_query_size + tt_len;
	skb = dev_alloc_skb(len + ETH_HLEN + NET_IP_ALIGN);
	if (!skb)
		goto out;

	skb_reserve(skb, ETH_HLEN + NET_IP_ALIGN);
	tt_response = (struct batadv_tt_query_packet *)skb_put(skb, len);
	tt_response->ttvn = ttvn;

	tt_change = (struct batadv_tt_change *)(skb->data + tt_query_size);
	tt_count = 0;
=======
	return batadv_tt_global_entry_has_orig(tt_global_entry, orig_node,
					       flags);
}

/**
 * batadv_tt_tvlv_generate - fill the tvlv buff with the tt entries from the
 *  specified tt hash
 * @bat_priv: the bat priv with all the soft interface information
 * @hash: hash table containing the tt entries
 * @tt_len: expected tvlv tt data buffer length in number of bytes
 * @tvlv_buff: pointer to the buffer to fill with the TT data
 * @valid_cb: function to filter tt change entries and to return TT flags
 * @cb_data: data passed to the filter function as argument
 *
 * Fills the tvlv buff with the tt entries from the specified hash. If valid_cb
 * is not provided then this becomes a no-op.
 */
static void batadv_tt_tvlv_generate(struct batadv_priv *bat_priv,
				    struct batadv_hashtable *hash,
				    void *tvlv_buff, u16 tt_len,
				    bool (*valid_cb)(const void *,
						     const void *,
						     u8 *flags),
				    void *cb_data)
{
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tvlv_tt_change *tt_change;
	struct hlist_head *head;
	u16 tt_tot, tt_num_entries = 0;
	u8 flags;
	bool ret;
	u32 i;

	tt_tot = batadv_tt_entries(tt_len);
	tt_change = (struct batadv_tvlv_tt_change *)tvlv_buff;

	if (!valid_cb)
		return;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rcu_read_lock();
	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		hlist_for_each_entry_rcu(tt_common_entry,
					 head, hash_entry) {
<<<<<<< HEAD
			if (tt_count == tt_tot)
				break;

			if ((valid_cb) && (!valid_cb(tt_common_entry, cb_data)))
				continue;

			memcpy(tt_change->addr, tt_common_entry->addr,
			       ETH_ALEN);
			tt_change->flags = tt_common_entry->flags;

			tt_count++;
=======
			if (tt_tot == tt_num_entries)
				break;

			ret = valid_cb(tt_common_entry, cb_data, &flags);
			if (!ret)
				continue;

			ether_addr_copy(tt_change->addr, tt_common_entry->addr);
			tt_change->flags = flags;
			tt_change->vid = htons(tt_common_entry->vid);
			memset(tt_change->reserved, 0,
			       sizeof(tt_change->reserved));

			tt_num_entries++;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			tt_change++;
		}
	}
	rcu_read_unlock();
<<<<<<< HEAD

	/* store in the message the number of entries we have successfully
	 * copied
	 */
	tt_response->tt_data = htons(tt_count);

out:
	return skb;
}

static int batadv_send_tt_request(struct batadv_priv *bat_priv,
				  struct batadv_orig_node *dst_orig_node,
				  uint8_t ttvn, uint16_t tt_crc,
				  bool full_table)
{
	struct sk_buff *skb = NULL;
	struct batadv_tt_query_packet *tt_request;
	struct batadv_hard_iface *primary_if;
	struct batadv_tt_req_node *tt_req_node = NULL;
	int ret = 1;
	size_t tt_req_len;
=======
}

/**
 * batadv_tt_global_check_crc - check if all the CRCs are correct
 * @orig_node: originator for which the CRCs have to be checked
 * @tt_vlan: pointer to the first tvlv VLAN entry
 * @num_vlan: number of tvlv VLAN entries
 *
 * Return: true if all the received CRCs match the locally stored ones, false
 * otherwise
 */
static bool batadv_tt_global_check_crc(struct batadv_orig_node *orig_node,
				       struct batadv_tvlv_tt_vlan_data *tt_vlan,
				       u16 num_vlan)
{
	struct batadv_tvlv_tt_vlan_data *tt_vlan_tmp;
	struct batadv_orig_node_vlan *vlan;
	int i, orig_num_vlan;
	u32 crc;

	/* check if each received CRC matches the locally stored one */
	for (i = 0; i < num_vlan; i++) {
		tt_vlan_tmp = tt_vlan + i;

		/* if orig_node is a backbone node for this VLAN, don't check
		 * the CRC as we ignore all the global entries over it
		 */
		if (batadv_bla_is_backbone_gw_orig(orig_node->bat_priv,
						   orig_node->orig,
						   ntohs(tt_vlan_tmp->vid)))
			continue;

		vlan = batadv_orig_node_vlan_get(orig_node,
						 ntohs(tt_vlan_tmp->vid));
		if (!vlan)
			return false;

		crc = vlan->tt.crc;
		batadv_orig_node_vlan_put(vlan);

		if (crc != ntohl(tt_vlan_tmp->crc))
			return false;
	}

	/* check if any excess VLANs exist locally for the originator
	 * which are not mentioned in the TVLV from the originator.
	 */
	rcu_read_lock();
	orig_num_vlan = 0;
	hlist_for_each_entry_rcu(vlan, &orig_node->vlan_list, list)
		orig_num_vlan++;
	rcu_read_unlock();

	if (orig_num_vlan > num_vlan)
		return false;

	return true;
}

/**
 * batadv_tt_local_update_crc - update all the local CRCs
 * @bat_priv: the bat priv with all the soft interface information
 */
static void batadv_tt_local_update_crc(struct batadv_priv *bat_priv)
{
	struct batadv_softif_vlan *vlan;

	/* recompute the global CRC for each VLAN */
	rcu_read_lock();
	hlist_for_each_entry_rcu(vlan, &bat_priv->softif_vlan_list, list) {
		vlan->tt.crc = batadv_tt_local_crc(bat_priv, vlan->vid);
	}
	rcu_read_unlock();
}

/**
 * batadv_tt_global_update_crc - update all the global CRCs for this orig_node
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the orig_node for which the CRCs have to be updated
 */
static void batadv_tt_global_update_crc(struct batadv_priv *bat_priv,
					struct batadv_orig_node *orig_node)
{
	struct batadv_orig_node_vlan *vlan;
	u32 crc;

	/* recompute the global CRC for each VLAN */
	rcu_read_lock();
	hlist_for_each_entry_rcu(vlan, &orig_node->vlan_list, list) {
		/* if orig_node is a backbone node for this VLAN, don't compute
		 * the CRC as we ignore all the global entries over it
		 */
		if (batadv_bla_is_backbone_gw_orig(bat_priv, orig_node->orig,
						   vlan->vid))
			continue;

		crc = batadv_tt_global_crc(bat_priv, orig_node, vlan->vid);
		vlan->tt.crc = crc;
	}
	rcu_read_unlock();
}

/**
 * batadv_send_tt_request - send a TT Request message to a given node
 * @bat_priv: the bat priv with all the soft interface information
 * @dst_orig_node: the destination of the message
 * @ttvn: the version number that the source of the message is looking for
 * @tt_vlan: pointer to the first tvlv VLAN object to request
 * @num_vlan: number of tvlv VLAN entries
 * @full_table: ask for the entire translation table if true, while only for the
 *  last TT diff otherwise
 *
 * Return: true if the TT Request was sent, false otherwise
 */
static bool batadv_send_tt_request(struct batadv_priv *bat_priv,
				   struct batadv_orig_node *dst_orig_node,
				   u8 ttvn,
				   struct batadv_tvlv_tt_vlan_data *tt_vlan,
				   u16 num_vlan, bool full_table)
{
	struct batadv_tvlv_tt_data *tvlv_tt_data = NULL;
	struct batadv_tt_req_node *tt_req_node = NULL;
	struct batadv_tvlv_tt_vlan_data *tt_vlan_req;
	struct batadv_hard_iface *primary_if;
	bool ret = false;
	int i, size;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	/* The new tt_req will be issued only if I'm not waiting for a
	 * reply from the same orig_node yet
	 */
<<<<<<< HEAD
	tt_req_node = batadv_new_tt_req_node(bat_priv, dst_orig_node);
	if (!tt_req_node)
		goto out;

	skb = dev_alloc_skb(sizeof(*tt_request) + ETH_HLEN + NET_IP_ALIGN);
	if (!skb)
		goto out;

	skb_reserve(skb, ETH_HLEN + NET_IP_ALIGN);

	tt_req_len = sizeof(*tt_request);
	tt_request = (struct batadv_tt_query_packet *)skb_put(skb, tt_req_len);

	tt_request->header.packet_type = BATADV_TT_QUERY;
	tt_request->header.version = BATADV_COMPAT_VERSION;
	memcpy(tt_request->src, primary_if->net_dev->dev_addr, ETH_ALEN);
	memcpy(tt_request->dst, dst_orig_node->orig, ETH_ALEN);
	tt_request->header.ttl = BATADV_TTL;
	tt_request->ttvn = ttvn;
	tt_request->tt_data = htons(tt_crc);
	tt_request->flags = BATADV_TT_REQUEST;

	if (full_table)
		tt_request->flags |= BATADV_TT_FULL_TABLE;

	batadv_dbg(BATADV_DBG_TT, bat_priv, "Sending TT_REQUEST to %pM [%c]\n",
		   dst_orig_node->orig, (full_table ? 'F' : '.'));

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_REQUEST_TX);

	if (batadv_send_skb_to_orig(skb, dst_orig_node, NULL))
		ret = 0;

out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	if (ret)
		kfree_skb(skb);
	if (ret && tt_req_node) {
		spin_lock_bh(&bat_priv->tt.req_list_lock);
		list_del(&tt_req_node->list);
		spin_unlock_bh(&bat_priv->tt.req_list_lock);
		kfree(tt_req_node);
	}
	return ret;
}

static bool
batadv_send_other_tt_response(struct batadv_priv *bat_priv,
			      struct batadv_tt_query_packet *tt_request)
{
	struct batadv_orig_node *req_dst_orig_node;
	struct batadv_orig_node *res_dst_orig_node = NULL;
	uint8_t orig_ttvn, req_ttvn, ttvn;
	int ret = false;
	unsigned char *tt_buff;
	bool full_table;
	uint16_t tt_len, tt_tot;
	struct sk_buff *skb = NULL;
	struct batadv_tt_query_packet *tt_response;
	uint8_t *packet_pos;
	size_t len;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received TT_REQUEST from %pM for ttvn: %u (%pM) [%c]\n",
		   tt_request->src, tt_request->ttvn, tt_request->dst,
		   (tt_request->flags & BATADV_TT_FULL_TABLE ? 'F' : '.'));

	/* Let's get the orig node of the REAL destination */
	req_dst_orig_node = batadv_orig_hash_find(bat_priv, tt_request->dst);
	if (!req_dst_orig_node)
		goto out;

	res_dst_orig_node = batadv_orig_hash_find(bat_priv, tt_request->src);
	if (!res_dst_orig_node)
		goto out;

	orig_ttvn = (uint8_t)atomic_read(&req_dst_orig_node->last_ttvn);
	req_ttvn = tt_request->ttvn;

	/* I don't have the requested data */
	if (orig_ttvn != req_ttvn ||
	    tt_request->tt_data != htons(req_dst_orig_node->tt_crc))
		goto out;

	/* If the full table has been explicitly requested */
	if (tt_request->flags & BATADV_TT_FULL_TABLE ||
=======
	tt_req_node = batadv_tt_req_node_new(bat_priv, dst_orig_node);
	if (!tt_req_node)
		goto out;

	size = sizeof(*tvlv_tt_data) + sizeof(*tt_vlan_req) * num_vlan;
	tvlv_tt_data = kzalloc(size, GFP_ATOMIC);
	if (!tvlv_tt_data)
		goto out;

	tvlv_tt_data->flags = BATADV_TT_REQUEST;
	tvlv_tt_data->ttvn = ttvn;
	tvlv_tt_data->num_vlan = htons(num_vlan);

	/* send all the CRCs within the request. This is needed by intermediate
	 * nodes to ensure they have the correct table before replying
	 */
	tt_vlan_req = (struct batadv_tvlv_tt_vlan_data *)(tvlv_tt_data + 1);
	for (i = 0; i < num_vlan; i++) {
		tt_vlan_req->vid = tt_vlan->vid;
		tt_vlan_req->crc = tt_vlan->crc;

		tt_vlan_req++;
		tt_vlan++;
	}

	if (full_table)
		tvlv_tt_data->flags |= BATADV_TT_FULL_TABLE;

	batadv_dbg(BATADV_DBG_TT, bat_priv, "Sending TT_REQUEST to %pM [%c]\n",
		   dst_orig_node->orig, full_table ? 'F' : '.');

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_REQUEST_TX);
	batadv_tvlv_unicast_send(bat_priv, primary_if->net_dev->dev_addr,
				 dst_orig_node->orig, BATADV_TVLV_TT, 1,
				 tvlv_tt_data, size);
	ret = true;

out:
	if (primary_if)
		batadv_hardif_put(primary_if);

	if (ret && tt_req_node) {
		spin_lock_bh(&bat_priv->tt.req_list_lock);
		if (!hlist_unhashed(&tt_req_node->list)) {
			hlist_del_init(&tt_req_node->list);
			batadv_tt_req_node_put(tt_req_node);
		}
		spin_unlock_bh(&bat_priv->tt.req_list_lock);
	}

	if (tt_req_node)
		batadv_tt_req_node_put(tt_req_node);

	kfree(tvlv_tt_data);
	return ret;
}

/**
 * batadv_send_other_tt_response - send reply to tt request concerning another
 *  node's translation table
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 * @req_dst: mac address of tt request recipient
 *
 * Return: true if tt request reply was sent, false otherwise.
 */
static bool batadv_send_other_tt_response(struct batadv_priv *bat_priv,
					  struct batadv_tvlv_tt_data *tt_data,
					  u8 *req_src, u8 *req_dst)
{
	struct batadv_orig_node *req_dst_orig_node;
	struct batadv_orig_node *res_dst_orig_node = NULL;
	struct batadv_tvlv_tt_change *tt_change;
	struct batadv_tvlv_tt_data *tvlv_tt_data = NULL;
	struct batadv_tvlv_tt_vlan_data *tt_vlan;
	bool ret = false, full_table;
	u8 orig_ttvn, req_ttvn;
	u16 tvlv_len;
	s32 tt_len;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received TT_REQUEST from %pM for ttvn: %u (%pM) [%c]\n",
		   req_src, tt_data->ttvn, req_dst,
		   ((tt_data->flags & BATADV_TT_FULL_TABLE) ? 'F' : '.'));

	/* Let's get the orig node of the REAL destination */
	req_dst_orig_node = batadv_orig_hash_find(bat_priv, req_dst);
	if (!req_dst_orig_node)
		goto out;

	res_dst_orig_node = batadv_orig_hash_find(bat_priv, req_src);
	if (!res_dst_orig_node)
		goto out;

	orig_ttvn = (u8)atomic_read(&req_dst_orig_node->last_ttvn);
	req_ttvn = tt_data->ttvn;

	tt_vlan = (struct batadv_tvlv_tt_vlan_data *)(tt_data + 1);
	/* this node doesn't have the requested data */
	if (orig_ttvn != req_ttvn ||
	    !batadv_tt_global_check_crc(req_dst_orig_node, tt_vlan,
					ntohs(tt_data->num_vlan)))
		goto out;

	/* If the full table has been explicitly requested */
	if (tt_data->flags & BATADV_TT_FULL_TABLE ||
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	    !req_dst_orig_node->tt_buff)
		full_table = true;
	else
		full_table = false;

<<<<<<< HEAD
	/* In this version, fragmentation is not implemented, then
	 * I'll send only one packet with as much TT entries as I can
=======
	/* TT fragmentation hasn't been implemented yet, so send as many
	 * TT entries fit a single packet as possible only
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 */
	if (!full_table) {
		spin_lock_bh(&req_dst_orig_node->tt_buff_lock);
		tt_len = req_dst_orig_node->tt_buff_len;
<<<<<<< HEAD
		tt_tot = tt_len / sizeof(struct batadv_tt_change);

		len = sizeof(*tt_response) + tt_len;
		skb = dev_alloc_skb(len + ETH_HLEN + NET_IP_ALIGN);
		if (!skb)
			goto unlock;

		skb_reserve(skb, ETH_HLEN + NET_IP_ALIGN);
		packet_pos = skb_put(skb, len);
		tt_response = (struct batadv_tt_query_packet *)packet_pos;
		tt_response->ttvn = req_ttvn;
		tt_response->tt_data = htons(tt_tot);

		tt_buff = skb->data + sizeof(*tt_response);
		/* Copy the last orig_node's OGM buffer */
		memcpy(tt_buff, req_dst_orig_node->tt_buff,
		       req_dst_orig_node->tt_buff_len);

		spin_unlock_bh(&req_dst_orig_node->tt_buff_lock);
	} else {
		tt_len = (uint16_t)atomic_read(&req_dst_orig_node->tt_size);
		tt_len *= sizeof(struct batadv_tt_change);
		ttvn = (uint8_t)atomic_read(&req_dst_orig_node->last_ttvn);

		skb = batadv_tt_response_fill_table(tt_len, ttvn,
						    bat_priv->tt.global_hash,
						    bat_priv,
						    batadv_tt_global_valid,
						    req_dst_orig_node);
		if (!skb)
			goto out;

		tt_response = (struct batadv_tt_query_packet *)skb->data;
	}

	tt_response->header.packet_type = BATADV_TT_QUERY;
	tt_response->header.version = BATADV_COMPAT_VERSION;
	tt_response->header.ttl = BATADV_TTL;
	memcpy(tt_response->src, req_dst_orig_node->orig, ETH_ALEN);
	memcpy(tt_response->dst, tt_request->src, ETH_ALEN);
	tt_response->flags = BATADV_TT_RESPONSE;

	if (full_table)
		tt_response->flags |= BATADV_TT_FULL_TABLE;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Sending TT_RESPONSE %pM for %pM (ttvn: %u)\n",
		   res_dst_orig_node->orig, req_dst_orig_node->orig, req_ttvn);

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_RESPONSE_TX);

	if (batadv_send_skb_to_orig(skb, res_dst_orig_node, NULL))
		ret = true;
=======

		tvlv_len = batadv_tt_prepare_tvlv_global_data(req_dst_orig_node,
							      &tvlv_tt_data,
							      &tt_change,
							      &tt_len);
		if (!tt_len)
			goto unlock;

		/* Copy the last orig_node's OGM buffer */
		memcpy(tt_change, req_dst_orig_node->tt_buff,
		       req_dst_orig_node->tt_buff_len);
		spin_unlock_bh(&req_dst_orig_node->tt_buff_lock);
	} else {
		/* allocate the tvlv, put the tt_data and all the tt_vlan_data
		 * in the initial part
		 */
		tt_len = -1;
		tvlv_len = batadv_tt_prepare_tvlv_global_data(req_dst_orig_node,
							      &tvlv_tt_data,
							      &tt_change,
							      &tt_len);
		if (!tt_len)
			goto out;

		/* fill the rest of the tvlv with the real TT entries */
		batadv_tt_tvlv_generate(bat_priv, bat_priv->tt.global_hash,
					tt_change, tt_len,
					batadv_tt_global_valid,
					req_dst_orig_node);
	}

	/* Don't send the response, if larger than fragmented packet. */
	tt_len = sizeof(struct batadv_unicast_tvlv_packet) + tvlv_len;
	if (tt_len > atomic_read(&bat_priv->packet_size_max)) {
		net_ratelimited_function(batadv_info, bat_priv->soft_iface,
					 "Ignoring TT_REQUEST from %pM; Response size exceeds max packet size.\n",
					 res_dst_orig_node->orig);
		goto out;
	}

	tvlv_tt_data->flags = BATADV_TT_RESPONSE;
	tvlv_tt_data->ttvn = req_ttvn;

	if (full_table)
		tvlv_tt_data->flags |= BATADV_TT_FULL_TABLE;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Sending TT_RESPONSE %pM for %pM [%c] (ttvn: %u)\n",
		   res_dst_orig_node->orig, req_dst_orig_node->orig,
		   full_table ? 'F' : '.', req_ttvn);

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_RESPONSE_TX);

	batadv_tvlv_unicast_send(bat_priv, req_dst_orig_node->orig,
				 req_src, BATADV_TVLV_TT, 1, tvlv_tt_data,
				 tvlv_len);

	ret = true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	goto out;

unlock:
	spin_unlock_bh(&req_dst_orig_node->tt_buff_lock);

out:
	if (res_dst_orig_node)
<<<<<<< HEAD
		batadv_orig_node_free_ref(res_dst_orig_node);
	if (req_dst_orig_node)
		batadv_orig_node_free_ref(req_dst_orig_node);
	if (!ret)
		kfree_skb(skb);
	return ret;
}

static bool
batadv_send_my_tt_response(struct batadv_priv *bat_priv,
			   struct batadv_tt_query_packet *tt_request)
{
	struct batadv_orig_node *orig_node;
	struct batadv_hard_iface *primary_if = NULL;
	uint8_t my_ttvn, req_ttvn, ttvn;
	int ret = false;
	unsigned char *tt_buff;
	bool full_table;
	uint16_t tt_len, tt_tot;
	struct sk_buff *skb = NULL;
	struct batadv_tt_query_packet *tt_response;
	uint8_t *packet_pos;
	size_t len;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received TT_REQUEST from %pM for ttvn: %u (me) [%c]\n",
		   tt_request->src, tt_request->ttvn,
		   (tt_request->flags & BATADV_TT_FULL_TABLE ? 'F' : '.'));


	my_ttvn = (uint8_t)atomic_read(&bat_priv->tt.vn);
	req_ttvn = tt_request->ttvn;

	orig_node = batadv_orig_hash_find(bat_priv, tt_request->src);
=======
		batadv_orig_node_put(res_dst_orig_node);
	if (req_dst_orig_node)
		batadv_orig_node_put(req_dst_orig_node);
	kfree(tvlv_tt_data);
	return ret;
}

/**
 * batadv_send_my_tt_response - send reply to tt request concerning this node's
 *  translation table
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 *
 * Return: true if tt request reply was sent, false otherwise.
 */
static bool batadv_send_my_tt_response(struct batadv_priv *bat_priv,
				       struct batadv_tvlv_tt_data *tt_data,
				       u8 *req_src)
{
	struct batadv_tvlv_tt_data *tvlv_tt_data = NULL;
	struct batadv_hard_iface *primary_if = NULL;
	struct batadv_tvlv_tt_change *tt_change;
	struct batadv_orig_node *orig_node;
	u8 my_ttvn, req_ttvn;
	u16 tvlv_len;
	bool full_table;
	s32 tt_len;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received TT_REQUEST from %pM for ttvn: %u (me) [%c]\n",
		   req_src, tt_data->ttvn,
		   ((tt_data->flags & BATADV_TT_FULL_TABLE) ? 'F' : '.'));

	spin_lock_bh(&bat_priv->tt.commit_lock);

	my_ttvn = (u8)atomic_read(&bat_priv->tt.vn);
	req_ttvn = tt_data->ttvn;

	orig_node = batadv_orig_hash_find(bat_priv, req_src);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!orig_node)
		goto out;

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	/* If the full table has been explicitly requested or the gap
	 * is too big send the whole local translation table
	 */
<<<<<<< HEAD
	if (tt_request->flags & BATADV_TT_FULL_TABLE || my_ttvn != req_ttvn ||
=======
	if (tt_data->flags & BATADV_TT_FULL_TABLE || my_ttvn != req_ttvn ||
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	    !bat_priv->tt.last_changeset)
		full_table = true;
	else
		full_table = false;

<<<<<<< HEAD
	/* In this version, fragmentation is not implemented, then
	 * I'll send only one packet with as much TT entries as I can
	 */
	if (!full_table) {
		spin_lock_bh(&bat_priv->tt.last_changeset_lock);
		tt_len = bat_priv->tt.last_changeset_len;
		tt_tot = tt_len / sizeof(struct batadv_tt_change);

		len = sizeof(*tt_response) + tt_len;
		skb = dev_alloc_skb(len + ETH_HLEN + NET_IP_ALIGN);
		if (!skb)
			goto unlock;

		skb_reserve(skb, ETH_HLEN + NET_IP_ALIGN);
		packet_pos = skb_put(skb, len);
		tt_response = (struct batadv_tt_query_packet *)packet_pos;
		tt_response->ttvn = req_ttvn;
		tt_response->tt_data = htons(tt_tot);

		tt_buff = skb->data + sizeof(*tt_response);
		memcpy(tt_buff, bat_priv->tt.last_changeset,
		       bat_priv->tt.last_changeset_len);
		spin_unlock_bh(&bat_priv->tt.last_changeset_lock);
	} else {
		tt_len = (uint16_t)atomic_read(&bat_priv->tt.local_entry_num);
		tt_len *= sizeof(struct batadv_tt_change);
		ttvn = (uint8_t)atomic_read(&bat_priv->tt.vn);

		skb = batadv_tt_response_fill_table(tt_len, ttvn,
						    bat_priv->tt.local_hash,
						    bat_priv,
						    batadv_tt_local_valid_entry,
						    NULL);
		if (!skb)
			goto out;

		tt_response = (struct batadv_tt_query_packet *)skb->data;
	}

	tt_response->header.packet_type = BATADV_TT_QUERY;
	tt_response->header.version = BATADV_COMPAT_VERSION;
	tt_response->header.ttl = BATADV_TTL;
	memcpy(tt_response->src, primary_if->net_dev->dev_addr, ETH_ALEN);
	memcpy(tt_response->dst, tt_request->src, ETH_ALEN);
	tt_response->flags = BATADV_TT_RESPONSE;

	if (full_table)
		tt_response->flags |= BATADV_TT_FULL_TABLE;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Sending TT_RESPONSE to %pM [%c]\n",
		   orig_node->orig,
		   (tt_response->flags & BATADV_TT_FULL_TABLE ? 'F' : '.'));

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_RESPONSE_TX);

	if (batadv_send_skb_to_orig(skb, orig_node, NULL))
		ret = true;
=======
	/* TT fragmentation hasn't been implemented yet, so send as many
	 * TT entries fit a single packet as possible only
	 */
	if (!full_table) {
		spin_lock_bh(&bat_priv->tt.last_changeset_lock);

		tt_len = bat_priv->tt.last_changeset_len;
		tvlv_len = batadv_tt_prepare_tvlv_local_data(bat_priv,
							     &tvlv_tt_data,
							     &tt_change,
							     &tt_len);
		if (!tt_len || !tvlv_len)
			goto unlock;

		/* Copy the last orig_node's OGM buffer */
		memcpy(tt_change, bat_priv->tt.last_changeset,
		       bat_priv->tt.last_changeset_len);
		spin_unlock_bh(&bat_priv->tt.last_changeset_lock);
	} else {
		req_ttvn = (u8)atomic_read(&bat_priv->tt.vn);

		/* allocate the tvlv, put the tt_data and all the tt_vlan_data
		 * in the initial part
		 */
		tt_len = -1;
		tvlv_len = batadv_tt_prepare_tvlv_local_data(bat_priv,
							     &tvlv_tt_data,
							     &tt_change,
							     &tt_len);
		if (!tt_len || !tvlv_len)
			goto out;

		/* fill the rest of the tvlv with the real TT entries */
		batadv_tt_tvlv_generate(bat_priv, bat_priv->tt.local_hash,
					tt_change, tt_len,
					batadv_tt_local_valid, NULL);
	}

	tvlv_tt_data->flags = BATADV_TT_RESPONSE;
	tvlv_tt_data->ttvn = req_ttvn;

	if (full_table)
		tvlv_tt_data->flags |= BATADV_TT_FULL_TABLE;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Sending TT_RESPONSE to %pM [%c] (ttvn: %u)\n",
		   orig_node->orig, full_table ? 'F' : '.', req_ttvn);

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_RESPONSE_TX);

	batadv_tvlv_unicast_send(bat_priv, primary_if->net_dev->dev_addr,
				 req_src, BATADV_TVLV_TT, 1, tvlv_tt_data,
				 tvlv_len);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	goto out;

unlock:
	spin_unlock_bh(&bat_priv->tt.last_changeset_lock);
out:
<<<<<<< HEAD
	if (orig_node)
		batadv_orig_node_free_ref(orig_node);
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	if (!ret)
		kfree_skb(skb);
	/* This packet was for me, so it doesn't need to be re-routed */
	return true;
}

bool batadv_send_tt_response(struct batadv_priv *bat_priv,
			     struct batadv_tt_query_packet *tt_request)
{
	if (batadv_is_my_mac(bat_priv, tt_request->dst)) {
		/* don't answer backbone gws! */
		if (batadv_bla_is_backbone_gw_orig(bat_priv, tt_request->src))
			return true;

		return batadv_send_my_tt_response(bat_priv, tt_request);
	} else {
		return batadv_send_other_tt_response(bat_priv, tt_request);
	}
=======
	spin_unlock_bh(&bat_priv->tt.commit_lock);
	if (orig_node)
		batadv_orig_node_put(orig_node);
	if (primary_if)
		batadv_hardif_put(primary_if);
	kfree(tvlv_tt_data);
	/* The packet was for this host, so it doesn't need to be re-routed */
	return true;
}

/**
 * batadv_send_tt_response - send reply to tt request
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 * @req_dst: mac address of tt request recipient
 *
 * Return: true if tt request reply was sent, false otherwise.
 */
static bool batadv_send_tt_response(struct batadv_priv *bat_priv,
				    struct batadv_tvlv_tt_data *tt_data,
				    u8 *req_src, u8 *req_dst)
{
	if (batadv_is_my_mac(bat_priv, req_dst))
		return batadv_send_my_tt_response(bat_priv, tt_data, req_src);
	return batadv_send_other_tt_response(bat_priv, tt_data, req_src,
					     req_dst);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void _batadv_tt_update_changes(struct batadv_priv *bat_priv,
				      struct batadv_orig_node *orig_node,
<<<<<<< HEAD
				      struct batadv_tt_change *tt_change,
				      uint16_t tt_num_changes, uint8_t ttvn)
=======
				      struct batadv_tvlv_tt_change *tt_change,
				      u16 tt_num_changes, u8 ttvn)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int i;
	int roams;

	for (i = 0; i < tt_num_changes; i++) {
		if ((tt_change + i)->flags & BATADV_TT_CLIENT_DEL) {
			roams = (tt_change + i)->flags & BATADV_TT_CLIENT_ROAM;
			batadv_tt_global_del(bat_priv, orig_node,
					     (tt_change + i)->addr,
<<<<<<< HEAD
=======
					     ntohs((tt_change + i)->vid),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					     "tt removed by changes",
					     roams);
		} else {
			if (!batadv_tt_global_add(bat_priv, orig_node,
						  (tt_change + i)->addr,
<<<<<<< HEAD
=======
						  ntohs((tt_change + i)->vid),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						  (tt_change + i)->flags, ttvn))
				/* In case of problem while storing a
				 * global_entry, we stop the updating
				 * procedure without committing the
				 * ttvn change. This will avoid to send
				 * corrupted data on tt_request
				 */
				return;
		}
	}
<<<<<<< HEAD
	orig_node->tt_initialised = true;
}

static void batadv_tt_fill_gtable(struct batadv_priv *bat_priv,
				  struct batadv_tt_query_packet *tt_response)
{
	struct batadv_orig_node *orig_node;

	orig_node = batadv_orig_hash_find(bat_priv, tt_response->src);
=======
	set_bit(BATADV_ORIG_CAPA_HAS_TT, &orig_node->capa_initialized);
}

static void batadv_tt_fill_gtable(struct batadv_priv *bat_priv,
				  struct batadv_tvlv_tt_change *tt_change,
				  u8 ttvn, u8 *resp_src,
				  u16 num_entries)
{
	struct batadv_orig_node *orig_node;

	orig_node = batadv_orig_hash_find(bat_priv, resp_src);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!orig_node)
		goto out;

	/* Purge the old table first.. */
<<<<<<< HEAD
	batadv_tt_global_del_orig(bat_priv, orig_node, "Received full table");

	_batadv_tt_update_changes(bat_priv, orig_node,
				  (struct batadv_tt_change *)(tt_response + 1),
				  ntohs(tt_response->tt_data),
				  tt_response->ttvn);
=======
	batadv_tt_global_del_orig(bat_priv, orig_node, -1,
				  "Received full table");

	_batadv_tt_update_changes(bat_priv, orig_node, tt_change, num_entries,
				  ttvn);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_bh(&orig_node->tt_buff_lock);
	kfree(orig_node->tt_buff);
	orig_node->tt_buff_len = 0;
	orig_node->tt_buff = NULL;
	spin_unlock_bh(&orig_node->tt_buff_lock);

<<<<<<< HEAD
	atomic_set(&orig_node->last_ttvn, tt_response->ttvn);

out:
	if (orig_node)
		batadv_orig_node_free_ref(orig_node);
=======
	atomic_set(&orig_node->last_ttvn, ttvn);

out:
	if (orig_node)
		batadv_orig_node_put(orig_node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void batadv_tt_update_changes(struct batadv_priv *bat_priv,
				     struct batadv_orig_node *orig_node,
<<<<<<< HEAD
				     uint16_t tt_num_changes, uint8_t ttvn,
				     struct batadv_tt_change *tt_change)
=======
				     u16 tt_num_changes, u8 ttvn,
				     struct batadv_tvlv_tt_change *tt_change)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	_batadv_tt_update_changes(bat_priv, orig_node, tt_change,
				  tt_num_changes, ttvn);

<<<<<<< HEAD
	batadv_tt_save_orig_buffer(bat_priv, orig_node,
				   (unsigned char *)tt_change, tt_num_changes);
	atomic_set(&orig_node->last_ttvn, ttvn);
}

bool batadv_is_my_client(struct batadv_priv *bat_priv, const uint8_t *addr)
=======
	batadv_tt_save_orig_buffer(bat_priv, orig_node, tt_change,
				   batadv_tt_len(tt_num_changes));
	atomic_set(&orig_node->last_ttvn, ttvn);
}

/**
 * batadv_is_my_client - check if a client is served by the local node
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client to check
 * @vid: VLAN identifier
 *
 * Return: true if the client is served by this node, false otherwise.
 */
bool batadv_is_my_client(struct batadv_priv *bat_priv, const u8 *addr,
			 unsigned short vid)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct batadv_tt_local_entry *tt_local_entry;
	bool ret = false;

<<<<<<< HEAD
	tt_local_entry = batadv_tt_local_hash_find(bat_priv, addr);
=======
	tt_local_entry = batadv_tt_local_hash_find(bat_priv, addr, vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!tt_local_entry)
		goto out;
	/* Check if the client has been logically deleted (but is kept for
	 * consistency purpose)
	 */
	if ((tt_local_entry->common.flags & BATADV_TT_CLIENT_PENDING) ||
	    (tt_local_entry->common.flags & BATADV_TT_CLIENT_ROAM))
		goto out;
	ret = true;
out:
	if (tt_local_entry)
<<<<<<< HEAD
		batadv_tt_local_entry_free_ref(tt_local_entry);
	return ret;
}

void batadv_handle_tt_response(struct batadv_priv *bat_priv,
			       struct batadv_tt_query_packet *tt_response)
{
	struct batadv_tt_req_node *node, *safe;
	struct batadv_orig_node *orig_node = NULL;
	struct batadv_tt_change *tt_change;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received TT_RESPONSE from %pM for ttvn %d t_size: %d [%c]\n",
		   tt_response->src, tt_response->ttvn,
		   ntohs(tt_response->tt_data),
		   (tt_response->flags & BATADV_TT_FULL_TABLE ? 'F' : '.'));

	/* we should have never asked a backbone gw */
	if (batadv_bla_is_backbone_gw_orig(bat_priv, tt_response->src))
		goto out;

	orig_node = batadv_orig_hash_find(bat_priv, tt_response->src);
	if (!orig_node)
		goto out;

	if (tt_response->flags & BATADV_TT_FULL_TABLE) {
		batadv_tt_fill_gtable(bat_priv, tt_response);
	} else {
		tt_change = (struct batadv_tt_change *)(tt_response + 1);
		batadv_tt_update_changes(bat_priv, orig_node,
					 ntohs(tt_response->tt_data),
					 tt_response->ttvn, tt_change);
	}

	/* Delete the tt_req_node from pending tt_requests list */
	spin_lock_bh(&bat_priv->tt.req_list_lock);
	list_for_each_entry_safe(node, safe, &bat_priv->tt.req_list, list) {
		if (!batadv_compare_eth(node->addr, tt_response->src))
			continue;
		list_del(&node->list);
		kfree(node);
	}
	spin_unlock_bh(&bat_priv->tt.req_list_lock);

	/* Recalculate the CRC for this orig_node and store it */
	orig_node->tt_crc = batadv_tt_global_crc(bat_priv, orig_node);
out:
	if (orig_node)
		batadv_orig_node_free_ref(orig_node);
}

int batadv_tt_init(struct batadv_priv *bat_priv)
{
	int ret;

	ret = batadv_tt_local_init(bat_priv);
	if (ret < 0)
		return ret;

	ret = batadv_tt_global_init(bat_priv);
	if (ret < 0)
		return ret;

	INIT_DELAYED_WORK(&bat_priv->tt.work, batadv_tt_purge);
	queue_delayed_work(batadv_event_workqueue, &bat_priv->tt.work,
			   msecs_to_jiffies(BATADV_TT_WORK_PERIOD));

	return 1;
=======
		batadv_tt_local_entry_put(tt_local_entry);
	return ret;
}

/**
 * batadv_handle_tt_response - process incoming tt reply
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @resp_src: mac address of tt reply sender
 * @num_entries: number of tt change entries appended to the tt data
 */
static void batadv_handle_tt_response(struct batadv_priv *bat_priv,
				      struct batadv_tvlv_tt_data *tt_data,
				      u8 *resp_src, u16 num_entries)
{
	struct batadv_tt_req_node *node;
	struct hlist_node *safe;
	struct batadv_orig_node *orig_node = NULL;
	struct batadv_tvlv_tt_change *tt_change;
	u8 *tvlv_ptr = (u8 *)tt_data;
	u16 change_offset;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received TT_RESPONSE from %pM for ttvn %d t_size: %d [%c]\n",
		   resp_src, tt_data->ttvn, num_entries,
		   ((tt_data->flags & BATADV_TT_FULL_TABLE) ? 'F' : '.'));

	orig_node = batadv_orig_hash_find(bat_priv, resp_src);
	if (!orig_node)
		goto out;

	spin_lock_bh(&orig_node->tt_lock);

	change_offset = sizeof(struct batadv_tvlv_tt_vlan_data);
	change_offset *= ntohs(tt_data->num_vlan);
	change_offset += sizeof(*tt_data);
	tvlv_ptr += change_offset;

	tt_change = (struct batadv_tvlv_tt_change *)tvlv_ptr;
	if (tt_data->flags & BATADV_TT_FULL_TABLE) {
		batadv_tt_fill_gtable(bat_priv, tt_change, tt_data->ttvn,
				      resp_src, num_entries);
	} else {
		batadv_tt_update_changes(bat_priv, orig_node, num_entries,
					 tt_data->ttvn, tt_change);
	}

	/* Recalculate the CRC for this orig_node and store it */
	batadv_tt_global_update_crc(bat_priv, orig_node);

	spin_unlock_bh(&orig_node->tt_lock);

	/* Delete the tt_req_node from pending tt_requests list */
	spin_lock_bh(&bat_priv->tt.req_list_lock);
	hlist_for_each_entry_safe(node, safe, &bat_priv->tt.req_list, list) {
		if (!batadv_compare_eth(node->addr, resp_src))
			continue;
		hlist_del_init(&node->list);
		batadv_tt_req_node_put(node);
	}

	spin_unlock_bh(&bat_priv->tt.req_list_lock);
out:
	if (orig_node)
		batadv_orig_node_put(orig_node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void batadv_tt_roam_list_free(struct batadv_priv *bat_priv)
{
	struct batadv_tt_roam_node *node, *safe;

	spin_lock_bh(&bat_priv->tt.roam_list_lock);

	list_for_each_entry_safe(node, safe, &bat_priv->tt.roam_list, list) {
		list_del(&node->list);
<<<<<<< HEAD
		kfree(node);
=======
		kmem_cache_free(batadv_tt_roam_cache, node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	spin_unlock_bh(&bat_priv->tt.roam_list_lock);
}

static void batadv_tt_roam_purge(struct batadv_priv *bat_priv)
{
	struct batadv_tt_roam_node *node, *safe;

	spin_lock_bh(&bat_priv->tt.roam_list_lock);
	list_for_each_entry_safe(node, safe, &bat_priv->tt.roam_list, list) {
		if (!batadv_has_timed_out(node->first_time,
					  BATADV_ROAMING_MAX_TIME))
			continue;

		list_del(&node->list);
<<<<<<< HEAD
		kfree(node);
=======
		kmem_cache_free(batadv_tt_roam_cache, node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	spin_unlock_bh(&bat_priv->tt.roam_list_lock);
}

<<<<<<< HEAD
/* This function checks whether the client already reached the
 * maximum number of possible roaming phases. In this case the ROAMING_ADV
 * will not be sent.
 *
 * returns true if the ROAMING_ADV can be sent, false otherwise
 */
static bool batadv_tt_check_roam_count(struct batadv_priv *bat_priv,
				       uint8_t *client)
=======
/**
 * batadv_tt_check_roam_count - check if a client has roamed too frequently
 * @bat_priv: the bat priv with all the soft interface information
 * @client: mac address of the roaming client
 *
 * This function checks whether the client already reached the
 * maximum number of possible roaming phases. In this case the ROAMING_ADV
 * will not be sent.
 *
 * Return: true if the ROAMING_ADV can be sent, false otherwise
 */
static bool batadv_tt_check_roam_count(struct batadv_priv *bat_priv, u8 *client)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct batadv_tt_roam_node *tt_roam_node;
	bool ret = false;

	spin_lock_bh(&bat_priv->tt.roam_list_lock);
	/* The new tt_req will be issued only if I'm not waiting for a
	 * reply from the same orig_node yet
	 */
	list_for_each_entry(tt_roam_node, &bat_priv->tt.roam_list, list) {
		if (!batadv_compare_eth(tt_roam_node->addr, client))
			continue;

		if (batadv_has_timed_out(tt_roam_node->first_time,
					 BATADV_ROAMING_MAX_TIME))
			continue;

		if (!batadv_atomic_dec_not_zero(&tt_roam_node->counter))
			/* Sorry, you roamed too many times! */
			goto unlock;
		ret = true;
		break;
	}

	if (!ret) {
<<<<<<< HEAD
		tt_roam_node = kmalloc(sizeof(*tt_roam_node), GFP_ATOMIC);
=======
		tt_roam_node = kmem_cache_alloc(batadv_tt_roam_cache,
						GFP_ATOMIC);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!tt_roam_node)
			goto unlock;

		tt_roam_node->first_time = jiffies;
		atomic_set(&tt_roam_node->counter,
			   BATADV_ROAMING_MAX_COUNT - 1);
<<<<<<< HEAD
		memcpy(tt_roam_node->addr, client, ETH_ALEN);
=======
		ether_addr_copy(tt_roam_node->addr, client);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		list_add(&tt_roam_node->list, &bat_priv->tt.roam_list);
		ret = true;
	}

unlock:
	spin_unlock_bh(&bat_priv->tt.roam_list_lock);
	return ret;
}

<<<<<<< HEAD
static void batadv_send_roam_adv(struct batadv_priv *bat_priv, uint8_t *client,
				 struct batadv_orig_node *orig_node)
{
	struct sk_buff *skb = NULL;
	struct batadv_roam_adv_packet *roam_adv_packet;
	int ret = 1;
	struct batadv_hard_iface *primary_if;
	size_t len = sizeof(*roam_adv_packet);
=======
/**
 * batadv_send_roam_adv - send a roaming advertisement message
 * @bat_priv: the bat priv with all the soft interface information
 * @client: mac address of the roaming client
 * @vid: VLAN identifier
 * @orig_node: message destination
 *
 * Send a ROAMING_ADV message to the node which was previously serving this
 * client. This is done to inform the node that from now on all traffic destined
 * for this particular roamed client has to be forwarded to the sender of the
 * roaming message.
 */
static void batadv_send_roam_adv(struct batadv_priv *bat_priv, u8 *client,
				 unsigned short vid,
				 struct batadv_orig_node *orig_node)
{
	struct batadv_hard_iface *primary_if;
	struct batadv_tvlv_roam_adv tvlv_roam;

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* before going on we have to check whether the client has
	 * already roamed to us too many times
	 */
	if (!batadv_tt_check_roam_count(bat_priv, client))
		goto out;

<<<<<<< HEAD
	skb = dev_alloc_skb(sizeof(*roam_adv_packet) + ETH_HLEN + NET_IP_ALIGN);
	if (!skb)
		goto out;

	skb_reserve(skb, ETH_HLEN + NET_IP_ALIGN);

	roam_adv_packet = (struct batadv_roam_adv_packet *)skb_put(skb, len);

	roam_adv_packet->header.packet_type = BATADV_ROAM_ADV;
	roam_adv_packet->header.version = BATADV_COMPAT_VERSION;
	roam_adv_packet->header.ttl = BATADV_TTL;
	roam_adv_packet->reserved = 0;
	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;
	memcpy(roam_adv_packet->src, primary_if->net_dev->dev_addr, ETH_ALEN);
	batadv_hardif_free_ref(primary_if);
	memcpy(roam_adv_packet->dst, orig_node->orig, ETH_ALEN);
	memcpy(roam_adv_packet->client, client, ETH_ALEN);

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Sending ROAMING_ADV to %pM (client %pM)\n",
		   orig_node->orig, client);

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_ROAM_ADV_TX);

	if (batadv_send_skb_to_orig(skb, orig_node, NULL))
		ret = 0;

out:
	if (ret && skb)
		kfree_skb(skb);
	return;
=======
	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Sending ROAMING_ADV to %pM (client %pM, vid: %d)\n",
		   orig_node->orig, client, BATADV_PRINT_VID(vid));

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_ROAM_ADV_TX);

	memcpy(tvlv_roam.client, client, sizeof(tvlv_roam.client));
	tvlv_roam.vid = htons(vid);

	batadv_tvlv_unicast_send(bat_priv, primary_if->net_dev->dev_addr,
				 orig_node->orig, BATADV_TVLV_ROAM, 1,
				 &tvlv_roam, sizeof(tvlv_roam));

out:
	if (primary_if)
		batadv_hardif_put(primary_if);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void batadv_tt_purge(struct work_struct *work)
{
	struct delayed_work *delayed_work;
	struct batadv_priv_tt *priv_tt;
	struct batadv_priv *bat_priv;

<<<<<<< HEAD
	delayed_work = container_of(work, struct delayed_work, work);
	priv_tt = container_of(delayed_work, struct batadv_priv_tt, work);
	bat_priv = container_of(priv_tt, struct batadv_priv, tt);

	batadv_tt_local_purge(bat_priv);
=======
	delayed_work = to_delayed_work(work);
	priv_tt = container_of(delayed_work, struct batadv_priv_tt, work);
	bat_priv = container_of(priv_tt, struct batadv_priv, tt);

	batadv_tt_local_purge(bat_priv, BATADV_TT_LOCAL_TIMEOUT);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	batadv_tt_global_purge(bat_priv);
	batadv_tt_req_purge(bat_priv);
	batadv_tt_roam_purge(bat_priv);

	queue_delayed_work(batadv_event_workqueue, &bat_priv->tt.work,
			   msecs_to_jiffies(BATADV_TT_WORK_PERIOD));
}

void batadv_tt_free(struct batadv_priv *bat_priv)
{
<<<<<<< HEAD
=======
	batadv_tvlv_handler_unregister(bat_priv, BATADV_TVLV_ROAM, 1);

	batadv_tvlv_container_unregister(bat_priv, BATADV_TVLV_TT, 1);
	batadv_tvlv_handler_unregister(bat_priv, BATADV_TVLV_TT, 1);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cancel_delayed_work_sync(&bat_priv->tt.work);

	batadv_tt_local_table_free(bat_priv);
	batadv_tt_global_table_free(bat_priv);
	batadv_tt_req_list_free(bat_priv);
	batadv_tt_changes_list_free(bat_priv);
	batadv_tt_roam_list_free(bat_priv);

	kfree(bat_priv->tt.last_changeset);
}

<<<<<<< HEAD
/* This function will enable or disable the specified flags for all the entries
 * in the given hash table and returns the number of modified entries
 */
static uint16_t batadv_tt_set_flags(struct batadv_hashtable *hash,
				    uint16_t flags, bool enable)
{
	uint32_t i;
	uint16_t changed_num = 0;
	struct hlist_head *head;
	struct batadv_tt_common_entry *tt_common_entry;

	if (!hash)
		goto out;
=======
/**
 * batadv_tt_local_set_flags - set or unset the specified flags on the local
 *  table and possibly count them in the TT size
 * @bat_priv: the bat priv with all the soft interface information
 * @flags: the flag to switch
 * @enable: whether to set or unset the flag
 * @count: whether to increase the TT size by the number of changed entries
 */
static void batadv_tt_local_set_flags(struct batadv_priv *bat_priv, u16 flags,
				      bool enable, bool count)
{
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
	struct batadv_tt_common_entry *tt_common_entry;
	u16 changed_num = 0;
	struct hlist_head *head;
	u32 i;

	if (!hash)
		return;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		hlist_for_each_entry_rcu(tt_common_entry,
					 head, hash_entry) {
			if (enable) {
				if ((tt_common_entry->flags & flags) == flags)
					continue;
				tt_common_entry->flags |= flags;
			} else {
				if (!(tt_common_entry->flags & flags))
					continue;
				tt_common_entry->flags &= ~flags;
			}
			changed_num++;
<<<<<<< HEAD
		}
		rcu_read_unlock();
	}
out:
	return changed_num;
=======

			if (!count)
				continue;

			batadv_tt_local_size_inc(bat_priv,
						 tt_common_entry->vid);
		}
		rcu_read_unlock();
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Purge out all the tt local entries marked with BATADV_TT_CLIENT_PENDING */
static void batadv_tt_local_purge_pending_clients(struct batadv_priv *bat_priv)
{
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
	struct batadv_tt_common_entry *tt_common;
	struct batadv_tt_local_entry *tt_local;
	struct hlist_node *node_tmp;
	struct hlist_head *head;
	spinlock_t *list_lock; /* protects write access to the hash lists */
<<<<<<< HEAD
	uint32_t i;
=======
	u32 i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!hash)
		return;

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
		hlist_for_each_entry_safe(tt_common, node_tmp, head,
					  hash_entry) {
			if (!(tt_common->flags & BATADV_TT_CLIENT_PENDING))
				continue;

			batadv_dbg(BATADV_DBG_TT, bat_priv,
<<<<<<< HEAD
				   "Deleting local tt entry (%pM): pending\n",
				   tt_common->addr);

			atomic_dec(&bat_priv->tt.local_entry_num);
=======
				   "Deleting local tt entry (%pM, vid: %d): pending\n",
				   tt_common->addr,
				   BATADV_PRINT_VID(tt_common->vid));

			batadv_tt_local_size_dec(bat_priv, tt_common->vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			hlist_del_rcu(&tt_common->hash_entry);
			tt_local = container_of(tt_common,
						struct batadv_tt_local_entry,
						common);
<<<<<<< HEAD
			batadv_tt_local_entry_free_ref(tt_local);
=======

			batadv_tt_local_entry_put(tt_local);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		spin_unlock_bh(list_lock);
	}
}

<<<<<<< HEAD
static int batadv_tt_commit_changes(struct batadv_priv *bat_priv,
				    unsigned char **packet_buff,
				    int *packet_buff_len, int packet_min_len)
{
	uint16_t changed_num = 0;

	if (atomic_read(&bat_priv->tt.local_changes) < 1)
		return -ENOENT;

	changed_num = batadv_tt_set_flags(bat_priv->tt.local_hash,
					  BATADV_TT_CLIENT_NEW, false);

	/* all reset entries have to be counted as local entries */
	atomic_add(changed_num, &bat_priv->tt.local_entry_num);
	batadv_tt_local_purge_pending_clients(bat_priv);
	bat_priv->tt.local_crc = batadv_tt_local_crc(bat_priv);
=======
/**
 * batadv_tt_local_commit_changes_nolock - commit all pending local tt changes
 *  which have been queued in the time since the last commit
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Caller must hold tt->commit_lock.
 */
static void batadv_tt_local_commit_changes_nolock(struct batadv_priv *bat_priv)
{
	lockdep_assert_held(&bat_priv->tt.commit_lock);

	/* Update multicast addresses in local translation table */
	batadv_mcast_mla_update(bat_priv);

	if (atomic_read(&bat_priv->tt.local_changes) < 1) {
		if (!batadv_atomic_dec_not_zero(&bat_priv->tt.ogm_append_cnt))
			batadv_tt_tvlv_container_update(bat_priv);
		return;
	}

	batadv_tt_local_set_flags(bat_priv, BATADV_TT_CLIENT_NEW, false, true);

	batadv_tt_local_purge_pending_clients(bat_priv);
	batadv_tt_local_update_crc(bat_priv);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Increment the TTVN only once per OGM interval */
	atomic_inc(&bat_priv->tt.vn);
	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Local changes committed, updating to ttvn %u\n",
<<<<<<< HEAD
		   (uint8_t)atomic_read(&bat_priv->tt.vn));

	/* reset the sending counter */
	atomic_set(&bat_priv->tt.ogm_append_cnt, BATADV_TT_OGM_APPEND_MAX);

	return batadv_tt_changes_fill_buff(bat_priv, packet_buff,
					   packet_buff_len, packet_min_len);
}

/* when calling this function (hard_iface == primary_if) has to be true */
int batadv_tt_append_diff(struct batadv_priv *bat_priv,
			  unsigned char **packet_buff, int *packet_buff_len,
			  int packet_min_len)
{
	int tt_num_changes;

	/* if at least one change happened */
	tt_num_changes = batadv_tt_commit_changes(bat_priv, packet_buff,
						  packet_buff_len,
						  packet_min_len);

	/* if the changes have been sent often enough */
	if ((tt_num_changes < 0) &&
	    (!batadv_atomic_dec_not_zero(&bat_priv->tt.ogm_append_cnt))) {
		batadv_tt_realloc_packet_buff(packet_buff, packet_buff_len,
					      packet_min_len, packet_min_len);
		tt_num_changes = 0;
	}

	return tt_num_changes;
}

bool batadv_is_ap_isolated(struct batadv_priv *bat_priv, uint8_t *src,
			   uint8_t *dst)
{
	struct batadv_tt_local_entry *tt_local_entry = NULL;
	struct batadv_tt_global_entry *tt_global_entry = NULL;
	bool ret = false;

	if (!atomic_read(&bat_priv->ap_isolation))
		goto out;

	tt_local_entry = batadv_tt_local_hash_find(bat_priv, dst);
	if (!tt_local_entry)
		goto out;

	tt_global_entry = batadv_tt_global_hash_find(bat_priv, src);
=======
		   (u8)atomic_read(&bat_priv->tt.vn));

	/* reset the sending counter */
	atomic_set(&bat_priv->tt.ogm_append_cnt, BATADV_TT_OGM_APPEND_MAX);
	batadv_tt_tvlv_container_update(bat_priv);
}

/**
 * batadv_tt_local_commit_changes - commit all pending local tt changes which
 *  have been queued in the time since the last commit
 * @bat_priv: the bat priv with all the soft interface information
 */
void batadv_tt_local_commit_changes(struct batadv_priv *bat_priv)
{
	spin_lock_bh(&bat_priv->tt.commit_lock);
	batadv_tt_local_commit_changes_nolock(bat_priv);
	spin_unlock_bh(&bat_priv->tt.commit_lock);
}

bool batadv_is_ap_isolated(struct batadv_priv *bat_priv, u8 *src, u8 *dst,
			   unsigned short vid)
{
	struct batadv_tt_local_entry *tt_local_entry = NULL;
	struct batadv_tt_global_entry *tt_global_entry = NULL;
	struct batadv_softif_vlan *vlan;
	bool ret = false;

	vlan = batadv_softif_vlan_get(bat_priv, vid);
	if (!vlan)
		return false;

	if (!atomic_read(&vlan->ap_isolation))
		goto out;

	tt_local_entry = batadv_tt_local_hash_find(bat_priv, dst, vid);
	if (!tt_local_entry)
		goto out;

	tt_global_entry = batadv_tt_global_hash_find(bat_priv, src, vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!tt_global_entry)
		goto out;

	if (!_batadv_is_ap_isolated(tt_local_entry, tt_global_entry))
		goto out;

	ret = true;

out:
<<<<<<< HEAD
	if (tt_global_entry)
		batadv_tt_global_entry_free_ref(tt_global_entry);
	if (tt_local_entry)
		batadv_tt_local_entry_free_ref(tt_local_entry);
	return ret;
}

void batadv_tt_update_orig(struct batadv_priv *bat_priv,
			   struct batadv_orig_node *orig_node,
			   const unsigned char *tt_buff, uint8_t tt_num_changes,
			   uint8_t ttvn, uint16_t tt_crc)
{
	uint8_t orig_ttvn = (uint8_t)atomic_read(&orig_node->last_ttvn);
	bool full_table = true;
	struct batadv_tt_change *tt_change;

	/* don't care about a backbone gateways updates. */
	if (batadv_bla_is_backbone_gw_orig(bat_priv, orig_node->orig))
		return;
=======
	batadv_softif_vlan_put(vlan);
	if (tt_global_entry)
		batadv_tt_global_entry_put(tt_global_entry);
	if (tt_local_entry)
		batadv_tt_local_entry_put(tt_local_entry);
	return ret;
}

/**
 * batadv_tt_update_orig - update global translation table with new tt
 *  information received via ogms
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the orig_node of the ogm
 * @tt_buff: pointer to the first tvlv VLAN entry
 * @tt_num_vlan: number of tvlv VLAN entries
 * @tt_change: pointer to the first entry in the TT buffer
 * @tt_num_changes: number of tt changes inside the tt buffer
 * @ttvn: translation table version number of this changeset
 */
static void batadv_tt_update_orig(struct batadv_priv *bat_priv,
				  struct batadv_orig_node *orig_node,
				  const void *tt_buff, u16 tt_num_vlan,
				  struct batadv_tvlv_tt_change *tt_change,
				  u16 tt_num_changes, u8 ttvn)
{
	u8 orig_ttvn = (u8)atomic_read(&orig_node->last_ttvn);
	struct batadv_tvlv_tt_vlan_data *tt_vlan;
	bool full_table = true;
	bool has_tt_init;

	tt_vlan = (struct batadv_tvlv_tt_vlan_data *)tt_buff;
	has_tt_init = test_bit(BATADV_ORIG_CAPA_HAS_TT,
			       &orig_node->capa_initialized);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* orig table not initialised AND first diff is in the OGM OR the ttvn
	 * increased by one -> we can apply the attached changes
	 */
<<<<<<< HEAD
	if ((!orig_node->tt_initialised && ttvn == 1) ||
	    ttvn - orig_ttvn == 1) {
=======
	if ((!has_tt_init && ttvn == 1) || ttvn - orig_ttvn == 1) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* the OGM could not contain the changes due to their size or
		 * because they have already been sent BATADV_TT_OGM_APPEND_MAX
		 * times.
		 * In this case send a tt request
		 */
		if (!tt_num_changes) {
			full_table = false;
			goto request_table;
		}

<<<<<<< HEAD
		tt_change = (struct batadv_tt_change *)tt_buff;
=======
		spin_lock_bh(&orig_node->tt_lock);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		batadv_tt_update_changes(bat_priv, orig_node, tt_num_changes,
					 ttvn, tt_change);

		/* Even if we received the precomputed crc with the OGM, we
		 * prefer to recompute it to spot any possible inconsistency
		 * in the global table
		 */
<<<<<<< HEAD
		orig_node->tt_crc = batadv_tt_global_crc(bat_priv, orig_node);
=======
		batadv_tt_global_update_crc(bat_priv, orig_node);

		spin_unlock_bh(&orig_node->tt_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* The ttvn alone is not enough to guarantee consistency
		 * because a single value could represent different states
		 * (due to the wrap around). Thus a node has to check whether
		 * the resulting table (after applying the changes) is still
		 * consistent or not. E.g. a node could disconnect while its
		 * ttvn is X and reconnect on ttvn = X + TTVN_MAX: in this case
		 * checking the CRC value is mandatory to detect the
		 * inconsistency
		 */
<<<<<<< HEAD
		if (orig_node->tt_crc != tt_crc)
=======
		if (!batadv_tt_global_check_crc(orig_node, tt_vlan,
						tt_num_vlan))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto request_table;
	} else {
		/* if we missed more than one change or our tables are not
		 * in sync anymore -> request fresh tt data
		 */
<<<<<<< HEAD
		if (!orig_node->tt_initialised || ttvn != orig_ttvn ||
		    orig_node->tt_crc != tt_crc) {
request_table:
			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "TT inconsistency for %pM. Need to retrieve the correct information (ttvn: %u last_ttvn: %u crc: %#.4x last_crc: %#.4x num_changes: %u)\n",
				   orig_node->orig, ttvn, orig_ttvn, tt_crc,
				   orig_node->tt_crc, tt_num_changes);
			batadv_send_tt_request(bat_priv, orig_node, ttvn,
					       tt_crc, full_table);
=======
		if (!has_tt_init || ttvn != orig_ttvn ||
		    !batadv_tt_global_check_crc(orig_node, tt_vlan,
						tt_num_vlan)) {
request_table:
			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "TT inconsistency for %pM. Need to retrieve the correct information (ttvn: %u last_ttvn: %u num_changes: %u)\n",
				   orig_node->orig, ttvn, orig_ttvn,
				   tt_num_changes);
			batadv_send_tt_request(bat_priv, orig_node, ttvn,
					       tt_vlan, tt_num_vlan,
					       full_table);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return;
		}
	}
}

<<<<<<< HEAD
/* returns true whether we know that the client has moved from its old
 * originator to another one. This entry is kept is still kept for consistency
 * purposes
 */
bool batadv_tt_global_client_is_roaming(struct batadv_priv *bat_priv,
					uint8_t *addr)
=======
/**
 * batadv_tt_global_client_is_roaming - check if a client is marked as roaming
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client to check
 * @vid: VLAN identifier
 *
 * Return: true if we know that the client has moved from its old originator
 * to another one. This entry is still kept for consistency purposes and will be
 * deleted later by a DEL or because of timeout
 */
bool batadv_tt_global_client_is_roaming(struct batadv_priv *bat_priv,
					u8 *addr, unsigned short vid)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct batadv_tt_global_entry *tt_global_entry;
	bool ret = false;

<<<<<<< HEAD
	tt_global_entry = batadv_tt_global_hash_find(bat_priv, addr);
=======
	tt_global_entry = batadv_tt_global_hash_find(bat_priv, addr, vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!tt_global_entry)
		goto out;

	ret = tt_global_entry->common.flags & BATADV_TT_CLIENT_ROAM;
<<<<<<< HEAD
	batadv_tt_global_entry_free_ref(tt_global_entry);
=======
	batadv_tt_global_entry_put(tt_global_entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out:
	return ret;
}

/**
 * batadv_tt_local_client_is_roaming - tells whether the client is roaming
 * @bat_priv: the bat priv with all the soft interface information
<<<<<<< HEAD
 * @addr: the MAC address of the local client to query
 *
 * Returns true if the local client is known to be roaming (it is not served by
=======
 * @addr: the mac address of the local client to query
 * @vid: VLAN identifier
 *
 * Return: true if the local client is known to be roaming (it is not served by
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * this node anymore) or not. If yes, the client is still present in the table
 * to keep the latter consistent with the node TTVN
 */
bool batadv_tt_local_client_is_roaming(struct batadv_priv *bat_priv,
<<<<<<< HEAD
				       uint8_t *addr)
=======
				       u8 *addr, unsigned short vid)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct batadv_tt_local_entry *tt_local_entry;
	bool ret = false;

<<<<<<< HEAD
	tt_local_entry = batadv_tt_local_hash_find(bat_priv, addr);
=======
	tt_local_entry = batadv_tt_local_hash_find(bat_priv, addr, vid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!tt_local_entry)
		goto out;

	ret = tt_local_entry->common.flags & BATADV_TT_CLIENT_ROAM;
<<<<<<< HEAD
	batadv_tt_local_entry_free_ref(tt_local_entry);
=======
	batadv_tt_local_entry_put(tt_local_entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out:
	return ret;
}

bool batadv_tt_add_temporary_global_entry(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig_node,
<<<<<<< HEAD
					  const unsigned char *addr)
{
	bool ret = false;

	/* if the originator is a backbone node (meaning it belongs to the same
	 * LAN of this node) the temporary client must not be added because to
	 * reach such destination the node must use the LAN instead of the mesh
	 */
	if (batadv_bla_is_backbone_gw_orig(bat_priv, orig_node->orig))
		goto out;

	if (!batadv_tt_global_add(bat_priv, orig_node, addr,
=======
					  const unsigned char *addr,
					  unsigned short vid)
{
	bool ret = false;

	if (!batadv_tt_global_add(bat_priv, orig_node, addr, vid,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				  BATADV_TT_CLIENT_TEMP,
				  atomic_read(&orig_node->last_ttvn)))
		goto out;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
<<<<<<< HEAD
		   "Added temporary global client (addr: %pM orig: %pM)\n",
		   addr, orig_node->orig);
=======
		   "Added temporary global client (addr: %pM, vid: %d, orig: %pM)\n",
		   addr, BATADV_PRINT_VID(vid), orig_node->orig);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ret = true;
out:
	return ret;
}
<<<<<<< HEAD
=======

/**
 * batadv_tt_local_resize_to_mtu - resize the local translation table fit the
 *  maximum packet size that can be transported through the mesh
 * @soft_iface: netdev struct of the mesh interface
 *
 * Remove entries older than 'timeout' and half timeout if more entries need
 * to be removed.
 */
void batadv_tt_local_resize_to_mtu(struct net_device *soft_iface)
{
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	int packet_size_max = atomic_read(&bat_priv->packet_size_max);
	int table_size, timeout = BATADV_TT_LOCAL_TIMEOUT / 2;
	bool reduced = false;

	spin_lock_bh(&bat_priv->tt.commit_lock);

	while (true) {
		table_size = batadv_tt_local_table_transmit_size(bat_priv);
		if (packet_size_max >= table_size)
			break;

		batadv_tt_local_purge(bat_priv, timeout);
		batadv_tt_local_purge_pending_clients(bat_priv);

		timeout /= 2;
		reduced = true;
		net_ratelimited_function(batadv_info, soft_iface,
					 "Forced to purge local tt entries to fit new maximum fragment MTU (%i)\n",
					 packet_size_max);
	}

	/* commit these changes immediately, to avoid synchronization problem
	 * with the TTVN
	 */
	if (reduced)
		batadv_tt_local_commit_changes_nolock(bat_priv);

	spin_unlock_bh(&bat_priv->tt.commit_lock);
}

/**
 * batadv_tt_tvlv_ogm_handler_v1 - process incoming tt tvlv container
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node of the ogm
 * @flags: flags indicating the tvlv state (see batadv_tvlv_handler_flags)
 * @tvlv_value: tvlv buffer containing the gateway data
 * @tvlv_value_len: tvlv buffer length
 */
static void batadv_tt_tvlv_ogm_handler_v1(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig,
					  u8 flags, void *tvlv_value,
					  u16 tvlv_value_len)
{
	struct batadv_tvlv_tt_vlan_data *tt_vlan;
	struct batadv_tvlv_tt_change *tt_change;
	struct batadv_tvlv_tt_data *tt_data;
	u16 num_entries, num_vlan;

	if (tvlv_value_len < sizeof(*tt_data))
		return;

	tt_data = (struct batadv_tvlv_tt_data *)tvlv_value;
	tvlv_value_len -= sizeof(*tt_data);

	num_vlan = ntohs(tt_data->num_vlan);

	if (tvlv_value_len < sizeof(*tt_vlan) * num_vlan)
		return;

	tt_vlan = (struct batadv_tvlv_tt_vlan_data *)(tt_data + 1);
	tt_change = (struct batadv_tvlv_tt_change *)(tt_vlan + num_vlan);
	tvlv_value_len -= sizeof(*tt_vlan) * num_vlan;

	num_entries = batadv_tt_entries(tvlv_value_len);

	batadv_tt_update_orig(bat_priv, orig, tt_vlan, num_vlan, tt_change,
			      num_entries, tt_data->ttvn);
}

/**
 * batadv_tt_tvlv_unicast_handler_v1 - process incoming (unicast) tt tvlv
 *  container
 * @bat_priv: the bat priv with all the soft interface information
 * @src: mac address of tt tvlv sender
 * @dst: mac address of tt tvlv recipient
 * @tvlv_value: tvlv buffer containing the tt data
 * @tvlv_value_len: tvlv buffer length
 *
 * Return: NET_RX_DROP if the tt tvlv is to be re-routed, NET_RX_SUCCESS
 * otherwise.
 */
static int batadv_tt_tvlv_unicast_handler_v1(struct batadv_priv *bat_priv,
					     u8 *src, u8 *dst,
					     void *tvlv_value,
					     u16 tvlv_value_len)
{
	struct batadv_tvlv_tt_data *tt_data;
	u16 tt_vlan_len, tt_num_entries;
	char tt_flag;
	bool ret;

	if (tvlv_value_len < sizeof(*tt_data))
		return NET_RX_SUCCESS;

	tt_data = (struct batadv_tvlv_tt_data *)tvlv_value;
	tvlv_value_len -= sizeof(*tt_data);

	tt_vlan_len = sizeof(struct batadv_tvlv_tt_vlan_data);
	tt_vlan_len *= ntohs(tt_data->num_vlan);

	if (tvlv_value_len < tt_vlan_len)
		return NET_RX_SUCCESS;

	tvlv_value_len -= tt_vlan_len;
	tt_num_entries = batadv_tt_entries(tvlv_value_len);

	switch (tt_data->flags & BATADV_TT_DATA_TYPE_MASK) {
	case BATADV_TT_REQUEST:
		batadv_inc_counter(bat_priv, BATADV_CNT_TT_REQUEST_RX);

		/* If this node cannot provide a TT response the tt_request is
		 * forwarded
		 */
		ret = batadv_send_tt_response(bat_priv, tt_data, src, dst);
		if (!ret) {
			if (tt_data->flags & BATADV_TT_FULL_TABLE)
				tt_flag = 'F';
			else
				tt_flag = '.';

			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "Routing TT_REQUEST to %pM [%c]\n",
				   dst, tt_flag);
			/* tvlv API will re-route the packet */
			return NET_RX_DROP;
		}
		break;
	case BATADV_TT_RESPONSE:
		batadv_inc_counter(bat_priv, BATADV_CNT_TT_RESPONSE_RX);

		if (batadv_is_my_mac(bat_priv, dst)) {
			batadv_handle_tt_response(bat_priv, tt_data,
						  src, tt_num_entries);
			return NET_RX_SUCCESS;
		}

		if (tt_data->flags & BATADV_TT_FULL_TABLE)
			tt_flag =  'F';
		else
			tt_flag = '.';

		batadv_dbg(BATADV_DBG_TT, bat_priv,
			   "Routing TT_RESPONSE to %pM [%c]\n", dst, tt_flag);

		/* tvlv API will re-route the packet */
		return NET_RX_DROP;
	}

	return NET_RX_SUCCESS;
}

/**
 * batadv_roam_tvlv_unicast_handler_v1 - process incoming tt roam tvlv container
 * @bat_priv: the bat priv with all the soft interface information
 * @src: mac address of tt tvlv sender
 * @dst: mac address of tt tvlv recipient
 * @tvlv_value: tvlv buffer containing the tt data
 * @tvlv_value_len: tvlv buffer length
 *
 * Return: NET_RX_DROP if the tt roam tvlv is to be re-routed, NET_RX_SUCCESS
 * otherwise.
 */
static int batadv_roam_tvlv_unicast_handler_v1(struct batadv_priv *bat_priv,
					       u8 *src, u8 *dst,
					       void *tvlv_value,
					       u16 tvlv_value_len)
{
	struct batadv_tvlv_roam_adv *roaming_adv;
	struct batadv_orig_node *orig_node = NULL;

	/* If this node is not the intended recipient of the
	 * roaming advertisement the packet is forwarded
	 * (the tvlv API will re-route the packet).
	 */
	if (!batadv_is_my_mac(bat_priv, dst))
		return NET_RX_DROP;

	if (tvlv_value_len < sizeof(*roaming_adv))
		goto out;

	orig_node = batadv_orig_hash_find(bat_priv, src);
	if (!orig_node)
		goto out;

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_ROAM_ADV_RX);
	roaming_adv = (struct batadv_tvlv_roam_adv *)tvlv_value;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received ROAMING_ADV from %pM (client %pM)\n",
		   src, roaming_adv->client);

	batadv_tt_global_add(bat_priv, orig_node, roaming_adv->client,
			     ntohs(roaming_adv->vid), BATADV_TT_CLIENT_ROAM,
			     atomic_read(&orig_node->last_ttvn) + 1);

out:
	if (orig_node)
		batadv_orig_node_put(orig_node);
	return NET_RX_SUCCESS;
}

/**
 * batadv_tt_init - initialise the translation table internals
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return: 0 on success or negative error number in case of failure.
 */
int batadv_tt_init(struct batadv_priv *bat_priv)
{
	int ret;

	/* synchronized flags must be remote */
	BUILD_BUG_ON(!(BATADV_TT_SYNC_MASK & BATADV_TT_REMOTE_MASK));

	ret = batadv_tt_local_init(bat_priv);
	if (ret < 0)
		return ret;

	ret = batadv_tt_global_init(bat_priv);
	if (ret < 0)
		return ret;

	batadv_tvlv_handler_register(bat_priv, batadv_tt_tvlv_ogm_handler_v1,
				     batadv_tt_tvlv_unicast_handler_v1,
				     BATADV_TVLV_TT, 1, BATADV_NO_FLAGS);

	batadv_tvlv_handler_register(bat_priv, NULL,
				     batadv_roam_tvlv_unicast_handler_v1,
				     BATADV_TVLV_ROAM, 1, BATADV_NO_FLAGS);

	INIT_DELAYED_WORK(&bat_priv->tt.work, batadv_tt_purge);
	queue_delayed_work(batadv_event_workqueue, &bat_priv->tt.work,
			   msecs_to_jiffies(BATADV_TT_WORK_PERIOD));

	return 1;
}

/**
 * batadv_tt_global_is_isolated - check if a client is marked as isolated
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client
 * @vid: the identifier of the VLAN where this client is connected
 *
 * Return: true if the client is marked with the TT_CLIENT_ISOLA flag, false
 * otherwise
 */
bool batadv_tt_global_is_isolated(struct batadv_priv *bat_priv,
				  const u8 *addr, unsigned short vid)
{
	struct batadv_tt_global_entry *tt;
	bool ret;

	tt = batadv_tt_global_hash_find(bat_priv, addr, vid);
	if (!tt)
		return false;

	ret = tt->common.flags & BATADV_TT_CLIENT_ISOLA;

	batadv_tt_global_entry_put(tt);

	return ret;
}

/**
 * batadv_tt_cache_init - Initialize tt memory object cache
 *
 * Return: 0 on success or negative error number in case of failure.
 */
int __init batadv_tt_cache_init(void)
{
	size_t tl_size = sizeof(struct batadv_tt_local_entry);
	size_t tg_size = sizeof(struct batadv_tt_global_entry);
	size_t tt_orig_size = sizeof(struct batadv_tt_orig_list_entry);
	size_t tt_change_size = sizeof(struct batadv_tt_change_node);
	size_t tt_req_size = sizeof(struct batadv_tt_req_node);
	size_t tt_roam_size = sizeof(struct batadv_tt_roam_node);

	batadv_tl_cache = kmem_cache_create("batadv_tl_cache", tl_size, 0,
					    SLAB_HWCACHE_ALIGN, NULL);
	if (!batadv_tl_cache)
		return -ENOMEM;

	batadv_tg_cache = kmem_cache_create("batadv_tg_cache", tg_size, 0,
					    SLAB_HWCACHE_ALIGN, NULL);
	if (!batadv_tg_cache)
		goto err_tt_tl_destroy;

	batadv_tt_orig_cache = kmem_cache_create("batadv_tt_orig_cache",
						 tt_orig_size, 0,
						 SLAB_HWCACHE_ALIGN, NULL);
	if (!batadv_tt_orig_cache)
		goto err_tt_tg_destroy;

	batadv_tt_change_cache = kmem_cache_create("batadv_tt_change_cache",
						   tt_change_size, 0,
						   SLAB_HWCACHE_ALIGN, NULL);
	if (!batadv_tt_change_cache)
		goto err_tt_orig_destroy;

	batadv_tt_req_cache = kmem_cache_create("batadv_tt_req_cache",
						tt_req_size, 0,
						SLAB_HWCACHE_ALIGN, NULL);
	if (!batadv_tt_req_cache)
		goto err_tt_change_destroy;

	batadv_tt_roam_cache = kmem_cache_create("batadv_tt_roam_cache",
						 tt_roam_size, 0,
						 SLAB_HWCACHE_ALIGN, NULL);
	if (!batadv_tt_roam_cache)
		goto err_tt_req_destroy;

	return 0;

err_tt_req_destroy:
	kmem_cache_destroy(batadv_tt_req_cache);
	batadv_tt_req_cache = NULL;
err_tt_change_destroy:
	kmem_cache_destroy(batadv_tt_change_cache);
	batadv_tt_change_cache = NULL;
err_tt_orig_destroy:
	kmem_cache_destroy(batadv_tt_orig_cache);
	batadv_tt_orig_cache = NULL;
err_tt_tg_destroy:
	kmem_cache_destroy(batadv_tg_cache);
	batadv_tg_cache = NULL;
err_tt_tl_destroy:
	kmem_cache_destroy(batadv_tl_cache);
	batadv_tl_cache = NULL;

	return -ENOMEM;
}

/**
 * batadv_tt_cache_destroy - Destroy tt memory object cache
 */
void batadv_tt_cache_destroy(void)
{
	kmem_cache_destroy(batadv_tl_cache);
	kmem_cache_destroy(batadv_tg_cache);
	kmem_cache_destroy(batadv_tt_orig_cache);
	kmem_cache_destroy(batadv_tt_change_cache);
	kmem_cache_destroy(batadv_tt_req_cache);
	kmem_cache_destroy(batadv_tt_roam_cache);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
