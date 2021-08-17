#
# Makefile - Intel Management Engine Interface (Intel MEI) Linux driver
<<<<<<< HEAD
# Copyright (c) 2010-2011, Intel Corporation.
=======
# Copyright (c) 2010-2014, Intel Corporation.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#
obj-$(CONFIG_INTEL_MEI) += mei.o
mei-objs := init.o
mei-objs += hbm.o
mei-objs += interrupt.o
mei-objs += client.o
mei-objs += main.o
mei-objs += amthif.o
<<<<<<< HEAD
mei-objs += wd.o
mei-objs += bus.o
mei-objs += nfc.o
=======
mei-objs += bus.o
mei-objs += bus-fixup.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
mei-$(CONFIG_DEBUG_FS) += debugfs.o

obj-$(CONFIG_INTEL_MEI_ME) += mei-me.o
mei-me-objs := pci-me.o
mei-me-objs += hw-me.o
<<<<<<< HEAD
=======

obj-$(CONFIG_INTEL_MEI_TXE) += mei-txe.o
mei-txe-objs := pci-txe.o
mei-txe-objs += hw-txe.o

mei-$(CONFIG_EVENT_TRACING) += mei-trace.o
CFLAGS_mei-trace.o = -I$(src)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
