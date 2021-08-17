#ifndef __LINUX_IIO_KFIFO_BUF_H__
#define __LINUX_IIO_KFIFO_BUF_H__

#include <linux/kfifo.h>
#include <linux/iio/iio.h>
#include <linux/iio/buffer.h>

<<<<<<< HEAD
struct iio_buffer *iio_kfifo_allocate(struct iio_dev *indio_dev);
void iio_kfifo_free(struct iio_buffer *r);

=======
struct iio_buffer *iio_kfifo_allocate(void);
void iio_kfifo_free(struct iio_buffer *r);

struct iio_buffer *devm_iio_kfifo_allocate(struct device *dev);
void devm_iio_kfifo_free(struct device *dev, struct iio_buffer *r);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
