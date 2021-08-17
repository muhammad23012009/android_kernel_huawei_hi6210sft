# Makefile for kernel virtual machines on s390
#
# Copyright IBM Corp. 2008
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License (version 2 only)
# as published by the Free Software Foundation.

KVM := ../../../virt/kvm
<<<<<<< HEAD
common-objs = $(KVM)/kvm_main.o $(KVM)/eventfd.o

ccflags-y := -Ivirt/kvm -Iarch/s390/kvm

kvm-objs := $(common-objs) kvm-s390.o intercept.o interrupt.o priv.o sigp.o diag.o
=======
common-objs = $(KVM)/kvm_main.o $(KVM)/eventfd.o  $(KVM)/async_pf.o $(KVM)/irqchip.o $(KVM)/vfio.o

ccflags-y := -Ivirt/kvm -Iarch/s390/kvm

kvm-objs := $(common-objs) kvm-s390.o intercept.o interrupt.o priv.o sigp.o
kvm-objs += diag.o gaccess.o guestdbg.o sthyi.o vsie.o

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_KVM) += kvm.o
