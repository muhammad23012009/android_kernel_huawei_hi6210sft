#
# Makefile for the linux kernel.
#

CPPFLAGS_vmlinux.lds	:= -DTEXT_OFFSET=$(TEXT_OFFSET)
AFLAGS_head.o		:= -DTEXT_OFFSET=$(TEXT_OFFSET)
<<<<<<< HEAD
CFLAGS_efi-stub.o 	:= -DTEXT_OFFSET=$(TEXT_OFFSET) \
			   -I$(src)/../../../scripts/dtc/libfdt
=======
CFLAGS_armv8_deprecated.o := -I$(src)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

CFLAGS_REMOVE_ftrace.o = -pg
CFLAGS_REMOVE_insn.o = -pg
CFLAGS_REMOVE_return_address.o = -pg

<<<<<<< HEAD
# Object file lists.
arm64-obj-y		:= cputable.o debug-monitors.o entry.o irq.o fpsimd.o	\
			   entry-fpsimd.o process.o ptrace.o setup.o signal.o	\
			   sys.o stacktrace.o time.o traps.o io.o vdso.o	\
			   hyp-stub.o psci.o cpu_ops.o insn.o return_address.o \
			   opcodes.o

arm64-obj-$(CONFIG_COMPAT)		+= sys32.o kuser32.o signal32.o	\
					   sys_compat.o
arm64-obj-$(CONFIG_FUNCTION_TRACER)	+= ftrace.o entry-ftrace.o
arm64-obj-$(CONFIG_MODULES)		+= arm64ksyms.o module.o
arm64-obj-$(CONFIG_SMP)			+= smp.o smp_spin_table.o
arm64-obj-$(CONFIG_SMP)			+= topology.o
arm64-obj-$(CONFIG_PERF_EVENTS)		+= perf_regs.o
arm64-obj-$(CONFIG_HW_PERF_EVENTS)	+= perf_event.o
arm64-obj-$(CONFIG_HAVE_HW_BREAKPOINT)	+= hw_breakpoint.o
arm64-obj-$(CONFIG_EARLY_PRINTK)	+= early_printk.o
arm64-obj-$(CONFIG_ARM64_CPU_SUSPEND)	+= sleep.o suspend.o

arm64-obj-$(CONFIG_ARM_TIMER_SP804)	+= timer-sp.o

arm64-obj-$(CONFIG_JUMP_LABEL)		+= jump_label.o
arm64-obj-$(CONFIG_CPU_IDLE)		+= cpuidle.o
arm64-obj-$(CONFIG_KGDB)		+= kgdb.o
arm64-obj-$(CONFIG_EFI)			+= efi.o efi-stub.o efi-entry.o

obj-$(CONFIG_SWP_EMULATE)	+= swp_emulate.o

obj-y					+= $(arm64-obj-y) vdso/
obj-m					+= $(arm64-obj-m)
head-y					:= head.o
extra-y					:= $(head-y) vmlinux.lds

# vDSO - this must be built first to generate the symbol offsets
$(call objectify,$(arm64-obj-y)): $(obj)/vdso/vdso-offsets.h
$(obj)/vdso/vdso-offsets.h: $(obj)/vdso
=======
CFLAGS_setup.o = -DUTS_MACHINE='"$(UTS_MACHINE)"'

# Object file lists.
arm64-obj-y		:= debug-monitors.o entry.o irq.o fpsimd.o		\
			   entry-fpsimd.o process.o ptrace.o setup.o signal.o	\
			   sys.o stacktrace.o time.o traps.o io.o vdso.o	\
			   hyp-stub.o psci.o cpu_ops.o insn.o	\
			   return_address.o cpuinfo.o cpu_errata.o		\
			   cpufeature.o alternative.o cacheinfo.o		\
			   smp.o smp_spin_table.o topology.o smccc-call.o

extra-$(CONFIG_EFI)			:= efi-entry.o

OBJCOPYFLAGS := --prefix-symbols=__efistub_
$(obj)/%.stub.o: $(obj)/%.o FORCE
	$(call if_changed,objcopy)

arm64-obj-$(CONFIG_COMPAT)		+= sys32.o kuser32.o signal32.o 	\
					   sys_compat.o entry32.o
arm64-obj-$(CONFIG_FUNCTION_TRACER)	+= ftrace.o entry-ftrace.o
arm64-obj-$(CONFIG_MODULES)		+= arm64ksyms.o module.o
arm64-obj-$(CONFIG_ARM64_MODULE_PLTS)	+= module-plts.o
arm64-obj-$(CONFIG_PERF_EVENTS)		+= perf_regs.o perf_callchain.o
arm64-obj-$(CONFIG_HW_PERF_EVENTS)	+= perf_event.o
arm64-obj-$(CONFIG_HAVE_HW_BREAKPOINT)	+= hw_breakpoint.o
arm64-obj-$(CONFIG_CPU_PM)		+= sleep.o suspend.o
arm64-obj-$(CONFIG_CPU_IDLE)		+= cpuidle.o
arm64-obj-$(CONFIG_JUMP_LABEL)		+= jump_label.o
arm64-obj-$(CONFIG_KGDB)		+= kgdb.o
arm64-obj-$(CONFIG_EFI)			+= efi.o efi-entry.stub.o
arm64-obj-$(CONFIG_PCI)			+= pci.o
arm64-obj-$(CONFIG_ARMV8_DEPRECATED)	+= armv8_deprecated.o
arm64-obj-$(CONFIG_ACPI)		+= acpi.o
arm64-obj-$(CONFIG_ACPI_NUMA)		+= acpi_numa.o
arm64-obj-$(CONFIG_ARM64_ACPI_PARKING_PROTOCOL)	+= acpi_parking_protocol.o
arm64-obj-$(CONFIG_PARAVIRT)		+= paravirt.o
arm64-obj-$(CONFIG_RANDOMIZE_BASE)	+= kaslr.o
arm64-obj-$(CONFIG_HIBERNATION)		+= hibernate.o hibernate-asm.o
arm64-obj-$(CONFIG_KEXEC)		+= machine_kexec.o relocate_kernel.o	\
					   cpu-reset.o
arm64-obj-$(CONFIG_ARM64_SSBD)		+= ssbd.o

ifeq ($(CONFIG_KVM),y)
arm64-obj-$(CONFIG_HARDEN_BRANCH_PREDICTOR)	+= bpi.o
endif

obj-y					+= $(arm64-obj-y) vdso/ probes/
obj-m					+= $(arm64-obj-m)
head-y					:= head.o
extra-y					+= $(head-y) vmlinux.lds
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
