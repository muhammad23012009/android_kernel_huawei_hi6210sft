/*
 *  Unified handling of special chars.
 *
 *    Copyright IBM Corp. 2001
 *    Author(s): Fritz Elfert <felfert@millenux.com> <elfert@de.ibm.com>
 *
 */

#include <linux/tty.h>
<<<<<<< HEAD
=======
#include <linux/sysrq.h>
#include <linux/workqueue.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern unsigned int
ctrlchar_handle(const unsigned char *buf, int len, struct tty_struct *tty);


#define CTRLCHAR_NONE  (1 << 8)
#define CTRLCHAR_CTRL  (2 << 8)
#define CTRLCHAR_SYSRQ (3 << 8)

#define CTRLCHAR_MASK (~0xffu)
<<<<<<< HEAD
=======


#ifdef CONFIG_MAGIC_SYSRQ
struct sysrq_work {
	int key;
	struct work_struct work;
};

void schedule_sysrq_work(struct sysrq_work *sw);
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
