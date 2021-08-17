#
# Makefile
#

ifdef CONFIG_FUNCTION_TRACER
# Do not trace early boot code and low level code
CFLAGS_REMOVE_timer.o = -pg
CFLAGS_REMOVE_intc.o = -pg
CFLAGS_REMOVE_early_printk.o = -pg
<<<<<<< HEAD
CFLAGS_REMOVE_selfmod.o = -pg
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
CFLAGS_REMOVE_heartbeat.o = -pg
CFLAGS_REMOVE_ftrace.o = -pg
CFLAGS_REMOVE_process.o = -pg
endif

extra-y := head.o vmlinux.lds

obj-y += dma.o exceptions.o \
	hw_exception_handler.o intc.o irq.o \
<<<<<<< HEAD
	process.o prom.o prom_parse.o ptrace.o \
=======
	platform.o process.o prom.o ptrace.o \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	reset.o setup.o signal.o sys_microblaze.o timer.o traps.o unwind.o

obj-y += cpu/

obj-$(CONFIG_EARLY_PRINTK)	+= early_printk.o
<<<<<<< HEAD
obj-$(CONFIG_SELFMOD)		+= selfmod.o
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
obj-$(CONFIG_HEART_BEAT)	+= heartbeat.o
obj-$(CONFIG_MODULES)		+= microblaze_ksyms.o module.o
obj-$(CONFIG_MMU)		+= misc.o
obj-$(CONFIG_STACKTRACE)	+= stacktrace.o
obj-$(CONFIG_FUNCTION_TRACER)	+= ftrace.o mcount.o
obj-$(CONFIG_KGDB)		+= kgdb.o

obj-y	+= entry$(MMU).o
