#ifndef _PVCLOCK_GTOD_H
#define _PVCLOCK_GTOD_H

#include <linux/notifier.h>

<<<<<<< HEAD
=======
/*
 * The pvclock gtod notifier is called when the system time is updated
 * and is used to keep guest time synchronized with host time.
 *
 * The 'action' parameter in the notifier function is false (0), or
 * true (non-zero) if system time was stepped.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int pvclock_gtod_register_notifier(struct notifier_block *nb);
extern int pvclock_gtod_unregister_notifier(struct notifier_block *nb);

#endif /* _PVCLOCK_GTOD_H */
