<<<<<<< HEAD
obj-$(CONFIG_SH_DMAE_BASE) += shdma-base.o
obj-$(CONFIG_SH_DMAE) += shdma.o
=======
#
# DMA Engine Helpers
#

obj-$(CONFIG_SH_DMAE_BASE) += shdma-base.o shdma-of.o

#
# DMA Controllers
#

shdma-y := shdmac.o
shdma-$(CONFIG_SH_DMAE_R8A73A4) += shdma-r8a73a4.o
shdma-objs := $(shdma-y)
obj-$(CONFIG_SH_DMAE) += shdma.o

obj-$(CONFIG_RCAR_DMAC) += rcar-dmac.o
obj-$(CONFIG_RENESAS_USB_DMAC) += usb-dmac.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_SUDMAC) += sudmac.o
