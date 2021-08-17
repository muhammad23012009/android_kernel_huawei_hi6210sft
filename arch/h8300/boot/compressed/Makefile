#
# linux/arch/sh/boot/compressed/Makefile
#
# create a compressed vmlinux image from the original vmlinux
#

targets		:= vmlinux vmlinux.bin vmlinux.bin.gz head.o misc.o piggy.o
<<<<<<< HEAD
asflags-y	:= -traditional
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

OBJECTS = $(obj)/head.o $(obj)/misc.o

#
# IMAGE_OFFSET is the load offset of the compression loader
# Assign dummy values if these 2 variables are not defined,
# in order to suppress error message.
#
CONFIG_MEMORY_START     ?= 0x00400000
<<<<<<< HEAD
CONFIG_BOOT_LINK_OFFSET ?= 0x00140000
IMAGE_OFFSET := $(shell printf "0x%08x" $$[$(CONFIG_MEMORY_START)+$(CONFIG_BOOT_LINK_OFFSET)])

LDFLAGS_vmlinux := -Ttext $(IMAGE_OFFSET) -estartup $(obj)/vmlinux.lds

$(obj)/vmlinux: $(OBJECTS) $(obj)/piggy.o FORCE
	$(call if_changed,ld)
	@:
=======
CONFIG_BOOT_LINK_OFFSET ?= 0x00280000
IMAGE_OFFSET := $(shell printf "0x%08x" $$(($(CONFIG_MEMORY_START)+$(CONFIG_BOOT_LINK_OFFSET))))

LIBGCC := $(shell $(CROSS-COMPILE)$(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)
LDFLAGS_vmlinux := -Ttext $(IMAGE_OFFSET) -estartup -T $(obj)/vmlinux.lds \
	--defsym output=$(CONFIG_MEMORY_START)

$(obj)/vmlinux: $(OBJECTS) $(obj)/piggy.o $(LIBGCC) FORCE
	$(call if_changed,ld)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

<<<<<<< HEAD
$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)

LDFLAGS_piggy.o := -r --format binary --oformat elf32-h8300 -T
OBJCOPYFLAGS := -O binary

$(obj)/piggy.o: $(obj)/vmlinux.scr $(obj)/vmlinux.bin.gz FORCE
	$(call if_changed,ld)
=======
suffix-$(CONFIG_KERNEL_GZIP)    := gzip
suffix-$(CONFIG_KERNEL_LZO)     := lzo

$(obj)/vmlinux.bin.$(suffix-y): $(obj)/vmlinux.bin FORCE
	$(call if_changed,$(suffix-y))

LDFLAGS_piggy.o := -r --format binary --oformat elf32-h8300-linux -T
OBJCOPYFLAGS := -O binary

$(obj)/piggy.o: $(obj)/vmlinux.scr $(obj)/vmlinux.bin.$(suffix-y) FORCE
	$(call if_changed,ld)

CFLAGS_misc.o = -O0
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
