#
# Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.

# Pass UTS_MACHINE for user_regset definition
CFLAGS_ptrace.o		+= -DUTS_MACHINE='"$(UTS_MACHINE)"'

<<<<<<< HEAD
obj-y	:= arcksyms.o setup.o irq.o time.o reset.o ptrace.o entry.o process.o
obj-y	+= signal.o traps.o sys.o troubleshoot.o stacktrace.o disasm.o clk.o
obj-y	+= devtree.o

obj-$(CONFIG_MODULES)			+= arcksyms.o module.o
obj-$(CONFIG_SMP) 			+= smp.o
obj-$(CONFIG_ARC_DW2_UNWIND)		+= unwind.o
obj-$(CONFIG_KPROBES)      		+= kprobes.o
obj-$(CONFIG_ARC_MISALIGN_ACCESS) 	+= unaligned.o
obj-$(CONFIG_KGDB)			+= kgdb.o
obj-$(CONFIG_ARC_METAWARE_HLINK)	+= arc_hostlink.o
=======
obj-y	:= arcksyms.o setup.o irq.o time.o reset.o ptrace.o process.o devtree.o
obj-y	+= signal.o traps.o sys.o troubleshoot.o stacktrace.o disasm.o
obj-$(CONFIG_ISA_ARCOMPACT)		+= entry-compact.o intc-compact.o
obj-$(CONFIG_ISA_ARCV2)			+= entry-arcv2.o intc-arcv2.o
obj-$(CONFIG_PCI)  			+= pcibios.o

obj-$(CONFIG_MODULES)			+= arcksyms.o module.o
obj-$(CONFIG_SMP) 			+= smp.o
obj-$(CONFIG_ARC_MCIP)			+= mcip.o
obj-$(CONFIG_ARC_DW2_UNWIND)		+= unwind.o
obj-$(CONFIG_KPROBES)      		+= kprobes.o
obj-$(CONFIG_ARC_EMUL_UNALIGNED) 	+= unaligned.o
obj-$(CONFIG_KGDB)			+= kgdb.o
obj-$(CONFIG_ARC_METAWARE_HLINK)	+= arc_hostlink.o
obj-$(CONFIG_PERF_EVENTS)		+= perf_event.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

obj-$(CONFIG_ARC_FPU_SAVE_RESTORE)	+= fpu.o
CFLAGS_fpu.o   += -mdpfp

ifdef CONFIG_ARC_DW2_UNWIND
CFLAGS_ctx_sw.o += -fno-omit-frame-pointer
obj-y += ctx_sw.o
else
obj-y += ctx_sw_asm.o
endif

extra-y := vmlinux.lds head.o
