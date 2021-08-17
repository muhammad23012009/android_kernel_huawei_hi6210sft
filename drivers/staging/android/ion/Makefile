<<<<<<< HEAD
obj-$(CONFIG_ION) +=	ion.o ion_heap.o ion_page_pool.o ion_system_heap.o \
			ion_carveout_heap.o ion_chunk_heap.o ion_cma_heap.o \
			ion_cpudraw_heap.o
=======
obj-$(CONFIG_ION) +=	ion.o ion-ioctl.o ion_heap.o \
			ion_page_pool.o ion_system_heap.o \
			ion_carveout_heap.o ion_chunk_heap.o ion_cma_heap.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_ION_TEST) += ion_test.o
ifdef CONFIG_COMPAT
obj-$(CONFIG_ION) += compat_ion.o
endif

obj-$(CONFIG_ION_DUMMY) += ion_dummy_driver.o
obj-$(CONFIG_ION_TEGRA) += tegra/
<<<<<<< HEAD
obj-$(CONFIG_ION_HISI) += hisi/
=======
obj-$(CONFIG_ION_HISI) += hisilicon/
obj-$(CONFIG_ION_OF) += ion_of.o

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
