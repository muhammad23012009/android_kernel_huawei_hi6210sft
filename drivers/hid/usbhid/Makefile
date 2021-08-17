#
# Makefile for the USB input drivers
#

<<<<<<< HEAD
# Multipart objects.
usbhid-y	:= hid-core.o hid-quirks.o

# Optional parts of multipart objects.

ifeq ($(CONFIG_USB_HIDDEV),y)
	usbhid-y	+= hiddev.o
endif
ifeq ($(CONFIG_HID_PID),y)
	usbhid-y	+= hid-pidff.o
endif
=======
usbhid-y	:= hid-core.o hid-quirks.o
usbhid-$(CONFIG_USB_HIDDEV)	+= hiddev.o
usbhid-$(CONFIG_HID_PID)	+= hid-pidff.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

obj-$(CONFIG_USB_HID)		+= usbhid.o
obj-$(CONFIG_USB_KBD)		+= usbkbd.o
obj-$(CONFIG_USB_MOUSE)		+= usbmouse.o

