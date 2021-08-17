#
# arch/x86/realmode/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
#
<<<<<<< HEAD
=======
KASAN_SANITIZE			:= n
OBJECT_FILES_NON_STANDARD	:= y
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

subdir- := rm

obj-y += init.o
obj-y += rmpiggy.o

$(obj)/rmpiggy.o: $(obj)/rm/realmode.bin

$(obj)/rm/realmode.bin: FORCE
	$(Q)$(MAKE) $(build)=$(obj)/rm $@
