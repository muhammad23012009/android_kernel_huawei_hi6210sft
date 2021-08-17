<<<<<<< HEAD
# $Id: Makefile,v 1.6 2004/12/13 12:21:51 starvik Exp $
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#
# Makefile for the linux kernel.
#

extra-y	:= head.o


obj-y   := entry.o traps.o shadows.o debugport.o irq.o \
	   process.o setup.o signal.o traps.o time.o ptrace.o \
	   dma.o io_interface_mux.o

obj-$(CONFIG_ETRAX_KGDB) += kgdb.o
obj-$(CONFIG_ETRAX_FAST_TIMER) += fasttimer.o
obj-$(CONFIG_MODULES)    += crisksyms.o

clean:

