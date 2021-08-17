#
# for Renesas USB
#

obj-$(CONFIG_USB_RENESAS_USBHS)	+= renesas_usbhs.o

<<<<<<< HEAD
renesas_usbhs-y			:= common.o mod.o pipe.o fifo.o
=======
renesas_usbhs-y			:= common.o mod.o pipe.o fifo.o rcar2.o rcar3.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

ifneq ($(CONFIG_USB_RENESAS_USBHS_HCD),)
	renesas_usbhs-y		+= mod_host.o
endif

ifneq ($(CONFIG_USB_RENESAS_USBHS_UDC),)
	renesas_usbhs-y		+= mod_gadget.o
endif
