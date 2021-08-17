#
# Makefile for RapidIO devices
#

<<<<<<< HEAD
obj-$(CONFIG_RAPIDIO_TSI721)	+= tsi721.o
ifeq ($(CONFIG_RAPIDIO_DMA_ENGINE),y)
obj-$(CONFIG_RAPIDIO_TSI721)	+= tsi721_dma.o
endif
=======
obj-$(CONFIG_RAPIDIO_TSI721)	+= tsi721_mport.o
tsi721_mport-y			:= tsi721.o
tsi721_mport-$(CONFIG_RAPIDIO_DMA_ENGINE) += tsi721_dma.o
obj-$(CONFIG_RAPIDIO_MPORT_CDEV) += rio_mport_cdev.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
