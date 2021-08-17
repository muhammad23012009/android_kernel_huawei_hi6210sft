#
# Makefile for code common across different PXA processor families
#
<<<<<<< HEAD

obj-y	:= dma.o
=======
ccflags-$(CONFIG_ARCH_MMP) := -I$(srctree)/$(src)/include
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

obj-$(CONFIG_PXA3xx)		+= mfp.o
obj-$(CONFIG_ARCH_MMP)		+= mfp.o

obj-$(CONFIG_PXA_SSP)		+= ssp.o
