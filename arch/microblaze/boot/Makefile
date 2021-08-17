#
# arch/microblaze/boot/Makefile
#

<<<<<<< HEAD
targets := linux.bin linux.bin.gz simpleImage.%
=======
targets := linux.bin linux.bin.gz linux.bin.ub simpleImage.%
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

OBJCOPYFLAGS := -R .note -R .comment -R .note.gnu.build-id -O binary

$(obj)/linux.bin: vmlinux FORCE
	$(call if_changed,objcopy)
<<<<<<< HEAD
	$(call if_changed,uimage)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

$(obj)/linux.bin.gz: $(obj)/linux.bin FORCE
	$(call if_changed,gzip)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'
=======

$(obj)/linux.bin.ub: $(obj)/linux.bin FORCE
	$(call if_changed,uimage)

$(obj)/linux.bin.gz: $(obj)/linux.bin FORCE
	$(call if_changed,gzip)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

quiet_cmd_cp = CP      $< $@$2
	cmd_cp = cat $< >$@$2 || (rm -f $@ && echo false)

<<<<<<< HEAD
quiet_cmd_strip = STRIP   $@
	cmd_strip = $(STRIP) -K microblaze_start -K _end -K __log_buf \
				-K _fdt_start vmlinux -o $@

UIMAGE_IN = $@
UIMAGE_OUT = $@.ub
UIMAGE_LOADADDR = $(CONFIG_KERNEL_BASE_ADDR)
=======
quiet_cmd_strip = STRIP   $< $@$2
	cmd_strip = $(STRIP) -K microblaze_start -K _end -K __log_buf \
				-K _fdt_start $< -o $@$2

UIMAGE_LOADADDR = $(CONFIG_KERNEL_BASE_ADDR)
UIMAGE_IN = $@
UIMAGE_OUT = $@.ub
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

$(obj)/simpleImage.%: vmlinux FORCE
	$(call if_changed,cp,.unstrip)
	$(call if_changed,objcopy)
	$(call if_changed,uimage)
<<<<<<< HEAD
	$(call if_changed,strip)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'


clean-files += simpleImage.*.unstrip linux.bin.ub
=======
	$(call if_changed,strip,.strip)

clean-files += simpleImage.*.unstrip linux.bin.ub dts/*.dtb
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
