<<<<<<< HEAD
ccflags-$(CONFIG_ARCH_MULTIPLATFORM) := -I$(srctree)/$(src)/include \
	-I$(srctree)/arch/arm/plat-orion/include

AFLAGS_coherency_ll.o		:= -Wa,-march=armv7-a

obj-y				 += system-controller.o
obj-$(CONFIG_MACH_ARMADA_370_XP) += armada-370-xp.o
obj-$(CONFIG_ARCH_MVEBU)	 += coherency.o coherency_ll.o pmsu.o
obj-$(CONFIG_SMP)                += platsmp.o headsmp.o
obj-$(CONFIG_HOTPLUG_CPU)        += hotplug.o
=======
ccflags-$(CONFIG_ARCH_MULTIPLATFORM) := -I$(srctree)/arch/arm/plat-orion/include

AFLAGS_coherency_ll.o		:= -Wa,-march=armv7-a
CFLAGS_pmsu.o			:= -march=armv7-a

obj-$(CONFIG_MACH_MVEBU_ANY)	 += system-controller.o mvebu-soc-id.o

ifeq ($(CONFIG_MACH_MVEBU_V7),y)
obj-y				 += cpu-reset.o board-v7.o coherency.o coherency_ll.o pmsu.o pmsu_ll.o

obj-$(CONFIG_PM)		 += pm.o pm-board.o
obj-$(CONFIG_SMP)		 += platsmp.o headsmp.o platsmp-a9.o headsmp-a9.o
endif

obj-$(CONFIG_MACH_DOVE)		 += dove.o

ifeq ($(CONFIG_MACH_KIRKWOOD),y)
obj-y				 += kirkwood.o
obj-$(CONFIG_PM)		 += kirkwood-pm.o
endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
