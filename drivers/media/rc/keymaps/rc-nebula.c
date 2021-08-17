/* nebula.h - Keytable for nebula Remote Controller
 *
 * keymap imported from ir-keymaps.c
 *
<<<<<<< HEAD
 * Copyright (c) 2010 by Mauro Carvalho Chehab <mchehab@redhat.com>
=======
 * Copyright (c) 2010 by Mauro Carvalho Chehab
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <media/rc-map.h>
#include <linux/module.h>

static struct rc_map_table nebula[] = {
<<<<<<< HEAD
	{ 0x00, KEY_0 },
	{ 0x01, KEY_1 },
	{ 0x02, KEY_2 },
	{ 0x03, KEY_3 },
	{ 0x04, KEY_4 },
	{ 0x05, KEY_5 },
	{ 0x06, KEY_6 },
	{ 0x07, KEY_7 },
	{ 0x08, KEY_8 },
	{ 0x09, KEY_9 },
	{ 0x0a, KEY_TV },
	{ 0x0b, KEY_AUX },
	{ 0x0c, KEY_DVD },
	{ 0x0d, KEY_POWER },
	{ 0x0e, KEY_CAMERA },	/* labelled 'Picture' */
	{ 0x0f, KEY_AUDIO },
	{ 0x10, KEY_INFO },
	{ 0x11, KEY_F13 },	/* 16:9 */
	{ 0x12, KEY_F14 },	/* 14:9 */
	{ 0x13, KEY_EPG },
	{ 0x14, KEY_EXIT },
	{ 0x15, KEY_MENU },
	{ 0x16, KEY_UP },
	{ 0x17, KEY_DOWN },
	{ 0x18, KEY_LEFT },
	{ 0x19, KEY_RIGHT },
	{ 0x1a, KEY_ENTER },
	{ 0x1b, KEY_CHANNELUP },
	{ 0x1c, KEY_CHANNELDOWN },
	{ 0x1d, KEY_VOLUMEUP },
	{ 0x1e, KEY_VOLUMEDOWN },
	{ 0x1f, KEY_RED },
	{ 0x20, KEY_GREEN },
	{ 0x21, KEY_YELLOW },
	{ 0x22, KEY_BLUE },
	{ 0x23, KEY_SUBTITLE },
	{ 0x24, KEY_F15 },	/* AD */
	{ 0x25, KEY_TEXT },
	{ 0x26, KEY_MUTE },
	{ 0x27, KEY_REWIND },
	{ 0x28, KEY_STOP },
	{ 0x29, KEY_PLAY },
	{ 0x2a, KEY_FASTFORWARD },
	{ 0x2b, KEY_F16 },	/* chapter */
	{ 0x2c, KEY_PAUSE },
	{ 0x2d, KEY_PLAY },
	{ 0x2e, KEY_RECORD },
	{ 0x2f, KEY_F17 },	/* picture in picture */
	{ 0x30, KEY_KPPLUS },	/* zoom in */
	{ 0x31, KEY_KPMINUS },	/* zoom out */
	{ 0x32, KEY_F18 },	/* capture */
	{ 0x33, KEY_F19 },	/* web */
	{ 0x34, KEY_EMAIL },
	{ 0x35, KEY_PHONE },
	{ 0x36, KEY_PC },
=======
	{ 0x0000, KEY_0 },
	{ 0x0001, KEY_1 },
	{ 0x0002, KEY_2 },
	{ 0x0003, KEY_3 },
	{ 0x0004, KEY_4 },
	{ 0x0005, KEY_5 },
	{ 0x0006, KEY_6 },
	{ 0x0007, KEY_7 },
	{ 0x0008, KEY_8 },
	{ 0x0009, KEY_9 },
	{ 0x000a, KEY_TV },
	{ 0x000b, KEY_AUX },
	{ 0x000c, KEY_DVD },
	{ 0x000d, KEY_POWER },
	{ 0x000e, KEY_CAMERA },	/* labelled 'Picture' */
	{ 0x000f, KEY_AUDIO },
	{ 0x0010, KEY_INFO },
	{ 0x0011, KEY_F13 },	/* 16:9 */
	{ 0x0012, KEY_F14 },	/* 14:9 */
	{ 0x0013, KEY_EPG },
	{ 0x0014, KEY_EXIT },
	{ 0x0015, KEY_MENU },
	{ 0x0016, KEY_UP },
	{ 0x0017, KEY_DOWN },
	{ 0x0018, KEY_LEFT },
	{ 0x0019, KEY_RIGHT },
	{ 0x001a, KEY_ENTER },
	{ 0x001b, KEY_CHANNELUP },
	{ 0x001c, KEY_CHANNELDOWN },
	{ 0x001d, KEY_VOLUMEUP },
	{ 0x001e, KEY_VOLUMEDOWN },
	{ 0x001f, KEY_RED },
	{ 0x0020, KEY_GREEN },
	{ 0x0021, KEY_YELLOW },
	{ 0x0022, KEY_BLUE },
	{ 0x0023, KEY_SUBTITLE },
	{ 0x0024, KEY_F15 },	/* AD */
	{ 0x0025, KEY_TEXT },
	{ 0x0026, KEY_MUTE },
	{ 0x0027, KEY_REWIND },
	{ 0x0028, KEY_STOP },
	{ 0x0029, KEY_PLAY },
	{ 0x002a, KEY_FASTFORWARD },
	{ 0x002b, KEY_F16 },	/* chapter */
	{ 0x002c, KEY_PAUSE },
	{ 0x002d, KEY_PLAY },
	{ 0x002e, KEY_RECORD },
	{ 0x002f, KEY_F17 },	/* picture in picture */
	{ 0x0030, KEY_KPPLUS },	/* zoom in */
	{ 0x0031, KEY_KPMINUS },	/* zoom out */
	{ 0x0032, KEY_F18 },	/* capture */
	{ 0x0033, KEY_F19 },	/* web */
	{ 0x0034, KEY_EMAIL },
	{ 0x0035, KEY_PHONE },
	{ 0x0036, KEY_PC },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static struct rc_map_list nebula_map = {
	.map = {
		.scan    = nebula,
		.size    = ARRAY_SIZE(nebula),
<<<<<<< HEAD
		.rc_type = RC_TYPE_UNKNOWN,	/* Legacy IR type */
=======
		.rc_type = RC_TYPE_RC5,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.name    = RC_MAP_NEBULA,
	}
};

static int __init init_rc_map_nebula(void)
{
	return rc_map_register(&nebula_map);
}

static void __exit exit_rc_map_nebula(void)
{
	rc_map_unregister(&nebula_map);
}

module_init(init_rc_map_nebula)
module_exit(exit_rc_map_nebula)

MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
=======
MODULE_AUTHOR("Mauro Carvalho Chehab");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
