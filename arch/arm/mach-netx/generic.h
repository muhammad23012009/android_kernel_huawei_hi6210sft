/*
 * arch/arm/mach-netx/generic.h
 *
 * Copyright (c) 2005 Sascha Hauer <s.hauer@pengutronix.de>, Pengutronix
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

<<<<<<< HEAD
extern void __init netx_map_io(void);
extern void __init netx_init_irq(void);
extern void netx_restart(char, const char *);
=======
#include <linux/reboot.h>

extern void __init netx_map_io(void);
extern void __init netx_init_irq(void);
extern void netx_restart(enum reboot_mode, const char *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern void netx_timer_init(void);
