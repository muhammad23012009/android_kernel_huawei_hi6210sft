#ifndef EADM_SCH_H
#define EADM_SCH_H

<<<<<<< HEAD
=======
#include <linux/completion.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/list.h>
#include "orb.h"

struct eadm_private {
	union orb orb;
	enum {EADM_IDLE, EADM_BUSY, EADM_NOT_OPER} state;
<<<<<<< HEAD
	struct timer_list timer;
	struct list_head head;
	struct subchannel *sch;
=======
	struct completion *completion;
	struct subchannel *sch;
	struct timer_list timer;
	struct list_head head;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
} __aligned(8);

#define get_eadm_private(n) ((struct eadm_private *)dev_get_drvdata(&n->dev))
#define set_eadm_private(n, p) (dev_set_drvdata(&n->dev, p))

#endif
