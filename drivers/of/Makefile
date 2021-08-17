<<<<<<< HEAD
obj-y = base.o
obj-$(CONFIG_OF_DYNAMIC) += dynamic.o
obj-$(CONFIG_OF_FLATTREE) += fdt.o
obj-$(CONFIG_OF_PROMTREE) += pdt.o
obj-$(CONFIG_OF_ADDRESS)  += address.o
obj-$(CONFIG_OF_IRQ)    += irq.o
obj-$(CONFIG_OF_DEVICE) += device.o platform.o
obj-$(CONFIG_OF_NET)	+= of_net.o
obj-$(CONFIG_OF_SELFTEST) += selftest.o
obj-$(CONFIG_OF_MDIO)	+= of_mdio.o
obj-$(CONFIG_OF_PCI)	+= of_pci.o
obj-$(CONFIG_OF_PCI_IRQ)  += of_pci_irq.o
obj-$(CONFIG_OF_MTD)	+= of_mtd.o
obj-$(CONFIG_OF_RESERVED_MEM) += of_reserved_mem.o
obj-$(CONFIG_OF_RESOLVE)  += resolver.o
obj-$(CONFIG_OF_OVERLAY) += overlay.o

CFLAGS_fdt.o = -I$(src)/../../scripts/dtc/libfdt
=======
obj-y = base.o device.o platform.o
obj-$(CONFIG_OF_DYNAMIC) += dynamic.o
obj-$(CONFIG_OF_FLATTREE) += fdt.o
obj-$(CONFIG_OF_EARLY_FLATTREE) += fdt_address.o
obj-$(CONFIG_OF_PROMTREE) += pdt.o
obj-$(CONFIG_OF_ADDRESS)  += address.o
obj-$(CONFIG_OF_IRQ)    += irq.o
obj-$(CONFIG_OF_NET)	+= of_net.o
obj-$(CONFIG_OF_UNITTEST) += unittest.o
obj-$(CONFIG_OF_MDIO)	+= of_mdio.o
obj-$(CONFIG_OF_PCI)	+= of_pci.o
obj-$(CONFIG_OF_PCI_IRQ)  += of_pci_irq.o
obj-$(CONFIG_OF_RESERVED_MEM) += of_reserved_mem.o
obj-$(CONFIG_OF_RESOLVE)  += resolver.o
obj-$(CONFIG_OF_OVERLAY) += overlay.o
obj-$(CONFIG_OF_NUMA) += of_numa.o

obj-$(CONFIG_OF_UNITTEST) += unittest-data/
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
