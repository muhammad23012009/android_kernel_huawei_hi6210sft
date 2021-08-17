#
<<<<<<< HEAD
# arch/microblaze/boot/Makefile
#
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

obj-y += linked_dtb.o

# Ensure system.dtb exists
$(obj)/linked_dtb.o: $(obj)/system.dtb

# Generate system.dtb from $(DTB).dtb
ifneq ($(DTB),system)
$(obj)/system.dtb: $(obj)/$(DTB).dtb
	$(call if_changed,cp)
endif

quiet_cmd_cp = CP      $< $@$2
	cmd_cp = cat $< >$@$2 || (rm -f $@ && echo false)

# Rule to build device tree blobs
DTC_FLAGS := -p 1024
<<<<<<< HEAD

clean-files += *.dtb
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
