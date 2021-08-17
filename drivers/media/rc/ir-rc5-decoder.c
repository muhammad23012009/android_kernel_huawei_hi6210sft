<<<<<<< HEAD
/* ir-rc5-decoder.c - handle RC5(x) IR Pulse/Space protocol
 *
 * Copyright (C) 2010 by Mauro Carvalho Chehab <mchehab@redhat.com>
=======
/* ir-rc5-decoder.c - decoder for RC5(x) and StreamZap protocols
 *
 * Copyright (C) 2010 by Mauro Carvalho Chehab
 * Copyright (C) 2010 by Jarod Wilson <jarod@redhat.com>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

/*
<<<<<<< HEAD
 * This code handles 14 bits RC5 protocols and 20 bits RC5x protocols.
 * There are other variants that use a different number of bits.
 * This is currently unsupported.
 * It considers a carrier of 36 kHz, with a total of 14/20 bits, where
 * the first two bits are start bits, and a third one is a filing bit
=======
 * This decoder handles the 14 bit RC5 protocol, 15 bit "StreamZap" protocol
 * and 20 bit RC5x protocol.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#include "rc-core-priv.h"
#include <linux/module.h>

#define RC5_NBITS		14
<<<<<<< HEAD
=======
#define RC5_SZ_NBITS		15
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define RC5X_NBITS		20
#define CHECK_RC5X_NBITS	8
#define RC5_UNIT		888888 /* ns */
#define RC5_BIT_START		(1 * RC5_UNIT)
#define RC5_BIT_END		(1 * RC5_UNIT)
#define RC5X_SPACE		(4 * RC5_UNIT)
<<<<<<< HEAD
=======
#define RC5_TRAILER		(6 * RC5_UNIT) /* In reality, approx 100 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

enum rc5_state {
	STATE_INACTIVE,
	STATE_BIT_START,
	STATE_BIT_END,
	STATE_CHECK_RC5X,
	STATE_FINISHED,
};

/**
 * ir_rc5_decode() - Decode one RC-5 pulse or space
 * @dev:	the struct rc_dev descriptor of the device
 * @ev:		the struct ir_raw_event descriptor of the pulse/space
 *
 * This function returns -EINVAL if the pulse violates the state machine
 */
static int ir_rc5_decode(struct rc_dev *dev, struct ir_raw_event ev)
{
	struct rc5_dec *data = &dev->raw->rc5;
	u8 toggle;
	u32 scancode;
<<<<<<< HEAD

	if (!(dev->enabled_protocols & (RC_BIT_RC5 | RC_BIT_RC5X)))
		return 0;
=======
	enum rc_type protocol;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!is_timing_event(ev)) {
		if (ev.reset)
			data->state = STATE_INACTIVE;
		return 0;
	}

	if (!geq_margin(ev.duration, RC5_UNIT, RC5_UNIT / 2))
		goto out;

again:
<<<<<<< HEAD
	IR_dprintk(2, "RC5(x) decode started at state %i (%uus %s)\n",
=======
	IR_dprintk(2, "RC5(x/sz) decode started at state %i (%uus %s)\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		   data->state, TO_US(ev.duration), TO_STR(ev.pulse));

	if (!geq_margin(ev.duration, RC5_UNIT, RC5_UNIT / 2))
		return 0;

	switch (data->state) {

	case STATE_INACTIVE:
		if (!ev.pulse)
			break;

		data->state = STATE_BIT_START;
		data->count = 1;
<<<<<<< HEAD
		/* We just need enough bits to get to STATE_CHECK_RC5X */
		data->wanted_bits = RC5X_NBITS;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		decrease_duration(&ev, RC5_BIT_START);
		goto again;

	case STATE_BIT_START:
<<<<<<< HEAD
=======
		if (!ev.pulse && geq_margin(ev.duration, RC5_TRAILER, RC5_UNIT / 2)) {
			data->state = STATE_FINISHED;
			goto again;
		}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!eq_margin(ev.duration, RC5_BIT_START, RC5_UNIT / 2))
			break;

		data->bits <<= 1;
		if (!ev.pulse)
			data->bits |= 1;
		data->count++;
		data->state = STATE_BIT_END;
		return 0;

	case STATE_BIT_END:
		if (!is_transition(&ev, &dev->raw->prev_ev))
			break;

<<<<<<< HEAD
		if (data->count == data->wanted_bits)
			data->state = STATE_FINISHED;
		else if (data->count == CHECK_RC5X_NBITS)
=======
		if (data->count == CHECK_RC5X_NBITS)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			data->state = STATE_CHECK_RC5X;
		else
			data->state = STATE_BIT_START;

		decrease_duration(&ev, RC5_BIT_END);
		goto again;

	case STATE_CHECK_RC5X:
		if (!ev.pulse && geq_margin(ev.duration, RC5X_SPACE, RC5_UNIT / 2)) {
<<<<<<< HEAD
			/* RC5X */
			data->wanted_bits = RC5X_NBITS;
			decrease_duration(&ev, RC5X_SPACE);
		} else {
			/* RC5 */
			data->wanted_bits = RC5_NBITS;
		}
=======
			data->is_rc5x = true;
			decrease_duration(&ev, RC5X_SPACE);
		} else
			data->is_rc5x = false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		data->state = STATE_BIT_START;
		goto again;

	case STATE_FINISHED:
		if (ev.pulse)
			break;

<<<<<<< HEAD
		if (data->wanted_bits == RC5X_NBITS) {
=======
		if (data->is_rc5x && data->count == RC5X_NBITS) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* RC5X */
			u8 xdata, command, system;
			if (!(dev->enabled_protocols & RC_BIT_RC5X)) {
				data->state = STATE_INACTIVE;
				return 0;
			}
			xdata    = (data->bits & 0x0003F) >> 0;
			command  = (data->bits & 0x00FC0) >> 6;
			system   = (data->bits & 0x1F000) >> 12;
			toggle   = (data->bits & 0x20000) ? 1 : 0;
			command += (data->bits & 0x01000) ? 0 : 0x40;
			scancode = system << 16 | command << 8 | xdata;
<<<<<<< HEAD

			IR_dprintk(1, "RC5X scancode 0x%06x (toggle: %u)\n",
				   scancode, toggle);

		} else {
=======
			protocol = RC_TYPE_RC5X;

		} else if (!data->is_rc5x && data->count == RC5_NBITS) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* RC5 */
			u8 command, system;
			if (!(dev->enabled_protocols & RC_BIT_RC5)) {
				data->state = STATE_INACTIVE;
				return 0;
			}
			command  = (data->bits & 0x0003F) >> 0;
			system   = (data->bits & 0x007C0) >> 6;
			toggle   = (data->bits & 0x00800) ? 1 : 0;
			command += (data->bits & 0x01000) ? 0 : 0x40;
			scancode = system << 8 | command;
<<<<<<< HEAD

			IR_dprintk(1, "RC5 scancode 0x%04x (toggle: %u)\n",
				   scancode, toggle);
		}

		rc_keydown(dev, scancode, toggle);
=======
			protocol = RC_TYPE_RC5;

		} else if (!data->is_rc5x && data->count == RC5_SZ_NBITS) {
			/* RC5 StreamZap */
			u8 command, system;
			if (!(dev->enabled_protocols & RC_BIT_RC5_SZ)) {
				data->state = STATE_INACTIVE;
				return 0;
			}
			command  = (data->bits & 0x0003F) >> 0;
			system   = (data->bits & 0x02FC0) >> 6;
			toggle   = (data->bits & 0x01000) ? 1 : 0;
			scancode = system << 6 | command;
			protocol = RC_TYPE_RC5_SZ;

		} else
			break;

		IR_dprintk(1, "RC5(x/sz) scancode 0x%06x (p: %u, t: %u)\n",
			   scancode, protocol, toggle);

		rc_keydown(dev, protocol, scancode, toggle);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		data->state = STATE_INACTIVE;
		return 0;
	}

out:
<<<<<<< HEAD
	IR_dprintk(1, "RC5(x) decode failed at state %i (%uus %s)\n",
		   data->state, TO_US(ev.duration), TO_STR(ev.pulse));
=======
	IR_dprintk(1, "RC5(x/sz) decode failed at state %i count %d (%uus %s)\n",
		   data->state, data->count, TO_US(ev.duration), TO_STR(ev.pulse));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	data->state = STATE_INACTIVE;
	return -EINVAL;
}

static struct ir_raw_handler rc5_handler = {
<<<<<<< HEAD
	.protocols	= RC_BIT_RC5 | RC_BIT_RC5X,
=======
	.protocols	= RC_BIT_RC5 | RC_BIT_RC5X | RC_BIT_RC5_SZ,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.decode		= ir_rc5_decode,
};

static int __init ir_rc5_decode_init(void)
{
	ir_raw_handler_register(&rc5_handler);

<<<<<<< HEAD
	printk(KERN_INFO "IR RC5(x) protocol handler initialized\n");
=======
	printk(KERN_INFO "IR RC5(x/sz) protocol handler initialized\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static void __exit ir_rc5_decode_exit(void)
{
	ir_raw_handler_unregister(&rc5_handler);
}

module_init(ir_rc5_decode_init);
module_exit(ir_rc5_decode_exit);

MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
MODULE_AUTHOR("Red Hat Inc. (http://www.redhat.com)");
MODULE_DESCRIPTION("RC5(x) IR protocol decoder");
=======
MODULE_AUTHOR("Mauro Carvalho Chehab and Jarod Wilson");
MODULE_AUTHOR("Red Hat Inc. (http://www.redhat.com)");
MODULE_DESCRIPTION("RC5(x/sz) IR protocol decoder");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
