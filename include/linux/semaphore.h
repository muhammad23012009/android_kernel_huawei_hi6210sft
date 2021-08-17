/*
 * Copyright (c) 2008 Intel Corporation
 * Author: Matthew Wilcox <willy@linux.intel.com>
 *
 * Distributed under the terms of the GNU GPL, version 2
 *
 * Please see kernel/semaphore.c for documentation of these functions
 */
#ifndef __LINUX_SEMAPHORE_H
#define __LINUX_SEMAPHORE_H

#include <linux/list.h>
#include <linux/spinlock.h>
<<<<<<< HEAD
#ifdef CONFIG_ILOCKDEP
#include <linux/ilockdep.h>
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Please don't access any members of this structure directly */
struct semaphore {
	raw_spinlock_t		lock;
	unsigned int		count;
	struct list_head	wait_list;
<<<<<<< HEAD
#ifdef CONFIG_ILOCKDEP
	struct ilockdep_map     idep_map;
#endif
};
#ifdef CONFIG_ILOCKDEP
#define __IDEP_MAP_SEM_INITIALIZER(lockname) \
	.idep_map = { .name = #lockname },
#else
#define __IDEP_MAP_SEM_INITIALIZER(lockname)
#endif
=======
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define __SEMAPHORE_INITIALIZER(name, n)				\
{									\
	.lock		= __RAW_SPIN_LOCK_UNLOCKED((name).lock),	\
	.count		= n,						\
	.wait_list	= LIST_HEAD_INIT((name).wait_list),		\
<<<<<<< HEAD
	__IDEP_MAP_SEM_INITIALIZER(name)				\
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#define DEFINE_SEMAPHORE(name)	\
	struct semaphore name = __SEMAPHORE_INITIALIZER(name, 1)

<<<<<<< HEAD
#ifdef CONFIG_ILOCKDEP
#define sema_init(sem, val)	__sema_init((sem), #sem, val);

static inline void __sema_init(struct semaphore *sem, const char *name,	int val)
{
	static struct lock_class_key __key;
	*sem = (struct semaphore) __SEMAPHORE_INITIALIZER(*sem, val);
	sem->idep_map.name = name;
	lockdep_init_map(&sem->lock.dep_map, name, &__key, 0);
}
#else
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline void sema_init(struct semaphore *sem, int val)
{
	static struct lock_class_key __key;
	*sem = (struct semaphore) __SEMAPHORE_INITIALIZER(*sem, val);
	lockdep_init_map(&sem->lock.dep_map, "semaphore->lock", &__key, 0);
}
<<<<<<< HEAD
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern void down(struct semaphore *sem);
extern int __must_check down_interruptible(struct semaphore *sem);
extern int __must_check down_killable(struct semaphore *sem);
extern int __must_check down_trylock(struct semaphore *sem);
extern int __must_check down_timeout(struct semaphore *sem, long jiffies);
extern void up(struct semaphore *sem);

#endif /* __LINUX_SEMAPHORE_H */
