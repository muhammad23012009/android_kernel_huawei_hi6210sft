#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

<<<<<<< HEAD
ifdef CONFIG_INITIALIZE_XTENSA_MMU_INSIDE_VMLINUX
UIMAGE_LOADADDR = 0x00003000
else
UIMAGE_LOADADDR = 0xd0003000
endif
=======
UIMAGE_LOADADDR = $(CONFIG_KERNEL_LOAD_ADDRESS)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
UIMAGE_COMPRESSION = gzip

$(obj)/../uImage: vmlinux.bin.gz FORCE
	$(call if_changed,uimage)
	$(Q)$(kecho) '  Kernel: $@ is ready'

zImage: $(obj)/../uImage
