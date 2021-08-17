#
# Makefile for Inertial Measurement Units
#

<<<<<<< HEAD
=======
# When adding new entries keep the list in alphabetical order
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
adis16400-y             := adis16400_core.o
adis16400-$(CONFIG_IIO_BUFFER) += adis16400_buffer.o
obj-$(CONFIG_ADIS16400) += adis16400.o
obj-$(CONFIG_ADIS16480) += adis16480.o

adis_lib-y += adis.o
adis_lib-$(CONFIG_IIO_ADIS_LIB_BUFFER) += adis_trigger.o
adis_lib-$(CONFIG_IIO_ADIS_LIB_BUFFER) += adis_buffer.o
obj-$(CONFIG_IIO_ADIS_LIB) += adis_lib.o

<<<<<<< HEAD
obj-y += inv_mpu6050/
=======
obj-y += bmi160/
obj-y += inv_mpu6050/

obj-$(CONFIG_KMX61) += kmx61.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
