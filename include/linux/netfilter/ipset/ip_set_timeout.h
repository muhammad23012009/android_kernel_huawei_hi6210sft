#ifndef _IP_SET_TIMEOUT_H
#define _IP_SET_TIMEOUT_H

/* Copyright (C) 2003-2013 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifdef __KERNEL__

/* How often should the gc be run by default */
#define IPSET_GC_TIME			(3 * 60)

/* Timeout period depending on the timeout value of the given set */
#define IPSET_GC_PERIOD(timeout) \
	((timeout/3) ? min_t(u32, (timeout)/3, IPSET_GC_TIME) : 1)

/* Entry is set with no timeout value */
#define IPSET_ELEM_PERMANENT	0

/* Set is defined with timeout support: timeout value may be 0 */
#define IPSET_NO_TIMEOUT	UINT_MAX

<<<<<<< HEAD
#define ip_set_adt_opt_timeout(opt, map)	\
((opt)->ext.timeout != IPSET_NO_TIMEOUT ? (opt)->ext.timeout : (map)->timeout)
=======
#define ip_set_adt_opt_timeout(opt, set)	\
((opt)->ext.timeout != IPSET_NO_TIMEOUT ? (opt)->ext.timeout : (set)->timeout)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline unsigned int
ip_set_timeout_uget(struct nlattr *tb)
{
	unsigned int timeout = ip_set_get_h32(tb);

	/* Normalize to fit into jiffies */
	if (timeout > UINT_MAX/MSEC_PER_SEC)
		timeout = UINT_MAX/MSEC_PER_SEC;

	/* Userspace supplied TIMEOUT parameter: adjust crazy size */
	return timeout == IPSET_NO_TIMEOUT ? IPSET_NO_TIMEOUT - 1 : timeout;
}

static inline bool
<<<<<<< HEAD
ip_set_timeout_test(unsigned long timeout)
{
	return timeout == IPSET_ELEM_PERMANENT ||
	       time_is_after_jiffies(timeout);
}

static inline bool
ip_set_timeout_expired(unsigned long *timeout)
{
	return *timeout != IPSET_ELEM_PERMANENT &&
	       time_is_before_jiffies(*timeout);
}

static inline void
ip_set_timeout_set(unsigned long *timeout, u32 t)
{
	if (!t) {
=======
ip_set_timeout_expired(unsigned long *t)
{
	return *t != IPSET_ELEM_PERMANENT && time_is_before_jiffies(*t);
}

static inline void
ip_set_timeout_set(unsigned long *timeout, u32 value)
{
	unsigned long t;

	if (!value) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		*timeout = IPSET_ELEM_PERMANENT;
		return;
	}

<<<<<<< HEAD
	*timeout = msecs_to_jiffies(t * 1000) + jiffies;
	if (*timeout == IPSET_ELEM_PERMANENT)
		/* Bingo! :-) */
		(*timeout)--;
=======
	t = msecs_to_jiffies(value * MSEC_PER_SEC) + jiffies;
	if (t == IPSET_ELEM_PERMANENT)
		/* Bingo! :-) */
		t--;
	*timeout = t;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline u32
ip_set_timeout_get(unsigned long *timeout)
{
<<<<<<< HEAD
	return *timeout == IPSET_ELEM_PERMANENT ? 0 :
		jiffies_to_msecs(*timeout - jiffies)/1000;
=======
	u32 t;

	if (*timeout == IPSET_ELEM_PERMANENT)
		return 0;

	t = jiffies_to_msecs(*timeout - jiffies)/MSEC_PER_SEC;
	/* Zero value in userspace means no timeout */
	return t == 0 ? 1 : t;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif	/* __KERNEL__ */
#endif /* _IP_SET_TIMEOUT_H */
