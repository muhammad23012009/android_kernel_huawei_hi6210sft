#
# Makefile for the linux pstorefs routines.
#

<<<<<<< HEAD
obj-y += pstore.o

pstore-objs += inode.o platform.o
obj-$(CONFIG_PSTORE_FTRACE)	+= ftrace.o

obj-$(CONFIG_PSTORE_PMSG)	+= pmsg.o
=======
obj-$(CONFIG_PSTORE) += pstore.o

pstore-objs += inode.o platform.o
pstore-$(CONFIG_PSTORE_FTRACE)	+= ftrace.o

pstore-$(CONFIG_PSTORE_PMSG)	+= pmsg.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

ramoops-objs += ram.o ram_core.o
obj-$(CONFIG_PSTORE_RAM)	+= ramoops.o
