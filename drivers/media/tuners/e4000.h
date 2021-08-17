/*
 * Elonics E4000 silicon tuner driver
 *
 * Copyright (C) 2012 Antti Palosaari <crope@iki.fi>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef E4000_H
#define E4000_H

<<<<<<< HEAD
#include <linux/kconfig.h>
#include "dvb_frontend.h"

struct e4000_config {
	/*
	 * I2C address
	 * 0x64, 0x65, 0x66, 0x67
	 */
	u8 i2c_addr;
=======
#include "dvb_frontend.h"

/*
 * I2C address
 * 0x64, 0x65, 0x66, 0x67
 */
struct e4000_config {
	/*
	 * frontend
	 */
	struct dvb_frontend *fe;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * clock
	 */
	u32 clock;
};

<<<<<<< HEAD
#if IS_ENABLED(CONFIG_MEDIA_TUNER_E4000)
extern struct dvb_frontend *e4000_attach(struct dvb_frontend *fe,
		struct i2c_adapter *i2c, const struct e4000_config *cfg);
#else
static inline struct dvb_frontend *e4000_attach(struct dvb_frontend *fe,
		struct i2c_adapter *i2c, const struct e4000_config *cfg)
{
	pr_warn("%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
