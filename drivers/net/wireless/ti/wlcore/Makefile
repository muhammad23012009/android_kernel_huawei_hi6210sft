wlcore-objs		= main.o cmd.o io.o event.o tx.o rx.o ps.o acx.o \
<<<<<<< HEAD
			  boot.o init.o debugfs.o scan.o
=======
			  boot.o init.o debugfs.o scan.o sysfs.o vendor_cmd.o
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

wlcore_spi-objs 	= spi.o
wlcore_sdio-objs	= sdio.o

wlcore-$(CONFIG_NL80211_TESTMODE)	+= testmode.o
obj-$(CONFIG_WLCORE)			+= wlcore.o
obj-$(CONFIG_WLCORE_SPI)		+= wlcore_spi.o
obj-$(CONFIG_WLCORE_SDIO)		+= wlcore_sdio.o

ccflags-y += -D__CHECK_ENDIAN__
