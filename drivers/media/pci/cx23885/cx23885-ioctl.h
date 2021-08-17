/*
 *  Driver for the Conexant CX23885/7/8 PCIe bridge
 *
 *  Various common ioctl() support functions
 *
 *  Copyright (c) 2009 Andy Walls <awalls@md.metrocast.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
<<<<<<< HEAD
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#ifndef _CX23885_IOCTL_H_
#define _CX23885_IOCTL_H_

<<<<<<< HEAD
int cx23885_g_chip_ident(struct file *file, void *fh,
			 struct v4l2_dbg_chip_ident *chip);
=======
int cx23885_g_chip_info(struct file *file, void *fh,
			 struct v4l2_dbg_chip_info *chip);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_VIDEO_ADV_DEBUG
int cx23885_g_register(struct file *file, void *fh,
		       struct v4l2_dbg_register *reg);


int cx23885_s_register(struct file *file, void *fh,
		       const struct v4l2_dbg_register *reg);

#endif
#endif
