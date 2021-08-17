#
# Makefile for TILE-specific library files..
#

lib-y = cacheflush.o checksum.o cpumask.o delay.o uaccess.o \
	memmove.o memcpy_$(BITS).o memchr_$(BITS).o memset_$(BITS).o \
<<<<<<< HEAD
	strchr_$(BITS).o strlen_$(BITS).o

ifeq ($(CONFIG_TILEGX),y)
CFLAGS_REMOVE_memcpy_user_64.o = -fno-omit-frame-pointer
lib-y += memcpy_user_64.o
else
lib-y += atomic_32.o atomic_asm_32.o memcpy_tile64.o
endif

lib-$(CONFIG_SMP) += spinlock_$(BITS).o usercopy_$(BITS).o

obj-$(CONFIG_MODULES) += exports.o
=======
	strchr_$(BITS).o strlen_$(BITS).o strnlen_$(BITS).o

lib-$(CONFIG_TILEGX) += memcpy_user_64.o
lib-$(CONFIG_TILEPRO) += atomic_32.o atomic_asm_32.o
lib-$(CONFIG_SMP) += spinlock_$(BITS).o usercopy_$(BITS).o

obj-$(CONFIG_MODULES) += exports.o

# The finv_buffer_remote() and copy_{to,from}_user() routines can't
# have -pg added, since they both rely on being leaf functions.
CFLAGS_REMOVE_cacheflush.o = -pg
CFLAGS_REMOVE_memcpy_user_64.o = -pg
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
