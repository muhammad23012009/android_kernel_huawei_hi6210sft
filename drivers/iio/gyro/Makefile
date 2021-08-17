#
# Makefile for industrial I/O gyroscope sensor drivers
#

<<<<<<< HEAD
obj-$(CONFIG_ADIS16080) += adis16080.o
obj-$(CONFIG_ADIS16136) += adis16136.o
obj-$(CONFIG_ADXRS450) += adxrs450.o
=======
# When adding new entries keep the list in alphabetical order
obj-$(CONFIG_ADIS16080) += adis16080.o
obj-$(CONFIG_ADIS16130) += adis16130.o
obj-$(CONFIG_ADIS16136) += adis16136.o
obj-$(CONFIG_ADIS16260) += adis16260.o
obj-$(CONFIG_ADXRS450) += adxrs450.o
obj-$(CONFIG_BMG160) += bmg160_core.o
obj-$(CONFIG_BMG160_I2C) += bmg160_i2c.o
obj-$(CONFIG_BMG160_SPI) += bmg160_spi.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

obj-$(CONFIG_HID_SENSOR_GYRO_3D) += hid-sensor-gyro-3d.o

itg3200-y               := itg3200_core.o
itg3200-$(CONFIG_IIO_BUFFER) += itg3200_buffer.o
obj-$(CONFIG_ITG3200)   += itg3200.o

<<<<<<< HEAD
=======
obj-$(CONFIG_IIO_SSP_SENSORS_COMMONS) += ssp_gyro_sensor.o

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_IIO_ST_GYRO_3AXIS) += st_gyro.o
st_gyro-y := st_gyro_core.o
st_gyro-$(CONFIG_IIO_BUFFER) += st_gyro_buffer.o

obj-$(CONFIG_IIO_ST_GYRO_I2C_3AXIS) += st_gyro_i2c.o
obj-$(CONFIG_IIO_ST_GYRO_SPI_3AXIS) += st_gyro_spi.o
