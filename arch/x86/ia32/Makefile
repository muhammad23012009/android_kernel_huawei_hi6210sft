#
# Makefile for the ia32 kernel emulation subsystem.
#

<<<<<<< HEAD
obj-$(CONFIG_IA32_EMULATION) := ia32entry.o sys_ia32.o ia32_signal.o
obj-$(CONFIG_IA32_EMULATION) += nosyscall.o syscall_ia32.o
=======
obj-$(CONFIG_IA32_EMULATION) := sys_ia32.o ia32_signal.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

obj-$(CONFIG_IA32_AOUT) += ia32_aout.o

audit-class-$(CONFIG_AUDIT) := audit.o
obj-$(CONFIG_IA32_EMULATION) += $(audit-class-y)
