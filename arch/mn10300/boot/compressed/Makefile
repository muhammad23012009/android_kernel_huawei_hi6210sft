#
# Create a compressed vmlinux image from the original vmlinux
#

targets		:= vmlinux vmlinux.bin vmlinux.bin.gz head.o misc.o piggy.o

LDFLAGS_vmlinux := -Ttext $(CONFIG_KERNEL_ZIMAGE_BASE_ADDRESS) -e startup_32

$(obj)/vmlinux: $(obj)/head.o $(obj)/misc.o $(obj)/piggy.o FORCE
	$(call if_changed,ld)
<<<<<<< HEAD
	@:
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)

LDFLAGS_piggy.o := -r --format binary --oformat elf32-am33lin -T

$(obj)/piggy.o: $(obj)/vmlinux.lds $(obj)/vmlinux.bin.gz FORCE
	$(call if_changed,ld)
