#
# Makefile for the SuperH specific drivers.
#
<<<<<<< HEAD
obj-y	:= intc/

obj-$(CONFIG_HAVE_CLK)		+= clk/
obj-$(CONFIG_MAPLE)		+= maple/
obj-$(CONFIG_SUPERHYWAY)	+= superhyway/

obj-y				+= pm_runtime.o
=======
obj-$(CONFIG_SH_INTC)			+= intc/
ifneq ($(CONFIG_COMMON_CLK),y)
obj-$(CONFIG_HAVE_CLK)			+= clk/
endif
obj-$(CONFIG_MAPLE)			+= maple/
obj-$(CONFIG_SUPERHYWAY)		+= superhyway/

obj-y					+= pm_runtime.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
