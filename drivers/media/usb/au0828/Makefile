au0828-objs	:= au0828-core.o au0828-i2c.o au0828-cards.o au0828-dvb.o

ifeq ($(CONFIG_VIDEO_AU0828_V4L2),y)
  au0828-objs   += au0828-video.o au0828-vbi.o
endif

<<<<<<< HEAD
=======
ifeq ($(CONFIG_VIDEO_AU0828_RC),y)
  au0828-objs   += au0828-input.o
endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_VIDEO_AU0828) += au0828.o

ccflags-y += -Idrivers/media/tuners
ccflags-y += -Idrivers/media/dvb-core
ccflags-y += -Idrivers/media/dvb-frontends

ccflags-y += $(extra-cflags-y) $(extra-cflags-m)
