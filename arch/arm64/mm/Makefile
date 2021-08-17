obj-y				:= dma-mapping.o extable.o fault.o init.o \
				   cache.o copypage.o flush.o \
				   ioremap.o mmap.o pgd.o mmu.o \
<<<<<<< HEAD
				   context.o proc.o pageattr.o alignment.o

obj-$(CONFIG_HUGETLB_PAGE)	+= hugetlbpage.o
obj-$(CONFIG_ARM64_PTDUMP)	+= dump.o
=======
				   context.o proc.o pageattr.o
obj-$(CONFIG_HUGETLB_PAGE)	+= hugetlbpage.o
obj-$(CONFIG_ARM64_PTDUMP)	+= dump.o
obj-$(CONFIG_NUMA)		+= numa.o

obj-$(CONFIG_KASAN)		+= kasan_init.o
KASAN_SANITIZE_kasan_init.o	:= n
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
