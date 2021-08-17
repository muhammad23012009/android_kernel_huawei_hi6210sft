/* Copyright (C) 2008-2013 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* Kernel module implementing an IP set type: the list:set type */

#include <linux/module.h>
#include <linux/ip.h>
<<<<<<< HEAD
=======
#include <linux/rculist.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/skbuff.h>
#include <linux/errno.h>

#include <linux/netfilter/ipset/ip_set.h>
#include <linux/netfilter/ipset/ip_set_list.h>

<<<<<<< HEAD
#define REVISION_MIN	0
#define REVISION_MAX	1 /* Counters support added */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
IP_SET_MODULE_DESC("list:set", REVISION_MIN, REVISION_MAX);
=======
#define IPSET_TYPE_REV_MIN	0
/*				1    Counters support added */
/*				2    Comments support added */
#define IPSET_TYPE_REV_MAX	3 /* skbinfo support added */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
IP_SET_MODULE_DESC("list:set", IPSET_TYPE_REV_MIN, IPSET_TYPE_REV_MAX);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_ALIAS("ip_set_list:set");

/* Member elements  */
struct set_elem {
<<<<<<< HEAD
	ip_set_id_t id;
};

struct sett_elem {
	struct {
		ip_set_id_t id;
	} __attribute__ ((aligned));
	unsigned long timeout;
};

struct setc_elem {
	struct {
		ip_set_id_t id;
	} __attribute__ ((aligned));
	struct ip_set_counter counter;
};

struct setct_elem {
	struct {
		ip_set_id_t id;
	} __attribute__ ((aligned));
	struct ip_set_counter counter;
	unsigned long timeout;
};
=======
	struct rcu_head rcu;
	struct list_head list;
	struct ip_set *set;	/* Sigh, in order to cleanup reference */
	ip_set_id_t id;
} __aligned(__alignof__(u64));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct set_adt_elem {
	ip_set_id_t id;
	ip_set_id_t refid;
	int before;
};

/* Type structure */
struct list_set {
<<<<<<< HEAD
	size_t dsize;		/* element size */
	size_t offset[IPSET_OFFSET_MAX]; /* Offsets to extensions */
	u32 size;		/* size of set list array */
	u32 timeout;		/* timeout value */
	struct timer_list gc;	/* garbage collection */
	struct set_elem members[0]; /* the set members */
};

static inline struct set_elem *
list_set_elem(const struct list_set *map, u32 id)
{
	return (struct set_elem *)((void *)map->members + id * map->dsize);
}

#define ext_timeout(e, m)	\
(unsigned long *)((void *)(e) + (m)->offset[IPSET_OFFSET_TIMEOUT])
#define ext_counter(e, m)	\
(struct ip_set_counter *)((void *)(e) + (m)->offset[IPSET_OFFSET_COUNTER])

=======
	u32 size;		/* size of set list array */
	struct timer_list gc;	/* garbage collection */
	struct net *net;	/* namespace */
	struct list_head members; /* the set members */
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int
list_set_ktest(struct ip_set *set, const struct sk_buff *skb,
	       const struct xt_action_param *par,
	       struct ip_set_adt_opt *opt, const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct set_elem *e;
<<<<<<< HEAD
	u32 i, cmdflags = opt->cmdflags;
=======
	u32 cmdflags = opt->cmdflags;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	/* Don't lookup sub-counters at all */
	opt->cmdflags &= ~IPSET_FLAG_MATCH_COUNTERS;
	if (opt->cmdflags & IPSET_FLAG_SKIP_SUBCOUNTER_UPDATE)
<<<<<<< HEAD
		opt->cmdflags &= ~IPSET_FLAG_SKIP_COUNTER_UPDATE;
	for (i = 0; i < map->size; i++) {
		e = list_set_elem(map, i);
		if (e->id == IPSET_INVALID_ID)
			return 0;
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, map)))
=======
		opt->cmdflags |= IPSET_FLAG_SKIP_COUNTER_UPDATE;
	list_for_each_entry_rcu(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;
		ret = ip_set_test(e->id, skb, par, opt);
		if (ret > 0) {
			if (SET_WITH_COUNTER(set))
<<<<<<< HEAD
				ip_set_update_counter(ext_counter(e, map),
						      ext, &opt->ext,
						      cmdflags);
=======
				ip_set_update_counter(ext_counter(e, set),
						      ext, &opt->ext,
						      cmdflags);
			if (SET_WITH_SKBINFO(set))
				ip_set_get_skbinfo(ext_skbinfo(e, set),
						   ext, &opt->ext,
						   cmdflags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return ret;
		}
	}
	return 0;
}

static int
list_set_kadd(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
	      struct ip_set_adt_opt *opt, const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct set_elem *e;
<<<<<<< HEAD
	u32 i;
	int ret;

	for (i = 0; i < map->size; i++) {
		e = list_set_elem(map, i);
		if (e->id == IPSET_INVALID_ID)
			return 0;
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, map)))
=======
	int ret;

	list_for_each_entry(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;
		ret = ip_set_add(e->id, skb, par, opt);
		if (ret == 0)
			return ret;
	}
	return 0;
}

static int
list_set_kdel(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
	      struct ip_set_adt_opt *opt, const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct set_elem *e;
<<<<<<< HEAD
	u32 i;
	int ret;

	for (i = 0; i < map->size; i++) {
		e = list_set_elem(map, i);
		if (e->id == IPSET_INVALID_ID)
			return 0;
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, map)))
=======
	int ret;

	list_for_each_entry(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;
		ret = ip_set_del(e->id, skb, par, opt);
		if (ret == 0)
			return ret;
	}
	return 0;
}

static int
list_set_kadt(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
	      enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
<<<<<<< HEAD
	struct list_set *map = set->data;
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, map);

	switch (adt) {
	case IPSET_TEST:
		return list_set_ktest(set, skb, par, opt, &ext);
	case IPSET_ADD:
		return list_set_kadd(set, skb, par, opt, &ext);
	case IPSET_DEL:
		return list_set_kdel(set, skb, par, opt, &ext);
	default:
		break;
	}
	return -EINVAL;
}

static bool
id_eq(const struct ip_set *set, u32 i, ip_set_id_t id)
{
	const struct list_set *map = set->data;
	const struct set_elem *e;

	if (i >= map->size)
		return 0;

	e = list_set_elem(map, i);
	return !!(e->id == id &&
		 !(SET_WITH_TIMEOUT(set) &&
		   ip_set_timeout_expired(ext_timeout(e, map))));
}

static int
list_set_add(struct ip_set *set, u32 i, struct set_adt_elem *d,
	     const struct ip_set_ext *ext)
{
	struct list_set *map = set->data;
	struct set_elem *e = list_set_elem(map, i);

	if (e->id != IPSET_INVALID_ID) {
		if (i == map->size - 1)
			/* Last element replaced: e.g. add new,before,last */
			ip_set_put_byindex(e->id);
		else {
			struct set_elem *x = list_set_elem(map, map->size - 1);

			/* Last element pushed off */
			if (x->id != IPSET_INVALID_ID)
				ip_set_put_byindex(x->id);
			memmove(list_set_elem(map, i + 1), e,
				map->dsize * (map->size - (i + 1)));
		}
	}

	e->id = d->id;
	if (SET_WITH_TIMEOUT(set))
		ip_set_timeout_set(ext_timeout(e, map), ext->timeout);
	if (SET_WITH_COUNTER(set))
		ip_set_init_counter(ext_counter(e, map), ext);
	return 0;
}

static int
list_set_del(struct ip_set *set, u32 i)
{
	struct list_set *map = set->data;
	struct set_elem *e = list_set_elem(map, i);

	ip_set_put_byindex(e->id);

	if (i < map->size - 1)
		memmove(e, list_set_elem(map, i + 1),
			map->dsize * (map->size - (i + 1)));

	/* Last element */
	e = list_set_elem(map, map->size - 1);
	e->id = IPSET_INVALID_ID;
	return 0;
=======
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);
	int ret = -EINVAL;

	rcu_read_lock();
	switch (adt) {
	case IPSET_TEST:
		ret = list_set_ktest(set, skb, par, opt, &ext);
		break;
	case IPSET_ADD:
		ret = list_set_kadd(set, skb, par, opt, &ext);
		break;
	case IPSET_DEL:
		ret = list_set_kdel(set, skb, par, opt, &ext);
		break;
	default:
		break;
	}
	rcu_read_unlock();

	return ret;
}

/* Userspace interfaces: we are protected by the nfnl mutex */

static void
__list_set_del_rcu(struct rcu_head * rcu)
{
	struct set_elem *e = container_of(rcu, struct set_elem, rcu);
	struct ip_set *set = e->set;
	struct list_set *map = set->data;

	ip_set_put_byindex(map->net, e->id);
	ip_set_ext_destroy(set, e);
	kfree(e);
}

static inline void
list_set_del(struct ip_set *set, struct set_elem *e)
{
	list_del_rcu(&e->list);
	call_rcu(&e->rcu, __list_set_del_rcu);
}

static inline void
list_set_replace(struct set_elem *e, struct set_elem *old)
{
	list_replace_rcu(&old->list, &e->list);
	call_rcu(&old->rcu, __list_set_del_rcu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void
set_cleanup_entries(struct ip_set *set)
{
	struct list_set *map = set->data;
<<<<<<< HEAD
	struct set_elem *e;
	u32 i;

	for (i = 0; i < map->size; i++) {
		e = list_set_elem(map, i);
		if (e->id != IPSET_INVALID_ID &&
		    ip_set_timeout_expired(ext_timeout(e, map)))
			list_set_del(set, i);
	}
=======
	struct set_elem *e, *n;

	list_for_each_entry_safe(e, n, &map->members, list)
		if (ip_set_timeout_expired(ext_timeout(e, set)))
			list_set_del(set, e);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int
list_set_utest(struct ip_set *set, void *value, const struct ip_set_ext *ext,
	       struct ip_set_ext *mext, u32 flags)
{
	struct list_set *map = set->data;
	struct set_adt_elem *d = value;
<<<<<<< HEAD
	struct set_elem *e;
	u32 i;
	int ret;

	for (i = 0; i < map->size; i++) {
		e = list_set_elem(map, i);
		if (e->id == IPSET_INVALID_ID)
			return 0;
		else if (SET_WITH_TIMEOUT(set) &&
			 ip_set_timeout_expired(ext_timeout(e, map)))
			continue;
		else if (e->id != d->id)
			continue;

		if (d->before == 0)
			return 1;
		else if (d->before > 0)
			ret = id_eq(set, i + 1, d->refid);
		else
			ret = i > 0 && id_eq(set, i - 1, d->refid);
=======
	struct set_elem *e, *next, *prev = NULL;
	int ret;

	list_for_each_entry(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		else if (e->id != d->id) {
			prev = e;
			continue;
		}

		if (d->before == 0) {
			ret = 1;
		} else if (d->before > 0) {
			next = list_next_entry(e, list);
			ret = !list_is_last(&e->list, &map->members) &&
			      next->id == d->refid;
		} else {
			ret = prev && prev->id == d->refid;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return ret;
	}
	return 0;
}

<<<<<<< HEAD
=======
static void
list_set_init_extensions(struct ip_set *set, const struct ip_set_ext *ext,
			 struct set_elem *e)
{
	if (SET_WITH_COUNTER(set))
		ip_set_init_counter(ext_counter(e, set), ext);
	if (SET_WITH_COMMENT(set))
		ip_set_init_comment(ext_comment(e, set), ext);
	if (SET_WITH_SKBINFO(set))
		ip_set_init_skbinfo(ext_skbinfo(e, set), ext);
	/* Update timeout last */
	if (SET_WITH_TIMEOUT(set))
		ip_set_timeout_set(ext_timeout(e, set), ext->timeout);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int
list_set_uadd(struct ip_set *set, void *value, const struct ip_set_ext *ext,
	      struct ip_set_ext *mext, u32 flags)
{
	struct list_set *map = set->data;
	struct set_adt_elem *d = value;
<<<<<<< HEAD
	struct set_elem *e;
	bool flag_exist = flags & IPSET_FLAG_EXIST;
	u32 i, ret = 0;

	/* Check already added element */
	for (i = 0; i < map->size; i++) {
		e = list_set_elem(map, i);
		if (e->id == IPSET_INVALID_ID)
			goto insert;
		else if (SET_WITH_TIMEOUT(set) &&
			 ip_set_timeout_expired(ext_timeout(e, map)))
			continue;
		else if (e->id != d->id)
			continue;

		if ((d->before > 1 && !id_eq(set, i + 1, d->refid)) ||
		    (d->before < 0 &&
		     (i == 0 || !id_eq(set, i - 1, d->refid))))
			/* Before/after doesn't match */
			return -IPSET_ERR_REF_EXIST;
		if (!flag_exist)
			/* Can't re-add */
			return -IPSET_ERR_EXIST;
		/* Update extensions */
		if (SET_WITH_TIMEOUT(set))
			ip_set_timeout_set(ext_timeout(e, map), ext->timeout);
		if (SET_WITH_COUNTER(set))
			ip_set_init_counter(ext_counter(e, map), ext);
		/* Set is already added to the list */
		ip_set_put_byindex(d->id);
		return 0;
	}
insert:
	ret = -IPSET_ERR_LIST_FULL;
	for (i = 0; i < map->size && ret == -IPSET_ERR_LIST_FULL; i++) {
		e = list_set_elem(map, i);
		if (e->id == IPSET_INVALID_ID)
			ret = d->before != 0 ? -IPSET_ERR_REF_EXIST
				: list_set_add(set, i, d, ext);
		else if (e->id != d->refid)
			continue;
		else if (d->before > 0)
			ret = list_set_add(set, i, d, ext);
		else if (i + 1 < map->size)
			ret = list_set_add(set, i + 1, d, ext);
	}

	return ret;
=======
	struct set_elem *e, *n, *prev, *next;
	bool flag_exist = flags & IPSET_FLAG_EXIST;

	/* Find where to add the new entry */
	n = prev = next = NULL;
	list_for_each_entry(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		else if (d->id == e->id)
			n = e;
		else if (d->before == 0 || e->id != d->refid)
			continue;
		else if (d->before > 0)
			next = e;
		else
			prev = e;
	}
	/* Re-add already existing element */
	if (n) {
		if ((d->before > 0 && !next) ||
		    (d->before < 0 && !prev))
			return -IPSET_ERR_REF_EXIST;
		if (!flag_exist)
			return -IPSET_ERR_EXIST;
		/* Update extensions */
		ip_set_ext_destroy(set, n);
		list_set_init_extensions(set, ext, n);

		/* Set is already added to the list */
		ip_set_put_byindex(map->net, d->id);
		return 0;
	}
	/* Add new entry */
	if (d->before == 0) {
		/* Append  */
		n = list_empty(&map->members) ? NULL :
		    list_last_entry(&map->members, struct set_elem, list);
	} else if (d->before > 0) {
		/* Insert after next element */
		if (!list_is_last(&next->list, &map->members))
			n = list_next_entry(next, list);
	} else {
		/* Insert before prev element */
		if (prev->list.prev != &map->members)
			n = list_prev_entry(prev, list);
	}
	/* Can we replace a timed out entry? */
	if (n &&
	    !(SET_WITH_TIMEOUT(set) &&
	      ip_set_timeout_expired(ext_timeout(n, set))))
		n =  NULL;

	e = kzalloc(set->dsize, GFP_ATOMIC);
	if (!e)
		return -ENOMEM;
	e->id = d->id;
	e->set = set;
	INIT_LIST_HEAD(&e->list);
	list_set_init_extensions(set, ext, e);
	if (n)
		list_set_replace(e, n);
	else if (next)
		list_add_tail_rcu(&e->list, &next->list);
	else if (prev)
		list_add_rcu(&e->list, &prev->list);
	else
		list_add_tail_rcu(&e->list, &map->members);

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int
list_set_udel(struct ip_set *set, void *value, const struct ip_set_ext *ext,
	      struct ip_set_ext *mext, u32 flags)
{
	struct list_set *map = set->data;
	struct set_adt_elem *d = value;
<<<<<<< HEAD
	struct set_elem *e;
	u32 i;

	for (i = 0; i < map->size; i++) {
		e = list_set_elem(map, i);
		if (e->id == IPSET_INVALID_ID)
			return d->before != 0 ? -IPSET_ERR_REF_EXIST
					      : -IPSET_ERR_EXIST;
		else if (SET_WITH_TIMEOUT(set) &&
			 ip_set_timeout_expired(ext_timeout(e, map)))
			continue;
		else if (e->id != d->id)
			continue;

		if (d->before == 0)
			return list_set_del(set, i);
		else if (d->before > 0) {
			if (!id_eq(set, i + 1, d->refid))
				return -IPSET_ERR_REF_EXIST;
			return list_set_del(set, i);
		} else if (i == 0 || !id_eq(set, i - 1, d->refid))
			return -IPSET_ERR_REF_EXIST;
		else
			return list_set_del(set, i);
	}
	return -IPSET_ERR_EXIST;
=======
	struct set_elem *e, *next, *prev = NULL;

	list_for_each_entry(e, &map->members, list) {
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, set)))
			continue;
		else if (e->id != d->id) {
			prev = e;
			continue;
		}

		if (d->before > 0) {
			next = list_next_entry(e, list);
			if (list_is_last(&e->list, &map->members) ||
			    next->id != d->refid)
				return -IPSET_ERR_REF_EXIST;
		} else if (d->before < 0) {
			if (!prev || prev->id != d->refid)
				return -IPSET_ERR_REF_EXIST;
		}
		list_set_del(set, e);
		return 0;
	}
	return d->before != 0 ? -IPSET_ERR_REF_EXIST : -IPSET_ERR_EXIST;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int
list_set_uadt(struct ip_set *set, struct nlattr *tb[],
	      enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	struct list_set *map = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct set_adt_elem e = { .refid = IPSET_INVALID_ID };
<<<<<<< HEAD
	struct ip_set_ext ext = IP_SET_INIT_UEXT(map);
	struct ip_set *s;
	int ret = 0;

	if (unlikely(!tb[IPSET_ATTR_NAME] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PACKETS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_BYTES)))
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

	ret = ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;
	e.id = ip_set_get_byname(nla_data(tb[IPSET_ATTR_NAME]), &s);
=======
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	struct ip_set *s;
	int ret = 0;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

	if (unlikely(!tb[IPSET_ATTR_NAME] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
		return -IPSET_ERR_PROTOCOL;

	ret = ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;
	e.id = ip_set_get_byname(map->net, nla_data(tb[IPSET_ATTR_NAME]), &s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (e.id == IPSET_INVALID_ID)
		return -IPSET_ERR_NAME;
	/* "Loop detection" */
	if (s->type->features & IPSET_TYPE_NAME) {
		ret = -IPSET_ERR_LOOP;
		goto finish;
	}

	if (tb[IPSET_ATTR_CADT_FLAGS]) {
		u32 f = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		e.before = f & IPSET_FLAG_BEFORE;
	}

	if (e.before && !tb[IPSET_ATTR_NAMEREF]) {
		ret = -IPSET_ERR_BEFORE;
		goto finish;
	}

	if (tb[IPSET_ATTR_NAMEREF]) {
<<<<<<< HEAD
		e.refid = ip_set_get_byname(nla_data(tb[IPSET_ATTR_NAMEREF]),
=======
		e.refid = ip_set_get_byname(map->net,
					    nla_data(tb[IPSET_ATTR_NAMEREF]),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					    &s);
		if (e.refid == IPSET_INVALID_ID) {
			ret = -IPSET_ERR_NAMEREF;
			goto finish;
		}
		if (!e.before)
			e.before = -1;
	}
	if (adt != IPSET_TEST && SET_WITH_TIMEOUT(set))
		set_cleanup_entries(set);

	ret = adtfn(set, &e, &ext, &ext, flags);

finish:
	if (e.refid != IPSET_INVALID_ID)
<<<<<<< HEAD
		ip_set_put_byindex(e.refid);
	if (adt != IPSET_ADD || ret)
		ip_set_put_byindex(e.id);
=======
		ip_set_put_byindex(map->net, e.refid);
	if (adt != IPSET_ADD || ret)
		ip_set_put_byindex(map->net, e.id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ip_set_eexist(ret, flags) ? 0 : ret;
}

static void
list_set_flush(struct ip_set *set)
{
	struct list_set *map = set->data;
<<<<<<< HEAD
	struct set_elem *e;
	u32 i;

	for (i = 0; i < map->size; i++) {
		e = list_set_elem(map, i);
		if (e->id != IPSET_INVALID_ID) {
			ip_set_put_byindex(e->id);
			e->id = IPSET_INVALID_ID;
		}
	}
=======
	struct set_elem *e, *n;

	list_for_each_entry_safe(e, n, &map->members, list)
		list_set_del(set, e);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void
list_set_destroy(struct ip_set *set)
{
	struct list_set *map = set->data;
<<<<<<< HEAD

	if (SET_WITH_TIMEOUT(set))
		del_timer_sync(&map->gc);
	list_set_flush(set);
=======
	struct set_elem *e, *n;

	if (SET_WITH_TIMEOUT(set))
		del_timer_sync(&map->gc);

	list_for_each_entry_safe(e, n, &map->members, list) {
		list_del(&e->list);
		ip_set_put_byindex(map->net, e->id);
		ip_set_ext_destroy(set, e);
		kfree(e);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(map);

	set->data = NULL;
}

static int
list_set_head(struct ip_set *set, struct sk_buff *skb)
{
	const struct list_set *map = set->data;
	struct nlattr *nested;
<<<<<<< HEAD
=======
	struct set_elem *e;
	u32 n = 0;

	rcu_read_lock();
	list_for_each_entry_rcu(e, &map->members, list)
		n++;
	rcu_read_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
	if (!nested)
		goto nla_put_failure;
	if (nla_put_net32(skb, IPSET_ATTR_SIZE, htonl(map->size)) ||
<<<<<<< HEAD
	    (SET_WITH_TIMEOUT(set) &&
	     nla_put_net32(skb, IPSET_ATTR_TIMEOUT, htonl(map->timeout))) ||
	    (SET_WITH_COUNTER(set) &&
	     nla_put_net32(skb, IPSET_ATTR_CADT_FLAGS,
			   htonl(IPSET_FLAG_WITH_COUNTERS))) ||
	    nla_put_net32(skb, IPSET_ATTR_REFERENCES, htonl(set->ref - 1)) ||
	    nla_put_net32(skb, IPSET_ATTR_MEMSIZE,
			  htonl(sizeof(*map) + map->size * map->dsize)))
=======
	    nla_put_net32(skb, IPSET_ATTR_REFERENCES, htonl(set->ref)) ||
	    nla_put_net32(skb, IPSET_ATTR_MEMSIZE,
			  htonl(sizeof(*map) + n * set->dsize)))
		goto nla_put_failure;
	if (unlikely(ip_set_put_flags(skb, set)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto nla_put_failure;
	ipset_nest_end(skb, nested);

	return 0;
nla_put_failure:
	return -EMSGSIZE;
}

static int
list_set_list(const struct ip_set *set,
	      struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct list_set *map = set->data;
	struct nlattr *atd, *nested;
<<<<<<< HEAD
	u32 i, first = cb->args[2];
	const struct set_elem *e;
=======
	u32 i = 0, first = cb->args[IPSET_CB_ARG0];
	struct set_elem *e;
	int ret = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	atd = ipset_nest_start(skb, IPSET_ATTR_ADT);
	if (!atd)
		return -EMSGSIZE;
<<<<<<< HEAD
	for (; cb->args[2] < map->size; cb->args[2]++) {
		i = cb->args[2];
		e = list_set_elem(map, i);
		if (e->id == IPSET_INVALID_ID)
			goto finish;
		if (SET_WITH_TIMEOUT(set) &&
		    ip_set_timeout_expired(ext_timeout(e, map)))
			continue;
		nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
		if (!nested) {
			if (i == first) {
				nla_nest_cancel(skb, atd);
				return -EMSGSIZE;
			} else
				goto nla_put_failure;
		}
		if (nla_put_string(skb, IPSET_ATTR_NAME,
				   ip_set_name_byindex(e->id)))
			goto nla_put_failure;
		if (SET_WITH_TIMEOUT(set) &&
		    nla_put_net32(skb, IPSET_ATTR_TIMEOUT,
				  htonl(ip_set_timeout_get(
						ext_timeout(e, map)))))
			goto nla_put_failure;
		if (SET_WITH_COUNTER(set) &&
		    ip_set_put_counter(skb, ext_counter(e, map)))
			goto nla_put_failure;
		ipset_nest_end(skb, nested);
	}
finish:
	ipset_nest_end(skb, atd);
	/* Set listing finished */
	cb->args[2] = 0;
	return 0;

nla_put_failure:
	nla_nest_cancel(skb, nested);
	ipset_nest_end(skb, atd);
	if (unlikely(i == first)) {
		cb->args[2] = 0;
		return -EMSGSIZE;
	}
	return 0;
=======

	rcu_read_lock();
	list_for_each_entry_rcu(e, &map->members, list) {
		if (i < first ||
		    (SET_WITH_TIMEOUT(set) &&
		     ip_set_timeout_expired(ext_timeout(e, set)))) {
			i++;
			continue;
		}
		nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
		if (!nested)
			goto nla_put_failure;
		if (nla_put_string(skb, IPSET_ATTR_NAME,
				   ip_set_name_byindex(map->net, e->id)))
			goto nla_put_failure;
		if (ip_set_put_extensions(skb, set, e, true))
			goto nla_put_failure;
		ipset_nest_end(skb, nested);
		i++;
	}

	ipset_nest_end(skb, atd);
	/* Set listing finished */
	cb->args[IPSET_CB_ARG0] = 0;
	goto out;

nla_put_failure:
	nla_nest_cancel(skb, nested);
	if (unlikely(i == first)) {
		nla_nest_cancel(skb, atd);
		cb->args[IPSET_CB_ARG0] = 0;
		ret = -EMSGSIZE;
	} else {
		cb->args[IPSET_CB_ARG0] = i;
		ipset_nest_end(skb, atd);
	}
out:
	rcu_read_unlock();
	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static bool
list_set_same_set(const struct ip_set *a, const struct ip_set *b)
{
	const struct list_set *x = a->data;
	const struct list_set *y = b->data;

	return x->size == y->size &&
<<<<<<< HEAD
	       x->timeout == y->timeout &&
=======
	       a->timeout == b->timeout &&
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	       a->extensions == b->extensions;
}

static const struct ip_set_type_variant set_variant = {
	.kadt	= list_set_kadt,
	.uadt	= list_set_uadt,
	.adt	= {
		[IPSET_ADD] = list_set_uadd,
		[IPSET_DEL] = list_set_udel,
		[IPSET_TEST] = list_set_utest,
	},
	.destroy = list_set_destroy,
	.flush	= list_set_flush,
	.head	= list_set_head,
	.list	= list_set_list,
	.same_set = list_set_same_set,
};

static void
list_set_gc(unsigned long ul_set)
{
<<<<<<< HEAD
	struct ip_set *set = (struct ip_set *) ul_set;
	struct list_set *map = set->data;

	write_lock_bh(&set->lock);
	set_cleanup_entries(set);
	write_unlock_bh(&set->lock);

	map->gc.expires = jiffies + IPSET_GC_PERIOD(map->timeout) * HZ;
=======
	struct ip_set *set = (struct ip_set *)ul_set;
	struct list_set *map = set->data;

	spin_lock_bh(&set->lock);
	set_cleanup_entries(set);
	spin_unlock_bh(&set->lock);

	map->gc.expires = jiffies + IPSET_GC_PERIOD(set->timeout) * HZ;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	add_timer(&map->gc);
}

static void
list_set_gc_init(struct ip_set *set, void (*gc)(unsigned long ul_set))
{
	struct list_set *map = set->data;

	init_timer(&map->gc);
<<<<<<< HEAD
	map->gc.data = (unsigned long) set;
	map->gc.function = gc;
	map->gc.expires = jiffies + IPSET_GC_PERIOD(map->timeout) * HZ;
=======
	map->gc.data = (unsigned long)set;
	map->gc.function = gc;
	map->gc.expires = jiffies + IPSET_GC_PERIOD(set->timeout) * HZ;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	add_timer(&map->gc);
}

/* Create list:set type of sets */

<<<<<<< HEAD
static struct list_set *
init_list_set(struct ip_set *set, u32 size, size_t dsize,
	      unsigned long timeout)
{
	struct list_set *map;
	struct set_elem *e;
	u32 i;

	map = kzalloc(sizeof(*map) + size * dsize, GFP_KERNEL);
	if (!map)
		return NULL;

	map->size = size;
	map->dsize = dsize;
	map->timeout = timeout;
	set->data = map;

	for (i = 0; i < size; i++) {
		e = list_set_elem(map, i);
		e->id = IPSET_INVALID_ID;
	}

	return map;
}

static int
list_set_create(struct ip_set *set, struct nlattr *tb[], u32 flags)
{
	struct list_set *map;
	u32 size = IP_SET_LIST_DEFAULT_SIZE, cadt_flags = 0;
	unsigned long timeout = 0;
=======
static bool
init_list_set(struct net *net, struct ip_set *set, u32 size)
{
	struct list_set *map;

	map = kzalloc(sizeof(*map), GFP_KERNEL);
	if (!map)
		return false;

	map->size = size;
	map->net = net;
	INIT_LIST_HEAD(&map->members);
	set->data = map;

	return true;
}

static int
list_set_create(struct net *net, struct ip_set *set, struct nlattr *tb[],
		u32 flags)
{
	u32 size = IP_SET_LIST_DEFAULT_SIZE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (unlikely(!ip_set_optattr_netorder(tb, IPSET_ATTR_SIZE) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_SIZE])
		size = ip_set_get_h32(tb[IPSET_ATTR_SIZE]);
	if (size < IP_SET_LIST_MIN_SIZE)
		size = IP_SET_LIST_MIN_SIZE;

<<<<<<< HEAD
	if (tb[IPSET_ATTR_CADT_FLAGS])
		cadt_flags = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
	if (tb[IPSET_ATTR_TIMEOUT])
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	set->variant = &set_variant;
	if (cadt_flags & IPSET_FLAG_WITH_COUNTERS) {
		set->extensions |= IPSET_EXT_COUNTER;
		if (tb[IPSET_ATTR_TIMEOUT]) {
			map = init_list_set(set, size,
					sizeof(struct setct_elem), timeout);
			if (!map)
				return -ENOMEM;
			set->extensions |= IPSET_EXT_TIMEOUT;
			map->offset[IPSET_OFFSET_TIMEOUT] =
				offsetof(struct setct_elem, timeout);
			map->offset[IPSET_OFFSET_COUNTER] =
				offsetof(struct setct_elem, counter);
			list_set_gc_init(set, list_set_gc);
		} else {
			map = init_list_set(set, size,
					    sizeof(struct setc_elem), 0);
			if (!map)
				return -ENOMEM;
			map->offset[IPSET_OFFSET_COUNTER] =
				offsetof(struct setc_elem, counter);
		}
	} else if (tb[IPSET_ATTR_TIMEOUT]) {
		map = init_list_set(set, size,
				    sizeof(struct sett_elem), timeout);
		if (!map)
			return -ENOMEM;
		set->extensions |= IPSET_EXT_TIMEOUT;
		map->offset[IPSET_OFFSET_TIMEOUT] =
			offsetof(struct sett_elem, timeout);
		list_set_gc_init(set, list_set_gc);
	} else {
		map = init_list_set(set, size, sizeof(struct set_elem), 0);
		if (!map)
			return -ENOMEM;
=======
	set->variant = &set_variant;
	set->dsize = ip_set_elem_len(set, tb, sizeof(struct set_elem),
				     __alignof__(struct set_elem));
	if (!init_list_set(net, set, size))
		return -ENOMEM;
	if (tb[IPSET_ATTR_TIMEOUT]) {
		set->timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
		list_set_gc_init(set, list_set_gc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return 0;
}

static struct ip_set_type list_set_type __read_mostly = {
	.name		= "list:set",
	.protocol	= IPSET_PROTOCOL,
	.features	= IPSET_TYPE_NAME | IPSET_DUMP_LAST,
	.dimension	= IPSET_DIM_ONE,
	.family		= NFPROTO_UNSPEC,
<<<<<<< HEAD
	.revision_min	= REVISION_MIN,
	.revision_max	= REVISION_MAX,
=======
	.revision_min	= IPSET_TYPE_REV_MIN,
	.revision_max	= IPSET_TYPE_REV_MAX,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.create		= list_set_create,
	.create_policy	= {
		[IPSET_ATTR_SIZE]	= { .type = NLA_U32 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
	},
	.adt_policy	= {
		[IPSET_ATTR_NAME]	= { .type = NLA_STRING,
					    .len = IPSET_MAXNAMELEN },
		[IPSET_ATTR_NAMEREF]	= { .type = NLA_STRING,
					    .len = IPSET_MAXNAMELEN },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_LINENO]	= { .type = NLA_U32 },
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
		[IPSET_ATTR_BYTES]	= { .type = NLA_U64 },
		[IPSET_ATTR_PACKETS]	= { .type = NLA_U64 },
<<<<<<< HEAD
=======
		[IPSET_ATTR_COMMENT]	= { .type = NLA_NUL_STRING,
					    .len  = IPSET_MAX_COMMENT_SIZE },
		[IPSET_ATTR_SKBMARK]	= { .type = NLA_U64 },
		[IPSET_ATTR_SKBPRIO]	= { .type = NLA_U32 },
		[IPSET_ATTR_SKBQUEUE]	= { .type = NLA_U16 },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	.me		= THIS_MODULE,
};

static int __init
list_set_init(void)
{
	return ip_set_type_register(&list_set_type);
}

static void __exit
list_set_fini(void)
{
<<<<<<< HEAD
=======
	rcu_barrier();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ip_set_type_unregister(&list_set_type);
}

module_init(list_set_init);
module_exit(list_set_fini);
