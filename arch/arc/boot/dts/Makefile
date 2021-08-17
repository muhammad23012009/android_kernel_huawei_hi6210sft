# Built-in dtb
<<<<<<< HEAD
builtindtb-y		:= angel4
=======
builtindtb-y		:= nsim_700
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

ifneq ($(CONFIG_ARC_BUILTIN_DTB_NAME),"")
	builtindtb-y	:= $(patsubst "%",%,$(CONFIG_ARC_BUILTIN_DTB_NAME))
endif

obj-y   += $(builtindtb-y).dtb.o
<<<<<<< HEAD
targets += $(builtindtb-y).dtb

.SECONDARY: $(obj)/$(builtindtb-y).dtb.S

dtbs:  $(addprefix  $(obj)/, $(builtindtb-y).dtb)

=======
dtb-y := $(builtindtb-y).dtb

.SECONDARY: $(obj)/$(builtindtb-y).dtb.S

dtstree		:= $(srctree)/$(src)
dtb-$(CONFIG_OF_ALL_DTBS) := $(patsubst $(dtstree)/%.dts,%.dtb, $(wildcard $(dtstree)/*.dts))

always := $(dtb-y)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
clean-files := *.dtb  *.dtb.S
