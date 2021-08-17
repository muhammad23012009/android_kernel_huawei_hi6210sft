obj-y					:= highbank.o system.o smc.o

plus_sec := $(call as-instr,.arch_extension sec,+sec)
AFLAGS_smc.o				:=-Wa,-march=armv7-a$(plus_sec)

<<<<<<< HEAD
obj-$(CONFIG_SMP)			+= platsmp.o
obj-$(CONFIG_HOTPLUG_CPU)		+= hotplug.o
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_PM_SLEEP)			+= pm.o
