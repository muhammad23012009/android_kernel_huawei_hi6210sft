#ifndef __LINUX_MUTEX_DEBUG_H
#define __LINUX_MUTEX_DEBUG_H

#include <linux/linkage.h>
#include <linux/lockdep.h>
<<<<<<< HEAD
=======
#include <linux/debug_locks.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Mutexes - debugging helpers:
 */

#define __DEBUG_MUTEX_INITIALIZER(lockname)				\
	, .magic = &lockname

#define mutex_init(mutex)						\
do {									\
	static struct lock_class_key __key;				\
									\
	__mutex_init((mutex), #mutex, &__key);				\
} while (0)

extern void mutex_destroy(struct mutex *lock);

#endif
