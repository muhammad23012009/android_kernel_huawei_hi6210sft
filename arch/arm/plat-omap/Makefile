#
# Makefile for the linux kernel.
#

ccflags-$(CONFIG_ARCH_MULTIPLATFORM) := -I$(srctree)/arch/arm/plat-omap/include

# Common support
obj-y := sram.o dma.o counter_32k.o
<<<<<<< HEAD
obj-m :=
obj-n :=
obj-  :=
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

# omap_device support (OMAP2+ only at the moment)

obj-$(CONFIG_OMAP_DM_TIMER) += dmtimer.o
obj-$(CONFIG_OMAP_DEBUG_LEDS) += debug-leds.o
i2c-omap-$(CONFIG_I2C_OMAP) := i2c.o
obj-y += $(i2c-omap-m) $(i2c-omap-y)

<<<<<<< HEAD
# OMAP mailbox framework
obj-$(CONFIG_OMAP_MBOX_FWK) += mailbox.o

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
