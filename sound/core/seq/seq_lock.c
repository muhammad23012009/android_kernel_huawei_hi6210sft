/*
 *  Do sleep inside a spin-lock
 *  Copyright (c) 1999 by Takashi Iwai <tiwai@suse.de>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <linux/export.h>
#include <sound/core.h>
#include "seq_lock.h"

<<<<<<< HEAD
#if defined(CONFIG_SMP) || defined(CONFIG_SND_DEBUG)

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* wait until all locks are released */
void snd_use_lock_sync_helper(snd_use_lock_t *lockp, const char *file, int line)
{
	int warn_count = 5 * HZ;

	if (atomic_read(lockp) < 0) {
<<<<<<< HEAD
		printk(KERN_WARNING "seq_lock: lock trouble [counter = %d] in %s:%d\n", atomic_read(lockp), file, line);
=======
		pr_warn("ALSA: seq_lock: lock trouble [counter = %d] in %s:%d\n", atomic_read(lockp), file, line);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;
	}
	while (atomic_read(lockp) > 0) {
		if (warn_count-- == 0)
			pr_warn("ALSA: seq_lock: waiting [%d left] in %s:%d\n", atomic_read(lockp), file, line);
		schedule_timeout_uninterruptible(1);
	}
}

EXPORT_SYMBOL(snd_use_lock_sync_helper);
<<<<<<< HEAD

#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
