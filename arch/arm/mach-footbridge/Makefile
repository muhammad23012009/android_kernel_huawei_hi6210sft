#
# Makefile for the linux kernel.
#

# Object file lists.

<<<<<<< HEAD
obj-y			:= common.o dc21285.o dma.o isa-irq.o
obj-m			:=
obj-n			:=
obj-			:=

=======
obj-y			:= common.o dma.o isa-irq.o

pci-y			+= dc21285.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
pci-$(CONFIG_ARCH_CATS) += cats-pci.o
pci-$(CONFIG_ARCH_EBSA285_HOST) += ebsa285-pci.o
pci-$(CONFIG_ARCH_NETWINDER) += netwinder-pci.o
pci-$(CONFIG_ARCH_PERSONAL_SERVER) += personal-pci.o

obj-$(CONFIG_ARCH_CATS) += cats-hw.o isa-timer.o
obj-$(CONFIG_ARCH_EBSA285) += ebsa285.o dc21285-timer.o
obj-$(CONFIG_ARCH_NETWINDER) += netwinder-hw.o isa-timer.o
obj-$(CONFIG_ARCH_PERSONAL_SERVER) += personal.o dc21285-timer.o

obj-$(CONFIG_PCI)	+=$(pci-y)

obj-$(CONFIG_ISA)	+= isa.o isa-rtc.o
