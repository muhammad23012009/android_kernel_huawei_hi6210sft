#
# Makefile for ALSA
#

snd-usb-audio-objs := 	card.o \
			clock.o \
			endpoint.o \
			format.o \
			helper.o \
			mixer.o \
			mixer_quirks.o \
<<<<<<< HEAD
=======
			mixer_scarlett.o \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			pcm.o \
			proc.o \
			quirks.o \
			stream.o

snd-usbmidi-lib-objs := midi.o

# Toplevel Module Dependency
obj-$(CONFIG_SND_USB_AUDIO) += snd-usb-audio.o snd-usbmidi-lib.o

obj-$(CONFIG_SND_USB_UA101) += snd-usbmidi-lib.o
obj-$(CONFIG_SND_USB_USX2Y) += snd-usbmidi-lib.o
obj-$(CONFIG_SND_USB_US122L) += snd-usbmidi-lib.o

<<<<<<< HEAD
obj-$(CONFIG_SND) += misc/ usx2y/ caiaq/ 6fire/
=======
obj-$(CONFIG_SND) += misc/ usx2y/ caiaq/ 6fire/ hiface/ bcd2000/
obj-$(CONFIG_SND_USB_LINE6)	+= line6/
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
