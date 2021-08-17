/*
 * STMicroelectronics sensors buffer library driver
 *
 * Copyright 2012-2013 STMicroelectronics Inc.
 *
 * Denis Ciocca <denis.ciocca@st.com>
 *
 * Licensed under the GPL-2.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/iio/iio.h>
#include <linux/iio/trigger.h>
#include <linux/interrupt.h>
#include <linux/iio/buffer.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/iio/triggered_buffer.h>
#include <linux/irqreturn.h>

#include <linux/iio/common/st_sensors.h>


<<<<<<< HEAD
int st_sensors_get_buffer_element(struct iio_dev *indio_dev, u8 *buf)
{
	int i, n = 0, len;
	u8 addr[ST_SENSORS_NUMBER_DATA_CHANNELS];
	struct st_sensor_data *sdata = iio_priv(indio_dev);

	for (i = 0; i < ST_SENSORS_NUMBER_DATA_CHANNELS; i++) {
		if (test_bit(i, indio_dev->active_scan_mask)) {
			addr[n] = indio_dev->channels[i].address;
			n++;
		}
	}
	switch (n) {
	case 1:
		len = sdata->tf->read_multiple_byte(&sdata->tb, sdata->dev,
			addr[0], ST_SENSORS_BYTE_FOR_CHANNEL, buf,
			sdata->multiread_bit);
		break;
	case 2:
		if ((addr[1] - addr[0]) == ST_SENSORS_BYTE_FOR_CHANNEL) {
			len = sdata->tf->read_multiple_byte(&sdata->tb,
					sdata->dev, addr[0],
					ST_SENSORS_BYTE_FOR_CHANNEL*n,
					buf, sdata->multiread_bit);
		} else {
			u8 rx_array[ST_SENSORS_BYTE_FOR_CHANNEL*
				    ST_SENSORS_NUMBER_DATA_CHANNELS];
			len = sdata->tf->read_multiple_byte(&sdata->tb,
				sdata->dev, addr[0],
				ST_SENSORS_BYTE_FOR_CHANNEL*
				ST_SENSORS_NUMBER_DATA_CHANNELS,
				rx_array, sdata->multiread_bit);
			if (len < 0)
				goto read_data_channels_error;

			for (i = 0; i < n * ST_SENSORS_NUMBER_DATA_CHANNELS;
									i++) {
				if (i < n)
					buf[i] = rx_array[i];
				else
					buf[i] = rx_array[n + i];
			}
			len = ST_SENSORS_BYTE_FOR_CHANNEL*n;
		}
		break;
	case 3:
		len = sdata->tf->read_multiple_byte(&sdata->tb, sdata->dev,
			addr[0], ST_SENSORS_BYTE_FOR_CHANNEL*
			ST_SENSORS_NUMBER_DATA_CHANNELS,
			buf, sdata->multiread_bit);
		break;
	default:
		len = -EINVAL;
		goto read_data_channels_error;
	}
	if (len != ST_SENSORS_BYTE_FOR_CHANNEL*n) {
		len = -EIO;
		goto read_data_channels_error;
	}

read_data_channels_error:
	return len;
}
EXPORT_SYMBOL(st_sensors_get_buffer_element);
=======
static int st_sensors_get_buffer_element(struct iio_dev *indio_dev, u8 *buf)
{
	int i;
	struct st_sensor_data *sdata = iio_priv(indio_dev);
	unsigned int num_data_channels = sdata->num_data_channels;

	for_each_set_bit(i, indio_dev->active_scan_mask, num_data_channels) {
		const struct iio_chan_spec *channel = &indio_dev->channels[i];
		unsigned int bytes_to_read =
			DIV_ROUND_UP(channel->scan_type.realbits +
				     channel->scan_type.shift, 8);
		unsigned int storage_bytes =
			channel->scan_type.storagebits >> 3;

		buf = PTR_ALIGN(buf, storage_bytes);
		if (sdata->tf->read_multiple_byte(&sdata->tb, sdata->dev,
						  channel->address,
						  bytes_to_read, buf,
						  sdata->multiread_bit) <
		    bytes_to_read)
			return -EIO;

		/* Advance the buffer pointer */
		buf += storage_bytes;
	}

	return 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

irqreturn_t st_sensors_trigger_handler(int irq, void *p)
{
	int len;
	struct iio_poll_func *pf = p;
	struct iio_dev *indio_dev = pf->indio_dev;
	struct st_sensor_data *sdata = iio_priv(indio_dev);
<<<<<<< HEAD
=======
	s64 timestamp;

	/*
	 * If we do timetamping here, do it before reading the values, because
	 * once we've read the values, new interrupts can occur (when using
	 * the hardware trigger) and the hw_timestamp may get updated.
	 * By storing it in a local variable first, we are safe.
	 */
	if (iio_trigger_using_own(indio_dev))
		timestamp = sdata->hw_timestamp;
	else
		timestamp = iio_get_time_ns(indio_dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	len = st_sensors_get_buffer_element(indio_dev, sdata->buffer_data);
	if (len < 0)
		goto st_sensors_get_buffer_element_error;

<<<<<<< HEAD
	if (indio_dev->scan_timestamp)
		*(s64 *)((u8 *)sdata->buffer_data +
				ALIGN(len, sizeof(s64))) = pf->timestamp;

	iio_push_to_buffers(indio_dev, sdata->buffer_data);
=======
	iio_push_to_buffers_with_timestamp(indio_dev, sdata->buffer_data,
					   timestamp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

st_sensors_get_buffer_element_error:
	iio_trigger_notify_done(indio_dev->trig);

	return IRQ_HANDLED;
}
EXPORT_SYMBOL(st_sensors_trigger_handler);

MODULE_AUTHOR("Denis Ciocca <denis.ciocca@st.com>");
MODULE_DESCRIPTION("STMicroelectronics ST-sensors buffer");
MODULE_LICENSE("GPL v2");
