dtb-y	+= skeleton.dtb
<<<<<<< HEAD

# Built-in dtb
builtindtb-y				:= skeleton
=======
dtb-y	+= tz1090_generic.dtb

# Built-in dtb
builtindtb-y				:= skeleton
builtindtb-$(CONFIG_SOC_TZ1090)		:= tz1090_generic
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

ifneq ($(CONFIG_METAG_BUILTIN_DTB_NAME),"")
	builtindtb-y			:= $(patsubst "%",%,$(CONFIG_METAG_BUILTIN_DTB_NAME))
endif

dtb-$(CONFIG_METAG_BUILTIN_DTB)	+= $(builtindtb-y).dtb
obj-$(CONFIG_METAG_BUILTIN_DTB)	+= $(builtindtb-y).dtb.o

<<<<<<< HEAD
targets	+= dtbs
targets	+= $(dtb-y)

.SECONDARY: $(obj)/$(builtindtb-y).dtb.S

dtbs: $(addprefix $(obj)/, $(dtb-y))

=======
dtstree		:= $(srctree)/$(src)
dtb-$(CONFIG_OF_ALL_DTBS) := $(patsubst $(dtstree)/%.dts,%.dtb, $(wildcard $(dtstree)/*.dts))

.SECONDARY: $(obj)/$(builtindtb-y).dtb.S

always += $(dtb-y)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
clean-files += *.dtb *.dtb.S
