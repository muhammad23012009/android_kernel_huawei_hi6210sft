/*
 *  mxl111sf-i2c.h - driver for the MaxLinear MXL111SF
 *
<<<<<<< HEAD
 *  Copyright (C) 2010 Michael Krufky <mkrufky@kernellabs.com>
=======
 *  Copyright (C) 2010-2014 Michael Krufky <mkrufky@linuxtv.org>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DVB_USB_MXL111SF_I2C_H_
#define _DVB_USB_MXL111SF_I2C_H_

#include <linux/i2c.h>

int mxl111sf_i2c_xfer(struct i2c_adapter *adap,
		      struct i2c_msg msg[], int num);

#endif /* _DVB_USB_MXL111SF_I2C_H_ */
<<<<<<< HEAD

/*
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
