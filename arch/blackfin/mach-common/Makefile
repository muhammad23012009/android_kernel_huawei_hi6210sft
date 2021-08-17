#
# arch/blackfin/mach-common/Makefile
#

obj-y := \
	cache.o cache-c.o entry.o head.o \
	interrupt.o arch_checks.o ints-priority.o

obj-$(CONFIG_PM)          += pm.o
ifneq ($(CONFIG_BF60x),y)
obj-$(CONFIG_PM)	  += dpmc_modes.o
endif
<<<<<<< HEAD
=======
obj-$(CONFIG_SCB_PRIORITY)	+= scb-init.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_CPU_VOLTAGE) += dpmc.o
obj-$(CONFIG_SMP)         += smp.o
obj-$(CONFIG_BFIN_KERNEL_CLOCK) += clocks-init.o
