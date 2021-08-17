#
# Makefile for USB Core files and filesystem
#

<<<<<<< HEAD
ccflags-$(CONFIG_USB_DEBUG) := -DDEBUG

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
usbcore-y := usb.o hub.o hcd.o urb.o message.o driver.o
usbcore-y += config.o file.o buffer.o sysfs.o endpoint.o
usbcore-y += devio.o notify.o generic.o quirks.o devices.o
usbcore-y += port.o

<<<<<<< HEAD
=======
usbcore-$(CONFIG_OF)		+= of.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
usbcore-$(CONFIG_PCI)		+= hcd-pci.o
usbcore-$(CONFIG_ACPI)		+= usb-acpi.o

obj-$(CONFIG_USB)		+= usbcore.o
<<<<<<< HEAD
=======

obj-$(CONFIG_USB_LEDS_TRIGGER_USBPORT)	+= ledtrig-usbport.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
