/*
 * ring_hw.h - common functionality for iio hardware ring buffers
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * Copyright (c) 2009 Jonathan Cameron <jic23@kernel.org>
 *
 */

<<<<<<< HEAD
=======
#ifndef _RING_HW_H_
#define _RING_HW_H_

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * struct iio_hw_ring_buffer- hardware ring buffer
 * @buf:	generic ring buffer elements
 * @private:	device specific data
 */
struct iio_hw_buffer {
	struct iio_buffer buf;
	void *private;
};

#define iio_to_hw_buf(r) container_of(r, struct iio_hw_buffer, buf)
<<<<<<< HEAD
=======

#endif /* _RING_HW_H_ */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
