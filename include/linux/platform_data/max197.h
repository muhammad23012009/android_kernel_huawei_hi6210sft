/*
 * Maxim MAX197 A/D Converter Driver
 *
 * Copyright (c) 2012 Savoir-faire Linux Inc.
 *          Vivien Didelot <vivien.didelot@savoirfairelinux.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * For further information, see the Documentation/hwmon/max197 file.
 */

<<<<<<< HEAD
=======
#ifndef _PDATA_MAX197_H
#define _PDATA_MAX197_H

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * struct max197_platform_data - MAX197 connectivity info
 * @convert:	Function used to start a conversion with control byte ctrl.
 *		It must return the raw data, or a negative error code.
 */
struct max197_platform_data {
	int (*convert)(u8 ctrl);
};
<<<<<<< HEAD
=======

#endif /* _PDATA_MAX197_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
