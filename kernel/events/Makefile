ifdef CONFIG_FUNCTION_TRACER
<<<<<<< HEAD
CFLAGS_REMOVE_core.o = -pg
=======
CFLAGS_REMOVE_core.o = $(CC_FLAGS_FTRACE)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
endif

obj-y := core.o ring_buffer.o callchain.o

obj-$(CONFIG_HAVE_HW_BREAKPOINT) += hw_breakpoint.o
obj-$(CONFIG_UPROBES) += uprobes.o

