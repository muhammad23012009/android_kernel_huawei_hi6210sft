/*
 * STMicroelectronics accelerometers driver
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
#include <linux/spi/spi.h>
#include <linux/iio/iio.h>

#include <linux/iio/common/st_sensors.h>
#include <linux/iio/common/st_sensors_spi.h>
#include "st_accel.h"

static int st_accel_spi_probe(struct spi_device *spi)
{
	struct iio_dev *indio_dev;
	struct st_sensor_data *adata;
	int err;

<<<<<<< HEAD
	indio_dev = iio_device_alloc(sizeof(*adata));
	if (indio_dev == NULL) {
		err = -ENOMEM;
		goto iio_device_alloc_error;
	}

	adata = iio_priv(indio_dev);
	adata->dev = &spi->dev;
=======
	indio_dev = devm_iio_device_alloc(&spi->dev, sizeof(*adata));
	if (!indio_dev)
		return -ENOMEM;

	adata = iio_priv(indio_dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	st_sensors_spi_configure(indio_dev, spi, adata);

	err = st_accel_common_probe(indio_dev);
	if (err < 0)
<<<<<<< HEAD
		goto st_accel_common_probe_error;

	return 0;

st_accel_common_probe_error:
	iio_device_free(indio_dev);
iio_device_alloc_error:
	return err;
=======
		return err;

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int st_accel_spi_remove(struct spi_device *spi)
{
	st_accel_common_remove(spi_get_drvdata(spi));

	return 0;
}

static const struct spi_device_id st_accel_id_table[] = {
	{ LSM303DLH_ACCEL_DEV_NAME },
	{ LSM303DLHC_ACCEL_DEV_NAME },
	{ LIS3DH_ACCEL_DEV_NAME },
	{ LSM330D_ACCEL_DEV_NAME },
	{ LSM330DL_ACCEL_DEV_NAME },
	{ LSM330DLC_ACCEL_DEV_NAME },
	{ LIS331DLH_ACCEL_DEV_NAME },
	{ LSM303DL_ACCEL_DEV_NAME },
	{ LSM303DLM_ACCEL_DEV_NAME },
	{ LSM330_ACCEL_DEV_NAME },
<<<<<<< HEAD
=======
	{ LSM303AGR_ACCEL_DEV_NAME },
	{ LIS2DH12_ACCEL_DEV_NAME },
	{ LIS3L02DQ_ACCEL_DEV_NAME },
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{},
};
MODULE_DEVICE_TABLE(spi, st_accel_id_table);

static struct spi_driver st_accel_driver = {
	.driver = {
<<<<<<< HEAD
		.owner = THIS_MODULE,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.name = "st-accel-spi",
	},
	.probe = st_accel_spi_probe,
	.remove = st_accel_spi_remove,
	.id_table = st_accel_id_table,
};
module_spi_driver(st_accel_driver);

MODULE_AUTHOR("Denis Ciocca <denis.ciocca@st.com>");
MODULE_DESCRIPTION("STMicroelectronics accelerometers spi driver");
MODULE_LICENSE("GPL v2");
