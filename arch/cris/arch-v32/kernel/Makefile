#
# Makefile for the linux kernel.
#

extra-y	:= head.o


obj-y   := entry.o traps.o irq.o debugport.o \
	   process.o ptrace.o setup.o signal.o traps.o time.o \
	   cache.o cacheflush.o

<<<<<<< HEAD
obj-$(CONFIG_SMP) += smp.o
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_ETRAX_KGDB) += kgdb.o kgdb_asm.o
obj-$(CONFIG_ETRAX_FAST_TIMER) += fasttimer.o
obj-$(CONFIG_MODULES)    += crisksyms.o

clean:

