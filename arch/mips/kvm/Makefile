# Makefile for KVM support for MIPS
#

<<<<<<< HEAD
common-objs = $(addprefix ../../../virt/kvm/, kvm_main.o coalesced_mmio.o)

EXTRA_CFLAGS += -Ivirt/kvm -Iarch/mips/kvm

kvm-objs := $(common-objs) kvm_mips.o kvm_mips_emul.o kvm_locore.o \
	    kvm_mips_int.o kvm_mips_stats.o kvm_mips_commpage.o \
	    kvm_mips_dyntrans.o kvm_trap_emul.o

obj-$(CONFIG_KVM)	+= kvm.o
obj-y			+= kvm_cb.o kvm_tlb.o
=======
common-objs-y = $(addprefix ../../../virt/kvm/, kvm_main.o coalesced_mmio.o)

EXTRA_CFLAGS += -Ivirt/kvm -Iarch/mips/kvm

common-objs-$(CONFIG_CPU_HAS_MSA) += msa.o

kvm-objs := $(common-objs-y) mips.o emulate.o entry.o \
	    interrupt.o stats.o commpage.o \
	    dyntrans.o trap_emul.o fpu.o
kvm-objs += mmu.o

obj-$(CONFIG_KVM)	+= kvm.o
obj-y			+= callback.o tlb.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
