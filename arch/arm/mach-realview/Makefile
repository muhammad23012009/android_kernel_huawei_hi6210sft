#
# Makefile for the linux kernel.
#
<<<<<<< HEAD

obj-y					:= core.o
obj-$(CONFIG_MACH_REALVIEW_EB)		+= realview_eb.o
obj-$(CONFIG_MACH_REALVIEW_PB11MP)	+= realview_pb11mp.o
obj-$(CONFIG_MACH_REALVIEW_PB1176)	+= realview_pb1176.o
obj-$(CONFIG_MACH_REALVIEW_PBA8)	+= realview_pba8.o
obj-$(CONFIG_MACH_REALVIEW_PBX)		+= realview_pbx.o
obj-$(CONFIG_SMP)			+= platsmp.o
=======
ccflags-$(CONFIG_ARCH_MULTIPLATFORM) := -I$(srctree)/arch/arm/plat-versatile/include

obj-y					+= realview-dt.o
obj-$(CONFIG_SMP)			+= platsmp-dt.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_HOTPLUG_CPU)		+= hotplug.o
