# Makefile for the Linux device tree

<<<<<<< HEAD
obj-y			:= core.o bus.o dd.o syscore.o \
			   driver.o class.o platform.o \
			   cpu.o firmware.o init.o map.o devres.o \
			   attribute_container.o transport_class.o \
			   topology.o
=======
obj-y			:= component.o core.o bus.o dd.o syscore.o \
			   driver.o class.o platform.o \
			   cpu.o firmware.o init.o map.o devres.o \
			   attribute_container.o transport_class.o \
			   topology.o container.o property.o cacheinfo.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_DEVTMPFS)	+= devtmpfs.o
obj-$(CONFIG_DMA_CMA) += dma-contiguous.o
obj-y			+= power/
obj-$(CONFIG_HAS_DMA)	+= dma-mapping.o
obj-$(CONFIG_HAVE_GENERIC_DMA_COHERENT) += dma-coherent.o
<<<<<<< HEAD
obj-$(CONFIG_DMA_SHARED_BUFFER) += dma-buf.o
obj-$(CONFIG_ISA)	+= isa.o
=======
obj-$(CONFIG_ISA_BUS_API)	+= isa.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_FW_LOADER)	+= firmware_class.o
obj-$(CONFIG_NUMA)	+= node.o
obj-$(CONFIG_MEMORY_HOTPLUG_SPARSE) += memory.o
ifeq ($(CONFIG_SYSFS),y)
obj-$(CONFIG_MODULES)	+= module.o
endif
obj-$(CONFIG_SYS_HYPERVISOR) += hypervisor.o
obj-$(CONFIG_REGMAP)	+= regmap/
obj-$(CONFIG_SOC_BUS) += soc.o
obj-$(CONFIG_PINCTRL) += pinctrl.o
<<<<<<< HEAD
=======
obj-$(CONFIG_DEV_COREDUMP) += devcoredump.o
obj-$(CONFIG_GENERIC_MSI_IRQ_DOMAIN) += platform-msi.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

ccflags-$(CONFIG_DEBUG_DRIVER) := -DDEBUG

