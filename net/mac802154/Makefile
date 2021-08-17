obj-$(CONFIG_MAC802154)	+= mac802154.o
<<<<<<< HEAD
mac802154-objs		:= ieee802154_dev.o rx.o tx.o mac_cmd.o mib.o monitor.o wpan.o
=======
mac802154-objs		:= main.o rx.o tx.o mac_cmd.o mib.o \
			   iface.o llsec.o util.o cfg.o trace.o

CFLAGS_trace.o := -I$(src)

ccflags-y += -D__CHECK_ENDIAN__
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
