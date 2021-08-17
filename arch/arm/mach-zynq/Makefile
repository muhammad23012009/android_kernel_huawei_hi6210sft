#
# Makefile for the linux kernel.
#

# Common support
<<<<<<< HEAD
obj-y				:= common.o slcr.o
CFLAGS_REMOVE_hotplug.o		=-march=armv6k
CFLAGS_hotplug.o 		=-Wa,-march=armv7-a -mcpu=cortex-a9
obj-$(CONFIG_HOTPLUG_CPU)	+= hotplug.o
=======
obj-y				:= common.o slcr.o pm.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_SMP)		+= headsmp.o platsmp.o
