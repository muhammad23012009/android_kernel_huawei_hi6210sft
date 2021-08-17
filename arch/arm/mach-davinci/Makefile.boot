<<<<<<< HEAD
ifeq ($(CONFIG_ARCH_DAVINCI_DA8XX),y)
ifeq ($(CONFIG_ARCH_DAVINCI_DMx),y)
$(error Cannot enable DaVinci and DA8XX platforms concurrently)
else
   zreladdr-y	+= 0xc0008000
params_phys-y	:= 0xc0000100
initrd_phys-y	:= 0xc0800000
endif
else
   zreladdr-y	+= 0x80008000
params_phys-y	:= 0x80000100
initrd_phys-y	:= 0x80800000
endif
=======
zreladdr-$(CONFIG_ARCH_DAVINCI_DA8XX)		+= 0xc0008000
params_phys-$(CONFIG_ARCH_DAVINCI_DA8XX)	:= 0xc0000100
initrd_phys-$(CONFIG_ARCH_DAVINCI_DA8XX)	:= 0xc0800000

zreladdr-$(CONFIG_ARCH_DAVINCI_DMx)		+= 0x80008000
params_phys-$(CONFIG_ARCH_DAVINCI_DMx)		:= 0x80000100
initrd_phys-$(CONFIG_ARCH_DAVINCI_DMx)		:= 0x80800000
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
