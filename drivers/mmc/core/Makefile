#
# Makefile for the kernel mmc core.
#

obj-$(CONFIG_MMC)		+= mmc_core.o
mmc_core-y			:= core.o bus.o host.o \
				   mmc.o mmc_ops.o sd.o sd_ops.o \
				   sdio.o sdio_ops.o sdio_bus.o \
				   sdio_cis.o sdio_io.o sdio_irq.o \
				   quirks.o slot-gpio.o
<<<<<<< HEAD

mmc_core-$(CONFIG_MMC_PASSWORDS) += lock.o sysfs.o
=======
mmc_core-$(CONFIG_OF)		+= pwrseq.o
obj-$(CONFIG_PWRSEQ_SIMPLE)	+= pwrseq_simple.o
obj-$(CONFIG_PWRSEQ_EMMC)	+= pwrseq_emmc.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
mmc_core-$(CONFIG_DEBUG_FS)	+= debugfs.o
